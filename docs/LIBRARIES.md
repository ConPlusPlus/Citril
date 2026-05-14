# Dynamic Citril Libraries

Citril libraries are designed to be simple for new users.

## Naming rule (easy)

Every library ends with `Lib`.

Examples:

- `printLib`
- `typesLib`
- `stringsLib`
- `timeLib`

In Citril files:

```citril
include "printLib"
include "stringsLib"
```

> Tip: if you write `include "print"`, Citril automatically tries `printLib` too.

## Two ways to load libraries

1. Put library files in `libraries/` (auto-loaded by CLI).
2. Use explicit `--lib /path/to/file`.

## Fastest way to add a new library

### 1) Create a source file

Create `src/math_library.cpp`:

```cpp
#include "citril/interpreter.hpp"
#include "citril/value.hpp"

// Called automatically when the library is loaded.
extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    // Adds: double(number)
    interpreter.register_native("double", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        return std::get<double>(args[0]) * 2.0;
    });
}
```

### 2) Register it in CMake

Add one line:

```cmake
add_citril_library(math src/math_library.cpp)
```

This automatically builds a library named `mathLib`.

### 3) Use it in Citril

```citril
include "printLib"
include "mathLib"
print(double(21))
```

## Built-in example libraries

- `printLib` → `print(value)`
- `typesLib` → `type(value)`
- `stringsLib` → `len(value)`, `upper(value)`
- `timeLib` → `clock()`
