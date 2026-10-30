# Graph Path Program

## Requirements

- GCC or another C compiler
- GNU Make

## Build

From the project root, run:

```sh
make
```

This creates the executable at `build/final_program`.

## Run

Provide an input file through standard input:

```sh
./build/final_program < data/test01.uwg
```

Other sample inputs are available in the `data/` directory:

```sh
./build/final_program < data/test02.uwg
```

The program prints the selected edges and the resulting solution to standard output.

## Clean

Remove generated build files with:

```sh
make clean
```
