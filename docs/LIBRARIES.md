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
Citril is now intentionally minimal in core. Even `print` comes from a library.

## Auto-loaded `libraries/` folder

At startup, `citril` auto-loads dynamic libraries found in the repo `libraries/` folder.

- Linux: `.so`
- macOS: `.dylib`
- Windows: `.dll`

You can also add explicit libraries with `--lib`.

## Runtime loading examples

```bash
cmake -S . -B build
cmake --build build
./build/citril examples/basics.citril
./build/citril --lib ./build/libsample_citril_lib.so examples/library_demo.citril
```

## Library API

Each dynamic library exports this symbol:
You can extend the language at runtime without recompiling the core interpreter.

## Runtime loading

Use one or more `--lib` flags:

```bash
./build/citril --lib ./build/libsample_citril_lib.so examples/library_demo.citril
```

Windows:

```powershell
.\build\Release\citril.exe --lib .\build\Release\sample_citril_lib.dll examples\library_demo.citril
```

## Library API

Each dynamic library exports this C symbol:

```cpp
extern "C" void register_citril_library(citril::Interpreter& interpreter);
```

Register natives with:
Inside that function, register functions using:

```cpp
interpreter.register_native("name", arity, [](const std::vector<citril::Value>& args) -> citril::Value {
    // ...
});
```

## Included libraries

- `libraries/print_library.cpp` → registers `print(value)`
- `src/sample_library.cpp` → registers `len(value)` and `type(value)`


## Language include statement

You can load a library from code:

```citril
include "print_citril_lib"
include "sample_citril_lib"
```

This resolves to files in `libraries/` (for example `libraries/libprint_citril_lib.so` on Linux).
## Starter example

See `src/sample_library.cpp` for a library that adds:

- `len(value)`
- `type(value)`

## Demo script

Run `examples/library_demo.citril` with the sample library loaded.
