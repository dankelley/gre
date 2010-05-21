#include <string>
#include <math.h>
#include <stdlib.h>
#include "gr.h"
#include "GriPath.h"
#include "extern.h"

#define TIC_RATIO	0.5	/* (Length small tic) / (large tic) */
//REMOVED//extern char     _grXAxisLabel[];
//REMOVED//extern char     _grYAxisLabel[];
//REMOVED-2000-5-2 extern char     _grNumFormat_x[];
//REMOVED-2000-5-2 extern char     _grNumFormat_y[];
extern char     _grTempString[];
extern int      _grAxisStyle_x;
extern int      _grAxisStyle_y;
extern gr_axis_properties _grTransform_x;
extern gr_axis_properties _grTransform_y;
extern int      _grNeedBegin;
extern int      _grNumSubDiv_x;
extern int      _grNumSubDiv_y;
extern double   _grCmPerUser_x;
extern double   _grCmPerUser_y;
extern double   _grTicSize_cm;

#define	round_down_to_10(x) 	(pow (10.0, floor (log10 ( (x) ))))
#define	round_to_1(x)		(floor (0.5 + (x) ))

/* Some twiddles */
#define AXIS_TWIDDLE            0.005	/* .05mm overhang on axis */
#define SMALLNUM                (1.0e-3)	/* twiddle axis range */
#define SMALLERNUM              (1.0e-4)	/* twiddle axis range */
#define SMALLFONTSIZE           (1.0e-3)	/* too small to draw */
/*
 * local functions
 */
static void same_decimals(double start, double finish, double inc, const string& in, string&out);
static bool next_tic(double *next, double present, double final, double inc, gr_axis_properties axis_type, bool increasing);



// Using format in the string 'in', scan the resultant values of what
// would be printed (drawn), and then make a new format 'out' that
// will give all the same number of digits after the decimal point.  This
// makes axes look a lot nicer.
static void
same_decimals(double start, double finish, double inc, const string& in, string&out)
{
	if (start > finish) {
		double tmp = finish;
		finish = start;
		start = tmp;
	}
	inc = fabs(inc);
	int max_decimal_places = 0;
	for (double val = start; val <= finish; val += inc) {
		char buf[200];
		sprintf(buf, in.c_str(), val);
		int l = strlen(buf);
		for (int i = l - 1; i >= 0; i--) {
			if (buf[i] == '.') {
				if ((l - 1 - i) > max_decimal_places)
					max_decimal_places = l - 1 - i;
				//printf("DEBUG '%s' %d\n",buf,l-1-i);
				break;
			}
		}
	}
	//printf("DEBUG: num dec %d\n",max_decimal_places);
	char new_fmt[100];
	sprintf(new_fmt, "%%.%df", max_decimal_places);
	out.assign(new_fmt);
}


// Calculate next tic location on axis.  Note that if the axis starts off
// ragged, this will assign to *next the first multiple of "inc". Returns
// true if more axis to do
static bool
next_tic(double *next,
	 double present,
	 double final,
	 double inc,
	 gr_axis_properties axis_type,
	 bool increasing)
{
	double          order_of_mag, mantissa;
	// Check to see if already ran out of axis.
	if (present >= final && increasing == true)
		return false;
	if (present <= final && increasing == false)
		return false;
	// Determine next tic to draw to, treating linear/log separately.
	switch (axis_type) {
	case gr_axis_LINEAR:
		*next = inc * (1.0 + floor((SMALLERNUM * inc + present) / inc));
		break;
	case gr_axis_LOG:
		if (present <= 0.0)
			gr_Error("zero or negative on log axis (internal error).");
		order_of_mag = round_down_to_10(present);
		mantissa = present / order_of_mag;
		if (increasing)
			mantissa = ceil(mantissa + SMALLNUM);
		else {
			if (mantissa <= 1.0)
				mantissa = 0.9;
			else
				mantissa = floor(mantissa - SMALLNUM);
		}
		*next = order_of_mag * mantissa;
		break;
	default:
		gr_Error("unknown axis type (internal error)");
	}
	// Set flag if this will overrun axis.
	if (increasing == true)
		return (*next <= final) ? true : false;
	else
		return (*next >= final) ? true : false;
}

