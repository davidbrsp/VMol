/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (HEADER) FUNÇÕES RELACIONADAS A MOVIMENTO DA CÂMERA, ZOOM ...                                      **/
/************************************************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "tk.h"

/*#include "gp.h"
#include "image.h"
#include "defs.h"
#include "geom.h"
#include "stack.h"
#include "view.h"
#include "poly.h"
#include "prim.h"
#include "hier.h"
#include "lang.h"
#include "clip.h"
#include "raster.h"
#include "shade.h"
#include "ray.h"
#include "csg.h"
#include "rt.h"
#include "scene.h"*/

#define ZOOM_FACTOR 4
#define ANGULO_ROTACAO PI/4

typedef struct _camera_
{
   Vector3 center;
   Vector3 up;
   Vector3 normal;
}
CAMERA;

/* Prototipos de funcoes */

CAMERA pega_camera( void );
  void muda_camera( CAMERA c );
   int norte( void );
   int sul( void );
   int leste( void );
   int oeste( void );
   int nordeste( void );
   int sudeste( void );
   int noroeste( void );
   int sudoeste( void );
   int centro( void );
   int zoomIn( void );
   int zoomOut( void );

#endif
