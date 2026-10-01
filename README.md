# Symbolic Indefinite Integration Calculator Engine

A fully modular, compiler-architected Symbolic Calculus Computer Algebra System (CAS) built from scratch using Modern C++ (C++17).

## 🚀 Key Framework Features
- **Lexical Analyzer (Lexer)**: Processes character strings into token sequences.
- **Recursive Precedence Parser**: Translates token sequences into an Abstract Syntax Tree (AST) respecting operator precedence.
- **Heuristic Integration by Parts (IBP)**: Automatically implements the algebraic-trigonometric product rule algorithm based on ILATE sequencing.
- **Advanced Linear Chain Rule Engine**: Automatically evaluates linear polynomial composite inner function structures (\(\int f(ax+b)dx\)).
- **Algebraic Optimization Pass (Simplifier)**: Condenses compound constant coefficients and eliminates trivial math identity values.

## 🛠️ Supported Mathematical System Profile
- **Variables**: `x`
- **Algebraic Operators**: `+`, `-`, `*`, `/`, `^`
- **Trigonometric Core Layout**: `sin(x)`, `cos(x)`, `tan(x)`, `sec(x)`, `cosec(x)`, `cot(x)`
- **Inverse Trigonometric Extensions**: `arcsin(x)`, `arccos(x)`, `arctan(x)`, `arcsec(x)`, `arccosec(x)`, `arccot(x)`
- **Transcendental Operators**: Exponential `exp(x)` (\(e^x\)), Logarithmic natural base `ln(x)`
