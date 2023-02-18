mod functions;

use crate::evaluation::functions::evaluate_call;
use crate::parser::expression::Expression;
use crate::parser::spanned::Spanned;
use crate::parser::ParseError;
use ariadne::{Color, Fmt};
use chumsky::error::Simple;
use std::collections::HashMap;
use std::ops;
use text_trees::StringTreeNode;

impl ops::Add<Spanned<Expression>> for Spanned<Expression> {
    type Output = Result<Spanned<Expression>, ParseError>;

    fn add(self, rhs: Spanned<Expression>) -> Self::Output {
        let span = self.span.start..rhs.span.end;

        match (&self.content, &rhs.content) {
            (Expression::Scalar(value), Expression::Scalar(rhs_value)) => {
                Ok(Spanned::new(Expression::Scalar(value + rhs_value), span))
            }
            (Expression::Vec(vec_lhs), Expression::Vec(vec_rhs)) => {
                let result = vec_lhs
                    .iter()
                    .zip(vec_rhs.iter())
                    .map::<Result<_, _>, _>(|(lhs_element, rhs_element)| {
                        lhs_element.clone() + rhs_element.clone()
                    })
                    .collect::<Result<Vec<_>, _>>()?;

                Ok(Spanned::new(Expression::Vec(result), span))
            }
            (lhs, rhs) => Err(Simple::custom(
                span,
                format!(
                    "Cannot add type '{}' and type '{}'",
                    lhs.value_type_name(),
                    rhs.value_type_name()
                ),
            )),
        }
    }
}

impl ops::Sub<Spanned<Expression>> for Spanned<Expression> {
    type Output = Result<Spanned<Expression>, ParseError>;

    fn sub(self, rhs: Spanned<Expression>) -> Self::Output {
        let span = self.span.start..rhs.span.end;

        match (&self.content, &rhs.content) {
            (Expression::Scalar(value), Expression::Scalar(rhs_value)) => {
                Ok(Spanned::new(Expression::Scalar(value - rhs_value), span))
            }
            (lhs, rhs) => Err(Simple::custom(
                span,
                format!(
                    "Cannot subtract type '{}' from type '{}'",
                    lhs.value_type_name(),
                    rhs.value_type_name()
                ),
            )),
        }
    }
}

impl ops::Mul<Spanned<Expression>> for Spanned<Expression> {
    type Output = Result<Spanned<Expression>, ParseError>;

    fn mul(self, rhs: Spanned<Expression>) -> Self::Output {
        let span = self.span.start..rhs.span.end;

        match (&self.content, &rhs.content) {
            (Expression::Scalar(value), Expression::Scalar(rhs_value)) => {
                Ok(Spanned::new(Expression::Scalar(value * rhs_value), span))
            }
            (Expression::Scalar(_), Expression::Vec(vec)) => {
                let result = vec
                    .iter()
                    .map::<Result<_, _>, _>(|expr| self.clone() * expr.clone())
                    .collect::<Result<_, _>>()?;

                Ok(Spanned::new(Expression::Vec(result), span))
            }
            (Expression::Vec(_), Expression::Scalar(_)) => rhs * self,
            (lhs, rhs) => Err(Simple::custom(
                span,
                format!(
                    "Cannot multiply type '{}' with type '{}'",
                    lhs.value_type_name(),
                    rhs.value_type_name()
                ),
            )),
        }
    }
}

pub const ERR_DIVIDE_BY_ZERO: &str = "Cannot divide by 0";

impl ops::Div<Spanned<Expression>> for Spanned<Expression> {
    type Output = Result<Spanned<Expression>, ParseError>;

    fn div(self, rhs: Spanned<Expression>) -> Self::Output {
        let span = self.span.start..rhs.span.end;

        match (&self.content, &rhs.content) {
            (Expression::Scalar(value), Expression::Scalar(rhs_value)) => {
                if rhs_value == &0f64 {
                    return Err(Simple::custom(span, ERR_DIVIDE_BY_ZERO));
                }

                Ok(Spanned::new(Expression::Scalar(value / rhs_value), span))
            }
            (Expression::Vec(vec), Expression::Scalar(scalar_value)) => {
                if scalar_value == &0f64 {
                    return Err(Simple::custom(span, ERR_DIVIDE_BY_ZERO));
                }

                let result = vec
                    .iter()
                    .map::<Result<_, _>, _>(|expr| expr.clone() / rhs.clone())
                    .collect::<Result<_, _>>()?;

                Ok(Spanned::new(Expression::Vec(result), span))
            }
            (lhs, rhs) => Err(Simple::custom(
                span,
                format!(
                    "Cannot divide type '{}' by type '{}'",
                    lhs.value_type_name(),
                    rhs.value_type_name()
                ),
            )),
        }
    }
}

impl ops::Neg for Spanned<Expression> {
    type Output = Result<Spanned<Expression>, ParseError>;

    fn neg(self) -> Self::Output {
        match &self.content {
            Expression::Scalar(value) => Ok(Spanned::new(Expression::Scalar(-*value), self.span)),
            expr => Err(Simple::custom(
                self.span,
                format!(
                    "Cannot negate type '{}'",
                    expr.value_type_name()
                ),
            ))
        }
    }
}

const VALUE_COLOR: Color = Color::Green;
const OPERATION_RESULT_COLOR: Color = Color::Cyan;

