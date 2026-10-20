# OAAX tools

This repository provides utility libraries for developing, testing, and using OAAX runtimes and toolchains.

[![Run tests](https://github.com/OAAX-standard/tools/actions/workflows/run-tests.yml/badge.svg)](https://github.com/OAAX-standard/tools/actions/workflows/run-tests.yml)
[![Run cpplint](https://github.com/OAAX-standard/tools/actions/workflows/run-linter.yml/badge.svg)](https://github.com/OAAX-standard/tools/actions/workflows/run-linter.yml)

## oaax-runtime-utils

A standalone C99 static library (`liboaax_utils`) implementing helper functions for the OAAX v2.0.0 runtime interface. Works on Linux and Windows.

### Tensor helpers

| Function | Description |
|---|---|
| `tensors_alloc(n)` | Allocate a `Tensors` with `n` zero-initialised descriptors |
| `tensors_free(t)` | Deep-free a `Tensors` and all its descriptors |
| `tensors_deep_copy(src)` | Deep-copy a `Tensors` into independently owned memory |
| `tensors_set(t, i, name, type, rank, shape, data, size)` | Fill one descriptor; copies name and shape, stores data pointer as-is |
| `tensors_compare(a, b)` | Compare two `Tensors` for equality (metadata + data) |
| `tensors_print(t)` | Print a human-readable summary to stdout |

### Data size helpers

| Function | Description |
|---|---|
| `compute_data_size(type, rank, shape)` | Byte size of a tensor buffer; handles sub-byte types correctly |
| `element_byte_size(type)` | Byte size of one element; returns 0 for sub-byte types |

### Config helpers

| Function | Description |
|---|---|
| `config_alloc()` | Allocate an empty `Config` |
| `config_free(c)` | Free a heap-owned `Config` and all its strings |
| `config_set(c, key, value)` | Insert or update a key-value pair |
| `config_get(c, key)` | Look up a value by key; returns `NULL` if not found |
| `config_delete(c, key)` | Remove an entry by key |

### Status helper

| Function | Description |
|---|---|
| `runtime_status_string(status)` | Human-readable name for a `RuntimeStatus` value |

## Building and testing

```bash
bash tests/build-and-run.sh
```

## Contributing

Please follow the guidelines in [CONTRIBUTING.md](CONTRIBUTING.md).

## License

Apache License 2.0 — see [LICENSE](LICENSE).
