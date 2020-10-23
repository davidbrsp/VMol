/*------------------------------------------------------------------------------
  MAC0420 - MolecularCo.
    Funcoes relacionadas a interface, modificadas a partir da toolkit de LUIZ VELHO.
------------------------------------------------------------------------------*/

#include "tk.h"

/* Local Variables */
Widget **wa = NULL;
int wn = 0;
int wi = 0;

void image_draw( Image *i )
{
  switch( img_type( i ) )
  {
    case IMG_MONOCOL:
      set_gpgrey( );
      break;
    case IMG_TRUECOL:
      i = img_quant_popul( i, 256, 4 );
    case IMG_MAPPCOL:
      set_gpmap( i->m );
      break;
    default:
      break;
  }
  img_gpdisplay( i );
/* codigo de ouro
  mvopen( 10 );
  mvwindow( 0, 0, 320, 0, 240 );
  mvviewport( 0, 0, (320./240.)/2., .5, 1 );
  mvact( 0 );
  set_gpmap( i->m );
  img_gpdisplay( i );
  mvmainloop( );
*/
}
void tk_open( int n )
{
  int i;
 
  mvopen( n );
  wa = NEWARRAY( n, Widget* );
  for ( i = 0; i < n; i++ )
    wa[ i ] = NULL;
  wn = n;
  wi = 0;
  mvregister( -1, "r", tk_redraw, NULL );
}
void tk_close( )
{
  efree( wa );
  wa = NULL;
  wn = wi = 0;
  mvclose( );
}
void tk_mainloop( )
{
  mvmainloop( );
}
int tk_redraw( void* p, int v, real x, real y, char* e ) /* repinta tudo */
{
  int i;
  
  fprintf( stderr, "redraw\n" );
  fflush( stderr );
  for( i = 0; i < wi; i++ )
  {
    switch( wa[ i ]->type )
    {
      case TK_BUTTON:
        button_draw( wa[ i ], 1 );
        break;
      default:
        error( "tk" );
        break;
    }
  }

  return 0;
}
/* cria objeto da toolkit */
Widget* tk_widget( int type, real x, real y, real hori, real vert, int ( *f )( ), void *d, Image *i )
{
  Widget *w = widget_new( type, x, y, hori, vert, f );
  
  if( wi >= wn )
    error( "tk" );
  w->id = wi++;
  wa[ w->id ] = w;
  if( i )
    mvwindow( w->id, 0, i->w, 0, i->h );
  else
    mvwindow( w->id, 0, 1, 0, 1 );
  mvviewport( w->id, w->xo, w->xo + w->xs, w->yo, w->yo + w->ys );
  switch( type )
  {
    case TK_BUTTON:
      button_make( w, d, i );
      break;
    default:
      error( "tk" );
      break;
  }
  
  return w;
}
Widget* widget_new( int type, real x, real y, real hori, real vert, int ( *f )( ) )
{
  Widget *w = NEWSTRUCT( Widget );
  w->id = -1;
  w->type = type;
  w->xo = x;
  w->yo = y;
  w->xs = hori;
  w->ys = vert;
  w->f = f;
  w->d = NULL;
  w->img = NULL;

  return w;
}
void button_make( Widget *w, void *s, Image *i ) /* associa um evento no botao a uma funcao */
{
  mvregister( w->id, "b1+", button_pressed, w );
  mvregister( w->id, "b1-", button_released, w );
  w->d = s;
  w->img = i;
  button_draw( w, 1 );
}
void button_draw( Widget *w, int posneg ) /* desenha botao */
{
  mvact( w->id );
  if( !w->img )
  {
    gppalette( 0, "white" );	/* black on white */
    gppalette( 1, "black" );
    gpcolor( posneg ? 0 : 1 );
    gpbox( 0., 1., 0., 1. );
    gpcolor( posneg );
    gptext( .05, .0, w->d, NULL );
  }
  else
    image_draw( w->img );
  mvframe( );
}
int button_pressed( void* p, int v, real x, real y, char* e )
{
  Widget *w = p;
  button_draw( w, 0 );
  return 0;
}
int button_released( void* p, int v, real x, real y, char* e )
{
  Widget *w = p;
  button_draw( w, 1 );
  return w->f( w );
}
