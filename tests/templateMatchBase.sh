#!/bin/sh
#################################################
# Automation of Serial Baseline Data Collection #
#################################################

# Variable Definitions
execName=colorSearch
longName="Color Search"
outFile=colorSearchBase.txt
imageIn=data/LA.ppm
imageOut=cs.ppm
runs=1000

# Header for File
echo                                                                 > $outFile
echo $longName "Serial Baseline Run"                                >> $outFile
echo                                                                >> $outFile

# Make Program
make clean                                                          >> $outFile
make                                                                >> $outFile
echo                                                                >> $outFile

# Initial Test Run - shows which values are which.
./$execName $imageIn $imageOut 0 1 255 255 255 0.05 0.1 0.15        >> $outFile
echo                                                                >> $outFile

# Job Start Time
echo -n "Job Start "                                                >> $outFile
date                                                                >> $outFile

# Automated Run Loop with Varying Core Utilization
iteration=0
while [ $iteration -lt $runs ]
do
  # Remove Previous Output Images
  rm $imageOut

  # Collect Data
  s=$(./$execName $imageIn $imageOut 0 0 255 255 255 0.05 0.1 0.15) >> $outFile
  echo "$iteration $s"                                              >> $outFile

  # Indicate Status
  echo "Iteration $iteration $s"

  # Iterate Loop
  iteration=$((iteration+1))
done

# Job End Time
echo -n "Job Completion "                                           >> $outFile
date                                                                >> $outFile
echo                                                                >> $outFile

# Can now grab newline-delimited data from output text file!



# End *Base.sh  - EWG SDG
