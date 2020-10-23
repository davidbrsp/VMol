/************************************************************************************************************/
/**     MAC0420 - MOLECULAR CO.                                                                            **/
/**     (CÓDIGO) FUNÇÕES RELACIONADAS A RENDERIZAÇÃO, IMAGEAMENTO, CENAS ...                               **/
/************************************************************************************************************/

#include "imagem.h"

/************************************************************************************************************/

FILE* file_open (const char* filename, const char* mode)   /* Abre arquivo tratando erro */
{
   FILE* pFile;

   pFile = fopen (filename, mode);
   if (pFile == NULL)
   {
      fprintf (stderr, "Arquivo: %s  Modo: %s\n", filename, mode);
      fprintf (stderr, "Nao foi possivel abrir o arquivo ...\n");
      exit (EXIT_FAILURE);
   }
   return pFile;
}

/************************************************************************************************************/

HASTE cria_haste (ATOM at1, ATOM at2, double raio)
{
   HASTE h;
   double ang_x, ang_y, ang_z;

   // Cálculo do centro da haste (média entre átomos em cada eixo)
   h.c.x = (at1.c.x + at2.c.x) / 2;
   h.c.y = (at1.c.y + at2.c.y) / 2;
   h.c.z = (at1.c.z + at2.c.z) / 2;

   // Cálculo dos raios (2 são fixos, 1 é metade da distância entre centros dos 2 átomos)
   if (at1.c.x == at2.c.x)
   {
      h.r.x = raio;
      if (at1.c.z == at2.c.z)
      {
         h.r.z = raio;
         h.r.y = v3_norm (v3_sub (at1.c, at2.c)) / 2;
      }
      else
      {
         h.r.z = v3_norm (v3_sub (at1.c, at2.c)) / 2;
         h.r.y = raio;
      }
   }
   else
   {
      if (at1.c.y == at2.c.y)
      {
         h.r.y = raio;
         if (at1.c.x == at2.c.x)
         {
            h.r.x = raio;
            h.r.z = v3_norm (v3_sub (at1.c, at2.c)) / 2;
         }
         else
         {
            h.r.x = v3_norm (v3_sub (at1.c, at2.c)) / 2;
            h.r.z = raio;
         }
      }
      else  // (at1.c.z = at2.c.z)
      {
         h.r.z = raio;
         if (at1.c.x == at2.c.x)
         {
            h.r.x = raio;
            h.r.y = v3_norm (v3_sub (at1.c, at2.c)) / 2;
         }
         else
         {
            h.r.x = v3_norm (v3_sub (at1.c, at2.c)) / 2;
            h.r.y = raio;
         }
      }
   }

   // Cálculo das rotações em cada eixo (haste entre 2 átomos só pode girar em torno de
   // um eixo se os centros desses átomos estiverem no plano formado pelos outros eixos)
   ang_x = 0; ang_y = 0; ang_z = 0;
   if (at1.c.x == at2.c.x)  // Plano YZ
   {
      if (at1.c.z != at2.c.z && at1.c.y != at2.c.y)
         ang_x = atan (-(at1.c.y - at2.c.y) / (at1.c.z - at2.c.z));
   }
   if (at1.c.y == at2.c.y)  // Plano XZ
   {
      if (at1.c.x != at2.c.x && at1.c.z != at2.c.z)
         ang_y = atan ((at1.c.z - at2.c.z) / (at1.c.x - at2.c.x));
   }
   if (at1.c.z == at2.c.z)  // Plano XY
   {
      if (at1.c.y != at2.c.y && at1.c.x != at2.c.x)
         ang_z = atan ((at1.c.y - at2.c.y) / (at1.c.x - at2.c.x));
   }

/*   ang_x = 0; ang_y = 0; ang_z = 0;
   if (at1.c.x == at2.c.x)  // Plano YZ
   {
      if ((at1.c.z - at2.c.z) == 0)  ang_x = M_PI_2;
      else                           ang_x = atan (-(at1.c.y - at2.c.y) / (at1.c.z - at2.c.z));
   }
   if (at1.c.y == at2.c.y)  // Plano XZ
   {
      if ((at1.c.x - at2.c.x) == 0)  ang_y = M_PI_2;
      else                           ang_y = atan ((at1.c.z - at2.c.z) / (at1.c.x - at2.c.x));
   }
   if (at1.c.z == at2.c.z)  // Plano XY
   {
      if ((at1.c.x - at2.c.x) == 0)  ang_z = M_PI_2;
      else                           ang_z = atan ((at1.c.y - at2.c.y) / (at1.c.x - at2.c.x));
   }
*/

   h.rot.x = ang_x;
   h.rot.y = ang_y;
   h.rot.z = ang_z;

   return h;
}

