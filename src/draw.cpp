#include <string>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "parser.h"
#include "errors.h"
#include "extern.h"
#include "GriPath.h"

using namespace std;

#if 0
#include "gr.hh"
#include "defaults.hh"
#endif
#include "image_ex.h"
#if 0
#include "GriPath.hh"
#include "GriTimer.hh"
#endif

double          _contour_space_first = -1.0;     // <-> set.c
double          _contour_space_later = -1.0;	 // <-> set.c
bool            _contour_space_centered = false; // <-> set.c
double          _contour_minlength = 0.0;	 // <-> lines must exceed this

extern FILE *_grPS;

#define OFFSET_AFTER_TITLE 1.0	// title is this many cm above plot 

static gr_symbol_type  determine_symbol_code(const char * s);
static bool draw_symbols(void);

#if 0
bool            draw_circleCmd(void);
#endif

bool            draw_contourCmd(void);
bool            draw_arcCmd(void);
bool            draw_arrowsCmd(void);

static bool draw_curve(bool filled, bool overlying);
static bool draw_curve_filled_to_value(bool to_y, double value);

#if 0
bool            draw_image_gridCmd(void);
bool            draw_image_histogramCmd(void);
bool            draw_image_paletteCmd(void);
#endif
bool            draw_imageCmd(void);
bool            draw_labelCmd(void);
bool            draw_line_fromCmd(void);
#if 0
bool            draw_patchesCmd(void);
#endif
bool            draw_polygonCmd(void);
#if 0
bool            draw_valuesCmd(void);
#endif
bool            draw_x_axisCmd(void);
bool            draw_y_axisCmd(void);
bool            draw_x_box_plotCmd(void);
bool            draw_y_box_plotCmd(void);
#if 0
bool            draw_zero_lineCmd(void);
bool            draw_zeroline_horizontally(void);
bool            draw_zeroline_vertically(void);
bool            set_x_scale(void);
#endif

void
set_line_width_symbol()
{
	_griState.set_linewidth_symbol(get_var_scalar("$_linewidth_symbol"));
	fprintf(_grPS, "%.3f w\n", _griState.linewidth_symbol());
}

void
set_line_width_curve()
{
	_griState.set_linewidth_line(get_var_scalar("$_linewidth"));
	fprintf(_grPS, "%.3f w\n", _griState.linewidth_line());
}

void
set_line_width_axis()
{
	_griState.set_linewidth_axis(get_var_scalar("$_linewidth_axis"));
	fprintf(_grPS, "%.3f w\n", _griState.linewidth_axis());
}

void
no_scales_error()
{
	err("Either x or y scale not defined.  Try using `set x axis' or `set y axis', or reading non-trivial dataset so that scales will be picked automatically.");
}

//cmd draw box ($$$$;$$) { # [filled] $ll_x $ll_y $ur_x $ur_y [cm]'
bool
draw_boxCmd()
{
	unsigned int num_params = cmd_parameters();
	bool in_cm = false;
	bool filled = false;
	if (cmd_parameter_is_n(0) == false) {
		if (strEQ(cmd_parameter_s(0), "filled"))
			filled = true;
		else {
			err("3rd word of `draw box ...' may only be `filled', if specified, not `\\", cmd_parameter_s(0), "' as given", "\\");
			return false;
		}
	}
	if (cmd_parameter_is_n(num_params - 1) == false) {
		if (strEQ(cmd_parameter_s(num_params - 1), "cm"))
			in_cm = true;
		else {
			err("last word of `draw box ...' may only be `cm', if specified, not `\\", cmd_parameter_s(num_params - 1), "' as given", "\\");
			return false;
		}
	}

	static GriPath p(5);	// static since might be done often
	p.clear();
	double llx = cmd_parameter_n(filled ? 1 : 0);
	double lly = cmd_parameter_n(filled ? 2 : 1);
	double urx = cmd_parameter_n(filled ? 3 : 2);
	double ury = cmd_parameter_n(filled ? 4 : 3);
	if (in_cm) {
		set_environment();
		p.push_back(llx, lly, 'm');
		p.push_back(urx, lly, 'l');
		p.push_back(urx, ury, 'l');
		p.push_back(llx, ury, 'l');
		//p.push_back(llx, lly, 'l');
		p.closepath();
		set_line_width_curve();
		if (filled)
			p.fill(units_cm);
		else
			p.stroke(units_cm);
		_drawingstarted = true;
	} else {
		if (!scales_defined()) {
			no_scales_error();
			return false;
		}
		// Coordinates in user units
		set_x_scale();
		set_y_scale();
		set_environment();
		p.push_back(llx, lly, 'm');
		p.push_back(urx, lly, 'l');
		p.push_back(urx, ury, 'l');
		p.push_back(llx, ury, 'l');
		//p.push_back(llx, lly, 'l');
		p.closepath();
		set_line_width_curve();
		if (filled)
			p.fill(units_user);
		else	
			p.stroke(units_user);
		_drawingstarted = true;
		draw_axes_if_needed();
	}
	return true;
}

//(;$$$$$) { # [$type [at $x $y [cm]]]
bool
draw_symbolCmd()
{
	unsigned int num_params = cmd_parameters();

	if (num_params == 0 || num_params == 1) 
		return draw_symbols();

	bool in_cm = false;
	if (num_params == 5) {
		if (strEQ("cm", cmd_parameter_s(4))) {
			in_cm = true;
		} else {
			err("5th word of `draw symbol ...' may only be `cm', if specified, not `\\", cmd_parameter_s(4), "' as given", "\\");
			return false;
		}
	}
	gr_symbol_type symbolCode;
	if (cmd_parameter_is_n(0)) {
		int tmp = int(floor(0.5 + cmd_parameter_n(0)));
		if (tmp <= 19) {
			symbolCode = gr_symbol_type(tmp);
		} else {
			symbolCode = gr_unknown_symbol;
		}
	} else {
		symbolCode = determine_symbol_code(cmd_parameter_s(0));
	}
	if (symbolCode == gr_unknown_symbol) {
		warning("`draw symbol' using bullet instead of specified, unknown, symbol");
		symbolCode = gr_bullet_symbol;
	}
	if (strcmp("at", cmd_parameter_s(1))) {
		err("4th word of `draw symbol ...' must be `at', not `\\", cmd_parameter_s(1), "'", "\\");
		return false;
	}
	double x = cmd_parameter_n(2);
	double y = cmd_parameter_n(3);
	if (!gr_missingx(x) && !gr_missingy(y)) {
		set_environment();
		set_ps_color('p');
		set_line_width_symbol();
		if (in_cm) {
			gr_drawsymbol(x, y, symbolCode);
		} else {
			if (using_proj()) {
				double xgeom, ygeom;
				proj_lonlat_to_xy(&x, &y, 1, &xgeom, &ygeom);
				x = xgeom;
				y = ygeom;
			}
			double xcm, ycm;
			gr_usertocm(x, y, &xcm, &ycm);
			gr_drawsymbol(xcm, ycm, symbolCode);
		}
		_drawingstarted = true;
		draw_axes_if_needed();
	}
	return true;
}

