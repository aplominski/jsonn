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

namespace jsonn {
__attribute__((visibility("default"))) 
std::vector<Value> parse_jsonl(const std::string& jsonl) {
    std::vector<std::string> lines_tmp;
    std::stringstream ss(jsonl);
    std::string line_tmp;

    while (std::getline(ss, line_tmp)) {
        if (!line_tmp.empty() && line_tmp.back() == '\r') {
            line_tmp.pop_back();
        }
        if (!line_tmp.empty()) {
            lines_tmp.push_back(line_tmp);
        }
    }

    std::vector<Value> result_values(lines_tmp.size());
    std::mutex exception_mutex;
    std::exception_ptr first_exception = nullptr;

    auto worker = [&](size_t start, size_t end) {
        try {
            for (size_t i = start; i < end; ++i) {
                result_values[i] = jsonn::parse(lines_tmp[i]);
            }
        } catch (...) {
            std::lock_guard<std::mutex> lock(exception_mutex);
            if (!first_exception) first_exception = std::current_exception();
        }
    };

    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    size_t block_size = (lines_tmp.size() + num_threads - 1) / num_threads;

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * block_size;
        size_t end = std::min(start + block_size, lines_tmp.size());
        if (start < end) {
            threads.emplace_back(worker, start, end);
        }
    }

    for (auto& th : threads) {
        th.join();
    }

    if (first_exception) {
        std::rethrow_exception(first_exception);
    }

    return result_values;
}
}
