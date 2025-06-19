#!/usr/bin/python3

# shark.py
# read a "dat" file (formatted for gnuplot" and show summary statistics

import subprocess

import tkinter as tk
from tkinter import filedialog

import sys
import math
import io

class Graph:
	def __init__( self, dat_names="" ):
		self.dat_names = dat_names ;
		if dat_names == "":
			# Use Tk file dialog
			root = tk.Tk()
			root.withdraw()
			self.dat_names = filedialog.askopenfilenames(title="Enter the DAT file to parse:",multiple=True,filetypes=(
				("dat files","*.dat"),
				("DAT files","*.DAT"),
				("Text file","*.txt"),
				("Text file","*.TXT"),
				("All files","*"),
				("All files","*.*"),)
				)
			root.destroy()

	def sim_type( self ) :
		return self.dat_names[0].split("/")[-1].split("_")[0]
		
	def plotfile( self, filename, index ):
		key = ".".join(filename.split("/")[-1].split(".")[:-1]).split("_",1)[-1].replace("_",":")
		color = "#{:02x}{:02x}{:02x}".format(0xFF-index*15,(index&6) * 16,index*8)
		return "\"{}\" using 1:2 index 0 title \"{}\" with lines lc rgb \"{}\", \"{}\" using 1:2 index 1 title \"{}\" with lines lc rgb \"{}\", ".format(filename,key,color,filename,"",color)

	def write( self, process ):
		process.stdin.write("unset border\n")
		process.stdin.write("set polar\n")
		process.stdin.write("unset xtics\n")
		process.stdin.write("unset ytics\n")
		process.stdin.write("unset rtics\n")
		process.stdin.write("set ttics axis\n")
		process.stdin.write("set rtics axis\n")
		process.stdin.write("set title \"{}\"\n".format(self.sim_type()))
		process.stdin.write("set grid polar\n")
		process.stdin.write("set size square\n")
		process.stdin.write("set key outside\n")
		process.stdin.write("set rrange [0:1.500000]\n")
		process.stdin.write("set rtics ('0.25' 0.25, '0.50' 0.50, '1.00' 1.00)\n")
		process.stdin.write("set key autotitle columnheader\n")
		process.stdin.write("plot ")
		index = 0
		for f in self.dat_names:
			process.stdin.write(self.plotfile( f, index&15 ))
			index += 1
		process.stdin.write("1\n")
#plot for [speed in speeds] for [i=0:1] sim_type.points."_".speed.".dat"  using 1:2 index i title (i&1)?"":speed with lines ; print 3
		process.stdin.close()
		
	def run( self ):
		process = subprocess.Popen( ["gnuplot", "-p", "-"] , stdin=subprocess.PIPE, text=True )
		self.write( process )

		
def main( sysargs ):
	if len(sysargs) > 1:
		graph = Graph(sysargs[1])
	else :
		graph = Graph()
	graph.run()
	


if __name__ == "__main__":
    sys.exit(main(sys.argv))
else:
    print("Standalone program")
