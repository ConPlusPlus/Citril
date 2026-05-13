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
