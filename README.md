
# TemplateMatcher

This repository contains a simple C template matching application parallelized with OpenMP.  It uses normalized cross correlation to find matches in a given image to a given template.


## Build

This application can be built by simply running the ```make``` command from within this directory.


## Usage

The template matching application can be run using the following command.

```./templateMatch <input> <template> <output> <cores> <display>```

The parameters that follow apply.

+ Input    - a .ppm image upon which to perform the operation.
+ Template - a .ppm image to use as a template.
+ Output   - a .ppm image to output.
+ Cores    - the number of cores to utilize for this parallel operation.
+ Display  - 0 for timing, 1 for full report, and 2 for best match.

Example:  ```./templateMatch input.ppm template.ppm output.ppm 4 1```


## Conversion

One may convert to and from .ppm images using the Unix terminal command:

```convert <filenameIn>.<extensionIn> <filenameOut>.<extensionOut>```


## Automated Data Acquisition

Scripts designed to automate the process of acquiring execution time and power data on the facsimiles are included.  The scripts default to one thousand runs and parallelize to four cores.

For parallel execution data, run the included ```./tests/templateMatchRun.sh```.  It will build and run the program and output all data to a text file.

For the serial baseline, run the included ```./tests/templateMatchBase.sh```.  It will build and run the program and output all serial baseline timings to a text file.

For power measures, run the included ```./tests/templateMatchPower.sh```.  It will run the iterations sequentially for different numbers of cores for more stable per-core power measures.
