[![Build Status](https://travis-ci.com/BenSokol/EECS675-Project1.svg?branch=master)](https://travis-ci.com/BenSokol/EECS675-Project1) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/9be4f74279de46b89bcc44d5cd41d47c)](https://www.codacy.com/app/BenSokol/EECS675-Project1?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=BenSokol/EECS675-Project1&amp;utm_campaign=Badge_Grade) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
# EECS675-Project1

## Project Specification
The project specification is located [here](https://web.archive.org/web/20190221140502/https://people.eecs.ku.edu/~jrmiller/Courses/675/Spring2019/Projects/Project1.html).

## Requirements
|             | Required                   | Prefered                      |
| ----------- | -------------------------- | ----------------------------- |
| OS          | Linux<br>macOS             |                               |
| CPU         |                            | Dual core Processor or better |
| Compiler    | gcc >= 5.4<br>clang >= 3.4 | gcc >= 6<br>clang >= 3.7      |
| C Version   | c11                        |                               |
| C++ Version | c++14                      | c++17                         |

## To use
### To compile
```bash
make
```
For compilation help, see [this](https://github.com/BenSokol/build-tools) page.

### To run
```bash
./battleship P N M
```
Where:
P is the number of players (threads) to spawn.
N is the NxN size of the board
M is the number of targets to place on the board.

The following conditions must be satisfied: P≥2, N>0, M>0, and M≤N²

### Example
```bash
./battleship 4 3 4
```
Will launch 4 players (threads), each with a 3x3 board, each containing 4 targets.

## License
[MIT](https://github.com/BenSokol/EECS675-Project1/blob/master/LICENSE).
