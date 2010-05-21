#include	<stdio.h>
#include	<stdlib.h>
#include	<string>
#include	<string.h>
#include	<math.h>
#include        <vector>
#include	<ctype.h>
#include	"extern.h"
#include        "defaults.h"
#include        "private.h"
#include        "macro.h"
#include        "debug.h"

#if defined(HAVE_PROJ)
extern "C" {
#include "proj/projects.h" /* for 'proj' */
}
#endif

#if 0
#include	"gr.hh"
#endif
#include	"image_ex.h"
#if 0
#include        "files.hh"
#include        "superus.hh"
#include        "gr_coll.hh"
#include        "GriState.hh"

static inline bool between(double x, double lim0, double lim1);
#endif
bool            postscript_clipping_on = false; // used in endup.m also
#if 0
extern char     _grTempString[];
void            reset_top_of_plot(void);
bool            ignore_initial_newline(void);
bool            set_ignore_initial_newlineCmd(void);
bool            set_image_colorscaleCmd(void);
bool            set_image_grayscaleCmd(void);
bool            set_image_grayscale_using_hist(void);
bool            set_image_missingCmd(void);
#endif
bool            set_image_rangeCmd(void);
#if 0
bool            set_grid_missingCmd(void);
bool            set_grid_missing_curve(bool inside);
bool            mask_an_island(double *x, double *y, unsigned int n);
#endif
bool            set_y_axis_labelCmd(void);
static bool     already_landscape = false;
#if 0
// following shared with read.c
double          _input_data_window_x_min = 1.0;
double          _input_data_window_x_max = -1.0;
double          _input_data_window_y_min = 1.0;
double          _input_data_window_y_max = -1.0;
bool            _input_data_window_x_exists = false;
bool            _input_data_window_y_exists = false;
static double   tmp, tmp2, xpt, ypt;

static inline bool between(double x, double lim0, double lim1)
{
	if (lim0 <= x && x < lim1)
		return true;
	if (lim1 <= x && x < lim0)
		return true;
	return false;
}

#endif
void
reset_top_of_plot()
{
	_top_of_plot = get_var_scalar("$_ymargin") + get_var_scalar("$_ysize");
}
#if 0

static bool     ignore_initial_newline_flag = false;
bool
set_ignore_initial_newlineCmd()
{
	switch (_nword) {
	case 4:
		ignore_initial_newline_flag = true;
		break;
	case 5:
		if (!strcmp(_word[4].c_str(), "off")) {
			ignore_initial_newline_flag = false;
			return true;
		} else {
			demonstrate_command_usage();
			return false;
		}
	default:
		demonstrate_command_usage();
		return false;
	}
	return false;
}
bool
ignore_initial_newline()
{
	return ((ignore_initial_newline_flag) ? true : false);
}

bool
set_axes_styleCmd()
{
	if (_nword < 3) {
		err("Too few words in `set axes'");
		return false;
	}
	if (strcmp(_word[2].c_str(), "style")) {
		err("Third word of `set axes' must be \"style\".");
		return false;
	}
	if (_nword < 4) {
		err("`set axes style' to what?");
		return false;
	}
	// `set axes style offset [.distance_cm.]'
	if (_nword >= 4 && !strcmp(_word[3].c_str(), "offset")) {
		double          tmp;
		if (_nword == 5) {
			if (!getdnum(_word[4].c_str(), &tmp)) {
				READ_WORD_ERROR(".distance_cm.");
				return false;
			}
			_axes_offset = tmp;
		} else {
			tmp = get_var_scalar("$_tic_size");
		}
		_axes_offset = tmp;
		return true;
	}
	switch (_nword) {
	case 4:
		// `set axes style .style.|rectangular|polar|default|none'
		if (!strcmp(_word[3].c_str(), "default")) {
			_axesStyle = 0;
			_axes_offset = 0.0;
			return true;
		} else if (!strcmp(_word[3].c_str(), "none")) {
			_need_x_axis = false;
			_need_y_axis = false;
		} else if (!strcmp(_word[3].c_str(), "rectangular")) {
			// `set axes style rectangular'
			_axesStyle = 0;
			return true;
		} else if (!strcmp(_word[3].c_str(), "polar")) {
			// `set axes style polar'
			_axesStyle = AXES_POLAR;
			return true;
		} else {
			// `set axes style .style.'
			if (!getdnum(_word[3].c_str(), &tmp))
				return false;
			if (tmp < 0.0 || tmp > 2.0) {
				err("Ignoring bad axes type <0 or >2");
				return false;
			}
			_axesStyle = (int) floor(0.5 + tmp);
			return true;
		}
		break;
	default:
		NUMBER_WORDS_ERROR;
		return false;
	}
	return true;
}
#endif

// Set size of arrowheads
// @param $size arrowhead half-width @unit cm @default 0.2
// @param $p percentage
// ($;$$$$) { # $size|{as $p percent of length}|default
bool
set_arrow_sizeCmd()
{
	unsigned int num_params = cmd_parameters();
	double size;
	switch (num_params) {
	case 1:
		if (cmd_parameter_is_n(0)) {
			size = cmd_parameter_n(0);
			if (size < 0.0) {
				err("Ignoring bad (negative) arrow size.");
				return false;
			}
			put_var("$_arrow_size", size);
		} else {
			if (strNE(cmd_parameter_s(0), "default")) {
				err("`set arrow size' expecting word `default', not `\\", cmd_parameter_s(0), "' as found", "\\");
				return false;
			}
			put_var("$_arrow_size", ARROWSIZE_DEFAULT);
		}
		break;
	case 5:
		if (cmd_parameter_is_n(0)) {
			err("`set arrow size' expecting word `as' but found numeric argument `\\", dtoa(cmd_parameter_n(0)), "'", "\\");
			return false;
		}
		if (strNE(cmd_parameter_s(0), "as")) {
			err("`set arrow size' expecting word `as' but found `\\", cmd_parameter_s(0), "'", "\\");
			return false;
		}
		if (!cmd_parameter_is_n(1)) {
			err("`set arrow size as' expecting numeric argument but found string `\\", dtoa(cmd_parameter_n(1)), "'", "\\");
			return false;
		}
		put_var("$_arrow_size", -fabs(cmd_parameter_n(1) / 100.0));
		if (!strEQ(cmd_parameter_s(2), "percent")) {
			err("`set arrow size as ...' expecting word `percent' but found `\\", cmd_parameter_s(2), "'", "\\");
			return false;
		}
		if (!strEQ(cmd_parameter_s(3), "of")) {
			err("`set arrow size as ...' expecting word `of' but found `\\", cmd_parameter_s(2), "'", "\\");
			return false;
		}
		if (!strEQ(cmd_parameter_s(4), "length")) {
			err("`set arrow size as ...' expecting word `length' but found `\\", cmd_parameter_s(2), "'", "\\");
			return false;
		}
		break;
	default:
		err("`set arrow size' may take only 1 or 5 arguments, not \\", dtoa(num_params), " as given", "\\");
		return false;
	}
	return true;
}
bool
set_arrow_typeCmd()
{
	if (_arrow_type != 0 && _arrow_type != 1 && _arrow_type != 2) {
		err("Valid arrow types are 0, 1 and 2");
		return false;
	}
	_arrow_type = int(floor(0.5 + cmd_parameter_n(0)));
	return true;
}

#if 0
bool
set_bounding_boxCmd()
{
	// Set to _bounding_box_user
	_user_gave_bounding_box = true;
	double ll_x, ll_y, ur_x, ur_y; // user-units
	Require(_nword == 7, err("Must specify .ll_x. .ll_y. .ur_x. .ur_y."));
	if (!getdnum(_word[3].c_str(), &ll_x)) {
		demonstrate_command_usage();
		err("Can't read .ll_x. in `\\", _word[3].c_str(), "'.", "\\");
		return false;
	}
	if (!getdnum(_word[4].c_str(), &ll_y)) {
		demonstrate_command_usage();
		err("Can't read .ll_y. in `\\", _word[4].c_str(), "'.", "\\");
		return false;
	}
	if (!getdnum(_word[5].c_str(), &ur_x)) {
		demonstrate_command_usage();
		err("Can't read .ur_x. in `\\", _word[5].c_str(), "'.", "\\");
		return false;
	}
	if (!getdnum(_word[6].c_str(), &ur_y)) {
		demonstrate_command_usage();
		err("Can't read .ur_y. in `\\", _word[6].c_str(), "'.", "\\");
		return false;
	}
	double ll_x_cm, ll_y_cm, ur_x_cm, ur_y_cm; // poitns
	gr_usertocm(ll_x, ll_y, &ll_x_cm, &ll_y_cm);
	gr_usertocm(ur_x, ur_y, &ur_x_cm, &ur_y_cm);
	_bounding_box_user.set(ll_x_cm, ll_y_cm, ur_x_cm, ur_y_cm);
	return true;
}
#endif

#if 0                           // 1999-jun-16 not using _clipped_xy
static void
set_grid_clipping_on()
{
	unsigned int nx = _xmatrix.size();
	unsigned int ny = _ymatrix.size();
	if (_xgrid_exists && _ygrid_exists && _grid_exists) {
		for (unsigned int i = 0; i < nx; i++) {
			for (unsigned int j = 0; j < ny; j++) {
				printf("clip-DEBUG i=%d j=%d\n",i,j);
				_clipped_xy(i, j) = (inside_box(_xmatrix[i], _ymatrix[j]) == false) ? true : false;
				bool dan = (inside_box(_xmatrix[i], _ymatrix[j]) == false) ? true : false;
			}
		}
	}
}
static void
set_grid_clipping_off()
{
	unsigned int nx = _xmatrix.size();
	unsigned int ny = _ymatrix.size();
	if (_xgrid_exists && _ygrid_exists && _grid_exists)
		for (unsigned int i = 0; i < nx; i++)
			for (unsigned int j = 0; j < ny; j++)
				_clipped_xy(i, j) = false;
}
#endif

// cmd set clip ($;@) { # off|on|{rectangle $xl $yb $xr $yt}|{to curve}
bool
set_clipCmd()
{
	extern FILE    *_grPS;
	unsigned int num_params = cmd_parameters();
	double xl, xr, yb, yt;

	switch (num_params) {
        case 1:
        {
		if (cmd_parameter_is_n(0)) {
			err("`set clip' expecting `on' or `off', not numeric argument `\\", dtoa(cmd_parameter_n(0)), "' as given", "\\");
			return false;
		}
		const char *first_word = cmd_parameter_s(0);

		if (strEQ(first_word, "on")) {
			xl = _xleft;
			xr = _xright;
			yb = _ybottom;
			yt = _ytop;
		} else if (strEQ(first_word, "off")) {
			_clipData = 0;
			_clipxleft = _clipxright = _clipybottom = _clipytop = 0.0;
			//set_grid_clipping_off();
			if (postscript_clipping_on) {
				fprintf(_grPS, "S Q %% turn clipping off\n");
				check_psfile();
				postscript_clipping_on = false;
				set_environment();
				set_ps_color('p', true);
				set_line_width_curve();
			}
			return true;
		} else {
			err("`set clip' expecting `on' or `off'");
			return false;
		}
        }
	break;
        case 2:                 // 'set clip to curve'
        {
		if (cmd_parameter_is_n(0)) {
			err("`set clip' expecting `to curve', not numeric argument `\\", dtoa(cmd_parameter_n(0)), "' as given", "\\");
			return false;
		}
		if (cmd_parameter_is_n(1)) {
			err("`set clip' expecting `to curve', not numeric argument `\\", dtoa(cmd_parameter_n(1)), "' as given", "\\");
			return false;
		}
		if (strNE("to", cmd_parameter_s(0)) || strNE("curve",  cmd_parameter_s(1))) {
			err("`set clip' expecting `to curve', not `\\", cmd_parameter_s(0), " ", cmd_parameter_s(1), "' as given.", "\\");
			return false;
		}

		// Put the command into the ps file
		if (postscript_clipping_on) {
			fprintf(_grPS, "S Q %% turn clipping off\n");
			check_psfile();
		}
                fprintf(_grPS, "q n %% `set clip to curve' setting clipping on\n");

		int xind = get_vector_var_index("$_x");
		if (xind < 0) {
			err("X column ($_x) has become non-existent\n");
			return false;
		}
		int yind = get_vector_var_index("$_y");
		if (yind < 0) {
			err("Y column ($_y) has become non-existent\n");
			return false;
		}
		extern vector<var> vector_vars;
		var* xvar = &vector_vars[xind];
		var* yvar = &vector_vars[yind];
		int xlen = 1 + xvar->get_end_index();
		if (xlen < 1) {
			err("`set clip to curve' noticed that no x data exist\n");
			return false;
		}
		int ylen = 1 + yvar->get_end_index();
		if (ylen < 1) {
			err("`set clip to curve' noticed that no y data exist\n");
			return false;
		}
		if (xlen != ylen) {
			err("`set clip to curve' requires xlen == ylen\n");
			return false;
		}

		// Set scale, so can convert user units to page units
		if (!scales_defined()) {
			no_scales_error();
			return false;
		}
		set_environment();  

		for (int i = 0; i < xlen; i++) {
			double x = xvar->get_at_index(i).get_numeric();
			double y = yvar->get_at_index(i).get_numeric();
			double xpt, ypt;
			gr_usertopt(x, y, &xpt, &ypt);
			if (i == 0)
				fprintf(_grPS, "%f %f moveto\n", xpt, ypt);
			else 
				fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
		}
		fprintf(_grPS, "h W\n");
		fprintf(_grPS, "n %% turn clipping on\n");

		check_psfile();
		postscript_clipping_on = true;
		_clipData = -1;     // KEEP??

		return true;
        }
	break;
        case 5:                 // 'set clip xl yb xr yt'
        {
		const char *first_word = cmd_parameter_s(0);
		if (!strEQ(first_word, "rectangle")) {
			fatal_error("`set clip' expecting `rectangle' but got `", first_word, "'", "\\");
			return false;
		}
		xl = cmd_parameter_n(1);
		xr = cmd_parameter_n(2);
		yb = cmd_parameter_n(3);
		yt = cmd_parameter_n(4);
        }
	break;
        default:
		err("`set clip off|on|{rectangle $xl $yb $xr $yt}|{to curve}' given wrong number of parameters");
		return false;
	}

	set_environment();		// so that usertopt knows scales

	_clipxleft = xl;		// *** for inside_box() ***
	_clipxright = xr;
	_clipybottom = yb;
	_clipytop = yt;

	// Update the bounding box (since it will be ignored whilst
	// clipping is on)
	static rectangle clip_rect;
	double xcm, ycm;
	gr_usertocm(xl, yb, &xcm, &ycm);
	clip_rect.set_llx(xcm);
	clip_rect.set_lly(ycm);
	//printf("set clip_rect LL to (%f %f) cm\n",xcm,ycm);
	gr_usertocm(xr, yt, &xcm, &ycm);
	clip_rect.set_urx(xcm);
	clip_rect.set_ury(ycm);
	//printf("set clip_rect UR to (%f %f) cm\n",xcm,ycm);
	bounding_box_update(clip_rect);

	// Put the command into the ps file
	if (postscript_clipping_on) {
		fprintf(_grPS, "S Q %% turn clipping off\n");
		check_psfile();
	}

	//printf("Clip x between %f and %f\n", xl, xr);
	//printf("Clip y between %f and %f\n", yb, yt);

	fprintf(_grPS, "q n %% turn clipping on\n");
	double xpt, ypt;
	gr_usertopt(xl, yb, &xpt, &ypt);
	fprintf(_grPS, "%f %f moveto\n", xpt, ypt);
	gr_usertopt(xr, yb, &xpt, &ypt);
	fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
	gr_usertopt(xr, yt, &xpt, &ypt);
	fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
	gr_usertopt(xl, yt, &xpt, &ypt);
	fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
	fprintf(_grPS, "h W\n");
	fprintf(_grPS, "n %% turn clipping on\n");

	//set_grid_clipping_on();
	check_psfile();
	postscript_clipping_on = true;
	_clipData = -1;
	return true;
}

