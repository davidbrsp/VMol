/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (HEADER) PROGRAMA PRINCIPAL                                                                        **/
/************************************************************************************************************/

#ifndef VMOL_H
#define VMOL_H

#include "tk.h"
#include "imagem.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

//#include "gp.h"
//#include "prim.h"
//#include "shade.h"
//#include "rt.h"
//#include "scene.h"
//#include "image.h"
//#include "defs.h"
//#include "geom.h"
//#include "stack.h"
//#include "view.h"
//#include "poly.h"
//#include "hier.h"
//#include "lang.h"
//#include "clip.h"
//#include "raster.h"
//#include "ray.h"
//#include "csg.h"
//#include "laux.h"
//#include "mv.h"

#define pause( ) { printf ( "Press ENTER to continue...\n" ); getchar( ); }
#define panic( m ) { fprintf( stderr, "%s!\n", m ); pause( );  exit( EXIT_FAILURE ); }
#define MAX_NUM_WIDGET 90
//#define NUM_MOLECULAS 7
#define NUM_BOTOES 5
#define MAX_BROWSER_NAME_LEN 20
#define MAX_MOLECULAS_NAME_LEN 20
#define MAX_NUM_ATOMOS 100
#define MOL_LIST "LIST.txt"

void* mallocX( size_t nbytes );
FILE* fopenX( const char* filename, const char* mode );

#endif
