
/*======================================
    main.c
    GKSuite Application Runner
  ======================================
    This template can be extended to
      many other apps.  Just change:
        1) Inclusions
        2) Number of Arguments
        3) App Name
        4) Usage and Argument Parsing
        5) Function Call
    This variant supports image input
      and output.  Other variants for
      output only, input only, dual-
      input single-output, etc. are
      available.
  ======================================*/


// Inclusions
#include <inttypes.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "colorSearch.h"
#include "ppmTools.h"


// Definitions
#define NUM_ARGS 9

// Main
int main( int argc, char **argv ) {

  // General
  const char    *name          = "Color Search";
        uint8_t  i             = 0;                  // Loop Iterator
        uint8_t  cores         = 0;                  // Number of Cores to Utilize
        uint8_t  display       = 0;                  // Argument to Display Debug Text
  // Image
        uint8_t *image         = NULL;               // Image Pointer - RGB888
  const char    *in            = NULL;               // Input Filename
  const char    *out           = NULL;               // Output Filename
        uint16_t width         = 0;                  // Image Width
        uint16_t height        = 0;                  // Image Height
  // Search Values
        uint8_t  search[3]     = {   0,   0,   0 };  // RGB Components of Color to Search
  // Replacement Colors
  const uint8_t  replace[9]    = { 255,   0,   0,    // First Range Replacement Color (RGB)
                                   255, 255,   0,    // Second Range Replacement Color
                                     0, 255,   0 };  // Third Range Replacement Color
  // Percentages and Ranges
        float    percent[3]    = {   0,   0,   0 };  // Ranges for Search and Return Values
  // Timing Components
        double   jobStart      = 0.0;                // Job Clock Start Time
        double   imageInStart  = 0.0;                // Image Input Start Time
        double   imageInEnd    = 0.0;                // Image Input End Time
        double   opStart       = 0.0;                // Operation Start Time
        double   opEnd         = 0.0;                // Operation End Time
        double   imageOutStart = 0.0;                // Image Output Start Time
        double   imageOutEnd   = 0.0;                // Image Output End Time
        double   jobEnd        = 0.0;                // Job Clock End Time
  // Final Time Values
        double   imageInTime   = 0.0;                // Image Input and Preparation Time
        double   opTime        = 0.0;                // Operation Execution Time
        double   imageOutTime  = 0.0;                // Image Output Time
        double   addOverhead   = 0.0;                // Additional Overhead Time
        double   jobTime       = 0.0;                // Total Program Time

  // Input Parsing and Error Handling
  if( ( argc >= ( NUM_ARGS ) ) && ( argc <= ( NUM_ARGS + 2 ) ) ) {
    // Parsing Normal Arguments
    in      = argv[1];
    out     = argv[2];
    cores   = atoi( argv[3] );
    display = atoi( argv[4] );
    if( sscanf( argv[5],"%" SCNu8, &search[0] ) != 1 ) {
      fprintf( stderr, "\nERROR: Issue with red argument.\n" );
      return 1;
    }
    if( sscanf( argv[6],"%" SCNu8, &search[1] ) != 1 ) {
      fprintf( stderr, "\nERROR: Issue with green argument.\n" );
      return 1;
    }
    if( sscanf( argv[7],"%" SCNu8, &search[2] ) != 1 ) {
      fprintf( stderr, "\nERROR: Issue with blue argument.\n" );
      return 1;
    }
    // Parsing Varied Number of Percent Range Arguments
      // Handle First One
    if( sscanf( argv[8],"%f", &percent[0] ) != 1 ) {
      fprintf( stderr, "\nERROR: Issue with first percent range argument.\n" );
      return 1;
    }
      // Handle Second One (One Additional Argument)
    if( argc == ( NUM_ARGS + 1 ) ) {
      if( sscanf( argv[9],"%f", &percent[1] ) != 1 ) {
        fprintf( stderr, "\nERROR: Issue with second percent range argument.\n" );
        return 1;
      }
      // Handle Third One (Two Additional Arguments)
    } else if ( argc == ( NUM_ARGS + 2 ) ) {
      if( sscanf( argv[9],"%f", &percent[1] ) != 1 ) {
        fprintf( stderr, "\nERROR: Issue with second percent range argument.\n" );
        return 1;
      }
      if( sscanf( argv[10],"%f", &percent[2] ) != 1 ) {
        fprintf( stderr, "\nERROR: Issue with third percent range argument.\n" );
        return 1;
      }
    }
    /*/ Check All Percent Ranges for Any Unreasonable Values
    for( size_t i = 0; i < rangesToRun; i++ ) {
      if( ( percentRange[i] > 0.9 ) || ( percentRange[i] <= 0.000001 ) ) {
        fprintf( stderr, "One or more percent ranges specified may be too large or small.\n" );
        return 1;
      }
    } //*/
    // Check to Ensure Percent Ranges Input in Increasing Magnitude
    if( !( ( percent[0] < percent[1] ) && ( percent[1] < percent[2] ) ) ) {
      fprintf( stderr, "\nWARNING: Percent ranges have not been input in increasing magnitude.\n" );
      //return 1;
    }
  // If Argument Number Does Not Fall Into Acceptable Range, Print Usage
  } else {
    fprintf( stderr, "Usage: %s <input> <output> <cores> <display> <red> <green> <blue> <percentRange> (<percentRangeB> <percentRangeC>)\n", argv[0] );
    fprintf( stderr, "  Input            - input .ppm file.\n" );
    fprintf( stderr, "  Output           - desired output filename.ppm.\n" );
    fprintf( stderr, "  Cores            - number of cores to utilize for parallel operation\n" );
    fprintf( stderr, "                       or zero for a serial baseline run.\n" );
    fprintf( stderr, "  Display          - 0 for timing, 1 for full report, 2 for percents.\n" );
    fprintf( stderr, "  Red              - red component value of desired search color.\n" );
    fprintf( stderr, "  Green            - green component value of desired search color.\n" );
    fprintf( stderr, "  Blue             - blue component value of desired search color.\n" );
    fprintf( stderr, "  Percent Range(s) - allowed percent difference (+/-) off RGB.\n" );
    fprintf( stderr, "                     Specify up to three in increasing magnitude.\n" );
    fprintf( stderr, "Example:  %s input.ppm output.ppm 4 1 255 255 255 0.05 0.1 0.15\n\n", argv[0] );
    return 0;
  }

  // Intro Message
  if( display == 1 ) {
    fprintf( stdout, "\n   = = =  %s  = = =   \n\n", name );
  }

  // OpenMP Test
  if( display == 1 ) {
    if( cores != 0 ) {
      fprintf( stdout, "Using %d Cores of Maximum %d Cores Available\nTesting - Report\n", cores, omp_get_max_threads( ) );
      #pragma omp parallel for num_threads( cores )
      for( i = 0; i < omp_get_num_threads( ); i++ ) {
        fprintf( stdout, "  Core %d of %d Reporting!\n", omp_get_thread_num( ), omp_get_num_threads( ) );
      }
    } else {
      fprintf( stdout, "Running Serial Baseline\n" );
    }
  }

  // Job Start Timer
  jobStart     = omp_get_wtime( );

  // Image Allocation
  if( display == 1 ) {
    fprintf( stdout, "Initializing Image...\n" );
  }
  imageInStart = omp_get_wtime( );
  image = readPPM( in, &width, &height );
  if( !image ) {
    fprintf( stderr, "Failed to Initialize Image\n" );
    return 1;
  }
  imageInEnd   = omp_get_wtime( );

  // Application Operation
  if( display == 1 ) {
    fprintf( stdout, "Performing %s Operation...\n", name );
  }
  if( cores == 0 ) {
    opStart      = omp_get_wtime( );
    colorSearchSerial( image, width, height, search, percent, replace );
    opEnd      = omp_get_wtime( );
  } else {
    opStart = omp_get_wtime( );
    colorSearchOMP( image, width, height, search, percent, replace, cores );
    opEnd   = omp_get_wtime( );
  }

  // Display Match Percentage and Given Criteria
  if( display == 1 ) {
    fprintf( stdout, "\nFor File %s, Criteria Specified:  R%" SCNu8 " G%" SCNu8 " B%" SCNu8 "\n",
               in, search[0], search[1], search[2]                                                );
    for( i = 0; i < 3; i++ ) {
      if( percent[i] != 0.0 ) {
        fprintf( stdout, "  Percent Range %d - Matches %f%% of Image\n",
                   (int)( i + 1 ), percent[i]                           );
      }
    }
    fprintf( stdout, "\n" );
  }

  // Image Write
  if( display == 1 ) {
    fprintf( stdout, "Writing Image...\n" );
  }
  imageOutStart = omp_get_wtime( );
  writePPM( image, out, width, height );
  imageOutEnd   = omp_get_wtime( );

  // Final Tasks
  free( image );
  if( display == 1 ) {
    fprintf( stdout, "Operation Complete!\n\n" );
  }

  // Final Timing Component
  jobEnd       = omp_get_wtime( );                               // End Full Program Clock
  imageInTime  = imageInEnd  - imageInStart;                     // Calculate Image Input and Preparation Time
  opTime       = opEnd       - opStart;                          // Calculate Operation Time
  imageOutTime = imageOutEnd - imageOutStart;                    // Calculate Image Output Time
  jobTime      = jobEnd      - jobStart;                         // Calculate Overall Time
  addOverhead  = jobTime - imageInTime - opTime - imageOutTime;  // Calculate Additional Overhead Time
  // Handle Displaying All Timing Data
  if( display == 1 ) {  // Messages only when display is 1.
    fprintf( stdout, "=== Timing Data ===\n  Image In:\t\t" );
    fprintf( stdout, "%0.7lf\n", imageInTime );                                                                                                                  fprintf( stdout, "  Operation:\t\t" );
  }
  if( display <= 1 ) {  // Gives Timing Regardless
    fprintf( stdout, "%0.7lf ", opTime );
  }
  if( display == 1 ) {
    fprintf( stdout, "\n  Image Out:\t\t" );
    fprintf( stdout, "%0.7lf\n", imageOutTime );
    fprintf( stdout, "  Overhead:\t\t" );
    fprintf( stdout, "%0.7lf\n", addOverhead );
    fprintf( stdout, "  Total Job Time:\t" );
    fprintf( stdout, "%0.7lf\n\n", jobTime );
  // Or Just Display Resultant Percentages
  } else if( display == 2 ) {
    fprintf( stdout, "Percent Match: %f%%", percent[0] );
    for( i = 1; i < 3; i++ ) {
      if( percent[i] != 0.0 ) {
        fprintf( stdout, " %f%%", percent[0] );
      }
    }
    fprintf( stdout, "\n" );
  }
  // Define Additional Output Functions with Different Display Values

  return 0;

}



// END main.c  - EWG SDG
