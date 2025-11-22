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


#pragma once
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <stdexcept>
#include <optional>

namespace jsonn {

struct Value;

using Object = std::map<std::string, Value>;
using Array = std::vector<Value>;
using ValueType = std::variant<std::nullptr_t, bool, int, double, std::string, Array, Object>;

struct Value {
    ValueType data;

    // Constructors
    Value() : data(nullptr) {}
    Value(std::nullptr_t) : data(nullptr) {}
    Value(bool b) : data(b) {}
    Value(int i) : data(i) {}
    Value(double d) : data(d) {}
    Value(const std::string& s) : data(s) {}
    Value(const char* s) : data(std::string(s)) {}
    Value(const Array& a) : data(a) {}
    Value(const Object& o) : data(o) {}
    
    // Type checks
    bool is_object() const { return std::holds_alternative<Object>(data); }
    bool is_array()  const { return std::holds_alternative<Array>(data); }
    bool is_string() const { return std::holds_alternative<std::string>(data); }
    bool is_number() const { return std::holds_alternative<int>(data) || std::holds_alternative<double>(data); }    bool is_bool()   const { return std::holds_alternative<bool>(data); }
    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(data); }

    // Getters
    const Array& as_array() const { return std::get<Array>(data); }
    const Object& as_object() const { return std::get<Object>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    double as_number() const {
        if (std::holds_alternative<int>(data)) return static_cast<double>(std::get<int>(data));
        return std::get<double>(data);
    }
    bool as_bool() const { return std::get<bool>(data); }

    // Safe getters
    std::optional<double> try_get_number() const { return is_number() ? std::make_optional(as_number()) : std::nullopt; }
    std::optional<std::string> try_get_string() const { return is_string() ? std::make_optional(as_string()) : std::nullopt; }
    std::optional<bool> try_get_bool() const { return is_bool() ? std::make_optional(as_bool()) : std::nullopt; }
    std::optional<Array> try_get_array() const { return is_array() ? std::make_optional(as_array()) : std::nullopt; }
    std::optional<Object> try_get_object() const { return is_object() ? std::make_optional(as_object()) : std::nullopt; }
    
        // Operators for objects
    Value& operator[](const std::string& key) {
        if (!is_object()) {
            data = Object{};
        }
        return std::get<Object>(data)[key];
    }

    const Value& operator[](const std::string& key) const {
        if (!is_object()) throw std::runtime_error("Value is not an object");
        return std::get<Object>(data).at(key);
    }

    // Operators for arrays
    Value& operator[](size_t index) {
        if (!is_array()) {
            data = Array{}; 
        }
        auto& arr = std::get<Array>(data);
        if (index >= arr.size()) arr.resize(index + 1);
        return arr[index];
    }

    const Value& operator[](size_t index) const {
        if (!is_array()) throw std::runtime_error("Value is not an array");
        return std::get<Array>(data).at(index);
    }

    // Comparison operator
    bool operator==(const Value& other) const {
        return data == other.data;
    }

    bool operator!=(const Value& other) const {
        return !(*this == other);
    }
};

__attribute__((visibility("default"))) std::string serialize(const Value& v);
__attribute__((visibility("default"))) Value parse(const std::string& json);

__attribute__((visibility("default"))) std::string serialize_jsonl(const std::vector<Value&> v);
__attribute__((visibility("default"))) std::vector<Value> parse_jsonl(const std::string& json);

} // namespace jsonn
