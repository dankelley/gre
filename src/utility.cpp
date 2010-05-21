#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <regex.h>
#include "files.h"

#if defined(HAVE_PROJ)
extern "C" {
#include "proj/projects.h" /* for 'proj' */
}
#endif

using namespace std;

//extern double strtod(const char *, char **);

#include "private.h"
#include "extern.h"
#include "debug.h"

static void create_gre_regexp(const char *in, string& out);
static const char* decode_regexp_error(int err);


/* Number of parameters */
unsigned int
cmd_parameters()
{
	return vector_vars[0].get_size();
}
/* is this param numeric? */
bool
cmd_parameter_is_n(unsigned int i)
{
	return vector_vars[0].get_at_index(i).is_numeric();
}
double
cmd_parameter_n(unsigned int i)
{
	scalar val = vector_vars[0].get_at_index(i);
	if (val.is_numeric())
		return val.get_numeric();
	else {
		char *ptr = NULL;
		double rval = (int) strtod(val.get_text(), &ptr);
		if (*ptr != '\0') {
			warning("cannot coerce `\\",
				val.get_text(),
				"' to numeric value; using 0 instead",
				"\\");
			rval = 0.0;
		}
		return rval;
	}
}
char*
cmd_parameter_s(unsigned int i)
{
	scalar val = vector_vars[0].get_at_index(i);
	if (val.is_numeric()) {
		// coerce
		char rval[40];
		sprintf(rval, "%g", val.get_numeric());
		return strdup(rval);
	} else {
		//return strdup(val.get_text());
		return (vector_vars[0].get_at_index(i)).get_text();
	}
}


#if 0
// get full filename, searching through directory list in the environment
// variable GREPATH, or, if that environment variable doesn't exist, in the
// symbol GREPATH, defined in defaults.h
const char*
file_in_path(const char *name)
{
#if defined(VMS) || defined(MSDOS)
	return name;		// BUG -- VMS has no env-var
#else
	if (*name == '/') {
		return name;		// Obey absolute filenames
	} else {			// Check path
		string path = gre_getenv("GREPATH");
		if (path.empty())
			return name;
		char* gre_input = new char[1 + path.size()];
		if (!gre_input) {
			fatal_error("", 0, "Out of memory");
			return "";
		}
		char* to_free = gre_input; // save pointer so can delete later
		strcpy(gre_input, path.c_str());
		char* return_name = new char[2 + strlen(gre_input) + strlen(name)];
		if (!return_name) {
			fatal_error("", 0, "Out of memory");
			return "";
		}
		gre_input = strtok(gre_input, ":");
		do {
			FILE           *fp;
			sprintf(return_name, "%s/%s", gre_input, name);
			if (NULL != (fp = fopen(return_name, "r"))) {
				fclose(fp); delete [] to_free; return return_name;
			}
		} while ((gre_input = (char *) strtok(NULL, ":")));
		// Could not find commandfile.
		fprintf(stderr, "Cannot locate file `%s'\n", name);
		return "";
	}
#endif
}
#endif

void
swap(double& a, double& b)
{
	double tmp = a;
	a = b;
	b = tmp;
}

// getdnum -- return true (setting *d) or return false if can't get number
bool
getdnum(const char *s, double *d)
{
	char *            ptr = NULL;
	*d = strtod(s, &ptr);
	if (*ptr == '\0') {
		// Normal number; check for infinity/not-a-number
#if defined(HAVE_ISNAN) && defined(HAVE_ISINF)
#if !defined(__MACHTEN__)
		if (isinf(*d) || isnan(*d)) {
			//printf("DEBUG: found NaN or Inf\n");
			*d = (double) _grMissingValue;
		}
#endif
#endif
		return true;
	}
	// Check to see if it's a variable or NaN/Inf.
	if (!strcmp(s, "-NaN")    || !strcmp(s, "-nan")
	    || !strcmp(s, "NaN")  || !strcmp(s, "nan")
	    || !strcmp(s, "-Inf") || !strcmp(s, "Inf")
	    || !strcmp(s, "-inf") || !strcmp(s, "inf")) {
		//printf("DEBUG: found NaN or Inf ... manually\n");
		*d = _grMissingValue;
		return true;
	}
#if 0
	// It's a mystery.  report error, fatal or nonfatal depending on
	// whether the user is interested in errors.
	if (!_ignore_error) {
		err("Cannot interpret `\\",
		    s,
		    "' as a number.",
		    "\\");
	}
#endif
	return false;
}

bool
is_var(const char *s)
{
	return *s == '$';
}

/*
 * err () -- print error message.  If the first (string) argument ends in
 * '\\', then several strings follow, ended by a string consisting only of
 * "\\".
 */
