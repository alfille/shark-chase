# shark-chase (simulated annealing)
Puzzle: Can you get to the beach before the shark gets there? 

## Puzzle

![picture](picture.png)

* You are on a circular island.
* There is a smart hungry shark that can swim 4 times faster than you can run. 
* You can see his position (and he can see yours).

Can you get to the beach before he gets there?
You're lazy. What's the shortest distance you need to run to get there?

# Optimization

We will use [Simulated Annealing](https://en.wikipedia.org/wiki/Simulated_annealing) as the optimization algorithm. It's a way to search a potentially large solution space and converge on an optimal solution.

It is designed to explore beyond local minima to find better solutions.

## Tools

* C program
* [GNU Scientific Library](https://www.gnu.org/software/gsl/doc/html/siman.html)
  * Simulated Annealing
  * Random numbers
* [Gnuplot](http://gnuplot.info/) for visulizing the solutions

## Setup
Get the program:

`git clone https://github.com/alfille/shark-chase`

Install prerequisites in linux (Debian or Ubuntu):

`sudo apt install libgsl-dev gnuplot`

(this assumes you have already installed the compiler `gcc`).

## Compiling and running
    cd shark-chase
    gcc -o shark shark.c -lgsl -lm
    chmod +x ./shark
    ./shark

# The code

The entire program is in `shark.c`

Most of the parameters for the simulated annealing are taken directly from the examples in the GSL documentation.

The number of points defaults to 100 but can be changed on the command line 

`./shark -p 1000`

for 1000 points.

Commnnication with the external plotting program is with 2 files that are created each time:

* Gnuplot commands: controlnnnn.gplot (nnnn is number of points)
* Gnuplot data: datannnn.dat

# Graphing

To repeat the last graph generated:

```
gnuplot control100.gplot
```
assuming that the last run was 100 points.

# Data file

The data file (e.g. `data100.dat`) is in a simple format:
```
Man
angle0 distance0
angle1 distance1
angle2 distance2
...


Shark
angle0 distance0
angle1 distance1
angle2 distance2
...
```
* The angle is in radians
* The distance is from the center
* Distance of 1.0 is the beach
* The Shark distance is an arbitrary spiral inward to make visulaization easier

# Command line options:
```
e$ ./shark -h 
shark-chase
        find fastest way to beach avoiding shark
        by Paul H Alfille 2023 -- MIT Licence
        See https://github.com/alfille/shark-chase

shark [options]

Options
        -p100   --path          number of steps (default 100)
        -s4     --speed         Shark speed (default 4)
        -v      --verbose       show progress during search
        -h      --help          this help

Obscure options
        -n10000 --ntries        Points before stepping (default 10000)
        -i10000 --iterations    Iterations at each temperature (default 10000)
        -m1     --multiplier    Multiplier for pertubation (default 1)
        -K1     --boltzman      Boltzman constant (default 1)
        -u1.003 --mu    Temperature damping factor (default 1.003)
        -t0.008 --temperature   Initial temperature (default 0.008)
        -x2e-06 --final Final temperature (default 2e-06)
```

The obscure options just tune the simulated annealing algorithm. The defaults work well.
# Results

------------
### [NEXT](README5.md)

* [Puzzle posed](README.md)
* [Naive solution](README2.md)
* [Computing strategy](README3.md)
* [The program](README4.md)
* [Solution results](README5.md) <- NEXT
-----------
by Paul H Alfille 2025
[on Github](https://github.com/alfille/shark-chase)

