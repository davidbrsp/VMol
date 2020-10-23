/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (CÓDIGO) FUNÇÕES RELACIONADAS A MOVIMENTO DA CÂMERA, ZOOM ...                                      **/
/************************************************************************************************************/

#include "camera.h"

/************************************************************************************************************/

Widget *imagem;

void muda_camera (CAMERA c)
{
   View* v = initview();
  
   lookat (v, c.center.x, c.center.y, c.center.z, 0, 0, 0, c.up.x, c.up.y, c.up.z);
   s->view = v;
   muda_luz (c.center); /* luz na direcao da camera */
}

/************************************************************************************************************/

CAMERA pega_camera (void)
{
   CAMERA c;
   View *v = s->view;

   c.center = v->center;
   c.up = v->up;
   c.normal = v->normal;
   return c;
}

/************************************************************************************************************/
/**     FUNÇÕES PARA GIRAR MOLÉCULAS (MOVIMENTA CÂMERA)                                                    **/
/************************************************************************************************************/

int norte (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);
   
   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int sul (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;

   cNovo = v3_add(c.center, v3_scale(-2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);
   
   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int leste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   
   c.center = cNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int oeste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(v3_unit(c.center), c.up);
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   
   c.center = cNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );

   return 0;  
}

/************************************************************************************************************/

int nordeste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   //norte
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);

   //leste
   cNovo = v3_add(cNovo, v3_scale(2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;

   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int sudeste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   //leste
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   
   outro = v3_cross(c.up, v3_unit(cNovo));

   //sul
   cNovo = v3_add(cNovo, v3_scale(-2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);

   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int noroeste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   //norte
   cNovo = v3_add(c.center, v3_scale(2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);

   //oeste
   cNovo = v3_add(cNovo, v3_scale(-2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;

   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int sudoeste (void)
{
   CAMERA c = pega_camera();
   Vector3 outro = v3_cross(c.up, v3_unit(c.center));
   Vector3 centroDeMassa = v3_make(0,0,0);
   Vector3 vetorRaioInvertido = v3_sub(centroDeMassa, c.center);
   Real normaVetorRaio = v3_norm(vetorRaioInvertido);
   Vector3 cNovo, vUpNovo, vriNovo;
   
   //oeste
   cNovo = v3_add(c.center, v3_scale(-2*normaVetorRaio/3, v3_unit(outro)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;

   outro = v3_cross(c.up, v3_unit(cNovo));
   
   //sul
   cNovo = v3_add(cNovo, v3_scale(-2*normaVetorRaio/3, v3_unit(c.up)));
   vriNovo = v3_sub(cNovo, centroDeMassa);
   vriNovo = v3_unit(vriNovo);
   vriNovo = v3_scale(normaVetorRaio, vriNovo);
   cNovo = vriNovo;
   vriNovo = v3_sub(centroDeMassa, vriNovo);
   vUpNovo = v3_cross(v3_unit(outro), vriNovo);

   c.center = cNovo;
   c.up = vUpNovo;

   muda_camera( c );
   init_render( );
   cria_imagem( );
   imagem->img = s->img;
   button_draw( imagem, 1 );
   
   return 0;
}

/************************************************************************************************************/

int centro (void)
{
   CAMERA c;
	
   c.center = v3_make (0, 0, 2*ZOOM_FACTOR); /* sempre com o ZOOM_FACTOR */
   c.up = v3_make (0, 1, 0);
   muda_camera (c);
   init_render();
   cria_imagem();
   imagem->img = s->img;
   button_draw (imagem, 1);
  
   return 0;
}

/************************************************************************************************************/
/**     FUNÇÕES PARA ZOOM NA MOLÉCULA (MOVIMENTA CÂMERA)                                                   **/
/************************************************************************************************************/

int zoomIn (void)
{
   CAMERA c = pega_camera();
   Vector3 origem = v3_make(0, 0, 0);
   Vector3 vetorRaioInvertido = c.center;
   Real normaVRI = v3_norm(vetorRaioInvertido);
   
   if (normaVRI > ZOOM_FACTOR)
   {
      vetorRaioInvertido = v3_scale((normaVRI - ZOOM_FACTOR), v3_unit(vetorRaioInvertido));
   
      c.center = vetorRaioInvertido;
      muda_camera (c);
      init_render();
      cria_imagem();
      imagem->img = s->img;
      button_draw (imagem, 1);
   }
  
   return 0;
}

/************************************************************************************************************/

int zoomOut (void)
{
    CAMERA c = pega_camera();
    Vector3 origem = v3_make(0, 0, 0);
    Vector3 vetorRaioInvertido = c.center;
    Real normaVRI = v3_norm(vetorRaioInvertido);
   
    vetorRaioInvertido = v3_scale((normaVRI + ZOOM_FACTOR), v3_unit(vetorRaioInvertido));
  
    c.center = vetorRaioInvertido;
    muda_camera (c);
    init_render();
    cria_imagem();
    imagem->img = s->img;
    button_draw (imagem, 1);

    return 0;
}

/************************************************************************************************************/
