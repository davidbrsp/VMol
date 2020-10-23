/*------------------------------------------------------------------------------
  MAC0420 - Introducao a Computacao Grafica
    Professor: Roberto Marcondes Cesar Junior
  MolecularCo.
    Integrantes:
    David Macedo da Conceicao nUSP: 3681479
    Fernando Waitman          nUSP: 4951109
    Leonardo Ventura          nUSP: 5382607
    Ricardo A. Fernandes      nUSP: 3510300
------------------------------------------------------------------------------*/

#include "vmol.h"

/* Variaveis Globais */
//char moleculas[ NUM_MOLECULAS ][ MAX_MOLECULAS_NAME_LEN ] =
//     { "H0O", "H1O", "H2O", "H3O", "H4O", "H5O", "H6O" };
Widget *botoes[ NUM_BOTOES + 2 ],
       *browser[ 9 ],
       *zoom[ 3 ],
       *imagem;
char browserNames[ 9 ][ MAX_BROWSER_NAME_LEN ] =
     { "NW" , "N", "NE",
       "W"  , "*", "E" ,
       "SW" , "S", "SE" };
char zoomNames[ 3 ][ 10 ] = { "+", "Zoom", "-" };
int inic = 0, fim = 4;
char **moleculas;
int numMoleculas = 0;

/* *
 * malloc do Feofiloff.
 * @param nbytes - numero de bytes a serem alocados.
 */
void* mallocX( size_t nbytes )
{
   void *ptr;

   ptr = malloc( nbytes );
   if( ptr == NULL )
     panic( "Socorro! malloc devolveu NULL" );
   return ptr;
}
/* *
 * malloc do Feofiloff adaptado pra realloc.
 * @param pointer - ponteiro para memoria previamente alocada.
 * @param nbytes - numero de bytes a serem alocados.
 */
void *reallocX( void *pointer, size_t nbytes )
{
  void *ptr;

  ptr = realloc( pointer, nbytes );
  if( ptr == NULL )
    panic( "Socorro! realloc devolveu NULL" );
  return ptr;
}
/* *
 * fopen baseado nos algoritmos do Feofiloff.
 * @param filename - nome do arquivo a ser aberto.
 * @param mode - modo de abertura do arquivo.
 */
