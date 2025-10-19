/*
    jsonn - minimalist C++ library for json
    Copyright (c) 2025 Aleksander Płomiński

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the author nor the names of its contributors may be
       used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#include "jsonn.h"
#include <stdexcept>
#include <sstream>
namespace jsonn {
class Parser {
    const std::string& str;
    size_t pos;

public:
    Parser(const std::string& s) : str(s), pos(0) {}

    void skip_whitespace() {
        while (pos < str.size() && std::isspace(str[pos])) ++pos;
    }

    size_t get_pos() { return pos; } 

    char peek() {
        skip_whitespace();
        if (pos >= str.size()) throw std::runtime_error("Unexpected end of input at position " + std::to_string(pos));
        return str[pos];
    }

    char get() {
        skip_whitespace();
        if (pos >= str.size()) throw std::runtime_error("Unexpected end of input at position " + std::to_string(pos));
        return str[pos++];
    }

    Value parse_value() {
        char c = peek();
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return parse_string();
        if (std::isdigit(c) || c == '-') return parse_number();
        if (c == 't') return parse_true();
        if (c == 'f') return parse_false();
        if (c == 'n') return parse_null();
        throw std::runtime_error("Unexpected character: " + std::string(1, c) + " at position " + std::to_string(pos));
    }

private:
    Value parse_object() {
        get(); // consume '{'
        Object obj;
        skip_whitespace();
        if (peek() == '}') { get(); return obj; }

        while (true) {
            std::string key = parse_string();
            if (get() != ':') throw std::runtime_error("Expected ':' after key at position " + std::to_string(pos));
            Value val = parse_value();
            obj[key] = val;

            char c = get();
            if (c == '}') break;
            if (c != ',') throw std::runtime_error("Expected ',' in object at position " + std::to_string(pos));
        }
        return obj;
    }

    Value parse_array() {
        get(); // consume '['
        Array arr;
        skip_whitespace();
        if (peek() == ']') { get(); return arr; }

        while (true) {
            arr.push_back(parse_value());
            char c = get();
            if (c == ']') break;
            if (c != ',') throw std::runtime_error("Expected ',' in array at position " + std::to_string(pos));
        }
        return arr;
    }

    std::string parse_string() {
        get(); // consume '"'
        std::ostringstream oss;
        while (true) {
            if (pos >= str.size()) throw std::runtime_error("Unterminated string at position " + std::to_string(pos));
            char c = str[pos++];
            if (c == '"') break;
            if (c == '\\') {
                if (pos >= str.size()) throw std::runtime_error("Unterminated escape sequence at position " + std::to_string(pos));
                char esc = str[pos++];
                switch (esc) {
                    case 'n': oss << '\n'; break;
                    case 't': oss << '\t'; break;
                    case 'r': oss << '\r'; break;
                    case 'b': oss << '\b'; break;
                    case 'f': oss << '\f'; break;
                    case '"': oss << '"'; break;
                    case '\\': oss << '\\'; break;
                    case 'u': { // Unicode escape
                        if (pos + 4 > str.size()) throw std::runtime_error("Invalid unicode escape at position " + std::to_string(pos));
                        std::string hex = str.substr(pos, 4);
                        pos += 4;
                        char16_t code = static_cast<char16_t>(std::stoi(hex, nullptr, 16));
                        oss << static_cast<char>(code); // Simple conversion, nie obsługuje surrogate pairs
                        break;
                    }
                    default:
                        throw std::runtime_error("Unknown escape character: \\" + std::string(1, esc) + " at position " + std::to_string(pos));
                }
            } else {
                oss << c;
            }
        }
        return oss.str();
    }

    Value parse_number() {
        size_t start = pos;
        if (str[pos] == '-') ++pos;
        while (pos < str.size() && std::isdigit(str[pos])) ++pos;
        bool is_double = false;
        if (pos < str.size() && str[pos] == '.') {
            is_double = true;
            ++pos;
            while (pos < str.size() && std::isdigit(str[pos])) ++pos;
        }
        if (pos < str.size() && (str[pos] == 'e' || str[pos] == 'E')) {
            is_double = true;
            ++pos;
            if (str[pos] == '+' || str[pos] == '-') ++pos;
            while (pos < str.size() && std::isdigit(str[pos])) ++pos;
        }

        std::string num_str = str.substr(start, pos - start);
        try {
            if (is_double) return std::stod(num_str);
            return std::stoi(num_str);
        } catch (...) {
            throw std::runtime_error("Invalid number: " + num_str + " at position " + std::to_string(start));
        }
    }

    Value parse_true() {
        if (str.substr(pos, 4) != "true") throw std::runtime_error("Invalid literal at position " + std::to_string(pos));
        pos += 4;
        return true;
    }

    Value parse_false() {
        if (str.substr(pos, 5) != "false") throw std::runtime_error("Invalid literal at position " + std::to_string(pos));
        pos += 5;
        return false;
    }

    Value parse_null() {
        if (str.substr(pos, 4) != "null") throw std::runtime_error("Invalid literal at position " + std::to_string(pos));
        pos += 4;
        return nullptr;
    }
};

Value parse(const std::string& json) {
    Parser p(json);
    Value v = p.parse_value();
    p.skip_whitespace();
    if (p.get_pos() != json.size()) throw std::runtime_error("Extra data after JSON at position " + std::to_string(p.get_pos()));
    return v;
}

} // namespace jsonn
