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
    bool is_number() const { return std::holds_alternative<double>(data); }
    bool is_bool()   const { return std::holds_alternative<bool>(data); }
    bool is_null()   const { return std::holds_alternative<std::nullptr_t>(data); }

    // Getters
    const Array& as_array() const { return std::get<Array>(data); }
    const Object& as_object() const { return std::get<Object>(data); }
    const std::string& as_string() const { return std::get<std::string>(data); }
    double as_number() const { return std::get<double>(data); }
    bool as_bool() const { return std::get<bool>(data); }

    // Safe getters
    std::optional<double> try_get_number() const { return is_number() ? std::make_optional(as_number()) : std::nullopt; }
    std::optional<std::string> try_get_string() const { return is_string() ? std::make_optional(as_string()) : std::nullopt; }
    std::optional<bool> try_get_bool() const { return is_bool() ? std::make_optional(as_bool()) : std::nullopt; }

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

} // namespace jsonn