#if 0
bool
set_clipCmd()
{
	const int segments = 50;	// segs along side of nonrect coord region
	Require(_nword > 2, err("Must specify `set clip on' or `set clip off'"));
	if (!strcmp(_word[2].c_str(), "postscript")) {
		// PostScript clipping
		extern FILE    *_grPS;
		if (_nword < 4) {
			err("`set clip postscript ...' needs >= 4 words.");
			return false;
		}
		if (!strcmp(_word[3].c_str(), "off")) {
			if (!postscript_clipping_on) {
				// Ignore if already off
				return true;
			}
			fprintf(_grPS, "S Q %% turn clipping off\n");
			check_psfile();
			postscript_clipping_on = false;
			return true;
		} else if (!strcmp(_word[3].c_str(), "on")) {
			double xl, xr, yb, yt, xx, yy, delta;
			unsigned i;
			extern gr_axis_properties _grTransform_x;
			if (postscript_clipping_on) {
				fprintf(_grPS, "S Q %% turn clipping off\n");
				check_psfile();
			}
			if (_nword == 4) {
				// set clip postscript on
				xl = _xleft;
				xr = _xright;
				yb = _ybottom;
				yt = _ytop;
			} else if (_nword == 8) {
				if (!getdnum(_word[4].c_str(), &xl)) {
					demonstrate_command_usage();
					err("Can't read .xleft. in `\\", _word[4].c_str(), "'.", "\\");
					return false;
				}
				if (!getdnum(_word[5].c_str(), &xr)) {
					demonstrate_command_usage();
					err("Can't read .xright. in `\\", _word[5].c_str(), "'.", "\\");
					return false;
				}
				if (!getdnum(_word[6].c_str(), &yb)) {
					demonstrate_command_usage();
					err("Can't read $ybottom in `\\", _word[6].c_str(), "'.", "\\");
					return false;
				}
				if (!getdnum(_word[7].c_str(), &yt)) {
					demonstrate_command_usage();
					err("Can't read .ytop. in `\\", _word[7].c_str(), "'.", "\\");
					return false;
				}
			} else {
				NUMBER_WORDS_ERROR;
				demonstrate_command_usage();
				return false;
			}
			set_environment();
			fprintf(_grPS, "q n %% turn clipping on\n");

			gr_usertopt(xl, yb, &xpt, &ypt);
			fprintf(_grPS, "%f %f moveto\n", xpt, ypt);
			gr_usertopt(xr, yb, &xpt, &ypt);
			fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
			gr_usertopt(xr, yt, &xpt, &ypt);
			fprintf(_grPS, "%f %f lineto\n", xpt, ypt);
			gr_usertopt(xl, yt, &xpt, &ypt);
			fprintf(_grPS, "%f %f lineto\n", xpt, ypt);

			fprintf(_grPS, "h W\n");
			fprintf(_grPS, "n %% turn clipping on\n");
			check_psfile();
			postscript_clipping_on = true;
			return true;
		}
	} else if (!strcmp(_word[2].c_str(), "on")) {
		// Non-PostScript clipping
		if (_nword == 3)
//DONE	    _clipData = -1;
			else if (_nword == 7) {
				if (!getdnum(_word[3].c_str(), &tmp)) {
					READ_WORD_ERROR(".xleft.");
					_clipData = 0;
					_clipxleft = _clipxright = _clipybottom = _clipytop = 0.0;
					return false;
				}
				_clipxleft = tmp;
				if (!getdnum(_word[4].c_str(), &tmp)) {
					READ_WORD_ERROR(".xright.");
					_clipData = 0;
					_clipxleft = _clipxright = _clipybottom = _clipytop = 0.0;
					return false;
				}
				_clipxright = tmp;
				if (!getdnum(_word[5].c_str(), &tmp)) {
					READ_WORD_ERROR(".ybottom.");
					_clipData = 0;
					_clipxleft = _clipxright = _clipybottom = _clipytop = 0.0;
					return false;
				}
				_clipybottom = tmp;
				if (!getdnum(_word[6].c_str(), &tmp)) {
					READ_WORD_ERROR(".ytop.");
					_clipData = 0;
					_clipxleft = _clipxright = _clipybottom = _clipytop = 0.0;
					return false;
				}
				_clipytop = tmp;
				_clipData = 1;
			} else
				err("`set clip on' takes 4 parameters");
		//set_grid_clipping_on();
//DONE    } else if (!strcmp(_word[2].c_str(), "off")) {
//DONE	_clipData = 0;
//DONE	set_grid_clipping_off();
	} else {
		err("Must specify `set clip on' or `set clip off'");
		return false;
	}
	return true;
}
#endif

#define CHECK_RGB_RANGE(item) {						\
    if ((item) < 0.0) {							\
	warning("`set color rgb' value being clipped to range 0-1");	\
	(item) = 0.0;							\
    } else if ((item) > 1.0) {						\
	warning("`set color rgb' value being clipped to range 0-1");	\
        (item) = 1.0;							\
    }									\
}

// 'set color "red"'
// 'set color hsv $h $s $v'
// 'set color rgb $r $g $b'
bool
set_colorCmd()
{
	double          red, green, blue;
	if (cmd_parameters() == 1) {
		const char *name = cmd_parameter_s(0);
		if (!look_up_color(name, &red, &green, &blue)) {
			err("Unknown color `\\", name, "'.", "\\");
			return false;
		} else {
			put_var("$_red", red);
			put_var("$_green", green);
			put_var("$_blue", blue);
			GriColor c;
			c.setRGB(red, green, blue);
			_griState.set_color_line(c);
			if (_griState.separate_text_color() == false)
				_griState.set_color_text(c);
			return true;
		}
	} else if (cmd_parameters() == 4) {
		if (strEQ(cmd_parameter_s(0), "rgb")) {
			red   = pin0_1(cmd_parameter_n(1));
			green = pin0_1(cmd_parameter_n(2));
			blue  = pin0_1(cmd_parameter_n(3));
			put_var("$_red", red);
			put_var("$_green", green);
			put_var("$_blue", blue);
			GriColor c;
			c.setRGB(red, green, blue);
			_griState.set_color_line(c);
			if (_griState.separate_text_color() == false)
				_griState.set_color_text(c);
			return true;
		} else if (strEQ(cmd_parameter_s(0), "hsv")) {
			double hue        = pin0_1(cmd_parameter_n(1));
			double saturation = pin0_1(cmd_parameter_n(2));
			double value      = pin0_1(cmd_parameter_n(3));
			put_var("$_hue", hue);
			put_var("$_saturation", saturation);
			put_var("$_value", value);
			GriColor c;
			c.setHSV(hue, saturation, value);
			_griState.set_color_line(c);
			if (_griState.separate_text_color() == false)
				_griState.set_color_text(c);
			return true;
		} else {
			err("`set color' was expecting `rgb' or `hsv' but got `\\", cmd_parameter_s(0), "'", "\\");
			return false;
		}
	} else {
		err("Wrong number (\\", dtoa(cmd_parameters()), ") of arguments to `set color name|{rgb $r $g $b}|{hsv $h $s $b}; expecting 1 or 4\n", "\\");
		return false;
	}
}

bool
set_x_type_linearCmd()
{
	_xtype = gr_axis_LINEAR;
	_xtype_map = ' ';
	return true;
}
bool
set_x_type_logCmd()
{
	_xtype_map = ' ';
	if (_xtype == gr_axis_LOG)	// only change if necessary 
		return true;
	if (!_xscale_exists) {
		_xtype = gr_axis_LOG;
		gr_setxtransform(_xtype);
		return true;
	}
	if ((_xright > _xleft) && (_xleft > 0.0) && (_xright > 0.0)) {
		put_var("$_xleft", pow(10.0, floor(0.5 + log10(_xleft))));
		put_var("$_xright", pow(10.0, floor(0.5 + log10(_xright))));
		_xinc = 1;
		_xtype = gr_axis_LOG;
		return true;
	} else {
		err("Cannot convert to log axis because present x-axis has negative numbers.  First `delete scale'.");
		return false;
	}
}
#if 0
bool
set_x_typeCmd()
{
	extern char      _xtype_map;
	switch (_nword) {
	case 5:
		if (word_is(3, "map")) {
			if (word_is(4, "E")) {
				_xtype_map = 'E';
			} else if (word_is(4, "W")) {
				_xtype_map = 'W';
			} else if (word_is(4, "S")) {
				_xtype_map = 'S';
			} else if (word_is(4, "N")) {
				_xtype_map = 'N';
			} else {
				err("X map type must be `E', `W', `S' or `N'");
				return false;
			}
		} else {
			err("Type must be `linear', `log', or `map'");
			return false;
		}
		break;
	case 4:
	DONE:	if (!strcmp(_word[3].c_str(), "linear")) {
	DONE:	} else if (!strcmp(_word[3].c_str(), "log")) {
	}
	}
}
#endif

bool
set_y_type_linearCmd()
{
	_ytype = gr_axis_LINEAR;
	_ytype_map = ' ';
	return true;
}
bool
set_y_type_logCmd()
{
	_ytype_map = ' ';
	if (_ytype == gr_axis_LOG)	// only change if necessary 
		return true;
	if (!_yscale_exists) {
		_ytype = gr_axis_LOG;
		gr_setytransform(_ytype);
		return true;
	}
	if ((_ytop > _ybottom) && (_ytop > 0.0) && (_ybottom > 0.0)) {
		put_var("$_ybottom", pow(10.0, floor(0.5 + log10(_ybottom))));
		put_var("$_ytop", pow(10.0, floor(0.5 + log10(_ytop))));
		_yinc = 1;
		_ytype = gr_axis_LOG;
		return true;
	} else {
		err("Cannot convert to log axis because present y-axis has negative numbers.  First `delete scale'.");
		return false;
	}
}
#if 0
bool
set_y_typeCmd()
{
	extern char      _ytype_map;
	switch (_nword) {
	case 5:
		if (word_is(3, "map")) {
			if (word_is(4, "N")) {
				_ytype_map = 'N';
			} else if (word_is(4, "S")) {
				_ytype_map = 'S';
			} else if (word_is(4, "E")) {
				_ytype_map = 'E';
			} else if (word_is(4, "W")) {
				_ytype_map = 'W';
			} else {
				err("Y map type must be `E', `W', `S' or `N'");
				return false;
			}
		} else {
			err("Type must be `linear', `log', or `map'");
			return false;
		}
		break;
	case 4:
	DONE:	if (!strcmp(_word[3].c_str(), "linear")) {
	DONE:	} else if (!strcmp(_word[3].c_str(), "log")) {
	}
	}
}
#endif

