# Dynamic Citril Libraries

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

Inside that function, register functions using:

```cpp
interpreter.register_native("name", arity, [](const std::vector<citril::Value>& args) -> citril::Value {
    // ...
});
```

## Starter example

See `src/sample_library.cpp` for a library that adds:

- `len(value)`
- `type(value)`

## Demo script

Run `examples/library_demo.citril` with the sample library loaded.
