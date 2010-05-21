// -*- mode: c++ -*-

// gr.h  -- header file for gr and gri (Copyright 1997 Dan Kelley) You must
// #include "gr.h" at start of any C program which uses the gr library.

#ifndef _grh_
#define         _grh_

#include <string>
using namespace std;
#include        <assert.h>
#include        <stdio.h>
#include        <string.h>
#include        <math.h>
// For getpid etc.
#if HAVE_UNISTD_H
#include        <sys/types.h>
#include        <unistd.h>
#endif

// Standard libraries, malloc, etc (this confuses me)
#if STDC_HEADERS
#include        <stdlib.h>
#else
#ifndef MSDOS
#include        <malloc.h>
#endif				// not MSDOS
#endif				// Doesn't have stdlib

// Should not really need these, but on gcc 2.5.8 on my sparc
// machine, they are not prototyped.  I only prototyping them 
// for GNU cc because e.g. alpha compiler chokes on a 
// disagreement with /usr/include/stdlib.h
#if defined(__GNUC__)
extern "C" {
	int  pclose(FILE *stream);
}
#endif

#include "types.h"
#include "GriColor.h"
#include "private.h"

#if 0
#include "gr_coll.hh"
#endif
#include "GMatrix.h"

// Useful things to know.

#define	_grTempStringLEN	4096

#if 0
#define PASTE_CHAR    char(255)
#endif

// Postscript things.
#define GR_POINTS_IN_PS_PATH 1499 // really 1500, but be safe
#define PT_PER_IN 72.27		// points per inch
#define PT_PER_CM 28.45		// points per centimetre
#define DEG_PER_RAD 57.29577951	// degrees per radian

// An io buffer that resorts to file i/o only if a buffer is exhausted. Used
// by gr_textget() and gr_textsave().
typedef struct {
	FILE           *fp;		// file to read if buffer exhausted
	char           *buf;	// the buffer
	int             buf_capacity;	// chars in buffer
	int             buf_position;	// next char to read in buffer
}               FBUFFER;

#if 0
// Axis properties
typedef enum {
	gr_axis_BOTTOM = 1,
	gr_axis_TOP = 0,
	gr_axis_LEFT = 1,
	gr_axis_RIGHT = 0,
	gr_axis_LOG = 0,
	gr_axis_LINEAR = 1
}               gr_axis_properties;
#endif
#if 0

typedef struct {
	double          width_pt;	/* width of pen */
}               gr_pen;

// Font numbers.  To add new fonts, use 'extract_font_widths' perlscript to
// get size info, then incorporate into grstring.c, in a list before the
// gr_charwidth_cm() function.
// DO NOT change the =0 below, or you'll mess everything up in grstring (esp in
// the gr_drawstring() function).
typedef enum {
	gr_font_Courier = 0,	// Courier
	gr_font_CourierOblique,	// Courier-Oblique
	gr_font_CourierBold,	// Courier-Bold
	gr_font_CourierBoldOblique,	// Courier-BoldOblique
	gr_font_Helvetica,		// Helvetica
	gr_font_HelveticaOblique,	// Helvetica-Oblique
	gr_font_HelveticaBold,	// Helvetica-Bold
	gr_font_HelveticaBoldOblique,	// Helvetica-BoldOblique
	gr_font_PalatinoRoman,	// Palatino-Roman
	gr_font_PalatinoItalic,	// Palatino-Italic
	gr_font_PalatinoBold,	// Palatino-Bold
	gr_font_PalatinoBoldItalic,	// Palatino-BoldItalic
	gr_font_Symbol,		// Symbol
	gr_font_TimesRoman,		// Times-Roman
	gr_font_TimesItalic,	// Times-Italic
	gr_font_TimesBold,		// Times-Bold
	gr_font_TimesBoldItalic,	// Times-BoldItalic
	gr_font_ZapfChancery,	// Zapf-Chancery
	gr_font_ZapfChanceryMediumItalic,	// Zapf-Chancery
	gr_font_end_of_list
}               gr_fontID;

typedef struct {
	gr_fontID       id;
	char           *name;
}               gr_font_info;	// used in grstring.c for font_list

typedef struct {
	gr_fontID       id;
	double          size_pt;
}               gr_font;

