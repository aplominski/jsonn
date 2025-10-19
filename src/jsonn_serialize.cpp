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
