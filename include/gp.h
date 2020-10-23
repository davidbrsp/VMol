/*
* gp.h
* a simple 2d graphics package
* Luiz Henrique de Figueiredo (lhf@visgraf.impa.br)
*/

#ifndef GP_H
#define GP_H

#define real	double
#define Box	gpBox

typedef struct {
 real xmin;
 real xmax;
 real ymin;
 real ymax;
 real xu;
 real yu;
} Box;


real	gpopen		(char* name);
void	gpclose		(int wait);
void	gpclear		(int wait);
void	gpflush		(void);
void	gpwait		(int t);
int	gppalette	(int c, char* name);
int	gprgb		(int c, real r, real g, real b);
int	gpcolor		(int c);
int	gpfont		(char* name);
void	gpmark		(int size, char* mark);
void	gpline		(real x1, real y1, real x2, real y2);
void	gpbox		(real xmin, real xmax, real ymin, real ymax);
void	gptri		(real x1, real y1, real x2, real y2, real x3, real y3);
void	gptext		(real x, real y, char* s, char* mode);
void	gpcircle	(real x, real y, real r);
void	gpplot		(real x, real y);
void	gpbegin		(int c);
int	gppoint		(real x, real y);
void	gpend		(void);
char*	gpevent		(int wait, real* x, real* y);
real	gpwindow	(real xmin, real xmax, real ymin, real ymax);
real	gpviewport	(real xmin, real xmax, real ymin, real ymax);
void	gpview		(real* x, real* y);
void	gpunview	(real* x, real* y);
void	gpmake		(void);

#define	gpbegin		dvbegin
#define	gpclear		dvclear
#define	gpclose		dvclose
#define	gpcolor		dvcolor
#define	gpend		dvend
#define	gpflush		dvflush
#define	gpfont		dvfont
#define	gpmark		dvmark
#define	gppalette	dvpalette
#define	gprgb		dvrgb
#define	gpwait		dvwait

#define	gpdoublebuffer  dvdoublebuffer
#define	gpswapbuffers   dvswapbuffers
#define	gpfrontbuffer   dvfrontbuffer
#define	gpbackbuffer    dvbackbuffer
#define	gpbufferarea    dvbufferarea

#define gpsetsize        dvsetsize

/* device */

Box*	dvopen		(char* name);
void	dvclose		(int wait);
void	dvclear		(int wait);
void	dvflush		(void);
void	dvwait		(int t);
int	dvpalette	(int c, char* name);
int	dvrgb		(int c, real r, real g, real b);
int	dvcolor		(int c);
int	dvfont		(char* name);
void	dvmark		(int size, char* mark);
void	dvclip		(int xmin, int xmax, int ymin, int ymax);
void	dvline		(int x1, int y1, int x2, int y2);
void	dvbox		(int xmin, int xmax, int ymin, int ymax);
void	dvtri		(int x1, int y1, int x2, int y2, int x3, int y3);
void	dvtext		(int x, int y, char* s, char* mode);
void	dvcircle	(int x, int y, int r);
void	dvplot		(int x, int y);
void	dvbegin		(int c);
int	dvpoint		(int x, int y);
void	dvend		(void);
char*	dvevent		(int wait, int* x, int* y);

void    dvsetsize       (int w, int h, int x, int y);

void    dvdoublebuffer  (int on);
void    dvswapbuffers   (void);
void    dvfrontbuffer   (void);
void    dvbackbuffer    (void);
void    dvbufferarea    (int xmin, int xmax, int ymin, int ymax);


#ifndef rad
#define rad(a)		((a)*(real)0.01745329252)
#endif

#ifndef WIN32
#ifdef __DARWIN_UNIX03
extern double round( double );
#else
#ifndef round 
#define round(x)	((int)((x)+(real)0.5))
#endif
#endif
#endif

#ifndef min
#define	min(x,y)	( ((x)<(y)) ? (x) : (y) )
#endif

#ifndef max
#define	max(x,y)	( ((x)>(y)) ? (x) : (y) )
#endif


/* ***********************************************************************
 * Let windows aplications start in function "main", just like regular C *
 * ************************************************************************/

#if !defined (_CONSOLE) 
#if defined(_WINDOWS) || defined(WIN32) || defined(__WINDOWS__) || defined(__NT__)

int gpmain (int argc, char** argv);

#define main gpmain /* define main as gpmain */

#endif /* _WINDOWS || WIN32 || __WINDOWS__ || __NT__ */
#endif /* _CONSOLE */

#endif // GP_H
