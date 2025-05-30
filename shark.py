#!/usr/bin/python3

# shark.py
# read a "dat" file (formatted for gnuplot" and show summary statistics

import tkinter as tk
from tkinter import filedialog

import sys
import math
import io

class Series:
	def __init__( self, series ):
		lines = series.strip().split("\n")
		self.title = lines[0] 
		self.t = [ float((xx.split())[0]) for xx in lines[1:] ]
		self.r = [ float((xx.split())[1]) for xx in lines[1:] ]

	def info( self ) :
		print( "Series: {}".format(self.title) )
		print( "\tData points: {}".format( len(self.t) ) )
		print( "\tColumn 1 range {} to {}".format(min(self.t),max(self.t) ) )
		print( "\tColumn 2 range {} to {}".format(min(self.r),max(self.r) ) )
		
	def calc_path( self ):
		self.segments = [ math.sqrt( self.r[i]**2 + self.r[i-1]**2 - 2*self.r[i]*self.r[i-1]*math.cos(self.t[i]-self.t[i-1])  ) for i in range(1,len(self.t)) ]
		print( math.fsum( self.segments ) )

def dat_slurp(dat_name=""):
	# Possibly request file (if not specified on command line) and read it in
	
	if dat_name == "":
		# Use Tk file dialog
		root = tk.Tk()
		root.withdraw()
		dat_name = filedialog.askopenfilename(title="Enter the DAT file to parse:",filetypes=(
			("dat files","*.dat"),
			("DAT files","*.DAT"),
			("Text file","*.txt"),
			("Text file","*.TXT"),
			("All files","*"),
			("All files","*.*"),)
			)
		root.destroy()

	try: 
		with open(dat_name,"r") as dat:
			return dat.read()
	except:
		print(f"Unable to read {dat_name}\n") 
		sys.exit(1)
		
def main( sysargs ):
	if len(sysargs) > 1:
		dat_data = dat_slurp(sysargs[1])
	else :
		dat_data = dat_slurp()

	dat_data = dat_data.split( "\n\n\n" )

	M = Series( dat_data[0] )
	M.info()
	S = Series( dat_data[1] )
	S.info()
	M.calc_path()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
else:
    print("Standalone program")
