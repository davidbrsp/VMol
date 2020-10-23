/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (HEADER) FUNÇÕES RELACIONADAS A RENDERIZAÇÃO, IMAGEAMENTO, CENAS ...                               **/
/************************************************************************************************************/

#ifndef IMAGEM_H
#define IMAGEM_H

/************************************************************************************************************/

#include "gp.h"
#include "prim.h"
#include "shade.h"
#include "rt.h"
#include "scene.h"

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
//#include "rtcsg.h"

/************************************************************************************************************/

Scene *s;
RContext *rc;
Matrix4 mclip, mdpy;
static Color bgcolor = { 0.1, 0.1, 0.1 };
Real ar;

/************************************************************************************************************/

typedef struct _atomo_
{
   Vector3  c;
   double   r;
   Vector3  cor;
   int      lig[4];
}
ATOM;

typedef struct _haste_
{
   Vector3  c;
   Vector3  r;
   Vector3  rot;
}
HASTE;

/************************************************************************************************************/

HASTE cria_haste (ATOM at1, ATOM at2, double raio);
void cria_cena (char* filename);
FILE* file_open (const char* filename, const char* mode);
void init_render (void);
Ray ray_view (int u, int v);
void init_sdl (void);
void img_gpdisplay (Image *i);
void set_gpgrey (void);
void set_gpmap (CMap *m);
void cria_imagem (void);
void monta_janela (void);
void fecha_janela (void);
void muda_luz (Vector3 p);
void muda_cor (Object *obj, Vector3 cor);

//Inode *ray_intersect(Object *olist, Ray r);
//void calculate_aspect( void );

/************************************************************************************************************/

#endif

/************************************************************************************************************/
