# Citril Codex Brief

## Current status

Citril is a small, modular programming language currently implemented as a C++17 tree-walk interpreter. The project is intended to grow into a language that is simple like Python/Lua, powerful and modular like C++, and eventually faster through a bytecode VM or compiler pipeline.

The repository currently contains:

- A CMake-based C++17 build.
- A reusable `citril_core` library.
- A command-line runner named `citril`.
- Lexer, parser, AST, environment, value, and interpreter modules.
- Basic examples in `.citril` and `.ctl` formats.
- A starter VS Code language extension for file recognition, syntax highlighting, and snippets.
- A basic C++ test executable registered with CTest.

## File formats

Use these source file extensions:

- `.citril`: official full Citril source extension.
- `.ctl`: short Citril source extension for quick scripts.

Possible future formats:

- `citril.json` or `Citrilfile`: project/package configuration.
- `.citmod`: compiled module format.
- `.citlock`: dependency lock file.

## Current syntax

Supported syntax includes:

```citril
let x = 10;
print(x + 5);

fn add(a, b) {
    return a + b;
}

if (add(1, 2) == 3) {
    print("works");
} else {
    print("broken");
}

while (x > 0) {
    x = x - 1;
}
```

`print` is currently a native function, not a special keyword. Keep it as a normal callable for now so user-defined functions and built-ins share the same model.

## Build and test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run an example:

```bash
./build/citril examples/hello.citril
```

On Windows, the executable may be at `build/Debug/citril.exe` depending on the CMake generator.

## VS Code support

The starter VS Code extension lives in:

```text
editor/vscode/citril-language-support
```

It should recognize:

- `.citril`
- `.ctl`
- `Citrilfile`
- shebangs containing `citril`

The workspace settings also associate `*.citril` and `*.ctl` with the Citril language id.

## Important implementation notes

- Use C++17 or newer. `Value` depends on `std::variant`, `std::holds_alternative`, and `std::get_if`.
- Do not compile as C++14; that causes missing `std::variant` errors.
- Keep engine pieces modular: lexer, parser, AST, interpreter/runtime, stdlib, CLI, editor tooling.
- Avoid making every built-in a keyword. Prefer native functions where possible.
- Keep `.DS_Store`, `__MACOSX`, and build outputs out of git.

## Recommended next tasks for Codex

1. Make the VS Code extension packageable as a `.vsix` using `vsce` or `@vscode/vsce`.
2. Add a Citril logo/icon and wire it into the VS Code extension package.
3. Expand tests for lexer, parser, runtime errors, functions, closures, loops, and native functions.
4. Add more built-in functions, such as `input`, `len`, `clock`, `type`, and string utilities.
5. Improve error messages with file name, line, column, and source context.
6. Add arrays/lists, maps/dictionaries, modules/imports, and a basic standard library.
7. Design `citril.json` or `Citrilfile` for project configuration.
8. Add a bytecode VM roadmap and begin separating frontend parsing from backend execution.
9. Add GitHub Actions CI for macOS, Windows, and Linux builds.
10. Add installation docs for Mac and Windows.

## Long-term goal

Citril should eventually move through these stages:

1. C++ interpreter.
2. C++ bytecode VM.
3. Optimized bytecode compiler.
4. Standard library and package/module system.
5. Partial self-hosting where parts of Citril tooling are written in Citril.
6. Full or near-full self-hosting if the language becomes stable enough.
