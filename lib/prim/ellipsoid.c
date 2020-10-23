#include <stdio.h>
#include <math.h>
#include "lang.h"
#include "prim.h"
#include "poly.h"
#include "ellipsoid.h"


PrimFuncs ellipsoid_funcs = {
  ellipsoid_instance,
  ellipsoid_destroy,
  ellipsoid_bbox,
  ellipsoid_classify,
  ellipsoid_point,
  ellipsoid_normal,
  ellipsoid_gradient,
  ellipsoid_intersect,
  ellipsoid_transform,
  ellipsoid_uv_decomp,
  ellipsoid_texc,
  ellipsoid_du,
  ellipsoid_dv,
  ellipsoid_local,
  ellipsoid_id,
  ellipsoid_write,
  ellipsoid_draw,
};

/* ********** Função alterada ********** Mudança nos parâmetros ********** */
Prim *ellipsoid_instance(PrimFuncs *f)
{
  Vector3 ll = {-1,-1,-1}, ur = {1,1,1};
  Prim *p = NEWSTRUCT(Prim);
  Ellipsoid *s = NEWSTRUCT(Ellipsoid);

  p->f = f;
  p->b.ll = ll; p->b.ur = ur;
  p->ti = p->td = m4_ident();
  s->c = v3_make(0,0,0);
  s->r = v3_make(1,1,1);
  s->rot = v3_make(0,0,0);
  p->d = s;
  return p;
}

void ellipsoid_destroy(Prim *p)
{
  efree(p->d);
  efree(p);
}

/* ********** Função alterada ********** Mudança nos parâmetros e transformações ********** */
Prim *ellipsoid_set(Prim *p, Vector3 c, Vector3 r, Vector3 rot)
{
  Ellipsoid *s = p->d;
  s->c = c;
  s->r = r;
  p->ti = p->td = m4_ident();
  p = ellipsoid_transform(p, m4_scale(r.x,r.y,r.z), m4_scale(1/r.x,1/r.y,1/r.z)); 
  p = ellipsoid_transform(p, m4_rotate('x', rot.x), m4_rotate('x', -rot.x));
  p = ellipsoid_transform(p, m4_rotate('y', rot.y), m4_rotate('y', -rot.y));
  p = ellipsoid_transform(p, m4_rotate('z', rot.z), m4_rotate('z', -rot.z));
  p = ellipsoid_transform(p, m4_translate(c.x,c.y,c.z), m4_translate(-c.x,-c.y,-c.z));
  p->b = ellipsoid_bbox(p);
  return p;
}


Box3d ellipsoid_bbox(Prim *p)
{
  Box3d b;
  Vector3 v;
  double x, y, z;

  for (x = -1; x <= 1; x +=2) {
    for (y = -1; y <= 1; y +=2) {
      for (z = -1; z <= 1; z +=2) {
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


int ellipsoid_classify(Prim *p, Vector3 q)
{
  Vector3 w = v3_m4mult(q, p->ti);
  Real d = v3_norm(w);
  return (d < 1)? PRIM_IN : ((d > 1)? PRIM_OUT : PRIM_ON);
}


Vector3 ellipsoid_point(Prim *p, Real u, Real v)
{
  Vector3 w;
  w.x = cos(u)*cos(v);
  w.y = sin(u)*cos(v);
  w.z = sin(v);
  return v3_m4mult(w, p->td);
}


Vector3 ellipsoid_normal(Prim *p, Real u, Real v)
{
  Vector3 w;
  w.x = cos(u)*cos(v);
  w.y = sin(u)*cos(v);
  w.z = sin(v);
  return v3_m3mult(w, m4_transpose(p->ti));
}


Vector3 ellipsoid_gradient(Prim *p, Vector3 q)
{
  Vector3 w = v3_m4mult(q, p->ti);
  return v3_m3mult(v3_scale(2., w), m4_transpose(p->ti));
}


Inode *ellipsoid_intersect(Prim *p, Ray ro)
{
  double a, b, c, disc, t0, t1;
  Inode *i0, *i1;
  Ray r = ray_transform(ro, p->ti);

  a = v3_sqrnorm(r.d);
  b = 2 * v3_dot(r.d, r.o);
  c = v3_sqrnorm(r.o) - 1;
  if ((disc = SQR(b) - 4 * a * c) <= 0)
    return (Inode *)0;
  t0 = (-b - sqrt(disc)) / (2 * a);
  t1 = (-b + sqrt(disc)) / (2 * a);
  if (t1 < RAY_EPS)
    return (Inode *)0;
  if (t0 < RAY_EPS) {
    Vector3 n1 = v3_unit(ellipsoid_gradient(p, ray_point(r, t1)));
    return inode_alloc(t1, n1, FALSE);
  } else {
    Vector3 n0 = v3_unit(ellipsoid_gradient(p, ray_point(ro, t0)));
    Vector3 n1 = v3_unit(ellipsoid_gradient(p, ray_point(ro, t1)));
    i0 = inode_alloc(t0, n0, TRUE);
    i1 = inode_alloc(t1, n1, FALSE);
    i0->next = i1;
    return i0;
  }
}


Prim *ellipsoid_transform(Prim *p, Matrix4 md, Matrix4 mi)
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


Poly *ellipsoid_uv_decomp(Prim *p, Real level)
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

Vector3 ellipsoid_texc(Prim *p, Real u, Real v)
{
  return v3_make((u - UMIN)/ULEN, (v - VMIN)/VLEN, 0);
}

Vector3 ellipsoid_du(Prim *p, Real u, Real v)
{
  return v3_make(- sin(u) * cos(v), cos(u) * cos(v), 0);
}

Vector3 ellipsoid_dv(Prim *p, Real u, Real v)
{
  return v3_make(- cos(u) * sin(v), - sin(u) * sin(v), cos(v));
}

Matrix4 ellipsoid_local(Prim *p)
{
  return p->ti;
}


int ellipsoid_id(Prim *p)
{
  return ELLIPSOID;
}

/* ********** Função alterada ********** Mudança nos parâmetros ********** */
void ellipsoid_write(Prim *p, FILE *fp)
{
  Ellipsoid *s = p->d;
  fprintf(fp, "ellipsoid { \n");
  fprintf(fp, "\t\t center = {%g, %g, %g},\n",s->c.x,s->c.y,s->c.z);
  fprintf(fp, "\t\t radius = %g, %g, %g \n}\n",s->r.x, s->r.y, s->r.z);
  fprintf(fp, "\t\t rotation = %g, %g, %g \n}\n",s->rot.x, s->rot.y, s->rot.z);
}

void ellipsoid_draw(Prim *p, int axis)
{
  Vector3 v[5];
  Box3d b;
  int i;
  
  b = ellipsoid_bbox(p);
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
    warning("(ellipsoid draw) invalid projection");
  }

}


/* ********** Função alterada ********** Mudança nos parâmetros ********** */
Val ellipsoid_parse(int pass, Pval *pl)
{
  Val v = {V_NULL, 0};

  if (pass == T_EXEC) {
    Vector3 c = pvl_get_v3(pl, "center", v3_make(0,0,0));
    Vector3 r = pvl_get_v3(pl, "radius", v3_make(1,1,1));
    Vector3 rot = pvl_get_v3(pl, "rotation", v3_make(0,0,0));
    v.type = V_PRIM;
    ellipsoid_set(v.u.v = ellipsoid_instance(&ellipsoid_funcs), c, r, rot);
  }
  return v;
}
