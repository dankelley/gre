#include	<math.h>
#include	"gr.h"
#include	"extern.h"
#include	"private.h"

// Return true if could make a scale
bool
create_x_scale()
{
	int             nsegs;
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		return false;
	double xmin, xmax;		// of column
	extremal_values(xind, &xmin, &xmax);
	double min, max;		// may be of grid
	if (_need_x_axis && !_user_set_x_axis) {
		if (!gr_missing(xmin)) {
			min = xmin;
			max = xmax;
		} else {
			if (_xmatrix.size() > 0) {
				min = max = _xmatrix[0];
				for (unsigned int i = 1; i < _xmatrix.size(); i++) {
					if (_xmatrix[i] < min)
						min = _xmatrix[i];
					if (_xmatrix[i] > max)
						max = _xmatrix[i];
				}
			} else {
				return false;
			}
		}
		if (_xtype == gr_axis_LINEAR) {
			int             nsegs_est;
			double          xsize;
			xsize = get_var_scalar("$_xsize");
			nsegs_est = 1 + (int) (xsize / 2.0); // ??? Should use fontsize
			if (_xincreasing)
				gr_scale125(min, max, nsegs_est, &_xleft, &_xright, &nsegs);
			else
				gr_scale125(min, max, nsegs_est, &_xright, &_xleft, &nsegs);
			_xinc = (_xright - _xleft) / nsegs;
		} else {
			// LOG axis
			if (min <= 0.0 || max <= 0.0) {
				// log data not OK
				int             nsegs_est;
				double          xsize;
				warning("Have non-positive data -- switching to linear axis");
				xsize = get_var_scalar("$_xsize");
				nsegs_est = 1 + (int) (xsize / 2.0);
				_xtype = gr_axis_LINEAR;
				gr_scale125(min, max, nsegs_est, &_xleft, &_xright, &nsegs);
				_xinc = ((double) _xright - (double) _xleft) / nsegs;
			} else {
				_xleft = pow(10.0, floor(log10((double) min)));
				_xright = pow(10.0, ceil(log10((double) max)));
				_xinc = 1.0;
			}
		}
		put_var("$_xleft", _xleft);
		put_var("$_xright", _xright);
		put_var("$_xinc", _xinc);
		_xscale_exists = true;
	}
	return true;
}

// Return true if could make a scale
bool
create_y_scale()
{
	int             nsegs;
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		return false;
	double ymin, ymax;		// of column
	extremal_values(yind, &ymin, &ymax);
	double min, max;		// may be of grid
	if (_need_y_axis && !_user_set_y_axis) {
		if (!gr_missing(ymin)) {
			min = ymin;
			max = ymax;
		} else {
			if (_ymatrix.size() > 0) {
				min = max = _ymatrix[0];
				for (unsigned int i = 1; i < _ymatrix.size(); i++) {
					if (_ymatrix[i] < min)
						min = _ymatrix[i];
					if (_ymatrix[i] > max)
						max = _ymatrix[i];
				}
			} else {
				return false;
			}
		}
		if (_ytype == gr_axis_LINEAR) {
			int             nsegs_est;
			double          ysize;
			ysize = get_var_scalar("$_ysize");
			nsegs_est = 1 + (int) (ysize / 2.0);
			if (_yincreasing)
				gr_scale125(min, max, nsegs_est, &_ybottom, &_ytop, &nsegs);
			else
				gr_scale125(min, max, nsegs_est, &_ytop, &_ybottom, &nsegs);
			_yinc = (_ytop - _ybottom) / nsegs;
		} else {
			// LOG axis
			if (min <= 0.0 || max <= 0.0) {
				// log data not OK
				int             nsegs_est;
				double          ysize;
				warning("Have non-positive data -- switching to linear axis");
				ysize = get_var_scalar("$_ysize");
				nsegs_est = 1 + (int) (ysize / 2.0);
				_ytype = gr_axis_LINEAR;
				gr_scale125(min, max, nsegs_est, &_ybottom, &_ytop, &nsegs);
				_yinc = (_ytop - _ybottom) / nsegs;
			} else {
				_ybottom = pow(10.0, floor(log10(min)));
				_ytop = pow(10.0, ceil(log10(max)));
				_yinc = 1.0;
			}
		}
		put_var("$_ybottom", _ybottom);
		put_var("$_ytop", _ytop);
		put_var("$_yinc", _yinc);
		_yscale_exists = true;
	}
	return true;
}
