# stdinify.so

This shared library can be injected into processes via `LD_PRELOAD` to force input from `/dev/tty` to be pulled from `stdin`.

It's a toy project that should not be used except you know what you are doing.
This can break some apps relying on `/dev/tty`, but it tries it's best at keeping applications functional.

The "intended" usecase for this will be the benchmarking function that [rdo](https://codeberg.org/sw1tchbl4d3/rdo) provides, to make doas take input from stdin in a fun way.

To that it should act as a small example on how to intercept libc functions with `LD_PRELOAD`.

### Usage

The library can be built by running:

```sh
make
```

The resulting library can then be used with `LD_PRELOAD`.

If you want to use the library with all applications or on an SUID binary, you will have to install it into the preload globally.

This is not advised except you know what this implies, or if you are running this in a container or similar, as this may break core applications by replacing open(), read() and close() calls **globally**.

If you are certain, you can execute

```
make install
```

to make every application preload it, given that `/etc/ld.so.preload` is respected by the dynamic loader (pretty much glibc only, not musl).

To revert this change, run

```
make uninstall
```
