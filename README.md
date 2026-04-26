# Project README

## Overview
This project is a development environment for building C/C++ applications using various operating systems and environments. It includes support for Linux, Windows, WebAssembly, and Wine platforms.

## Features
- Multi-platform build system (Linux, Windows, WebAssembly, Wine)
- Standard development tools
- Libraries for specific languages and frameworks

## Project Structure
```
<Project>/
├── build/              # .exe files produced by Main.c
├── bin/                # .so / .dll produced by *.c in libs
├── libs/               # *.c for bin
├── lib/                # librarys for my own languages
├── code/               # scripts from my custom languages for example .rex, .ll, .omml
├── data/               # Datafile for example .txt or dumped files
├── assets/             # images and sound files
├── src/                # source code
│   ├── Main.c          # Entry point
│   └── *.h             # header-based C-files without *.c implementation
├── Makefile.linux      # Linux Build configuration
├── Makefile.windows    # Windows Build configuration
├── Makefile.wine       # Wine Build configuration
└── README.md           # This file
```

## Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- Libraries needed in specific projects (example WINAPI, X11, ALSA)

## Build & Run
### Linux
```sh
cd <Project>
make -f Makefile.linux all
./build/Main
```

### Windows
```sh
cd <Project>
make -f Makefile.windows all
./bin/Main.exe
```

### WebAssembly (Wasm)
```sh
cd <Project>
make -f Makefile.web all
emrun build/index.html --browser chrome
```

### Wine
```sh
cd <Project>
make -f Makefile.wine all
wine bin/Main.exe
```

### Build Options
- `make -f Makefile.(os) all`        # build output
- `make -f Makefile.(os) do`         # build + exe output
- `make -f Makefile.(os) clean`      # Remove build artifacts

For more detailed instructions, refer to the specific makefiles in the project.