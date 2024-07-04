# OAAX tools

This repository is meant to provide utility functions that are helpful for developing, testing and using OAAX runtimes and toolchains.

## Overview

The C utilities folder contains two scripts for profiling the memory usage of a runtime, and benchmarking its latency.

An illustrative example is available to help get started with using the utilities:
```bash
cd c-utilities
mkdir build
cd build
cmake ..
make
./c-utilities
```