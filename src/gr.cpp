#define NEW_TRANSFORM  1	// 11jun95, try new mapping method

// Global variable/function names begin with _gr.

// Sections of uncertain or buggy code are sometimes marked by 
// comments containing the strings "BUG" or "??".

// Cross-dependent code is sometimes marked with comments 
// containing the string XREF.


#include	<ctype.h>
#include	<string.h>
#include        <time.h>
#include        <math.h>
#include        <stdio.h>
#include        <stdlib.h>

// Local includes
#include	"gr.h"
#include	"errors.h"
#include        "postscpt.h"
#include        "private.h"
#include        "macro.h"
#include        "GriPath.h"
#include        "GriState.h"
#include        "defaults.h"
#include        "superus.h"

static void     close_ps_file(FILE * fp);
static void     handle_landscape_scale(FILE * fp);
static void     insert_ps_header(FILE * fp);
static void     skip_ps_header(FILE * PSfile);
static char     creator_name[256] = "";
static char     ps_filename[256] = "gre.ps";	/* basename */
static char     ps_filename_used[256] = "";	/* actual name used */
static bool     user_gave_ps_filename = false;	/* did user give the name? */
static int      which_page = 0;	/* page counter */

bool            _no_bounding_box = false;


extern GriState _griState;	// <-> gri.cc

//
// Utilities.
//

// gr_multiple - is x a multiple of d (to given precision)?
bool
gr_multiple(double x, double d, double precision)
{
	double          epsilon;
	if (d < 0.0)
		d = -d;
	if (x < 0.0)
		x = -x;
	if (precision < 0.0)
		precision = -precision;
	epsilon = GRI_ABS(x - d * (floor(0.5 + x / d)));
	return ((epsilon < precision) ? true : false);
}

//
// UNIVERSAL GLOBALS
#define	default_linewidth	0.5
#define	LEN_num			50
#define	LEN_lab			256
gr_pen          _grCurrentPen;
//REMOVED//char            _grXAxisLabel[LEN_lab];	         // name of x axis
//REMOVED//char            _grYAxisLabel[LEN_lab];		 // name of y axis
//REMOVED-2000-5-2 char            _grNumFormat_x[LEN_num];	 // format for x axis
//REMOVED-2000-5-2 char            _grNumFormat_y[LEN_num];	 // format for y axis
char            grTempString[_grTempStringLEN];	 // local scratch string
char            grTempString2[_grTempStringLEN]; // local scratch string

/*
 * stdin buffer
 */
FBUFFER         stdin_buffer =
{stdin, 0, 0, 0};		/* note 0 for buffer */

/* GLOBAL scratch strings */
char            _grTempString[_grTempStringLEN];
char            _grTempString2[_grTempStringLEN];
double
_grCmPerUser_y = 1.0,	/* y cm-on-page / user-unit */
	_grCmPerUser_x = 1.0,	/* x cm-on-page / user-unit */
	_grCurrentPoint_x,	/* current x coordinate of pen */
	_grCurrentPoint_y,	/* current y coordinate of pen */
	_grGrayScreenQuantum = .0625,	/* gray step on mac */
	_grMagx = 1.0,	/* x magnification on screen */
	_grMagy = 1.0,	/* y magnification on screen */
	_grMissingValue = 1.0e22,	/* missing value */
	_grMissingValue_tolerance = 1.0e20,	/* tolerance */
	_grOriginx = 0.0,	/* x origin on screen */
	_grOriginy = 0.0,	/* y origin on screen */
	_grPageHeight_cm = 27.94,	/* page height in cm */
	_grPageWidth_cm = 21.59,	/* page width in cm */
	_grPSScale_x = 1.0,	/* _Mag factor for x scale on ps */
	_grPSScale_y = 1.0,	/* _Mag factor for y */
	_grPtPerUser_x = PT_PER_CM,	/* x pt-on-page / user-unit
					 * (assume cm) */
	_grPtPerUser_y = PT_PER_CM,	/* y pt-on-page / user-unit
					 * (assume cm) */
	_grSymbolSize_pt,	/* symbol size in point */
	_grTicSize_cm,	/* axis tic size */
	_grPROJx0, _grPROJxfac, _grPROJy0, _grPROJyfac,
	_grxl = 0.0,	/* x left - user units */
	_grxl_pt = 0.0,	/* x left - points */
	_gryb = 0.0,	/* y bottom - user units */
	_gryb_pt = 0.0	/* y bottom - points */
;

double _ll_x_pt, _ll_y_pt, _ur_x_pt, _ur_y_pt; // lower-left,  point units
double _ll_x_us, _ll_y_us, _ur_x_us, _ur_y_us; // upper-right,  user units

gr_axis_properties _grTransform_x = gr_axis_LINEAR;
gr_axis_properties _grTransform_y = gr_axis_LINEAR;

bool            _grTicsPointIn = false;
int             _grAxisStyle_x = 0;	/* always 0 */
int             _grAxisStyle_y = 0;	/* 0 = falsermal, 1 = label horizontal */
bool            _grDrawingDash = false;	/* =1 if drawing dash part of a line */
bool            _grPS_Landscape = false;	/* flag for landscape */
int             _grSpecifications = 1;	/* specifications in gr_begin */
int             _grNumSubDiv_x;	/* # x-subdivision per labelled tic */
int             _grNumSubDiv_y;	/* # x-subdivision per labelled tic */
bool            _grNeedBegin = true;	/* Need to call gr_begin()? */
bool            _grPathExists = false;	/* does a postscript 'path' exist? */
bool            _grWritePS = true;	/* ==1 if postscript to be written */

FILE           *_grPS;		// file of postscript output
// Local prototypes.
static void     set_page_characteristics();
#if 0

// Always returns true
bool
gr_textsave(const char *s)
{
	static bool     first = true;
	unsigned int    len;
	len = strlen(s);
	if (first == true) {
		stdin_buffer.fp = stdin;
		GET_STORAGE(stdin_buffer.buf, char, 2 + len);
		strcpy(stdin_buffer.buf, s);
		first = false;
	} else {
		stdin_buffer.buf = (char *)realloc(stdin_buffer.buf,
						   2 + len + stdin_buffer.buf_capacity);
		strcat(stdin_buffer.buf, s);
	}
	strcat(stdin_buffer.buf, "\n");
	stdin_buffer.buf_capacity += 1 + len;
	return true;
}

/*
 * gr_textget (s, max) Get a line of text.  If the line ends in backslash,
 * get next line and concatenate the two together; if succeeding lines end in
 * backslash, continue the contatenation. RETURN VALUE 1 if got text; 0 if
 * got EOF.
 */