// Symbol numbers
enum gr_symbol_type {
	gr_unknown_symbol = -1,
	gr_plus_symbol,
	gr_times_symbol,
	gr_box_symbol,
	gr_circ_symbol,
	gr_diamond_symbol,
	gr_triangleup_symbol,
	gr_triangleright_symbol,
	gr_triangledown_symbol,
	gr_triangleleft_symbol,
	gr_asterisk_symbol,
	gr_star_symbol,
	gr_filledbox_symbol,
	gr_bullet_symbol,
	gr_filleddiamond_symbol,
	gr_filledtriangleup_symbol,
	gr_filledtriangleright_symbol,
	gr_filledtriangledown_symbol,
	gr_filledtriangleleft_symbol,
	gr_filledhalfmoonup_symbol,
	gr_filledhalfmoondown_symbol
};

// Color.  Is this used??
typedef enum {
	bw_model, rgb_model, hsb_model
}               gr_color_model;

typedef struct {
	double          red;
	double          green;
	double          blue;
	double          hue;
	double          saturation;
	double          brightness;
	gr_color_model  color_model;
}               gr_color;


// Function headers.
#endif
void            abort_gri(void);
void            gr_begin(int specifications);
#if 0
bool            gr_buffgets(char *s, unsigned int most, FBUFFER * fbuf);
#endif
void            gr_cmtouser(double x_cm, double y_cm, double *x, double *y);
#if 0
void            gr_comment(const char *message);
#endif
void            gr_contour(const std::vector<double> x,
			   const std::vector<double> y,
			   /*const*/ GriMatrix<double> &z,
			   /*const*/ GriMatrix<bool> &legit,
			   int nx,
			   int ny,
			   double z0,
			   const char *lab,
			   bool rotate_labels,
			   bool whiteunder_labels,
			   bool center_labels,
			   const GriColor& line_color,
			   const GriColor& text_color,
			   double contour_minlength,
			   double contour_space_first,
			   double contour_space_later,
			   FILE * out_file,
                           bool filled);
gr_fontID       gr_currentfont(void);
double          gr_currentCapHeight_cm(void);	// From font metric info
double          gr_currentXHeight_cm(void);	// From font metric info
double          gr_currentAscender_cm(void);	// From font metric info
double          gr_currentDesscender_cm(void);	// From font metric info
double          gr_currentfontsize_pt(void);
#define gr_currentfontsize_cm() (gr_currentfontsize_pt() / PT_PER_CM)
double          gr_currentmissingvalue(void);
char           *gr_currentPSfilename(void);
#if 0
FILE           *gr_currentPSFILEpointer(void);
bool            gr_current_ps_landscape(void);
#endif
double          gr_currentsymbolsize_pt(void);
#define gr_currentsymbolsize_cm() (gr_currentsymbolsize_pt() / PT_PER_CM)
#if 0
double          gr_currentticsize_cm(void);
#endif
void            gr_drawimage(unsigned char *im, unsigned char *imTransform, gr_color_model color_model, unsigned char *mask, double mask_r, double maskg, double mask_b, int imax, int jmax, double llx_cm, double lly_cm, double urx_cm, double ury_cm, bool insert_placer);
void            gr_drawarrow_cm(double x, double y, double xend, double yend, double halfwidth);
void            gr_drawarrow2_cm(double x, double y, double xend, double yend, double halfwidth);
void            gr_drawarrow3_cm(double x, double y, double xend, double yend, double halfwidth);

#if 0
void            gr_drawerrorbars(double x, double xmin, double xmax, double y, double ymin, double ymax, int type);
#endif
void            gr_drawsymbol(double xcm, double ycm, gr_symbol_type symbol_name);
void            gr_drawxaxis(double y, double xl, double xinc, double xr, gr_axis_properties side);
void            gr_drawxyaxes(double xl, double xinc, double xr, double yb, double yinc, double yt);
void            gr_drawyaxis(double x, double yb, double yinc, double yt, gr_axis_properties side);
void            gr_end(const char *filename);
void            gr_error(const char *lab);
#if 0
//
// Gridding routines.
int             gr_grid1(double *x, double *y, double *f, unsigned int n, double x0, double y0, double xRadius, double yRadius, int method, unsigned int neighbors, int enlargements, double *fOut);

#endif
void            gr_hsv2rgb(double h, double s, double br, double *r, double *g, double *b);

