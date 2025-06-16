# Files

Besides programs, shark-chase generates several file types:

* gnuplot control
  * Example __anneal100_4.00.gplot__
    * simulated *anneal*ing
    * *100* segments
    * shark speed *4.00*
  * Encodes the name of the data file directly
  * Can be run from the command line. Example:
```
    gnuplot -p anneal100_4.00.gplot
```
* data files
  * See below
* [graphs](./display.md) (optional)
  * PNG files using the "-o" command line option
* [CSV (comma separated values)](./CSV_file.md) (optional)
  * for speadsheet and further analysis
  * using the "dat2csv.py" program

-------------
## Data file

* Example __anneal100_4.00.dat__ 
  * Simulated *anneal*ing
  * *100* segments
  * shark speed *4.00*
* 2 series of data separated by 2 blank lines
  * Series name (Man, then Shark)
  * data point per line (101 data points in this case)
    * 2 floating point numbers
    * space in between
    * radian distance
* Shark distance numbers is an artificial spiral from 1.5 to 1 for display purposes
  * All calculations use correct shark distance = 1.00

```
Man
0 0
0.0267048 0.01
0.0405583 0.02
0.0813778 0.03
-0.00291629 0.04
...


Shark
3.14159 1.5
3.10159 1.495
3.06158 1.49
3.02139 1.485
2.97972 1.48
...
```
