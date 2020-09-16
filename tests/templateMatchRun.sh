#!/bin/sh
####################################################
# Automation of Parallel Execution Data Collection #
####################################################

# Variable Definitions
execName=colorSearch
longName="Color Search"
outFile=colorSearchOut.txt
imageIn=data/LA.ppm
imageOut=cs.ppm
runs=1000

# Header for File
echo                                                                  > $outFile
echo $longName "Data Acquisition Run"                                >> $outFile
echo                                                                 >> $outFile

# Make Program
make clean                                                           >> $outFile
make                                                                 >> $outFile
echo                                                                 >> $outFile

# Initial Test Run - shows which values are which.
./$execName $imageIn 4$imageOut 4 1 255 255 255 0.05 0.1 0.15        >> $outFile
echo                                                                 >> $outFile

# Job Start Time
echo -n "Job Start "                                                 >> $outFile
date                                                                 >> $outFile

# Automated Run Loop with Varying Core Utilization
iteration=0
while [ $iteration -lt $runs ]
do
  # Remove Previous Output Images
  rm *$imageOut

  # Collect Data
  a=$(./$execName $imageIn 1$imageOut 1 0 255 255 255 0.05 0.1 0.15) >> $outFile
  b=$(./$execName $imageIn 2$imageOut 2 0 255 255 255 0.05 0.1 0.15) >> $outFile
  c=$(./$execName $imageIn 3$imageOut 3 0 255 255 255 0.05 0.1 0.15) >> $outFile
  d=$(./$execName $imageIn 4$imageOut 4 0 255 255 255 0.05 0.1 0.15) >> $outFile
  echo "$iteration $a $b $c $d"                                      >> $outFile

  # Indicate Status
  echo "Iteration $iteration $a $b $c $d"

  # Iterate Loop
  iteration=$((iteration+1))
done

# Job End Time
echo -n "Job Completion "                                            >> $outFile
date                                                                 >> $outFile
echo                                                                 >> $outFile

# Can now grab newline-delimited data from output text file!



# End *Run*.sh  - EWG SDG
