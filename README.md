# jsonn

Minimalist and fast JSON library for C++

---

## Overview

`jsonn` is a lightweight and easy-to-use JSON/JSONL library for C++. It provides a simple API while keeping performance and type safety in mind.

---

## Features

* Full JSON support: objects, arrays, strings, numbers, booleans, null.
* Ergonomic API: `operator[]`, `push_back`, `insert`, `try_get_*`.
* Type safety: `is_*` checks and optional-based getters.
* Comparison: `operator==` for easy value comparison.
* Header-only, no dependencies.
* BSD 3-Clause License, permissive for commercial or open-source use.
* Support load from JSONL and save to JSONL

---

## Installation

Simply include the header in your project:

```cpp
#include "jsonn.hpp"
```

You must to install a shared/static lib. Shared library are builded for linux and macos.

No additional dependencies required.

---

## Example Usage

```cpp
#include "jsonn.hpp"
#include <iostream>

int main() {
    jsonn::Value v;

    // Create an object
    v["title"] = "Example Document";
    v["version"] = 1;
    v["tags"] = jsonn::Array{"json", "cpp", "example"};

    // Access values
    std::cout << v["title"].as_string() << "\n";

    // Safe number access
    if (auto version = v["version"].try_get_number()) {
        std::cout << "Version: " << *version << "\n";
    }

    // Append to array
    v["tags"].push_back("library");

    // Iterate over array
    for (const auto& tag : v["tags"].as_array()) {
        std::cout << tag.as_string() << "\n";
    }

    return 0;
}
```

---

## Roadmap

* Full JSON parser and serializer
* Pretty-print formatting
* JSON Pointer support
* Performance optimizations

---

## License

BSD 3-Clause License – see the `LICENSE` file.
