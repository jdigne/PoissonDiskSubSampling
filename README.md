# Poisson Disk subsampling

Author: Julie Digne 2013-2021
julie.digne@liris.cnrs.fr

## Disclaimer

This code is a simple implementation of Poisson Disk Subsampling.

## License

This code is released under the GNU GPL v3 License. See the attached LICENSE file.

## Build instructions

The build process relies on cmake/make which should be installed. Tested on Ubuntu 16.04, 18.04, 20.04

1) Clone the project
2) cd PoissonDiskSubSampling
3) mkdir build
4) cd build
5) cmake -DCMAKE_BUILD_TYPE=Release ..
6) make

This project has no dependency.

##Usage

pdss -i input_file -o output -r radius 

By default the output file is saved in OFF format as FILENAME_seeds.off, use the optional -a option to save in ascii directly (FILENAME_seeds.asc).

NOTE: every file containing oriented points is formatted as:
x1	y1	z1	nx1	ny1	nz1
x2	y2	z2	nx2	ny2	nz2
.	.	.	.	.	.
.	.	.	.	.	.
xn	yn	zn	nxn	nyn	nzn