void
err(const char *emsg, ...)
{
	char            msg[1024];
	bool            several = false;
	int             len;
	char *            p;
	va_list         ap;
	if (emsg != NULL) {
		va_start(ap, emsg);
		strcpy(msg, emsg);
		len = strlen(msg);
		if (msg[len - 1] == '\\') {
			msg[len - 1] = '\0';
			several = true;
		}
		gr_textput("ERROR: ");
		do {
			gr_textput(msg);
			if (several) {
				p = va_arg(ap, char *);
				strcpy(msg, p);
			}
		}
		while (several && strcmp(msg, "\\"));
		//gr_textput("\n");
	}
	_error_in_cmd = true;
	va_end(ap);
}

// print string, allowing for TAB/NEWLINE/... escapes
void
gr_textput(const char *s)
{
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

// warning () -- print warning message.  All arguments must be char*.
// If the first argument ends in '\\', then several strings follow,
// ended by a string consisting only of "\\".  If only string is '\\',
// then this is a request to notify of number of repeats of last msg.
void
warning(const char *s, ...)
{
	static          unsigned int msg_last_copies = 0;
	static string   msg_last;	// must be static
	static string msg;		// static only for efficiency if called a lot
	bool            several = false;
	int             len;
	char           *p = NULL;
	va_list         ap;
	// Check for final wrap-up command
	if (!strcmp(s, "\\\\")) {
		if (msg_last_copies > 0) {
			char buffer[100];
			sprintf(buffer,
				"         (The last warning was repeated %d times.)\n",
				++msg_last_copies);
			gr_textput(buffer);
		}
		return;
	}
	if (s != NULL) {
		va_start(ap, s);
		len = strlen(s);
		msg = s;
		if (msg[len - 1] == '\\') {
			msg.STRING_ERASE(len - 1, 1);
			several = true;
		}
		do {
			if (several) {
				p = va_arg(ap, char *);
				if (strcmp(p, "\\"))
					msg += p;
			}
		} while (several && strcmp(p, "\\"));
		if (msg == msg_last) {
			// Same as last msg.  Collect till a new one.
			msg_last_copies++;
		} else {
			// Not same as last msg.
			if (msg_last_copies > 0) {
				// This msg repeated
				char buffer[100];
				sprintf(buffer,
					"         (The last warning was repeated %d times.)\n",
					msg_last_copies + 1);
				gr_textput(buffer);
				// Now show current msg
				gr_textput("Warning: ");
				gr_textput(msg.c_str());
				gr_textput("\n");
			} else {
				// First time for this msg
				gr_textput("Warning: ");
				gr_textput(msg.c_str());
				gr_textput("\n");
			}
			msg_last_copies = 0;
		}
		msg_last = msg;
	} va_end(ap);
}

#define BETWEEN(l,r,m)  ( (l)<(r) ? (m)<=(r)&&(l)<=(m) : (r)<=(m)&&(m)<=(l) )
// find min/max of matrix
void
extrema_of_matrix(double *min, double *max)
{
	bool            first = true;
	double          f;
	*min = *max = 0.0;
	unsigned int nx = _xmatrix.size();
	unsigned int ny = _ymatrix.size();
	for (unsigned int c = 0; c < nx; c++) {
		for (unsigned int r = 0; r < ny; r++) {
			if (_legit_xy(c, r) == true) {
				f = _f_xy(c, r);
				if (first == true) {
					*min = *max = f;
					first = false;
				}
				if (f < *min)
					*min = f;
				if (*max < f)
					*max = f;
			}
		}
	}
	if (first == true) {
		*min = gr_currentmissingvalue();
		*max = gr_currentmissingvalue();
	}
}

// DESCRIPTION:  Get words w[0], w[1], from string `s', finding at most 'max'
// words; set 'nw' to number words read.
// 
// NB: input string `s' is destroyed in the process!
void
chop_into_words(char *s, char **w, int *nw, int max)
{
	register char   c, *cp;
	max--;
	cp = s;
	*nw = 0;
	if (*cp == '\0')
		return;

	// Traverse s, getting pointers to words and terminating ends
	while (*nw < max) {
		// Skip space and tabs; break if done.
		while (*cp == ' ' || *cp == '\t' || *cp == '\n' || *cp == '\r')
			cp++;
		if (!*cp)
			break;		// end of string
	
		// Now point to non-blank.
		w[(*nw)++] = cp;
		while (*++cp 
		       && !(*cp == ' ' || *cp == '\t' || *cp == '\n' || *cp == '\r')) {
			;		// EMPTY
		}
		c = *cp;
		*cp++ = '\0';
		// Break if done
		if (!c)
			break;
	}
}

void
set_ps_color(char what, bool force /*= false */) // what='p' for path or 't' for text; 'force' does even if same as old
{
	static double r_old = -1.0, g_old = -1.0, b_old = -1.0; // speeds up
	extern FILE *_grPS;
	extern bool _grWritePS;
	if (!_grWritePS)
		return;
	double r, g, b;
	if (what == 'p')
		_griState.color_line().getRGB(&r, &g, &b);
	else
		_griState.color_text().getRGB(&r, &g, &b);
	if (force || (r_old != r || g_old != g || b_old != b)) {
		if (r == g && g == b) {
			fprintf(_grPS, "%.3g g\n", r);
			fprintf(_grPS, "%.3g G\n", r);
		} else {
			fprintf(_grPS, "%.3g %.3g %.3g rg\n", r, g, b);
			fprintf(_grPS, "%.3g %.3g %.3g RG\n", r, g, b);
		}
	}
	r_old = r;
	g_old = g;
	b_old = b;
}

void
demonstrate_command_usage()
{
#if 1
	ShowStr("PROPER USAGE: `-- message under construction --'");
#else
	ShowStr("PROPER USAGE: `");
	if (cmd_being_done() > -1) {
		ShowStr(_command[cmd_being_done()].syntax);
		ShowStr("'\n");
	} else {
		ShowStr(" unknown.\n");
	}
#endif
}
bool
draw_axes_if_needed()
{
	if (_need_x_axis && _need_y_axis)
		draw_axes(_axesStyle, 0.0, (gr_axis_properties) gr_axis_LEFT, true);
	return true;
}

// Set environment for line drawing.
bool
set_environment()
{
	// Update some things possibly ruined by other routines. BUG: I don't
	// think most of this is required; Macintosh remants
	gr_fontID       old_font = gr_currentfont();
	double          fontsize = FONTSIZE_PT_DEFAULT;
	double          linewidth = LINEWIDTH_DEFAULT;
	double          symbolsize = SYMBOLSIZE_DEFAULT;
	double          tic_direction = 0.0; // out
	double          tic_size = 0.2;
	double          xmargin = XMARGIN_DEFAULT;
	double          ymargin = YMARGIN_DEFAULT;
	double          xsize = XSIZE_DEFAULT;
	double          ysize = YSIZE_DEFAULT;
	fontsize = get_var_scalar("$_font_size");
	gr_setfontsize_pt(fontsize);
	gr_setfont(old_font);	// weird, since already set
	linewidth = get_var_scalar("$_linewidth");
	_griState.set_linewidth_line(linewidth);
	linewidth = get_var_scalar("$_linewidthsymbol");
	_griState.set_linewidth_symbol(linewidth);
	symbolsize = get_var_scalar("$_symbol_size");
	gr_setsymbolsize_cm(symbolsize);
	tic_direction = get_var_scalar("$_tic_direction");
	gr_setticdirection(int(floor(0.5 + tic_direction)) ? true : false);
	tic_size = get_var_scalar("$_tic_size");
	gr_setticsize_cm(tic_size);
	xmargin = get_var_scalar("$_xmargin");
	ymargin = get_var_scalar("$_ymargin"); 
	xsize = get_var_scalar("$_xsize");
	ysize = get_var_scalar("$_ysize"); 
	gr_setxtransform(_xtype);
	gr_setxscale(xmargin, xmargin + xsize, _xleft, _xright);
	gr_setytransform(_ytype);
	gr_setyscale(ymargin, ymargin + ysize, _ybottom, _ytop);
	gr_record_scale();
	return true;
}

double
vector_min(double *v, unsigned n)
{
	double return_value = v[0];
	for (unsigned i = 1; i < n; i++)
		if (v[i] < return_value)
			return_value = v[i];
	return return_value;
}
double
vector_max(double *v, unsigned n)
{
	double return_value = v[0];
	for (unsigned i = 1; i < n; i++)
		if (return_value < v[i])
			return_value = v[i];
	return return_value;
}
/*
 * inside_box - see if data point inside clip box DESCRIPTION Returns 1 if
 * either: (1) clipping turned off or	(2) clipping on and inside axes or
 * (3) clipping on and inside (xl,xr)(yb,yt) box
 */
bool
inside_box(double x, double y)
{
	//extern char     _grTempString[];
	extern gr_axis_properties _grTransform_x, _grTransform_y;
	/* If logarithmic, ensure that positive value */
	if (_grTransform_x == gr_axis_LOG && x <= 0.0)
		return false;
	if (_grTransform_y == gr_axis_LOG && y <= 0.0)
		return false;
	if (_clipData == 0) {	/* user did `set clip off' */
		return true;
	} else if (_clipData == 1) {/* user did `set clip on xl xr yb yt' */
		if (!BETWEEN(_clipxleft, _clipxright, x)) {
			return false;
		}
		if (!BETWEEN(_clipybottom, _clipytop, y)) {
			return false;
		}
		return true;
	} else {			/* user did `set clip on' */
		if (!BETWEEN(_xleft, _xright, x)) {
			return false;
		}
		if (!BETWEEN(_ybottom, _ytop, y)) {
			return false;
		}
		return true;
	}
}
void
check_psfile()
{
	extern FILE    *_grPS;
	if (ferror(_grPS)) {
		/* never returns */
		fatal_error(" IO error on PostScript output file");
	}
}

bool
scales_defined()
{
	bool know_scales = (_xscale_exists && _yscale_exists) ? true : false;
#if 1                           // 990117 - in case data created by loop
	if (!know_scales) {
		if (!create_x_scale())
			return false;
		if (!create_y_scale())
			return false;
		return true;
	}
#endif
	return know_scales;
}

// Update bounding box (stored in cm on page)
// 20 Feb 98: ignore if clipping on (already updated for the clip region)
void
bounding_box_update(const rectangle& box)
{
	// Ignore if clipping
	if (_clipData != 0)
		return;

    // Ignore degenerate boxes
	if (box.llx() == box.urx() || box.lly() == box.ury())
		return;

    // If have existing bbox, see if this lies outside ...
	if (_bounding_box.llx()    != _bounding_box.urx()
	    || _bounding_box.lly() != _bounding_box.ury()) {
		if (box.llx() < _bounding_box.llx())
			_bounding_box.set_llx(box.llx());
		if (box.lly() < _bounding_box.lly()) 
			_bounding_box.set_lly(box.lly());
		if (_bounding_box.urx() < box.urx())
			_bounding_box.set_urx(box.urx());
		if (_bounding_box.ury() < box.ury()) 
			_bounding_box.set_ury(box.ury());
	} else {		// ... else just copy it
		_bounding_box.set_llx(box.llx());
		_bounding_box.set_lly(box.lly());
		_bounding_box.set_urx(box.urx());
		_bounding_box.set_ury(box.ury());
	}
}

void
gre_exit(int code)
{
	close_data_files();
	exit(code);
}

void
extrema_of_xcol(double *min, double *max)
{
	*min = *max = gr_currentmissingvalue();
	int xind = get_vector_var_index("$_x"); 
	if (xind < 0) 
		fatal_error("X column ($_x) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	unsigned int xlen = 1 + xvar->get_end_index();
	bool first = true;
	for (unsigned int i = 0; i < xlen; i++) {
		double xx = xvar->get_at_index(i).get_numeric();
		if (!gr_missingx(xx)) {
			if (first) {
				*min = *max = xx;
				first = false;
			} else {
				if (xx   < *min) *min = xx;
				if (*max < xx)   *max = xx;
			}
		}
	}
}
void
extrema_of_ycol(double *min, double *max)
{
	*min = *max = gr_currentmissingvalue();
	int yind = get_vector_var_index("$_y");
	if (yind < 0) 
		fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* yvar = &vector_vars[yind];
	unsigned int ylen = 1 + yvar->get_end_index();
	bool first = true;
	for (unsigned int i = 0; i < ylen; i++) {
		double yy = yvar->get_at_index(i).get_numeric();
		if (!gr_missingy(yy)) {
			if (first) {
				*min = *max = yy;
				first = false;
			} else {
				if (yy   < *min) *min = yy;
				if (*max < yy)   *max = yy;
			}
		}
	}
}
bool
extremal_values(int vector_index, double *min, double *max) 
{
	*min = *max = gr_currentmissingvalue();
	if (vector_index < 0)
		return false;
	var* var = &vector_vars[vector_index];
	int len = 1 + var->get_end_index();
	if (len < 1)
		return false;
	bool first = true;
	for (int i = 0; i < len; i++) {
		double val = var->get_at_index(i).get_numeric();
		if (!gr_missing(val)) {
			if (first) {
				*min = *max = val;
				first = false;
			} else {
				if (*max < val)
					*max = val;
				if (val < *min)
					*min = val;
			}
		}
	}
	return true;
}

// Determine a 1-2-5 scaling for interval xl<x<xr, trying
// to get n increments each of which is a multiple of 1, 2, or 5.
// The results are xlr, ylr = the new range, which includes 
// the old range as a subset, and nr = the number of increments.
void
gr_scale125(double xl, double xr, int n, double *xlr, double *xrr, int *nr)
{
	const int max_tries = 5;
	int n_orig = n;
	int tries = 0;
	do {
		double delta = fabs((xr - xl) / n);
		if (delta == 0.0) {
			*xlr = xl - 1.0;
			*xrr = xr + 1.0;
			*nr = 1;
			return;
		}
		double order_of_magnitude = pow(10.0, floor(log10(delta)));
		double delta125 = delta / order_of_magnitude;
		if (delta125 < 2.0)
			delta125 = 1.0;
		else if (delta125 < 5.0)
			delta125 = 2.0;
		else
			delta125 = 5.0;
		delta125 *= order_of_magnitude;
		if (xr > xl) {
			*xlr = delta125 * (floor(xl / delta125));
			*xrr = delta125 * (ceil(xr / delta125));
			*nr = (int) floor(0.5 + (*xrr - *xlr) / delta125);
		} else {
			*xlr = delta125 * (ceil(xl / delta125));
			*xrr = delta125 * (floor(xr / delta125));
			*nr = (int) floor(0.5 - (*xrr - *xlr) / delta125);
		}
		n = int(0.8 * n);
		if (n < 1) 
			n = 1;		// need at least 1 segment
	} while ((*nr > int(1.75 * n_orig)) && ++tries < max_tries);
}

char *
pwd()
{
#if defined(VMS)		/* vms version braindead */
	return "";
#elif defined(MSDOS)		/* msdos version braindead */
	return "";
#else
#if !defined(HAVE_POPEN)
	err("Cannot do `pwd' because computer lacks popen() subroutine");
	return "";
#else
	char            msg[1024];
	FILE           *pipefile = call_popen("pwd", "r");
	if (pipefile) {
		char *            result;
		if (1 == fscanf(pipefile, "%s", msg)) {
			pclose(pipefile);
			result = new char[1 + strlen(msg)];
			if (!result) OUT_OF_MEMORY;
			strcpy(result, msg);
			return result;
		} else {
			pclose(pipefile);
			warning("Can't determine name of working directory; using `.' instead.");
			result = new char[2];
			if (!result) OUT_OF_MEMORY;
			strcpy(result, ".");
			return result;
		}
	} else {
		return ".";
	}
#endif
#endif
}


char *
egetenv(const char *s)
{
	if (!strcmp(s, "PWD")) {
		return (char *) pwd();
	} else if (!strcmp(s, "USER")) {
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return "unknown";
#endif
	} else if (!strcmp(s, "SYSTEM")) {
#if defined(VMS)
		return "vax";
#elif defined(MSDOS)
		return "msdos";
#else
		return "unix";
#endif
	} else if (!strcmp(s, "HOST")) {
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return "unknown";
#endif
	} else if (!strcmp(s, "HOME")) {
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return "~";
#endif
	} else if (!strcmp(s, "PAGER")) {
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return "";
#endif
	} else if (!strcmp(s, "GRIINPUTS")) {
		/*
		 * Of form ".:/usr/local/lib/gri"
		 */
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return NULL;		/* will use default */
#endif
	} else {
#if defined(HAVE_GETENV)
		return (char *) getenv(s);
#else
		return "unknown";
#endif
	}
}

#define NP 0x00c /* newpage, control-L */
char 
first_unwhite(const char *s)
{
	while (*s != '\0' && *s == ' ' || *s == '\t' || *s == '\n' || *s == NP)
		s++;
	return *s;
}
#undef NP

#define NP 0x00c /* newpage, control-L */
char 
last_unwhite(const char *s)
{
	int len = strlen(s);
	const char *e = s + len - 1;
	while (e != s && *e == ' ' || *e == '\t' || *e == '\n' || *e == NP)
		e--;
	return *e;
}
#undef NP

// Get name for temporary file (hide details of libraries here)
char*
tmp_file_name()
{
	char *rval;

	if (_gre_debug & DEBUG_OS_CALLS) printf("Getting temporary filename from OS ...");

#if defined(HAVE_TEMPNAM)
	rval = tempnam("/usr/tmp", "gre");
	if (rval == NULL) {
		err("Cannot determine name of temporary file.");
		return NULL;
	}
	if (_gre_debug & DEBUG_OS_CALLS) printf(" got `%s'\n", rval);
	return rval;
#else
#if defined(HAVE_TMPNAM)
	rval = tmpnam(NULL);
	if (rval == NULL) {
		err("Cannot determine name of temporary file.");
		return NULL;
	}
	if (_gre_debug & DEBUG_OS_CALLS) printf(" got `%s'\n", rval);
	return rval;
#else
	return GRE_TMP_FILE;
#endif
#endif
}

#define SWAP(var, len, i, inew) if ((len) > (i) && (len) > (inew)) {\
    tmp = (var)->get_at_index((i)).get_numeric();\
    (var)->set_value((var)->get_at_index((inew)).get_numeric(), (i));\
    (var)->set_value(tmp,                           (inew));\
}
bool
reorder_columns_randomlyCmd()
{
	int xind = get_vector_var_index("$_x");
	int yind = get_vector_var_index("$_y");
	int zind = get_vector_var_index("$_z");
	int uind = get_vector_var_index("$_u");
	int vind = get_vector_var_index("$_v");
	if (xind < 0 || yind < 0 || zind < 0 || uind < 0 || vind < 0) {
		err("Some columns have become undefined");
		return false;		// never reached
	}
	var* xvar = &vector_vars[xind];
	var* yvar = &vector_vars[yind];
	var* zvar = &vector_vars[zind];
	var* uvar = &vector_vars[uind];
	var* vvar = &vector_vars[vind];

	// Seed, then do a few random numbers
#if defined(HAVE_GETPID)
#if defined(HAVE_DRAND48)
	srand48(getpid());
#else
	srand(getpid());
#endif
#endif

	int i, n = 0;
	int xlen = xvar->get_end_index();
	int ylen = yvar->get_end_index();
	int zlen = zvar->get_end_index();
	int ulen = uvar->get_end_index();
	int vlen = vvar->get_end_index();
	if (xlen > n) n = xlen;
	if (ylen > n) n = ylen;
	if (zlen > n) n = zlen;
	if (ulen > n) n = ulen;
	if (vlen > n) n = vlen;

	//printf("xind,yind,zind,uind,vind= %d %d %d %d %d\n",xind,yind,zind,uind,vind);
	//printf("xvar,...                = %d %d %d %d %d\n",(int)xvar,(int)yvar,(int)zvar,(int)uvar,(int)vvar);
	//printf("xlen,...                = %d %d %d %d %d\n",(int)xlen,(int)ylen,(int)zlen,(int)ulen,(int)vlen);

	for (i = 0; i < n; i++) {
		double tmp;
#if defined(HAVE_DRAND48)	// range is 0 to 1, but do modulus in case
		int inew = int(drand48() * n) % n;
#else
		int inew = int(rand() % n);
#endif
		//printf("i=%d inew=%d\n",i,inew);
		SWAP(xvar, xlen, i, inew);
		SWAP(yvar, ylen, i, inew);
		SWAP(zvar, zlen, i, inew);
		SWAP(uvar, ulen, i, inew);
		SWAP(vvar, vlen, i, inew);
	}
	return true;
}

/** Find intersection between two lines, each defined by 2 points 
 * (x,y) on line 1; (X,Y) on line 2;
 * return true if lines intersect
 *
 *    Lines are:    y = y0 + s * (x - x0)   ... 1
 *                  Y = Y0 + S * (X - X0)   ... 2
 */
bool
intersection(double x0, double y0, double x1, double y1,
	     double X0, double Y0, double X1, double Y1,
	     double *x, double *y)
{
	*x = *y = 0.0;		// just to define as something
	if (x0 == x1) {		// line 1 has infinite slope
		if (X0 == X1) {
			if (x0 == X1) {
				*x = x0;	// lines overlap so any value okay
				*y = y0;
				return true;
			} else {
				return false;	// parallel lines
			}
		} else {
			*x = x0;
			*y = Y0 + (Y1 - Y0) / (X1 - X0) * (x0 - X0);
			return true;
		}
	}
	if (X0 == X1) {		// line 2 has infinite slope
		if (x0 == x1) {
			if (x0 == X1) {
				*x = x0;	// lines overlap so any value okay
				*y = y0;
				return true;
			} else {
				return false;	// parallel lines
			}
		} else {
			*x = X0;
			*y = y0 + (y1 - y0) / (x1 - x0) * (X0 - x0);
			return true;
		}
	}
	// Lines are:    y = y0 + s * (x - x0)
	//               Y = Y0 + S * (X - X0)
	double s = (y1 - y0) / (x1 - x0);
	double S = (Y1 - Y0) / (X1 - X0);
	if (s == S)
		return false;		// parallel
	*x = ((s * x0 - S * X0) - (y0 - Y0)) / (s - S);
	*y = y0 + s * (*x - x0);
	return true;
}

char itoa_buf[50];
const char*
itoa(int i)
{
	sprintf(itoa_buf, "%d", i);
	return itoa_buf;
}

char dtoa_buf[50];
const char*
dtoa(double d)
{
	sprintf(dtoa_buf, "%g", d);
	//printf("DEBUG: dtoa(%f) returning '%s'\n",d,dtoa_buf);
	return dtoa_buf;
}

bool
grid_exists()
{
	Require(_xgrid_exists,
		err("First `set x grid' or `read grid x'"));
	Require(_ygrid_exists,
		err("First `set y grid' or `read grid y'"));
	Require(_grid_exists,
		err("First `read grid data' or `convert columns to grid'"));
	return true;
}

bool
using_proj()
{
	if (_proj_command.length() > 0)
		return true;
	else
		return false;
}

// Return true if can do whole list
bool
proj_lonlat_to_xy(double lon[], double lat[], unsigned int num, double x[], double y[])
{
#if !defined(HAVE_PROJ)
	for (unsigned int i = 0; i < num; i++) {
		x[i] = 0.0;
		y[i] = 0.0;
	}
#else
	XY p;
	extern PJ *_proj_pj; // in gre.cc
	extern double _proj_scale_factor; // in gre.cc
	double missing = gr_currentmissingvalue();
	for (unsigned int i = 0; i < num; i++) {
		if (gr_missingx(lon[i]) || gr_missingy(lat[i])) {
			x[i] = y[i] = missing;
		} else {
			p.u = lon[i] * DEG_TO_RAD;
			p.v = lat[i] * DEG_TO_RAD;
			p = pj_fwd(p, _proj_pj);
			x[i] = p.u * _proj_scale_factor;
			y[i] = p.v * _proj_scale_factor;
		}
		//printf("(%f,%f) -> (%f %f)\n", lon[i],lat[i],x[i],y[i]);
	}
#endif
	return true;		// okay
}	    

unsigned int			// same as perl
hash(const char *s)
{
	unsigned int slen = strlen(s);
	unsigned int i = slen;
	unsigned int h = 0;
	while(i--)
		h = h * 33 + *s++;
	return h;
}

static char getenv_buffer[512];
char*
call_getenv(const char *e)
{
#if defined(HAVE_GETENV)
	if (_gre_debug & DEBUG_OS_CALLS) printf("Getting value of environment-variable \"%s\" ...", e);
	char *result = getenv(e);
	if (result != NULL)
		strncpy(getenv_buffer, getenv(e), 511);
	else
		getenv_buffer[0] = '\0';
#else
	getenv_buffer[0] = '\0';
#endif
	if (_gre_debug & DEBUG_OS_CALLS) printf(" done\n");
	return getenv_buffer;
}

int
call_the_OS(const char *cmd)
{
	if (_gre_debug & DEBUG_OS_CALLS) printf("Asking OS to do \"%s\" ...\n", cmd);
	int rval = system(cmd);
	if (_gre_debug & DEBUG_OS_CALLS) printf(" done\n");
	return rval;
}
FILE*
call_popen(const char *cmd, const char *mode)
{
#if defined(HAVE_POPEN)
	FILE* rval;
	if (_gre_debug & DEBUG_OS_CALLS) printf("Opening pipe to OS command \"%s\" ...", cmd);
	rval = popen(cmd, mode);
	if (_gre_debug & DEBUG_OS_CALLS) printf(" done\n");
	return rval;
#else
	fprintf(stderr, "Cannot continue, since the operating system lacks the `popen()' subroutine");
	gre_exit(1);
	return (FILE*)NULL;		// never executed
#endif
}

int
call_OS_unlink(const char *file)
{
	int rval;
	if (_gre_debug & DEBUG_OS_CALLS) printf("Removing \"%s\" file ...", file);
#if HAVE_UNISTD_H
	rval = unlink(file);
	if (rval) {
		fprintf(stderr, "Error -- cannot unlink temporary file `%s'\n", file);
		return rval;
	}
#else
	string cmd = "rm -f ";
	cmd += file;
	rval = call_the_OS(cmd.c_str());
#endif
	if (_gre_debug & DEBUG_OS_CALLS) printf(" done\n");
	return rval;
}
void
string_to_pager(const char* s)
{
	char *fname = tmp_file_name();
	if (fname != NULL) {
		FILE *f = fopen(fname, "w");
		if (f) {
			fprintf(f, "%s", s);
			fclose(f);
			char sys_cmd[100];
			sprintf(sys_cmd, "more %s", fname);
			call_the_OS(sys_cmd);
			call_OS_unlink(fname);
			return;
		}
	}
	printf("%s", s);
}

bool
regexp_match(const char *re, const char *s, const char *flag, int subexp_max, regmatch_t *matchptr, int *subexpressions)
{
	//printf("regexp_match: input   re='%s'  s='%s'  flag='%s'\n",re, s, flag);
	regex_t compiled;
	unsigned int FLAG = REG_EXTENDED;
	unsigned int len = strlen(flag); // first character is '/'
	for (unsigned int i = 1; i < len; i++) {
		if (flag[i] == 'i')
			FLAG |= REG_ICASE;
		else {
			string msg = "Unknown flag `";
			msg += flag[i];
			msg += "' in regular expression.  Only `i' allowed.\n";
			fatal_error(msg.c_str());
		}
	}
	string regexp_new;
	create_gre_regexp(re, regexp_new);
	int res = regcomp(&compiled, regexp_new.c_str(), FLAG);
	if (res != 0) {
		string msg = "Error in regular expression: ";
		msg += decode_regexp_error(res);
		msg += "\n";
		fatal_error(msg.c_str());
	}
	*subexpressions = compiled.re_nsub;

	bool rval = false;

	res = regexec(&compiled, s, subexp_max, matchptr, 0);

        // Ensure didn't run out of memory
	if (res == REG_ESPACE) {
		string msg = "Out of memory when compiling regular expression `";
		msg += re;
		msg += "'\n";
		fatal_error(msg.c_str());
		return false;           // never executed, actually
	}

	rval = (res == 0) ? true : false;
	//printf("regexp_match: returning %d\n", int(rval));
	return rval;
}

// Transform regexp into a perl-like regexp, e.g. define
// "\s" to match any whitespace character, and "\w" to
// match any word character (alphanumeric plus _).
//
// MISSING PERLISH THINGS:
//    \b  Match a word boundary
//    \B  Match a non-(word boundary)
//    \A  Match at only beginning of string
//    \Z  Match at only end of string (or before newline at the end)
//    \G  Match only where previous m//g left off (works only with /g)
static void
create_gre_regexp(const char *in, string& out)
{
	out.assign(in);
	for (unsigned int i = 0; i < out.size() - 1; i++) {
		if (out[i] == '\\') {
			if (out[i+1] == 's') {
				// \s Match a whitespace character (space, tab, or newline).
				out.replace(i, 2, "[ \t\n]");
			} else if (out[i+1] == 'S') {
				// \S Match a nonwhitespace character.
				out.replace(i, 2, "[^ \t\n]");
			} else if (out[i+1] == 'd') {
				// \d Match a digit character.
				out.replace(i, 2, "[0123456789]");
			} else if (out[i+1] == 'D') {
				// \D Match a nondigit character.
				out.replace(i, 2, "[^0123456789]");
			} else if (out[i+1] == 'w') {
				// \w Match a single "word" character (alphanumeric plus "_")
				out.replace(i, 2, "[a-zA-Z0-9_]");
			} else if (out[i+1] == 'W') {
				// \W Match a single non-word character.
				out.replace(i, 2, "[^a-zA-Z0-9_]");
			}
		}
	}
	//printf("Regular expression\n\t'%s'\nconverted to perl-like\n\t'%s'\nexpression.\n\n", in, out.c_str());
}

static const char*
decode_regexp_error(int err) 
{
	switch (err) {
        case 0:
		return "";
        case REG_BADBR:
		return "Invalid `{...}' construct in regular expression.";
            
        case REG_BADPAT:
		return "Syntax error in regular expression.";
            
        case REG_BADRPT:
		return "Repetition operator (e.g. `?' or `*') has no preceding subexpression to act on).";
            
        case REG_ECOLLATE:
		return "Regular expression refers to an invalid collating element for current locale.";
            
        case REG_ECTYPE:
		return "Regular expression refers to an invalid character class name.";
            
        case REG_EESCAPE:
		return "Regular expression ends with `\\'.";
            
        case REG_ESUBREG:
		return "Regular expression has invalid number in the `\\DIGIT' construct.";
            
        case REG_EBRACK:
		return "Regular expression has unbalanced square brackets [].";
            
        case REG_EPAREN:
		return "Regular expression has unbalanced parentheses ().";
            
        case REG_EBRACE:
		return "Regular expression has unbalanced curly braces {}.";
            
        case REG_ERANGE:
		return "Regular expression has an invalid endpoint.";
            
        case REG_ESPACE:
		return "Ran out of memory while studying regular expression.";
        default:
		return "Unknown error in regular expression";
	}
}

bool
new_pageCmd()
{
	extern void reset_top_of_plot(void); // in set.cc
	gr_showpage();
	_need_x_axis = true;
	_need_y_axis = true;
	reset_top_of_plot();
	return true;
}
bool
pscommentCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params != 1) {
		err("`pscomment' takes 1 argument");
		return false;
	}
	extern FILE* _grPS;
	if (cmd_parameter_is_n(0)) {
		char msg[100];
		sprintf(msg, "%g", cmd_parameter_n(0));
		fprintf(_grPS, "%%%% %s\n", msg);
	} else {
		fprintf(_grPS, "%%%% %s\n", cmd_parameter_s(0));
	}
	return true;
}
/* Return transformed -- see code that calls it. */
double
quantize(double x, int levels, double dx)
{
	if (dx < 0.0)
		dx = -dx;
	if (dx)
		return (dx * levels / (1.0 + levels) * (floor(1.0 + x / dx)));
	else
		return x;
}
