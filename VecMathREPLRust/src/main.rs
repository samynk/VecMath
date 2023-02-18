use crate::parser::expression::Expression;
use crate::parser::spanned::Spanned;
use crate::parser::statement::Statement;
use crate::parser::ParseError;
use ariadne::{Color, Fmt, Label, Report, ReportKind, Source};
use chumsky::error::SimpleReason;
use chumsky::Parser;
use rustyline::error::ReadlineError;
use rustyline::Editor;
use std::collections::HashMap;
use std::process::exit;
use text_trees::{FormatCharacters, TreeFormatting};

mod evaluation;
mod parser;

const PROMPT_PREFIX: &str = "vecmath> ";
const SRC_ID: &str = "stdin";

fn handle_errors(input: String, errors: &Vec<ParseError>) {
    for error in errors {
        let report = Report::build::<&str>(ReportKind::Error, SRC_ID, error.span().start);

        let report = match error.reason() {
            SimpleReason::Unexpected => report
                .with_message(
                    (if error.found().is_some() {
                        "Unexpected token"
                    } else {
                        "Unexpected end of input"
                    })
                    .to_string(),
                )
                .with_label(
                    Label::new((SRC_ID, error.span())).with_message(
                        format!(
                            "Expected this to be {}.",
                            if error.expected().next().is_none() {
                                "something else".to_string()
                            } else {
                                error
                                    .expected()
                                    .map(|expected| match expected {
                                        Some(character) => format!("'{}'", *character),
                                        None => "end of input".to_string(),
                                    })
                                    .collect::<Vec<_>>()
                                    .join(", ")
                            }
                            .fg(Color::Yellow)
                        )
                        .fg(Color::Red),
                    ),
                ),

            SimpleReason::Unclosed { delimiter, .. } => report
                .with_message(format!("Unclosed delimiter {}", delimiter.fg(Color::Red)))
                .with_label(Label::new((SRC_ID, error.span())).with_message(format!(
                    "Instead, {} was encountered.",
                    error
                        .found()
                        .map(|found| String::from(*found))
                        .unwrap_or_else(|| "EOF".to_string())
                        .fg(Color::Red)
                ))),

            SimpleReason::Custom(message) => report.with_message(message).with_label(
                Label::new((SRC_ID, error.span()))
                    .with_message(format!("{}", message.fg(Color::Red)))
                    .with_color(Color::Yellow),
            ),
        };

        report
            .finish()
            .eprint(("stdin", Source::from(&input)))
            .unwrap();
    }
}

pub type Variables = HashMap<String, Spanned<Expression>>;

fn handle_input(input: String, variables: &mut Variables) -> Result<(), Vec<ParseError>> {
    let parser = parser::parser();

    let (statement, mut errors) = parser.parse_recovery(input.clone());

    if let Some(statement) = statement {
        let error = match statement {
            Statement::Variable(name, value) => value.evaluate(variables).map(|(evaluated, _)| {
                if let Some(variable) = variables.get_mut(&name.content) {
                    *variable = evaluated;
                } else {
                    variables.insert(name.content, evaluated);
                };
            }),
            Statement::PrintWithSteps(expression) => {
                expression.evaluate(variables).map(|(expression, tree)| {
                    let tree_result = tree
                        .to_string_with_format(&TreeFormatting::dir_tree(
                            FormatCharacters::box_chars(),
                        ))
                        .unwrap_or("Failed to render node tree".to_string());

                    println!("{}", tree_result);

                    println!("{}", expression.content);
                })
            }
            Statement::Print(expression) => expression
                .evaluate(variables)
                .map(|(expression, _)| println!("{}", expression.content)),
            Statement::PrintAll => todo!(),
            Statement::Exit => exit(0),
            Statement::Help => todo!(),
        };

        if let Err(err) = error {
            errors.push(*err)
        }
    }

    if errors.is_empty() {
        Ok(())
    } else {
        handle_errors(input, &errors);

        Err(errors)
    }
}

fn main() -> rustyline::Result<()> {
    let args_input = std::env::args().collect::<Vec<_>>()[1..].join(" ");
    let mut variables = HashMap::new();

    if !args_input.is_empty() {
        #[allow(clippy::bool_to_int_with_if)]
        let exit_code = if handle_input(args_input, &mut variables).is_err() {
            1
        } else {
            0
        };

        exit(exit_code);
    }

    let mut editor = Editor::<()>::new()?;

    loop {
        let read_line = editor.readline(PROMPT_PREFIX);

        match read_line {
            Ok(line) => {
                editor.add_history_entry(line.as_str());
                let _ = handle_input(line, &mut variables);
            }
            Err(ReadlineError::Interrupted) => {
                println!("CTRL-C");
                break;
            }
            Err(ReadlineError::Eof) => {
                println!("CTRL-D");
                break;
            }
            Err(err) => {
                println!("Error: {:?}", err);
                break;
            }
        }
    }

    Ok(())
}
