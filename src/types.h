// Define some useful types
#ifndef _GRI_TYPES_HH_
#define _GRI_TYPES_HH_

#include <stdio.h>
#include <math.h>

enum gr_textStyle {		// Text writing options
	TEXT_LJUST,
	TEXT_RJUST,
	TEXT_CENTERED
};

enum units {			// Units for GriPath
	units_cm,
	units_user
};

enum data_file_type {		// Types of dataFILE
	FILE_FROM_CMDFILE,
	FILE_ASCII,
	FILE_BIN_UCHAR,
	FILE_BIN_INT,
	FILE_BIN_FLOAT,
	FILE_BIN_DOUBLE,
	FILE_BIN_NETCDF
};


static inline double min4(double a, double b, double c, double d) {
	double rval = a;
	if (b < a) rval = b;
	if (c < a) rval = c;
	if (d < a) rval = d;
	return rval;
}
static inline double max4(double a, double b, double c, double d) {
	double rval = a;
	if (b > a) rval = b;
	if (c > a) rval = c;
	if (d > a) rval = d;
	return rval;
}

class rectangle { // a rectangle oriented parallel to axis
 public:
	rectangle(double llx, double lly, double urx, double ury) {
		ll_x = llx;
		ll_y = lly;
		ur_x = urx;
		ur_y = ury;
	}
	rectangle(void) { ll_x = ll_y = ur_x = ur_y = 0.0; }
	void clear() { ll_x = ll_y = ur_x = ur_y = 0.0; }
	~rectangle() {}
	void set(double llx, double lly, double urx, double ury) {
		ll_x = llx;
		ll_y = lly;
		ur_x = urx;
		ur_y = ury;
	}
	double llx(void) const { return ll_x; }
	double lly(void) const { return ll_y; }
	double urx(void) const { return ur_x; }
	double ury(void) const { return ur_y; }
	void set_llx(double llx) { ll_x = llx;} 
	void set_lly(double lly) { ll_y = lly;} 
	void set_urx(double urx) { ur_x = urx;} 
	void set_ury(double ury) { ur_y = ury;} 
	void shift_x(double dx)  { ll_x += dx; ur_x += dx;}
	void shift_y(double dy)  { ll_y += dy; ur_y += dy;}
	void rotate(double degrees_ccw) {
		double rad_ccw = degrees_ccw / 57.29577951;
		double c = cos(rad_ccw);
		double s = sin(rad_ccw);
		double x1 = c * ll_x - s * ll_y;
		double y1 = s * ll_x + c * ll_y;
		double x2 = c * ur_x - s * ll_y;
		double y2 = s * ur_x + c * ll_y;
		double x3 = c * ur_x - s * ur_y;
		double y3 = s * ur_x + c * ur_y;
		double x4 = c * ll_x - s * ur_y;
		double y4 = s * ll_x + c * ur_y;
		ll_x = min4(x1, x2, x3, x4);
		ur_x = max4(x1, x2, x3, x4);
		ll_y = min4(y1, y2, y3, y4);
		ur_y = max4(y1, y2, y3, y4);
	}
	double get_llx() { return ll_x; }
	double get_lly() { return ll_y; }
	double get_urx() { return ur_x; }
	double get_ury() { return ur_y; }
 private:
	double ll_x;		// lower-left
	double ll_y;
	double ur_x;		// upper-right
	double ur_y;
};
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
	gr_plus_symbol,		/* 0 */
	gr_times_symbol,		/* 1 */
	gr_box_symbol,		/* 2 */
	gr_circ_symbol,		/* 3 */
	gr_diamond_symbol,		/* 4 */
	gr_triangleup_symbol,	/* 5 */
	gr_triangleright_symbol,	/* 6 */
	gr_triangledown_symbol,	/* 7 */
	gr_triangleleft_symbol,	/* 8 */
	gr_asterisk_symbol,		/* 9 */
	gr_star_symbol,		/* 10 */
	gr_filledbox_symbol,	/* 11 */
	gr_bullet_symbol,		/* 12 */
	gr_filleddiamond_symbol,	/* 13 */
	gr_filledtriangleup_symbol,	/* 14 */
	gr_filledtriangleright_symbol, /* 15 */
	gr_filledtriangledown_symbol, /* 16 */
	gr_filledtriangleleft_symbol, /* 17 */
	gr_filledhalfmoonup_symbol,	/* 18 */
	gr_filledhalfmoondown_symbol /* 19 */
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

// Axis properties
typedef enum {
	gr_axis_BOTTOM = 1,
	gr_axis_TOP = 0,
	gr_axis_LEFT = 1,
	gr_axis_RIGHT = 0,
	gr_axis_LOG = 0,
	gr_axis_LINEAR = 1
}               gr_axis_properties;

typedef struct {
	double          width_pt;	/* width of pen */
}               gr_pen;

#endif // _GRI_TYPES_HH_