bool
gr_textget(char *s, int max)
{
	int             len, most = max;
	bool            got_eof;
	got_eof = gr_buffgets(s, most, &stdin_buffer);
	if (got_eof) {
		/*
		 * No newline at end of file.  Paste one on.
		 */
		strcat(s, "\n");
		return false;
	} else {
		while ((len = strlen(s)) > 1 && s[len - 2] == '\\' && most > 0) {
			most += len - 1;
			got_eof = gr_buffgets(s + len - 2, most, &stdin_buffer);
			if (got_eof) {
				/*
				 * No newline at end of file.  Paste one on.
				 */
				strcat(s, "\n");
				return true;
			}
		}
		return true;
	}
}

/*
 * Get string from io buffer, or from io file, if buffer empty.  Stop reading
 * when newline or eof encountered.
 * 
 * Return true if got to eof of the file.
 */
bool
gr_buffgets(char *s, unsigned int most, FBUFFER * fbuf)
{
	if (fbuf->buf_position < fbuf->buf_capacity) {
		unsigned int             i;
		for (i = 0; i < most - 1; i++) {
			s[i] = fbuf->buf[fbuf->buf_position++];
			if (s[i] == '\n')
				break;
			if (fbuf->buf_position >= fbuf->buf_capacity)
				break;		/* will lose some characters, but no warning */
		}
		s[i + 1] = '\0';
	} else {
		fgets(s, most, (FILE *) fbuf->fp);
		if (feof(fbuf->fp))
			return true;
	}
	return false;
}

// print string, allowing for TAB/NEWLINE/... escapes
void
gr_textput(const char *s)
{
	fflush(stdout);
	const char *c = s;
	while (*c) {
		if (*c == '\\' && *(c + 1) == 'n')
			putchar('\n'), c++;
		else if (*c == '\\' && *(c + 1) == '\\')
			putchar('\\'), c++;
		else if (*c == '\\' && *(c + 1) == 't')
			putchar('\t'), c++;
		else
			putchar(*c);
		c++;
	}
}

// print string, NOT allowing for (\n, \t) escapes
void
gr_textput2(const char *s)
{
	const char           *c = s;
	while (*c) {
		if (*c == '\n') {
			putchar('\\');
			putchar('n');
		} else if (*c == '\t') {
			putchar('\\');
			putchar('t');
		} else {
			putchar(*c++);
		}
	}
}
#endif

// gr_begin() -- prepare for plotting
// `specifications' is a code used to specify various options. you 
// form specifications as a sum of codes defined in gr.hh.
void
gr_begin(int specifications)
{
	int             version;
	_grNeedBegin = false;
	_grSpecifications = specifications;
	// open gre.ps file, avoiding overwriting existing ones
	if (user_gave_ps_filename)
		strcpy(ps_filename_used, ps_filename);
	else {
#if defined(VMS)
		// On vax, just use version numbers
		sprintf(ps_filename_used, "%s", ps_filename);
#else
		for (version = 0; version < 9999; version++) {
			FILE           *trial;
			sprintf(ps_filename_used, "gre-%02d.ps", version);
			trial = fopen(ps_filename_used, "r");
			if (NULL == trial)
				break;
			fclose(trial);
		}
#endif
	}
	_grPS = fopen(ps_filename_used, "w+");
	// if can't open file, try again
	if (_grPS == NULL) {
		int             ok_after_all = false;
#if defined(HAVE_GETENV)
		char            homename[100];
		if (getenv("home")) {
			strcpy(homename, getenv("home"));
			strcat(homename, "/gre.ps");
			strcpy(ps_filename_used, homename);
			_grPS = fopen(ps_filename_used, "w+");
			if (_grPS == NULL) {
				sprintf(_grTempString, "\
Cannot open output PostScript file named\n\t`%s'\nin this directory.  Do you have write permission here?\n", ps_filename_used);
				gr_textput(_grTempString);
				gre_exit(1);
			} else {
				ok_after_all = 1;
			}
		}
#endif
		if (ok_after_all) {
			sprintf(_grTempString, "Couldn't create intended postscript file, so created `%s' instead\n", ps_filename_used);
			warning(_grTempString);
		} else {
			sprintf(_grTempString, "\
Cannot open output PostScript file named\n\t`%s'\nin this directory.  Do you have write permission here?\n", ps_filename);
			gr_textput(_grTempString);
			gre_exit(1);
		}
	}
	// write some header stuff in postscript file
	_grWritePS = true;
	// define postscript abbreviations
	insert_ps_header(_grPS);
	set_page_characteristics();
}

static void set_page_characteristics()
{
	gr_setfont(gr_font_Helvetica);
	gr_setfontsize_pt(-1.0);
	_griState.set_linewidth_line(LINEWIDTH_DEFAULT);
	gr_setsymbolsize_cm(0.2);
	gr_setticdirection(false);	// point out
	gr_setticsize_cm(0.2);
	//REMOVED//gr_setxlabel("x");
	//REMOVED//gr_setylabel("y");
	//REMOVED-2000-5-2 gr_setxnumberformat("%g");
	//REMOVED-2000-5-2 gr_setynumberformat("%g");
	gr_setxpagesize_cm(21.59);
	gr_setypagesize_cm(27.94);
	gr_setxtransform(gr_axis_LINEAR);
	gr_setytransform(gr_axis_LINEAR);
	gr_setxscale(0.0, 1.0, 0.0, 1.0);
	gr_setyscale(0.0, 1.0, 0.0, 1.0);
	gr_setxsubdivisions(1);
	gr_setysubdivisions(1);
	_grPathExists = true;
}

