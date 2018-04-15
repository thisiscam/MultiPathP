# Project Title

Symbolic execution engine for P programming language using Value Summaries.
We apply the [MultiSE](https://people.eecs.berkeley.edu/~ksen/papers/multise.pdf) approach to perform efficient symbolic exploration of concurrent P programs with state-merging. 

## Getting Started

```
$ git clone https://github.com/thisiscam/MultiPathP.git
```

### Prerequisites

- java (>= 8)
- maven
- cmake (>= 3.0)
- a descent c++11 compiler such as gcc or clang 
- sylvan

To install sylvan, one may follow the instruction at [sylvan's documentation](https://trolando.github.io/sylvan/#building)

### Installing

```
$ cd MultiPathP
$ mvn package
```

### Running the translator

```
$ java -jar target/MultiPathP-0.1-jar-with-dependencies.jar -h
Usage: <main class> [options] input-file
  Options:
    -h, --help
      displays this help message
    -I, --include
      include search directories
      Default: []
    --name
      name of the project, default to input file name
    -o, --out-dir
      output directory
      Default: <empty string>
    -t, --translator
      translator to use
      Default: CppTranslator
```

#### Example

```
$ java -jar target/MultiPathP-0.1-jar-with-dependencies.jar -t CppTranslator --name two_phase_commit -o two-phase-commit path/to/two-phase-commit.p
```

The command above should generate a self contained folder named two_phase_commit

To build the generated code

```
$ export P_CPP_RUNTIME=/path/to/MultiPathP/runtimes/cpp_runtime/ # tell the build system where to find cpp_runtime header library
$ cd two_phase_commit; 
$ cmake -DCMAKE_BUILD_TYPE=Release .. (or =Debug)
$ ./MultiPath_two_phase_commit 
   or
  ./RandTest_two_phase_commit
```

## License

This project is licensed under the Apache License Version 2.0 License - see the [LICENSE.md](LICENSE.md) file for details
