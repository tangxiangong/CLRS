//! The utilities functions.
//!

use crate::{Error, Result};
use num_traits::Float;
use rand::{Rng, rng};
use rand_distr::{Distribution, StandardNormal};

/// Generate a vector of normal random numbers
///
/// # Arguments
///
/// * `mean` - The mean of the normal distribution.
/// * `std_dev` - The standard deviation of the normal distribution, must be greater than 0.
/// * `n` - The number of random numbers to generate, must be greater than 0.
///
/// # Example
///
/// ```rust
/// use crate::utils::randns;
///
/// let randoms = randns(0.0, 1.0, 10).unwrap();
/// ```
pub fn randn<T: Float + Send + Sync>(mean: T, std_dev: T, n: usize) -> Result<Vec<T>>
where
    StandardNormal: Distribution<T>,
{
    if std_dev <= T::zero() {
        return Err(Error::InvalidArguments(
            "std_dev must be greater than 0".to_string(),
        ));
    }
    let normal = rand_distr::Normal::new(mean, std_dev)
        .map_err(|e| Error::InvalidArguments(e.to_string()))?;
    Ok((0..n).map(|_| rng().sample(normal)).collect())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_randn() {
        let rnds = randn(0.0, 1.0, 10).unwrap();
        assert_eq!(rnds.len(), 10);
        println!("{:?}", rnds);
    }
}
