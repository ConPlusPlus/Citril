# Dynamic Citril Libraries

Citril can load libraries in two ways:

1. Automatically from the `libraries/` folder.
2. Explicitly with `--lib /path/to/library`.

## Nice library names

Library names are now clean and short:

- `include "print"`
- `include "types"`
- `include "strings"`
- `include "time"`

No `lib...` prefix is required in Citril source.

## Super quick library creation

1. Create a source file, for example `src/math_library.cpp`:

```cpp
#include "citril/interpreter.hpp"
#include "citril/value.hpp"

extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    interpreter.register_native("double", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        return std::get<double>(args[0]) * 2.0;
    });
}
```

2. Add one line in `CMakeLists.txt`:

```cmake
add_citril_library(math src/math_library.cpp)
```

3. Build and use it from Citril:

```citril
include "math"
print(double(21))
```

## Include statement

You can load libraries directly in code:

```citril
include "print"
include "strings"
```

## Built-in example libraries

- `print` → `print(value)`
- `types` → `type(value)`
- `strings` → `len(value)`, `upper(value)`
- `time` → `clock()`
