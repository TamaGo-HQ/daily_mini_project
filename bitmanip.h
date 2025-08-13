

int SET_BIT(value, position){
    return value | (1 << position);
}
//---------------------------------//
// b | 1 == 1 -> bit is set to one
//--------------------------------//

int CLEAR_BIT(value, position){
    return value & ~(1 << position);
}
//--------------------------------//
// CLEAR_BIT(101001,4)
//  101001 
//& 110111 (== ~001000 == ~(1 << 4))
//= 100001 -> 4th bit is reset
//--------------------------------//

int TOGGLE_BIT(value, position){
    return value ^ (1 << position);
}
//--------------------------------//
// b^1 = !b -> b XOR 1 toggles b
// b^0 = b
//--------------------------------//

int CHECK_BIT(value, position){
    return value & (1 << position);
}
// returns non-zero if the bit is set

