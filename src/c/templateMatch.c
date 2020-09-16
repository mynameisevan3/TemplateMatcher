
/*=========================================
    colorSearch.c
    Color Search Function Implementations
  =========================================*/


// Inclusions
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colorSearch.h"


// Function Implementations

// colorSearchSerial - given an input image file as well as red, green, and blue color values and
//   percentage ranges to consider, this function searches the image for pixels that fall
//   within the given range and replaces them with additional passed in color values.
uint8_t colorSearchSerial( uint8_t *input, uint16_t width, uint16_t height, uint8_t *search,
                             float *percent, const uint8_t *replace                          ) {

  uint32_t size         = 0;            // Size of Image (Times Three for RGB)
  uint8_t  redVar       = 0;            // Red Variance
  uint8_t  greenVar     = 0;            // Green Variance
  uint8_t  blueVar      = 0;            // Blue Variance
  uint32_t i            = 0;            // Pixel Loop Iterator
  uint32_t distance     = 0;            // Euclidean Color Distance
  float    percentRange = 0.0;          // Euclidean Color Distance Percentage - 0 to 195075 => 0% to 100%
  uint8_t  j            = 0;            // Threshold Loop Iterator
  uint32_t count[3]     = { 0, 0, 0 };  // Number of Matching Pixels per Threshold

  // Determine Image Size - H * W
  size = height * width * 3;

  // Loop Through Image Checking Pixels
  for( i = 0; i < size; i += 3 ) {
    // Calculate Euclidean Color Distance
    redVar       = abs( input[ i + 0 ] - search[0] );  // Red Pixel at 0 Offset
    greenVar     = abs( input[ i + 1 ] - search[1] );  // Green Pixel at +1
    blueVar      = abs( input[ i + 2 ] - search[2] );  // Blue Pixel at +2
    distance     = redVar * redVar + greenVar * greenVar + blueVar * blueVar;
    // One Color Distance: Black -> Red = 255^2 = 65025
    // Maximum Distance: Black -> White = 255^2 * 3 = 195075

    // Get Percent Range - based on maximum color distance 195075, 255^2 * 3.
    percentRange = distance / 195075.0;

    // Consider three given thresholds.
    for( j = 0; j < 3; j++ ) {
      // If percent given and within threshold...
      if( ( percent[j] != 0.0 ) && ( percentRange <= percent[j] ) ) {
        // ...increment this threshold's counter for matching pixels, ...
        count[j]++;
        // ...set color to passed in value, ...
        input[ i + 0 ] = replace[ 3 * j + 0 ];
        input[ i + 1 ] = replace[ 3 * j + 1 ];
        input[ i + 2 ] = replace[ 3 * j + 2 ];
        // ...and break for loop to prevent threshold override.
        break;
      }
    }
  }

  // Sum Counts for Overlapping Thresholds
    // Say 5% and 10%, all those that meet 5% threshold will meet 10%.
  for( i = 1; i < 3; i++ ) {
    if( percent[i] != 0.0 ) {
      count[i] = count[ i - 1 ] + count[i];
    }
  }
  // Calculate and Return Similarity Percentage
  for( i = 0; i < 3; i++ ) {
    if( percent[i] != 0.0 ) {
      percent[i] = (double)count[i] * 100 / size;
    }
  }

  return 0;

}


// colorSearchOMP - same as above, parallelized with OpenMP.
uint8_t colorSearchOMP( uint8_t *input, uint16_t width, uint16_t height, uint8_t *search,
                             float *percent, const uint8_t *replace, uint8_t cores        ) {
  uint32_t size         = 0;            // Size of Image (Times Three for RGB)
  uint8_t  redVar       = 0;            // Red Variance
  uint8_t  greenVar     = 0;            // Green Variance
  uint8_t  blueVar      = 0;            // Blue Variance
  uint32_t i            = 0;            // Pixel Loop Iterator
  uint32_t distance     = 0;            // Euclidean Color Distance
  float    percentRange = 0.0;          // Euclidean Color Distance Percentage - 0 to 195075 => 0% to 100%
  uint8_t  j            = 0;            // Threshold Loop Iterator
  uint32_t count[3]     = { 0, 0, 0 };  // Number of Matching Pixels per Threshold

  // Determine Image Size - H * W
  size = height * width * 3;

  #pragma omp parallel for default( none ) num_threads( cores ) schedule( static ) \
            shared(  input, width, height, size, search, percent, count, replace ) \
            private( redVar, greenVar, blueVar, distance, i, j, percentRange )

  // Loop Through Image Checking Pixels
  for( i = 0; i < size; i += 3 ) {
    // Calculate Euclidean Color Distance
    redVar       = abs( input[ i + 0 ] - search[0] );  // Red Pixel at 0 Offset
    greenVar     = abs( input[ i + 1 ] - search[1] );  // Green Pixel at +1
    blueVar      = abs( input[ i + 2 ] - search[2] );  // Blue Pixel at +2
    distance     = redVar * redVar + greenVar * greenVar + blueVar * blueVar;
    // One Color Distance: Black -> Red = 255^2 = 65025
    // Maximum Distance: Black -> White = 255^2 * 3 = 195075

    // Get Percent Range - based on maximum color distance 195075, 255^2 * 3.
    percentRange = distance / 195075.0;

    // Consider three given thresholds.
    for( j = 0; j < 3; j++ ) {
      // If percent given and within threshold...
      if( ( percent[j] != 0.0 ) && ( percentRange <= percent[j] ) ) {
        // ...increment this threshold's counter for matching pixels, ...
        #pragma omp atomic
        count[j]++;
        // ...set color to passed in value, ...
        input[ i + 0 ] = replace[ 3 * j + 0 ];
        input[ i + 1 ] = replace[ 3 * j + 1 ];
        input[ i + 2 ] = replace[ 3 * j + 2 ];
        // ...and break for loop to prevent threshold override.
        break;
      }
    }
  }

  // Sum Counts for Overlapping Thresholds
    // Say 5% and 10%, all those that meet 5% threshold will meet 10%.
  for( i = 1; i < 3; i++ ) {
    if( percent[i] != 0.0 ) {
      count[i] = count[ i - 1 ] + count[i];
    }
  }
  // Calculate and Return Similarity Percentage
  for( i = 0; i < 3; i++ ) {
    if( percent[i] != 0.0 ) {
      percent[i] = (double)count[i] * 100 / size;
    }
  }

  return 0;

}



// END colorsearch.c  - EWG SDG