// `draw symbols'
// `draw symbols $type'
// OLD `draw symbol [[.code.|\name] [color hue z|.h. [brightness z|.b.] [saturation
// z|.s.]]]'
// OLD `draw symbol [[.code.|\name] [graylevel z]'
static bool
draw_symbols()
{
	unsigned int num_params = cmd_parameters();
	gr_symbol_type  symbolCode = gr_bullet_symbol;
	if (num_params == 1) {
		const char *t = cmd_parameter_s(0);
		symbolCode = determine_symbol_code(t);
		if (symbolCode == gr_unknown_symbol) {
			err("Unknown symbol `\\", t, "'", "\\");
			return false;
		}
	}
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		fatal_error("X column ($_x) has become non-existent");
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	var* yvar = &vector_vars[yind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw symbol' noticed that no X data exist");
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw symbol' noticed that no y data exist");
	if (xlen != ylen) {
		fatal_error("`draw symbol' requires xlen == ylen");
	}

#if 1                           // 19990117 - was 0 before
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
#endif
	set_environment();
	set_ps_color('p');
	set_line_width_symbol();
	if (using_proj()) {
		double *lat = new double[xlen];   if (!lat) OUT_OF_MEMORY;
		double *lon = new double[xlen];   if (!lon) OUT_OF_MEMORY;
		double *xgeom = new double[xlen]; if (!xgeom) OUT_OF_MEMORY;
		double *ygeom = new double[xlen]; if (!ygeom) OUT_OF_MEMORY;
		int xlen_nonmissing = 0;
		int i;
		for (i = 0; i < xlen; i++) {
			double xx = xvar->get_at_index(i).get_numeric();
			double yy = yvar->get_at_index(i).get_numeric();
			// 19990211 -- want *only* postscript clipping
			if (!gr_missingx(xx) && !gr_missingy(yy)) {
				lon[xlen_nonmissing] = xx;
				lat[xlen_nonmissing] = yy;
				xlen_nonmissing++;
			}
		}
		if (!proj_lonlat_to_xy(lon, lat, xlen_nonmissing, xgeom, ygeom)) {
			err("Error converting lat/lon data using 'proj'");
			return false;
		}
	
		for (i = 0; i < xlen_nonmissing; i++) {
			double xcm, ycm;
			gr_usertocm(xgeom[i], ygeom[i], &xcm, &ycm);
			gr_drawsymbol(xcm, ycm, symbolCode);
		}

		delete [] lat;
		delete [] lon;
		delete [] xgeom;
		delete [] ygeom;

	} else {
		int i;
		for (i = 0; i < xlen; i++) {
			double xx = xvar->get_at_index(i).get_numeric();
			double yy = yvar->get_at_index(i).get_numeric();
				// 19990211 -- want *only* postscript clipping
			if (!gr_missingx(xx) && !gr_missingy(yy)) {
				double xcm, ycm;
				gr_usertocm(xx, yy, &xcm, &ycm);
				gr_drawsymbol(xcm, ycm, symbolCode);
			}
		}
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}

static gr_symbol_type
determine_symbol_code(const char * s)
{
	double          tmp;
	bool            old = _ignore_error;
	_ignore_error = true;
	if (getdnum(s, &tmp)) {
		_ignore_error = old;
		return gr_symbol_type(int(floor(0.5 + (double) tmp)));
	} else if (!strcmp(s, "plus"))
		return gr_plus_symbol;
	else if (!strcmp(s, "times"))
		return gr_times_symbol;
	else if (!strcmp(s, "box"))
		return gr_box_symbol;
	else if (!strcmp(s, "circ"))
		return gr_circ_symbol;
	else if (!strcmp(s, "diamond"))
		return gr_diamond_symbol;
	else if (!strcmp(s, "triangleup"))
		return gr_triangleup_symbol;
	else if (!strcmp(s, "triangleright"))
		return gr_triangleright_symbol;
	else if (!strcmp(s, "triangledown"))
		return gr_triangledown_symbol;
	else if (!strcmp(s, "triangleleft"))
		return gr_triangleleft_symbol;
	else if (!strcmp(s, "asterisk"))
		return gr_asterisk_symbol;
	else if (!strcmp(s, "star"))
		return gr_star_symbol;
	else if (!strcmp(s, "filledbox"))
		return gr_filledbox_symbol;
	else if (!strcmp(s, "bullet"))
		return gr_bullet_symbol;
	else if (!strcmp(s, "filleddiamond"))
		return gr_filleddiamond_symbol;
	else if (!strcmp(s, "filledtriangleup"))
		return gr_filledtriangleup_symbol;
	else if (!strcmp(s, "filledtriangleright"))
		return gr_filledtriangleright_symbol;
	else if (!strcmp(s, "filledtriangledown"))
		return gr_filledtriangledown_symbol;
	else if (!strcmp(s, "filledtriangleleft"))
		return gr_filledtriangleleft_symbol;
	else if (!strcmp(s, "filledhalfmoonup"))
		return gr_filledhalfmoonup_symbol;
	else if (!strcmp(s, "filledhalfmoondown"))
		return gr_filledhalfmoondown_symbol;
	else
		return gr_unknown_symbol;
}

#if 0
// draw circle with radius .r_cm. at .x_cm. .y_cm. 
bool
draw_circleCmd()
{
	double          r_cm, x_cm, y_cm;
	double old_size = gr_currentsymbolsize_cm();
	switch (_nword) {
	case 8:
		if (!getdnum(_word[4], &r_cm))
			return false;
		if (!getdnum(_word[6], &x_cm))
			return false;
		if (!getdnum(_word[7], &y_cm))
			return false;
		gr_setsymbolsize_cm(2 * r_cm);
		set_line_width_symbol();
		gr_drawsymbol(x_cm, y_cm, gr_circ_symbol);
		gr_setsymbolsize_cm(old_size);
		return true;
	default:
		return false;
	}
}
#endif

// cmd draw contour (;$$$$$) { # [{$c [unlabelled|{labelled $lab}]} | {$min $max $inc [$inc_unlabelled] [unlabelled]}]

// BUG -- not all forms work, I think (1999 apr 11)

static bool
draw_contour(bool filled)
{
	unsigned int    num_params = cmd_parameters();
	extern bool     _contour_label_rotated;	// <-> startup.c set.c
	extern bool     _contour_label_whiteunder;	// <-> startup.c set.c
	double          min, max; //  inc, inc_unlabelled
	double          dlevel, dmin, dinc = 0.0, dinc_unlabelled = 0.0, dmax;
	double          contour_space_first = _contour_space_first;
	double          contour_space_later = _contour_space_later;
	double          contour_minlength = _contour_minlength;
	double          xsize = XSIZE_DEFAULT;
	double          ysize = YSIZE_DEFAULT;
	string          user_label;
	bool            user_gave_label = false;	// only for .value. format
	bool            have_unlabelled_and_labelled = false;
	bool            labelled = true;
	int             contour, numcontours = 0;

	//printf("\t`draw contour' num params = %d\n", num_params);

	// Check that data exist.
	if (!grid_exists())
		return false;
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
				// Decode command
	labelled = true;
	if (!cmd_parameter_is_n(num_params - 1)
	    && strEQ(cmd_parameter_s(num_params - 1), "unlabelled")) {
		labelled = false;
		num_params--;
	}
				//printf("\t`draw contour' labelled = %s\n", labelled ? "Y" : "N");

// DONE? NUM_PARAMS  SYNTAX
//  Y          0    draw contour
//  Y          1    draw contour $c
//  Y          3    draw contour $c labelled $lab
//  Y          3    draw contour $min $max $inc
//  ?          4    draw contour $min $max $inc $inc_unlabelled
// FOLLOWING ARE CAUGHT EARLY, WITH 'unlabelled' removed
//             2-1  draw contour $c unlabelled
//             4-1  draw contour $min $max $inc unlabelled
//             5-1  draw contour $min $max $inc $inc_unlabelled unlabelled

	int             nlevels;
	switch (num_params) {
	case 0:
		min = _f_min;		// matrix limits
		max = _f_max;		// matrix limits
		gr_scale125((double) min, (double) max, 5, &dmin, &dmax, &nlevels);
		dinc = (dmax - dmin) / nlevels;
		break;
	case 1:
		dmin = cmd_parameter_n(0);
		dmax = dmin;
		dinc = dmin;
		numcontours = 1;
		break;
	case 3:
		if (cmd_parameter_is_n(1)) {
			// draw contour $min $max $inc
			dmin = cmd_parameter_n(0);
			dmax = cmd_parameter_n(1);
			dinc = cmd_parameter_n(2);
			if ((dmin + dinc) > dmax) {
				err("Require ($min + $inc) <= $max");
				return false;
			}
			if (!gr_multiple(dmax - dmin, dinc, 0.001 * dinc)) {
				err("Require ($max - $min) to be a multiple of $inc to within 0.1%");
				return false;
			}
		} else {
			// draw contour $c labelled $lab
			expecting_the_keyword("`draw contour'", 1, "labelled");
			dmin = cmd_parameter_n(0);
			dmax = dinc = dmin;
			user_gave_label = true;	// label is _word[4]
			user_label.assign(cmd_parameter_s(2));
		}
		break;
	case 4:
		dmin = cmd_parameter_n(0);
		dmax = cmd_parameter_n(1);
		dinc = cmd_parameter_n(2);
		dinc_unlabelled = fabs(cmd_parameter_n(3));
		if (dinc < 0.0)
			dinc_unlabelled = -dinc_unlabelled;
		if ((dmin + dinc) > dmax) {
			err("Require ($min + $inc) <= $max");
			return false;
		}
		if (!gr_multiple(dmax - dmin, dinc, 0.001 * dinc)) {
			err("Require ($max - $min) to be a multiple of $inc to within 0.1%");
			return false;
		}
		if (!gr_multiple(dmax - dmin, dinc_unlabelled, 0.001 * dinc_unlabelled)) {
			err("Require ($max - $min) a multiple of $inc_unlabelled to within 0.1%");
			return false;
		}
		have_unlabelled_and_labelled = true;
		break;
	default:
		fatal_error("Sorry, 'draw contour' only works in crude way for now");
		break;
	}

#if 0
	else if (num_params == 1) {
		// `draw contour $c'
		double level = cmd_parameter_n(0);
		dmin = level;
		dmax = level;
		dinc = level;
		numcontours = 1;
	} else if (num_params == 3 && strEQ(cmd_parameter_s(1), "labelled")) {
		// `draw contour $c labelled $lab'
		double level = cmd_parameter_n(0);
		user_gave_label = true;	// label is _word[4]
		user_label.assign(cmd_parameter_s(1));
		if (user_label[0] == '"')
			user_label.remove(0, 1);
		if (user_label[user_label.size()-1] == '"')
			user_label.remove(user_label.size()-1,1);
		dmin = level;
		dmax = level;
		dinc = level;
		numcontours = 1;
	} else if (nword == 5) {
		// `draw contour .min. .max. .inc.'
		if (!getdnum(_word[2], &min)
		    || !getdnum(_word[3], &max)
		    || !getdnum(_word[4], &inc))
			return false;
		if (inc > 0.0) {
			if ((min + inc) > max) {
				demonstrate_command_usage();
				err("Increment has wrong sign to go between the stated min and max");
				return false;
			}
			dmin = min;
			dmax = max;
			dinc = inc;
		} else if (inc < 0.0) {
			if ((min + inc) < max) {
				demonstrate_command_usage();
				err("Increment has wrong sign to go between the stated min and max");
				return false;
			}
			dmin = max;
			dmax = min;
			dinc = -inc;
		} else {
			err("Cannot use increment of zero.");
			return false;
		}
		if (!gr_multiple(dmax - dmin, dinc, 0.001 * dinc)) {
			demonstrate_command_usage();
			err("Require (.max. - .min.) a multiple of .inc. to within 0.1%");
			return false;
		}
	} else if (nword == 6) {
		// `draw contour .min. .max. .inc. .inc_unlabelled.'
		if (!getdnum(_word[2], &min)
		    || !getdnum(_word[3], &max)
		    || !getdnum(_word[4], &inc)
		    || !getdnum(_word[5], &inc_unlabelled))
			return false;
		if ((min + inc) > max) {
			demonstrate_command_usage();
			err("Require (.min. + .inc.) <= .max.");
			return false;
		}
		// Ensure same sign for unlabelled increment
		if (inc < 0.0)
			inc_unlabelled = -fabs(inc_unlabelled);
		else
			inc_unlabelled = fabs(inc_unlabelled);
		dmin = min;
		dmax = max;
		dinc = inc;
		dinc_unlabelled = inc_unlabelled;
		if (!gr_multiple(dmax - dmin, dinc, 0.001 * dinc)) {
			demonstrate_command_usage();
			err("Require (.max. - .min.) a multiple of .inc. to within 0.1%");
			return false;
		}
		if (!gr_multiple(dmax - dmin, dinc_unlabelled, 0.001 * dinc_unlabelled)) {
			demonstrate_command_usage();
			err("Require (.max. - .min.) a multiple of .inc_unlabelled. to within 0.1%");
			return false;
		}
		have_unlabelled_and_labelled = true;
	} else {
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	if (dinc == 0.0 && dmin != 0.0 && dmax != 0.0) {
		demonstrate_command_usage();
		err("`draw contour .min. .max. 0' not allowed");
		return false;
	}
				// Do contour(s)
	if (!get_var("..xsize..", &xsize))
		warning("Don't know value of ..xsize.. so using XSIZE_DEFAULT");
	if (!get_var("..ysize..", &ysize))
		warning("Don't know value of ..ysize.. so using YSIZE_DEFAULT");
#endif
	set_environment();
	set_line_width_curve();
				// If numcontours not already set, try to figure it out.
	if (!numcontours) {
		if (have_unlabelled_and_labelled) {
			if (dinc_unlabelled == 0.0 && dmin == 0.0 && dmax == 0.0)
				numcontours = 1;
			else
				numcontours = 1 + (int) fabs(0.5 + (dmax - dmin) / dinc_unlabelled);
		} else {
			if (dinc == dmin && dmax == dmin)
				numcontours = 1;
			else
				numcontours = 1 + (int) fabs(0.5 + (dmax - dmin) / dinc);
		}
	}
	dlevel = dmax;
#if 0
	bool warned = false;
	GriTimer t;
#endif
	GriString label;
	for (contour = 0; contour < numcontours; contour++) {
		if (labelled && (numcontours == 1 || gr_multiple(dlevel, dinc, 0.001 * dinc))) {
			if (fabs(dlevel) <= 1.0e-6 * fabs(dinc)) {
				label.assign("0");
			} else {
				char tmp[1000];
				sprintf(tmp, _contourFmt.c_str(), dlevel);
				label.assign(tmp);
			}
			// Figure distance spacing for contours.  Use stored values if
			// the space-later is > 0; otherwise do old default
			if (_contour_space_later < 0.0) {
				contour_space_first = 1.0;
				contour_space_later = labelled ? 0.5 * (xsize + ysize) : 0.0;
			} else {
				contour_space_first = _contour_space_first;
				contour_space_later = _contour_space_later;
			}
		} else {
			label.assign("");
			contour_space_first = 0.0;
			contour_space_later = 0.0;
		}
		if (user_gave_label) {
			if (_f_min <= dlevel && dlevel <= _f_max) {
				gr_contour(_xmatrix,
					   _ymatrix,
					   _f_xy,
					   _legit_xy,
					   _xmatrix.size(),
					   _ymatrix.size(),
					   dlevel,
					   user_label.c_str(),
					   _contour_label_rotated,
					   _contour_label_whiteunder,
					   _contour_space_centered,
					   _griState.color_line(),
					   _griState.color_text(),
					   contour_minlength,
					   contour_space_first,
					   contour_space_later,
					   NULL,
					   filled);
			}
		} else {
			if (_f_min <= dlevel && dlevel <= _f_max) {
				gr_contour(_xmatrix,
					   _ymatrix,
					   _f_xy,
					   _legit_xy,
					   _xmatrix.size(),
					   _ymatrix.size(),
					   dlevel,
					   label.c_str(),
					   _contour_label_rotated,
					   _contour_label_whiteunder,
					   _contour_space_centered,
					   _griState.color_line(),
					   _griState.color_text(),
					   contour_minlength,
					   contour_space_first,
					   contour_space_later,
					   NULL,
					   filled);
			}
		}
		if (have_unlabelled_and_labelled)
			dlevel -= dinc_unlabelled;
		else
			dlevel -= dinc;
#if 0
		if (!warned) {
			double frac = (1.0 + contour) / numcontours;
			warned = warn_if_slow(&t, frac, "draw contour");
		}
#endif
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}
bool
draw_filled_contourCmd()
{
	return draw_contour(true);
}
bool
draw_contourCmd()
{
	return draw_contour(false);
}

bool
draw_arrowsCmd()
{
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		fatal_error("X column ($_x) has become non-existent");
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		fatal_error("Y column ($_y) has become non-existent");
	int uind = get_vector_var_index("$_u");
	if (uind < 0)
		fatal_error("U column ($_u) has become non-existent");
	int vind = get_vector_var_index("$_v");
	if (vind < 0)
		fatal_error("V column ($_v) has become non-existent");
	
	extern vector<var> vector_vars;
	
	var* xvar = &vector_vars[xind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw arrows' noticed that no x data exist");
	
	var* yvar = &vector_vars[yind];
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw arrows' noticed that no y data exist");
	
	var* uvar = &vector_vars[uind];
	int ulen = 1 + uvar->get_end_index();
	if (ulen < 1)
		fatal_error("`draw arrows' noticed that no u data exist");
	
	var* vvar = &vector_vars[vind];
	int vlen = 1 + vvar->get_end_index();
	if (vlen < 1)
		fatal_error("`draw arrows' noticed that no v data exist");
	
	if (xlen != ylen)
		fatal_error("`draw arrows' requires xlen == ylen");
	
	if (xlen != ulen)
		fatal_error("`draw arrows' requires xlen == ulen");
	
	if (xlen != vlen)
		fatal_error("`draw arrows' requires xlen == vlen");
	
	if (!scales_defined()) {
		err("`draw arrows' failed because no x/y scales defined\n");
		return false;
	}
	if (!_uscale_exists) {
		err("Cannot `draw arrows' without first `set u scale'");
		return false;
	}
	if (!_vscale_exists) {
		err("Cannot `draw arrows' without first `set v scale'");
		return false;
	}
	double halfwidth = get_var_scalar("$_arrow_size");
				//printf("halfwidth = %f\n", halfwidth);
	set_environment();
	set_line_width_curve();
	int i;
	for (i = 0; i < xlen; i++) {
		double xx = xvar->get_at_index(i).get_numeric();
		double yy = yvar->get_at_index(i).get_numeric();
		double uu = uvar->get_at_index(i).get_numeric();
		double vv = vvar->get_at_index(i).get_numeric();
		// 19990211 -- want *only* postscript clipping
		if (!gr_missingx(xx) && !gr_missingy(yy)
		    && !gr_missing(uu) && !gr_missing(vv)
		    && (uu != 0.0 || vv != 0.0)) {
			double          x0, y0, x1, y1;
			gr_usertocm(xx, yy, &x0, &y0);
			x1 = x0 + uu * _cm_per_u;
			y1 = y0 + vv * _cm_per_v;
			switch(_arrow_type) {
			case 0: gr_drawarrow_cm(x0, y0, x1, y1, halfwidth);  break;
			case 1: gr_drawarrow2_cm(x0, y0, x1, y1, halfwidth); break;
			case 2: gr_drawarrow3_cm(x0, y0, x1, y1, halfwidth); break;
			default:gr_drawarrow_cm(x0, y0, x1, y1, halfwidth);  break;
			}
		}
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}

//cmd draw arc ($$$$$;$) { # [filled] $xc $yc $r $angle1 $angle2
bool
draw_arcCmd()
{
	unsigned int num_params = cmd_parameters();
	bool filled = false;
	int first_param = 0;
	if (num_params == 6) {
		if (strEQ("filled", cmd_parameter_s(0))) {
			filled = true; 
			first_param++;
		} else {
			err("`draw arc' expecting `filled' but got `\\", cmd_parameter_s(0), "'.", "\\");
			return false;
		}
	} else if (num_params != 5) {
		err("`draw arc' requires 5 or 6 parameters, [filled] $xc $yc $r $angle1 $angle2\n");
		return false;
	}
	expecting_numeric("`draw arc ...'", first_param+0);
	double xc = cmd_parameter_n(first_param+0);
	expecting_numeric("`draw arc ...'", first_param+1);
	double yc = cmd_parameter_n(first_param+1);
	expecting_numeric("`draw arc ...'", first_param+2);
	double r = cmd_parameter_n(first_param+2);
	expecting_numeric("`draw arc ...'", first_param+3);
	double angle1 = cmd_parameter_n(first_param+3);
	expecting_numeric("`draw arc ...'", first_param+4);
	double angle2 = cmd_parameter_n(first_param+4);

	set_environment();
	set_ps_color('p');
	set_line_width_curve();

	extern FILE *_grPS;

	if (filled) { 
		fprintf(_grPS, "%.1f %.1f m  %.1f %.1f %.1f %.1f %.1f arc fill\n",
			xc * PT_PER_CM + r * PT_PER_CM, yc * PT_PER_CM,
			xc * PT_PER_CM, yc * PT_PER_CM,
			r * PT_PER_CM,
			angle1, angle2);
	} else {
		fprintf(_grPS, "%.1f %.1f m  %.1f %.1f %.1f %.1f %.1f arc stroke\n",
			xc * PT_PER_CM + r * PT_PER_CM, yc * PT_PER_CM,
			xc * PT_PER_CM, yc * PT_PER_CM,
			r * PT_PER_CM,
			angle1, angle2);
	}

	double lw = _griState.linewidth_line() / 2.0 / PT_PER_CM;
	rectangle bbox(xc - r - lw, yc - r - lw,
		       xc + r + lw, yc + r + lw);
	bounding_box_update(bbox);
	_drawingstarted = true;
	return true;
}

//cmd draw arrow ($$$$$$;$) # "from" $x0 $y0 "to" $x1 $y1 [cm]
bool
draw_arrowCmd()
{
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (cmd_parameter_is_n(0)) {
		err("`draw arrow' first parameter must be `from', not `\\",
		    dtoa(cmd_parameter_n(0)), "' as given", "\\");
		return false;
	}
	if (strNE("from", cmd_parameter_s(0))) {
		err("`draw arrow' first parameter must be `from', not `\\",
		    cmd_parameter_s(0), "' as given", "\\");
		return false;
	}
	if (cmd_parameter_is_n(3)) {
		err("`draw arrow' first parameter must be `to', not `\\",
		    dtoa(cmd_parameter_n(3)), "' as given", "\\");
		return false;
	}
	if (strNE("to", cmd_parameter_s(3))) {
		err("`draw arrow' first parameter must be `to', not `\\",
		    cmd_parameter_s(3), "' as given", "\\");
		return false;
	}
	bool is_cm = false;
	if (cmd_parameters() == 7) {
		if (cmd_parameter_is_n(6)) {
			err("`draw arrow' last (6th) parameter must be `cm' if specified, not `\\", dtoa(cmd_parameter_n(6)), "' as given", "\\");
			return false;
		}
		if (strNE("cm", cmd_parameter_s(6))) {
			err("`draw arrow' last (6th) parameter must be `cm' if specified, not `\\", cmd_parameter_s(6), "' as given", "\\");
			return false;
		}
		is_cm = true;
	}
	set_environment();
	set_line_width_curve();
	double halfwidth = get_var_scalar("$_arrow_size");
	double x0 = cmd_parameter_n(1), x0_cm;
	double y0 = cmd_parameter_n(2), y0_cm;
	double x1 = cmd_parameter_n(4), x1_cm;
	double y1 = cmd_parameter_n(5), y1_cm;
	if (is_cm) {
		x0_cm = x0;
		y0_cm = y0;
		x1_cm = x1;
		y1_cm = y1;
	} else {
		if (using_proj()) {
			double xgeom, ygeom;
			proj_lonlat_to_xy(&x0, &y0, 1, &xgeom, &ygeom);
			gr_usertocm(xgeom, ygeom, &x0_cm, &y0_cm);
			proj_lonlat_to_xy(&x1, &y1, 1, &xgeom, &ygeom);
			gr_usertocm(xgeom, ygeom, &x1_cm, &y1_cm);
		} else {
			gr_usertocm(x0, y0, &x0_cm, &y0_cm);
			gr_usertocm(x1, y1, &x1_cm, &y1_cm);
		}
	}
	//printf("arrow type %d\n", _arrow_type);
	switch(_arrow_type) {
	case 0: 
		gr_drawarrow_cm(x0_cm, y0_cm, x1_cm, y1_cm, halfwidth);  
		break;
	case 1:
		gr_drawarrow2_cm(x0_cm, y0_cm, x1_cm, y1_cm, halfwidth);
		break;
	case 2:
		gr_drawarrow3_cm(x0_cm, y0_cm, x1_cm, y1_cm, halfwidth); 
		break;
	default:
		gr_drawarrow_cm(x0_cm, y0_cm, x1_cm, y1_cm, halfwidth);
		break;
	}
	_drawingstarted = true;
	return true;
}

bool
draw_axesCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params == 0) {
		draw_axes(int(_axesStyle), 0.0, (gr_axis_properties) 0, true);
	} else if (num_params == 1) {
		if (cmd_parameter_is_n(0)) {
			int type = int(floor(0.5 + cmd_parameter_n(0)));
			if (type < 0 || type > 2) {
				err("numeric type for `draw axis' must be in range 0 to 2 inclusive, not \\",
				    dtoa(cmd_parameter_n(0)), " as given", "\\");
				return false;
			}
			draw_axes(type, 0.0, (gr_axis_properties) 0, true);
		} else {
			if (strEQ("none", cmd_parameter_s(0))) {
				_drawingstarted = true;
				_need_x_axis = false;
				_need_y_axis = false;
			} else if (strEQ("frame", cmd_parameter_s(0))) {
				draw_axes(2, 0.0, (gr_axis_properties) 0, true);
			} else {
				err("3rd word of `draw axes' must be `none' or `frame', if specified, not '\\", cmd_parameter_s(0), "' as given", "\\");
				return false;
			}
		}
	} else {
		err("`draw axes' may not have more than 1 parameter");
		return false;
	}
	return true;
}

void
draw_outline_frame()
{
	int old_linecap = _griState.line_cap();
	int old_linejoin = _griState.line_join();
	_griState.set_line_cap(0);
	_griState.set_line_join(0);
	GriPath p(5);
	p.push_back(_xleft, _ytop, 'm');
	p.push_back(_xright, _ytop, 'l');
	p.push_back(_xright, _ybottom, 'l');
	p.push_back(_xleft, _ybottom, 'l');
	p.push_back(_xleft, _ytop, 'l');
	p.stroke(units_user, _griState.linewidth_axis());
	_griState.set_line_cap(old_linecap);
	_griState.set_line_join(old_linejoin);
}

bool
draw_axes(int type, double loc, gr_axis_properties side, bool allow_offset)
{
	double          tic_direction = 0; // tics extend out by default
	double          tic_size = TICSIZE_DEFAULT;
	double          ysize = YSIZE_DEFAULT;
	double          ymargin = YMARGIN_DEFAULT;
#if 0                           // 19990117 - not used
	double          xsize = XSIZE_DEFAULT;
	double          xmargin = XMARGIN_DEFAULT;
#endif
	double          fontsize = FONTSIZE_PT_DEFAULT;
	double          oldFontsize_pt = gr_currentfontsize_pt();
	gr_fontID       old_font = gr_currentfont();
	double          tmpx, tmpx_cm, tmpy, tmpy_cm;
	double          axes_offset;
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (allow_offset)
		axes_offset = _axes_offset;
	else
		axes_offset = 0.0;
	gr_setxtransform(_xtype);
	gr_setytransform(_ytype);
	//REMOVED//gr_setxlabel(_colX.getName());
	//REMOVED//gr_setylabel(_colY.getName());
	//REMOVED-2000-5-2 gr_setxnumberformat(_xFmt.c_str());
	//REMOVED-2000-5-2 gr_setynumberformat(_yFmt.c_str());
	gr_setxsubdivisions(_xsubdiv);
	gr_setysubdivisions(_ysubdiv);
	tic_direction = get_var_scalar("..tic_direction");
	gr_setticdirection(int(floor(0.5 + tic_direction)) ? true : false);
	tic_size = get_var_scalar("$_tic_size");
	gr_setticsize_cm(tic_size);
	fontsize = get_var_scalar("$_font_size");
	gr_setfontsize_pt(fontsize);
	gr_setfont(old_font);
#if 0                           // 19990117 - not used
	xsize = get_var_scalar("$_xsize");
	xmargin = get_var_scalar("$_xmargin");
#endif
	ymargin = get_var_scalar("$_ymargin");
	ysize = get_var_scalar("$_ysize");
	set_x_scale();
	set_y_scale();
	// Set to proper linewidth, and turn dashing off
	set_line_width_axis();
	vector<double> old_dash;
	unsigned int i;
	for (i = 0; i < _dash.size(); i++)
		old_dash.push_back(_dash[i]);
	vector<double> dash;
	_dash.erase(_dash.begin(), _dash.end());

	switch (type) {
	case 0:			// full axes 
		if (_xatbottom) {
			gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm - axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, gr_axis_BOTTOM);
			gr_setfontsize_pt(0.0);
			gr_usertocm(_xleft, _ytop, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm + axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, gr_axis_TOP);
			gr_setfontsize_pt(fontsize);
		} else {
			gr_setfontsize_pt(0.0);
			gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm - axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, gr_axis_BOTTOM);
			gr_setfontsize_pt(fontsize);
			gr_usertocm(_xleft, _ytop, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm + axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, gr_axis_TOP);
		}
		if (_yatleft == true) {
			gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm - axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, gr_axis_LEFT);
			gr_setfontsize_pt(0.0);
			gr_usertocm(_xright, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm + axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, gr_axis_RIGHT);
			gr_setfontsize_pt(fontsize);
		} else {
			gr_setfontsize_pt(0.0);
			gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm - axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, gr_axis_LEFT);
			gr_setfontsize_pt(fontsize);
			gr_usertocm(_xright, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm + axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, gr_axis_RIGHT);
		}
		_need_x_axis = false;
		_need_y_axis = false;
		if ((ymargin + ysize) > _top_of_plot)
			_top_of_plot = ymargin + ysize;
		break;
	case 1:			// axes at left and bottom + simple frame 
		gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
		gr_cmtouser(tmpx_cm, tmpy_cm - axes_offset, &tmpx, &tmpy);
		gr_drawxaxis(tmpy, _xleft, _xinc, _xright, gr_axis_BOTTOM);
		gr_usertocm(_xleft, _ybottom, &tmpx_cm, &tmpy_cm);
		gr_cmtouser(tmpx_cm - axes_offset, tmpy_cm, &tmpx, &tmpy);
		gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, gr_axis_LEFT);
		draw_outline_frame();
		_need_x_axis = false;
		_need_y_axis = false;
		if ((ymargin + ysize) > _top_of_plot)
			_top_of_plot = ymargin + ysize;
		break;
	case 2:			// simple frame, no axes 
		draw_outline_frame();
		_need_x_axis = false;
		_need_y_axis = false;
		if ((ymargin + ysize) > _top_of_plot)
			_top_of_plot = ymargin + ysize;
		break;
	case 3:			// x axis only -- don't do frame if offset 
		if (side == gr_axis_BOTTOM) {
			gr_usertocm(_xleft, loc, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm - axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, side);
		} else {
			gr_usertocm(_xleft, loc, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm, tmpy_cm + axes_offset, &tmpx, &tmpy);
			gr_drawxaxis(tmpy, _xleft, _xinc, _xright, side);
		}
		_need_x_axis = false;
		{
			// add space for tics (maybe), space, number, space, label, and
			// then some inter-axis space
			double          x_cm, y_cm;
			extern double  _grTicSize_cm;
			extern bool    _grTicsPointIn;
			gr_usertocm(_xleft, loc, &x_cm, &y_cm);
			y_cm += _grTicsPointIn == true ? 0.0 : _grTicSize_cm;
			y_cm += 5.0 * gr_currentCapHeight_cm();
			if (y_cm > _top_of_plot)
				_top_of_plot = y_cm;
		}
		break;
	case 4:			// y axis only -- don't do frame if offset 
		if (side == gr_axis_BOTTOM) {
			gr_usertocm(loc, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm - axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, side);
		} else {
			gr_usertocm(loc, _ybottom, &tmpx_cm, &tmpy_cm);
			gr_cmtouser(tmpx_cm + axes_offset, tmpy_cm, &tmpx, &tmpy);
			gr_drawyaxis(tmpx, _ybottom, _yinc, _ytop, side);
		}
		_need_y_axis = false;
		{
			// add space 
			double          x_cm, y_cm;
			gr_usertocm(loc, _ytop, &x_cm, &y_cm);
			if (y_cm > _top_of_plot)
				_top_of_plot = y_cm;
		}
		break;
	default:
		err("unknown axis type");
	}
	gr_setfontsize_pt(oldFontsize_pt);
	_drawingstarted = true;
