# Reinforcement Learning Practical Second Project

This is the repository for the second project for the bachelor's course _Reinforcement Learning Practical_
for Artificial Intelligence. Its authors are:
* Niels de Jong, [Mail](mailto:n.a.de.jong@student.rug.nl), s3366235.
* Bart van der Woude, [Mail](mailto:b.r.van.der.woude@student.rug.nl), s3498891.

#### Requirements

* `g++`
* `cmake`
* `make`
* `Rscript`

#### OS support

This program is known to work on:

* Ubuntu 16.04.6 LTS
* MacOS Catalina 10.15.2

## How to compile the program

1. First, `cd` to this directory.
2. Then `cd code/`.
3. `cmake CMakeLists.txt`
4. `make`

## How to run the program

1. Ensure you're in the `code/` subdirectory.
2. `./executables/maze`
3. Follow the instructions within the program. The mazes you can choose are shown below.

The options for mazes to include are:

| Maze | Name (type _exactly_ this in the program)  | Location from root of project |
|---|---|---|
| Static maze 1 | `static/maze-1`  | `code/input/static/maze-1` |
| Static maze 2 | `static/maze-2`  | `code/input/static/maze-2` |
| Static maze 3 | `static/maze-3` | `code/input/static/maze-3` |
| Static maze 4 | `static/maze-4` | `code/input/static/maze-4` |
| Dynamic maze 1 | `dynamic/maze-1` | `code/input/dynamic/maze-1` |
| Dynamic maze 2 | `dynamic/maze-2` | `code/input/dynamic/maze-2` |
| Dynamic maze 3 | `dynamic/maze-3` | `code/input/dynamic/maze-3` |
| Dynamic maze 4 | `dynamic/maze-4` | `code/input/dynamic/maze-4` |

Enter each maze on a separate line. Use `done` to stop. For instance:

```
  ...
  Which mazes would you like to use?
      Options: Pre-built maze in input/ (excluding .in), or self-built maze (by typing '-').
      Multiple entries possible. Stop by typing 'done'.
      Choice: static/maze-1
      Choice: dynamic/maze-3
      Choice: done
  ...
```

4. Wait for the program to finish (this may take a while).

## How to view results

1. Ensure you're in the `code/` subdirectory.
2. `cd output/`
3. `Rscript figure-creator.r`
4. Wait for the figure creator to finish.
5. `cd figures/`
6. Open any of the figures. E.g., `xdg-open exploitation-boxplot.eps &`.
