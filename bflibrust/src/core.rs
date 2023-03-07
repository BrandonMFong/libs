// 
// author: Brando
// date: 3/3/23
//

pub mod string {
    pub fn string_is_numeric(str: String) -> bool {
        for c in str.chars() {
            if !c.is_numeric() {
                return false;
            }
        }
        return true;
    }
}

