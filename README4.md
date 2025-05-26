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

# Results

See [next page](README5.md) for results

-----------
by Paul H Alfille 2025
[on Github](https://github.com/alfille/shark-chase)

