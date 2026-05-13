# Installing and Running Citril

This guide covers local setup for macOS and Windows. Linux uses the same CMake flow shown in the README.

## macOS

### 1) Install build tools

Use either Xcode Command Line Tools alone or CLT + Homebrew CMake.

```bash
xcode-select --install
brew install cmake
```

### 2) Clone and build

```bash
git clone <your-fork-or-citril-url>
cd Citril
cmake -S . -B build
cmake --build build
```

### 3) Run and test

```bash
./build/citril examples/hello.citril
ctest --test-dir build --output-on-failure
```

## Windows

### Option A: Visual Studio (recommended)

1. Install **Visual Studio 2022** with the **Desktop development with C++** workload.
2. Open **x64 Native Tools Command Prompt for VS 2022**.
3. Build:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

4. Run and test:

```powershell
.\build\Release\citril.exe examples\hello.citril
ctest --test-dir build --output-on-failure --build-config Release
```

### Option B: Ninja + Clang/LLVM or MSYS2

As long as your compiler supports C++17 and CMake can find it, Citril should build with the same configure/build/test steps.

## Notes

- Citril requires **C++17**.
- If tests are not discovered, re-run CMake configure step and then `ctest --test-dir build --output-on-failure`.
