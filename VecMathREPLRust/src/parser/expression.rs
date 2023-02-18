use crate::parser::{SimpleError, Spanned};
use chumsky::error::Simple;
use std::fmt::{Display, Formatter};

type ExpressionChild = Box<Spanned<Expression>>;
type ExpressionChildren = Vec<Spanned<Expression>>;
type ExpressionIdentifier = Spanned<String>;

#[derive(Debug, Clone)]
pub enum Expression {
    // Value types
    Scalar(f64),
    Vec(ExpressionChildren),
    Complex(ExpressionChild, ExpressionChild),
    Quaternion(
        ExpressionChild,
        ExpressionChild,
        ExpressionChild,
        ExpressionChild,
    ),

    // Miscellaneous
    Addition(ExpressionChild, ExpressionChild),
    Subtraction(ExpressionChild, ExpressionChild),
    Multiplication(ExpressionChild, ExpressionChild),
    Division(ExpressionChild, ExpressionChild),
    Negative(ExpressionChild),
    Magnitude(ExpressionChild),
    Brackets(ExpressionChild),
    VariableReference(ExpressionIdentifier),
    FnCall(ExpressionIdentifier, Spanned<ExpressionChildren>),
}

impl Expression {
    pub fn value_type_name(&self) -> String {
        match self {
            Self::Scalar(_) => "scalar",
            Self::Vec(_) => "vector",
            Expression::Complex(_, _) => "complex",
            Expression::Quaternion(_, _, _, _) => "quaternion",
            Expression::VariableReference(_) => "variable reference",
            _ => panic!("Type {:?} is not a value type", self),
        }
        .to_string()
    }
}

impl Spanned<Expression> {
    pub fn scalar(self) -> Result<f64, Box<SimpleError>> {
        match self.content {
            Expression::Scalar(value) => Ok(value),
            expr => Err(Box::new(Simple::custom(
                self.span,
                format!(
                    "Expected this expression to be a scalar but it was of type '{}'",
                    expr.value_type_name()
                ),
            ))),
        }
    }

    pub fn vec(self) -> Result<ExpressionChildren, Box<SimpleError>> {
        match self.content {
            Expression::Vec(children) => Ok(children),
            expr => Err(Box::new(Simple::custom(
                self.span.clone(),
                format!(
                    "Expected this expression to be a vector but it was of type '{}'",
                    expr.value_type_name()
                ),
            ))),
        }
    }
}

impl Display for Expression {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Scalar(value) => write!(f, "{}", value),
            Self::Vec(vec) => {
                let formatted = vec
                    .iter()
                    .map(|spanned_expression| spanned_expression.content.to_string())
                    .collect::<Vec<_>>()
                    .join(", ");

                write!(f, "[{}]", formatted)
            }
            Self::Addition(lhs, rhs) => {
                write!(f, "{} + {}", lhs.content, rhs.content)
            }
            Self::Subtraction(lhs, rhs) => {
                write!(f, "{} - {}", lhs.content, rhs.content)
            }
            Self::Multiplication(lhs, rhs) => {
                write!(f, "{} * {}", lhs.content, rhs.content)
            }
            Self::Division(lhs, rhs) => {
                write!(f, "{} / {}", lhs.content, rhs.content)
            }
            Self::Brackets(inner_expression) => {
                write!(f, "({})", inner_expression.content)
            }
            Self::FnCall(fn_name, arguments) => {
                write!(
                    f,
                    "{}({})",
                    fn_name.content,
                    arguments
                        .content
                        .iter()
                        .map(|spanned| spanned.content.to_string())
                        .collect::<Vec<_>>()
                        .join(", ")
                )
            }
            Self::Negative(inner_expression) => {
                write!(f, "-{}", inner_expression.content)
            }
            Expression::Complex(_, _) => todo!(),
            Expression::Quaternion(_, _, _, _) => todo!(),
            Expression::VariableReference(name) => {
                write!(f, "{}", name.content)
            }
            Expression::Magnitude(expr) => {
                write!(f, "|{}|", expr.content)
            }
        }
    }
}