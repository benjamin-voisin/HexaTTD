# HexaTTD

Some experiments on implementing an openttd like game with hexagonal grid.

Work in progress.

## Build

This project uses [raylib](raylib.com), included in the git as a submodule. If
you don’t already have raylib, use 
```
git clone --recursive https://codeberg.org/BenjaminV/HexaTTD.git
```
to clone the project.

If you already have the source code somewhere, you just have to change the
variable `RAYLIB_DIR` and `RAYLIB_RELEASE_PATH` in the Makefile.

Then, juste
```
make
```
will build the project.