#if 0
// `set z missing above|below .intercept. .slope.'
bool
set_z_missingCmd()
{
	double slope, intercept;
	typedef enum { above, below, dont_know } WHERE;
	WHERE where = dont_know;
	switch (_nword) {
	case 6:
		if (!getdnum(_word[4].c_str(), &intercept))
			return false;
		if (!getdnum(_word[5].c_str(), &slope))
			return false;
		if (word_is(3, "above"))
			where = above;
		else if (word_is(3, "below"))
			where = below;
		break;
	default:
		NUMBER_WORDS_ERROR;
		return false;
	}
	if (_colX.size() < 1) {
		err("First `read columns ... x'");
		return false;
	}
	if (_colY.size() < 1) {
		err("First `read columns ... y'");
		return false;
	}
	if (_colZ.size() < 1) {
		err("First `read columns ... z'");
		return false;
	}
	double missing = gr_currentmissingvalue();
	unsigned int i;
	switch (where) {
	case above:
		for (i = 0; i < _colX.size(); i++) {
			if (_colY[i] > intercept + slope * _colX[i])
				_colZ[i] = missing;
		}
		break;
	case below:
		for (i = 0; i < _colX.size(); i++) {
			if (_colY[i] < intercept + slope * _colX[i])
				_colZ[i] = missing;
		}
		break;
	default:
		demonstrate_command_usage();
		err("Fourth word must be `above' or `below'");
		return false;
	}
	
	return true;
}



bool
set_font_colorCmd()
{
	double          red, green, blue;
	GriColor       c;
	switch (_nword) {
	case 4:
		if (!look_up_color(_word[3].c_str(), &red, &green, &blue)) {
			err("`set font color' given unknown colorname `\\", _word[3].c_str(), "'", "\\");
			return false;
		}
		c.setRGB(red, green, blue);
		_griState.set_color_text(c);
		_griState.set_separate_text_color(true);
		return true;
	case 7:
		if (!strcmp(_word[3].c_str(), "rgb")) {
			// `set color rgb .red. .green. .blue.'
			Require(getdnum(_word[4].c_str(), &red), READ_WORD_ERROR(".red."));
			Require(getdnum(_word[5].c_str(), &green), READ_WORD_ERROR(".green."));
			Require(getdnum(_word[6].c_str(), &blue), READ_WORD_ERROR(".blue."));
			// Clip if necessary
			if (red < 0.0) {
				warning("`set color rgb' value being clipped to 0");
				red = 0.0;
			} else if (red > 1.0) {
				warning("`set color rgb' value being clipped to 0");
				red = 1.0;
			}
			if (green < 0.0) {
				warning("`set color rgb' .green. value being clipped to 0");
				green = 0.0;
			} else if (green > 1.0) {
				warning("`set color rgb' value being clipped to 0");
				green = 1.0;
			}
			if (blue < 0.0) {
				warning("`set color rgb' value being clipped to 0");
				blue = 0.0;
			} else if (blue > 1.0) {
				warning("`set color rgb' value being clipped to 0");
				blue = 1.0;
			}
			_griState.set_separate_text_color(true);
			c.setRGB(red, green, blue);
			_griState.set_color_text(c);
			return true;
		} else if (!strcmp(_word[3].c_str(), "hsv")) {
			// `set color hsv $h $s $v'
			double          hue, saturation, brightness;
			Require(getdnum(_word[4].c_str(), &hue), READ_WORD_ERROR(".hue."));
			Require(getdnum(_word[5].c_str(), &saturation), READ_WORD_ERROR(".saturation."));
			Require(getdnum(_word[6].c_str(), &brightness), READ_WORD_ERROR(".brightness."));
			// Clip if necessary
			if (hue < 0.0) {
				warning("`set color hsv' value being clipped to 0");
				hue = 0.0;
			} else if (hue > 1.0) {
				warning("`set color hsv' value being clipped to 0");
				hue = 1.0;
			}
			if (saturation < 0.0) {
				warning("`set color hsv' value being clipped to 0");
				saturation = 0.0;
			} else if (saturation > 1.0) {
				warning("`set color hsv' value being clipped to 0");
				saturation = 1.0;
			}
			if (brightness < 0.0) {
				warning("`set color hsv' value being clipped to 0");
				brightness = 0.0;
			} else if (brightness > 1.0) {
				warning("`set color hsv' value being clipped to 0");
				brightness = 1.0;
			}
			c.setHSV(hue, saturation, brightness);
			_griState.set_color_text(c);
			_griState.set_separate_text_color(true);
			return true;
		} else {
			err("Can't understand command.");
			demonstrate_command_usage();
			return false;
		}
	default:
		NUMBER_WORDS_ERROR;
		demonstrate_command_usage();
		return false;
	}
}
#endif

bool
set_font_sizeCmd()
{
	if (cmd_parameter_is_n(0)) {
		double fs = cmd_parameter_n(0);
		if (fs < 0.0) {
			err("ignoring negative font size");
			return false;
		}
		put_var("$_font_size", fs);
		gr_setfontsize_pt(fs);
	} else {
		if (strNE(cmd_parameter_s(0), "default")) {
			err("`set font size' expecting word `default', but found word `\\", cmd_parameter_s(0), "'instead", "\\");
			return false;
		}
		put_var("$_font_size", FONTSIZE_PT_DEFAULT);
		gr_setfontsize_pt(FONTSIZE_PT_DEFAULT);
	}
	return true;
}

bool
set_font_toCmd()
{
	const char *name = cmd_parameter_s(0);
	if (!strcmp(name, "Courier"))
		gr_setfont(gr_font_Courier);
	else if (!strcmp(name, "Helvetica"))
		gr_setfont(gr_font_Helvetica);
	else if (!strcmp(name, "HelveticaBold"))
		gr_setfont(gr_font_HelveticaBold);
	else if (!strcmp(name, "Palatino"))
		gr_setfont(gr_font_PalatinoRoman);
	else if (!strcmp(name, "PalatinoRoman"))
		gr_setfont(gr_font_PalatinoRoman);
	else if (!strcmp(name, "PalatinoBold"))
		gr_setfont(gr_font_PalatinoBold);
	else if (!strcmp(name, "Symbol"))
		gr_setfont(gr_font_Symbol);
	else if (!strcmp(name, "Times"))
		gr_setfont(gr_font_TimesRoman);
	else if (!strcmp(name, "TimesRoman"))
		gr_setfont(gr_font_TimesRoman);
	else if (!strcmp(name, "TimesBold"))
		gr_setfont(gr_font_TimesBold);
	else {
		string msg = "Unknown font `";
		msg += name;
		msg += "'.\n    Available fonts: Helvetica (the default), HelveticaBold,\n"
			"        Palatino, PalatinoRoman, PalatinoBold,\n"
			"        Times, TimesRoman, TimesBold,\n"
			"        Courier, Symbol.\n";
		fatal_error(filename_runtime, fileline_runtime, msg.c_str());
		return false;
	}
	return true;
}

bool
set_dashCmd()
{
	_dash.erase(_dash.begin(), _dash.end()); // go to solid
	unsigned int num_params = cmd_parameters();

	if (num_params == 0) {      // 'set dash'
		_dash.push_back(0.4);
		_dash.push_back(0.1);
		return true;
	}
	if (num_params == 1) {
		if (cmd_parameter_is_n(0) == false) {
			if (strEQ(cmd_parameter_s(0), "on")) {
				_dash.push_back(0.4);
				_dash.push_back(0.1);
				return true;
			} else if (strEQ(cmd_parameter_s(0), "off")) {
				return true;
			} else {
				err("`set dash' expecting word `on' or `off', but found word `\\", cmd_parameter_s(0), "'", "\\");
				return false;
			}
		}
		switch ((int) (0.5 + fabs((double) cmd_parameter_n(0)))) {
		case 0:
			break;		// means solid line
		case 1:
			_dash.push_back(0.2);
			_dash.push_back(0.1);
			break;
		case 2:
			_dash.push_back(0.4);
			_dash.push_back(0.1);
			break;
		case 3:
			_dash.push_back(0.6);
			_dash.push_back(0.1);
			break;
		case 4:
			_dash.push_back(0.8);
			_dash.push_back(0.1);
			break;
		case 5:
			_dash.push_back(1.0);
			_dash.push_back(0.1);
			break;
		case 10:
			_dash.push_back(_griState.linewidth_line() / PT_PER_CM);
			_dash.push_back(_griState.linewidth_line() / PT_PER_CM);
			break;
		case 11:
			_dash.push_back(_griState.linewidth_line() / PT_PER_CM);
			_dash.push_back(2.0 * _griState.linewidth_line() / PT_PER_CM);
			break;
		default:
			_dash.push_back(0.2);
			_dash.push_back(0.1);
			break;
		}
	} else {
		// List of values
		for (unsigned int i = 0; i < num_params; i++) {
			_dash.push_back(cmd_parameter_n(i));
		}
	}
	return true;
}

#if 0
bool
set_ignoreCmd()
{
	switch (_nword) {
	case 4:
		if (!strcmp(_word[1].c_str(), "ignore")
		    && !strcmp(_word[2].c_str(), "error")
		    && !strcmp(_word[3].c_str(), "eof")) {
			_ignore_eof = true;
		} else {
			demonstrate_command_usage();
			err("Can't understand command.");
		}
		break;
	default:
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		break;
	}
	return false;
}

bool
set_input_data_windowCmd()
{
	double minval, maxval;
	switch (_nword) {
	case 6:			// set input data window x|y off
		if (strcmp(_word[5].c_str(), "off")) {
			demonstrate_command_usage();
			err("In this context, the last word must be `off'.");
			return false;
		}
		if (!strcmp(_word[4].c_str(), "x"))
			_input_data_window_x_exists = false;
		else if (!strcmp(_word[4].c_str(), "y"))
			_input_data_window_y_exists = false;
		else {
			demonstrate_command_usage();
			err("In this context, the fifth word must be \"x\" or \"y\".");
			return false;
		}
		break;
	case 7:
		if (!strcmp(_word[4].c_str(), "x")) {
			if (!getdnum(_word[5].c_str(), &minval)) {
				demonstrate_command_usage();
				err("Can't read x window .min.");
				return false;
			}
			if (!getdnum(_word[6].c_str(), &maxval)) {
				demonstrate_command_usage();
				err("Can't read x window .max.");
				return false;
			}
			if (minval == maxval) {
				demonstrate_command_usage();
				err("Can't have input data window of zero width.");
				return false;
			}
			if (minval < maxval) {
				_input_data_window_x_min = minval;
				_input_data_window_x_max = maxval;
			} else {
				_input_data_window_x_min = maxval;
				_input_data_window_x_max = minval;
			}
			_input_data_window_x_exists = true;
		} else if (!strcmp(_word[4].c_str(), "y")) {
			if (!getdnum(_word[5].c_str(), &minval)) {
				demonstrate_command_usage();
				err("Can't read y window .min.");
				return false;
			}
			if (!getdnum(_word[6].c_str(), &maxval)) {
				demonstrate_command_usage();
				err("Can't read y window .max.");
				return false;
			}
			if (minval == maxval) {
				demonstrate_command_usage();
				err("Can't have input data window of zero width.");
				return false;
			}
			if (minval < maxval) {
				_input_data_window_y_min = minval;
				_input_data_window_y_max = maxval;
			} else {
				_input_data_window_y_min = maxval;
				_input_data_window_y_max = minval;
			}
			_input_data_window_y_exists = true;
		} else {
			demonstrate_command_usage();
			err("In this context, the fifth word must be \"x\" or \"y\".");
			return false;
		}
		break;
	default:
		demonstrate_command_usage();
		err("Can't understand command.");
		return false;
	}
	return true;
}

bool
set_contour_formatCmd()
{
	if (_nword < 4) {
		err("Must specify a format for `set contour format'");
		return false;
	}
	if (!strcmp(_word[3].c_str(), "default")) {
		_contourFmt.assign(CONTOUR_FMT_DEFAULT);
	} else {
		char tmp[100];
		if (*_word[3].c_str() == '"') {
			strcpy(tmp, _word[3].c_str() + 1);
			if (tmp[-1 + strlen(tmp)] == '"')
				tmp[-1 + strlen(tmp)] = '\n';
		} else {
			strcpy(tmp, _word[3].c_str());
		}
		_contourFmt.assign(tmp);
	}
	return true;
}

// `set contour label position {.start_cm. .between_cm.}|default|centered'
// 0       1     2               3         4   5  6
// `set contour label for lines exceeding .x. cm'
// 0       1     2   3     4         5   6  7
bool
set_contour_labelCmd()
{
	extern double   _contour_space_first, _contour_space_later, _contour_minlength;	// <-> draw.c
	extern bool     _contour_space_centered;	// <-> draw.c
	if (word_is(3, "for")) {
		// `set contour label for lines exceeding .x. cm'
		Require(_nword == 8, NUMBER_WORDS_ERROR);
		Require(getdnum(_word[6].c_str(), &tmp), READ_WORD_ERROR(".x."));
		_contour_minlength = tmp;
	} else {
		// `set contour label position ...'
		switch (_nword) {
		case 5:
			if (word_is(4, "default")) {
				_contour_space_first = -1.0;
				_contour_space_later = -1.0;
			} else if (word_is(4, "centered") || word_is(4, "centred")) {
				_contour_space_centered = true;
			} else {
				demonstrate_command_usage();
				err("Expecting `default', not '\\", _word[4].c_str(), "'", "\\");
				return false;
			}
			break;
		case 6:
			Require(getdnum(_word[4].c_str(), &tmp), READ_WORD_ERROR(".start_cm."));
			Require(getdnum(_word[5].c_str(), &tmp2), READ_WORD_ERROR(".between_cm."));
			_contour_space_first = tmp;
			_contour_space_later = tmp2;
			_contour_space_centered = false;
			break;
		default:
			demonstrate_command_usage();
			err("Can't understand command.");
			return false;
		}
	}
	return true;
}
#endif

