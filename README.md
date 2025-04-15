# OAAX tools

This repository is meant to provide utility functions that are helpful for developing, testing and using OAAX runtimes and toolchains.

## Overview

The C utilities folder contains three modules:
- [Logger](c-utilities/include/logger.h): A simple logging library that provides a way to log messages with different severity levels (info, warning, error).
- [Timer](c-utilities/include/timer.h): A utility for measuring latency and IPS (instructions per second) of a given function.
- [Memory](c-utilities/include/memory.h): A utility for measuring memory usage of a process.

To test these modules, you can refer to the `tests` folder, which contains a `main.c` script. You can build it and run it by executing the following commands:
```bash
bash tests/build-and-run.sh
```

## Contributing

If you would like to contribute to this repository, please follow the guidelines in the [CONTRIBUTING.md](CONTRIBUTING.md) file.

## License

This repository is licensed under the Apache License 2.0. See the [LICENSE](LICENSE) file for more details.