/*
 * gr_drawxyaxes DESCRIPTION: Draws an x-y axis frame
 */
void
gr_drawxyaxes(double xl, double xinc, double xr, double yb, double yinc, double yt)
{
	double          old_fontsize_pt = gr_currentfontsize_pt();
	gr_drawxaxis(yb, xl, xinc, xr, gr_axis_BOTTOM);
	gr_drawyaxis(xl, yb, yinc, yt, gr_axis_LEFT);
	gr_setfontsize_pt(0.0);
	gr_drawxaxis(yt, xl, xinc, xr, gr_axis_TOP);
	gr_drawyaxis(xr, yb, yinc, yt, gr_axis_RIGHT);
	gr_setfontsize_pt(old_fontsize_pt);
}

// DESCRIPTION: The axis extends from 'xl' to 'xr',with numbers placed at
// intervals of 'xinc'.
// If 'side'==BOTTOM/TOP this is an axis designed to appear at the
// bottom/top of a plotting region (ie, the numbers are below/above).
void
gr_drawxaxis(double y, double xl, double xinc, double xr, gr_axis_properties side)
{
	string xFmt;
	if (!_xFmt_set_by_user && _grTransform_x == gr_axis_LINEAR)
		same_decimals(xl, xr, xinc, _xFmt, xFmt);

	GriString label;
	extern char     _xtype_map;
	double          CapHeight = gr_currentCapHeight_cm();
	double          angle = 0.0;	// angle of axis tics, labels, etc
	double          tic, tic_sml;	// length of tic marks
	double          xcm, ycm;	// tmp
	double          offset;	// for numbers
	double          present, next, final = xr, smallinc = 0.0;
	int             decade_between_labels;	// for log axes
	double          tmp1, tmp2;
	GriPath         axis_path;

	// XREF -- axis transform
	// Calculate size of large and small tic marks.
	extern bool     _grTicsPointIn;
	tic = ((side == gr_axis_LEFT && _grTicsPointIn == false)
	       || (side == gr_axis_RIGHT && _grTicsPointIn == true))
		? -_grTicSize_cm : _grTicSize_cm;
	if (_grTransform_x == gr_axis_LOG && _xsubdiv < 0)
		tic_sml = 0.0;
	else
		tic_sml = TIC_RATIO * tic;
	// Calculate offset = space for numbers.
	offset = 0.0;
	int old_linecap = _griState.line_cap();
	int old_linejoin = _griState.line_join();
	_griState.set_line_cap(0);
	_griState.set_line_join(0);
	switch (_grTransform_x) {
	case gr_axis_LINEAR:
	case gr_axis_LOG:
		if (side == gr_axis_BOTTOM) {
			if (0 != xFmt.size())
				offset -= 1.75 * CapHeight;
			offset -= ((_grTicsPointIn == false) ? _grTicSize_cm : 0.0);
		} else {
			if (0 != xFmt.size())
				offset += 0.75 * CapHeight;
			offset += ((_grTicsPointIn == false) ? _grTicSize_cm : 0.0);
		}
		break;
	default:
		gr_Error("unknown axis type (internal error)");
	}
	// Draw axis, moving from tic to tic.  Tics are advanced by smallinc for
	// linear axes and by something appropriate for log axes. Whenever the
	// current location should be a big tic, such a tic is drawn along with a
	// label.
	bool increasing = ((xr > xl) ? true : false);
	double xl_cm, y_cm;
	switch (_grTransform_x) {
	case gr_axis_LINEAR:
		smallinc = xinc / _grNumSubDiv_x;
		// Twiddle axes to extend a bit beyond the requested
		// region, to prevent rounding problems.
		present = xl - xinc / 1.0e3;
		final   = xr + xinc / 1.0e3;
		// Draw x-axis, moving from tic to tic.  Tics are advanced by
		// smallinc for linear axes and by something appropriate for log
		// axes. Whenever the current location should be a big tic, such a
		// tic is drawn along with a label.
		gr_usertocm(xl, y, &xl_cm, &y_cm);
		axis_path.push_back(xl_cm, y_cm, 'm');
		while (next_tic(&next, present, final, smallinc, _grTransform_x, increasing)) {
			// Determine angle of x-axis tics, for non-rectangular axes
			switch (_grTransform_x) {
			case gr_axis_LINEAR:
			case gr_axis_LOG:
				angle = atan2(1.0, 0.0);
				break;
			default:
				gr_Error("unknown axis type (internal error)");
				break;
			}
			gr_usertocm(next, y, &xcm, &ycm);
			axis_path.push_back(xcm, ycm, 'l');
			// Detect large tics on x axis
			if (gr_multiple(next, xinc, 0.01 * smallinc)) {
				// draw large tic
				axis_path.push_back(xcm + tic * cos(angle), ycm + tic * sin(angle), 'l');
				if (gr_currentfontsize_pt() > SMALLFONTSIZE) {
					if (_xtype_map != ' ') {
						// It's a map, so figure the deg/min/seconds;
						// over-ride any existing format
						int             hour, min, sec;
						if (gr_multiple(next, 1.0, 1.0e-6)) {
							hour = (int)floor(1.0e-4 + next);
							sprintf(_grTempString, "%d\\(\\circ\\)%c", hour, _xtype_map);
						} else if (gr_multiple(next, 1.0 / 60.0, 1.0e-7)) {
							hour = (int)floor(1.0e-4 + next);
							min = (int)floor(1e-5 + 60.0 * (next - hour));
							sprintf(_grTempString, "%d\\(\\circ\\)%d'%c", hour, min, _xtype_map);
						} else if (gr_multiple(next, 1.0 / 3600.0, 1.0e-8)) {
							hour = (int)floor(1.0e-4 + next);
							min = (int)floor(1e-5 + 60.0 * (next - hour));
							sec = (int)floor(1e-5 + 3600.0 * (next - hour - min / 60.0));
							sprintf(_grTempString, "%d\\(\\circ\\)%d'%d\"%c", hour, min, sec, _xtype_map);
						} else
							sprintf(_grTempString, "%f\\(\\circ\\)%c", next, _xtype_map);
					} else if (0 != xFmt.size()) {
						sprintf(_grTempString, xFmt.c_str(), next);
						if (get_flag("emulate_gre")) {
							string gs(_grTempString);
							size_t chop;
							if (STRING_NPOS != (chop = gs.find("E+0"))) {
								gs.replace(chop, 3, "\\(\\times10^{");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E-0"))) {
								gs.replace(chop, 3, "\\(\\times10^{-");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E+"))) {
								gs.replace(chop, 2, "\\(\\times10^{");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E-"))) {
								gs.replace(chop, 2, "\\(\\times10^{-");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E"))) {
								gs.replace(chop, 1, "\\(\\times10^{");
								gs.append("}\\)");
							}
							strcpy(_grTempString, gs.c_str());
						}
					} else
						*_grTempString = '\0';
					// Text is rotated
					angle -= 90.0 / DEG_PER_RAD;
					label.assign(_grTempString);
					label.draw(xcm - offset * sin(angle),
						   ycm + offset * cos(angle),
						   TEXT_CENTERED,
						   DEG_PER_RAD * angle);
				}
			} else {
				// Small tic
				axis_path.push_back(xcm + tic_sml * cos(angle), ycm + tic_sml * sin(angle), 'l');
			}
			axis_path.push_back(gr_usertocm_x(next, y), gr_usertocm_y(next, y), 'l');
			present = next;
		}
		// Finish by drawing to end of axis (in case there was no tic there).
		axis_path.push_back(gr_usertocm_x(final, y), gr_usertocm_y(final, y), 'l');
		axis_path.stroke(units_cm, _griState.linewidth_axis());
		break;
	case gr_axis_LOG:
		decade_between_labels = (int) floor(0.5 + xinc);
		gr_usertocm(xl, y, &xcm, &ycm);
		gr_cmtouser(xcm - AXIS_TWIDDLE, ycm, &tmp1, &tmp2);
		present = tmp1;
		axis_path.push_back(present, y, 'm');
		while (next_tic(&next, present, final, smallinc, _grTransform_x, increasing)) {
			double          tmp, next_log;
			double xuser, yuser;
			axis_path.push_back(next, y, 'l');
			next_log = log10(next);
			tmp = next_log - floor(next_log);
			gr_usertocm(next, y, &xcm, &ycm);
			if (-0.01 < tmp && tmp < 0.01) {
				// large tic & number
				gr_cmtouser(xcm, ycm+tic, &xuser, &yuser);
				axis_path.push_back(xuser, yuser, 'l');
				gr_cmtouser(xcm, ycm+offset, &xuser, &yuser);
				tmp = next_log
					- decade_between_labels * floor(next_log / decade_between_labels);
				if (gr_currentfontsize_pt() > SMALLFONTSIZE
				    && -0.01 < tmp / xinc && tmp / xinc < 0.01
				    && 0 != xFmt.size()) {
					// Draw "1" as a special case
					if (0.99 < next && next < 1.01)
						sprintf(_grTempString, "1");
					else
						sprintf(_grTempString, "\\(10^{%.0f}\\)", log10(next));
					label.assign(_grTempString);
					label.draw(xcm, ycm + offset, TEXT_CENTERED, 0.0);
				}
			} else {
				// small tic
				gr_cmtouser(xcm, ycm+tic_sml,&xuser, &yuser);
				axis_path.push_back(xuser, yuser, 'l');
			}
			axis_path.push_back(next, y, 'm');
			present = next;
		}
		// Finish by drawing to end of axis (in case there was no tic there).
		axis_path.push_back(final, y, 'l');
		axis_path.stroke(units_user, _griState.linewidth_axis());
		break;
	default:
		gr_Error("unknown axis type (internal error)");
	}
	// Draw axis title.
	if (gr_currentfontsize_pt() > SMALLFONTSIZE) {
		label.assign(_xname.c_str());
		switch (_grTransform_x) {
		case gr_axis_LINEAR:
			if (side == gr_axis_TOP) {
				double          xcm, ycm;
				gr_usertocm(0.5 * (xl + final), y, &xcm, &ycm);
				label.draw(xcm,
					   ycm + offset + 1.75 * CapHeight,
					   TEXT_CENTERED,
					   0.0);
			} else {
				double          xcm, ycm;
				gr_usertocm(0.5 * (xl + final), y, &xcm, &ycm);
				label.draw(xcm,
					   ycm + offset - 1.75 * CapHeight,
					   TEXT_CENTERED,
					   0.0);
			}
			break;
		case gr_axis_LOG:
			if (side == gr_axis_TOP) {
				double          xcm, ycm;
				gr_usertocm(sqrt(xl * final), y, &xcm, &ycm);
				label.draw(xcm,
					   ycm + offset + (1.75 + 0.75) * CapHeight,
					   TEXT_CENTERED,
					   0.0);
			} else {
				double          xcm, ycm;
				gr_usertocm(sqrt(xl * final), y, &xcm, &ycm);
				label.draw(xcm,
					   ycm + offset - 1.75 * CapHeight,
					   TEXT_CENTERED,
					   0.0);
			}
			break;
		default:
			gr_Error("unknown axis type (internal error)");
		}
	}
	_griState.set_line_cap(old_linecap);
	_griState.set_line_join(old_linejoin);
}

