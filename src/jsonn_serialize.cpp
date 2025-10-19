#include "jsonn.h"
#include <sstream>

namespace jsonn {

// Forward declarations
std::string serialize(const Array& a);
std::string serialize(const Object& o);

// Serialize Array
std::string serialize(const Array& a) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < a.size(); ++i) {
        oss << serialize(a[i]);
        if (i + 1 < a.size()) oss << ",";
    }
    oss << "]";
    return oss.str();
}

// Serialize Object
std::string serialize(const Object& o) {
    std::ostringstream oss;
    oss << "{";
    size_t count = 0;
    for (const auto& [key, val] : o) {
        oss << "\"" << key << "\":" << serialize(val);
        if (++count < o.size()) oss << ",";
    }
    oss << "}";
    return oss.str();
}

// Serialize Value
std::string serialize(const Value& v) {
    if (v.is_null()) return "null";
    if (v.is_bool()) return v.as_bool() ? "true" : "false";
    if (v.is_number()) return std::to_string(v.as_number());
    if (v.is_string()) return "\"" + v.as_string() + "\"";
    if (v.is_array()) return serialize(v.as_array());
    if (v.is_object()) return serialize(v.as_object());
    return "null";
}

} // namespace jsonn