#if 0                           // 19990117 - we're doing that anyway!
	draw_axes_if_needed();
#endif
	for (i = 0; i < old_dash.size(); i++)
		_dash.push_back(old_dash[i]);
	return true;
}
				
//(;$$$$) { # [overlying|{filled [{to x $x}]|[{to y $y}]}
bool
draw_curveCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params == 0) {
		return draw_curve(false, false);
	}
	if (num_params == 1) {
		if (cmd_parameter_is_n(0)) {
			err("`draw curve' expecting keyword `overlying' or `filled' but got numerical expression\\", dtoa(cmd_parameter_n(0)), "\\");
			return false;
		}
		if (strEQ(cmd_parameter_s(0), "overlying")) {
			return draw_curve(false, true);
			return false;
		} else if (strEQ(cmd_parameter_s(0), "filled")) {
			return draw_curve(true, false);
		} else {
			err("`draw curve' expecting keyword `filled' or `overlying' got `\\", cmd_parameter_s(0), "'", "\\");
			return false;
		}
	}

	// filled to x|y $value
	if (num_params != 4) {
		err("`draw curve' must have 0, 1 or 4 parameters, not \\", itoa(num_params), " as given", "\\");
		return false;
	}

	// filled to x|y $value ... check that 'filled'
	if (strNE(cmd_parameter_s(0), "filled")) {
		err("`draw curve' expecting keyword `filled' but got `\\", cmd_parameter_s(0), "'", "\\");
		return false;
	}

	// filled to x|y $value ... check that 'to'
	if (cmd_parameter_is_n(1)) {
		err("`draw curve filled' expecting keyword `to' but got numerical expression\\", dtoa(cmd_parameter_n(1)), "\\");
		return false;
	}
	if (strNE(cmd_parameter_s(1), "to")) {
		err("`draw curve filled' expecting keyword `to' `\\", cmd_parameter_s(1), "'", "\\");
		return false;
	}

	// filled to x|y $value ... check that 'x' or 'y'
	if (cmd_parameter_is_n(2)) {
		err("`draw curve filled to' expecting keyword `x' or `y' but got numerical expression\\", dtoa(cmd_parameter_n(2)), "\\");
		return false;
	}
	bool filled_to_y = true;
	if (strEQ(cmd_parameter_s(2), "x")) {
		filled_to_y = false;
	} else if (strEQ(cmd_parameter_s(2), "y")) {
		filled_to_y = true;
	} else {
		err("`draw curve filled to' expecting keyword `x' or `y' but got `\\", cmd_parameter_s(2), "'", "\\");
		return false;
	}

	if (cmd_parameter_is_n(3) == false) {
		if (filled_to_y)
			err("`draw curve filled to y' expecting numerical expression but got string value `\\", cmd_parameter_s(3), "'", "\\");
		else
			err("`draw curve filled to x' expecting numerical expression but got string value `\\", cmd_parameter_s(3), "'", "\\");
		return false;
	}
	return draw_curve_filled_to_value(filled_to_y, cmd_parameter_n(3));
}

