#ifndef ELLIP_H
#define ELLIP_H

#include "sdltypes.h"

typedef struct Ellipsoid {
  Vector3 c;       /* centro */
  Vector3 r;       /* raios */ 
  Vector3 rot;     /* ângulos de rotação nos eixos x, y, z */
} Ellipsoid;

struct Prim *ellipsoid_set(Prim *p, Vector3 c, Vector3 r, Vector3 rot);
void ellipsoid_destroy(Prim *p);
struct Prim *ellipsoid_instance(PrimFuncs *f);
Box3d ellipsoid_bbox(Prim *p);
int ellipsoid_classify(Prim *p, Vector3 q);
Vector3 ellipsoid_point(Prim *p, Real u, Real v);
Vector3 ellipsoid_normal(Prim *p, Real u, Real v);
Vector3 ellipsoid_gradient(Prim *p, Vector3 q);
Inode *ellipsoid_intersect(Prim *p, Ray r);
Prim *ellipsoid_transform(Prim *p, Matrix4 md, Matrix4 mi);
Poly *ellipsoid_uv_decomp(Prim *p, Real level);
Vector3 ellipsoid_texc(Prim *p, Real u, Real v);
Vector3 ellipsoid_du(Prim *p, Real u, Real v);
Vector3 ellipsoid_dv(Prim *p, Real u, Real v);
Matrix4 ellipsoid_local(Prim *p);
int ellipsoid_id(Prim *p);
void ellipsoid_write(Prim *p, FILE *fp);
void ellipsoid_draw(Prim *p, int axis);
Val ellipsoid_parse(int c, Pval *pl);

extern PrimFuncs ellipsoid_funcs;

#endif
