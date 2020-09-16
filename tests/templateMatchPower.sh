#!/bin/sh
##########################################
# Automation of Power Measure Collection #
##########################################

# Variable Definitions
execName=colorSearch
longName="Color Search"
imageIn=data/LA.ppm
imageOut=cs.ppm
coreStart=1
coreEnd=4
runs=1000

# Header
echo $longName "Power Measure Run"
# No File Generation Here - Just for Power Measures

# Make Program
make clean
make
echo

# Initial Test Run - shows which values are which.
./$execName $imageIn 4$imageOut 4 1 255 255 255 0.05 0.1 0.15

# Automated Run Loop with Varying Core Utilization
for cores in `seq $coreStart $coreEnd`
do
  echo " "
  echo "Running with $cores Cores..."
  iteration=0
  while [ $iteration -lt $runs ]
  do
    # Remove Previous Output Images
    rm *$imageOut

    # Run Application
    p=$(./$execName $imageIn $cores$imageOut $cores 0 255 255 255 0.05 0.1 0.15)

    # Indicate Status
    echo "Iteration $iteration $p"

    # Iterate Loop
    iteration=$((iteration+1))
  done
done



# End *Power.sh  - EWG SDG