bool
set_contour_label_positionCmd()
{
	extern double   _contour_space_first, _contour_space_later;
	_contour_space_first = cmd_parameter_n(0);
	_contour_space_later = cmd_parameter_n(1);
	return true;
}

#if 0
// `set contour labels rotated|horizontal'
// `set contour labels whiteunder|nowhiteunder'
// 0       1      2                  3 
bool
set_contour_labelsCmd()
{
	extern bool     _contour_label_rotated;	// <-> draw.c startup.c
	extern bool     _contour_label_whiteunder;	// <-> grcntour.c startup.c
	Require(_nword == 4, NUMBER_WORDS_ERROR);
	if (word_is(3, "rotated"))
		_contour_label_rotated = true;
	else if (word_is(3, "horizontal"))
		_contour_label_rotated = false;
	else if (word_is(3, "whiteunder"))
		_contour_label_whiteunder = true;
	else if (word_is(3, "nowhiteunder"))
		_contour_label_whiteunder = false;
	else {
		err("Last word of `set contour labels' must be `rotated|horizontal|whiteunder|nowhiteunder'");
		return false;
	}
	return true;
}
#endif

// Flags for `set flag'
#define NFLAG 100
typedef struct {
	GriString name;
	int value;
} FLAG;
FLAG flag[NFLAG];
unsigned int num_flags = 0;

bool
set_flagCmd()                   //# $name [on|off]
{
	int num_params = cmd_parameters();
	bool turn_on = true;
	if (cmd_parameter_is_n(0)) {
		err("`set flag' requires flag name to be character, but got `\\",
		    dtoa(cmd_parameter_n(0)), "'.", "\\");
		return false;
	}
	if (num_params == 2) {
		if (cmd_parameter_is_n(1)) {
			err("`set flag' requires flag status of `on' or `off' but got `\\",
			    dtoa(cmd_parameter_n(1)), "'.", "\\");
			return false;
		}
		if (strEQ(cmd_parameter_s(1), "on"))
			turn_on = true;
		else if (strEQ(cmd_parameter_s(1), "off"))
			turn_on = false;
		else {
			err("`set flag' requires flag status of `on' or `off' but got `\\",
			    dtoa(cmd_parameter_n(1)), "'.", "\\");
			return false;
		}
	}

	unsigned int i;
	string flagname(cmd_parameter_s(0));

	for (i = 0; i < num_flags; i++) {
		if (strEQ(flagname.c_str(), flag[i].name.c_str())) {
			flag[i].value = turn_on;
			return true;
		}
	}
	// It is a new flag
	if (i >= NFLAG) OUT_OF_MEMORY; 
	num_flags = i;
	(flag[num_flags].name).assign(flagname);
	flag[num_flags].value = turn_on;
	num_flags++;
	return true;
}

int
get_flag(const char *name)
{
	for (unsigned int i = 0; i < num_flags; i++)
		if (!strcmp(flag[i].name.c_str(), name))
			return flag[i].value;
	return 0;
}
#if 0
void
show_flags()
{
	extern char _grTempString[];
	if (num_flags) { 
		for (unsigned int i = 0; i < num_flags; i++) {
			sprintf(_grTempString, "Flag `%s' is %d\n",
				flag[i].name.c_str(), flag[i].value);
			gr_textput(_grTempString);
		}
	} else {
		gr_textput("No flags exist\n");
	}
}

// set error action ...
bool
set_error_actionCmd()
{
	// set error action to core dump
	//   0     1      2  3    4    5
	if (_nword == 6 
	    && word_is(2, "action")
	    && word_is(3, "to")  
	    && word_is(4, "core") 
	    && word_is(5, "dump")) {
		_error_action = 1;
		return true;
	} else {
		demonstrate_command_usage();
		err("Wrong syntax");
		return false;
	}
}
#endif

bool
set_graylevelCmd()
{
	double brightness = cmd_parameter_n(0);
	if (brightness < 0.0) {
		warning("Clipping graylevel to 0 (black)");
		brightness = 0;
	} else if (brightness > 1.0) {
		warning("Clipping graylevel to 1 (white)");
		brightness = 1;
	}
	put_var("$_graylevel", brightness);
	put_var("$_red", brightness);
	put_var("$_green", brightness);
	put_var("$_blue", brightness);
	GriColor c;
	c.setRGB(brightness, brightness, brightness);
	_griState.set_color_line(c);
	if (_griState.separate_text_color() == false)
		_griState.set_color_text(c);
	return true;
}

#if 0
// If x is very close to xmin or xmax, assign to same
double
tweak_to_limit(double x, double xmin, double xmax)
{
	double          range = fabs(xmax - xmin);
	if (fabs(x - xmin) < range / 1.0e4)
		return xmin;
	if (fabs(x - xmax) < range / 1.0e4)
		return xmax;
	return x;
}
#endif

// hsv|rgb $h|$r $s|$g $v|$b $im_low hsv|rgb $h|$r $s|$g $v|$b $im_high [increment $inc]
bool
set_image_colorscaleCmd()
{
	if (!image_range_exists()) {
		err("Must `set image range' before `set image colorscale'.");
		return false;
	}
	int num_params = cmd_parameters();
	bool gave_increment = false;
	int levels = 1;		// stop compiler warning
	double inc = 0.0;	// stop compiler warning
	if (num_params == 10) {
		;
	} else if (num_params == 12) {
		if (strNE(cmd_parameter_s(10), "increment")) {
			err("`set image colorscale' expecting `increment' but found",
			    cmd_parameter_s(10));
			return false;
		}
		inc = cmd_parameter_n(11);
		gave_increment = true;
	} else {
		err("`set image colorscale' takes 10 or 12 parameters, not ",
		    dtoa(double(cmd_parameters())), " as given");
		return false;
	}
	if (cmd_parameter_is_n(0)) {
		err("Fourth word of `set image colorscale' must be `hsv' or `hsb'.");
		return false;
	}
	if (cmd_parameter_is_n(5)) {
		err("Ninth word of `set image colorscale' must be `hsv' or `hsb'.");
		return false;
	}
	bool used_rgb = false;
	double hA=0.0, sA=0.0, vA=0.0, rA=0.0, gA=0.0, bA=0.0, valA=0.0; // A: hsv/rgb, value
	if (strEQ(cmd_parameter_s(0), "hsv")) {
		hA = cmd_parameter_n(1);
		sA = cmd_parameter_n(2);
		vA = cmd_parameter_n(3);
		used_rgb = false;
	} else if (strEQ(cmd_parameter_s(0), "rgb")) {
		rA = cmd_parameter_n(1);
		gA = cmd_parameter_n(2);
		bA = cmd_parameter_n(3);
		used_rgb = true;
	} else {
		err("Fourth word of `set image colorscale' must be `hsv' or `hsb', not `", cmd_parameter_s(0), "' as given");
		return false;
	}
	valA = cmd_parameter_n(4);
	double hB=0.0, sB=0.0, vB=0.0, rB=0.0, gB=0.0, bB=0.0, valB=0.0; // B: hsv/rgb, value
	if (strEQ(cmd_parameter_s(5), "hsv")) {
		if (used_rgb) {
			err("Cannot mix `rgb' and `hsv' for `set image colorscale'");
			return false;
		}
		hB = cmd_parameter_n(6);
		sB = cmd_parameter_n(7);
		vB = cmd_parameter_n(8);
		used_rgb = false;
	} else if (strEQ(cmd_parameter_s(5), "rgb")) {
		rB = cmd_parameter_n(6);
		gB = cmd_parameter_n(7);
		bB = cmd_parameter_n(8);
		used_rgb = true;
	} else {
		err("Fourth word of `set image colorscale' must be `hsv' or `hsb', not `", cmd_parameter_s(0), "' as given");
		return false;
	}
	valB = cmd_parameter_n(9);

	// Ensure endpoints ar distinct
	if (valA == valB) {
		err("Can't have equal image values at endpoints of `set colorscale'");
		return false;
	}
	double delta_image=0.0;	// stop compiler warning
	if (gave_increment) {
		if (!well_ordered(valA, valB, inc, inc))
			inc = -inc;
		if (!gr_multiple(fabs(valB - valA), fabs(inc), fabs(0.001 * inc))) {
			err("\
`set image colorscale ... requires range/increment to be integral to 0.1%");
			return false;
		}
		delta_image = 255.0 * fabs(inc / (valA - valB));
		levels = (int) floor(1.0e-5 + fabs((valA - valB) / inc));
	}


	// Calculate the transforms.  NB: indexA and indexB are of the
	// indices of transform endpoints.  Do RGB and HSV separately, each
	// interpolating linearly in its own space.  (NB: a nonlinear transform
	// links RGB and HSV, so linear movement between equivalent endpoints in
	// these two spaces yields nonequivalent values.)
	//
	double indexA = floor(255.0 * (valA - _image0) / (_image255 - _image0));
	double indexB = floor(255.0 * (valB - _image0) / (_image255 - _image0));

	if (used_rgb) {
		// Used rgb
		double           delta_r = delta_image * fabs(rA - rB);
		double           delta_g = delta_image * fabs(gA - gB);
		double           delta_b = delta_image * fabs(bA - bB);
		// For each target imageTransform[], make a clipped, linear blend
		// from (rA,gA,bA)  to (rB,gB,bB).  Possibly pass this linear blend
		// to 'quantize', to make it into a staircase function.
		for (unsigned int i = 0; i < 256; i++) {
			double r = 255.0
				* (rA + (rB - rA) * pin0_1((i - indexA) / (indexB - indexA)));
			double g = 255.0 *
				(gA + (gB - gA) * pin0_1((i - indexA) / (indexB - indexA)));
			double b = 255.0 *
				(bA + (bB - bA) * pin0_1((i - indexA) / (indexB - indexA)));
			if (gave_increment) {
				r = quantize(r, levels, delta_r);
				g = quantize(g, levels, delta_g);
				b = quantize(b, levels, delta_b);
			}
			_imageTransform[i] = (unsigned char) pin0_255(r);
			_imageTransform[i + 256] = (unsigned char) pin0_255(g);
			_imageTransform[i + 512] = (unsigned char) pin0_255(b);
		}
		// Ensure that endpoints match (rgb)A or (rgb)B, as appropriate.
		if (indexA < indexB) {
			_imageTransform[0] = (unsigned char) (255.0 * rA);
			_imageTransform[256] = (unsigned char) (255.0 * gA);
			_imageTransform[512] = (unsigned char) (255.0 * bA);
			_imageTransform[0 + 255] = (unsigned char) (255.0 * rB);
			_imageTransform[256 + 255] = (unsigned char) (255.0 * gB);
			_imageTransform[512 + 255] = (unsigned char) (255.0 * bB);
		} else {
			_imageTransform[0] = (unsigned char) (255.0 * rB);
			_imageTransform[256] = (unsigned char) (255.0 * gB);
			_imageTransform[512] = (unsigned char) (255.0 * bB);
			_imageTransform[0 + 255] = (unsigned char) (255.0 * rA);
			_imageTransform[256 + 255] = (unsigned char) (255.0 * gA);
			_imageTransform[512 + 255] = (unsigned char) (255.0 * bA);
		}
	} else {
		// Used hsv
		double           delta_h = delta_image * fabs(hA - hB);
		double           delta_s = delta_image * fabs(sA - sB);
		double           delta_v = delta_image * fabs(vA - vB);
		// For each target imageTransform[], make a clipped, linear blend
		// from (hA,sA,vA)  to (hB,sB,brB).  Possibly pass this linear blend
		// to 'quantize', to make it into a staircase function.
		for (unsigned int i = 0; i < 256; i++) {
			double h, s, v, r, g, b;
			h = (hA + (hB - hA) * pin0_1((i - indexA) / (indexB - indexA)));
			s = (sA + (sB - sA) * pin0_1((i - indexA) / (indexB - indexA)));
			v = (vA + (vB - vA) * pin0_1((i - indexA) / (indexB - indexA)));
			if (gave_increment) {
				// Quantize HSV *before* conversion to RGB
				h = quantize(255.0 * h, levels, delta_h) / 255.0;
				s = quantize(255.0 * s, levels, delta_s) / 255.0;
				v = quantize(255.0 * v, levels, delta_v) / 255.0;
			}
			gr_hsv2rgb(h, s, v, &r, &g, &b);
			_imageTransform[i] = (unsigned char) pin0_255(255.0 * r);
			_imageTransform[i + 256] = (unsigned char) pin0_255(255.0 * g);
			_imageTransform[i + 512] = (unsigned char) pin0_255(255.0 * b);
		}
		// Ensure that endpoints match (rgb)A or (rgb)B, as appropriate.
		gr_hsv2rgb(hA, sA, vA, &rA, &gA, &bA);
		gr_hsv2rgb(hB, sB, vB, &rB, &gB, &bB);
		if (indexA < indexB) {
			_imageTransform[0] = (unsigned char) (255.0 * rA);
			_imageTransform[256] = (unsigned char) (255.0 * gA);
			_imageTransform[512] = (unsigned char) (255.0 * bA);
			_imageTransform[0 + 255] = (unsigned char) (255.0 * rB);
			_imageTransform[256 + 255] = (unsigned char) (255.0 * gB);
			_imageTransform[512 + 255] = (unsigned char) (255.0 * bB);
		} else {
			_imageTransform[0] = (unsigned char) (255.0 * rB);
			_imageTransform[256] = (unsigned char) (255.0 * gB);
			_imageTransform[512] = (unsigned char) (255.0 * bB);
			_imageTransform[0 + 255] = (unsigned char) (255.0 * rA);
			_imageTransform[256 + 255] = (unsigned char) (255.0 * gA);
			_imageTransform[512 + 255] = (unsigned char) (255.0 * bA);
		}
	}
	// All done.  Everything was OK.
	_imageTransform_exists = true;
	_image_color_model = rgb_model;
	printf("OK- end of 'set image colorscale'\n");
	return true;
}

