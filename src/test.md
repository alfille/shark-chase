# Shark Test

Test the output of a simulation (the __dat__ file)

* Independent calculation of length and speed
* Count of data points (segments)
* Summary statitics
* python program

Example:

```
$ python3 shark_test.py 
Series: Man
        Data points: 101
        Column 1 range -1.21704 to 0.0813778
        Column 2 range 0.0 to 1.0
        Path length 1.0843548465846633
Series: Shark
        Data points: 101
        Column 1 range -1.19586 to 3.14159
        Column 2 range 1.0 to 1.0
        Path length 4.33707183052191
Compare speed
        Man: Man
        Shark: Shark
        Speed:
                max 4.000628839320472
                min 3.9984627885042867
                avg 3.999679481474318
```