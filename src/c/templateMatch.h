
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
uint8_t templateMatchSerial( float *match, uint16_t *matchX, uint16_t *matchY, uint8_t *image,
                               uint16_t imageWidth, uint16_t imageHeight, uint8_t *temp,
                               uint16_t tempWidth, uint16_t tempHeight );
uint8_t templateMatchOMP( float *match, uint16_t *matchX, uint16_t *matchY, uint8_t *image,
                            uint16_t imageWidth, uint16_t imageHeight, uint8_t *temp,
                            uint16_t tempWidth, uint16_t tempHeight, uint8_t cores );


// End Include Guard
#endif



// END colorSearch.h  - EWG SDG