FILE* fopenX( const char* filename, const char* mode )   /* Abre arquivo tratando erro */
{
  FILE* pFile;

  pFile = fopen( filename, mode );
  if( pFile == NULL )
  {
    fprintf( stderr, "Nao foi possivel abrir o Arquivo: %s  Modo: %s\n", filename, mode );
    exit( EXIT_FAILURE );
  }
  return pFile;
}
/* Funcoes chamadas pelos botoes da interface */
int elem( Widget *w )
{
  char nameBuf[ FILENAME_MAX + 5 ];
  FILE *fScn;

  nameBuf[ 0 ] = '\0';
  strncpy( nameBuf, w->d, FILENAME_MAX );
  strcat( nameBuf, ".txt" );
  cria_cena( nameBuf );
  fScn = fopenX( "vmol.scn", "r" );
  lang_file( fScn ); /* redireciona leitura de cena */
  s = scene_read( ); /* le a cena de um arquivo */
  fclose( fScn );
  imagem->d = w->d;
//  mudaCor( s->objs, v3_make( 255, 0, 255 ) );
  centro( );

  return 0;
}
int cima( void ) /* sobe lista */
{
  int i;

  if( inic )
  {
    inic -= 1;
    fim -= 1;
  	for( i = 0; i < NUM_BOTOES; i++ )
  	{
      botoes[ i + 1 ]->d = moleculas[ inic + i ];
    	button_draw( botoes[ i + 1 ], 1 );
    }
  }
/* debug
  fprintf( stderr, "Cima\n" );
  fprintf( stderr, "inic: %d fim: %d\n", inic, fim );
  fflush( stderr );
*/
  return 0;
}
int baixo( void ) /* desce lista */
{
  int i;

  inic += 1;
  fim += 1 ;
  if( inic <= numMoleculas - NUM_BOTOES )
    for( i = 0; i < 5; i++ )
    {
      botoes[ i + 1 ]->d = moleculas[ inic + i ];
      button_draw( botoes[ i + 1 ], 1 );
    }
  else
  {
    inic = numMoleculas - NUM_BOTOES;
    fim = inic + NUM_BOTOES;
  }
/* debug
  fprintf( stderr, "Baixo\n" );
  fprintf( stderr, "inic: %d fim: %d\n", inic, fim );
  fflush( stderr );
*/
  return 0;
}
int sair( )
{
  tk_close( );
  gpclose( 0 );
  exit( 0 );
  return 0;
}
int salvar( ) /* salva img atual em um arquivo NomeDaMolecula.ras */
{
  char nameBuf[ FILENAME_MAX + 5 ];

  nameBuf[ 0 ] = '\0';
  strncpy( nameBuf, imagem->d, FILENAME_MAX );
  strcat( nameBuf, ".ras" );
  img_write( imagem->img, nameBuf, 0 );
  printf( "%s saved.\n", nameBuf );
  return 0;
}
int sobre( void )
{
  imagem->img = img_read( "vmol.ras" );
  imagem->d = "sobre";
  button_draw( imagem, 1 );
  return 0;
}
int main( int argc, char* argv[ ] ) 
{
  int i = 0;
  FILE *fList;
  char nameBuf[ MAX_MOLECULAS_NAME_LEN ];
  
  fList = fopenX( MOL_LIST, "r" );  
  while( !feof( fList ) ) /* preenche lista de nomes das moleculas */
  {
    moleculas = ( char** ) reallocX( moleculas, ( i + 1 ) * sizeof( char* ) );
    fscanf( fList, "%s\n", &nameBuf );
    moleculas[ i ] = ( char* ) mallocX( ( strlen( nameBuf ) + 1 ) * sizeof( char ));
    strcpy( moleculas[ i++ ], nameBuf );
  }
  numMoleculas = i;
  fclose( fList );
  gpsetsize( 850, 450, 0, 0 );
  gpopen( "MAP0420 - VMol - MolecularCo." );
  gpviewport( 850, 450, 0, 0 );
  init_sdl( );    /* inicializa sdl */
  tk_open( MAX_NUM_WIDGET );
  /* cria botoes da lista de moleculas */
  botoes[ 0 ] = tk_widget( TK_BUTTON, 1.645,  .95, .155, .04,   cima,         "CIMA", img_read( "N.ras" ) );
  botoes[ 1 ] = tk_widget( TK_BUTTON, 1.645,  .90, .155, .04,   elem, moleculas[ 0 ], NULL );
  botoes[ 2 ] = tk_widget( TK_BUTTON, 1.645,  .85, .155, .04,   elem, moleculas[ 1 ], NULL );
  botoes[ 3 ] = tk_widget( TK_BUTTON, 1.645,  .80, .155, .04,   elem, moleculas[ 2 ], NULL );
  botoes[ 4 ] = tk_widget( TK_BUTTON, 1.645,  .75, .155, .04,   elem, moleculas[ 3 ], NULL );
  botoes[ 5 ] = tk_widget( TK_BUTTON, 1.645,  .70, .155, .04,   elem, moleculas[ 4 ], NULL );
  botoes[ 6 ] = tk_widget( TK_BUTTON, 1.645,  .65, .155, .04,  baixo,        "BAIXO", img_read( "S.ras" ) );
  botoes[ 7 ] = tk_widget( TK_BUTTON,  1.26, .212,  .12, .12, salvar,       "SALVAR", img_read( "SAVE.ras" ) );
  botoes[ 7 ] = tk_widget( TK_BUTTON, 1.501, .212,  .12, .12,  sobre,        "SOBRE", img_read( "ABOUT.ras" ) );
  botoes[ 8 ] = tk_widget( TK_BUTTON,  1.68,  .01,  .12, .12,   sair,         "SAIR", img_read( "EXIT.ras" ) );
  /* cria botoes de navegacao */
  browser[ 0 ] = tk_widget( TK_BUTTON,  1.26, .869, .12, .12, noroeste, browserNames[ 0 ], img_read( "NW.ras" ) );
  browser[ 1 ] = tk_widget( TK_BUTTON, 1.381, .869, .12, .12,    norte, browserNames[ 1 ], img_read( "N.ras" ) );
  browser[ 2 ] = tk_widget( TK_BUTTON, 1.501, .869, .12, .12, nordeste, browserNames[ 2 ], img_read( "NE.ras" ) );
  browser[ 3 ] = tk_widget( TK_BUTTON,  1.26, .749, .12, .12,    oeste, browserNames[ 3 ], img_read( "W.ras" ) );
  browser[ 4 ] = tk_widget( TK_BUTTON, 1.381, .749, .12, .12,   centro, browserNames[ 4 ], img_read( "CENTER.ras" ) );
  browser[ 5 ] = tk_widget( TK_BUTTON, 1.501, .749, .12, .12,    leste, browserNames[ 5 ], img_read( "E.ras" ) );
  browser[ 6 ] = tk_widget( TK_BUTTON,  1.26, .629, .12, .12, sudoeste, browserNames[ 6 ], img_read( "SE.ras" ) );
  browser[ 7 ] = tk_widget( TK_BUTTON, 1.381, .629, .12, .12,      sul, browserNames[ 7 ], img_read( "S.ras" ) );
  browser[ 8 ] = tk_widget( TK_BUTTON, 1.501, .629, .12, .12,  sudeste, browserNames[ 8 ], img_read( "SW.ras" ) );
  /* cria botoes de zoom*/
  zoom[ 0 ] = tk_widget( TK_BUTTON,  1.26, .412, .12, .12,  zoomIn, zoomNames[ 0 ], img_read( "ZOOM_IN.ras" ) );
//  zoom[ 1 ] = tk_widget( TK_BUTTON, 1.26, .15, .155, .04,  centro, "ABCDEFG", NULL );
  zoom[ 1 ] = tk_widget( TK_BUTTON, 1.381, .412, .12, .12,  centro, zoomNames[ 1 ], img_read( "CENTER.ras" ) );
  zoom[ 2 ] = tk_widget( TK_BUTTON, 1.501, .412, .12, .12, zoomOut, zoomNames[ 2 ], img_read( "ZOOM_OUT.ras" ) );
  imagem = tk_widget( TK_BUTTON, .01, .01, 1.23, .98, centro, "imagem", img_read( "vmol.ras" ) );
  elem( botoes[ 3 ] );
  tk_mainloop( );
  return 0;
}
