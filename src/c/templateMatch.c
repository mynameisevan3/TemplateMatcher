
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
//   matching and return the maximum match percentage and location X and Y coordinates.
// TODO: Consider alternatively returning match levels in input image.
uint8_t templateMatchSerial( float *match, uint16_t *matchX, uint16_t *matchY,
                               const uint8_t *image, const uint16_t imageWidth,
                               const uint16_t imageHeight, const uint8_t *temp,
                               const uint16_t tempWidth, const uint16_t tempHeight ) {

  const uint32_t tempSize      = tempWidth  * tempHeight;       // Size of Template
  const uint32_t maxDiff       = tempSize   * 256 * 3;          // Determine Maximum Difference Value
  const uint16_t maxXRange     = imageWidth  - tempWidth + 1;   // Maximum X Range of Image to Check Template
  const uint16_t maxYRange     = imageHeight - tempHeight + 1;  // Maximum Y Range of Image to Check Template
  const uint16_t maxXRange3    = maxXRange  * 3;                // Maximum X Range Times 3 for RGB888 Iteration
  const uint16_t imageWidth3   = imageWidth * 3;                // Image Width Times 3 for RGB888 Iteration
  const uint16_t tempWidth3    = tempWidth  * 3;                // Template Width Times 3 for RGB888 Iteration
        uint8_t  color         = 0;                             // Image RGB888 Color Iterator
        uint16_t i             = 0;                             // Image Y Loop Iterator
        uint16_t j             = 0;                             // Image X Loop Iterator
        uint16_t m             = 0;                             // Template/Image Overlap X Loop Iterator
        uint16_t n             = 0;                             // Template/Image Overlap Y Loop Iterator
        uint16_t x             = 0;                             // Template X Loop Iterator
        uint16_t y             = 0;                             // Template Y Loop Iterator
        uint32_t correlate     = 0;                             // Combined Correlation Value for this Template Location
        uint32_t minCorr       = 4294967295;                    // Minimum Correlation Value for All Running Template Overlaps

  // Max Value for Correlation Given 8K Image: 8192^2 * 255^2 = 4,363,753,881,600 => 64
  // Max Value for Normalization: ( ( 255 )^2 * 8192^2 )^2 = 1.90e25 => 128
  // This is why it's not normalized...
  // From Perspective of the Template, Determine Maximum Template Size
  //   For 2048px Template: 2048 x 2048 x 256 x 3 = 3,221,225,472 Maximum Difference
  //   For 1024px Template: 1024 x 1024 x 256 x 3 =   805,306,368 Maximum Difference
  //   For 512px Template:   512 x  512 x 256 x 3 =   201,326,592 Maximum Difference
  //     Much higher than a 2048px square template could overload a 32-bit integer,
  //       but I think we can safely assume that's good enough.

//fprintf( stdout, "Checking Template (%d, %d) in Image (%d, %d) to Range (%d, %d)\n",
//           tempWidth, tempHeight, imageWidth, imageHeight, maxXRange, maxYRange      );

  // Iterate Whole Process Over Image X and Y Coordinates (Remember * 3 for RGB888)
  for( j = 0; j < maxYRange; j++ ) {
    for( i = 0; i < maxXRange3; i += 3 ) {

      // Zero Correlation for this Image/Template Overlap
      correlate = 0;

      // Iterate Over Template X and Y Coordinates (Remember * 3 for RGB888)
      for( y = 0; y < tempHeight; y++ ) {
        for( x = 0; x < tempWidth3; x += 3 ) {

          // Set Parallel Indices for Base Image
          n = j + y;
          m = i + x;

//fprintf( stdout, "Template (%d, %d) RGB (%d, %d, %d)\n", x, y,
//           temp[ y * tempWidth3 + x + 0 ],
//           temp[ y * tempWidth3 + x + 1 ],
//           temp[ y * tempWidth3 + x + 2 ]  );

          // Iterate Over RGB888 Pixel Colors
          for( color = 0; color < 3; color++ ) {

            // Add to Running Correlation Value from All Colors
            correlate += abs( temp[ y * tempWidth3 + x + color ] - image[ n * imageWidth3 + m + color ] );

//fprintf( stdout, "  Check: abs( %d - %d ) += %d\n",
//           temp[ y * tempWidth3 + x + color ],
//           image[ n * imageWidth3 + m + color ],
//           correlate );

          }  // End Color

        }  // End Template C
      }  // End Template Y


//fprintf( stdout, "Correlation %d at (%d, %d)", correlate, i / 3, j );
      // Store New Minimum and Position if Difference Less than Current Minimum
      if( correlate < minCorr ) {
        minCorr = correlate;
        *matchX = i / 3;
        *matchY = j;
//fprintf( stdout, " - New Minimum!" );
      }
//fprintf( stdout, "\n" );

    } // End Image X
  } // End Image Y

  // Calculate Best Match Percentage
  *match = ( 1.0 - ( minCorr / maxDiff ) ) * 100.0;

  return 0;

}


