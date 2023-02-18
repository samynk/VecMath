use crate::parser::expression::Expression;
use crate::parser::Spanned;

#[derive(Debug)]
pub enum Statement {
    Variable(Spanned<String>, Box<Spanned<Expression>>),
    Print(Box<Spanned<Expression>>),
    PrintWithSteps(Box<Spanned<Expression>>),
    PrintAll,
    Exit,
    Help,
}
