# athr

[![Travis](https://img.shields.io/travis/horta/almosthere.svg?style=flat-square&label=linux%20%2F%20macos%20build)](https://travis-ci.org/horta/almosthere) [![AppVeyor](https://img.shields.io/appveyor/ci/Horta/almosthere.svg?style=flat-square&label=windows%20build)](https://ci.appveyor.com/project/Horta/almosthere)

Progress indicator C library.

ATHR is a simple yet powerfull progress indicator library that works on Windows, Linux, and macOS. It is non-blocking as the progress update is done via a dedicated, lightweight thread, as to be impair the performance of the calling program.

## Get started

On Linux or macOS, open a terminal and install it via
```bash
bash <(curl -fsSL https://raw.githubusercontent.com/limix/progressbar/master/install)
```

Similarly, on Windows, open a command prompt and install it via
```powershell
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://raw.githubusercontent.com/horta/almosthere/master/install.bat', 'install.bat')" && install.bat
```

The above commands will download the latest library version, compile it to create static and dynamic library objects and copy the C/C++ header into the appropriate directory. (For a more manual approach, see the section [Manual installation](#Manual%20installation) in this document.)

It is now ready to be linked into in your C/C++ project. Suppose you have a file `example.c` that makes use of this library. Using `gcc` compiler, you can do
```bash
gcc example.c -lathr -o example
```
to produce a binary `example` linked against `athr` library.

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

## Manual installation

Clone and enter into the repository folder
```bash
git clone https://github.com/horta/almosthere
cd almosthere
```

Create a `build` folder to not clutter the project and proceed with cmake steps
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make && make test && make install
```

## cmake

If you use [cmake](https://cmake.org/) to manage your project, you can use [findATHR.cmake](findATHR.cmake) module to configure it with the `athr` library once it has been installed.

## Interface

It consists in two functions
```c
void athr_consume(struct athr *at, long consume);
void athr_finish(struct athr *at);
```
and a variadic macro

```c
athr_create(...)
```



## Authors

* [Danilo Horta](https://github.com/horta)

## Acknowledgements

- [bk. answer](https://stackoverflow.com/a/2926165) for providing the mechanism of defining default arguments in C.
- [asciinema](https://asciinema.org/) for such amazing recording tool.
- [asciicast2gif](https://github.com/asciinema/asciicast2gif) for the converter tool from asciinema cast to gif.

## License

This project is licensed under the [MIT License](https://raw.githubusercontent.com/horta/almosthere/master/LICENSE.md).