#if 0
void            gr_lastxy(double *x, double *y);
#endif
bool            gr_missing(double x);
bool            gr_missingx(double x);
bool            gr_missingy(double y);
void            gr_moveto_cm(double x_cm, double y_cm);
bool            gr_multiple(double x, double d, double precision);
bool            gr_onxpage_cm(double x_cm);
bool            gr_onypage_cm(double y_cm);
double          gr_quad_cm();	// width of "M"
void            gr_record_handle(double x_cm, double y_cm);
void            gr_record_scale(void);
#if 0
bool            gr_reopen_postscript(const char *new_name);
void            gr_rgb2hsb(double r, double g, double b, double *h, double *s, double *br);
#endif
void            gr_rmoveto_cm(double rx_cm, double ry_cm);
void            gr_rmoveto_pt(double rx_pt, double ry_pt);
void            gr_rotate_xy(double x, double y, double angle, double *xx, double *yy);
void            gr_save_postscript(const char *PS_name, int normal_scale);
void            gr_scale125(double xl, double xr, int n, double *xlr, double *xrr, int *nr);
// Routines to set various things
void            gr_setfont(gr_fontID newID);
#define gr_setfontsize_cm(f) (gr_setfontsize_pt(f * PT_PER_CM))
void            gr_setfontsize_pt(double new_fontsize_pt);
void            gr_setmissingvalue(double missingvalue);

#if 0
void            gr_setscale(double xfactor, double yfactor);
#endif
#define gr_setsymbolsize_cm(s) (gr_setsymbolsize_pt(s * PT_PER_CM))
void            gr_setsymbolsize_pt(double size_pt);
void            gr_setticdirection(bool tics_point_in);
void            gr_setticsize_cm(double newsize_cm);
#if 0
void            gr_settranslate(double xcm, double ycm);
#endif
void            gr_setup_creatorname(const char *s);
void            gr_setup_ps_filename(const char *new_name);
void            gr_setup_ps_landscape(void);
#if 0
void            gr_setup_ps_scale(double xfactor, double yfactor);
void            gr_setxaxisstyle(int xstyle);
#endif
//REMOVED//void            gr_setxlabel(const char *xlab);
//REMOVED-2000-5-2 void            gr_setxnumberformat(const char *xformat);
void            gr_setxpagesize_cm(double x_cm);
void            gr_setxscale(double xl_cm, double xr_cm, double xl, double xr);
void            gr_setxsubdivisions(int num);
void            gr_setxtransform(gr_axis_properties xstyle);
void            gr_setyaxisstyle(int ystyle);
//REMOVED//void            gr_setylabel(const char *ylab);
//REMOVED-2000-5-2 void            gr_setynumberformat(const char *yformat);
void            gr_setypagesize_cm(double y_cm);
void            gr_setyscale(double yb_cm, double yt_cm, double yb, double yt);
void            gr_setysubdivisions(int num);
void            gr_setytransform(gr_axis_properties ystyle);
void            gr_show_at(const char *lab, double xcm, double ycm, gr_textStyle style, double angle);
void            gr_show_in_box(const string& s, const GriColor& text_color, const GriColor& box_color, double x, double y, double angle_deg);
void            gr_showpage(void);
#if 0
bool            gr_smootharray(double dx, double dy, double dt, GriMatrix<double> &z, GriMatrix<double> &zS, GriMatrix<bool> &legit, GriMatrix<bool> &legitS, int nx, int ny, int method);
#endif
void            gr_stringwidth(const char *s, double *x_cm, double *ascent_cm, double *descent_cm);
#if 0
bool            gr_textget(char *s, int max);
bool            gr_textsave(const char *s);
void            gr_textput(const char *s);
void            gr_textput2(const char *s);
#endif
double          gr_thinspace_cm();	// 1/6 width of "M"

void            gr_usertocm(double x, double y, double *x_cm, double *y_cm);
double          gr_usertocm_x(double x, double y);
double          gr_usertocm_y(double x, double y);
void            gr_usertopt(double x, double y, double *x_pt, double *y_pt);

// Macros

// Pin number to a range
#if !defined(pin0_1)
#define pin0_1(x)  	(x < 0 ? 0 : (x <   1 ? x :   1))
#endif
#if !defined(pin0_255)
#define pin0_255(x)	(x < 0 ? 0 : (x < 255 ? x : 255))
#endif

// Allocate storage, printing file/line if cannot
#if !defined(GET_STORAGE)
#define GET_STORAGE(var, type, num)					\
{									\
    if ((num) > 0) {							\
	if (! ((var) = ( type *) malloc( (num) * sizeof(type)))) {	\
	    gr_Error("Out of memory");					\
	}								\
    } else {								\
	gr_Error("Cannot allocate zero or fewer bytes of storage");	\
    }									\
}
#endif // GET_STORAGE

#define interpolate_linear(x,  x0,  y0,  x1,  y1)                       \
    ((y0) + ((x) - (x0)) * ((y1) - (y0)) / ((x1) - (x0)))


#endif				// not _gr_