#if 0
bool
set_image_grayscaleCmd()
{
	int             i;
	double          indexA;
	if (_nword < 3) {
		err("`set image' what?");
		return false;
	}
	// Do a (redundant?) check
	if (!word_is(2, "grayscale") && !word_is(2, "greyscale")) {
		err("`set image' what?");
		return false;
	}
	if (!image_range_exists()) {
		err("First `set image range'");
		return false;
	}
	// `set image grayscale'
	if (_nword == 3) {
		for (i = 0; i < 256; i++)
			_imageTransform[i] = (unsigned char) i;
		_imageTransform_exists = true;
		_image_color_model = bw_model;
		return true;
	}
	// `set image grayscale [black .bl. white .wh. [increment .inc.]]'
	if (_nword > 3
	    && (!strcmp(_word[3].c_str(), "black")
		|| !strcmp(_word[3].c_str(), "white")
		|| !strcmp(_word[3].c_str(), "increment"))) {
		double          inc = 0.0, valB, valA, scale;
		bool            gave_increment = false;
		if (1 == get_cmd_values(_word, _nword, "black", 1, _dstack))
			valA = _dstack[0];
		else {
			err("Can't read .bl. in [white .wh. black .bl.]");
			return false;
		}
		if (1 == get_cmd_values(_word, _nword, "white", 1, _dstack))
			valB = _dstack[0];
		else {
			err("Can't read .wh. in [white .wh. black .bl.]");
			return false;
		}
		if (1 == get_cmd_values(_word, _nword, "increment", 1, _dstack)) {
			inc = _dstack[0];
			gave_increment = true;
		}
		indexA = 255.0 * (valA - _image0) / (_image255 - _image0);
		scale = (_image255 - _image0) / (valB - valA);
		if (gave_increment) {
			// Set up quantized gray levels.  For example, `set image
			// grayscale white -1 black 1 increment 0.5' will set up 4 gray
			// levels; thus, _imageTransform[] will be set up with 4 bands,
			// containing the distinct values (1/5, 2/5, 3/5, 4/5) * 255.
			//
			// BUG -- this quantized graylevel code has had lots of bugs
			double          delta_image;	// image change per level
			int             levels;	// number of levels
			if (!well_ordered(valA, valB, inc, inc))
				inc = -inc;
			if (!gr_multiple(valB - valA, inc, 0.001 * inc)) {
				err("\
`set image grayscale black .bl. white .wh. increment .inc.'\n\
  has (.wh. - .bl.) not a multiple of .inc. to within 0.1%");
				return false;
			}
			if (valA == valB) {
				err("\
`set image grayscale black .bl. white .wh. increment .inc.'\n\
  has .wh. = .bl., which is not allowed.");
				return false;
			}
			delta_image = 255.0 * fabs(inc / (valB - valA));
			levels = (int) floor(1.0e-5 + fabs((valB - valA) / inc));
			for (i = 0; i < 256; i++) {
				// The .001 below is to prevent rounding problems.
				_imageTransform[i] = (unsigned char)
					floor(pin0_255((int) floor(.001 + quantize(scale * (i - indexA), levels, delta_image))));
			}
			// BUG -- the following is a total kludge, because I could not
			// find an adequate rounding macro
			_imageTransform[0] = (_imageTransform[0] <= 128) ? 0 : 255;
			_imageTransform[255] = (_imageTransform[255] <= 128) ? 0 : 255;
			_imageTransform_exists = true;
			_image_color_model = bw_model;
			return true;
		} else {
			// Increment was not given.
			for (i = 0; i < 256; i++) {
				_imageTransform[i]
					= (unsigned char) floor(pin0_255((int) floor(scale * (i - indexA))));
			}
		}
		_imageTransform_exists = true;
		_image_color_model = bw_model;
		return true;
	}
	demonstrate_command_usage();
	err("Can't understand command.");
	return false;
}

bool
set_image_grayscale_using_hist()
{
	int             i;
	double          sum = 0.0, range;
	double          wh, bl;
	if (!image_range_exists()) {
		err("First `set image range'");
		return false;
	}
	// Since parser got to this point, must at least match `set image
	// grayscale using histogram'
	if (_nword == 5) {
		// `set image grayscale using histogram'
		calculate_image_histogram();
		// The range is sum(255) - sum(0), but sum(255) is unity, because
		// _imageHist[] is defined that way, and sum(0) is _imageHist[0].
		range = 1.0 - _imageHist[0];
		if (range == 0.0)
			range = 1.0;
		for (i = 0; i < 256; i++) {
			_imageTransform[i] = (unsigned char) floor(pin0_255(255.0 * (1.0 - (sum - _imageHist[0]) / range)));
			sum += _imageHist[i];
		}
		_imageTransform_exists = true;
	} else if (_nword == 9) {
		// `set image grayscale using histogram black .bl. white .wh.'
		int             start, end;
		double          sum_to_end = 0.0, sum_to_start = 0.0;
		if (1 == get_cmd_values(_word, _nword, "black", 1, _dstack)) {
			bl = _dstack[0];
		} else {
			err("Can't read .bl. in [black .bl. white .wh.]");
			return false;
		}
		if (1 == get_cmd_values(_word, _nword, "white", 1, _dstack)) {
			wh = _dstack[0];
		} else {
			err("Can't read .wh. in [black .bl. white .wh.]");
			return false;
		}
		calculate_image_histogram();
		start = value_to_image(wh);	// pixel
		end = value_to_image(bl);	// pixel
		for (i = 0; i < start; i++)
			sum_to_start += _imageHist[i];
		for (i = 0; i < end; i++)
			sum_to_end += _imageHist[i];
		range = sum_to_end - sum_to_start;
		if (range == 0.0)
			range = 1.0;
		for (i = 0; i < 256; i++) {
			_imageTransform[i] = (unsigned char) floor(pin0_255(255.0 * (1.0 - (sum - sum_to_start) / range)));
			sum += _imageHist[i];
		}
		_imageTransform_exists = true;
	} else {
		demonstrate_command_usage();
		err("Can't understand command.");
		return false;
	}
	_image_color_model = bw_model;
	return true;
}

// `set image missing value color to white|black|{graylevel .brightness.}'
bool
set_image_missingCmd()
{
	if (!image_range_exists()) {
		err("First `set image range'");
		return false;
	}
	if (_nword == 7) { // `set image missing value color to white|black'
		if (word_is(6, "white")) {
			_image_missing_color_red = 1.0;
			_image_missing_color_green = 1.0;
			_image_missing_color_blue = 1.0;
		} else if (word_is(6, "black")) {
			_image_missing_color_red = 0.0;
			_image_missing_color_green = 0.0;
			_image_missing_color_blue = 0.0;
		} else {
			demonstrate_command_usage();
			err("Unknown color specified for image missing-value.");
			return false;
		}
	} else if (_nword == 10) {
		// `set image missing value color to rgb .red. .green. .blue.'
		if (!strcmp(_word[6].c_str(), "rgb")) {
			double red, green, blue;
			Require(getdnum(_word[7].c_str(), &red), READ_WORD_ERROR(".red."));
			Require(getdnum(_word[8].c_str(), &green), READ_WORD_ERROR(".green."));
			Require(getdnum(_word[9].c_str(), &blue), READ_WORD_ERROR(".blue."));
			// Clip if necessary
			CHECK_RGB_RANGE(red);
			CHECK_RGB_RANGE(green);
			CHECK_RGB_RANGE(blue);
			_image_missing_color_red = red;
			_image_missing_color_green = green;
			_image_missing_color_blue = blue;
		}
	} else {
		// `set image missing value color to {graylevel .brightness.}'
		if (1 == get_cmd_values(_word, _nword, "graylevel", 1, _dstack)
		    || 1 ==get_cmd_values(_word, _nword, "greylevel", 1, _dstack)) {
			if (_dstack[0] < 0.0) {
				warning("`set image missing value color': clipping graylevel to 0.0");
				_image_missing_color_red = 0.0;
				_image_missing_color_green = 0.0;
				_image_missing_color_blue = 0.0;
			} else if (_dstack[0] > 1.0) {
				warning("`set image missing value color': clipping graylevel to 0.0");
				_image_missing_color_red = 1.0;
				_image_missing_color_green = 1.0;
				_image_missing_color_blue = 1.0;
			} else {
				_image_missing_color_red = _dstack[0];
				_image_missing_color_green = _dstack[0];
				_image_missing_color_blue = _dstack[0];
			}
		} else {
			demonstrate_command_usage();
			err("Can't understand color specification.");
			return false;
		}
	}
	return true;
}
#endif

bool
set_image_rangeCmd()
{
	extern double _image0, _image255; // in image.cc
	_image0 = cmd_parameter_n(0);
	_image255 = cmd_parameter_n(1);
	//printf("set_image_range _image0 %f   _image255 %f\n",_image0,_image255);
	return true;
}
			