impl Spanned<Expression> {
    pub fn evaluate(
        &self,
        variables: &mut HashMap<String, Spanned<Expression>>,
    ) -> Result<(Spanned<Expression>, StringTreeNode), Box<ParseError>> {
        let span = self.span.clone();

        match &self.content {
            Expression::Addition(lhs, rhs) => {
                let (lhs_value, lhs_node) = lhs.evaluate(variables)?;
                let (rhs_value, rhs_node) = rhs.evaluate(variables)?;

                let result = (lhs_value + rhs_value)?;
                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content,
                        result.content.clone().fg(OPERATION_RESULT_COLOR)
                    ),
                    vec![lhs_node, rhs_node].into_iter(),
                );

                Ok((result, node))
            }
            Expression::Subtraction(lhs, rhs) => {
                let (lhs_value, lhs_node) = lhs.evaluate(variables)?;
                let (rhs_value, rhs_node) = rhs.evaluate(variables)?;

                let result = (lhs_value - rhs_value)?;
                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content,
                        result.content.clone().fg(OPERATION_RESULT_COLOR)
                    ),
                    vec![lhs_node, rhs_node].into_iter(),
                );

                Ok((result, node))
            }
            Expression::Multiplication(lhs, rhs) => {
                let (lhs_value, lhs_node) = lhs.evaluate(variables)?;
                let (rhs_value, rhs_node) = rhs.evaluate(variables)?;

                let result = (lhs_value * rhs_value)?;
                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content,
                        result.content.clone().fg(OPERATION_RESULT_COLOR)
                    ),
                    vec![lhs_node, rhs_node].into_iter(),
                );

                Ok((result, node))
            }
            Expression::Division(lhs, rhs) => {
                let (lhs_value, lhs_node) = lhs.evaluate(variables)?;
                let (rhs_value, rhs_node) = rhs.evaluate(variables)?;

                let result = (lhs_value / rhs_value)?;
                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content,
                        result.content.clone().fg(OPERATION_RESULT_COLOR)
                    ),
                    vec![lhs_node, rhs_node].into_iter(),
                );

                Ok((result, node))
            }
            Expression::Vec(expressions) => {
                let mut evaluated_expressions = vec![];
                let mut nodes = vec![];

                for expression in expressions {
                    let (expr, node) = expression.evaluate(variables)?;

                    evaluated_expressions.push(expr);
                    nodes.push(node);
                }

                let resulting_expression = Expression::Vec(evaluated_expressions);
                let result = Spanned::new(resulting_expression, span);
                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content.clone().fg(VALUE_COLOR),
                        result.content.to_string().fg(OPERATION_RESULT_COLOR)
                    ),
                    nodes.into_iter(),
                );

                Ok((result, node))
            }
            Expression::Scalar(value) => {
                let node = StringTreeNode::new(value.fg(VALUE_COLOR).to_string());
                let spanned_expression = Spanned::new(Expression::Scalar(*value), span);

                Ok((spanned_expression, node))
            }
            Expression::Brackets(inner_expression) => {
                let spanned_expression = Spanned::new(*inner_expression.clone().content, span);
                let (resulting_expression, inner_node) = spanned_expression.evaluate(variables)?;
                let node = StringTreeNode::with_child_nodes(
                    self.content.to_string(),
                    vec![inner_node].into_iter(),
                );

                Ok((resulting_expression, node))
            }
            Expression::FnCall(fn_name, arguments) => {
                let mut resulting_expressions = vec![];
                let mut nodes = vec![];

                for argument in &arguments.content {
                    let (expr, node) = argument.evaluate(variables)?;

                    resulting_expressions.push(expr);
                    nodes.push(node);
                }

                let spanned_evaluated = Spanned::new(resulting_expressions, arguments.span.clone());
                let result = evaluate_call(fn_name.clone(), spanned_evaluated)?;
                let spanned_result = Spanned::new(result, span);

                let node = StringTreeNode::with_child_nodes(
                    format!(
                        "{} = {}",
                        self.content,
                        spanned_result
                            .content
                            .to_string()
                            .fg(OPERATION_RESULT_COLOR)
                    ),
                    nodes.into_iter(),
                );

                Ok((spanned_result, node))
            }
            Expression::VariableReference(name) => {
                if let Some(value) = variables.get(&name.content) {
                    let (contained_expr, _) = value.clone().evaluate(variables)?;

                    let node = StringTreeNode::new(format!(
                        "{} = {}",
                        name.content.to_string().fg(Color::Magenta),
                        contained_expr
                            .content
                            .to_string()
                            .fg(OPERATION_RESULT_COLOR)
                    ));

                    Ok((contained_expr, node))
                } else {
                    Err(Simple::custom(
                        name.span.clone(),
                        format!("No variable by the name '{}'", name.content),
                    ))
                }
            }
            Expression::Negative(expr) => {
                let (inner_evaluated, inner_nodes) = expr.evaluate(variables)?;
                let result = (-inner_evaluated)?;

                let node = StringTreeNode::with_child_nodes(format!(
                    "-{} = {}",
                    expr.content,
                    result
                        .content
                        .to_string()
                        .fg(OPERATION_RESULT_COLOR)
                ), vec![inner_nodes].into_iter());

                Ok((result, node))
            }
            _ => todo!(),
        }
        .map_err(Box::new)
    }
}
