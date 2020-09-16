
/*=================================
    colorSearch.h
    Color Search Function Headers
  =================================*/


// Include Guard
#ifndef COLORSEARCH_H
#define COLORSEARCH_H


// Inclusions
#include <stdint.h>


// Function Prototypes
uint8_t colorSearchSerial( uint8_t *input, uint16_t width, uint16_t height, uint8_t *search,
                             float *percent, const uint8_t *replace                          );
uint8_t colorSearchOMP( uint8_t *input, uint16_t width, uint16_t height, uint8_t *search,
                             float *percent, const uint8_t *replace, uint8_t cores        );


// End Include Guard
#endif



// END colorSearch.h  - EWG SDG
