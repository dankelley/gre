// -*- mode: c++ -*-
#if !defined(_extern_h_)
#define _extern_h_

/* System headers */
#include <string>
#include <vector>

/* Local headers */
#include "private.h"
#include "types.h"
#include "errors.h"
#include "parser.h"

#include "gr.h"
#include "gr_coll.h"
#include "GMatrix.h"
#include "GriState.h"

#include "macro.h"
#define AXES_RECTANGULAR	0	// normal axes style (rect)
#define AXES_POLAR		50	// polar axes


#define IMAGE_MAX 255

extern GriState _griState;	// <-> gri.cc

// The following globals have symbolic names associated with them, and
// MUST be updated whenever these names are assigned to.  See the note in
// put_var() in variable.c.  The reason for the parallel C storage is that
// the following are accessed for every data point plotted.  Certain other
// symbolic variables (like  ..publication.. for example) are not accessed
// frequently, and hence have no parallel C storage as the following do. Thus
// they are safe against breakage.
extern bool       put_var(const char *n, double val);
extern bool       put_var(const char *n, double val, unsigned int index);
extern bool       put_var(const char *n, const char *val);
extern bool       put_var(const char *n, const char *val, unsigned int index);

extern bool      _user_gave_bounding_box;
extern rectangle _bounding_box_user;
extern rectangle _bounding_box;
#if 0
// Column data
extern GriColumn _colR;
extern GriColumn _colTHETA;
extern GriColumn _colU;
extern GriColumn _colV;
extern GriColumn _colX;
extern GriColumn _colY;
extern GriColumn _colZ;
extern GriColumn _colWEIGHT;
#endif
extern bool     _uscale_exists;
extern bool     _vscale_exists;
extern bool     _columns_exist;
// Axes
extern double   _clipxleft, _clipxright, _clipybottom, _clipytop;
extern double   _cm_per_u;
extern double   _cm_per_v;
extern gr_axis_properties _xtype;

// Grid data
extern GriMatrix<double> _f_xy;
extern double     _f_min, _f_max;
extern GriMatrix<bool> _legit_xy;
extern GriMatrix<bool> _clipped_xy;
extern bool     _grid_exists;
extern string   _proj_command;
#if 0
// Commands
extern char    *_cmd_being_done_IP[];
extern int      _cmd_being_done_code[];
extern int      _cmd_being_done;
extern char    *_cmdLine;
extern char    *_cmdLineCOPY;
#endif

extern vector<string> _word;
/* Next two are shared to C functions, to navigate error messages. */
extern char *filename_runtime;
extern unsigned int fileline_runtime;
//extern vector<var> scalar_vars;
extern vector<var> vector_vars;

#if 0
extern char    *_Words2[];
extern char    *_Words3[];
// Misc
#endif
extern string    _contourFmt;
extern string    _current_directory;
extern char      _errorMsg[];
extern int       _error_action;
extern string    _lib_directory;
extern string    _margin;
extern string    _prompt;
extern double  *_dstack;
extern double   _gri_eof;
extern double   _grMissingValue;
extern double   _grMissingValue_tolerance;
extern double   _top_of_plot;
extern double   _axes_offset;
extern int      _axesStyle;
extern int      _braceLevel;
extern int      _chatty;
extern int      _clipData;
extern int      _debugFlag;
extern int      _done;
extern bool     _drawingstarted;
extern bool     _error_in_cmd;
extern int      _exit_value;
extern bool     _first;
extern gr_font  _font;
extern bool     _ignore_eof;
extern bool     _ignore_error;
extern int      _arrow_type;
extern bool     _warn_offpage;
extern vector<double> _dash;

// Numbers of things.
extern int      _num_command;
extern int      _nword;		/* in parser.y */
#if 0
// Version number.
extern double   _version; // this version
extern double   _version_expected; // expected version (if any)


// Flags indicating whether things exist yet.
extern bool     _use_default_for_query;


// Command stack
#define	COMMAND_STACK_SIZE	1000
typedef struct {
	char           *syntax;	// The 'name' of command
	char           *help;	// Help, if any
	char           *procedure;	// Commands to do
	char           *filename;	// Where defined
	int             fileline;	// Where defined
}               GRI_COMMAND;
extern GRI_COMMAND _command[COMMAND_STACK_SIZE];

#endif

/*
 * Things concerning x axis
 */
extern string   _xname;
extern double   _xleft;		// ..xleft..
extern double   _xright;	// ..xright..
extern double   _xinc;
extern string   _xFmt;
extern bool     _xFmt_set_by_user;
extern vector<double> _xmatrix;
extern bool     _xatbottom;
extern bool     _xincreasing;
extern int      _xsubdiv;
extern gr_axis_properties _xtype;
extern char     _xtype_map;
extern bool     _xgrid_exists;
extern bool     _xgrid_increasing;
extern bool     _xscale_exists;
extern bool     _user_set_x_axis;
extern bool     _need_x_axis;
/*
 * Things concerning y axis
 */
extern string   _yname;
extern double   _ybottom;	// ..ybottom..
extern double   _ytop;		// ..ytop..
extern double   _yinc;
extern string   _yFmt;
extern bool     _yFmt_set_by_user;
extern vector<double>  _ymatrix;
extern bool     _yatleft;
extern bool     _yincreasing;
extern int      _ysubdiv;
extern gr_axis_properties _ytype;
extern char     _ytype_map;
extern bool     _ygrid_exists;
extern bool     _ygrid_increasing;
extern bool     _yscale_exists;
extern bool     _user_set_y_axis;
extern bool     _need_y_axis;

#endif				// _extern_h_
