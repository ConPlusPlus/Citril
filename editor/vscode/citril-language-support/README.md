# Citril Language Support (VS Code)

This extension enables basic Citril support:

- File detection for `.citril`, `.ctl`, and `Citrilfile`
- Shebang detection containing `citril`
- Syntax highlighting for current core language tokens
- Bracket and comment behavior

## Local usage

1. Open this folder in VS Code:
   `editor/vscode/citril-language-support`
2. Press `F5` to run an Extension Development Host.
3. Open a `.citril` or `.ctl` file and verify highlighting.

## Packaging

```bash
npm install -g @vscode/vsce
cd editor/vscode/citril-language-support
vsce package
```