/************************************************************************************************************/

void cria_cena (char* filename)
{
   FILE *fMol, *fScn;
   int i, j, k, n;
   int ex, ey, ez, plana;
   ATOM atomo[10];
   HASTE h;
   double rmin, rhaste;

   fMol = file_open (filename, "r");
   fScn = file_open ("vmol.scn", "w");

   fprintf ( fScn, "scene\n{\n"
                   "   light = dist_light\n   {\n      direction = {0, 1, -1}\n   }\n" );

   i = 0;
   rmin = 0.16;
   while (!feof (fMol))
   {
      // Lê informações sobre átomos em cada linha do arquivo de entrada
      fscanf ( fMol, "centro = %lf %lf %lf, raio = %lf, cor = %lf %lf %lf, ligacoes = %d %d %d %d\n",
               &(atomo[i].c.x), &(atomo[i].c.y), &(atomo[i].c.z), &(atomo[i].r),
               &(atomo[i].cor.x), &(atomo[i].cor.y), &(atomo[i].cor.z),
               &(atomo[i].lig[0]), &(atomo[i].lig[1]), &(atomo[i].lig[2]), &(atomo[i].lig[3]) );
               
      if (rmin > atomo[i].r)   rmin = atomo[i].r; 

      // Grava arquivo de cena com as esferas (átomos)
      fprintf ( fScn, "   object = csgobj\n   {\n      "
                "shape = csg_prim {sphere {center = {%.2f, %.2f, %.2f}, radius = %.2f}},\n      "
                "material = plastic {ka = 0.2, kd = 0.8, ks = 0.0, "
                "d_col = {%.2f, %.2f, %.2f}, s_col = {%.2f, %.2f, %.2f}}\n   }\n",
                atomo[i].c.x, atomo[i].c.y, atomo[i].c.z, atomo[i].r,
                atomo[i].cor.x, atomo[i].cor.y, atomo[i].cor.z,
                atomo[i].cor.x, atomo[i].cor.y, atomo[i].cor.z );
      i ++;
   }
   rhaste = rmin / 2;

   // Define se molécula é planar ou espacial
   ex = 0; ey = 0; ez = 0;
   for (j = 1; j < i; j ++)
   {
       if (atomo[j-1].c.x != atomo[j].c.x)   ex = 1;
       if (atomo[j-1].c.y != atomo[j].c.y)   ey = 1;
       if (atomo[j-1].c.z != atomo[j].c.z)   ez = 1;
   }
   if ((ex + ey + ez) == 3)   plana = FALSE;
   else                       plana = TRUE;

   // Para cada ligação entre átomos, cria uma haste (ou plano) e grava no arquivo de cena
   for (j = 0; j < i; j ++)
   {
      for (k = 0; k <= 3; k ++)
      {
         n = atomo[j].lig[k];
         if (n != 0)
         {
            // Se molécula é planar, dá pra criar haste com o primitivo elipsoide
            if (plana == TRUE)
            {
               h = cria_haste (atomo[j], atomo[n], rhaste);
               fprintf ( fScn, "   object = csgobj\n   {\n      "
                         "shape = csg_prim {ellipsoid {center = {%.2f, %.2f, %.2f}, "
                         "radius = {%.2f, %.2f, %.2f}, rotation = {%.2f, %.2f, %.2f}}},\n      "
                         "material = plastic {ka = 0.2, kd = 0.8, ks = 0.0, "
                         "d_col = {0.75, 0.75, 0.75}, s_col = {0.75, 0.75, 0.75}}\n   }\n",
                         h.c.x, h.c.y, h.c.z, h.r.x, h.r.y, h.r.z, h.rot.x, h.rot.y, h.rot.z );
            }
            // Para moléculas espaciais, cria ligação com trilists (forma planos ligando os centros)
            else
            {
               if ( (atomo[j].c.y != atomo[n].c.y) || (atomo[j].c.z != atomo[n].c.z) )
               fprintf ( fScn, "   object = polyobj\n   {\n      material = plastic {se = 2},\n"
                         "      shape = trilist {\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}},\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}} }\n   }\n",
                         atomo[j].c.x, atomo[j].c.y, atomo[j].c.z, atomo[n].c.x, atomo[n].c.y,
                         atomo[n].c.z, atomo[j].c.x + rhaste, atomo[j].c.y, atomo[j].c.z,
                         atomo[n].c.x, atomo[n].c.y, atomo[n].c.z, atomo[j].c.x + rhaste, atomo[j].c.y,
                         atomo[j].c.z, atomo[n].c.x + rhaste, atomo[n].c.y, atomo[n].c.z );

               if ( (atomo[j].c.x != atomo[n].c.x) || (atomo[j].c.z != atomo[n].c.z) )
               fprintf ( fScn, "   object = polyobj\n   {\n      material = plastic {se = 2},\n"
                         "      shape = trilist {\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}},\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}} }\n   }\n",
                         atomo[j].c.x, atomo[j].c.y, atomo[j].c.z, atomo[n].c.x, atomo[n].c.y,
                         atomo[n].c.z, atomo[j].c.x, atomo[j].c.y + rhaste, atomo[j].c.z,
                         atomo[n].c.x, atomo[n].c.y, atomo[n].c.z, atomo[j].c.x, atomo[j].c.y + rhaste,
                         atomo[j].c.z, atomo[n].c.x, atomo[n].c.y + rhaste, atomo[n].c.z );

               if ( (atomo[j].c.x != atomo[n].c.x) || (atomo[j].c.y != atomo[n].c.y) )
               fprintf ( fScn, "   object = polyobj\n   {\n      material = plastic {se = 2},\n"
                         "      shape = trilist {\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}},\n         "
                         "{{%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}, {%.2f, %.2f, %.2f}} }\n   }\n",
                         atomo[j].c.x, atomo[j].c.y, atomo[j].c.z, atomo[n].c.x, atomo[n].c.y,
                         atomo[n].c.z, atomo[j].c.x, atomo[j].c.y, atomo[j].c.z + rhaste,
                         atomo[n].c.x, atomo[n].c.y, atomo[n].c.z, atomo[j].c.x, atomo[j].c.y,
                         atomo[j].c.z + rhaste, atomo[n].c.x, atomo[n].c.y, atomo[n].c.z + rhaste );
            }
         }
      }
   }

   fprintf (fScn, "};\n");
   fclose (fScn);
   fclose (fMol);
}

