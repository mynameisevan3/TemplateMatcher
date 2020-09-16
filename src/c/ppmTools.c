
/*====================================
            P P M   T O O L S
    Loading and Changing .ppm Images
  ====================================*/


// Inclusions
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ppmTools.h"


// Definitions
#define MAXCOLOR8 255


// Function Implementations

// readPPM - read a .ppm image file named filename and output a pointer to
//   that image.
uint8_t* readPPM( const char *filename, uint16_t *width, uint16_t *height ) {

  uint8_t *image;
  char     buffer[16];
  int32_t  c;
  int32_t  compdepth;
  FILE    *fp;

  // Open PPM File (Read)
  fp = fopen( filename, "rb" );
  if( fp == NULL ) {
    fprintf( stderr, "Unable to open file %s in readPPM.\n", filename );
    exit( 1 );
  }

  // Read First Line
  if( fgets( buffer, sizeof( buffer ), fp ) == NULL ) {
    fprintf( stderr, "ReadPPM - image file empty.\n" );
    exit( 1 );
  }

  // Check the Image Format
  if( ( buffer[0] != 'P' ) || ( ( buffer[1] != '5' ) && ( buffer[1] != '6' ) ) ) {
    fprintf( stderr, "ReadPPM - invalid image format, must be P5 or P6 PGM/PPM image.\n");
    exit( 1 );
  }

  // Comments Check - May Need to Include Elsewhere
  c = getc( fp );
  while( c == '#' ) {
    while( getc( fp ) != '\n' ) {
      c = getc( fp );
    }
  }
  ungetc( c, fp );

  // Read Image Size Information
  if( fscanf( fp, "%hu %hu", width, height ) != 2 ) {
    fprintf( stderr, "ReadPPM - invalid image size, error loading '%s'.\n", filename );
    exit( 1 );
  }

  // Read and Check Component Depth
  if( fscanf( fp, "%d", &compdepth ) != 1 ) {
    fprintf( stderr, "ReadPPM - invalid component depth, error loading '%s'.\n", filename );
    exit( 1 );
  }
  if( compdepth != MAXCOLOR8 ) {
    fprintf( stderr, "ReadPPM - '%s' does not have %d max component value.\n", filename, MAXCOLOR8 );
    exit( 1 );
  }
  while( fgetc( fp ) != '\n' );

  // Allocate Memory for Image
  image = malloc( ( *width ) * ( *height ) * 3 * ( sizeof( uint8_t ) ) );
  if( image == NULL ) {
    fprintf( stderr, "ReadPPM - unable to allocate memory for Image struct.\n" );
    exit( 1 );
  }

  // Read Pixel Data from File
  // TODO: Experiment with different fread argument ratios for potential speedup.
  if( fread( image, sizeof( uint8_t ), ( *width ) * ( *height ) * 3, fp )
      // Handle early ending by fread return value.
      != (unsigned)( ( *width ) * ( *height ) * 3 ) ) {
    fprintf( stderr, "ReadPPM - end of file '%s' reached early.\n", filename );
  }

  fclose( fp );
  return image;

}


// writePPM - write out the image given by the output pointer to the
//   filename specified.
void writePPM( uint8_t *image, const char *filename, uint16_t width, uint16_t height ) {

  FILE *fp;

  // Create/Overwrite File Named <filename>
  fp = fopen( filename, "wb" );
  if( !fp ) {
    fprintf( stderr, "WritePPM - unable to open file '%s'.\n", filename );
    exit( 1 );
  }

  // Write the Header File
  fprintf( fp, "P6\n" );
  fprintf( fp, "%hu %hu \n", width, height );
  fprintf( fp, "%d\n", MAXCOLOR8 );
  fwrite( image, width * 3, height, fp );

  fclose( fp );

}


// End ppmTools.c  - EWG SDG