static bool
draw_curve(bool filled, bool overlying)
{
	int xind = get_vector_var_index("$_x"); if (xind < 0) fatal_error("X column ($_x) has become non-existent");
	int yind = get_vector_var_index("$_y"); if (yind < 0) fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	var* yvar = &vector_vars[yind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw curve' noticed that no x data exist");
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw curve' noticed that no y data exist");
	if (xlen != ylen)
		fatal_error("`draw curve' requires xlen == ylen");
	
#if 1                           // 19990117 - was 0 before
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
#endif
	
	set_environment();
	
	double          lastx = gr_currentmissingvalue();
	double          lasty = gr_currentmissingvalue();
	bool            first = true, last_OK = true;
	GriPath  path(xlen);
	
	if (using_proj()) {
		int i;
		for (i = 0; i < xlen; i++) {
			double xx = xvar->get_at_index(i).get_numeric();
			double yy = yvar->get_at_index(i).get_numeric();
			// 19990211 -- want *only* postscript clipping
			if (!gr_missingx(xx) && !gr_missingy(yy)) {
				double xgeom, ygeom;
				proj_lonlat_to_xy(&xx, &yy, 1, &xgeom, &ygeom);
				if (first) {
					path.push_back(xgeom, ygeom, 'm');
					first = false;
				} else {
					if (last_OK) {
						path.push_back(xgeom, ygeom, 'l');
						lastx = xgeom;
						lasty = ygeom;
					} else {
						path.push_back(xgeom, ygeom, 'm');
					}
				}
				last_OK = true;
			} else {
				last_OK = false;
			}
		}
	} else {
		int i;
		for (i = 0; i < xlen; i++) {
			double xx = xvar->get_at_index(i).get_numeric();
			double yy = yvar->get_at_index(i).get_numeric();
			// 19990211 -- want *only* postscript clipping
			if (!gr_missingx(xx) && !gr_missingy(yy)) {
				if (first) {
					path.push_back(xx, yy, 'm');
					first = false;
				} else {
					if (last_OK) {
						path.push_back(xx, yy, 'l');
						lastx = xx;
						lasty = yy;
					} else {
						path.push_back(xx, yy, 'm');
					}
				}
				last_OK = true;
			} else {
				last_OK = false;
			}
		}
	}
	
	if (filled) {
		set_environment();
		set_line_width_curve();
		path.fill(units_user);
	} else {
		if (overlying) {
			double r, g, b;
			_griState.color_line().getRGB(&r, &g, &b);
			GriColor c;
			c.setRGB(1.0, 1.0, 1.0);
			_griState.set_color_line(c);
			double linewidth_actual = get_var_scalar("$_linewidth");
			put_var("$_linewidth", 3 * linewidth_actual);
			set_environment();
			set_line_width_curve();
			
			path.stroke(units_user);
			c.setRGB(r, g, b);
			_griState.set_color_line(c);
			put_var("$_linewidth", linewidth_actual);
			set_environment();
			set_line_width_curve();
			path.stroke(units_user);
		} else {
			set_environment();
			set_line_width_curve();
			path.stroke(units_user);
		}
	}
	put_var("$_lastx", lastx);
	put_var("$_lasty", lasty);
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}

static bool
draw_curve_filled_to_value(bool to_y, double value)
{
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		fatal_error("X column ($_x) has become non-existent");
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	var* yvar = &vector_vars[yind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw curve' noticed that no X data exist");
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw curve' noticed that no y data exist");
	if (xlen != ylen)
		fatal_error("`draw curve' requires xlen == ylen");
	
	GriPath p;
	set_environment();
	int i;
	bool last_OK = false, path_exists = false;
	double lastx = 0.0, lasty = 0.0;
	for (i = 0; i < xlen; i++) {
		double xx = xvar->get_at_index(i).get_numeric();
		double yy = yvar->get_at_index(i).get_numeric();
		// 19990211 -- want *only* postscript clipping
		if (!gr_missingx(xx) && !gr_missingy(yy)) {
			// This point is not missing, and it's in the box.
			if (!last_OK) {
				// Last point was not OK (or this is the first point), so
				// start a new path, beginning at y=value so the fill will
				// work correctly.
				path_exists = true;
				if (to_y)
					p.push_back(xx, value, 'm');
				else
					p.push_back(value, yy, 'm');
			}
			p.push_back(xx, yy, 'l');
			lastx = xx;
			lasty = yy;
			last_OK = true;
		} else {
			// This point is either missing or not in the box.
			if (last_OK) {
				// Must have just finished a run of good data.  Continue the
				// path to y=value, then fill it.
				if (to_y)
					p.push_back(lastx, value, 'l');
				else
					p.push_back(value, lasty, 'l');
				p.fill(units_user);
				path_exists = false;
			}
			// Last was not OK.  Nothing to do but wait.
			last_OK = false;
		}
	}
	if (path_exists) {
		if (to_y)
			p.push_back(lastx, value, 'l');
		else
			p.push_back(value, lasty, 'l');
		p.fill(units_user);
		put_var("$_lastx", lastx);
		put_var("$_lasty", lasty);
		draw_axes_if_needed();
		_drawingstarted = true;
	}
	return true;
}

// CHANGE: before 2.052, put circles at missing points
// (;)
bool
draw_gridCmd()
{
	if (!_grid_exists) {
		err("First create the grid");
		return false;
	}
	if (!_xgrid_exists || _xmatrix.size() < 1) {
		err("First `read grid x' or `set x grid'");
		return false;
	}
	if (!_ygrid_exists || _ymatrix.size() < 1) {
		err("First `read grid y' or `set y grid'");
		return false;
	}
	set_ps_color('p');
	set_environment();
	set_line_width_symbol();
	unsigned j = _ymatrix.size() - 1;
	set_line_width_symbol();
	do {
		double xcm, ycm;
		unsigned int nx = _xmatrix.size();
		for (unsigned int i = 0; i < nx; i++) {
			// 19990211 -- want *only* postscript clipping
			if (_legit_xy(i,j) == true) {
				gr_usertocm(_xmatrix[i], _ymatrix[j], &xcm, &ycm);
				gr_drawsymbol(xcm, ycm, gr_plus_symbol);
			}
		}
	} while (j-- != 0);
	return true;
}

#if 0
// `draw image histogram [box .ll_x_cm. .ll_y_cm. .ur_x_cm. .ur_y_cm.]'
bool
draw_image_histogramCmd()
{
	int             i;
	double          x_ll_cm = XMARGIN_DEFAULT, y_ll_cm, x_ur_cm, y_ur_cm;
	double          dx = XSIZE_DEFAULT, dy;
	double          xval, dxval, yMin = 1.e-4, yMax = 1.0;
	double          left, right;
	int             num;
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (!image_exists()) {
		err("Must `read image' or `convert grid to image' before `draw image'.");
		return false;
	}
	if (!_imageTransform_exists) {
		err("Must `set image colorscale' before `draw image'");
		return false;
	}
	switch (_nword) {
	case 3:
		// `draw image histogram' 
		if (!get_var("..xmargin..", &x_ll_cm))
			warning("Don't know value of ..xmargin.. so using XMARGIN_DEFAULT");
		y_ll_cm = _top_of_plot + 1.5;
		if (!get_var("..xsize..", &dx))
			warning("Don't know value of ..xsize.. so using XSIZE_DEFAULT");
		dy = 2.5;
		break;
	case 7:
		err("Sorry, but the old `draw histogram .ll_x. .ll_y. .dx. .dy.' is\n\
now `draw image histogram [box .ll_x_cm. .ll_y_cm. .ur_x_cm. .ur_y_cm.]'.\n\
Note the extra word `box', and the new meaning of the last 2 parameters.");
		return false;
	case 8:
		// `draw image histogram [box .ll_x_cm. .ll_y_cm. .ur_x_cm.
		// .ur_y_cm.]'
		if (4 == get_cmd_values(_word, _nword, "box", 4, _dstack)) {
			x_ll_cm = _dstack[0];
			y_ll_cm = _dstack[1];
			x_ur_cm = _dstack[2];
			y_ur_cm = _dstack[3];
		} else {
			err("Cannot read the `box ...' part of command");
			return false;
		}
		dx = x_ur_cm - x_ll_cm;
		dy = y_ur_cm - y_ll_cm;
		break;
	default:
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	calculate_image_histogram();
	set_environment();
	// Draw the histogram, using 125 scaling of x axis
	gr_setxtransform(gr_axis_LINEAR);
	gr_setytransform(gr_axis_LOG);
	gr_setxsubdivisions(1);
	gr_setysubdivisions(1);
	gr_scale125((double) _image0, (double) _image255, 4, &left, &right, &num);
	gr_setxscale((double) x_ll_cm, (double) x_ll_cm + dx, left, right);
	gr_setyscale((double) y_ll_cm, (double) y_ll_cm + dy, yMin, yMax);
	string oldxname = _xname;
	string oldyname = _yname;
	_xname = "";
	_yname = "";
	gr_drawxaxis(yMin, left, (right - left) / num, right, gr_axis_BOTTOM);
	gr_drawyaxis((double) left, yMin, 1., yMax, gr_axis_LEFT);
	_xname = oldxname;
	_yname = oldyname;
	GriPath p;
	p.push_back(left, yMin, 'm');
	p.push_back(left, yMax, 'l');
	p.push_back(right, yMax, 'l');
	p.push_back(right, yMin, 'l');
	xval = _image0;
	dxval = (_image255 - _image0) / 255.0;
	p.push_back(_image0, yMin, 'm');
	for (i = 0; i < 256; i++) {
		p.push_back(xval, yMin + _imageHist[i], 'l');
		xval += dxval;
	}
	p.stroke(units_user);
	gr_setytransform(gr_axis_LINEAR);
	if ((y_ll_cm + dy) > _top_of_plot)
		_top_of_plot = y_ll_cm + dy;
	return true;
}

bool
draw_image_paletteCmd()
{
	const double height = 1.0;	// height of box (cm)
	const double space = 2.0;	// space of box above top of plot
	const int LEN = 512;	// length of tmp image
	int             rotpal=0;
	int             otherside=0;
	int             i;
	double          left_cm, bottom_cm, right_cm, top_cm;
	int             words_understood = 3;	// 3 in `draw image palette'
	unsigned char   gray[LEN];
	double          left, inc = 0.0, right, grayVal, grayInc;
	int             num;
	double          llx, lly, dx = XSIZE_DEFAULT, urx, ury;
	if (!get_var("..xmargin..", &llx))
		warning("Sorry, don't know value of ..xmargin.. so using XMARGIN_DEFAULT");
	if (!get_var("..xsize..", &dx))
		warning("Sorry, don't know value of ..xsize.. so using XSIZE_DEFAULT");
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	// Check for old usage `draw image grayscale'.  Give warning, but proceed
	// anyway.
	if (word_is(2, "grayscale") || word_is(2, "greyscale")) {
		warning("`draw image grayscale' RENAMED `draw image palette'; please change your cmdfile");
	}
	if (word_is(3, "axistop")) {
		words_understood++;
		rotpal=0;
		otherside=1;
	}
	if (word_is(3, "axisbottom")) {
		words_understood++;
		rotpal=0;
		otherside=0;
	}
	if (word_is(3, "axisleft")) {
		words_understood++;
		rotpal=1;
		otherside=0;
	}
	if (word_is(3, "axisright")) {
		words_understood++;
		rotpal=1;
		otherside=1;
	}
	if (1 == get_cmd_values(_word, _nword, "left", 1, _dstack)) {
		// Getting light/dark levels from command.
		left = _dstack[0];
		if (1 == get_cmd_values(_word, _nword, "right", 1, _dstack)) {
			right = _dstack[0];
			words_understood += 4;
		} else {
			demonstrate_command_usage();
			err("Missing `right' keyword");
			return false;
		}
	} else {
		// Getting light/dark levels from image.
		if (!image_range_exists()) {
			err("Image range unknown.  First `convert grid to image' or `set image range'");
			return false;
		}
		gr_scale125((double) _image0, (double) _image255, 4, &left, &right, &num);
		inc = (right - left) / num;
	}
	if (1 == get_cmd_values(_word, _nword, "increment", 1, _dstack)) {
		inc = _dstack[0];
		words_understood += 2;
	} else {
		if (inc == 0.0) {
			// Make guess, probably an ugly one
			inc = 0.2 * (right - left);
		}
	}
	if (!well_ordered(left, right, inc))
		inc = -inc;
	if (4 == get_cmd_values(_word, _nword, "box", 4, _dstack)) {
		llx = _dstack[0];
		lly = _dstack[1];
		urx = _dstack[2];
		ury = _dstack[3];
		words_understood += 5;
	} else {
		llx = XMARGIN_DEFAULT;
		lly = _top_of_plot + space;
		urx = llx + dx;
		ury = lly + height;
	}
	// Check that command syntax was OK.
	if (words_understood != _nword) {
		NUMBER_WORDS_ERROR;
		demonstrate_command_usage();
		return false;
	}
	// Draw image, then box.
	set_environment();
	if (rotpal==0) {
		//REMOVED//gr_setxlabel("");
		//REMOVED-2000-5-2 gr_setxnumberformat(_xFmt.c_str());
		gr_setxsubdivisions(1);
		gr_setysubdivisions(1);
		gr_setxscale(llx, urx, left, right);
		gr_setyscale(lly, ury, 0.0, 1.0);
	} else {
		//REMOVED//gr_setylabel("");
		string oldxname = _xname;
		//REMOVED-2000-5-2 gr_setynumberformat(_yFmt.c_str());
		gr_setxsubdivisions(1);
		gr_setysubdivisions(1);
		gr_setxscale(llx, urx, 0.0, 1.0);
		gr_setyscale(lly, ury, left, right);
	}
	if (ury > _top_of_plot)
		_top_of_plot = ury;
	grayInc = (right - left) / (LEN - 1.0);
	grayVal = left;		// - 0.5 * grayInc
	for (i = 0; i < LEN; i++) {
		int             imval;
		imval = (int) floor(0.5 + (255.0 * (grayVal - _image0) / (_image255 - _image0)));
		if (imval < 0)
			imval = 0;
		else if (imval > 255)
			imval = 255;
		gray[i] = imval;
		grayVal += grayInc;
	}
	if (rotpal==0) {
		gr_usertocm(left, 0.0, &left_cm, &bottom_cm);
		gr_usertocm(right, 1.0, &right_cm, &top_cm);
	} else {
		gr_usertocm(0.0, left, &left_cm, &bottom_cm);
		gr_usertocm(1.0, right, &right_cm, &top_cm);
	}
	// Clip to this, because image overhangs
	extern FILE    *_grPS;
	fprintf(_grPS, "q n %% turn clipping on for image palette\n");
	fprintf(_grPS, "%f %f moveto\n", left_cm * PT_PER_CM, bottom_cm * PT_PER_CM);
	fprintf(_grPS, "%f %f lineto\n", right_cm * PT_PER_CM, bottom_cm * PT_PER_CM);
	fprintf(_grPS, "%f %f lineto\n", right_cm * PT_PER_CM, top_cm * PT_PER_CM);
	fprintf(_grPS, "%f %f lineto\n", left_cm * PT_PER_CM, top_cm * PT_PER_CM);
	fprintf(_grPS, "%f %f lineto\n", left_cm * PT_PER_CM, bottom_cm * PT_PER_CM);
	fprintf(_grPS, "closepath W\n");
	if (rotpal==0) {
		gr_drawimage(gray,
			     _imageTransform, 
			     _image_color_model,
			     NULL, 0.0, 0.0, 0.0,
			     LEN, 1,
			     left_cm, bottom_cm, right_cm, top_cm,
			     false);
	} else {
		gr_drawimage(gray,
			     _imageTransform, 
			     _image_color_model,
			     NULL, 0.0, 0.0, 0.0,
			     1, LEN,
			     left_cm, bottom_cm, right_cm, top_cm,
			     false);
	}
	fprintf(_grPS, "S Q %% turn clipping off for image palette\n");
	if (rotpal==0) {
		string oldxname = _xname;
		_xname = "";
		if (otherside==0)
			gr_drawxaxis(0.0, left, inc, right, gr_axis_BOTTOM);
		else
			gr_drawxaxis(1.0, left, inc, right, gr_axis_TOP);
		_xname = oldxname;
		GriPath p(4);
		p.push_back(left, 0.0, 'm');
		p.push_back(left, 1.0, 'l');
		p.push_back(right, 1.0, 'l');
		p.push_back(right, 0.0, 'l');
		p.stroke(units_user);
	} else {
		string oldyname = _yname;
		_yname = "";
		if (otherside==0)
			gr_drawyaxis(0.0, left, inc, right, gr_axis_LEFT);
		else
			gr_drawyaxis(1.0, left, inc, right, gr_axis_RIGHT);
		_yname = oldyname;
		GriPath p(4);
		p.push_back(0.0, left, 'm');
		p.push_back(1.0, left, 'l');
		p.push_back(1.0, right, 'l');
		p.push_back(0.0, right, 'l');
		p.stroke(units_user);
	}
	return true;
}
#endif

// TODO - handle missingcolor in color images
bool
draw_imageCmd()
{
	double          llx_cm, lly_cm, urx_cm, ury_cm;
	if (!image_scales_defined()) {
		err("First define box containing image (`set x grid' and `set y grid')");
		return false;
	}
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (!image_exists()) {
		err("Must `read image' or `convert grid to image' before `draw image'.");
		return false;
	}
	if (!_imageTransform_exists) {
		err("Must `set image colorscale' before `draw image'");
		return false;
	}
	set_environment();
	gr_usertocm(_image_llx, _image_lly, &llx_cm, &lly_cm);
	gr_usertocm(_image_urx, _image_ury, &urx_cm, &ury_cm);
	if (imageMask_exists()) {
		extern double   _image_missing_color_red; // in set.c
		extern double   _image_missing_color_green; // in set.c
		extern double   _image_missing_color_blue; // in set.c
		double          mask_r, mask_g, mask_b;
		mask_r = _image_missing_color_red;
		mask_g = _image_missing_color_green;
		mask_b = _image_missing_color_blue;
		gr_drawimage(_image.image, _imageTransform, _image_color_model,
			     _imageMask.image, mask_r, mask_g, mask_b,
			     _image.ras_width, _image.ras_height,
			     llx_cm, lly_cm, urx_cm, ury_cm,
			     true);
	} else {
		gr_drawimage(_image.image, _imageTransform, _image_color_model,
			     NULL, 0.0, 0.0, 0.0,
			     _image.ras_width, _image.ras_height,
			     llx_cm, lly_cm, urx_cm, ury_cm,
			     true);
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}

//($$$;$$$$){# $label [centered|rightjustified|whiteunder] at $x $y [cm] [rotated $deg]
bool
draw_labelCmd()
{
	//printf("\n\n*** about to initialize the string with '%s'.\n",cmd_parameter_s(0));
	GriString label;
	label.assign(cmd_parameter_s(0));
	//printf("draw_labelCmd() thinks string is '%s' ... or '%s' (in gristring's view)\n", cmd_parameter_s(0), label.c_str());

	typedef enum where {leftjustified, centered, rightjustified} WHERE;
	bool whiteunder = false;
	WHERE where = leftjustified;
	unsigned int skip = 0;

	if (cmd_parameter_is_n(1) == false) {
		if (strEQ(cmd_parameter_s(1), "centered") || strEQ(cmd_parameter_s(1), "centred")) {
			where = centered;
			skip++;
		} else if (strEQ(cmd_parameter_s(1), "rightjustified")) {
			where = rightjustified;
			skip++;
		} else if (strEQ(cmd_parameter_s(1), "whiteunder")) {
			whiteunder = true;
			skip++;
		} else if (strEQ(cmd_parameter_s(1), "at")) {
			;			// okay
		}
	}

	if (cmd_parameter_is_n(1 + skip)) {
		err("`draw label' cmd is missing word the word `at' (numerical expression \\",
		    dtoa(cmd_parameter_n(1 + skip)), " seen instead)", "\\");
		return false;
	}
	if (strNE(cmd_parameter_s(1 + skip), "at")) {
		err("`draw label' cmd should have the word `at' where the word `\\",
		    cmd_parameter_s(1 + skip), "' was seen", "\\");
		return false;
	}

	bool in_cm = false;
	if (cmd_parameters() > unsigned(4 + skip)) {
		if (strEQ(cmd_parameter_s(4 + skip), "cm")) {
			in_cm = true;
		} else if (strNE(cmd_parameter_s(4 + skip), "rotated")) {
			err("`draw label' expecting word `cm' or `rotated', not `\\",
			    cmd_parameter_s(4 + skip), "' as found", "\\");
			return false;
		}
	}

	if (!in_cm && !scales_defined()) {
		no_scales_error();
		return false;
	}


	double x0 = cmd_parameter_n(2 + skip);
	double y0 = cmd_parameter_n(3 + skip);


	bool rotated = false;
	double angle = 0.0;
	if (cmd_parameters() > unsigned(4 + skip + (in_cm?1:0))) {
		if (strEQ(cmd_parameter_s(4 + skip + (in_cm?1:0)), "rotated")) {
			rotated = true;
			if (cmd_parameters() < unsigned(5 + skip + (in_cm?1:0) + 1)) {
				err("`draw label ... rotated' needs a specification of the angle (in deg)");
				return false;
			}
			angle = cmd_parameter_n(4 + skip + (in_cm?1:0) + 1);
		} else {
			err("`draw label' expecting word `cm' or `rotated', not `\\", 
			    cmd_parameter_s(4 + skip + (in_cm?1:0))
				);
			return false;
		}
	}
	if (cmd_parameters() != unsigned(4 + skip + (in_cm?1:0) + (rotated?2:0))) {
		err("`draw label' found unexpected items at end of command\n");
		return false;
	}


	gr_setfontsize_pt(get_var_scalar("$_font_size"));
	gr_setfont(gr_currentfont());
	double xcm, ycm;
	if (in_cm) {
		xcm = x0;
		ycm = y0;
	} else {
		if (using_proj()) {
			double xgeom, ygeom;
			proj_lonlat_to_xy(&x0, &y0, 1, &xgeom, &ygeom);
			x0 = xgeom;
			y0 = ygeom;
		}
		set_ps_color('t');
		set_environment();
		gr_usertocm(x0, y0, &xcm, &ycm);
	}

	// Draw axes if in user-coordinates and no axes drawn yet
	if (!in_cm)
		draw_axes_if_needed();

	if (whiteunder) {
		double width_cm, ascent_cm, descent_cm;
		gr_stringwidth(label.c_str(), &width_cm, &ascent_cm, &descent_cm);
		static GriPath p(5);	// static since might be done often
		p.clear();
		double space = 0.1;	// a little space around text
		p.push_back(xcm            - space, ycm             - space, 'm');
		p.push_back(xcm + width_cm + space, ycm             - space, 'l');
		p.push_back(xcm + width_cm + space, ycm + ascent_cm + space, 'l');
		p.push_back(xcm            - space, ycm + ascent_cm + space, 'l');
		p.push_back(xcm            - space, ycm             - space, 'l');
		double r, g, b;
		_griState.color_line().getRGB(&r, &g, &b);
		GriColor c;
		c.setRGB(1.0, 1.0, 1.0);
		_griState.set_color_line(c);
		set_environment();
		set_line_width_curve();
		p.fill(units_cm);
		c.setRGB(r, g, b);
		label.draw(xcm, ycm, TEXT_LJUST, 0.0);
	} else {
		set_environment();
		set_ps_color('t');
		//printf("draw_labelCmd thinks string is '%s'\n", label.c_str());
		switch (where) {
		case leftjustified:
			label.draw(xcm, ycm, TEXT_LJUST, angle);
			break;
		case centered:
			label.draw(xcm, ycm, TEXT_CENTERED, angle);
			break;
		default:
			label.draw(xcm, ycm, TEXT_RJUST, angle);
			break;
		}
	}
	_drawingstarted = true;
	return true;
}

// cmd draw line from ($$$$$;$) # $x0 $y0 "to" $x1 $y1 ["cm"]
bool
draw_line_fromCmd()
{
	bool cm_units = false;
	if (cmd_parameters() == 6) {
		if (strEQ(cmd_parameter_s(5), "cm"))
			cm_units = true;
		else {
			err("10th word of `draw line from ...' may only be `cm', if specified, not `\\", cmd_parameter_s(5), "' as given", "\\");
			return false;
		}
	} 
	if (strNE(cmd_parameter_s(2), "to")) {
		err("10th word of `draw line from ...' must be `to', not `\\", cmd_parameter_s(2), "' as given", "\\");
	}
	double x0 = cmd_parameter_n(0);
	double y0 = cmd_parameter_n(1);
	double x1 = cmd_parameter_n(3);
	double y1 = cmd_parameter_n(4);
	// Check if zero-length line
	if (((x1 - x0)*(x1 - x0) + (y1 - y0) * (y1 - y0)) == 0.0)
		return true;

	set_x_scale();
	set_y_scale();
	set_environment();
	set_ps_color('p');
	set_line_width_curve();
	static GriPath p(2);	// static to speed up, since often repeated
	p.clear();
	// User units -- draw axes if needed
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (!gr_missingx(x0) && !gr_missingx(x1) && !gr_missingy(y0) && !gr_missingy(y1)) {
		if (using_proj() && !cm_units) {
			double xx[2]; xx[0] = x0; xx[1] = x1;
			double yy[2]; yy[0] = y0; yy[1] = y1;
			double xxgeom[2], yygeom[2];
			proj_lonlat_to_xy(xx, yy, 2, xxgeom, yygeom);
			p.push_back(xxgeom[0], yygeom[0], 'm');
			p.push_back(xxgeom[1], yygeom[1], 'l');
			p.stroke(units_user);
		} else {
			p.push_back(x0, y0, 'm');
			p.push_back(x1, y1, 'l');
			if (cm_units) {
				p.stroke(units_cm);
				//printf("stroking, cm %f %f %f %f\n", x0, y0, x1, y1);
			} else {
				//printf("stroking, user %f %f %f %f\n", x0, y0, x1, y1);
				p.stroke(units_user);
			}
		}
		_drawingstarted = true;
		if (!cm_units)
			draw_axes_if_needed();
	}
	return true;
}

#if 0
// `draw lines {vertically .left. .right. .inc.}|{horizontally .bottom. .top.
// .inc.}'
bool
draw_linesCmd(void)
{
	bool            vert = false;
	double          min, max, inc, tmp;
	static GriPath  p(2);
	p.clear();
	switch (_nword) {
	case 6:
		if (!strcmp(_word[2], "vertically"))
			vert = true;
		else if (!strcmp(_word[2], "horizontally"))
			vert = false;
		else {
			err("`\\", _word[2], "' not understood.", "\\");
			demonstrate_command_usage();
			return false;
		}
		set_environment();
		set_line_width_curve();
		if (!getdnum(_word[3], &min)
		    || !getdnum(_word[4], &max)
		    || !getdnum(_word[5], &inc)) {
			return false;
		}
		if (!well_ordered(min, max, inc))
			inc = -inc;
		for (tmp = min; tmp <= (max + 0.25 * inc); tmp += inc) {
			if (vert == true) {
				p.push_back(tmp, _ybottom, 'm');
				p.push_back(tmp, _ytop, 'l');
			} else {
				p.push_back(_xleft, tmp, 'm');
				p.push_back(_xright, tmp, 'l');
			}
		}
		p.stroke(units_user);
		_drawingstarted = true;
		return true;
	default:
		NUMBER_WORDS_ERROR;
		demonstrate_command_usage();
		return false;
	}
}

// `draw patches $width $height [cm]' 
bool
draw_patchesCmd()
{
	double          *xp = _colX.begin();
	double          *yp = _colY.begin();
	double          *zp = _colZ.begin();
	double          dx, dx2, dy, dy2;
	GriColor old_color = _griState.color_line();
	bool            cmUnits;
	int             i;
	if (_nword < 4 || _nword > 5) {
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	if (!_columns_exist || _colZ.size() <= 0) {
		warning("`draw patches' noticed that no column data exist.");
		return true;
	}
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	if (!_imageTransform_exists) {
		err("Image transform doesn't exist yet.  First `set image grayscale'");
		return false;
	}
	// Figure out units for patch size
	cmUnits = (!strcmp(_word[_nword - 1], "cm")) ? true : false;
	// Figure out patch size
	if (!getdnum(_word[2], &dx) || !getdnum(_word[3], &dy)) {
		demonstrate_command_usage();
		if (cmUnits) {
			READ_WORD_ERROR(".x_cm. and .y_cm.");
		} else {
			READ_WORD_ERROR(".x. and .y.");
		}
		return false;
	}
	if (dx <= 0. || dy <= 0.0) {
		demonstrate_command_usage();
		if (cmUnits)
			err("Can't have .dx_cm. <= 0 or .dy_cm. <= 0");
		else
			err("Can't have .dx. <= 0 or .dy. <= 0");
		return false;
	}
	dx2 = dx / 2.0;
	dy2 = dy / 2.0;
	set_environment();
	int i_max = _colX.size();
	static GriPath p(5);
	GriColor gray_level;
	for (i = 0; i < i_max; i++) {
		p.clear();
		double          xl, yb, xr, yt;
		if (!gr_missingx((double) *xp)
		    && !gr_missingy((double) *yp)
		    && !gr_missing((double) *zp)) {
			// Set grayscale
			gray_level.setRGB(_imageTransform[value_to_image(*zp)] / 255.0,
					  _imageTransform[value_to_image(*zp)] / 255.0,
					  _imageTransform[value_to_image(*zp)] / 255.0);
			_griState.set_color_line(gray_level);
			if (cmUnits) {
				gr_usertocm((double) *xp, (double) *yp, &xl, &yb);
				xl -= dx2;
				xr = xl + dx;
				yb -= dy2;
				yt = yb + dy;
				p.push_back(xl, yb, 'm');
				p.push_back(xr, yb, 'l');
				p.push_back(xr, yt, 'l');
				p.push_back(xl, yt, 'l');
				p.push_back(xl, yb, 'l');
				p.fill(units_cm);
			} else {
				xl = *xp;
				yb = *yp;
				xl -= dx2;
				xr = xl + dx;
				yb -= dy2;
				yt = yb + dy;
				p.push_back(xl, yb, 'm');
				p.push_back(xr, yb, 'l');
				p.push_back(xr, yt, 'l');
				p.push_back(xl, yt, 'l');
				p.push_back(xl, yb, 'l');
				p.fill(units_user);
			}
		}
		xp++;
		yp++;
		zp++;
	}
	_griState.set_color_line(old_color);
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
}
#endif

// cmd draw polygon (@) { # [filled] <x-y pairs> [cm]
bool
draw_polygonCmd(void)
{
	unsigned int num_params = cmd_parameters();
	if (num_params < 2) {
		err("`draw polygon' needs x-y pairs");
		return false;
	}
	//printf("draw_polygonCmd: num_params=%d\n", num_params);
	bool            filled;
	unsigned int start = 0;
	if (cmd_parameter_is_n(0))
		filled = false;
	else {
		if (strEQ(cmd_parameter_s(0), "filled")) {
			filled = true;
			start = 1;
			num_params--;
			//printf("debug: draw polygon filled. num_params is now %d\n", num_params);
		} else {
			err("3rd word of `draw polygon ...' may only be `filled', if specified, not `\\", cmd_parameter_s(0), "' as given", "\\");
			return false;
		}
	}
	bool in_user_units;
	if (cmd_parameter_is_n(num_params + start - 1)) {
		in_user_units = true;
	} else {
		if (strEQ(cmd_parameter_s(num_params + start - 1), "cm")) {
			in_user_units = false;
			num_params--;
			//printf("debug: draw polygon is in cm. num_params is now %d\n", num_params);
		} else {
			err("last word of `draw polygon ...' may only be `cm', if specified, not `\\", cmd_parameter_s(num_params + start - 1), "' as given", "\\");
			return false;
		}
	}
    
	//printf("draw_polygonCmd: num_params now %d\n", num_params);
	if (2 * (num_params / 2) != num_params) {
		err("Need 1 or more matched (x,y) pairs");
		return false;
	}
	set_x_scale();
	set_y_scale();
	set_environment();
	static GriPath p;
	p.clear();
	for (unsigned int i = 0; i < num_params; i += 2) {
		double x = cmd_parameter_n(    i + start);
		double y = cmd_parameter_n(1 + i + start);
		//printf("draw polygon on %f %f\n", x, y);
		p.push_back(x, y, i == 0 ? 'm' : 'l');
	}
	if (in_user_units) {
		if (filled)
			p.fill(units_user);
		else
			p.stroke(units_user);
		draw_axes_if_needed();
	} else {
		if (filled)
			p.fill(units_cm);
		else
			p.stroke(units_cm);
	}
	_drawingstarted = true;
	return true;
}


bool
draw_titleCmd()
{
	const char *title = cmd_parameter_s(0);
	double xmargin = get_var_scalar("$_xmargin");
	double xsize = get_var_scalar("$_xsize");
	set_environment();
	GriString label;
	label.assign(title);
	_top_of_plot += 1.5 * gr_currentfontsize_pt() / PT_PER_CM;
	label.draw(xmargin + 0.5 * xsize,
		   _top_of_plot,
		   TEXT_CENTERED,
		   0.0);
	return true;
}

#if 0
// draw values [.dx. .dy.] [\format] [separation .dist_cm.]
bool
draw_valuesCmd()
{
	if (!_columns_exist || _colZ.size() <= 0) {
		err("First `read columns'");
		return false;
	}
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	// Get separation x and y if they are given
	int gave_separation = 0;
	double xseparation = 0.0, yseparation = 0.0;
	if (2 == get_cmd_values(_word, _nword, "separation", 2, _dstack)) {
		xseparation = _dstack[0];
		yseparation = _dstack[1];
		if (xseparation < 0.0 || yseparation < 0.0) {
			warning("`draw values ... separation .xcm. .ycm.' ignoring negative values");
			xseparation = 0.0;
			yseparation = 0.0;
		}
		gave_separation = 1;
	}
	// Get format if it is given
	int             gave_fmt = 0;
	char            fmt[20];
	strcpy(fmt, "%g");
	int word;
	for (word = 2; word < _nword; word++) {
		if ('%' == *_word[word]) {
			strcpy(fmt, _word[word]);
			gave_fmt = 1;
			break;
		}
	}
	set_environment();
	double          dx_cm, dy_cm;
	if (_nword == 4 + gave_fmt + 3 * gave_separation) {
		if (!getdnum(_word[2], &dx_cm) || !getdnum(_word[3], &dy_cm)) {
			err("`draw values' can't read dx_cm and dy_cm");
			return false;
		}
	} else if (_nword == 2 + gave_fmt + 3 * gave_separation) {
		double          width_cm, ascent_cm, descent_cm;
		gr_stringwidth("M", &width_cm, &ascent_cm, &descent_cm);
		dx_cm = 0.5 * width_cm;
		dy_cm = -0.5 * ascent_cm;
	} else {
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	// Now start drawing
	double *xp = _colX.begin();
	double *yp = _colY.begin();
	double *zp = _colZ.begin();
	int num = _colX.size();
	double          xcm, ycm;
	double          xcm_last = 0.0, ycm_last = 0.0; // assignment calms compiler
	bool first = true;
	GriString label;
	int i;
	for (i = 0; i < num; i++) {
		if (!gr_missing(*zp)
		    && !gr_missingx(*xp)
		    && !gr_missingy(*yp)) {
			gr_usertocm(*xp, *yp, &xcm, &ycm);
			xcm += dx_cm;
			ycm += dy_cm;
			if (first || fabs(xcm - xcm_last) >= xseparation || fabs(ycm - ycm_last) >= yseparation) {
				sprintf(_grTempString, fmt, *zp);
				label.assign(_grTempString);
				label.draw(xcm, ycm, TEXT_LJUST, 0.0);
				xcm_last = xcm;
				ycm_last = ycm;
				first = false;
			}
		}
		xp++;
		yp++;
		zp++;
	}
	if (!first) {
		_drawingstarted = true;
		draw_axes_if_needed();
	}
	return true;
}
#endif

//OLD cmd draw x axis ($;$$) { # [at bottom|top|{$y [cm]} [lower|upper]]
//NEW cmd draw x axis ($;$$) { # [at bottom|top|{$y [cm]} [labelled at bottom|top]
bool
draw_x_axisCmd()
{
	double          where = _ybottom;
	gr_axis_properties side = gr_axis_BOTTOM;
	bool            position_in_cm = false;
	set_x_scale();
	set_y_scale();
	unsigned int num_params = cmd_parameters();	// NOTE: may alter value later
	if (num_params == 0)
		return draw_axes(3, _ybottom, gr_axis_BOTTOM, true);
	if (strNE(cmd_parameter_s(0), "at")) {
		err("4th word of `draw x axis' may only be `at', if specified, not `\\", cmd_parameter_s(0), "' as given", "\\");
		return false;
	}
	if (num_params == 1) {
		err("`draw x axis at' requires parameters specifying location");
		return false;
	}

	// see if lower|upper specified 
	bool specified_upper_or_lower = false;
	if (cmd_parameter_is_n(num_params - 1) == false) {
		if (strEQ(cmd_parameter_s(num_params - 1), "lower")) {
			side = gr_axis_BOTTOM;
			num_params--;
			specified_upper_or_lower = true;
		} else if (strEQ(cmd_parameter_s(num_params - 1), "upper")) {
			side = gr_axis_TOP;
			num_params--;
			specified_upper_or_lower = true;
		}
	}
	if (cmd_parameter_is_n(num_params - 1) == false) {    
		if (strEQ(cmd_parameter_s(num_params - 1), "cm")) {
			position_in_cm = true;
			num_params--;
		} else if (strNE(cmd_parameter_s(num_params - 1), "top")
			   && strNE(cmd_parameter_s(num_params - 1), "bottom")) {
			err("`draw x axis at' expecting word `top', `bottom', or `cm', but saw word `\\", cmd_parameter_s(num_params - 1), "' instead", "\\");
			return false;
		}
	}

	if (num_params != 2) {
		err("`draw x axis at ...' has wrong number of parameters");
		return false;
	}
    
	// bottom|top
	if (cmd_parameter_is_n(1) == false) {
		if (strEQ(cmd_parameter_s(1), "bottom")) {
			if (specified_upper_or_lower)
				return draw_axes(3, _ybottom, side, false);
			else
				return draw_axes(3, _ybottom, gr_axis_BOTTOM, false);
		} 
		if (strEQ(cmd_parameter_s(1), "top")) {
			if (specified_upper_or_lower)
				return draw_axes(3, _ytop, side, false);
			else
				return draw_axes(3, _ytop, gr_axis_TOP, false);
		}
		err("`draw x axis at' requires position to be numeric, or `top' or `bottom', not `\\", cmd_parameter_s(1), "' as given", "\\");
		return false;
	}
	where = cmd_parameter_n(1);
	if (position_in_cm) {
		double          xuser, yuser;
		gr_cmtouser((double) 1.0, where, &xuser, &yuser);
		where = yuser;
	}
	draw_axes(3, where, side, false);
	return true;
}

//cmd draw y axis (;$$$$) { # [at left|right|{$x [cm]} [left|right]]
bool
draw_y_axisCmd()
{
	double          where = _xleft;
	gr_axis_properties side = gr_axis_LEFT;
	bool            position_in_cm = false;
	set_x_scale();
	set_y_scale();
	unsigned int num_params = cmd_parameters();	// NOTE: may alter value later
	if (num_params == 0)
		return draw_axes(4, _ybottom, gr_axis_BOTTOM, true);
	if (strNE(cmd_parameter_s(0), "at")) {
		err("4th word of `draw y axis' may only be `at', if specified, not `\\", cmd_parameter_s(0), "' as given", "\\");
		return false;
	}
	if (num_params == 1) {
		err("`draw y axis at' requires parameters specifying location");
		return false;
	}

	// see if left|right specified 
	bool specified_left_or_right = false;
	if (num_params > 2 && cmd_parameter_is_n(num_params - 1) == false) {
		if (strEQ(cmd_parameter_s(num_params - 1), "left")) {
			side = gr_axis_LEFT;
			num_params--;
			specified_left_or_right = true;
		} else if (strEQ(cmd_parameter_s(num_params - 1), "right")) {
			side = gr_axis_RIGHT;
			num_params--;
			specified_left_or_right = true;
		}
	}
	if (cmd_parameter_is_n(num_params - 1) == false) {    
		if (strEQ(cmd_parameter_s(num_params - 1), "cm")) {
			position_in_cm = true;
			num_params--;
		} else if (strNE(cmd_parameter_s(num_params - 1), "left")
			   && strNE(cmd_parameter_s(num_params - 1), "right")) {
			err("`draw y axis at' expecting word `left', `right', or `cm', but saw word `\\", cmd_parameter_s(num_params - 1), "' instead", "\\");
			return false;
		}
	}

	if (num_params != 2) {
		err("`draw y axis at ...' has wrong number of parameters");
		return false;
	}
    
	// left|right
	if (cmd_parameter_is_n(1) == false) {
		if (strEQ(cmd_parameter_s(1), "left")) {
			if (specified_left_or_right)
				return draw_axes(4, _xleft, side, false);
			else
				return draw_axes(4, _xleft, gr_axis_LEFT, false);
		} 
		if (strEQ(cmd_parameter_s(1), "right")) {
			if (specified_left_or_right)
				return draw_axes(4, _xright, side, false);
			else
				return draw_axes(4, _xright, gr_axis_RIGHT, false);
		}
		err("`draw y axis at' requires position to be numeric, or `left' or `right', not `\\", cmd_parameter_s(1), "' as given", "\\");
		return false;
	}
	where = cmd_parameter_n(1);
	if (position_in_cm) {
		double          xuser, yuser;
		gr_cmtouser(where, 1.0, &xuser, &yuser);
		where = xuser;
	}
	return draw_axes(4, where, side, false);
}

// cmd draw x box plot at ($;$) {# $y [size $cm]
bool
draw_x_box_plotCmd()
{
	const double FRAC = 0.3;	// size of symbols compared to box 
	double          q1_cm, q2_cm, q3_cm; // quartiles in cm 
	double          y_cm, ymin_cm, ymax_cm; // box boundaries in cm 
	double          old_symbol_size = gr_currentsymbolsize_pt(); // store old 
	double          size_cm = 0.5;                    // box size 
	double          y;		                      // location 
	double          q1, q2, q3;			      // quartiles 
	double          upper_adjacent_value;	      // whisker>q3 
	double          lower_adjacent_value;	      // whisker<q1 
	double          iqr;			      // interquartile range 
	double          upper, lower;		      // temporary boundaries 
	double          xcm, ycm;

	if (!scales_defined()) {
		no_scales_error();
		return false;
	}

	unsigned int num_params = cmd_parameters();
	y = cmd_parameter_n(0);
	// Check command syntax
	if (num_params == 3) {
		if (cmd_parameter_is_n(1) != false) {
			err("`draw x box plot size $cm' expecting word `size' but saw an expression instead");
			return false;
		}
		if (strNE(cmd_parameter_s(1), "size")) {
			err("`draw x box plot size $cm' expecting word `size' but saw word `\\", cmd_parameter_s(1), "' instead", "\\");
			return false;
		}
		size_cm = cmd_parameter_n(2);
	}
	set_environment();
	// figure statistics
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		fatal_error("X column ($_x) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw x box plot' noticed that no X data exist");
	std::vector<double> values(xlen);
	for (int i = 0; i < xlen; i++)
		values[i] = xvar->get_at_index(i).get_numeric();
	histogram_stats(values, xlen, &q1, &q2, &q3);
	iqr = q3 - q1;
	// draw box
	gr_usertocm(q1, y, &q1_cm, &y_cm);
	gr_usertocm(q2, y, &q2_cm, &y_cm);
	gr_usertocm(q3, y, &q3_cm, &y_cm);
	ymin_cm = y_cm + size_cm / 2.0;
	ymax_cm = y_cm - size_cm / 2.0;
	static GriPath p(7);
	p.clear();
	p.push_back(q1_cm, ymax_cm, 'm');
	p.push_back(q3_cm, ymax_cm, 'l');
	p.push_back(q3_cm, ymin_cm, 'l');
	p.push_back(q1_cm, ymin_cm, 'l');
	p.push_back(q1_cm, ymax_cm, 'l');
	p.push_back(q2_cm, ymax_cm, 'm');
	p.push_back(q2_cm, ymin_cm, 'l');
	set_line_width_symbol();
	p.stroke(units_cm);
	// draw whiskers
	upper_adjacent_value = q3 - 1;	// will be max between q3 and upper 
	lower_adjacent_value = q1 + 1;	// will be min between q3 and upper 
	upper = q3 + 1.5 * iqr;	// upper fence 
	lower = q1 - 1.5 * iqr;	// lower fence 
	for (int i = 0; i < xlen; i++) {
		double           x = values[i];
		if (x > q3 && x < upper && x > upper_adjacent_value)
			upper_adjacent_value = x;
		if (x < q1 && x > lower && x < lower_adjacent_value)
			lower_adjacent_value = x;
	}
	set_line_width_symbol();
	gr_setsymbolsize_cm(FRAC * size_cm);
	if (upper_adjacent_value > q3) {	// draw whisker > q3 
		gr_usertocm(q3, y, &xcm, &ycm);
		p.push_back(xcm, ycm, 'm');
		gr_usertocm(upper_adjacent_value, y, &xcm, &ycm);
		p.push_back(xcm, ycm, 'l');
		p.stroke(units_cm);
		gr_drawsymbol(xcm, ycm, gr_times_symbol);
	}
	if (lower_adjacent_value < q1) {	// draw whisker < q1 
		gr_usertocm(q1, y, &xcm, &ycm);
		p.push_back(xcm, ycm, 'm');
		gr_usertocm(lower_adjacent_value, y, &xcm, &ycm);
		p.push_back(xcm, ycm, 'l');
		p.stroke(units_cm);
		gr_drawsymbol(xcm, ycm, gr_times_symbol);
	}
	// draw outliers > q3
	lower = q3 + 1.5 * iqr;
	upper = q3 + 3.0 * iqr;
	for (int i = 0; i < xlen; i++) {
		double           val = values[i];
		if (val > lower) {
			gr_usertocm(val, y, &xcm, &ycm);
			if (val <= upper)	// minor outlier 
				gr_drawsymbol(xcm, ycm, gr_circ_symbol);
			else		// major outlier 
				gr_drawsymbol(xcm, ycm, gr_bullet_symbol);
		}
	}
	// draw outliers < q1
	lower = q1 - 3.0 * iqr;
	upper = q1 - 1.5 * iqr;
	for (int i = 0; i < xlen; i++) {
		double           val = values[i];
		if (val < upper) {
			gr_usertocm(val, y, &xcm, &ycm);
			if (val >= lower)	// minor outlier 
				gr_drawsymbol(xcm, ycm, gr_circ_symbol);
			else		// major outlier 
				gr_drawsymbol(xcm, ycm, gr_bullet_symbol);
		}
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	gr_setsymbolsize_pt(old_symbol_size);
	return true;
}

// cmd draw y box plot at ($;$) {# $x [size $cm]
bool
draw_y_box_plotCmd()
{
	const double FRAC = 0.3;	// size of symbols compared to box
	double          q1_cm, q2_cm, q3_cm; // quartiles in cm 
	double          x_cm, xmin_cm, xmax_cm; // box boundaries in cm 
	double          old_symbol_size = gr_currentsymbolsize_pt(); // store old 
	double          size_cm = 0.5;                 // box size 
	double          x;				   // location 
	double          q1, q2, q3;			   // quartiles 
	double          upper_adjacent_value;	   // whisker>q3 
	double          lower_adjacent_value;	   // whisker<q1 
	double          iqr;			   // interquartile range 
	double          upper, lower;		   // temporary boundaries 
	double          xcm, ycm;

	if (!scales_defined()) {
		no_scales_error();
		return false;
	}

	unsigned int num_params = cmd_parameters();
	x = cmd_parameter_n(0);
	// Check command syntax
	if (num_params == 3) {
		if (cmd_parameter_is_n(1) != false) {
			err("`draw y box plot size $cm' expecting word `size' but saw an expression instead");
			return false;
		}
		if (strNE(cmd_parameter_s(1), "size")) {
			err("`draw y box plot size $cm' expecting word `size' but saw word `\\", cmd_parameter_s(1), "' instead", "\\");
			return false;
		}
		size_cm = cmd_parameter_n(2);
	}

	set_environment();
	// Figure statistics.
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* yvar = &vector_vars[yind];
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw y box plot' noticed that no Y data exist");
	std::vector<double> values(ylen);
	for (int i = 0; i < ylen; i++)
		values[i] = yvar->get_at_index(i).get_numeric();

	histogram_stats(values, ylen, &q1, &q2, &q3);
	iqr = q3 - q1;
	// Draw box.
	gr_usertocm(x, q1, &x_cm, &q1_cm);
	gr_usertocm(x, q2, &x_cm, &q2_cm);
	gr_usertocm(x, q3, &x_cm, &q3_cm);
	xmin_cm = x_cm + size_cm / 2.0;
	xmax_cm = x_cm - size_cm / 2.0;
	static GriPath p(7);
	p.clear();
	p.push_back(xmin_cm, q1_cm, 'm');
	p.push_back(xmax_cm, q1_cm, 'l');
	p.push_back(xmax_cm, q3_cm, 'l');
	p.push_back(xmin_cm, q3_cm, 'l');
	p.push_back(xmin_cm, q1_cm, 'l');
	p.push_back(xmin_cm, q2_cm, 'm');
	p.push_back(xmax_cm, q2_cm, 'l');
	set_line_width_symbol();
	p.stroke(units_cm);
	// Draw whiskers.
	upper_adjacent_value = q3 - 1; // will be max between q3 and upper 
	lower_adjacent_value = q1 + 1; // will be min between q3 and upper 
	upper = q3 + 1.5 * iqr;	   // upper fence 
	lower = q1 - 1.5 * iqr;	   // lower fence 
	for (int i = 0; i < ylen; i++) {
		double           y = values[i];
		if (y > q3 && y < upper && y > upper_adjacent_value)
			upper_adjacent_value = y;
		if (y < q1 && y > lower && y < lower_adjacent_value)
			lower_adjacent_value = y;
	}
	gr_setsymbolsize_cm(FRAC * size_cm);
	set_line_width_symbol();
	if (upper_adjacent_value > q3) {	// draw whisker > q3 
		gr_usertocm(x, q3, &xcm, &ycm);
		p.push_back(xcm, ycm, 'm');
		gr_usertocm(x, upper_adjacent_value, &xcm, &ycm);
		p.push_back(xcm, ycm, 'l');
		p.stroke(units_cm);
		gr_drawsymbol(xcm, ycm, gr_times_symbol);
	}
	if (lower_adjacent_value < q1) {	// draw whisker < q1 
		gr_usertocm(x, q1, &xcm, &ycm);
		p.push_back(xcm, ycm, 'm');
		gr_usertocm(x, lower_adjacent_value, &xcm, &ycm);
		p.push_back(xcm, ycm, 'l');
		p.stroke(units_cm);
		gr_drawsymbol(xcm, ycm, gr_times_symbol);
	}
	// Draw outliers > q3
	lower = q3 + 1.5 * iqr;
	upper = q3 + 3.0 * iqr;
	gr_setsymbolsize_cm(FRAC * size_cm);
	for (int i = 0; i < ylen; i++) {
		double           val = values[i];
		if (val > lower) {
			gr_usertocm(x, val, &xcm, &ycm);
			if (val <= upper)	// minor outlier 
				gr_drawsymbol(xcm, ycm, gr_circ_symbol);
			else		// major outlier 
				gr_drawsymbol(xcm, ycm, gr_bullet_symbol);
		}
	}
	// Draw outliers < q1
	lower = q1 - 3.0 * iqr;
	upper = q1 - 1.5 * iqr;
	for (int i = 0; i < ylen; i++) {
		double           val = values[i];
		if (val < upper) {
			gr_usertocm(x, val, &xcm, &ycm);
			if (val >= lower)	// minor outlier 
				gr_drawsymbol(xcm, ycm, gr_circ_symbol);
			else		// major outlier 
				gr_drawsymbol(xcm, ycm, gr_bullet_symbol);
		}
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	gr_setsymbolsize_pt(old_symbol_size);
	return true;
}

#if 0
bool
draw_zero_lineCmd()
{
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	switch (_nword) {
	case 3:
		// `draw zero line' 
		draw_zeroline_horizontally();
		break;
	case 4:
		if (!strcmp(_word[3], "horizontally"))
			draw_zeroline_horizontally();
		else if (!strcmp(_word[3], "vertically"))
			draw_zeroline_vertically();
		else {
			err("How should I draw this zero line?");
			demonstrate_command_usage();
			return false;
		}
		break;
	default:
		NUMBER_WORDS_ERROR;
		demonstrate_command_usage();
		return false;
	}
	return true;
}

bool
draw_zeroline_horizontally()
{
	if ((_ybottom * _ytop) < 0.0) {
		set_environment();
		set_line_width_curve();
		GriPath p(2);
		p.push_back(_xleft,  0.0, 'm');
		p.push_back(_xright, 0.0, 'l');
		p.stroke(units_user);
		_drawingstarted = true;
		draw_axes_if_needed();
	}
	return true;
}

bool
draw_zeroline_vertically()
{
	if ((_xleft * _xright) < 0.0) {
		set_environment();
		set_line_width_curve();
		GriPath p(2);
		p.push_back(0.0, _ybottom, 'm');
		p.push_back(0.0, _ytop,    'l');
		p.stroke(units_user);
		_drawingstarted = true;
		draw_axes_if_needed();
	}
	return true;
}
#endif

bool
set_x_scale()
{
	double xmargin = get_var_scalar("$_xmargin");
	double xsize = get_var_scalar("$_xsize");
	gr_setxtransform(_xtype);
	gr_setxscale(xmargin, xmargin + xsize, _xleft, _xright);
	_xscale_exists = true;
	return true;
}

bool
set_y_scale()
{
	double ymargin = get_var_scalar("$_ymargin");
	double ysize = get_var_scalar("$_ysize");
	gr_setytransform(_ytype);
	gr_setyscale(ymargin, ymargin + ysize, _ybottom, _ytop);
	_yscale_exists = true;
	return true;
}

#if 0
//`Draw gri logo .x_cm. .y_cm. .height_cm. .style. \fgcolor \bgcolor'
//   0   1    2    3      4        5          6        7        8
bool
draw_gri_logoCmd()
{
	double dx, dy;
	warning("DEBUG: using .dx. and .dy. temporarily.  draw_gri_logoCmd()/draw.cc");
	if (!get_var(".dx.", &dx))
		err("DEBUG: cannot find .dx.");
	if (!get_var(".dy.", &dy))
		err("DEBUG: cannot find .dy.");
	printf("DEBUG: dx=%f dy=%f cm LATER HARD-WIRE IN.\n", dx, dy);
	//
	// Process args
	double x_cm, y_cm, height_cm;
	int style;
	if (_nword != 9) {
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	Require(getdnum(_word[3], &x_cm),      READ_WORD_ERROR(".x_cm."));
	Require(getdnum(_word[4], &y_cm),      READ_WORD_ERROR(".y_cm."));
	Require(getdnum(_word[5], &height_cm), READ_WORD_ERROR(".height_cm."));
	Require(getinum(_word[6], &style),     READ_WORD_ERROR(".style."));
	double fg_r, fg_g, fg_b;
	double bg_r, bg_g, bg_b;
	Require(look_up_color(_word[7], &fg_r, &fg_g, &fg_b), 
		err("unknown colorname `\\", _word[7], "'", "\\"));
	Require(look_up_color(_word[8], &bg_r, &bg_g, &bg_b), 
		err("unknown colorname `\\", _word[8], "'", "\\"));
	GriColor bg_color;
	bg_color.setRGB(bg_r, bg_g, bg_b);
	GriColor fg_color;
	fg_color.setRGB(fg_r, fg_g, fg_b);
	//
	// Read data
	char fname[256];
	sprintf(fname, "%s/%s", _lib_directory.c_str(), "logo.dat");
	FILE *fp = fopen(fname, "r");
	if (!fp) {
		warning("Could not open logo data file `\\", fname,
			"'.\n         Continuing with rest of commandfile.", "\\");
		return true;
	}
	GriPath p;
	double xx, yy;
	//
	// Save entry values
	double old_grMissingValue = gr_currentmissingvalue();
	GriColor old_color = _griState.color_line();

	gr_setmissingvalue(-999.0);
	bool first = true, last_OK = true;
	while (2 == fscanf(fp, "%lf %lf", &xx, &yy)) {
		if (!gr_missing(xx)) {
			xx = x_cm + xx * height_cm;
			yy = y_cm + yy * height_cm;
			if (first) {
				p.push_back(xx, yy, 'm');
				first = false;
			} else {
				if (last_OK) {
					p.push_back(xx, yy, 'l');
				} else {
					p.push_back(xx, yy, 'm');
				}
			}
			last_OK = true;
		} else {	    
			last_OK = false;
		}
	}
	fclose(fp);
	GriPath bg;
	double xmax = 1.81291;	// of data in logo
	double ymax = 1.0;		// of data in logo
	switch (style) {
	case 0:			// stroke it
		_griState.set_color_line(fg_color);
		p.stroke(units_cm);
		break;
	default:
	case 1:			// fill it with no background
		_griState.set_color_line(fg_color);
		p.fill(units_cm);
		break;
	case 2:			// fill it in tight box
		_griState.set_color_line(bg_color);
		bg.push_back(x_cm - height_cm * 0.05,        y_cm - height_cm * 0.05,        'm');
		bg.push_back(x_cm + xmax * height_cm * 1.05, y_cm - height_cm * 0.05,        'l');
		bg.push_back(x_cm + xmax * height_cm * 1.05, y_cm + ymax * height_cm * 1.05, 'l');
		bg.push_back(x_cm - height_cm * 0.05,        y_cm + ymax * height_cm * 1.05, 'l');
		bg.fill(units_cm);
		_griState.set_color_line(fg_color);
		p.fill(units_cm);
		break;
	case 3:			// fill it in square box
		_griState.set_color_line(bg_color);
		bg.push_back(x_cm - height_cm * 0.05,        y_cm - height_cm * 0.05,        'm');
		bg.push_back(x_cm + xmax * height_cm * 1.05, y_cm - height_cm * 0.05,        'l');
		bg.push_back(x_cm + xmax * height_cm * 1.05, y_cm + xmax * height_cm * 1.05, 'l');
		bg.push_back(x_cm - height_cm * 0.05,        y_cm + xmax * height_cm * 1.05, 'l');
		bg.fill(units_cm);
		_griState.set_color_line(fg_color);
		p.fill(units_cm);
		break;
	case 4:			// draw over offset underlay
		_griState.set_color_line(bg_color);
		p.translate(+height_cm * dx, -height_cm * dy);
		p.fill(units_cm);
		p.translate(-height_cm * dx, +height_cm * dy);
		_griState.set_color_line(fg_color);
		p.fill(units_cm);
		break;
	}
	//
	// Reset to entry values
	_griState.set_color_line(old_color);
	gr_setmissingvalue(old_grMissingValue);
	return true;
}
#endif

// ($;$$$) { # [unlabelled] $density [$P_sigma $P_theta]
bool
draw_isopycnalCmd()
{
	if (!scales_defined()) {
		no_scales_error();
		return false;
	}
	bool labelled = true;
	if (cmd_parameter_is_n(0) == false 
	    && strEQ(cmd_parameter_s(0), "unlabelled")) {
		labelled = false;
	}
	double density = 0.0, P_sigma = 0.0, P_theta = 0.0;
	unsigned int num_params = cmd_parameters();
	unsigned int skip = labelled ? 0 : 1;
	switch (num_params - skip) {
	case 1:
		density = cmd_parameter_n(0 + skip);
		break;
	case 2:
		density = cmd_parameter_n(0 + skip);
		P_sigma = cmd_parameter_n(1 + skip);
		break;
	case 3:
		density = cmd_parameter_n(0 + skip);
		P_sigma = cmd_parameter_n(1 + skip);
		P_theta = cmd_parameter_n(2 + skip);
		break;
	default:
		if (labelled)
			err("`draw isopycnal' given wrong number of parameters; want 1, 2 or 3, but got \\", itoa(num_params - skip), "\\");
		else
			err("`draw isopycnal unlabelled' given wrong number of parameters; want 1, 2 or 3, but got \\", itoa(num_params - skip), "\\");
	}
	if (density > 100.0)
		density -= 1000;	// make in sigma-ish unit
	double Tmin = _ybottom;
	double Tmax = _ytop;
	if (Tmax <= Tmin) {
		err("`draw isopycnal': y-axis must have temperature, increasing upwards on axis.");
		return false;
	}
	set_environment();		// for scales
	//printf("in 'draw_isopycnal' rho = %f\n", density);
	vector<double> SS;
	vector<double> TT;
	const double S_tolerance = 0.0001; // stop when this close
	double dT = (Tmax - Tmin) / 100.0;
	//printf("Tmin %f Tmax %f dt %f\n", Tmin, Tmax, dT);
	for (double T = Tmin; T <= Tmax + dT / 10.0; T += dT) {
		//printf("should calc for T=%f\n", T);
		double S = -999.0;	// where isopycnal hits
		double S1 = 0.;		// S1 and S2 should bracket
		double S2 = 1000.0;
		double dev1 = sw_density(S1, sw_potential_temperature(S1, T, P_theta, P_sigma), P_sigma)
			- 1000.0 - density;
		double dev2 = sw_density(S2, sw_potential_temperature(S2, T, P_theta, P_sigma), P_sigma)
			- 1000.0 - density;
		//printf("dev1 = %f    dev2 = %f\n",dev1,dev2);
		if (dev1 * dev2 > 0.0)
			continue;		// not bracketted at this salinity
		while (true) {
			//printf("search now on S= %.4f  S1= %.4f S2= %.4f\n", S,S1,S2);
			if (S2 - S1 < S_tolerance)
				break;
			S = (S1 + S2) / 2.0; // midpoint
			double dev = sw_density(S, sw_potential_temperature(S, T, P_theta, P_sigma), P_sigma)
				- 1000.0 - density;
			if (dev * dev1 < 0.0) {
				S2 = S;
				dev2 = dev;
			} else if (dev * dev2 < 0.0) {
				S1 = S;
				dev1 = dev;
			} else {
				break;		// dev=0, so must have hit it exactly
			}
		}
		//printf("Pushing back S,T @ %d   %20.3f %20.3f\n",SS.size(),S,T);
		SS.push_back(S);
		TT.push_back(T);
	}
	// Clip to the region
	unsigned int len = SS.size();
	//printf("total of %d points in isopycnal line\n", len);
	if (!len)
		return true;		// no data (probably impossible)
    
	//
	// Since proper clipping not done yet, kludge in a clip to box (+ freezing line)
	//

	// Trim anything to right of plot
	bool was_an_intersection = false;
	len = SS.size();
	int i;
	for (i = len - 1; i > -1; i--) {
		//printf("examine %d -- %f %f\n",i,SS[i],TT[i]);
		if (SS[i] < _xright) {
			if (i < int(len) - 1) {
				//printf("\n*** %d-th point inside, at %f %f\n",i,SS[i],TT[i]);
				double S0, T0;
				bool ok = intersection(SS[i], TT[i], SS[i + 1], TT[i + 1],
						       _xright, _ybottom, _xright, _ytop,
						       &S0, &T0);
				if (ok) {
					//printf("intersection returns %f %f (ok=%d)\n",S0,T0,ok);
					SS[i + 1] = S0;
					TT[i + 1] = T0;
					if (i < int(len) - 2) {
						SS.erase(SS.begin() + i + 2, SS.end());
						TT.erase(TT.begin() + i + 2, TT.end());
					}
					//int l = SS.size(); printf("RHS after: %f %f     %f %f\n", SS[l-2], TT[l-2],SS[l-1],TT[l-1]);
				}
			}
			was_an_intersection = true;
			break;
		}
	}
	if (!was_an_intersection)
		return true;

	// Trim anything to left of plot
	was_an_intersection = false;
	for (i = 0; i < int(len); i++) {
		//printf("examine %d -- %f %f\n",i,SS[i],TT[i]);
		if (SS[i] > _xleft) {
			if (i > 0) {
				//printf("\n*** %d-th point inside, at %f %f\n",i,SS[i],TT[i]);
				double S0, T0;
				bool ok = intersection(SS[i-1], TT[i-1], SS[i], TT[i],
						       _xleft, _ybottom, _xleft, _ytop,
						       &S0, &T0);
				if (ok) {
					//printf("intersection returns %f %f (ok=%d)\n",S0,T0,ok);
					SS[i - 1] = S0;
					TT[i - 1] = T0;
					if (i > 1) {
						SS.erase(SS.begin(), SS.begin() + i - 1);
						TT.erase(TT.begin(), TT.begin() + i - 1);
					}
				}
			}
			was_an_intersection = true;
			break;
		}
	}
	//printf("was_an_intersection %d\n", was_an_intersection);

	if (!was_an_intersection)
		return true;

	len = SS.size();
	if (len < 1)
		return true;
    
	// Trim to above freezing-point line (with pressure indicated for pot-temp)
	//printf("about to trim to freezing.  have %d data\n",len);
	for (i = 0; i < int(len); i++) {
		double Tf = sw_freezing_temperature(SS[i], P_sigma);
		if (TT[i] > Tf) {
			if (i == 0)
				break;		// all curve is above
			//printf("intersect Tfree at i=%d\n",i);
			double S0, T0;
			bool ok = intersection(SS[i-1], TT[i-1], SS[i], TT[i],
					       SS[i-1], sw_freezing_temperature(SS[i-1], P_theta),
					       SS[i], Tf,
					       &S0, &T0);
			if (ok) {
				SS[i - 1] = S0;
				TT[i - 1] = T0;
				if (i > 1) {
					SS.erase(SS.begin(), SS.begin() + i - 1);
					TT.erase(TT.begin(), TT.begin() + i - 1);
				}
			}
			//printf("RHS after: %f %f     %f %f\n", SS[0], TT[0],SS[1],TT[1]);
			break;
		}
	}

	// Draw, labelling appropriately
	vector<double> S_label_cm;	// where to put labels
	vector<double> T_label_cm;
	double cum_dist = 0.0;
	extern double _contour_space_first, _contour_space_later;
	double contour_space_first, contour_space_later;
	if (_contour_space_later < 0.0) {
		double xsize = get_var_scalar("$_xsize");
		double ysize = get_var_scalar("$_ysize");
		contour_space_first = 1.0;
		contour_space_later = labelled ? 0.5 * (xsize + ysize) : 0.0;
	} else {
		contour_space_first = _contour_space_first;
		contour_space_later = _contour_space_later;
	}
	double contour_spacing = contour_space_first;
	double first = true;
	GriPath iso;
	double S_cm, T_cm, S_cm_last = 0.0, T_cm_last = 0.0;
	for (i = 0; i < int(SS.size()); i++) {
		gr_usertocm(SS[i], TT[i], &S_cm, &T_cm);
		iso.push_back(S_cm, T_cm, first ? 'm' : 'l');
		if (!first) {
			cum_dist += sqrt((S_cm - S_cm_last) * (S_cm - S_cm_last) 
					 + (T_cm - T_cm_last) * (T_cm - T_cm_last)); 
			if (cum_dist > contour_spacing) {
				S_label_cm.push_back(S_cm);
				T_label_cm.push_back(T_cm - 0.5 * gr_currentfontsize_pt()/PT_PER_CM);
				contour_spacing = contour_space_later;
				cum_dist = 0.0;	// start over
			}
		}
		iso.push_back(S_cm, T_cm, first ? 'm' : 'l');
		S_cm_last = S_cm;
		T_cm_last = T_cm;
		first = false;
		//printf(" -- S,T,cumdist = %.4f %.4f %.1f\n", SS[i], TT[i],cum_dist);
	}
	set_line_width_curve();
	iso.stroke(units_cm);
	if (labelled) {
		char clabel[20];
		sprintf(clabel, _contourFmt.c_str(), density);
#if 0				// old way -- along sides
		GriString label(clabel);
		double xcm, ycm;
		double tic_direction = 0;
		get_var("..tic_direction..", &tic_direction);
		double tic_size = TICSIZE_DEFAULT;
		get_var("..tic_size..", &tic_size);
		set_environment();
		gr_usertocm(S, T, &xcm, &ycm);
		set_ps_color('t');
		if (hit_right) {
			ycm += 2.0 * tic_size; // put above
			label.draw(xcm, ycm, TEXT_CENTERED, 0.0);
		} else {
			xcm += 2.0 * tic_size; // put to right
			ycm -= gr_currentfontsize_pt() / PT_PER_CM / 2;
			label.draw(xcm, ycm, TEXT_LJUST, 0.0);
		}
#else
		string label(clabel);
		set_ps_color('t');
		GriColor white;
		white.setRGB(1.0, 1.0, 1.0);
		for (unsigned int ii = 0; ii < S_label_cm.size(); ii++) {
			gr_show_in_box(label, _griState.color_text(), white, S_label_cm[ii], T_label_cm[ii], 0.0);
		}
#endif
	}
	_drawingstarted = true;
	draw_axes_if_needed();
	return true;
} 


/*
 * Draw image, possibly color, in rectangle given in cm coords.
 */
void
gr_drawimage(unsigned char *im,
	     unsigned char *imTransform,
	     gr_color_model color_model,
	     unsigned char *mask,
	     double mask_r,
	     double mask_g,
	     double mask_b,
	     int imax,
	     int jmax,
	     double xl,		// in cm
	     double yb,		// in cm
	     double xr,		// in cm
	     double yt,		// in cm
	     bool insert_placer)
{
	extern bool     _grWritePS;
	extern FILE    *_grPS;
	unsigned char   cmask_r, cmask_g, cmask_b;
	bool            have_mask;
	unsigned char   value, mask_value = 0; // assign to calm compiler
	register int    i, j;
	int             perline = 0;
	int             perlineMAX;
	if (!_grWritePS)
		return;
	/* Figure out about mask */
	have_mask = (mask == NULL) ? false : true;
	rectangle box(xl, yb, xr, yt);
	bounding_box_update(box);
	// Convert cm to pt
	xl *= PT_PER_CM;
	xr *= PT_PER_CM;
	yb *= PT_PER_CM;
	yt *= PT_PER_CM;
	// Make image overhang the region.  This change, vsn 2.005, *finally*
	// solves a confusion I've had for a long time about how to do
	// images.
	if (imax > 1) {
		double dx = (xr - xl) / (imax - 1); // pixel width
		xl -= dx / 2.0;
		xr += dx / 2.0;
	}
	if (jmax > 1) {
		double dy = (yt - yb) / (jmax - 1); // pixel height
		yb -= dy / 2.0;
		yt += dy / 2.0;
	}
	rectangle r(xl / PT_PER_CM,
		    yb / PT_PER_CM,
		    xr / PT_PER_CM,
		    yt / PT_PER_CM);
	bounding_box_update(r);
	/*
	 * Handle BW and color differently, since PostScript handles differently.
	 */
	switch (color_model) {
	default:			/* ? taken as BW */
	case bw_model:
		perlineMAX = 39;	/* use only 78 columns so more readible */
		if (imax < perlineMAX)
			perlineMAX = imax;
		check_psfile();
		/*
		 * Write map to PostScript, creating a linear one if none exists
		 */
		fprintf(_grPS, "%% Push map onto stack, then image stuff.\n");
		fprintf(_grPS, "[\n");
		if (imTransform == NULL) {
			for (i = 0; i < 256; i++) {
				fprintf(_grPS, "%.4f ", i / 255.0);
				if (!((i + 1) % 10))
					fprintf(_grPS, "\n");
			}
		} else {
			for (i = 0; i < 256; i++) {
				fprintf(_grPS, "%.4f ", imTransform[i] / 255.0);
				if (!((i + 1) % 10))
					fprintf(_grPS, "\n");
			}
		}
		fprintf(_grPS, "\n]\n");
		if (insert_placer)
			fprintf(_grPS, "%%BEGIN_IMAGE\n");	/* for grepping in ps file */
		/*
		 * Now write image.
		 */
		fprintf(_grPS, "%f %f %f %f %d %d im\n",
			xl, yb, xr, yt, jmax, imax);
		if (have_mask == true) {
			int             diff, min_diff = 256;
			unsigned char   index = 0; // assign to calm compiler ????
			mask_value = (unsigned char)(255.0 * mask_r);
			/*
			 * If there is a mapping, must (arduously) look up which image
			 * value corresponds to this color.
			 */
			if (imTransform != NULL) {
				for (i = 0; i < 256; i++) {
					diff = (int) fabs(imTransform[i] - mask_value);
					if (diff < min_diff) {
						min_diff = diff;
						index = i;
					}
				}
				mask_value = index;
			}
		}
		for (j = jmax - 1; j > -1; j--) {
			for (i = 0; i < imax; i++) {
				value = *(im + i * jmax + j);
				if (have_mask == true && *(mask + i * jmax + j) == 2) {
					fprintf(_grPS, "%02X", mask_value);
				} else {
					fprintf(_grPS, "%02X", value);
				}
				if ((++perline) == perlineMAX) {
					fprintf(_grPS, "\n");
					perline = 0;
				}
			}
		}
		check_psfile();
		if (perline != 0)
			fprintf(_grPS, "\n");
		check_psfile();
		if (insert_placer)
			fprintf(_grPS, "%%END_IMAGE\n");	/* for grepping in ps file */
		break;
	case rgb_model:
		perlineMAX = 13;	/* use only 78 columns so more readible */
		if (imax < perlineMAX)
			perlineMAX = imax;
		if (insert_placer)
			fprintf(_grPS, "%%BEGIN_IMAGE\n");
		fprintf(_grPS, "%f %f %f %f %d %d cim\n",
			xl, yb, xr, yt, jmax, imax);
		check_psfile();
		if (have_mask == true) {
			warning("BUG in grimage.c - masking of color images not working yet");
		}
		cmask_r = (unsigned char)pin0_255(mask_r * 255.0);
		cmask_g = (unsigned char)pin0_255(mask_g * 255.0);
		cmask_b = (unsigned char)pin0_255(mask_b * 255.0);
		if (imTransform == NULL) {
			for (j = jmax - 1; j > -1; j--) {
				for (i = 0; i < imax; i++) {
					value = *(im + i * jmax + j);
					if (have_mask == true && *(mask + i * jmax + j) == 2) {
						fprintf(_grPS, "%02X%02X%02X", cmask_r, cmask_g, cmask_b);
					} else {
						fprintf(_grPS, "%02X%02X%02X", value, value, value);
					}
					if ((++perline) == perlineMAX) {
						fprintf(_grPS, "\n");
						perline = 0;
					}
				}
			}
			check_psfile();
		} else {
			for (j = jmax - 1; j > -1; j--) {
				for (i = 0; i < imax; i++) {
					value = *(im + i * jmax + j);
					if (have_mask == true && *(mask + i * jmax + j) == 2) {
						fprintf(_grPS, "%02X%02X%02X", cmask_r, cmask_g, cmask_b);
					} else {
						fprintf(_grPS, "%02X%02X%02X", imTransform[value], imTransform[value + 256], imTransform[value + 512]);
					}
					if ((++perline) == perlineMAX) {
						fprintf(_grPS, "\n");
						perline = 0;
					}
				}
			}
		}
		if (perline != 0)
			fprintf(_grPS, "\n");
		if (insert_placer)
			fprintf(_grPS, "%%END_IMAGE\n");	/* for grepping in ps file */
		check_psfile();
	}				/* switch(color_model) */
}
