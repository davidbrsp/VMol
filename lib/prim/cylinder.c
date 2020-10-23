#include <stdio.h>
#include <math.h>
#include "lang.h"
#include "prim.h"
#include "poly.h"
#include "sphere.h"


PrimFuncs sphere_funcs = {
  sphere_instance,
  sphere_destroy,
  sphere_bbox,
  sphere_classify,
  sphere_point,
  sphere_normal,
  sphere_gradient,
  sphere_intersect,
  sphere_transform,
  sphere_uv_decomp,
  sphere_texc,
  sphere_du,
  sphere_dv,
  sphere_local,
  sphere_id,
  sphere_write,
  sphere_draw,
};

/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Prim *sphere_instance(PrimFuncs *f)
{
  Vector3 ll = {-1,-1,-1}, ur = {1,1,1};
  Prim *p = NEWSTRUCT(Prim);
  Sphere *s = NEWSTRUCT(Sphere);

  p->f = f;
  p->b.ll = ll; p->b.ur = ur;
  p->ti = p->td = m4_ident();
  s->c = v3_make(0,0,0); s->r = 1; s->h = 1;
  p->d = s;
  return p;
}

/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
void sphere_destroy(Prim *p)
{
  efree(p->d);
  efree(p);
}

/* !!! Acrescentei parâmetros h (altura), x, y, z (ângulos de rotação) !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Prim *sphere_set(Prim *p, Vector3 c, double  r, double h, Vector3 rot)
{
  Sphere *s = p->d;
  s->c = c; s->r = r;
  p->ti = p->td = m4_ident();
  p = sphere_transform(p, m4_scale(r,r,h), m4_scale(1/r,1/r,1/h)); 
  p = sphere_transform(p, m4_translate(c.x,c.y,c.z), m4_translate(-c.x,-c.y,-c.z));
  p = sphere_transform(p, m4_rotate('x', rot.x), m4_rotate('x', -rot.x));
  p = sphere_transform(p, m4_rotate('y', rot.y), m4_rotate('y', -rot.y));
  p = sphere_transform(p, m4_rotate('z', rot.z), m4_rotate('z', -rot.z));
  p->b = sphere_bbox(p);
  return p;
}


/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Box3d sphere_bbox(Prim *p)  /* Mudando os laços não influiu no desenho !!! */
{
  Box3d b;
  Vector3 v;
  double x, y, z;

  for (x = -1; x <= 1; x +=2) {
    for (y = -1; y <= 1; y +=2) {
      for (z = -1; z <= 1; z +=2) {
 // for (x = -0.5; x <= 0.5; x +=1) {
 //   for (y = -0.5; y <= 0.5; y +=1) {
 //     for (z = -0.5; z <= 0.5; z +=1) {
	v = v3_m4mult(v3_make(x, y, z), p->td);
	if (x == -1 && y == -1 && z == -1) {
	  b.ll = b.ur = v;
	} else {
	  if (v.x < b.ll.x) b.ll.x = v.x;
	  if (v.y < b.ll.y) b.ll.y = v.y;
	  if (v.z < b.ll.z) b.ll.z = v.z;
	  if (v.x > b.ur.x) b.ur.x = v.x;
	  if (v.y > b.ur.y) b.ur.y = v.y;
	  if (v.z > b.ur.z) b.ur.z = v.z;
	}	
      }
    }
  }
  return b;
}


/* !!! Classifica pontos com base na equação implícita (x^2 + y^2 = 1) (acho que precisa acrescentar informação do eixo z) !!!!!!! */
int sphere_classify(Prim *p, Vector3 q)
{
  Vector3 w = v3_m4mult(q, p->ti);
//  Real d = v3_norm(w);
  Real d = sqrt ((w.x*w.x)+(w.y*w.y));
//  return (d < 1)? PRIM_IN : ((d > 1)? PRIM_OUT : PRIM_ON);
  return ((d < 1) && (w.z < 1) && (w.z > -1))? PRIM_IN : ((d > 1)? PRIM_OUT : PRIM_ON);
}


/* !!! Alterei coordenadas do ponto com base na equação paramétrica (cos(u), sen(u), v) !!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_point(Prim *p, Real u, Real v)
{
  Vector3 w;
 // w.x = cos(u)*cos(v);
 // w.y = sin(u)*cos(v);
 // w.z = sin(v);
  w.x = cos(u);
  w.y = sin(u);
  w.z = v;
  return v3_m4mult(w, p->td);
}


/* !!! Alterei normal com base na equação paramétrica (cos(u), sen(u), v) !!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_normal(Prim *p, Real u, Real v)
{
  Vector3 w;
//  w.x = cos(u)*cos(v);
//  w.y = sin(u)*cos(v);
//  w.z = sin(v);
  w.x = cos(u);
  w.y = sin(u);
  w.z = v;
  return v3_m3mult(w, m4_transpose(p->ti));
}


/* !!! Nessa função não modifiquei nada (mas acho que precisa mudar alguma coisa) !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_gradient(Prim *p, Vector3 q)
{
  Vector3 w = v3_m4mult(q, p->ti);
  return v3_m3mult(v3_scale(2., w), m4_transpose(p->ti));
}


/* !!! Essa função é a que causa as alterações mais profundas na imagem (intersecção dos raios com objetos) !!!!!!!!!!!!!!!!!!!!!! */
Inode *sphere_intersect(Prim *p, Ray ro)
{
  double a, b, c, disc, t0, t1;
  Inode *i0=NULL, *i1;
  Ray r = ray_transform(ro, p->ti);

//  a = v3_sqrnorm(r.d);
//  b = 2 * v3_dot(r.d, r.o);
//  c = v3_sqrnorm(r.o) - 1;

  a = (r.d.x * r.d.x) + (r.d.y * r.d.y);
  b = 2 * ((r.d.x * r.o.x) + (r.d.y * r.o.y));
  c = (r.o.x * r.o.x) + (r.o.y * r.o.y) - 1;


  if ((disc = SQR(b) - 4 * a * c) <= 0)
    return (Inode *)0;
  t0 = (-b - sqrt(disc)) / (2 * a);
  t1 = (-b + sqrt(disc)) / (2 * a);
  if (t1 < RAY_EPS)
    return (Inode *)0;
  if (t0 < RAY_EPS) {
    Vector3 n1 = v3_unit(sphere_gradient(p, ray_point(r, t1)));
    return inode_alloc(t1, n1, FALSE);
  } else {
    Vector3 n0 = v3_unit(sphere_gradient(p, ray_point(ro, t0)));
    Vector3 n1 = v3_unit(sphere_gradient(p, ray_point(ro, t1)));
    i0 = inode_alloc(t0, n0, TRUE);
    i1 = inode_alloc(t1, n1, FALSE);
    i0->next = i1;
    return i0;
  }
}


