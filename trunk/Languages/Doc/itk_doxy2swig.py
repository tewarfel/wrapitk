#!/usr/bin/env python
"""Doxygen XML to SWIG docstring converter for ITK classes.

Usage:

  itk_doxy2swig.py [input directory name] [output swing interface file name]

"""

import sys
import glob
from doxy2swig import *

def d2s_dir(in_dir_name, out_swig_i):
	fpattern = in_dir_name + "/class*.xml"
	xml_file_names = glob.glob(fpattern)
	f = open(out_swig_i, 'w')
	for xfn in xml_file_names:
		print("-- Doxygen to SWIG:" + xfn)
		d2s = Doxy2SWIG(xfn)
		d2s.generate()
		d2s.write(out_swig_i, 'a+')
        else:
            f.close()

def main(in_dir_name, out_swig_i):
	d2s_dir(in_dir_name, out_swig_i)

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print __doc__
		sys.exit(1)
	main(sys.argv[1], sys.argv[2])