/************************************************************************************************************/

void init_sdl (void)   /* define as funcoes de parse */
{
   lang_defun ("scene", scene_parse);
   lang_defun ("view", view_parse);
   lang_defun ("dist_light", distlight_parse);
   lang_defun ("plastic", plastic_parse);
   lang_defun ("csgobj", obj_parse);
   lang_defun ("polyobj", obj_parse);
   lang_defun ("trilist", trilist_parse);
   lang_defun ("csg_union", csg_union_parse);
   lang_defun ("csg_inter", csg_inter_parse);
   lang_defun ("csg_diff", csg_diff_parse);
   lang_defun ("csg_prim", csg_prim_parse);
   lang_defun ("sphere", sphere_parse);
   lang_defun ("ellipsoid", ellipsoid_parse);
}

/************************************************************************************************************/

void init_render( void )
{
   mclip = m4_m4prod (s->view->Vinv, s->view->Cinv);
   mdpy = m4_m4prod (s->view->Pinv, s->view->Sinv);

//  s->objs->mat->c = v3_make(0,255,0);
   rc_sset (rc = NEWSTRUCT (RContext), s->view, s->lights, s->img);
//  rc->m.c = v3_make(0,255,0);
}

/************************************************************************************************************/

void muda_cor (Object *obj, Vector3 cor)
{
   obj->mat->c = cor;
   obj->mat->s = cor;
}