// templateMatchOMP - same as above, parallelized with OpenMP.
uint8_t templateMatchOMP( float *match, uint16_t *matchX, uint16_t *matchY,
                            const uint8_t *image, const uint16_t imageWidth,
                            const uint16_t imageHeight, const uint8_t *temp,
                            const uint16_t tempWidth, const uint16_t tempHeight,
                            const uint8_t cores                                  ) {

  const uint32_t tempSize      = tempWidth  * tempHeight;       // Size of Template
  const uint32_t maxDiff       = tempSize   * 256 * 3;          // Determine Maximum Difference Value
  const uint16_t maxXRange     = imageWidth  - tempWidth + 1;   // Maximum X Range of Image to Check Template
  const uint16_t maxYRange     = imageHeight - tempHeight + 1;  // Maximum Y Range of Image to Check Template
  const uint16_t maxXRange3    = maxXRange  * 3;                // Maximum X Range Times 3 for RGB888 Iteration
  const uint16_t imageWidth3   = imageWidth * 3;                // Image Width Times 3 for RGB888 Iteration
  const uint16_t tempWidth3    = tempWidth  * 3;                // Template Width Times 3 for RGB888 Iteration
        uint8_t  color         = 0;                             // Image RGB888 Color Iterator
        uint16_t i             = 0;                             // Image Y Loop Iterator
        uint16_t j             = 0;                             // Image X Loop Iterator
        uint16_t m             = 0;                             // Template/Image Overlap X Loop Iterator
        uint16_t n             = 0;                             // Template/Image Overlap Y Loop Iterator
        uint16_t x             = 0;                             // Template X Loop Iterator
        uint16_t y             = 0;                             // Template Y Loop Iterator
        uint32_t correlate     = 0;                             // Combined Correlation Value for this Template Location
        uint32_t minCorr       = 4294967295;                    // Minimum Correlation Value for All Running Template Overlaps

  #pragma omp parallel for default( none ) num_threads( cores ) schedule( static ) \
          shared( image, matchX, matchY, minCorr, temp ) \
          private( correlate, color, i, j, m, n, x, y )

  // Iterate Whole Process Over Image X and Y Coordinates (Remember * 3 for RGB888)
  for( j = 0; j < maxYRange; j++ ) {
    for( i = 0; i < maxXRange3; i += 3 ) {
      // Zero Correlation for this Image/Template Overlap
      correlate = 0;
      // Iterate Over Template X and Y Coordinates (Remember * 3 for RGB888)
      for( y = 0; y < tempHeight; y++ ) {
        for( x = 0; x < tempWidth3; x += 3 ) {
          // Set Parallel Indices for Base Image
          n = j + y;
          m = i + x;
          for( color = 0; color < 3; color++ ) {
            // Add to Running Correlation Value from All Colors
            correlate += abs( temp[ y * tempWidth3 + x + color ] - image[ n * imageWidth3 + m + color ] );
          }  // End Color
        }  // End Template C
      }  // End Template Y
      if( correlate < minCorr ) {
        #pragma omp critical( minCorr )
        {
          minCorr = correlate;
          *matchX = i / 3;
          *matchY = j;
        }
      }
    }  // End Image X
  }  // End Image Y

  // Calculate Best Match Percentage
  *match = ( 1.0 - ( minCorr / maxDiff ) ) * 100.0;

  return 0;

}



// END templateMatch.c  - EWG SDG
