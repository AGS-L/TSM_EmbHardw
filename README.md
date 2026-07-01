# TSM-EmbHardw

## Project Structure

- [`bin`](bin): precompiled binary
- [`bit`](bit): pregenerated bitstream
- [`docs`](docs): documentation
- [`soc/camera-program`](soc/camera-program): program source files, which are recompiled into the binary and loaded onto the softcore
- [`soc/or1300`](soc/or1300): softcore source files, which are used to regenerate the bitstream

## Project Setup

Clone with:
```sh
git clone https://github.com/AGS-L/TSM_EmbHardw.git
```

**Note**: These instructions have been taken from another source. Only the 'Windows native' instructions have been verified. If any documentation is unclear, please let us know so it may be improved.

Instructions for different OSes:

- [Linux](docs/README_linux.md)
- [macOS](docs/README_macOS.md)
- Windows
  - [Windows WSL](docs/README_windows_WSL.md)
  - [Windows native](docs/README_windows.md)

# Credit

<https://gecko5education.ti.bfh.ch/index.html>

## License

See [LICENSE](LICENSE)