#if 0
// `set grid missing inside|outside       curve'
// `set grid missing  above|below   .intercept. .slope'
//    0    1       2            3             4      5
bool
set_grid_missingCmd()
{
	double intercept, slope;
	typedef enum { above, below, inside, outside, dont_know } WHERE;
	WHERE where = dont_know;
	switch(_nword) {
	case 5:
		if (word_is(3, "inside"))
			where = inside;
		else if (word_is(3, "outside"))
			where = outside;
		break;
	case 6:
		if (!getdnum(_word[4].c_str(), &intercept))
			return false;
		if (!getdnum(_word[5].c_str(), &slope))
			return false;
		if (word_is(3, "above"))
			where = above;
		else if (word_is(3, "below"))
			where = below;
		break;
	default:
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	if (!grid_exists())
		return false;
	unsigned int i, j;
	int nx, ny;
	switch (where) {
	case above:
		nx = _xmatrix.size();
		ny = _ymatrix.size()
			for (i = 0; i < nx; i++)
				for (j = 0; j < ny; j++)
					if (_ymatrix[j] > intercept + slope * _xmatrix[i])
						_legit_xy(i, j) = false;
		return true;		// ok
	case below:
		nx = _xmatrix.size();
		ny = _ymatrix.size()
			for (i = 0; i < nx; i++)
				for (j = 0; j < ny; j++)
					if (_ymatrix[j] < intercept + slope * _xmatrix[i])
						_legit_xy(i, j) = false;
		return true;		// ok
	case inside:
		return set_grid_missing_curve(true);
	case outside:
		return set_grid_missing_curve(false);
	default:
		demonstrate_command_usage();
		err("Fourth word must be `above', `below', `inside' or `outside'");
		return false;
	}
}

bool
set_grid_missing_curve(bool inside)
{
	if (!inside) {
		err("'set grid missing outside curve' not allowed");
		return false;
	}
	unsigned int i, start = 0;
	for (i = 0; i < _colX.size(); i++) {
		if (gr_missingx(_colX[i]) 
		    || gr_missingy(_colY[i])
		    || i > _colX.size() - 1) {
			if (!mask_an_island(_colX.begin() + start,
					    _colY.begin() + start, i - start))
				return false;	// problem
			while ((gr_missingx(_colX[i]) || gr_missingy(_colY[i]))
			       && i < _colX.size())
				i++;
			start = i;
		}
	}
	if (i <= _colX.size()) {
		mask_an_island(_colX.begin() + start,
			       _colY.begin() + start, 
			       i - start);
	}
	return true;
}

bool
mask_an_island(double *x, double *y, unsigned int n)
{
	unsigned right_edge = 0;
	double xmax = x[0];
	unsigned int i;
	for (i = 1; i < n; i++) {
		if (x[i] > xmax) {
			xmax = x[i];
			right_edge = i;
		}
	}
//DEBUG printf("\nIsland:\n");
//DEBUG for (i = 0; i < n; i++) {
//DEBUG     printf("    %f %f\n", x[i], y[i]);
//DEBUG }
//DEBUG printf("Rotates to:\n");
//DEBUG for (i = 0; i < n; i++) {
//DEBUG     int ii = (i + right_edge) % n;
//DEBUG     printf("    %f %f\n", x[ii], y[ii]);
//DEBUG }
	for (i = 0; i < n; i++) {
		int ii = (i + right_edge) % n;
		for (unsigned int ixm = 0; ixm < _xmatrix.size(); ixm++) {
			if (between(_xmatrix[ixm], x[(ii+1)%n], x[ii])) {
//DEBUG printf("mask out ixm=%d at %f, bracketed by %f - %f\n", ixm, _xmatrix[ixm], x[(ii+1)%n],x[ii]);
				for (int iym = (int)_num_ymatrix_data - 1; iym > -1; iym--) {
					if (_ymatrix[iym] <=
					    interpolate_linear(_xmatrix[ixm], 
							       x[ii], y[ii], 
							       x[(ii+1)%n], y[(ii+1)%n])) {
						// Reverse things below
//DEBUG printf("reverse below y(%d)=%f\n", iym, _ymatrix[iym]);
						while (iym > -1) {
							_legit_xy(ixm, iym) = _legit_xy(ixm, iym) == true
								? false : true;
							iym--;
						}
//DEBUG printf("i=%d; grid=\n", i); show_grid_maskCmd();
						break;	// just in case
					}
				}
			}
		}
	}
	return true;
}

#endif
static bool
width_rapidograph(const char *s, double *w)
{
// Named pen sizes, following the notation of Rapidograph(TM)
// technical drawing pens.
#define NUM_RAPIDOGRAPH 16
	typedef struct {
		const char *name;		// allow both 'X' and 'x' in names
		double      width;		// in points
	} r;
	r r_table[NUM_RAPIDOGRAPH] = {
		{"6x0", 0.369},
		{"6X0", 0.369},
		{"4x0", 0.510},
		{"4X0", 0.510},
		{"3x0", 0.709},
		{"3X0", 0.709},
		{"00",  0.850},
		{"0",   0.992},
		{"1",   1.417},
		{"2",   1.701},
		{"2.5", 1.984},
		{"3",   2.268},
		{"3.5", 2.835},
		{"4",   3.402},
		{"6",   3.969},
		{"7",   5.669}
	};
	for (int i = 0; i < NUM_RAPIDOGRAPH; i++)
		if (strEQ(s, r_table[i].name)) {
			*w = r_table[i].width;
			return true;
		}
	err("Unknown rapidograph pen size `\\", s, "' requested.\n       Permitted sizes: 6x0, 4x0, 3x0, 00, 0, 1, 2, 2.5, 3, 3.5, 4, 6, 7", "\\");
	return false;
}

// ($$) { # {square butt}|{round overhang}|{square overhang}
bool
set_line_capCmd()
{
	if (cmd_parameter_is_n(0)) {
		fatal_error("`set line cap' requires first argument to be one of `square' or `round', not a numerical value as given");
		return false;
	}
	if (cmd_parameter_is_n(1)) {
		fatal_error("`set line cap' requires second argument to be one of `butt' or `overhang', not a numerical value as given");
		return false;
	}
	const char *s0 = cmd_parameter_s(0);
	const char *s1 = cmd_parameter_s(1);
	if (strEQ(s0, "square")) {
		if (strEQ(s1, "butt")) {
			_griState.set_line_cap(0);
		} else if (strEQ(s1, "overhang")) {
			_griState.set_line_cap(2);
		} else {
			fatal_error("`set line cap square' requires argument to be one of `butt' or `overhang', not `", s1, "' as given");
			return false;
		}
	} else if (strEQ(s0, "round")) {
		if (strNE(s1, "overhang")) {
			fatal_error("`set line cap round' requires argument to be `overhang', not `", s1, "' as given");
			return false;
		}
		_griState.set_line_cap(1);
	}
	return true;
}

// ($) { # mitered | round | bevelled
bool
set_line_joinCmd()
{
	if (cmd_parameter_is_n(0)) {
		fatal_error("`set line join' requires argument to be one of `mitered', `round' or `bevelled', not a numerical value as given");
		return false;
	}
	const char *opt = cmd_parameter_s(0);
	if (strEQ(opt, "mitered")) {
		_griState.set_line_join(0);
	} else if (strEQ(opt, "round")) {
		_griState.set_line_join(1);
	} else if (strEQ(opt, "bevelled")) {
		_griState.set_line_join(2);
	} else {
		void fatal_error(const char *msg, const char *msg2, const char *msg3);
		fatal_error("`set line join' requires argument to be one of `mitered', `round' or `bevelled', not `", opt, "' as given");
		return false;
	}
	return true;
}

// ($;$$) [axis|symbol|all] $width|"default"|{rapidograph $name}
bool
set_line_widthCmd()
{
	unsigned int num_params = cmd_parameters();
	double          w;		// the width, in pt
	unsigned int start = 0;
	bool first_is_string = !cmd_parameter_is_n(0);
	typedef enum {axis, symbol, all, not_given} WHICH;
	WHICH which = not_given;
	if (first_is_string && strEQ(cmd_parameter_s(0), "axis")) {
		which = axis;	start++;
	} else if (first_is_string && strEQ(cmd_parameter_s(0), "symbol")) {
		which = symbol;	start++;
	} else if (first_is_string && strEQ(cmd_parameter_s(0), "all")) {
		which = all;	start++;
	}
	//printf("DEBUG: start=%d\n",start);
	if (cmd_parameter_is_n(start)) {
		//printf("NUMERICAL.  start=%d, cmdparameters=%d\n",start,num_params);
		w = cmd_parameter_n(start);
		switch (which) {
		case axis:
			_griState.set_linewidth_axis(w);
			put_var("$_linewidth_axis", w);
			break;
		case symbol:
			_griState.set_linewidth_symbol(w);
			put_var("$_linewidth_symbol", w);
			break;
		case all:
			_griState.set_linewidth_axis(w);
			put_var("$_linewidth_axis", w);
			_griState.set_linewidth_symbol(w);
			put_var("$_linewidth_symbol", w);
			_griState.set_linewidth_line(w);
			put_var("$_linewidth", w);
			break;
		case not_given:
			_griState.set_linewidth_line(w);
			put_var("$_linewidth", w);
			break;
		default:
			break;
		}
	} else {
		//printf("DEBUG: start=%d; this is string '%s'\n",start,cmd_parameter_s(start));
		if (strEQ(cmd_parameter_s(start), "default")) {
			//printf("DEBUG: setting linewidth to default\n");
			switch (which) {
			case axis:
				w = LINEWIDTHAXIS_DEFAULT;
				_griState.set_linewidth_axis(w);
				put_var("$_linewidth_axis", w);
				break;
			case symbol:
				w = LINEWIDTHSYMBOL_DEFAULT;
				_griState.set_linewidth_symbol(w);
				put_var("$_linewidth_symbol", w);
				break;
			case all:
				w = LINEWIDTH_DEFAULT;
				_griState.set_linewidth_axis(w);
				put_var("$_linewidth_axis", w);
				_griState.set_linewidth_symbol(w);
				put_var("$_linewidth_symbol", w);
				_griState.set_linewidth_line(w);
				put_var("$_linewidth", w);
				break;
			case not_given:
				w = LINEWIDTH_DEFAULT;
				_griState.set_linewidth_line(w);
				put_var("$_linewidth", w);
				break;
			default:
				break;
			}
		} else if (strEQ(cmd_parameter_s(start), "rapidograph")) {
			//printf("DEBUG: rapidograph.  start=%d total=%d the char '%c'\n",start,num_params,*cmd_parameter_s(start+2));
			if (start != num_params - 2) {
				// Catch common problem -- not enclosing e.g. name in quotes, so that
				// e.g. parser breaks up '3x0' into '3' and 'x0'.
				if (start == num_params - 3 
				    && ('x' == *cmd_parameter_s(start + 2) || ('X' == *cmd_parameter_s(start + 2)))) {
					err("`set line width' can only have 1 expression following `rapidograph', not two as given.\n       Noticed that last expression was `\\", cmd_parameter_s(start + 2), "' -- did you forget to enclose the pen-name in quotes?", "\\");
				} else{
					err("`set line width' can only have 1 expression following `rapidograph', not \\", dtoa(num_params - 1 - start), " as given", "\\");
				}
				return false;
			}
			if (cmd_parameter_is_n(1 + start)) {
				//printf("DEBUG: converting %f to string\n", cmd_parameter_n(1 + start));
				if (!width_rapidograph(dtoa(cmd_parameter_n(1 + start)), &w))
					return false;
			} else {
				//printf("DEBUG: using string '%s'\n", cmd_parameter_s(1 + start));
				if (!width_rapidograph(cmd_parameter_s(1 + start), &w))
					return false;
			}
			switch (which) {
			case axis:
				_griState.set_linewidth_axis(w);
				put_var("$_linewidth_axis", w);
				break;
			case symbol:
				_griState.set_linewidth_symbol(w);
				put_var("$_linewidth_symbol", w);
				break;
			case all:
				_griState.set_linewidth_axis(w);
				put_var("$_linewidth_axis", w);
				_griState.set_linewidth_symbol(w);
				put_var("$_linewidth_symbol", w);
				_griState.set_linewidth_line(w);
				put_var("$_linewidth", w);
				break;
			case not_given:
				_griState.set_linewidth_line(w);
				put_var("$_linewidth", w);
				break;
			default:
				break;
			}
		} else {
			err("`set line width' expecting word `default' or `rapidograph' but found `\\", cmd_parameter_s(start), "'", "\\");
			return false;
		}
	}
	return true;
}

// set map projection ...as_for_proj...
bool
set_map_projectionCmd()
{
	extern string _proj_command;
	_proj_command = interpolate_var_in_string(cmd_parameter_s(0));
	//printf("DEBUG: should PROJ with cmd '%s'\n", _proj_command.c_str());
	char *copy = strdup(_proj_command.c_str());
	char *words[100];
	int num_words;
	chop_into_words(copy, words, &num_words, 100);
    
	// Interpret some of the '-' commandline args
	// -m flag, e.g.
	//     -m 0.001
	//     -m 1/1000
	//     -m 1:1000
	extern double _proj_scale_factor;
	_proj_scale_factor = 1.0; // in case set before
	for (int i = 0; i < num_words; i++) {
		//printf("check '%s'\n", words[i]);
		if (strEQ(words[i], "-m")) {
			//printf("got -m argument.  next arg '%s'\n", words[i+1]);
			if (i == (num_words - 1)) {
				err("`set map projection' argument -m needs an argument following it");
				return false;
			}
			extern double _proj_scale_factor;
			double m;
			if (1 == sscanf(words[i+1], "1/%lf", &m)) {
				_proj_scale_factor = 1.0 / m;
				//printf("got fractional scalefactor A %f\n", _proj_scale_factor);
			} else if (1 == sscanf(words[i+1], "1:%lf", &m)) {
				_proj_scale_factor = 1.0 / m;
				//printf("got fractional scalefactor B %f\n", _proj_scale_factor);
			} else if (1 == sscanf(words[i+1], "%lf", &m)) {
				_proj_scale_factor = m;
				//printf("got scalefactor %f\n", _proj_scale_factor);
			} else {
				err("`set map projection' cannot read the scale factor after the `-m'");
				return false;
			}
			for (int ii = i; ii < num_words; ii++) 
				words[ii] = words[ii + 2];
			num_words -= 2;
		}
	}
    
#if defined(HAVE_PROJ)
	extern PJ *_proj_pj; // in gre.cc
	if (_gre_debug & DEBUG_OS_CALLS) {
		printf("`set map projection' is about to send these arguments to the \"pj_init\" subroutine of the \"proj\" library\n");
		for (int i = 0; i < num_words; i++)
			printf("\t`%s'\n", words[i]);
		printf("\n");
	}
    
	if (!(_proj_pj = pj_init(num_words, words))) {
		err("`set map projection' had problem with args -- pj_init died\n");
		return false;
	}
#endif

	return true;
}

bool
set_missing_valueCmd()
{
	// `set missing value $_missing_value'
	gr_setmissingvalue(cmd_parameter_n(0));
	put_var("$_missing_value", gr_currentmissingvalue());
	return true;
}
#if 0

bool
set_pageCmd()
{
	double          mag, xcm, ycm;
	Require(_nword > 2, err("`set page' requires parameter(s)"));
	if (!strcmp(_word[2].c_str(), "portrait")) {
		;			// EMPTY
	} else if (!strcmp(_word[2].c_str(), "landscape")) {
		if (!already_landscape) {
			extern FILE *_grPS;
			fprintf(_grPS, "8.5 72 mul 0 translate 90 rotate %% Landscape\n");
			//check_psfile();
		}
		already_landscape = true;
		put_var("$_landscape", 1.0);
		gr_setup_ps_landscape();
	} else if (!strcmp(_word[2].c_str(), "factor")) {
		if (_nword != 4) {
			err("Must specify .mag. in `set page factor .mag.'");
			return false;
		}
		if (!getdnum(_word[3].c_str(), &mag))
			return false;
		if (mag <= 0.0) {
			err("Can't use negative enlargement factor");
			return false;
		}
		gr_setscale(mag, mag);
	} else if (!strcmp(_word[2].c_str(), "translate")) {
		if (_nword != 5) {
			err("Must specify .xcm. and .ycm. in `set page translate .xcm. .ycm.'");
			return false;
		}
		if (!getdnum(_word[3].c_str(), &xcm))
			return false;
		if (!getdnum(_word[4].c_str(), &ycm))
			return false;
		gr_settranslate(xcm, ycm);
	} else {
		err("Unknown `set page' parameter");
		return false;
	}
	return true;
}

bool
set_postscript_filenameCmd()
{
	if (_nword != 4) {
		demonstrate_command_usage();
		NUMBER_WORDS_ERROR;
		return false;
	}
	if (!gr_reopen_postscript(_word[3].c_str())) {
		demonstrate_command_usage();
		warning("Cannot open `\\", _word[3].c_str(), "', so using old name.", "\\");
	}
	return true;
}
#endif

bool
set_pageCmd()
{
	if (cmd_parameter_is_n(0)) {
		err("`set page' parameter must be \"landscape\" or \"portrait\"");
		return false;
	}
	const char *type = cmd_parameter_s(0);
	if (strEQ(type, "portrait")) {
		;			// EMPTY
	} else if (strEQ(type, "landscape")) {
		if (!already_landscape) {
			extern FILE *_grPS;
			fprintf(_grPS, "8.5 72 mul 0 translate 90 rotate %% Landscape\n");
			check_psfile();
		}
		already_landscape = true;
		put_var("$_landscape", 1.0);
		gr_setup_ps_landscape();
	} else {
		err("`set page' parameter must be \"landscape\" or \"portrait\"");
	}
	return true;
}

bool
set_symbol_sizeCmd()
{
	double radius;
	if (cmd_parameter_is_n(0)) {
		radius = cmd_parameter_n(0);
		if (radius < 0.0 || radius > 20.0) {
			err("Ignoring bad symbol size <0 or >20 cm");
			return false;
		}
	} else {
		if (strNE(cmd_parameter_s(0), "default")) {
			err("`set symbol size' expecting word `default' but found `\\", cmd_parameter_s(0), "'", "\\");
			return false;
		}
		radius = SYMBOLSIZE_DEFAULT;
	}
	put_var("$_symbol_size", radius);
	gr_setsymbolsize_cm(radius);
	return true;
}

// `set tic size $length'
bool
set_tic_sizeCmd()
{
	put_var("$_tic_size", cmd_parameter_n(0));
	return true;
}

bool
set_ticsCmd()
{
	if (cmd_parameter_is_n(0)) {
		err("`set tics' requires string argument, not numeric argument `\\",
		    dtoa(cmd_parameter_n(0)), "' as given", "\\");
		return false;
	}
	if (strEQ(cmd_parameter_s(0), "in")) {
		put_var("$_tic_direction", 1.0);
	} else if (strEQ(cmd_parameter_s(0), "out")) {
		put_var("$_tic_direction", 0.0);
	} else {
		err("`set tics' only accepts options `in' and `out'");
		return false;
	}
	return true;
}

// ($;$) { # $cm_per_unit.|{as y}
bool
set_u_scaleCmd()
{
	double scale;
	unsigned int num_params = cmd_parameters();
	switch (num_params) {
	case 1:
		scale = cmd_parameter_n(0);
		if (scale == 0) {
			err("`set u scale 0' illegal");
			return false;
		}
		_cm_per_u = scale;
		_uscale_exists = true;
		break;
	case 2:
		if (cmd_parameter_is_n(0)) {
			err("`set u scale' expecting word `as' but found numerical value \\", dtoa(cmd_parameter_n(0)), " instead", "\\");
			return false;
		}
		if (strNE(cmd_parameter_s(0), "as")) {
			err("`set u scale' expecting word `as', but found word `\\", cmd_parameter_s(0), "'instead", "\\");
			return false;
		} 
		if (cmd_parameter_is_n(1)) {
			err("`set u scale' expecting word `x' but found numerical value \\", dtoa(cmd_parameter_n(1)), " instead", "\\");
			return false;
		}
		if (strNE(cmd_parameter_s(1), "x")) {
			err("`set u scale' expecting word `x', but found word `\\", cmd_parameter_s(1), "'instead", "\\");
			return false;
		} 
		if (!_xscale_exists) {
			err("First `set x axis' or `read columns x ...'");
			return false;
		}
		if (_xtype != gr_axis_LINEAR) {
			err("Can only `set u scale as x' if x is LINEAR");
			return false;
		}
		_cm_per_u = get_var_scalar("$_xsize") / (_xright - _xleft);
		_uscale_exists = true;
		break;
	default:
		err("INTERNAL ERROR IN set_u_scale\n");
		return false;
	}
	//printf("debug: 'set u scale' set _cm_per_u to %f\n", _cm_per_u);
	return true;
}

// ($;$) { # $cm_per_unit.|{as y}
bool
set_v_scaleCmd()
{
	double scale;
	unsigned int num_params = cmd_parameters();
	switch (num_params) {
	case 1:
		scale = cmd_parameter_n(0);
		if (scale == 0) {
			err("`set v scale 0' illegal");
			return false;
		}
		_cm_per_v = scale;
		_vscale_exists = true;
		break;
	case 2:
		if (cmd_parameter_is_n(0)) {
			err("`set v scale' expecting word `as' but found numerical value \\", dtoa(cmd_parameter_n(0)), " instead", "\\");
			return false;
		}
		if (strNE(cmd_parameter_s(0), "as")) {
			err("`set v scale' expecting word `as', but found word `\\", cmd_parameter_s(0), "'instead", "\\");
			return false;
		} 
		if (cmd_parameter_is_n(1)) {
			err("`set v scale' expecting word `y' but found numerical value \\", dtoa(cmd_parameter_n(1)), " instead", "\\");
			return false;
		}
		if (strNE(cmd_parameter_s(1), "y")) {
			err("`set v scale' expecting word `y', but found word `\\", cmd_parameter_s(1), "'instead", "\\");
			return false;
		} 
		if (!_yscale_exists) {
			err("First `set y axis' or `read columns y ...'");
			return false;
		}
		if (_ytype != gr_axis_LINEAR) {
			err("Can only `set v scale as y' if y is LINEAR");
			return false;
		}
		_cm_per_v = get_var_scalar("$_ysize") / (_ytop - _ybottom);
		_vscale_exists = true;
		break;
	default:
		err("INTERNAL ERROR IN set_v_scale\n");
		return false;
	}
	//printf("debug: 'set v scale' set _cm_per_v to %f\n", _cm_per_v);
	return true;
}

#if 0
bool
set_traceCmd()
{
	switch (_nword) {
	case 2:
		put_var("$_trace", 1.0);
		break;
	case 3:
		if (!strcmp(_word[2].c_str(), "on")) {
			put_var("$_trace", 1.0);
		} else if (!strcmp(_word[2].c_str(), "off")) {
			put_var("$_trace", 0.0);
		}
		break;
	default:
		break;
	}
	return true;
}
#endif

bool
well_ordered(double min, double max, double inc_big, double inc_small)
{
	if (min < max)
		return ((inc_big > 0.0 && inc_small > 0.0) ? true : false);
	else
		return ((inc_big < 0.0 && inc_small < 0.0) ? true : false);
}

bool
set_x_axisCmd()
{
	_xatbottom = true;
#if 0
	if (!strcmp(_word[_nword - 1].c_str(), "bottom")) {
		_xatbottom = true;
		if (_nword == 4) {
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		}
		_nword--;
	} else if (!strcmp(_word[_nword - 1].c_str(), "top")) {
		_xatbottom = false;
		if (_nword == 4) {
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		}
		_nword--;
#endif
	// ... specifying x axis
	unsigned int num_params = cmd_parameters();
	if (num_params == 1) {	// 'set x axis decreasing|increasing|unknown'
		if (cmd_parameter_is_n(0)) {
			err("`set x axis' expecting `increasing', `decreasing', or `unknown', not numeric argument `\\", dtoa(cmd_parameter_n(0)), "' as given", "\\");
			return false;
		} else {
			if (strEQ(cmd_parameter_s(0), "increasing")) {
				_xincreasing = true;
				if (_xscale_exists && _xleft > _xright) {
					swap(_xleft, _xright);
					put_var("$_xleft", _xleft);
					put_var("$_xright", _xright);
					_xinc = fabs(_xinc);
				}
				return true;
			} else if (strEQ(cmd_parameter_s(0), "decreasing")) {
				_xincreasing = false;
				if (_xscale_exists && _xleft < _xright) {
					swap(_xleft, _xright);
					put_var("$_xleft", _xleft);
					put_var("$_xright", _xright);
					_xinc = -fabs(_xinc);
				}
				return true;
			} else if (strEQ(cmd_parameter_s(0), "unknown")) {
				_xscale_exists = false;
				_need_x_axis = true;
				_user_set_x_axis = false;
				return true;
			} else {
				err("`set x axis' expecting word `increasing', `decreasing', or `unknown', not `\\", cmd_parameter_s(0), "' as found", "\\");
				return false;
			}
		}
	} else if (num_params == 2) { // 'set x axis $left $right
		double xleft = cmd_parameter_n(0);
		double xright = cmd_parameter_n(1);
		_xleft = xleft;
		_xright = xright;
		if (_xtype == gr_axis_LOG)
			_xinc = 1.0;
		else
			_xinc = _xright - _xleft;
		put_var("$_xleft", _xleft);
		put_var("$_xright", _xright);
		put_var("$_xinc", _xinc);
		_xsubdiv = 1;
		_xscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		_user_set_x_axis = true;
		return true;
	} else if (num_params == 3) {
		double xleft = cmd_parameter_n(0);
		double xright = cmd_parameter_n(1);
		double xinc = cmd_parameter_n(2);
		Require(xinc != 0.0, 
			err("`set x axis $left $right $incBig' cannot have 0 for $incBig"));
		Require(well_ordered(xleft, xright, xinc, xinc / 2.0),
			err("`set x axis $left $right $incBig' has $incBig of wrong sign"));
		_xleft = xleft;
		_xright = xright;
		if (_xtype == gr_axis_LOG) {
			_xinc = xinc;
			_xsubdiv = 1;
		} else { 
			_xinc = xinc;
			_xsubdiv = 1;
		}
		put_var("$_xleft", _xleft);
		put_var("$_xright", _xright);
		put_var("$_xinc", _xinc);
		_xscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		_user_set_x_axis = true;
		return true;
	} else if (num_params == 4) {
		double xleft = cmd_parameter_n(0);
		double xright = cmd_parameter_n(1);
		double xinc = cmd_parameter_n(2);
		double xincinc = cmd_parameter_n(3);
		if (xinc == 0.0) {
			err("`set x axis $left $right $incBig' cannot have 0 for $incBig");
			return false;
		}
		Require (well_ordered(xleft, xright, xinc, xincinc),
			 err("`set x axis $left $right $incBig $incSml' has $incBig or $incSml of wrong sign"));
		_xleft = xleft;
		_xright = xright;
		put_var("$_xleft", _xleft);
		put_var("$_xright", _xright);
		if (_xtype == gr_axis_LOG) {
			_xinc = xinc;
			_xsubdiv = 1;
		} else { 
			_xinc = xinc;
			_xsubdiv = int(floor(0.5 + fabs((double) (xinc / xincinc))));
		}
		put_var("$_xinc", _xinc);
		_xscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		_user_set_x_axis = true;
		return true;
	} else {
		err("`set x axis' may have only 2, 3 or 4 parameters");
		return false;
	}
	_user_set_x_axis = true;
	return true;
}

bool
set_x_formatCmd()
{
	_xFmt = cmd_parameter_s(0);
	_xFmt_set_by_user = true;
	return true;
}

bool
set_x_gridCmd()
{
	double x;
	double xmin = cmd_parameter_n(0);
	double xmax = cmd_parameter_n(1);
	double xinc = cmd_parameter_n(2);
	// check for stupidity
	Require(xinc != 0.0, err("Can't have $xinc=0"));
	Require(xmin != xmax, err("Can't have $xleft = $xright"));
	Require(well_ordered(xmin, xmax, xinc, xinc / 2.0),
		err("`set x grid $xleft $xright $xinc' has $xinc of wrong sign"));
	int nx = int(floor(1.5 + fabs((double) ((xmax - xmin) / xinc))));
	Require(nx > 0, err("$xinc too big"));
	// check against existing matrix
	if (_grid_exists == true && nx != (int)_xmatrix.size()) {
		sprintf(_errorMsg, "# intervals %d disagrees with existing grid size %d",
			nx, _xmatrix.size());
		err(_errorMsg);
		return false;
	}
	// get storage space
	_xmatrix.erase(_xmatrix.begin(), _xmatrix.end());
	// set up x grid 
	int i;
	for (i = 0, x = xmin; i < nx; i++, x += xinc)
		_xmatrix.push_back(x);
	// Override any existing scale
	define_image_scales(_xmatrix[0], 0.0, _xmatrix[nx - 1], 0.0);
	_xgrid_exists = true;
	if (!_xscale_exists) {
		create_x_scale();
	}
	if (_xmatrix[1] > _xmatrix[0])
		_xgrid_increasing = true;
	else
		_xgrid_increasing = false;
	return true;
}				// end set_x_gridCmd()

bool
set_y_gridCmd()
{
	double y;
	double ymin = cmd_parameter_n(0);
	double ymax = cmd_parameter_n(1);
	double yinc = cmd_parameter_n(2);
	// check for stupidity
	Require(yinc != 0.0, err("Can't have $yinc=0"));
	Require(ymin != ymax, err("Can't have $ybottom=$ytop."));
	Require(well_ordered(ymin, ymax, yinc, yinc / 2.0),
		err("`set y grid $bottom $top $yinc' has $yinc of wrong sign"));
	int ny = 1 + int(floor(0.5 + fabs((double) ((ymax - ymin) / yinc))));
	Require(ny > 0, err("$yinc too big"));
	// check against existing matrix
	if (_grid_exists == true && ny != int(_ymatrix.size())) {
		demonstrate_command_usage();
		sprintf(_errorMsg,
			"# intervals %d disagrees with existing grid %d",
			ny, _ymatrix.size());
		err(_errorMsg);
		return false;
	}
	// get storage space
	_ymatrix.erase(_ymatrix.begin(), _ymatrix.end());
	// set up y grid
	int i;
	for (i = 0, y = ymin; i < ny; i++, y += yinc)
		_ymatrix.push_back(y);
	// Override any existing scale
	define_image_scales(0.0, _ymatrix[0], 0.0, _ymatrix[ny - 1]);
	_ygrid_exists = true;
	if (!_yscale_exists)
		create_y_scale();
	if (_ymatrix[1] > _ymatrix[0])
		_ygrid_increasing = true;
	else
		_ygrid_increasing = false;
	return true;
}				// end set_y_gridCmd()

bool
set_x_marginCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params == 1) {
		double m;
		if (cmd_parameter_is_n(0)) {
			m = cmd_parameter_n(0);
			if (m < 0.0) {
				warning("`set x margin' ignoring negative value ", dtoa(m), "\\");
				return true;
			}
		} else {
			expecting_the_keyword("`set x margin'", 0, "`default'");
			m = XMARGIN_DEFAULT;
		}
		put_var("$_xmargin", m);
		_need_x_axis = true;
		_need_y_axis = true;
		return true;
#if 0
	} else if (num_params == 2...) {
		double          old = 0.0;
		if (!strcmp(_word[3].c_str(), "bigger")) {
			if (!getdnum(_word[4].c_str(), &tmp)) {
				err("can't understand `set x margin bigger' parameter");
				return false;
			}
			old = get_var_scalar("$_xmargin");
			tmp = old + tmp;
			put_var("$_xmargin", tmp);
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		} else if (!strcmp(_word[3].c_str(), "smaller")) {
			if (!getdnum(_word[4].c_str(), &tmp)) {
				err("can't understand `set x margin smaller' parameter");
				return false;
			}
			old = get_var_scalar("$_xmargin");
			tmp = old - tmp;
			put_var("$_xmargin", tmp);
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		} else {
			err("bad `set x margin' parameter\nvalid list: bigger/smaller");
			return false;
		}
	} else {
		err("Must specify margin in cm");
		return false;
#endif
	}
	return false;
}