/************************************************************************************************************/

void muda_luz (Vector3 p)
{
//As instruções abaixo seriam para se caso a init_dist_light() substituísse a
//indicação da luz no arquivo .scn. Por algum motivo isso NÃO ocorre, então no
//momento é necessário o parâmetro light no arquivo .scn.
/*
  //Se a luz da cena eh a padrão (não foi indicada no arquivo .scn)
  if (s->lights->type == LIGHT_AMBIENT)
    init_dist_light();
*/

   s->lights->dir = v3_unit(p);
}

/************************************************************************************************************/

void init_dist_light( )
{
   Light *l = NEWSTRUCT( Light );

   l->type = LIGHT_DISTANT;
   l->color = C_WHITE;
   l->ambient = 0.1;
   l->intensity = 0.9;
   l->dir = v3_unit (v3_make (1, 1, 1));
   l->transport = distantlight;
   l->tinfo = NULL;
   s->lights = l;
}

/************************************************************************************************************/

void cria_imagem (void)
{
  Color c, bgcolor = v3_make( 50, 98, 100 ); /* cor de fundo */
  int u, v;
  Ray r;
  Inode *l;

  for( v = s->view->sc.ll.y; v < s->view->sc.ur.y; v += 1 )
  {
    for( u = s->view->sc.ll.x; u < s->view->sc.ur.x; u += 1 )
    {
      r = ray_unit( ray_transform( ray_view( u, v ), mclip ) );
      if( ( l = ray_intersect( s->objs, r ) ) != NULL )
        c = point_shade( ray_point( r, l->t ), l->n, s->view->center, rc, l->m );
      else
        c = bgcolor;
      inode_free( l );
      img_putc( s->img, u, v, col_dpymap( c ) );
    }
  }
}

/************************************************************************************************************/

Ray ray_view( int u, int v )
{
  Vector4 w = v4_m4mult( v4_make( u, v, s->view->sc.ur.z, 1 ), mdpy );

  return ray_make( v3_v4conv( v4_m4mult( v4_make( 0, 0, 1, 0 ), mdpy ) ), v3_make( w.x, w.y, w.z ) );
}

/************************************************************************************************************/

void img_gpdisplay( Image *i )
{
  int u, v, s;
  
  for( v = 0; v < i->h; v++ )
  {
    for( u = 0; u < i->w; u++ )
    {
      gpcolor( img_geti( i, u, v ) );
      gpbox( u, u + 1, v, v + 1 );
    }
  }
}

/************************************************************************************************************/

void set_gpgrey( void )
{
  int i, k;

  for( i = 0; i < 256; i++ )
  {
    k = gprgb( i,( Real ) ( i ) / 255., ( Real ) ( i ) / 255., ( Real ) ( i ) / 255. );
    if( k == 0 && i > 0 )
      printf( "color not bound %d ( %d %d %d )\n", i, i, i, i );
  }
}

/************************************************************************************************************/

void set_gpmap( CMap *m )
{
  int i, k;

  for( i = 0; i < 256; i++ )
  {
    k = gprgb( i,( Real ) ( m->r[ i ] ) / 255., ( Real ) ( m->g[ i ] ) / 255., ( Real ) ( m->b[ i ] ) / 255.);
    if( k == 0 && i > 0 )
      printf( "color not bound %d ( %d %d %d )\n", i, m->r[ i ], m->g[ i ], m->b[ i ] );
  }
}

/************************************************************************************************************/