static void
insert_ps_header(FILE * fp)
{
	/*
	 * write conforming postscript prolog
	 */
	/* fprintf(fp, "%%!PS-Adobe-1.0\n"); */
	fprintf(fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");
	fprintf(fp, "%%%%Creator: %s\n", creator_name);
	fprintf(fp, "%%%%Title: %s\n", ps_filename_used);
#if defined(VMS) || defined(MSDOS) || defined(IS_DEC_ALPHA)
	time_t          sec;
#else
#if defined(__DECCXX)
	int             sec;
#else
	long            sec;
#endif
#endif
	time(&sec);
	fprintf(fp, "%%%%CreationDate: %s", asctime(localtime(&sec)));
	fprintf(fp, "%%%%Pages: (atend)\n");
	fprintf(fp, "%%%%BoundingBox: (atend)\n");
	fprintf(fp, "%%%%TemplateBox: %d %d %d %d\n",
		0, 0, (int) (8.5 * 72.0), (int) (11.0 * 72.0));
	fprintf(fp, "%%%%DocumentFonts: (atend)\n");
	fprintf(fp, "%%%%Endcomments\n");
	int i = 0;
	while (PS_dict[i])
		fprintf(fp, "%s\n", PS_dict[i++]);
	handle_landscape_scale(fp);
	fprintf(fp, "10.0 M\n");
	fprintf(fp, "1 j\n");
	fprintf(fp, "%%%%EndProlog\n");
	which_page++;
	fprintf(_grPS, "%%%%Page: %d %d\n", which_page, which_page);
}

static void
handle_landscape_scale(FILE * fp)
{
	/* put landscape and scale commands in ps output file */
	if (_grPS_Landscape)
		fprintf(fp, "%g 0 translate 90 rotate %% Landscape\n", 8.5 * 72.0);
	fprintf(fp, "%g %g scale\n", _grPSScale_x, _grPSScale_y);
}

// gr_cmtouser() -- convert cm on page to user units.  COMPARE gr_usertopt(),
// which is the inverse
void
gr_cmtouser(double x_cm, double y_cm, double *x, double *y)
{
	*x = *y = 0.0;
	// XREF -- axis transform
	double          xx = 0.0, yy = 0.0;
	// Do X.
	switch (_grTransform_x) {
	case gr_axis_LINEAR:
		*x = _grxl + (x_cm - _grxl_pt / PT_PER_CM) / _grCmPerUser_x;
		break;
	case gr_axis_LOG:
		*x = _grxl * pow(10.0, (x_cm - _grxl_pt / PT_PER_CM) / _grCmPerUser_x);
		break;
	default:
		gr_Error("unknown axis mapping (internal error)");
		break;
	}
	// Do Y.
	switch (_grTransform_y) {
	case gr_axis_LINEAR:
		*y = _gryb + (y_cm - _gryb_pt / PT_PER_CM) / _grCmPerUser_y;
		break;
	case gr_axis_LOG:
		*y = _gryb * pow(10.0, (y_cm - _gryb_pt / PT_PER_CM) / _grCmPerUser_y);
		break;
	default:
		gr_Error("unknown axis mapping (internal error)");
		break;
	}
}

// gr_comment() -- write message in postscript file (without inserting
// newline)
void
gr_comment(const char *message)
{
	if (_grWritePS)
		fprintf(_grPS, PS_comment, message);
}

/*
 * gr_currentPSfilename() -- get name of Postscript file SYNOPSIS char
 * *gr_currentPSfilename(); DESCRIPTION Returns pointer to name of Postscript
 * file
 */
char *
gr_currentPSfilename()
{
	return ps_filename_used;
}
#if 0

/*
 * gr_currentPSFILEpointer() -- get pointer Postscript file SYNOPSIS char
 * *gr_currentPSFILEpointer(); DESCRIPTION Returns pointer to Postscript file
 */
FILE           *
gr_currentPSFILEpointer()
{
	return _grPS;
}

/*
 * gr_current_ps_landscape() -- get present landscape mode RETURN VALUE 0 if
 * portrait; 1 if landscape
 */
bool
gr_current_ps_landscape()
{
	return _grPS_Landscape;
}
#endif

/*
 * gr_currentmissingvalue() -- return current missing value
 */
double
gr_currentmissingvalue()
{
	return _grMissingValue;
}

/*
 * gr_currentsymbolsize_pt() -- return current symbol size in points
 */
double
gr_currentsymbolsize_pt()
{
	return _grSymbolSize_pt;
}
#if 0

/*
 * gr_currentticsize_cm() -- get current axis ticsize in cm
 */
double
gr_currentticsize_cm()
{
	return _grTicSize_cm;
}
#endif

// gr_drawarrow_cm -- Draw a stroke-line arrow
// If halfwidth>0, width of arrow head is 2*halfwidth.
// If halfwidth<0, width of arrow head is 2*halfwidth*length_of_arrow.
void
gr_drawarrow_cm(double x, double y, double xend, double yend, double halfwidth)
{
	double length = (xend - x) * (xend - x) + (yend - y) * (yend - y);
	if (length <= 0.0)
		return;
	length = sqrt(length);
	if (halfwidth < 0)
		halfwidth = -halfwidth * length;
	halfwidth /= length;
	static GriPath p(5);	// use static for reuse

	p.clear();
	p.push_back(0.0, 0.0, 'm');
	p.push_back(1.0, 0.0, 'l');
	p.push_back(1.0 - 3.0 * halfwidth, halfwidth, 'm');
	p.push_back(1.0, 0.0, 'l');
	p.push_back(1.0 - 3.0 * halfwidth, -halfwidth, 'l');
	p.rotate(atan2(yend - y, xend - x) * DEG_PER_RAD);
	p.scale(length);
	p.translate(x, y);
	p.stroke(units_cm);
}

// gr_drawarrow2_cm -- Draw an outlined arrow
// If halfwidth>0, width of arrow head is 2*halfwidth.
// If halfwidth<0, width of arrow head is 2*halfwidth*length_of_arrow.
void
gr_drawarrow2_cm(double x, double y, double xend, double yend, double halfwidth)
{
	double length = (xend - x) * (xend - x) + (yend - y) * (yend - y);
	if (length <= 0.0)
		return;			// ignore zero length arrows
	length = sqrt(length);
	if (halfwidth < 0)
		halfwidth = -halfwidth * length;
	static GriPath p(9);	// use static for reuse
	p.clear();
	halfwidth /= length;
	p.push_back(0.0,			 0.4 * halfwidth, 	'm');
	p.push_back(0.0,			-0.4 * halfwidth, 	'l');
	p.push_back(1.0 - 3. * halfwidth,	-0.4 * halfwidth,	'l');
	p.push_back(1.0 - 3. * halfwidth,	-halfwidth,		'l');
	p.push_back(1.0,		         0.0,			'l');
	p.push_back(1.0 - 3. * halfwidth,	 halfwidth,		'l');
	p.push_back(1.0 - 3. * halfwidth,	 0.4 * halfwidth,	'l');
	p.push_back(0.0,			 0.4 * halfwidth, 	'l');
	p.push_back(0.0,			-0.4 * halfwidth, 	'l');
	p.rotate(atan2(yend - y, xend - x) * DEG_PER_RAD);
	p.scale(length);
	p.translate(x, y);
	p.stroke(units_cm);
}

// gr_drawarrow3_cm -- Draw swept-back filled arrow
//
// If halfwidth>0, width of arrow head is 2*halfwidth.
// If halfwidth<0, width of arrow head is 2*halfwidth*length_of_arrow.
void
gr_drawarrow3_cm(double x, double y, double xend, double yend, double halfwidth)
{
	// METHOD: define arrow on unit square from (0,0) to (1,0) and then
	// rotate and scale as needed.

	double length = (xend - x) * (xend - x) + (yend - y) * (yend - y);
	if (length <= 0.0)
		return;			// ignore zero length arrows
	length = sqrt(length);
	if (halfwidth < 0)
		halfwidth = -halfwidth * length;
	static GriPath p(5);	// static to allow re-use of storage
	halfwidth /= length;
	
	// Determine angle of rotation, in degrees.
	double angle_deg = atan2(yend - y, xend - x) * DEG_PER_RAD;

	// ... fill, and then stroke, the arrow part ...
	p.clear();			// clear storage

	p.push_back(1 - 2.25 * halfwidth, 0.0, 'm');
	p.push_back(1 - 3. * halfwidth,  halfwidth, 'l');
	p.push_back(1.0, 0.0, 'l');
	p.push_back(1 - 3. * halfwidth, -halfwidth, 'l');
	p.push_back(1 - 2.25 * halfwidth, 0.0, 'l');

	p.rotate(angle_deg);
	p.scale(length);
	p.translate(x, y);
	p.fill(units_cm);
	p.stroke(units_cm);

	// ... then stroke the line.
	p.clear();			// clear storage
	p.push_back(0.0, 0.0, 'm');
	p.push_back(1.0, 0.0, 'l');
	p.rotate(angle_deg);
	p.scale(length);
	p.translate(x, y);
	p.stroke(units_cm);
}

#if 0
/*
 * gr_drawerrorbars() -- draw error bars about a point SYNOPSIS void
 * gr_drawerrorbars (double x, double xmin, double xmax, double y, double
 * ymin, double ymax, int type); DESCRIPTION: Draws error bars extending from
 * xmin to xmax and ymin to ymax. The type of error bars is controlled by
 * 'type' as follows:
 * 
 * 0 -> error bars done as lines drawn between the limits
 * 
 * 1 -> right-angled tags added at ends of limits (NOT IMPLEMENTED) NOTE: The
 * pen is left at (x,y), so that to draw a symbol with error bars, do
 * something like this: gr_drawerrorbars(x,x-dx,x+dx,y,y-dy,y+dy,0);
 * gr_drawsymbol(...,gr_circ_symbol);
 */
void
gr_drawerrorbars(double x, double xmin, double xmax, double y, double ymin, double ymax, int type)
{
	GriPath p(5);
	switch (type) {
	case 0:
	default:
		p.push_back(xmin, y, 'm');
		p.push_back(xmax, y, 'l');
		p.push_back(x, ymin, 'm');
		p.push_back(x, ymax, 'l');
		p.push_back(x, y, 'm');
		p.stroke(units_user);
		break;
	}
}
#endif

// gr_drawsymbol() -- draw symbol at indicated location in cm
// BUG: remove the "n" and "S" when newpath/stroke business sorted
void
gr_drawsymbol(double xcm, double ycm, gr_symbol_type symbol_name)
{
	double          old_grSymbolSize_pt;
	extern bool _warn_offpage;
	if (_warn_offpage 
	    && ( xcm < OFFPAGE_LEFT 
		 || xcm > OFFPAGE_RIGHT
		 || ycm < OFFPAGE_BOTTOM
		 || ycm > OFFPAGE_TOP)) {
		warning("Drawing a symbol at a location that is offpage.");
	}
	double xpt = xcm * PT_PER_CM;
	double ypt = ycm * PT_PER_CM;
	switch (symbol_name) {
	case gr_plus_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _plus S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_times_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _times S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_box_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _box S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_circ_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _circ S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_diamond_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _diamond S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_triangleup_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangleup S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_triangleright_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangleright S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_triangledown_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangledown S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_triangleleft_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangleleft S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_asterisk_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _plus S\n", xpt, ypt, _grSymbolSize_pt);
		fprintf(_grPS, "n %.1f %.1f m %.1f _times S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_star_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangleup S\n", xpt, ypt, _grSymbolSize_pt);
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangledown S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledbox_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledbox S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_bullet_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _bull S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filleddiamond_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filleddiamond S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledtriangleup_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledtriangleup S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledtriangleright_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledtriangleright S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledtriangledown_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledtriangledown S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledtriangleleft_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledtriangleleft S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledhalfmoonup_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledhalfmoonup S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	case gr_filledhalfmoondown_symbol:
		fprintf(_grPS, "n %.1f %.1f m %.1f _filledhalfmoondown S\n", xpt, ypt, _grSymbolSize_pt);
		break;
	default:			// tiny (1.0 mm) triangle
		old_grSymbolSize_pt = gr_currentsymbolsize_pt();
		gr_setsymbolsize_cm(0.1);
		fprintf(_grPS, "n %.1f %.1f m %.1f _triangledown S\n", xpt, ypt, _grSymbolSize_pt);
		gr_setsymbolsize_pt(old_grSymbolSize_pt);
		break;
	}
	rectangle box(xcm - _grSymbolSize_pt / 2 / PT_PER_CM,
		      ycm - _grSymbolSize_pt / 2 / PT_PER_CM,
		      xcm + _grSymbolSize_pt / 2 / PT_PER_CM,
		      ycm + _grSymbolSize_pt / 2 / PT_PER_CM);
	bounding_box_update(box);
}

//
// gr_end() -- end this plot
// DESCRIPTION: End current plot. If filename == "" then the window stays
// open, and the gri-00.ps PostScript file is closed. If filename == "!" then
// the window is closed.  If the filename is a valid filename, then the
// window stays open, and the PostScript file is copied into the indicated
// file. If the filename is "!" followed by a valid filename, then the window
// is closed and the PostScript is copied to the indicated file.
void
gr_end(const char *filename)
{
	extern bool     _grNeedBegin;
	extern bool     _grPathExists;
	extern FILE    *_grPS;
	if (_grNeedBegin)
		return;
	fprintf(_grPS, PS_showpage);
	fprintf(_grPS, "%%%%Trailer\n");
	if (_no_bounding_box) {	// use fullpage
		fprintf(_grPS, "%%%%BoundingBox: %d %d %d %d\n",
			0,
			0,
			int( 8.5 * 2.54 * PT_PER_CM),
			int(11.0 * 2.54 * PT_PER_CM));
	} else {
		extern bool      _user_gave_bounding_box;
		rectangle bbox;
		if (_user_gave_bounding_box) {
			extern rectangle _bounding_box_user;
			bbox.set(_bounding_box_user.get_llx(),
				 _bounding_box_user.get_lly(),
				 _bounding_box_user.get_urx(),
				 _bounding_box_user.get_ury());
		} else {
			extern rectangle _bounding_box;
			bbox.set(_bounding_box.get_llx(),
				 _bounding_box.get_lly(),
				 _bounding_box.get_urx(),
				 _bounding_box.get_ury());
		}
		double ll_x, ll_y, ur_x, ur_y;
		if (_grPS_Landscape) {
			gr_rotate_xy(bbox.llx(), bbox.lly(), 90, &ll_x, &ll_y);
			gr_rotate_xy(bbox.urx(), bbox.ury(), 90, &ur_x, &ur_y);
			ll_x += 2.54 * 8.5;
			ur_x += 2.54 * 8.5;
			double tmp = ll_x;
			ll_x = ur_x;
			ur_x = tmp;
		} else {
			ll_x = bbox.llx();
			ll_y = bbox.lly();
			ur_x = bbox.urx();
			ur_y = bbox.ury();
		}
#define POS(x) ((x) > 0 ? int(x) : 0)
		if (_user_gave_bounding_box)
			fprintf(_grPS, "%%%%BoundingBox: %d %d %d %d\n",
				POS(ll_x * PT_PER_CM),
				POS(ll_y * PT_PER_CM),
				POS(ur_x * PT_PER_CM),
				POS(ur_y * PT_PER_CM));
		else
			fprintf(_grPS, "%%%%BoundingBox: %d %d %d %d\n",
				POS(floor(-1.5 + ll_x * PT_PER_CM)),
				POS(floor(-1.5 + ll_y * PT_PER_CM)),
				POS(floor( 2.5 + ur_x * PT_PER_CM)),
				POS(floor( 2.5 + ur_y * PT_PER_CM)));
#undef POS
	}
#if 0				// 1999-aug-2
	fprintf(_grPS, "%%%%DocumentFonts: Courier Helvetica Palatino-Roman Palatino-Italic Symbol Times-Roman\n");
#else
	fprintf(_grPS, "%%%%DocumentFonts: Courier Helvetica Palatino-Roman Palatino-Italic Symbol Times-Roman\n");
#endif
	fprintf(_grPS, "%%%%Pages: %d\n", which_page);
	_grPathExists = false;
	user_gave_ps_filename = false;
	// See if filename was specified
	if ((strlen(filename) > 0)) {
		if (filename[0] == '!') {
			if (strlen(filename) > 1 && filename[1] != ' ') {
				// save to named file
				gr_save_postscript(filename + 1, 1);
				close_ps_file(_grPS);
				return;
			} else {
				// close without renaming
				close_ps_file(_grPS);
				return;
			}
		} else if (filename[0] != ' ') {
			gr_save_postscript(filename, 1);
			close_ps_file(_grPS);
		}
	}
}				// gr_end()

static void
close_ps_file(FILE * fp)
{
	fclose(fp);
	_grNeedBegin = true;
}

// gr_error() -- write an fatal message, then die
void
gr_error(const char *lab)
{
	sprintf(grTempString, "\nFATAL error: %s\n", lab);
	gr_textput(grTempString);
	abort_gri();
}

void
abort_gri()
{
	gr_textput("\
             Suggestion: in unix system with `gdb' command, type\n");
	gr_textput("\
                         gdb gri core\n");
	gr_textput("\
                         where\n");
	gr_textput("\
                         quit\n");
	gr_textput("\
             to get a traceback for use in a bug report.\n\n");
	abort();
}

/*
 * (hue, saturation, value) in (enforced) range from 0 to 1 is translated
 * to (red, green, blue) in range 0 to 1.  Inputs are clipped to range 0
 * to 1 if they are outside the range.
 * 
 * Algorithm: Foley + Van Dam
 */
void
gr_hsv2rgb(double h, double s, double v, double *r, double *g, double *b)
{
	h = 6.0 * pin0_1(h);
	s = pin0_1(s);
	v = pin0_1(v);

	int i = (int) floor(h);
	if (i > 5)
		i = 5;		// Prevent problem if hue is exactly 1
	double f = h - i;
	double p = v * (1.0 - s);
	double q = v * (1.0 - s * f);
	double t = v * (1.0 - s * (1.0 - f));
	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	case 5:
	default:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}

// Is value missing?  (No notice of whether x or y.)
bool
gr_missing(double x)
{
	if (_grMissingValue == 0.0)	// ignore tolerance if 0
		return (x == 0.0);
	if (x == _grMissingValue)	// speed up (?)
		return true;
	x = GRI_ABS(x - _grMissingValue);
	if (x < _grMissingValue_tolerance)
		return true;
	else
		return false;
}

// Is x-value missing? (Notice the x-transform.)
bool
gr_missingx(double x)
{
#if 0
	extern gr_axis_properties _grTransform_x;
	if (_grTransform_x == gr_axis_LOG && x <= 0.0)
		return true;
#endif
	if (_grMissingValue == 0.0)
		return (x == 0.0);
	if (x == _grMissingValue)
		return true;
	x = GRI_ABS(x - _grMissingValue);
	if (x < _grMissingValue_tolerance)
		return true;
	else
		return false;
}

// Is y-value missing?
bool
gr_missingy(double y)
{
#if 0
	extern gr_axis_properties _grTransform_y;
	if (_grTransform_y == gr_axis_LOG && y <= 0.0)
		return true;
#endif
	if (_grMissingValue == 0.0)
		return (y == 0.0);
	if (y == _grMissingValue)
		return true;
	y = GRI_ABS(y - _grMissingValue);
	if (y < _grMissingValue_tolerance)
		return true;
	else
		return false;
}

// Move the pen to new location given in points
void
gr_moveto_cm(double x_cm, double y_cm)
{
	double x_pt = x_cm * PT_PER_CM;
	double y_pt = y_cm * PT_PER_CM;
	extern FILE    *_grPS;
	// take care of postscript stuff
	if (_grWritePS) {
		if (_grPathExists) {	// a path exists
			fprintf(_grPS, PS_stroke);
			fprintf(_grPS, PS_weak_newpath);
			fprintf(_grPS, PS_moveto, x_pt, y_pt);
		} else {		// make new path
			_grPathExists = true;
			fprintf(_grPS, PS_weak_newpath);
			fprintf(_grPS, PS_moveto, x_pt, y_pt);
		}
	}
	_grCurrentPoint_x = x_pt;
	_grCurrentPoint_y = y_pt;
}

// See gr_onxpage_cm() -- see if point is on the page
bool
gr_onxpage_cm(double x_cm)
{
	return ((x_cm >= 0.0 && x_cm <= _grPageWidth_cm) ? true : false);
}

// See if point is on the page
bool
gr_onypage_cm(double y_cm)
{
	return ((y_cm >= 0.0 && y_cm <= _grPageHeight_cm) ? true : false);
}

// gr_record_handle() -- record a handle in the Postscript file (for later
// use by X window previewer, etc)
void
gr_record_handle(double x_cm, double y_cm)
{
	sprintf(grTempString, "^ handle %g %g\n", x_cm, y_cm);
	gr_comment(grTempString);
}

// gr_record_scale() -- record the scale in the Postscript file 
// (for later use by X window previewer, etc)
// Improvement 1999-apr-11: only record if different. (gre, not gri)
void
gr_record_scale()
{
	static gr_axis_properties grTransform_x = gr_axis_LINEAR;
	static gr_axis_properties grTransform_y = gr_axis_LINEAR;;
	static double grxl_pt = 0.0, grxl = 0.0, grPtPerUser_x = 0.0;
	static double gryb_pt = 0.0, gryb = 0.0, grPtPerUser_y = 0.0;
	if (grTransform_x    != _grTransform_x
	    || grxl_pt       != _grxl_pt
	    || grxl          != _grxl
	    || grPtPerUser_x != _grPtPerUser_x
	    || grTransform_y != _grTransform_y
	    || gryb_pt       != _gryb_pt
	    || gryb          != _gryb
	    || grPtPerUser_y != _grPtPerUser_y) {
		fprintf(_grPS, "%%^ scale %d %g %g %g %d %g %g %g\n",
			_grTransform_x, _grxl_pt, _grxl, _grPtPerUser_x,
			_grTransform_y, _gryb_pt, _gryb, _grPtPerUser_y);

	}
	grTransform_x =_grTransform_x;
	grTransform_y =_grTransform_y;
	grxl_pt =_grxl_pt;
	grxl =_grxl;
	grPtPerUser_x = _grPtPerUser_x;
	gryb_pt = _gryb_pt;
	gryb =_gryb;
	grPtPerUser_y =_grPtPerUser_y;
}

/*
 * (red, green, blue) in range from 0 to 1 is translated to (hue, saturation,
 * brightness) in same range.
 * 
 * NOTE: false checking on range of (r,g,b) is done, and strange results may result
 * outside this range.
 * 
 * Algorithm: Foley + Van Dam
 */
void
gr_rgb2hsb(double r, double g, double b, double *h, double *s, double *br)
{
	double          mx, mn;
	double          rc, gc, bc;
	mx = r;
	if (g > mx)
		mx = g;
	if (b > mx)
		mx = b;
	mn = r;
	if (g < mn)
		mn = g;
	if (b < mn)
		mn = b;
	*br = mx;
	if (mx)
		*s = (mx - mn) / mx;
	else
		*s = 0.0;
	if (*s) {
		rc = (mx - r) / (mx - mn);
		gc = (mx - g) / (mx - mn);
		bc = (mx - b) / (mx - mn);
		if (r == mx)
			*h = bc - gc;
		else if (g == mx)
			*h = 2.0 + rc - bc;
		else if (b == mx)
			*h = 4.0 + gc - rc;
		*h /= 6.0;
		if (*h < 0.0)
			*h = *h + 1.0;
	} else {
		*h = 0.0;		/* doesn't matter, since black anyway */
	}
}

// Move the pen to new location given in cm
void
gr_rmoveto_cm(double rx_cm, double ry_cm)
{
	rx_cm *= PT_PER_CM;
	ry_cm *= PT_PER_CM;
	gr_rmoveto_pt(rx_cm, ry_cm);
}

// Move the pen to new location given in pt
void
gr_rmoveto_pt(double rx_pt, double ry_pt)
{
	extern FILE    *_grPS;
	if (!_grPathExists)
		warning("IGNORING gr_rmoveto() NOT FOLLOWING gr_moveto()");
	else {
		_grCurrentPoint_x += rx_pt;
		_grCurrentPoint_y += ry_pt;
		if (_grWritePS)
			fprintf(_grPS, PS_rmoveto, rx_pt, ry_pt);
	}
}
#if 0

//
// change to putting output into named file
bool
gr_reopen_postscript(const char *new_name)
{
	FILE           *new_file;
	int             i = 0;
	fflush(_grPS);
	rewind(_grPS);
	if ((new_file = fopen((char *) new_name, "w")) == NULL)
		return false;		// couldn't open it
	while (!feof(_grPS)) {
		i++;
		fgets(grTempString, _grTempStringLEN, _grPS);
		if (feof(_grPS))
			break;
		fputs(grTempString, new_file);
	}
	fclose(_grPS);
#ifdef VMS
	sprintf(grTempString, "DEL %s;*", gr_currentPSfilename());
	system(grTempString);
	strcpy(ps_filename_used, new_name);
#else
	sprintf(grTempString, "rm -f %s", gr_currentPSfilename());
	system(grTempString);
	strcpy(ps_filename_used, new_name);
#endif
	_grPS = new_file;
	return true;
}

#endif
// gr_save_postscript() -- save postscript file SYNOPSIS void
// gr_save_postscript(char *PS_name, int normal_scale) DESCRIPTION:  Saves the
// postscript commands into the file named 'PS_name'. If normal_scale==1,
// it's at normal scale; otherwise, it's at screen scale
void
gr_save_postscript(const char *PS_name, int normal_scale)
{
	FILE           *PS_file;
	// Rewind gr file. Then skip header
	rewind(_grPS);
	skip_ps_header(_grPS);
	// Open the postscript output file, then write header
	if ((PS_file = fopen((char *) PS_name, "w")) == NULL) {
		sprintf(grTempString, "Can't open postscript file `%s'", PS_name);
		gr_Error(grTempString);
	}
	if (normal_scale) {
		double          old_grMagx = _grMagx, old_grMagy = _grMagy;
		double          old_grOriginx = _grOriginx, old_grOriginy = _grOriginy;
		_grMagx = 1.0;
		_grMagy = 1.0;
		_grOriginx = 0.0;
		_grOriginy = 0.0;
		insert_ps_header(PS_file);
		_grMagx = old_grMagx;
		_grMagy = old_grMagy;
		_grOriginx = old_grOriginx;
		_grOriginy = old_grOriginy;
	} else
		insert_ps_header(PS_file);
	while (!feof(_grPS)) {
		fgets(grTempString, _grTempStringLEN, _grPS);
		if (feof(_grPS))
			break;
		fputs(grTempString, PS_file);
	}
	fclose(PS_file);
}				// gr_save_postscript()

static void
skip_ps_header(FILE * PSfile)
{
	char            S[256];
	int             header_lines;
	/*
	 * Skip header if it exists
	 */
	fgets(S, 256, PSfile);
	if (feof(PSfile))
		gr_Error("PostScript file is empty");
	if (!strcmp(S, "%!\n")) {	/* old-style */
		fgets(S, 256, PSfile);
		if (feof(PSfile))
			gr_Error("PostScript file is nearly empty");
		if (1 == sscanf(&S[1], "%d header lines follow\n",
				&header_lines)) {
			while (header_lines-- > 0) {
				fgets(S, 256, PSfile);
				if (feof(PSfile))
					gr_Error("badly formed (old-style) header");
			}
		} else {
			gr_Error("badly formed (old-style) header");
		}
	} else if (!strcmp(S, "%!PS-Adobe-1.0\n"))	/* new-style */
		while (strncmp(S, "%%Page:", 7)) {
			fgets(S, 256, PSfile);
			if (feof(PSfile))
				gr_Error("badly formed header");
		}
}
#if 0

#endif
/*
 * gr_setmissingvalue(double x) SYNOPSIS void	gr_setmissingvalue(double x)
 * DESCRIPTION:  Sets missing value.
 */
void
gr_setmissingvalue(double x)
{
	_grMissingValue = x;
	_grMissingValue_tolerance = 0.0001 * GRI_ABS(x);
}

/*
 * gr_setxaxisstyle(int xstyle) Set style for x axis.  Presently there are no
 * options, so xstyle is ignored.
 */
void
gr_setxaxisstyle(int xstyle)
{
	_grAxisStyle_x = xstyle;
}

/*
 * gr_setyaxisstyle(int ystyle) Set style for y axis.  If ystyle = 0 (the
 * default), the y label is written vertically.  If ystyle = 1, the y label
 * is written horizontally.
 */
void
gr_setyaxisstyle(int ystyle)
{
	_grAxisStyle_y = ystyle;
}

/*
 * gr_setscale() -- set scale for PostScript
 */
void
gr_setscale(double xfactor, double yfactor)
{
	fprintf(_grPS, "%g %g scale\n", xfactor, yfactor);
}

/*
 * gr_setsymbolsize_pt() -- set symbol size in pt
 */
void
gr_setsymbolsize_pt(double size_pt)
{
	_grSymbolSize_pt = size_pt;
}

// Specify whether tics point in
void
gr_setticdirection(bool tics_point_in)
{
	_grTicsPointIn = tics_point_in ? true : false;
}

/*
 * gr_setticsize_cm() -- set tic size SYNOPSIS void gr_setticsize_cm(double
 * new_size) DESCRIPTION:  Sets the size of tics on further axis drawing to
 * 'new_size',in cm.
 */
void
gr_setticsize_cm(double new_size)
{
	_grTicSize_cm = new_size;
}

#if 0
/*
 * gr_settranslate() -- set translation (x,y) for PostScript
 */
void
gr_settranslate(double xcm, double ycm)
{
	fprintf(_grPS, "%g %g translate\n", xcm * PT_PER_CM, ycm * PT_PER_CM);
}
#endif

// gr_setup_creatorname() -- set up creator name for PostScript prolog
void
gr_setup_creatorname(const char *s)
{
	strcat(creator_name, s);
}

// gr_setup_ps_filename() -- set name of PostScript output file
// DESCRIPTION:  Set the name for the
// postscript output. If this function isn't called prior to gr_begin(), the
// filename will be 'gri-00.ps'. As with all gr functions begining with the
// letters 'gr_setup_ps_', this has an effect ONLY if it preceeds plotting
// calls which affect the pen.
void
gr_setup_ps_filename(const char *new_name)
{
	if (_grNeedBegin) {
		user_gave_ps_filename = true;
		if (strlen(new_name) > 0)
			strcpy(ps_filename, new_name);
		else
			strcpy(ps_filename, "gri.ps");
	}
}

/*
 * gr_setup_ps_landscape() -- set ps printer to landscape mode
 *
 * NOTE:  Must call before gr_begin()
 */
void
gr_setup_ps_landscape()
{
	double           t0 = _grPageWidth_cm;
	_grPS_Landscape = true;
	_grPageWidth_cm = _grPageHeight_cm;
	_grPageHeight_cm = t0;
}
#if 0

/*
 * gr_setup_ps_scale() -- set enlargement factors for x and y on ps printer
 * SYNOPSIS void	gr_setup_ps_scale(double xfactor,double yfactor)
 * DESCRIPTION:  Scales the plot on the page,by multiplying by the factors
 * 'xfactor' and 'yfactor'.  Note that x and y are exchanged on page if
 * gr_setup_grPS_Landscape() is also called. NOTE:  Must call before
 * gr_begin().
 */
void
gr_setup_ps_scale(double xfactor, double yfactor)
{
	_grPSScale_x = xfactor;
	_grPSScale_y = yfactor;
	_grPageWidth_cm *= xfactor;
	_grPageHeight_cm *= yfactor;
}
#endif

//REMOVED//// gr_setxlabel() -- set name for x-axis
//REMOVED//void
//REMOVED//gr_setxlabel(const char *xlab)
//REMOVED//{
//REMOVED//    strncpy(_grXAxisLabel, xlab, LEN_lab);
//REMOVED//}

//REMOVED-2000-5-2 // gr_setxnumberformat() -- set format for numbers
//REMOVED-2000-5-2 void
//REMOVED-2000-5-2 gr_setxnumberformat(const char *xformat)
//REMOVED-2000-5-2 {
//REMOVED-2000-5-2     strncpy(_grNumFormat_x, xformat, LEN_num);
//REMOVED-2000-5-2 }

// gr_setxpagesize_cm() -- set page size for x 
// DESCRIPTION: Sets page size to 'x_cm'.
// Future x pen locations should be in range [0,x_cm].  If 'x_cm'<0,use
// default of 21.59.
void
gr_setxpagesize_cm(double x_cm)
{
	if (x_cm <= 0.0) {
		double          default_value = 21.59;
		sprintf(grTempString,
			"(gr_setxpagesize_cm): USED %g INSTEAD OF 'x_cm'=%g",
			default_value, x_cm);
		warning(grTempString);
		_grPageWidth_cm = default_value;
	} else {
		_grPageWidth_cm = x_cm;
	}
}

// gr_setxscale() -- set the scale for x
void
gr_setxscale(double xl_cm, double xr_cm, double xl, double xr)
{
	// XREF -- axis transform
	/* set up scale */
	_grxl_pt = xl_cm * PT_PER_CM;
	switch (_grTransform_x) {
	case gr_axis_LINEAR:
		_grxl = xl;
		_grCmPerUser_x = (xr_cm - xl_cm) / (xr - xl);
#if 1
		_ll_x_pt = xl_cm * PT_PER_CM;
		_ur_x_pt = xr_cm * PT_PER_CM;
		_ll_x_us = xl;
		_ur_x_us = xr;
#endif
		break;
	case gr_axis_LOG:
		if (xl <= 0.0 || xr <= 0.0) {
			gr_Error("Log axis requires x values to exceed 0.\n");
		}
		_grxl = xl;
		_grCmPerUser_x = (xr_cm - xl_cm) / log10(xr / xl);
		break;
	default:
		gr_Error("unknown axis transformation (internal error)");
		break;
	}
	_grPtPerUser_x = _grCmPerUser_x * PT_PER_CM;
	/* test for weirdness */
	double error_test = GRI_ABS(_grxl_pt / PT_PER_CM - xr_cm);
	if (error_test < 0.1) {
		char msg[200];
		sprintf(msg, "(gr_setxscale): _grxl_pt/PT_PER_CM=%g VERY CLOSE TO xr_cm=%g", _grxl_pt / PT_PER_CM, xr_cm);
		warning(msg);
	}
	if (error_test == 0.0) {
		char msg[200];
		sprintf(msg, "(gr_setxscale): _grxl_pt/PT_PER_CM = %g  ==  xr_cm=%g", _grxl_pt / PT_PER_CM, xr_cm);
		warning(msg);
	}
}

/*
 * gr_setxsubdivisions() -- set # subdivisions on x-axis SYNOPSIS void
 * gr_setxsubdivisions(int num) DESCRIPTION:  Sets the number of subdivisions
 * between numbered tics to 'num'
 */
void
gr_setxsubdivisions(int num)
{
	if (num > 0 && num < 100)
		_grNumSubDiv_x = num;
	else
		_grNumSubDiv_x = 1;
}

// gr_setxtransform() -- set transform for x-mapping
void
gr_setxtransform(gr_axis_properties xstyle)
{
	// XREF -- axis transform
	_grTransform_x = xstyle;
}

//REMOVED//// gr_setylabel() -- set name for y-axis
//REMOVED//void
//REMOVED//gr_setylabel(const char *ylab)
//REMOVED//{
//REMOVED//    strncpy(_grYAxisLabel, ylab, LEN_lab);
//REMOVED//}

//REMOVED-2000-5-2 // gr_setynumberformat() -- set format for numbers on y-axis
//REMOVED-2000-5-2 void
//REMOVED-2000-5-2 gr_setynumberformat(const char *yformat)
//REMOVED-2000-5-2 {
//REMOVED-2000-5-2     strncpy(_grNumFormat_y, yformat, LEN_num);
//REMOVED-2000-5-2 }

// gr_setypagesize_cm() -- set page size for y
void
gr_setypagesize_cm(double y_cm)
{
	if (y_cm <= 0.0) {
		double          default_value = 27.94;
		sprintf(grTempString,
			"(gr_setypagesize_cm): USED %g INSTEAD OF 'y_cm'=%g",
			default_value, y_cm);
		warning(grTempString);
		_grPageHeight_cm = default_value;
	} else {
		_grPageHeight_cm = y_cm;
	}
}

// gr_setyscale() -- set the scale for y
void
gr_setyscale(double yb_cm, double yt_cm, double yb, double yt)
{
	// XREF -- axis transform
	/* set up scale */
	_gryb_pt = yb_cm * PT_PER_CM;
	switch (_grTransform_y) {
	case gr_axis_LINEAR:
		_gryb = yb;
		_grCmPerUser_y = (yt_cm - yb_cm) / (yt - yb);
#if 1
		_ll_y_pt = yb_cm * PT_PER_CM;
		_ur_y_pt = yt_cm * PT_PER_CM;
		_ll_y_us = yb;
		_ur_y_us = yt;
#endif
		break;
	case gr_axis_LOG:
		if (yb <= 0.0 || yt <= 0.0) {
			gr_Error("Log axis requires y values to exceed 0.\n");
		}
		_gryb = yb;
		_grCmPerUser_y = (yt_cm - yb_cm) / log10(yt / yb);
		break;
	default:
		gr_Error("unknown y transform"); // impossible to get here
		break;
	}
	_grPtPerUser_y = _grCmPerUser_y * PT_PER_CM;
	/* test for weirdness */
	double error_test = GRI_ABS(_gryb_pt / PT_PER_CM - yt_cm);
	if (error_test < 0.1) {
		char msg[200];
		sprintf(msg,
			"(gr_setyscale): _gryb_pt/PT_PER_CM=%g VERY CLOSE TO yt_cm=%g",
			_gryb_pt / PT_PER_CM, yt_cm);
		warning(msg);
	}
	if (error_test == 0.0) {
		char msg[200];
		sprintf(msg,
			"(gr_setyscale): _gryb_pt/PT_PER_CM=%g  ==  yt_cm=%g",
			_gryb_pt / PT_PER_CM, yt_cm);
		warning(msg);
	}
}				/* end of gr_setyscale() */

/*
 * gr_setysubdivisions() -- set # subdivisions on y-axis SYNOPSIS void
 * gr_setysubdivisions(int num) DESCRIPTION:  Corresponds to
 * gr_setxsubdivisions().
 */
void
gr_setysubdivisions(int num)
{
	if (num > 0 && num < 100)
		_grNumSubDiv_y = num;
	else
		_grNumSubDiv_y = 1;
}

// gr_setytransform() -- set transform for y-mapping
void
gr_setytransform(gr_axis_properties ystyle)
{
	_grTransform_y = ystyle;
}

// gr_showpage() -- print this page, and start a new one
void
gr_showpage()
{
	fprintf(_grPS, PS_showpage);
	handle_landscape_scale(_grPS);
	which_page++;
	fprintf(_grPS, "%%%%Page: %d %d\n", which_page, which_page);
}

// Convert user units to cm on page, using gr_usertopt
void
gr_usertocm(double x, double y, double *x_cm, double *y_cm)
{
	gr_usertopt(x, y, x_cm, y_cm);
	*x_cm /= PT_PER_CM;
	*y_cm /= PT_PER_CM;
}
double
gr_usertocm_x(double x, double y)
{
	double x_cm, y_cm;
	gr_usertopt(x, y, &x_cm, &y_cm);
	return (x_cm / PT_PER_CM);
}
double
gr_usertocm_y(double x, double y)
{
	double x_cm, y_cm;
	gr_usertopt(x, y, &x_cm, &y_cm);
	return (y_cm / PT_PER_CM);
}

/*
 * gr_usertopt() -- convert user units to points on the page.  This is a base
 * routine used by all drawing routines.
 * 
 * Adding projections: change this, also gr_cmtouser(), also
 * show_axesCmd()/show.c.
 * 
 * 
 * Known projections:
 * 
 * LINEAR: scale to page units, then plot.
 * 
 * LOG: log transform, scale to page units, then plot.
 * 
 */
void
gr_usertopt(double x, double y, double *x_pt, double *y_pt)
{
	*x_pt = *y_pt = 0.0;
	// XREF -- axis transform
	double          xx, yy;
	switch (_grTransform_x) {
	case gr_axis_LINEAR:
#if NEW_TRANSFORM
		*x_pt = interpolate_linear(x, _ll_x_us, _ll_x_pt, _ur_x_us, _ur_x_pt);
#else
		*x_pt = _grxl_pt + (x - _grxl) * _grPtPerUser_x;
#endif
		break;
	case gr_axis_LOG:
		if (x <= 0.0) {
			sprintf(_grTempString, "can't use negative x (%g) with LOG axis.", x);
			gr_Error(_grTempString);
		} else {
			xx = log10(x / _grxl);
			*x_pt = _grxl_pt + xx * _grPtPerUser_x;
		}
		break;
	default:
		gr_Error("unknown x transform"); // impossible to get here
	}
	switch (_grTransform_y) {
	case gr_axis_LINEAR:
#if NEW_TRANSFORM
		*y_pt = interpolate_linear(y, _ll_y_us, _ll_y_pt, _ur_y_us, _ur_y_pt);
#else
		*y_pt = _gryb_pt + (y - _gryb) * _grPtPerUser_y;
#endif
		break;
	case gr_axis_LOG:
		if (y <= 0.0) {
			sprintf(_grTempString, "can't use negative y (%g) with LOG axis.", y);
			gr_Error(_grTempString);
		} else {
			yy = log10(y / _gryb);
			*y_pt = _gryb_pt + yy * _grPtPerUser_y;
		}
		break;
	default:
		gr_Error("unknown y transform");
	}
}