#define FACTOR 1.35		// Kludge to scale fonts up
// Draw a y axis
void
gr_drawyaxis(double x, double yb, double yinc, double yt, gr_axis_properties side)
{
	string yFmt(_yFmt);	// may change this local copy
	if (!_yFmt_set_by_user && _grTransform_y == gr_axis_LINEAR)
		same_decimals(yb, yt, yinc, _yFmt, yFmt);
	//printf("DEBUG '%s' -> '%s'\n",_yFmt.c_str(),yFmt.c_str());

	GriString label;
	extern char     _ytype_map;
	double          CapHeight = gr_currentCapHeight_cm();
	double          angle = 0.0; // angle of axis tics, labels, etc
	double          tic, tic_sml; // length of tic marks
	double          xcm, ycm;	  // used to step along axis
	double          xcm2, ycm2;	  // tmp, allowed to mess with
	double          labelx_cm, labely_cm; // where tic label will go
	double          offset;		  // for numbers
	double          present, next, final = yt, smallinc = 0.0;
	int             decade_between_labels;	// for log axes
	double          max_num_width_cm = 0.0;	// use for positioning label
	double          tmp0, tmp1, tmp2;
	GriPath         axis_path;
	// Calculate size of large and small tic marks.
	extern bool     _grTicsPointIn;
	tic = ((side == gr_axis_LEFT && _grTicsPointIn == true)
	       || (side == gr_axis_RIGHT && _grTicsPointIn == false))
		? _grTicSize_cm : -_grTicSize_cm;
	if (_grTransform_y == gr_axis_LOG && _ysubdiv < 0)
		tic_sml = 0.0;
	else
		tic_sml = TIC_RATIO * tic;
	// Calculate offset = space for numbers.
	if (side == gr_axis_LEFT) {
		if (_grTicsPointIn == true) {
			offset = -0.5 * FACTOR * CapHeight;
		} else {
			offset = -0.5 * FACTOR * CapHeight - _grTicSize_cm;
		}
	} else if (_grTicsPointIn == true) {
		offset = 0.5 * FACTOR * CapHeight;
	} else {
		offset = 0.5 * FACTOR * CapHeight + _grTicSize_cm;
	}
	int old_linecap = _griState.line_cap();
	int old_linejoin = _griState.line_join();
	_griState.set_line_cap(0);
	_griState.set_line_join(0);
	// Draw y-axis, moving from tic to tic.  Tics are advanced by smallinc
	// for linear axes and by something appropriate for log axes. Whenever
	// the current location should be a big tic, such a tic is drawn along
	// with a label.
	bool increasing = ((yt > yb) ? true : false);
	switch (_grTransform_y) {
	case gr_axis_LINEAR:
		smallinc = yinc / _grNumSubDiv_y;
		present = yb - yinc / 1.0e3;
		final   = yt + yinc / 1.0e3;
		axis_path.push_back(gr_usertocm_x(x, yb), gr_usertocm_y(x, yb), 'm');
		while (next_tic(&next, present, final, smallinc, _grTransform_y, increasing)) {
			axis_path.push_back(gr_usertocm_x(x, next), gr_usertocm_y(x, next), 'l');
			gr_usertocm(x, next, &xcm, &ycm);
			// Determine angle of y-axis tics, for non-rectangular axes
			angle = 0.0;

			// Detect large tics on y axis
			if (gr_multiple(next, yinc, 0.01 * smallinc)) {
				double tmpx, tmpy;
				gr_cmtouser(xcm + tic * cos(angle), ycm + tic * sin(angle), &tmpx, &tmpy);
				axis_path.push_back(xcm + tic * cos(angle), ycm + tic * sin(angle), 'l');
				labelx_cm = xcm + offset * cos(angle);
				labely_cm = ycm + offset * sin(angle)
					- 0.5 * CapHeight;
				if (gr_currentfontsize_pt() > SMALLFONTSIZE) {
					if (_ytype_map != ' ') {
						// It's a map, so figure the deg/min/seconds;
						// over-ride any existing format
						int             hour, min, sec;
						if (gr_multiple(next, 1.0, 1.0e-6)) {
							hour = (int)floor(1.0e-4 + next);
							sprintf(_grTempString, "%d\\(\\circ\\)%c", hour, _ytype_map);
						} else if (gr_multiple(next, 1.0 / 60.0, 1.0e-7)) {
							hour = (int)floor(1.0e-4 + next);
							min = (int)floor(1e-5 + 60.0 * (next - hour));
							sprintf(_grTempString, "%d\\(\\circ\\)%d'%c", hour, min, _ytype_map);
						} else if (gr_multiple(next, 1.0 / 3600.0, 1.0e-8)) {
							hour = (int)floor(1.0e-4 + next);
							min = (int)floor(1e-5 + 60.0 * (next - hour));
							sec = (int)floor(1e-5 + 3600.0 * (next - hour - min / 60.0));
							sprintf(_grTempString, "%d\\(\\circ\\)%d'%d\"%c", hour, min, sec, _ytype_map);
						} else {
							sprintf(_grTempString, "%f\\(\\circ\\)%c", next, _ytype_map);
						}
					} else if (0 != yFmt.size()) {
						if (get_flag("emulate_gre")) {
							sprintf(_grTempString, yFmt.c_str(), next);
							string gs(_grTempString);
							size_t chop;
							if (STRING_NPOS != (chop = gs.find("E+0"))) {
								gs.replace(chop, 3, "\\(\\times10^{");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E-0"))) {
								gs.replace(chop, 3, "\\(\\times10^{-");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E+"))) {
								gs.replace(chop, 2, "\\(\\times10^{");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E-"))) {
								gs.replace(chop, 2, "\\(\\times10^{-");
								gs.append("}\\)");
							} else if (STRING_NPOS != (chop = gs.find("E"))) {
								gs.replace(chop, 1, "\\(\\times10^{");
								gs.append("}\\)");
							}
							strcpy(_grTempString, gs.c_str());
						} else {
							sprintf(_grTempString, yFmt.c_str(), next);
						}
						//printf("DEBUG: label will be '%s'\n", _grTempString);
					} else {
						*_grTempString = '\0';
					}
					label.assign(_grTempString);
					if (side == gr_axis_LEFT) {
						label.draw(labelx_cm, labely_cm, TEXT_RJUST, angle * DEG_PER_RAD);
					} else {
						label.draw(labelx_cm, labely_cm, TEXT_LJUST, angle * DEG_PER_RAD);
					}
					// Keep track of maximum width of axis numbers, so that
					// axis label can be offset right amount.
					gr_stringwidth(_grTempString, &tmp0, &tmp1, &tmp2);
					if (tmp0 > max_num_width_cm) {
						max_num_width_cm = tmp0;
					}
				}
			} else {
				// Small tic
				axis_path.push_back(xcm + tic_sml * cos(angle), ycm + tic_sml * sin(angle), 'l');
			}
			axis_path.push_back(gr_usertocm_x(x, next), gr_usertocm_y(x, next), 'l');
			present = next;
		}
		// Finish by drawing to end of axis (in case there was no tic there).
		axis_path.push_back(gr_usertocm_x(x, yt), gr_usertocm_y(x, yt), 'l');
		axis_path.stroke(units_cm, _griState.linewidth_axis());
		break;
	case gr_axis_LOG:
		decade_between_labels = (int) floor(0.5 + yinc);
		gr_usertocm(x, yb, &xcm, &ycm);
		gr_cmtouser(xcm, ycm - AXIS_TWIDDLE, &tmp1, &tmp2);
		present = tmp2;
		axis_path.push_back(x, present, 'm');
		while (next_tic(&next, present, final, smallinc, _grTransform_y, increasing)) {
			double          tmp, next_log;
			axis_path.push_back(x, next, 'l');
			next_log = log10(next);
			tmp = next_log - floor(next_log);
			gr_usertocm(x, next, &xcm2, &ycm2);	// NOTE: not using (xcm,ycm)
			if (-0.01 < tmp && tmp < 0.01) {
				// large tic & number
				double xuser, yuser;
				gr_cmtouser(xcm2 + tic, ycm2, &xuser, &yuser);
				axis_path.push_back(xuser, yuser, 'l');
				gr_cmtouser(xcm2 + tic, ycm2 - 0.5 * FACTOR * CapHeight, &xuser, &yuser);
				tmp = next_log - decade_between_labels * floor(next_log / decade_between_labels);
				if (gr_currentfontsize_pt() > SMALLFONTSIZE
				    && -0.01 < tmp / yinc && tmp / yinc< 0.01
				    && yFmt.size() != 0) {
					// Draw "1" as a special case
					if (0.99 < next && next < 1.01)
						sprintf(_grTempString, "1");
					else
						sprintf(_grTempString, "\\(10^{%.0f}\\)", log10(next));
					label.assign(_grTempString);
					if (side == gr_axis_LEFT)
						label.draw(xcm2 + offset, ycm2 - 0.5 * CapHeight, TEXT_RJUST, 0.0);
					else
						label.draw(xcm2 + offset, ycm2 - 0.5 * CapHeight, TEXT_LJUST, 0.0);
					// Keep track of maximum width of axis numbers, so that
					// axis label can be offset right amount.
					gr_stringwidth(_grTempString, &tmp0, &tmp1, &tmp2);
					if (tmp0 > max_num_width_cm)
						max_num_width_cm = tmp0;
				}
			} else {
				// small tic
				double xuser, yuser;
				gr_cmtouser(xcm2 + tic_sml, ycm2, &xuser, &yuser);
				axis_path.push_back(xuser, yuser, 'l');
			}
			axis_path.push_back(x, next, 'l');
			present = next;
		}
		// Finish by drawing to end of axis (in case there was no tic there).
		axis_path.push_back(x, final, 'l');
		axis_path.stroke(units_user, _griState.linewidth_axis());
		break;
	default:
		gr_Error("unknown axis type (internal error)");
	}
	// write label, rotated if necessary
	if (gr_currentfontsize_pt() > SMALLFONTSIZE) {
		// Start to calculate what x to put label at; this makes xcm be on
		// axis, so will have to shift depending on orientation of label.
		// Note: will now re-use 'angle' to mean angle of y axis

		if (_grTransform_y == gr_axis_LOG) {
			double          x_cm, xx_cm, y_cm, yy_cm;
			gr_usertocm(x, sqrt(yb * yt), &x_cm, &y_cm);
			gr_usertocm(x, 0.001 + sqrt(yb * yt), &xx_cm, &yy_cm);
			angle = DEG_PER_RAD * atan2(yy_cm - y_cm, xx_cm - x_cm);
		} else {
			double          x_cm, xx_cm, y_cm, yy_cm;
			gr_usertocm(x, 0.5 * (yb + yt), &x_cm, &y_cm);
			gr_usertocm(x, 0.01 * yinc + 0.5 * (yb + yt), &xx_cm, &yy_cm);
			angle = DEG_PER_RAD * atan2(yy_cm - y_cm, xx_cm - x_cm);
		}
		xcm = 0.5 * (gr_usertocm_x(x, yb) + gr_usertocm_x(x, yt));
		ycm = 0.5 * (gr_usertocm_y(x, yb) + gr_usertocm_y(x, yt));
		// Need  at least max_num_width_cm, i.e., widest numeric label, plus
		// a little space (check against above).
		max_num_width_cm += FACTOR * CapHeight;
		// Need space for tics too
		max_num_width_cm += (_grTicsPointIn == true ? 0.0 : _grTicSize_cm);
		// Do by cases -- inelegant but flexible to change
		label.assign(_yname.c_str());
		switch (_grAxisStyle_y) {
		default:
		case 0:		// label parallel to axis
			if (side == gr_axis_LEFT) {
				label.draw(xcm - max_num_width_cm,
					   ycm,
					   TEXT_CENTERED, angle);
			} else {
				label.draw(xcm + max_num_width_cm + FACTOR * CapHeight,
					   ycm,
					   TEXT_CENTERED, angle - 180);
			}
			break;
		case 1:			// horizontal label
			if (side == gr_axis_LEFT) {
				label.draw(xcm - max_num_width_cm,
					   ycm - 0.5 * CapHeight,
					   TEXT_RJUST, 90.0 - angle);
			} else {
				label.draw(xcm + max_num_width_cm,
					   ycm - 0.5 * CapHeight,
					   TEXT_LJUST, 90.0 - angle);
			}
			break;
		}
	}
	_griState.set_line_cap(old_linecap);
	_griState.set_line_join(old_linejoin);
}
