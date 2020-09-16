
/*==============================================
    templateMatch.c
    Template Matching Function Implementations
  ==============================================*/


// Inclusions
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "templateMatch.h"


// Function Implementations

// templateMatchSerial - given pointers to input and template RGB888 images, will perform template
//   matching and return the maximum match percentage and location X and Y coordinates as well as
//   highlight the best template match area in the input image.
// TODO: Use 128-bit integers to attempt normalized cross-correlation.
// TODO: Consider alternatively returning match percentages in input image.
uint8_t templateMatchSerial( float *match, uint16_t *matchX, uint16_t *matchY, uint8_t *image,
                               uint16_t imageWidth, uint16_t imageHeight, uint8_t *temp,
                               uint16_t tempWidth, uint16_t tempHeight ) {

  const uint32_t imageSize = imageWidth * imageHeight;  // Size of Image
  const uint32_t tempSize  = tempWidth  * tempHeight;   // Size of Template
        uint16_t i         = 0;                         // Image Y Loop Iterator
        uint16_t j         = 0;                         // Image X Loop Iterator
        uint16_t x         = 0;                         // Image X Loop Iterator
        uint16_t y         = 0;                         // Image Y Loop Iterator
        uint64_t corrRed   = 0;                         // Correlation of Red Values
        uint64_t corrGreen = 0;                         // Correlation of Green Values
        uint64_t corrBlue  = 0;                         // Correlation of Blue Values
        uint64_t correlate = 0;                         // Combined Correlation Value
        float    fitCorr   = 0.0;                       // Correlation Fit to Maximum Value Here

  // Max Value for Correlation Given 8K Image: 8192^2 * 255^2 = 4,363,753,881,600 => 64
  // Max Value for Normalization: ( ( 255 )^2 * 8192^2 )^2 = 1.90e25 => 128
  // This is why it's not normalized...

  // Iterate Whole Process Over Image X and Y Coordinates (Remember * 3 for RGB888)
  for( i = 0; i < imageHeight; i++ ) {
    for( j = 0; j < imageWidth; j++ ) {

      // Iterate Over Template X and Y Coordinates (Remember * 3 for RGB888)
      for( y = 0; y < tempHeight; y++ ) {
        for( x = 0; x < tempWidth; x++ ) {

          // Add to Correlation Values for Each Color
          corrRed   += 
          corrGreen += 
          corrBlue  += 

        } // End Template Y
      } // End Template X

      // Combine Correlation by Color
      correlate = 0.2126 * corrRed + 0.7152 * corrGreen + 0.0722 * corrBlue;

      // Normalize Based on Maximum Possible Correlation Value for These Sizes
      

      // Store if Greater than Current Maximum


    } // End Image Y
  } // End Image X

  return 0;

}


// templateMatchOMP - same as above, parallelized with OpenMP.
uint8_t templateMatchOMP( float *match, uint16_t *matchX, uint16_t *matchY, uint8_t *image,
                            uint16_t imageWidth, uint16_t imageHeight, uint8_t *temp,
                            uint16_t tempWidth, uint16_t tempHeight, uint8_t cores ) {

  return 0;

}



// END templateMatch.c  - EWG SDG
