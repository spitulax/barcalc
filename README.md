<h1 align="center">Barcalc</h1>

Barcalc is a simple GUI calculator made with [GTK4](https://www.gtk.org/). This
calculator supports some basic operations such as addition, multiplication,
subtraction, division, power, square root, etc.

## Building

### Nix (Flake)

```sh
$ nix build
$ ./result/bin/crt --help
```

### Other

```sh
$ meson setup build && meson compile -C build
$ ./build/crt --help
```
