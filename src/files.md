# Files

Besides programs, shark-chase generates several file types:

* gnuplot control
  * named "controlXXXX.gplot" where XXXX is the number of segments
  * Encodes the name of the data file directly
  * Can be run from the command line. Example:
```
    gnuplot -p control1000.gplot
```
* data files
  * See below
* [graphs](./display.md) (optional)
  * PNG files using the "-o" command line option
* [CSV (comma separated values)](./CSV_file.md) (optional)
  * for speadsheet and further analysis
  * using the "dat2csv.py" program

