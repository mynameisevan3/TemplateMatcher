
/*======================================
    templateMatch.h
    Template Matching Function Headers
  ======================================*/


// Include Guard
#ifndef TEMPLATEMATCH_H
#define TEMPLATEMATCH_H


// Inclusions
#include <stdint.h>


// Function Prototypes
uint8_t templateMatchSerial( float *match, uint16_t *matchX, uint16_t *matchY,
                               const uint8_t *image, const uint16_t imageWidth,
                               const uint16_t imageHeight, const uint8_t *temp,
                               const uint16_t tempWidth, const uint16_t tempHeight );
uint8_t templateMatchOMP( float *match, uint16_t *matchX, uint16_t *matchY,
                            const uint8_t *image, const uint16_t imageWidth,
                            const uint16_t imageHeight, const uint8_t *temp,
                            const uint16_t tempWidth, const uint16_t tempHeight,
                            const uint8_t cores                                  );


// End Include Guard
#endif



// END templateMatch.h  - EWG SDG
