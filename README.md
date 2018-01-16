# athr

[![Travis](https://img.shields.io/travis/horta/almosthere.svg?style=flat-square&label=linux%20%2F%20macos%20build)](https://travis-ci.org/horta/almosthere) [![AppVeyor](https://img.shields.io/appveyor/ci/Horta/almosthere.svg?style=flat-square&label=windows%20build)](https://ci.appveyor.com/project/Horta/almosthere)

Progress indicator C library.

ATHR is a simple yet powerfull progress indicator library that works on Windows, Linux, and macOS.

## Usage

On Linux or macOS, install it via
```bash
bash <(curl -fsSL https://raw.githubusercontent.com/limix/progressbar/master/install)
```

On Windows, install it via
```powershell
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://raw.githubusercontent.com/horta/almosthere/master/install.bat', 'install.bat')" && install.bat
```

Then link it in your C/C++ project. Suppose you have a file `example.c` that makes use of this library. Using `gcc` compiler, you can do
```bash
gcc example.c -lathr -o example
```
to produce a binary `example` linked against `athr` library.

If you use [cmake](https://cmake.org/) to build your project, you can use [findATHR.cmake](findATHR.cmake) module to configure it with the `athr` library.

## Examples

```c
/* example1.c */

#include "athr.h"

int main() {
    struct athr *at = athr_create(100);
    int i;

    for (i = 0; i < 100; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
```
![Example 1](example1.gif)

```c
/* example2.c */

#include "athr.h"

int main() {
    struct athr *at = athr_create(100, "My tasks");
    int i;

    for (i = 0; i < 100; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
```
![Example 2](example2.gif)

```c
/* example3.c */

#include "athr.h"

int main() {
    struct athr *at = athr_create(100, "My tasks", ATHR_PERC);
    int i;

    for (i = 0; i < 100; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
```
![Example 3](example3.gif)

```c
/* example4.c */

#include "athr.h"

int main() {
    struct athr *at = athr_create(100, .opts = ATHR_PERC);
    int i;

    for (i = 0; i < 100; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
```
![Example 4](example4.gif)

## Authors
* [Danilo Horta](https://github.com/horta)

## Acknowledgements

- [bk. answer](https://stackoverflow.com/a/2926165) for providing the mechanism of defining default arguments in C.
- [asciinema](https://asciinema.org/) for such amazing recording tool.
- [asciicast2gif](https://github.com/asciinema/asciicast2gif) for the converter tool from asciinema cast to gif.

## License

This project is licensed under the [MIT License](https://raw.githubusercontent.com/horta/almosthere/master/LICENSE.md).
