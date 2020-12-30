# athr

[![Travis](https://img.shields.io/travis/com/horta/almosthere.svg)](https://travis-ci.com/horta/almosthere)

Progress indicator C library.

ATHR is a simple yet powerful progress indicator library that works on Windows, Linux, and macOS. It is non-blocking as the progress update is done via a dedicated, lightweight thread, as to not impair the performance of the calling program.

## Get started

On Linux, macOS, and Windows (Git bash terminal) open a terminal and install it via
```bash
curl -fsSL https://git.io/JerYI | GITHUB_USER=horta GITHUB_PROJECT=almosthere bash
```

The above commands will download the latest library version, compile it to create a dynamic library and copy the C header into the appropriate directory. (For a more manual approach, see the section [Manual installation](#manual-installation) in this document.)

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
![Example 1](figures/example1.gif)

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
![Example 2](figures/example2.gif)

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
![Example 3](figures/example3.gif)

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
![Example 4](figures/example4.gif)

## Manual installation

Clone and enter into the repository folder
```bash
git clone https://github.com/horta/almosthere
cd almosthere
```

Create a `build` folder to not clutter the project and proceed with cmake steps
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
make && make test && make install
```

## Interface

It consists in two functions
```c
void athr_consume(struct athr *at, long consume);
void athr_finish(struct athr *at);
```

a variadic macro

```c
athr_create(...)
```

and three enum options

```c
enum ATHR_OPTS { ATHR_BAR = 1, ATHR_ETA = 2, ATHR_PERC = 4 };
```

The variadic macro is better explained by examples
```c
/* progress indicator with ATHR_BAR, ATHR_ETA, and ATHR_PERC widgets */
struct athr *at0 = athr_create(100);

/* progress indicator with all the widgets plus a description */
struct athr *at1 = athr_create(100, "Description");

/* progress indicator with ATHR_BAR widget plus a description */
struct athr *at2 = athr_create(100, "Description", ATHR_BAR);

/* progress indicator with ATHR_BAR and ATHR_ETA widgets plus a description */
struct athr *at3 = athr_create(100, "Description", ATHR_BAR | ATHR_ETA);

/* progress indicator with ATHR_PERC widget plus a description */
struct athr *at4 = athr_create(100, .opts=ATHR_PERC, .desc="Description");

/* progress indicator with ATHR_PERC widget only */
struct athr *at5 = athr_create(100, .opts=ATHR_PERC);
```

The first parameter is mandatory and specify the total volume from which we will consume through `athr_consume` calls.
A `athr_finish` call then ends the process.

## Authors

* [Danilo Horta](https://github.com/horta)

## Acknowledgements

- [bk. answer](https://stackoverflow.com/a/2926165) for providing the mechanism of defining default arguments in C.
- [asciinema](https://asciinema.org/) for such amazing recording tool.
- [asciicast2gif](https://github.com/asciinema/asciicast2gif) for the converter tool from asciinema cast to gif.

## License

This project is licensed under the [MIT License](https://raw.githubusercontent.com/horta/almosthere/main/LICENSE.md).