/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Prim *sphere_transform(Prim *p, Matrix4 md, Matrix4 mi)
{
  p->td = m4_m4prod(md, p->td);
  p->ti = m4_m4prod(p->ti, mi);
  return p;
}

#define UMIN (0)
#define UMAX (PITIMES2)
#define ULEN (UMAX - UMIN)
#define VEPS (0.02)
#define VMIN ((PI/2.0) + VEPS )
#define VMAX (PI + (PI/2.0) - VEPS)
#define VLEN (VMAX - VMIN)


/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Poly *sphere_uv_decomp(Prim *p, Real level)
{
  int i, j, nu = 20 * level, nv = 10 * level;
  Real u, v, iu = ULEN/(Real)nu, iv = VLEN/(Real)nv;
  Poly *l = NULL;

  for (u = UMIN, i = 0; i < nu; u += iu, i++) {
    for (v = VMIN, j = 0; j < nv; v += iv, j++) {
      l = poly_insert(l,
               poly3_make(v3_make(u,v,0),v3_make(u,v+iv,0),v3_make(u+iu,v,0)));
      l = poly_insert(l,
               poly3_make(v3_make(u+iu,v+iv,0),v3_make(u+iu,v,0),v3_make(u,v+iv,0)));
    }
  }
  return l;
}

/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_texc(Prim *p, Real u, Real v)
{
  return v3_make((u - UMIN)/ULEN, (v - VMIN)/VLEN, 0);
}

/* !!! Alterei derivadas em u com base na equação paramétrica (cos(u), sen(u), v) !!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_du(Prim *p, Real u, Real v)
{
  return v3_make(- sin(u), cos(u), 0);
}

/* !!! Alterei derivadas em v com base na equação paramétrica (cos(u), sen(u), v) !!!!!!!!!!!!!!!!!!!!!!! */
Vector3 sphere_dv(Prim *p, Real u, Real v)
{
  return v3_make(0, 0, 1);
}

/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Matrix4 sphere_local(Prim *p)
{
  return p->ti;
}


/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
int sphere_id(Prim *p)
{
  return SPHERE;
}

/* !!! Nessa função não modifiquei nada (precisa incluir rotações)!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void sphere_write(Prim *p, FILE *fp)
{
  Sphere *s = p->d;
  fprintf(fp, "sphere { \n");
  fprintf(fp, "\t\t center = {%g, %g, %g},\n",s->c.x,s->c.y,s->c.z);
  fprintf(fp, "\t\t radius = %g \n}\n",s->r);
}

/* !!! Nessa função não modifiquei nada !!!!!!!!!!!!!!!!!!!!!!!!!!! */
void sphere_draw(Prim *p, int axis)
{
  Vector3 v[5];
  Box3d b;
  int i;
  
  b = sphere_bbox(p);
  switch (axis) {
  case 'z':
    gpline(b.ll.x, b.ll.y, b.ll.x, b.ur.y);
    gpline(b.ll.x, b.ur.y, b.ur.x, b.ur.y);
    gpline(b.ur.x, b.ur.y, b.ur.x, b.ll.y);
    gpline(b.ur.x, b.ll.y, b.ll.x, b.ll.y);
    break;
  case 'y':
    gpline(b.ll.x, b.ll.z, b.ll.x, b.ur.z);
    gpline(b.ll.x, b.ur.z, b.ur.x, b.ur.z);
    gpline(b.ur.x, b.ur.z, b.ur.x, b.ll.z);
    gpline(b.ur.x, b.ll.z, b.ll.x, b.ll.z);
    break;
  case 'x':
    gpline(b.ll.z, b.ll.y, b.ll.z, b.ur.y);
    gpline(b.ll.z, b.ur.y, b.ur.z, b.ur.y);
    gpline(b.ur.z, b.ur.y, b.ur.z, b.ll.y);
    gpline(b.ur.z, b.ll.y, b.ll.z, b.ll.y);
    break;
  default:
    warning("(sphere draw) invalid projection");
  }

}

/* !!! Acrescentei os parâmetros na chamada de sphere_set, e na leitura da cena !!!!!!!!!!!!!!!!!!!!!!!!!!! */
Val sphere_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, 0};

  if (pass == T_EXEC) {
    Vector3 c = pvl_get_v3(pl, "center", v3_make(0,0,0));
    double r = pvl_get_num(pl, "radius", 1);
    double h = pvl_get_num(pl, "heigth", 1);
    Vector3 rot = pvl_get_v3(pl, "rotation", v3_make(0,0,0));
    v.type = V_PRIM;
    sphere_set(v.u.v = sphere_instance(&sphere_funcs), c, r, h, rot);
  }
  return v;
}
