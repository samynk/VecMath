pub mod expression;
pub mod spanned;
pub mod statement;

use crate::parser::{expression::Expression, spanned::Spanned, statement::Statement};
use chumsky::{
    error::Simple,
    prelude::{end, just, recursive},
    text,
    text::TextParser,
    Parser,
};
use std::ops::Range;

const CMD_EXIT: &str = "exit";
const CMD_PRINT: &str = "print";
const CMD_PRINT_WITH_STEPS: &str = "print_steps";

pub type SimpleError = Simple<char>;

pub fn parser() -> impl Parser<char, Statement, Error = SimpleError> + Clone {
    let ident = text::ident().padded().map_with_span(Spanned::<String>::new);

    let operator = |character| just(character).padded();

    let expression = recursive::<_, Spanned<Expression>, _, _, SimpleError>(|expr| {
        let scalar = text::int(10)
            .chain::<char, _, _>(just('.').chain(text::digits(10)).or_not())
            .collect::<String>()
            .try_map(
                |num_as_string, span: _| match num_as_string.parse::<f64>() {
                    Ok(num) => Ok(Expression::Scalar(num)),
                    Err(_) => Err(Simple::custom(span, "Couldn't parse as a 64-bit float")),
                },
            )
            .map_with_span(Spanned::new)
            .padded();

        let fn_call = ident
            .then(
                expr.clone()
                    .separated_by(just(',').padded())
                    .delimited_by(just('('), just(')'))
                    .map_with_span(Spanned::new),
            )
            .map_with_span(|(fn_name, arguments), span| {
                Spanned::new(Expression::FnCall(fn_name, arguments), span)
            })
            .padded();

        let var_reference = ident
            .map_with_span(|name, span| Spanned::new(Expression::VariableReference(name), span));

        let vector = expr
            .clone()
            .separated_by(just(',').padded())
            .delimited_by(just('['), just(']'))
            .map_with_span(|expr, span| Spanned::new(Expression::Vec(expr), span))
            .padded();

        let value = scalar.or(fn_call).or(var_reference).or(vector);

        let magnitude = expr
            .clone()
            .delimited_by(just('|'), just('|'))
            .map_with_span(|expr, span| Spanned::new(Expression::Magnitude(Box::new(expr)), span));

        let brackets = expr
            .clone()
            .delimited_by(just('('), just(')'))
            .map_with_span(|expr, span| {
                let inner_expression = Box::new(Spanned::new(expr.content, expr.span));

                Spanned::new(Expression::Brackets(inner_expression), span)
            })
            .or(magnitude)
            .or(value);

        let negative = just('-')
            .or_not()
            .then(brackets)
            .map_with_span(|(neg, expr), span| {
                if neg.is_some() {
                    Spanned::new(Expression::Negative(Box::new(expr)), span)
                } else {
                    expr
                }
            });

        let products_and_divisions = negative
            .clone()
            .then(
                operator('*')
                    .to(Expression::Multiplication as fn(_, _) -> _)
                    .or(operator('/').to(Expression::Division as fn(_, _) -> _))
                    .then(negative)
                    .repeated(),
            )
            .foldl(|lhs, (op, rhs)| {
                let span: Range<usize> = lhs.span.start..rhs.span.end;
                Spanned::new(op(Box::new(lhs), Box::new(rhs)), span)
            });

        let addition = products_and_divisions
            .clone()
            .then(
                operator('+')
                    .to(Expression::Addition as fn(_, _) -> _)
                    .or(operator('-').to(Expression::Subtraction as fn(_, _) -> _))
                    .then(products_and_divisions)
                    .repeated(),
            )
            .foldl(|lhs, (op, rhs)| {
                let span: Range<usize> = lhs.span.start..rhs.span.end;
                Spanned::new(op(Box::new(lhs), Box::new(rhs)), span)
            });

        addition.padded()
    });

    let exit = text::keyword(CMD_EXIT).map(|_| Statement::Exit);
    let print = text::keyword(CMD_PRINT)
        .ignore_then(expression.clone())
        .map(|expr| Statement::Print(Box::new(expr)));
    let print_with_steps = text::keyword(CMD_PRINT_WITH_STEPS)
        .ignore_then(expression.clone())
        .map(|expr| Statement::PrintWithSteps(Box::new(expr)));
    let var_declaration = ident
        .then_ignore(operator('='))
        .then(expression)
        .map(|(name, value)| Statement::Variable(name, Box::new(value)));

    exit.or(print.clone())
        .or(print_with_steps.clone())
        .or(var_declaration.clone())
        .padded()
        .then_ignore(end())
}
