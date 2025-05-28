#!/usr/bin/python3

# dat2csv
# read a "dat" file (formatted for gnuplot" and write out as CSV
# Different series are side by side

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

def dataslurp(dat_name=""):
	# Possibly request file (if not specified on command line) and read it in
	global CSVfile
	
	if dat_name == "":
		# Use Tk file dialog
		root = tk.Tk()
		root.withdraw()
		csv_name = filedialog.askopenfilename(title="Enter the DAT file to parse:",filetypes=(
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

def csvize( data ):
	return "\n".join( [ ",".join( [ data[s][i] for s in range(len(data)) ] ) for i in range(len(data[0])) ] )

def main( sysargs ):
	if len(sysargs) > 1:
		data = dataslurp(sysargs[1])
	else :
		data = dataslurp()

	data = [ series_parse( column ) for column in data.split( "\n\n\n" ) ]
	
	with open( CSVfile, "w" ) as f:
		f.write(csvize(data))

if __name__ == "__main__":
    sys.exit(main(sys.argv))
else:
    print("Standalone program")
