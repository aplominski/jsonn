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
#include <exception>
#include <sstream>
#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>

namespace jsonn {
   __attribute__((visibility("default"))) std::string serialize_jsonl(const std::vector<Value>& v) {
      std::vector<std::string> lines;

      for (size_t i = 0; i < v.size(); i++) {
         std::string s = jsonn::serialize(v[i]);

         s.erase(
         std::remove_if(s.begin(), s.end(),
                           [](unsigned char c) { return std::isspace(c); }),
          s.end()
        );

        lines.push_back(std::move(s));
    }

    std::ostringstream oss;
    for (const auto& line : lines) {
        oss << line << '\n';
    }

    return oss.str();
}
}