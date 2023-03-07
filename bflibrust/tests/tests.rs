
use bfrust::core::string;

#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(string::string_is_numeric("1".to_string()), true);
    }
}
