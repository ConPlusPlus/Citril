# Citril

Citril is a programming language designed to be simple, fast, compatible, modular, and easy to reshape.

This branch is a C++ implementation of the Citril starter interpreter.

## Goals

Citril aims to feel:

- **Simple like Python or Lua**: readable syntax and small core concepts.
- **Powerful like C++**: designed to grow toward native modules, systems APIs, and future compiled backends.
- **Fast to work with like C#**: straightforward build/run workflow and helpful errors.
- **Modular**: lexer, parser, AST, runtime, built-ins, and future codegen are separated.
- **Customizable**: people should be able to change the syntax, runtime, or standard library without rewriting everything.

## Current features

- `let` variables
- assignment
- numbers, strings, booleans, `nil`
- `+`, `-`, `*`, `/`, `%`
- comparisons: `==`, `!=`, `<`, `<=`, `>`, `>=`
- logic: `and`, `or`, `!`
- `if` / `else`
- `while`
- functions with `fn`
- `return`
- native built-in `print(value)`
- comments with `//` or `#`


## File extensions

Citril uses:

- `.citril` as the official source extension
- `.ctl` as the short source extension

Both extensions run the same language. Use `.citril` for docs and real projects, and `.ctl` for quick scripts. See `docs/FILE_FORMATS.md`.

## Editor support

A starter VS Code extension is available at `editor/vscode/citril-language-support`.
It recognizes `.citril`, `.ctl`, `Citrilfile`, and shebang lines containing `citril`.

## Installation

See `docs/INSTALL.md` for macOS and Windows setup instructions.

## Build

Requires a C++17 compiler and CMake.

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/citril examples/hello.citril
```

Or run a one-liner:

```bash
./build/citril -e 'print("hello from Citril");'
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

Or directly:

```bash
./build/citril_tests
```

## Example

```citril
fn fib(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

print("fib(8) = " + fib(8));
```

## Project structure

```text
include/citril/      public engine headers
src/                 lexer, parser, runtime implementation
bin/                 CLI executable
tests/               C++ tests
examples/            Citril example programs (`.citril`, `.ctl`)
std/                 future standard library modules
docs/                design notes
```

## Roadmap

Suggested next steps:

1. Add arrays/lists and maps.
2. Add module imports.
3. Add better error spans with source snippets.
4. Add a bytecode VM for much faster execution.
5. Add native module registration.
6. Add optional type annotations.
7. Add C/C++ FFI or LLVM backend later.

The current C++ interpreter is intentionally simple and hackable so contributors can modify it without needing to understand a massive compiler first.

## Dynamic libraries

Citril supports runtime extension libraries via `--lib`. See `docs/LIBRARIES.md`.


## Core syntax style

- Use `var` for normal variables.
- Use `localvar` for variables declared inside blocks.
- Semicolons are optional.
- `print` is provided by libraries (auto-loaded from `libraries/`).
