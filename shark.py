#!/usr/bin/python3

# shark.py
# read a "dat" file (formatted for gnuplot" and show summary statistics

import tkinter as tk
from tkinter import filedialog

import sys
import io
from pathlib import Path

CSVfile = "dummy.csv"

def vformat(vals) :
	match len(vals):
		case 0:
			return "\"\",\"\""
		case 1:
			return "\"{}_x\",\"{}_y\"".format(vals[0],vals[0])
		case 2:
			return "{},{}".format(vals[0],vals[1])

def dat_slurp(dat_name=""):
	# Possibly request file (if not specified on command line) and read it in
	global CSVfile
	
	if dat_name == "":
		# Use Tk file dialog
		root = tk.Tk()
		root.withdraw()
		dat_name = filedialog.askopenfilename(title="Enter the DAT file to parse:",filetypes=(
			("DAT files","*.DAT"),
			("dat files","*.dat"),
			("Text file","*.txt"),
			("Text file","*.TXT"),
			("All files","*"),
			("All files","*.*"),)
			)
		root.destroy()

	CSVfile = Path(dat_name).with_suffix(".csv")
	
	try: 
		with open(dat_name,"r") as dat:
			return dat.read()
	except:
		print(f"Unable to read {dat_name}\n") 
		sys.exit(1)
		
def series_parse( data ):
	return [ vformat( line.split() ) for line in data.strip().split("\n") ] 

def join_series( data ):
	return "\n".join( [ ",".join( [ data[s][i] for s in range(len(data)) ] ) for i in range(len(data[0])) ] )

def CSVwrite( dat_data ):
	with open( CSVfile, "w" ) as f:
		f.write(join_series(dat_data))

def series_info( dat_data ): 
	print( dat_data ) 

def main( sysargs ):
	if len(sysargs) > 1:
		dat_data = dat_slurp(sysargs[1])
	else :
		dat_data = dat_slurp()

	dat_data = [ series_parse( column ) for column in dat_data.split( "\n\n\n" ) ]
	
	series_info( dat_data )

if __name__ == "__main__":
    sys.exit(main(sys.argv))
else:
    print("Standalone program")
