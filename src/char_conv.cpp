#include "../include/char_conv.h"

std::string char32_to_utf8(const char32_t c){
    std::string s;
    char c1, c2, c3, c4;
    char32_t temp = c;
    switch(c){
        case 0x0000'0000 ... 0x0000'007f:
            s += static_cast<char>(c);
            break;

        case 0x0000'0080 ... 0x0000'07ff:
            c1 = 0b110'0'0000 | (temp >> 6);
            c2 = 0b10'00'0000 | (temp & 0b111'111);
            s += c1; s += c2;
            break;

        case 0x0000'0800 ... 0x0000'ffff:
            c3 = 0b10'00'0000 | (temp & 0b111'111);
            temp >>= 6;
            c2 = 0b10'00'0000 | (temp & 0b111'111);
            temp >>= 6;
            c1 = 0b1110'0000 | c;
            s += c1; s += c2; s += c3;
            break;

        case 0x0001'0000 ... 0x001f'ffff:
            c4 = 0b10'00'0000 | (temp & 0b111'111);
            temp >>= 6;
            c3 = 0b10'00'0000 | (temp & 0b111'111);
            temp >>= 6;
            c2 = 0b10'00'0000 | (temp & 0b111'111);
            temp >>= 6;
            c1 = 0b11110'000 | c;
            s += c1; s += c2; s += c3; s += c4;
            break;

        default:
            ;
    }
    return s;
}

std::string u32string_to_utf8(const std::u32string& u32str){
    std::string s;
    for(const char32_t c : u32str){
        s += char32_to_utf8(c);
    }
    return s;
}

std::u32string utf8_to_u32string(const char* utf8str){
    std::u32string s;
    enum class State{
        Start_state,                 Three_byte_char_second_byte,
        Four_byte_char_second_byte,  Four_byte_char_third_byte,
        Last_byte_of_char
    };
    State state = State::Start_state;
    char32_t current_char = 0;
    while(char c = *utf8str++){
        switch(state){
            case State::Start_state:
                if(c >= 0){
                    s += c;
                }else if((c & 0b1110'0000) == 0b1100'0000){
                    current_char = c & 0b0001'1111;
                    state = State::Last_byte_of_char;
                }else if((c & 0b1111'0000) == 0b1110'0000){
                    current_char = c & 0b0000'1111;
                    state = State::Three_byte_char_second_byte;
                }else if((c & 0b1111'1000) == 0b1111'0000){
                    current_char = c & 0b0000'0111;
                    state = State::Four_byte_char_second_byte;
                }
                break;

            case State::Last_byte_of_char:
                current_char = (current_char << 6) | (c & 0b0011'1111);
                s += current_char;
                state = State::Start_state;
                break;

            case State::Three_byte_char_second_byte:
                current_char = (current_char << 6) | (c & 0b0011'1111);
                state = State::Last_byte_of_char;
                break;

            case State::Four_byte_char_second_byte:
                current_char = (current_char << 6) | (c & 0b0011'1111);
                state = State::Four_byte_char_third_byte;
                break;

            case State::Four_byte_char_third_byte:
                current_char = (current_char << 6) | (c & 0b0011'1111);
                state = State::Last_byte_of_char;
                break;
        }
    }
    return s;
}
