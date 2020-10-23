/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (HEADER) FUNCOES RELACIONADAS A INTERFACE, TK ...                                                  **/
/************************************************************************************************************/

#ifndef TK_H
#define TK_H

/************************************************************************************************************/

#include "imagem.h"

//#include "defs.h"
//#include "laux.h"
//#include "geom.h"
#include "gp.h"
#include "mv.h"

/************************************************************************************************************/

#define TK_BUTTON 1
/* componente da toolkit */

/************************************************************************************************************/

typedef struct Widget {
  int id;
  int type;
  real xo, yo;
  real xs, ys;
  void* d;
  int ( *f )( );
  Image *img;
} Widget;

/************************************************************************************************************/

//Widget* widget_new(int type, real x, real y, real s, int (*f)());
Widget* widget_new( int type, real x, real y, real hori, real vert, int ( *f )( ) );

void tk_open( int n );
void tk_close( );
void tk_mainloop( );
//Widget* tk_widget(int type, real x, real y, int (*f)(), void *d);
Widget* tk_widget( int type, real x, real y, real hori, real vert, int (*f)( ), void *d, Image *i );
int  tk_redraw( void* p, int v, real x, real y, char* e );

//void button_make( Widget *w, char *s );
void button_make( Widget *w, void *s, Image *i );
//void button_draw( Widget *w, int posneg , Image *i );

void button_draw( Widget *w, int posneg );
int  button_pressed( void* p, int v, real x, real y, char* e );
int  button_released( void* p, int v, real x, real y, char* e );

void image_draw( Image *i );
//void image_show( Image *i ,real xOffSet, real yOffSet, int width, int height );
//void image_draw( Image *i ,real xOffSet, real yOffSet, int width, int height );

/************************************************************************************************************/

#endif

/************************************************************************************************************/
