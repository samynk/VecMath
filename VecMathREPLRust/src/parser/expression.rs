use crate::parser::Spanned;
use std::fmt::{Display, Formatter};

#[derive(Debug, Clone)]
pub enum Expression {
    Addition(Box<Spanned<Expression>>, Box<Spanned<Expression>>),
    Subtraction(Box<Spanned<Expression>>, Box<Spanned<Expression>>),
    Multiplication(Box<Spanned<Expression>>, Box<Spanned<Expression>>),
    Division(Box<Spanned<Expression>>, Box<Spanned<Expression>>),

    Negative(Box<Spanned<Expression>>),
    Scalar(f64),
    Vec(Vec<Spanned<Expression>>),
    Brackets(Spanned<Box<Expression>>),
    Complex(Box<Spanned<Expression>>, Box<Spanned<Expression>>),
    Quaternion(
        Box<Spanned<Expression>>,
        Box<Spanned<Expression>>,
        Box<Spanned<Expression>>,
        Box<Spanned<Expression>>,
    ),
    VariableReference(Spanned<String>),
    FnCall(Spanned<String>, Spanned<Vec<Spanned<Expression>>>),
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
        }
    }
}