bool
set_x_nameCmd()
{
	_xname = cmd_parameter_s(0);
 	return true;
}

// `set missing value $_missing_value' -- set width of plot
bool
set_x_sizeCmd()
{
	put_var("$_xsize", cmd_parameter_n(0));
	_need_x_axis = true;
	_need_y_axis = true;
	return true;
}

bool
set_y_axisCmd()
{
	_yatleft = true;
#if 0
	if (!strcmp(_word[_nword - 1].c_str(), "left")) {
		_yatleft = true;
		if (_nword == 4) {
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		}
		_nword--;
	} else if (!strcmp(_word[_nword - 1].c_str(), "right")) {
		_yatleft = false;
		if (_nword == 4) {
			_need_x_axis = true;
			_need_y_axis = true;
			return true;
		}
		_nword--;
	// set y axis label horizontal
	if (_nword == 5 && !strcmp(_word[3].c_str(), "label")) {
		set_y_axis_labelCmd();
		return true;
	}
#endif
	// ... specifying y axis
	unsigned int num_params = cmd_parameters();
	if (num_params == 1) {	// 'set y axis decreasing|increasing|unknown'
		if (cmd_parameter_is_n(0)) {
			err("`set y axis' expecting `increasing', `decreasing', or `unknown', not numeric argument `\\", dtoa(cmd_parameter_n(0)), "' as given", "\\");
			return false;
		} else {
			if (strEQ(cmd_parameter_s(0), "increasing")) {
				_yincreasing = true;
				if (_yscale_exists && _ybottom > _ytop) {
					swap(_ybottom, _ytop);
					put_var("$_ybottom", _ybottom);
					put_var("$_ytop", _ytop);
					_yinc = fabs(_yinc);
				}
				return true;
			} else if (strEQ(cmd_parameter_s(0), "decreasing")) {
				_yincreasing = false;
				if (_yscale_exists && _ybottom < _ytop) {
					swap(_ybottom, _ytop);
					put_var("$_ybottom", _ybottom);
					put_var("$_ytop", _ytop);
					_yinc = -fabs(_yinc);
				}
				return true;
			} else if (strEQ(cmd_parameter_s(0), "unknown")) {
				_yscale_exists = false;
				_need_y_axis = true;
				_user_set_y_axis = false;
				return true;
			} else {
				err("`set y axis' expecting word `increasing', `decreasing', or `unknown' not `\\", cmd_parameter_s(0), "' as found", "\\");
				return false;
			}
		}
	} else if (num_params == 2) {
		double ybottom = cmd_parameter_n(0);
		double ytop = cmd_parameter_n(1);
		_ybottom = ybottom;
		_ytop = ytop;
		if (_ytype == gr_axis_LOG)
			_yinc = 1.0;
		else
			_yinc = _ytop - _ybottom;
		put_var("$_ybottom", _ybottom);
		put_var("$_ytop", _ytop);
		put_var("$_yinc", _yinc);
		_ysubdiv = 1;
		_yscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		reset_top_of_plot();
		_user_set_y_axis = true;
		return true;
	} else if (num_params == 3) {
		double ybottom = cmd_parameter_n(0);
		double ytop = cmd_parameter_n(1);
		double yinc = cmd_parameter_n(2);
		Require(well_ordered(ybottom, ytop, yinc, yinc / 2.0),
			err("`set y axis .bottom. .top. .incBig.' has .incBig. of wrong sign"));
		_ybottom = ybottom;
		_ytop = ytop;
		if (_ytype == gr_axis_LOG) {
			_yinc = yinc;
			_ysubdiv = 1;
		} else { 
			_yinc = yinc;
			_ysubdiv = 1;
		}
		put_var("$_ybottom", _ybottom);
		put_var("$_ytop", _ytop);
		put_var("$_yinc", _yinc);
		_yscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		reset_top_of_plot();
		_user_set_y_axis = true;
		return true;
	} else if (num_params == 4) {
		double ybottom = cmd_parameter_n(0);
		double ytop = cmd_parameter_n(1);
		double yinc = cmd_parameter_n(2);
		double yincinc = cmd_parameter_n(3);
		Require(well_ordered(ybottom, ytop, yinc, yincinc),
			err("`set y axis .bottom. .top. .incBig. .incSml.' has .incBig. or .incSml. of wrong sign"));
		_ybottom = ybottom;
		_ytop = ytop;
		if (_ytype == gr_axis_LOG) {
			warning("`set y axis' ignoring increment because type is LOG");
			_yinc = yinc;
			_ysubdiv = 1;
		} else { 
			_yinc = yinc;
			_ysubdiv = 1;
			_ysubdiv = int(floor(0.5 + fabs((double) (yinc / yincinc))));
		}
		put_var("$_ybottom", _ybottom);
		put_var("$_ytop", _ytop);
		put_var("$_yinc", _yinc);
		_yscale_exists = true;
		_need_x_axis = true;
		_need_y_axis = true;
		reset_top_of_plot();
		_user_set_y_axis = true;
		return true;
	} else {
		err("`set y axis' may have only 2, 3 or 4 parameters");
		return false;
	}
	_user_set_y_axis = true;
	return true;
}

bool
set_y_axis_labelCmd()
{
	if (_nword == 5 && !strcmp(_word[3].c_str(), "label")) {
		if (!strcmp(_word[4].c_str(), "horizontal"))
			gr_setyaxisstyle(1);
		else if (!strcmp(_word[4].c_str(), "vertical"))
			gr_setyaxisstyle(0);
	}
	return true;
}

bool
set_y_formatCmd()
{
	_yFmt = cmd_parameter_s(0);
	_yFmt_set_by_user = true;
	return true;
}

bool
set_y_marginCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params == 1) {
		double m;
		if (cmd_parameter_is_n(0)) {
			m = cmd_parameter_n(0);
			if (m < 0.0) {
				warning("`set y margin' ignoring negative value ", dtoa(m), "\\");
				return true;
			}
		} else {
			expecting_the_keyword("`set y margin'", 0, "`default'");
			m = YMARGIN_DEFAULT;
		}
		put_var("$_ymargin", m);
		_need_x_axis = true;
		_need_y_axis = true;
		return true;
#if 0
	} else if (_nword == 5) {
		double          old = 0.0;
		if (!strcmp(_word[3].c_str(), "bigger")) {
			Require(getdnum(_word[4].c_str(), &tmp), 
				err("can't understand `set y margin bigger' parameter"));
			old = get_var_scalar("$_ymargin");
			tmp = old + tmp;
			put_var("$_ymargin", tmp);
			_need_x_axis = true;
			_need_y_axis = true;
			reset_top_of_plot();
			return true;
		} else if (!strcmp(_word[3].c_str(), "smaller")) {
			Require(getdnum(_word[4].c_str(), &tmp), 
				err("can't understand `set y margin smaller' parameter"));
			old = get_var_scalar("$_ymargin");
			tmp = old - tmp;
			put_var("$_ymargin", tmp);
			_need_x_axis = true;
			_need_y_axis = true;
			reset_top_of_plot();
			return true;
		} else {
			err("bad `set y margin' parameter\nvalid list: bigger/smaller");
			return false;
		}
	} else {
		err("Must specify margin in cm");
		return false;
#endif
	}
	return false;
}

