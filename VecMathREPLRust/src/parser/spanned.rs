use std::ops::Range;

#[derive(Debug, PartialEq, Clone)]
pub struct Spanned<T: Clone> {
    pub content: T,
    pub span: Range<usize>,
}

impl<T: Clone> Spanned<T> {
    pub fn new(content: T, span: Range<usize>) -> Self {
        Self { content, span }
    }
}
