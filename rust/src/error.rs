//! Custom error and result types.
//!

/// Custom error enums
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Error {
    /// Invalid arguments of functions/methods.
    InvalidArguments(String),
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::InvalidArguments(msg) => write!(f, "Invalid arguments: {}", msg),
        }
    }
}

impl std::error::Error for Error {}

pub type Result<T> = std::result::Result<T, Error>;
