
/*====================================
            P P M   T O O L S
    Loading and Changing .ppm Images
  ====================================*/


// Include Guard
#ifndef PPMTOOLS_H
#define PPMTOOLS_H


// Inclusions
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


// Function Prototypes
uint8_t* readPPM( const char *filename, uint16_t *width, uint16_t *height );
void     writePPM( uint8_t *image, const char *filename, uint16_t width, uint16_t height );


// End Include Guard
#endif



// End ppmTools.h  - EWG SDG
