use crate::evaluation::ERR_DIVIDE_BY_ZERO;
use crate::parser::expression::Expression;
use crate::parser::spanned::Spanned;
use crate::parser::SimpleError;
use ariadne::{Color, Fmt};
use chumsky::error::Simple;

macro_rules! incorrect_num_args {
    ($span: expr, $expected: expr, $actual: expr) => {
        Err(Box::new(Simple::custom(
            $span,
            format!(
                "Incorrect number of arguments, expected {} but got {}",
                $expected, $actual
            ),
        )))
    };
}

type Arguments = Spanned<Vec<Spanned<Expression>>>;
type CallResult = Result<Expression, Box<SimpleError>>;

/// The caller ought to already have evaluated the argument expressions as far as possible.
/// Function names are case insensitive.
///
/// # Errors
/// If one of the arguments passed is not a valid input to the called function, Err will be the result.
/// Otherwise, the result will contain the value returned from the function as an expression.
pub fn evaluate_call(fn_name: Spanned<String>, arguments: Arguments) -> CallResult {
    match fn_name.content.to_lowercase().as_str() {
        "sqrt" => sqrt(arguments),
        "element" => element(arguments),
        "x" => get_element_sugar(arguments, 0),
        "y" => get_element_sugar(arguments, 1),
        "z" => get_element_sugar(arguments, 2),
        "nthroot" => nth_root(arguments),
        _ => Err(Box::new(SimpleError::custom(
            fn_name.span,
            format!("Unknown function '{}'", fn_name.content.fg(Color::Yellow)),
        ))),
    }
}

fn sqrt(args: Arguments) -> CallResult {
    let argument_expressions = args.content;
    let arguments_span = args.span;

    if argument_expressions.len() == 1 {
        let first_arg = &argument_expressions[0];
        let result = first_arg.scalar()?.sqrt();

        Ok(Expression::Scalar(result))
    } else {
        incorrect_num_args!(arguments_span, 1, argument_expressions.len())
    }
}

fn get_element_sugar(args: Arguments, index: usize) -> CallResult {
    let argument_expressions = args.content;
    let arguments_span = args.span;
    let required_args = 1;

    if argument_expressions.len() != required_args {
        return incorrect_num_args!(arguments_span, required_args, argument_expressions.len());
    }

    let vec = argument_expressions[0].vec()?;

    let retrieval_fallback = Spanned::new(Expression::Scalar(0f64), arguments_span);
    let element = vec.get(index).unwrap_or(&retrieval_fallback);

    Ok(element.content.clone())
}

fn element(args: Arguments) -> CallResult {
    let argument_expressions = args.content;
    let arguments_span = args.span;
    let required_args = 2;

    if argument_expressions.len() != required_args {
        return incorrect_num_args!(arguments_span, required_args, argument_expressions.len());
    }
    let vec = argument_expressions[0].vec()?;
    let index = argument_expressions[1].scalar()?;

    if index <= 0f64 {
        return Err(Box::new(Simple::custom(
            argument_expressions[1].span.clone(),
            "Element index must be at least 1",
        )));
    }

    if index.fract() > 0f64 {
        return Err(Box::new(Simple::custom(
            argument_expressions[1].span.clone(),
            "Index must be an integer",
        )));
    }

    let vec_index = index as usize;

    let retrieval_fallback = Spanned::new(Expression::Scalar(0f64), arguments_span);
    let element = vec.get(vec_index - 1).unwrap_or(&retrieval_fallback);

    Ok(element.content.clone())
}

fn nth_root(args: Arguments) -> CallResult {
    let argument_expressions = args.content;
    let arguments_span = args.span;
    let required_args = 2;

    if argument_expressions.len() != required_args {
        return incorrect_num_args!(arguments_span, required_args, argument_expressions.len());
    }

    let value = argument_expressions[0].scalar()?;
    let n = argument_expressions[1].scalar()?;

    if n == 0f64 {
        Err(Box::new(Simple::custom(arguments_span, "Root index cannot be zero")))
    } else {
        let result = value.powf(1f64 / n);

        Ok(Expression::Scalar(result))
    }
}