bool
set_y_nameCmd()
{
	_yname = cmd_parameter_s(0);
	//printf("DEBUG 'set y name \"%s\"\n", cmd_parameter_s(0));
	return true;
}

// set_y_sizeCmd() -- store height of plot
bool
set_y_sizeCmd()
{
	put_var("$_ysize", cmd_parameter_n(0));
	reset_top_of_plot();
	_need_x_axis = true;
	_need_y_axis = true;
	return true;
}

#if 0
// Three formats are possible: `\syn = word .n. of "string"' `\syn =
// "string"' `\syn = system ...'
bool
assign_synonym()
{
	Require (_nword > 2, err("Can't understand command."));
	// Following check should never be needed, but keep for possible future
	// changes.
	Require(is_syn(_word[0].c_str()),
		err("`\\", _word[0].c_str(), "' must begin with `\\'", "\\"));
	// `\synonym = word .n. of "string"'
	if (_nword == 6
	    && !strcmp(_word[1].c_str(), "=")
	    && !strcmp(_word[2].c_str(), "word")
	    && !strcmp(_word[4].c_str(), "of")) {
		int             which = atoi(_word[3].c_str()), max, i;
		Require(which >= 0, err("Can't select word whose index is < 0"));
		GriString word_copy(_word[_nword - 1].c_str());
		// Use (tmp + 1) as string to allow chopping.  Then, must also clip
		// the trailing " from the last word.
		chop_into_words(word_copy.c_str() + 1, _Words2, &max, MAX_nword);
		i = strlen(_Words2[max - 1]);
		if (i > 2 && *(_Words2[max - 1] + i - 1) == '"')
			*(_Words2[max - 1] + i - 1) = '\0';
		if (which > (max - 1)) {
			err("The string \n`\\",
			    _word[_nword - 1].c_str(),
			    "'\ndoes not have that many words.  NOTE: the first word is counted as 0.",
			    "\\");
			return false;
		}
		Require(put_syn(_word[0].c_str(), _Words2[which], true),
			err("Cannot store synonym `\\", _word[0].c_str(), "'", "\\"));
		return true;
	} else if (!strcmp(_word[1].c_str(), "=") && !strcmp(_word[2].c_str(), "tmpname")) {
#if defined(HAVE_TEMPNAM)
		put_syn(_word[0].c_str(), tempnam("/usr/tmp", "gri"), true);
#else
#if defined(HAVE_TMPNAM)
		if (!put_syn(_word[0].c_str(), tmpnam(NULL), true))
			gr_Error("Ran out of storage");
#else
		if (!put_syn(_word[0].c_str(), GRE_TMP_FILE, true))
			gr_Error("Ran out of storage");
#endif // HAVE_TMPNAM
#endif // HAVE_TEMPNAME
		return true;
	} else if (!strcmp(_word[1].c_str(), "=") && !strcmp(_word[2].c_str(), "system")) {
		// `\synonym = system ...'
		FILE           *pipefile;
		// Much of following code duplicated in sytemCmd(), so if any
		// problems crop up, check there too.
		char *            s = _cmdLine;
		s += skip_space(s);	// skip any initial space
		s += skip_nonspace(s);	// skip first word "\syn"
		s += skip_space(s);	// skip space
		s += skip_nonspace(s);	// skip "="
		s += skip_space(s);	// skip space
		s += skip_nonspace(s);	// skip "system"
		s += skip_space(s);	// skip space
		if (*s == '\0' || *s == '\n') {
			err("`\\syn = system ...' needs a system command to do.");
			return false;
		}
		if (((unsigned) superuser()) & FLAG_SYS) {
			ShowStr("\n`\\synonym = system' sending the following command to the operating system:\n");
			ShowStr(s);
			ShowStr("\n");
		}
		pipefile = call_popen(s, "r");
		if (pipefile) {
			string result = "";
			GriString this_line;
			int             len_result;
			while (!this_line.line_from_FILE(pipefile))
				result.append(this_line.c_str());
			pclose(pipefile);
			// Remove last newline (or several newlines, if they exist).
			len_result = result.size();
			for (int i = len_result - 1; i > -1; i--)
				if (result[i] == '\n')
					result[i] = '\0';
			if (!put_syn(_word[0].c_str(), result.c_str(), true)) OUT_OF_MEMORY;
			return true;
		} else {
			err("`\\",
			    _word[0].c_str(),
			    " = system ...' can't access system",
			    "\\");
			return false;
		}
	} else {
		// `\synonym = "..."'
		if (!ExtractQuote(_errorMsg, _cmdLine)) {
			err("Can't extract value for synonym '\\",
			    _word[0].c_str(),
			    "'.  The value must be in double quotes.",
			    "\\");
			return false;
		}
		// Store synonym
		if (!put_syn(_word[0].c_str(), _errorMsg, true)) OUT_OF_MEMORY;
	}
	return true;
}
#endif
