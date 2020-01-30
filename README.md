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
3. Follow the instructions within the program. Choose some mazes you want to try out.
4. Wait for the program to finish (this may take a while).

## How to view results

1. Ensure you're in the `code/` subdirectory.
2. `cd output/`
3. `Rscript figure-creator.r`
4. Wait for the figure creator to finish.
5. `cd figures/`
6. Open any of the figures. E.g., `xdg-open exploitation-boxplot.eps &`.
