# Dynamic Citril Libraries

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

```cpp
extern "C" void register_citril_library(citril::Interpreter& interpreter);
```

Register natives with:

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
