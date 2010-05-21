char *gre_version = "0.2.2";

#include <string>		/* for c++ string class */
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <string.h>
#if HAVE_UNISTD_H
#include <sys/types.h>
#include <unistd.h>
#endif
#include <sys/timeb.h>
#include <popt.h>

/* Internationalization */
#if defined(HAVE_LIBINTL)
#include <libintl.h>
#else
#define gettext(aa) (aa)
#endif

#if defined(HAVE_LIBPOPT)
#include <popt.h>
#endif

#include "parser.h"
#include "files.h"
#include "gr.h"
#include "types.h"
#include "defaults.h"
#include "macro.h"
#include "errors.h"
#include "GriState.h"
#include "extern.h"
#include "private.h"
#include "debug.h"

#if defined(HAVE_PROJ)
extern "C" {
#include "proj/projects.h" /* for 'proj' */
}
PJ *_proj_pj = NULL;
#endif

double _proj_scale_factor = 1.0;

char *optArg;

static bool user_set_lib_dir = false;
static vector<char*> argv_list;
GriState _griState;
int	 yyparse(void);
FILE	*cmd_file = NULL;
char    *cmd_file_name = NULL;
char    *ps_file_name = NULL;
bool	 _trace = false;
int      _optimization = 0;
bool     _give_warnings = false;
string   lib_dir = DEFAULT_GRE_DIR;
string   _proj_command = "";
unsigned int _gre_debug = 0;


#if defined(_MSC_VER)
char *usage = "SORRY, NO USAGE NOTES FOR PC VERSION";
#else
char *usage ="\
     Usage: gre OPTIONS ARGS\n\
\n\
   The `ARGS' normally consists of just one argument, the name of a\n\
command file containing `gre' commands to be executed.  Usually the\n\
command file name ends in the suffix `.gre'.  Thus one might invoke\n\
`gre' as\n\
\n\
     gre foo.gre\n\
\n\
to run the commands in the file `foo.gre'.  Any extra `ARGS'\n\
provided in addition to the command-file name are stored into the\n\
built-in vector `$ARGV'.  Thus, invoking `gre' as\n\
\n\
     gre foo.gre hi there\n\
\n\
defines `$ARGV[0]' as the string `hi' and `$ARGV[1]' as the string\n\
`there'.  This is often useful in `gre' programs designed to work on a\n\
series of datafiles; for more information see *Note Vector System\n\
Variables::.\n\
\n\
   Most applications involve drawing something, and the result is a\n\
PostScript file.  The name of this file is normally derived from the\n\
command-file name by switching the suffix to `.ps', e.g. the example\n\
above creates a PostScript file called `foo.ps', although the use of\n\
the `-p' command-line argument permits the use of any file-name (see\n\
below).\n\
\n\
   If no command-file is given, then standard-input is used and the\n\
PostScript output is saved in a file called `gre-00.ps', or, if that\n\
file exists, `gre-01.ps', etc.\n\
\n\
   The `OPTIONS' are as follows:\n\
\n\
`-h'\n\
`--help'\n\
     Print this help message.\n\
\n\
`-p psname'\n\
`--postscript psname'\n\
     Name the PostScript output file.\n\
\n\
`-d D'\n\
`--debug D'\n\
     Turn on various types of debugging:\n\
     `1'   -> print information about commands as they are parsed\n\
     `2'   -> print info when values are assigned to variables\n\
     `4'   -> print info when the OS is used, e.g. to create\n\
              temporary files\n\
     `8'   -> while parsing, print any comments that start with ##\n\
     `128' -> used by developer only; meaning changes without warning\n\
     `256' -> output 'yacc' info during parsing; only the Gre\n\
              developer will find this useful\n\
\n\
     The value of `D' may be the sum of some of the above, or the `-d'\n\
     flag may be repeated, to yield several of the listed behaviors.\n\
\n\
`-O opt'\n\
`--Optimize opt'\n\
     Set optimization level, `opt', as defined in the following list.\n\
\n\
     `0' -> no optimization (the default)\n\
\n\
`-t'\n\
`--trace'\n\
     Turn on tracing during scanning (not during execution!).\n\
\n\
`-w'\n\
`--warnings'\n\
     Turn on warnings.\n\
\n\
`-v'\n\
`-version'\n\
     Print version number and exit.\n\
\n\
`-e cmd'\n\
`--execute cmd'\n\
     Add indicated command(s) to a list of things to be done instead of\n\
     processing a command file.  Because the operating system will be\n\
     reading this information, `cmd' is normally enclosed in quote\n\
     marks.\n\
\n\
`-o OPTION'\n\
`--option OPTION'\n\
     Specify an OPTION chosen from this list:\n\
\n\
     `no_startup' - do not use the system `gre.cmd' startup file (only\n\
     the developer should use this flag)\n\
\n\
     `no_grerc' - do not use `~/.grerc' user-startup file\n\
\n\
`-L dir'\n\
`--Library dir'\n\
     Name directory where startup file gre.cmd is to be found,\n\
     over-riding the system default of `/usr/share/gre'.\n\
     NOTE: Multiple `--Library' commands are allowed,\n\
     the last one being used and previous ones ignored.\n\
\n\
--which_library\n\
     Reports which directory stores the library files, as\n\
     set by the --Library command or be default, and then exits.\n\
     This is normally used only by the gre-mode.el Emacs mode.\n\
\n\
For more help, type `info gre' or access file:/usr/doc/gre-VERSION_NUMBER/html\n\
";
#endif



bool no_startup = false;
bool no_grerc = false;
bool gave_e_arguments = false;
FILE *e_file = NULL;		// any commands from '-e' arguments
char *e_file_name = NULL;	// name of the file for '-e' arguments

string          _contourFmt;
string          _current_directory;
int             _error_action = 0; // 0=message/exit(1), 1=message/dump core/exit(1)
char            _errorMsg[1024]; // buffer used all over the place
string          _lib_directory;
string          _margin;
string          _prompt;
//char           *_word[MAX_nword];
//char           *_Words2[MAX_nword];
//char           *_Words3[MAX_nword];
double          _clipxleft, _clipxright, _clipybottom, _clipytop;
double          _cm_per_u, _cm_per_v;
double         *_dstack;
double          _gri_eof = 0.0;
double          _top_of_plot;
GriMatrix<double>  _f_xy;
double          _f_min, _f_max;

/*
 * Things concerning x axis
 */
string          _xname("x");
string          _yname("y");
double          _xleft = 0.0;
double          _xright = XSIZE_DEFAULT;
double          _xinc;
string          _xFmt;
bool            _xFmt_set_by_user = false;
vector<double>  _xmatrix;
bool            _xatbottom = true;
bool            _xincreasing = true;
int             _xsubdiv = 1;
gr_axis_properties _xtype = gr_axis_LINEAR;
char            _xtype_map = ' '; // could be "EWNS "
bool            _xgrid_exists;
bool            _xgrid_increasing;
bool            _xscale_exists;
bool            _user_set_x_axis = false;
bool            _need_x_axis = true;
/*
 * Things concerning y axis
 */
double          _ybottom = 0.0;
double          _ytop = YSIZE_DEFAULT;
double          _yinc;
string          _yFmt;
bool            _yFmt_set_by_user = false;
vector<double>  _ymatrix;
bool            _yatleft = true;
bool            _yincreasing = true;
int             _ysubdiv = 1;
gr_axis_properties _ytype = gr_axis_LINEAR;
char            _ytype_map = ' '; // could be "EWNS "
bool            _ygrid_exists;
bool            _ygrid_increasing;
bool            _user_set_y_axis = false;
bool            _need_y_axis = true;

double          _zinc;
double          _axes_offset = 0.0;
int             _axesStyle;
bool            _user_gave_bounding_box = false;
rectangle       _bounding_box_user;
rectangle       _bounding_box;	// in cm on page
int             _braceLevel = 0;
int             _chatty;
int             _clipData = 0;
int             _debugFlag = 0;		      // used in debugging
int             _done = 0;		      // 1=`quit'; 2=`return'
bool            _drawingstarted = false;
bool            _error_in_cmd = false;
int             _exit_value = 0;
gr_font         _font;
bool            _first = true;
bool            _ignore_eof = false;
bool            _ignore_error = false;
int             _arrow_type = 0;
bool            _warn_offpage = true;
vector<double>  _dash;

GriMatrix<bool>  _legit_xy;
int             _nword = 0;
/* numbers of items existing */
int             _num_command = 0;	/* commands */
/* Version number */
double          _version;	/* this version */
double          _version_expected = 0.0;	/* expected version (if any) */
/* flags for whether things exist yet (set in  */
bool            _columns_exist;
bool            _grid_exists;
bool            _uscale_exists;
bool            _vscale_exists;
bool            _yscale_exists;
bool            _use_default_for_query = false;

void initialize_variables();
static char* make_in_local_directory(char *fn);

static unsigned int debug_requested = 0;


int
main(int argc, char *argv[])
{
#if YYDEBUG == 1
	extern int yydebug;
	yydebug = 0;
#endif

#if defined(_MSC_VER)
	// Make up something reasonable if don't have any way to parse commandline options
	cmd_file_name = "test.gre";
	ps_file_name = "test.ps";
 
#else

	// On normal (unix-like) systems, can read commandline arguments.
	// Use different technique if have the linux-derived popt library.


#if defined(HAVE_LIBPOPT)
#define GETOPT_WHICH_LIBRARY 1000
	static struct poptOption optionsTable[] = {
		{ "version",        'v', POPT_ARG_NONE,   NULL, 'v' },
		{ "help",           'h', POPT_ARG_NONE,   NULL, 'h' },
		{ "debug",          'd', POPT_ARG_INT,    NULL, 'd' },
		{ "postscript",     'p', POPT_ARG_STRING, NULL, 'p' },
		{ "Library",        'L', POPT_ARG_STRING, NULL, 'L' },
		{ "which_library", '\0', POPT_ARG_NONE,   NULL, GETOPT_WHICH_LIBRARY },
		{ "trace",          't', POPT_ARG_NONE,   NULL, 't' },
		{ "Optimization",   'O', POPT_ARG_INT,    NULL, 'O' },
		{ "warnings",       'w', POPT_ARG_NONE,   NULL, 'w' },
		{ "execute",        'e', POPT_ARG_STRING, NULL, 'e' },
		{ "option",         'o', POPT_ARG_STRING, NULL, 'o' },
		{ 0, 0, 0, 0, 0}
	};
	poptContext optCon;
	optCon = poptGetContext("gre", argc, argv, optionsTable, 0);
	poptReadDefaultConfig(optCon, 1);
	int arg;
	while ((arg = poptGetNextOpt(optCon)) > 0) {
		optArg = poptGetOptArg(optCon);
		switch (arg) {
		case GETOPT_WHICH_LIBRARY:
			printf("%s\n", lib_dir.c_str());
			gre_exit(0);
		case 'h':
			//string_to_pager(usage);
			printf("%s\n", usage);
			gre_exit(0);
			break;
		case 'd':
			debug_requested = debug_requested | atoi(optArg);
			break;
		case 'L':
			lib_dir.assign(optArg);
			user_set_lib_dir = true;
			break;
		case 'p':
			if (ps_file_name != NULL && _give_warnings)
				printf("Warning: supplied multiple commandline options for PostScript filename; using last\n");
			ps_file_name = optArg;
			break;
		case 't':
			_trace = true;
			break;
		case 'O':
			_optimization = atoi(optArg);
			break;
		case 'w':
			_give_warnings = true;
			break;
		case 'e':
			gave_e_arguments = true;
			if (e_file == NULL) {
				e_file_name = tmp_file_name();
				e_file = fopen(e_file_name, "w");
				if (!e_file) {
					fprintf(stderr, "Cannot open temporary file for -e commandline argument\n");
					gre_exit(1);
				}
			}
			fprintf(e_file, "%s\n", optArg);
			printf("DEBUG: -e argument '%s'\n", optArg);
			break;
		case 'o':		// options
			if (!strcmp(optArg, "no_startup")) {
				no_startup = true;
			} else if (!strcmp(optArg, "no_grerc")) {
				no_grerc = true;
			} else {
				fprintf(stderr, "Unknown `-o' option `%s'\n", optArg);
				fprintf(stderr, "Usage: %s %s", argv[0], usage);
				gre_exit(1);
			}
			break;
		case 'v':
			printf("Gre version %s\n(c) 1997-2000 Dan E. Kelley\n", gre_version);
			gre_exit(0);
			break;
		default:
			printf(gettext("Unknown option\n"));
			break;
		}
	}
	char *last_option = poptBadOption(optCon,arg);
	if (arg <= 0 && *last_option == '-') {
		fprintf(stderr, "Unknown option `%s'.  Type `gre -h' for valid options\n", last_option);
		gre_exit(1);
	}
    
#else   // below is if don't have libpopt

	extern char    *optarg;
	extern int      optind;
	int             c;

	while ((c = getopt (argc, argv, "-hp:d:O:L:e:o:twv")) != -1) {
		switch (c) {
		case 'h':
			fprintf(stderr, "Usage: gre %s\n", usage);
			gre_exit(0);
			break;
		case 'd':
			debug_requested = debug_requested | atoi(optarg);
			break;
		case 'L':
			lib_dir.assign(optArg);
			user_set_lib_dir = true;
			break;
		case 'p':
			ps_file_name = optarg;
			break;
		case 't':
			_trace = true;
			break;
		case 'O':
			_optimization = atoi(optarg);
			break;
		case 'w':
			_give_warnings = true;
			break;
		case 'e':
			gave_e_arguments = true;
			if (e_file == NULL) {
				e_file_name = tmp_file_name();
				e_file = fopen(e_file_name, "w");
				if (!e_file) {
					fprintf(stderr, "Cannot open temporary file for -e commandline argument\n");
					gre_exit(1);
				}
			}
			fprintf(e_file, "%s\n", optarg);
			printf("DEBUG: -e argument '%s'\n", optArg);
			break;
		case 'o':		// options
			if (!strcmp(optarg, "no_startup")) {
				no_startup = true;
			} else if (!strcmp(optarg, "no_grerc")) {
				no_grerc = true;
			} else {
				fprintf(stderr, "Unknown `-o' option `%s'\n", optarg);
				fprintf(stderr, "Usage: %s %s\n", argv[0], usage);
				gre_exit(1);
			}
			break;
		case 'v':
			printf("Gre version %s (c) 1997-2000 Dan E. Kelley\n", gre_version);
			gre_exit(0);
			break;
		case '?':
			fprintf(stderr, "Usage: %s %s\n", argv[0], usage);
			return 0;
		}
	}
#endif // HAVE_LIBPOPT

	if (gave_e_arguments)
		fclose(e_file);

#if YYDEBUG == 1
	if (debug_requested & DEBUG_YACC_PARSING)
		yydebug = 1;
#endif

#if HAVE_LIBPOPT
	char *leftover_arg = poptGetArg(optCon);
	if (leftover_arg == NULL) {
		cmd_file = stdin;
		cmd_file_name = "stdin";
		if (ps_file_name == NULL)
			ps_file_name = "gre.ps"; // BUG -- should use version
	} else {
		// Have at least 1 argument -- take it as filename
		cmd_file_name = leftover_arg;
		cmd_file = fopen(cmd_file_name, "r");
		// If can't open, try appending ".gre" if it wasn't there
		if (!cmd_file) {
			char *suffix = strstr(cmd_file_name, ".gre");
			if (suffix == NULL) {
				string cmd_file_name_with_suffix(cmd_file_name);
				cmd_file_name_with_suffix = cmd_file_name_with_suffix + ".gre";
				cmd_file = fopen(cmd_file_name_with_suffix.c_str(), "r");
				if (!cmd_file) {
					fprintf(stderr, "Cannot open cmdfile '%s' or '%s'\n", cmd_file_name, cmd_file_name_with_suffix.c_str());
					gre_exit(1);
				}
				cmd_file_name = strdup(cmd_file_name_with_suffix.c_str());
			} else {
				fprintf(stderr, "Cannot open cmdfile '%s'\n", cmd_file_name);
				gre_exit(1);
			}
		}
	}
	// Save any other items on commandline into the 'argv_list'
	while (NULL != (leftover_arg = poptGetArg(optCon))) {
		if (_gre_debug & DEBUG_VARIABLES) printf("Adding commandline item `%s' to argv list\n", leftover_arg);
		argv_list.push_back(leftover_arg);
	}

#else  
	// Save the command-line arguments
	if (optind == argc) {
		cmd_file = stdin;
		cmd_file_name = "stdin";
		if (ps_file_name == NULL)
			ps_file_name = "gre.ps"; // BUG -- should use version
	} else {
		// Have at least 1 argument -- take it as filename
		cmd_file_name = argv[optind];
		cmd_file = fopen(argv[optind], "r");
		// If can't open, try appending ".gre" if it wasn't there
		if (!cmd_file) {
			char *suffix = strstr(cmd_file_name, ".gre");
			if (suffix == NULL) {
				char *name_with_suffix = new char[4 + strlen(cmd_file_name)];
				if (!name_with_suffix) OUT_OF_MEMORY;
				strcpy(name_with_suffix, cmd_file_name);
				strcat(name_with_suffix, ".gre");
				cmd_file = fopen(name_with_suffix, "r");
				if (!cmd_file) {
					fprintf(stderr, "Cannot open cmdfile '%s' or '%s.gre'\n", argv[argc - 1], argv[argc - 1]);
					gre_exit(1);
				}
				cmd_file_name = strdup(name_with_suffix);
			} else {
				fprintf(stderr, "Cannot open cmdfile '%s'\n", cmd_file_name);
				gre_exit(1);
			}
		}
	}
	// Save any other items on commandline into the 'argv_list'
	for (int iarg = optind + 1; iarg < argc; iarg++) {
		if (_gre_debug & DEBUG_VARIABLES) printf("Adding commandline item `%s' to argv list\n", argv[iarg]);
		argv_list.push_back(argv[iarg]);
	}

#endif // HAVE_LIBPOPT

#endif // _MSC_VER


	if (ps_file_name != NULL && debug_requested & (unsigned int)256)
		printf("DEBUG: will use supplied ps_file_name '%s'\n", ps_file_name);

	if (ps_file_name == NULL) {
		// Named a cmdfile but no other args
		int namelen = strlen(cmd_file_name);
		if (namelen > 4 && !strcmp(cmd_file_name + namelen - 4, ".gre")) {
			ps_file_name = strdup(cmd_file_name);
			ps_file_name[namelen - 3] = 'p';
			ps_file_name[namelen - 2] = 's';
			ps_file_name[namelen - 1] = '\0';
		} else {
			ps_file_name = new char[3 + namelen];
			strcpy(ps_file_name, cmd_file_name);
			strcat(ps_file_name, ".ps");
		}
		ps_file_name = make_in_local_directory(ps_file_name);
		if (debug_requested & (unsigned int)256)
			printf("DEBUG: made ps_file_name '%s'\n", ps_file_name);
	}
	gr_setup_ps_filename(ps_file_name);

	gr_setup_creatorname("Gre");
	gr_setup_creatorname("-");
	gr_setup_creatorname(gre_version);
	gr_setup_creatorname(" (c) 1999 Dan E. Kelley");

#if 0 // 1999 Aug 24: removed fancy stuff not done by other apps
	gr_setup_creatorname(" for ");
	if (egetenv("USER"))
		gr_setup_creatorname(egetenv("USER"));
	else
		gr_setup_creatorname("unknown user");
	gr_setup_creatorname(". [");
	gr_setup_creatorname(cmd_file_name);
	gr_setup_creatorname(" -> ");
	gr_setup_creatorname(ps_file_name);
	gr_setup_creatorname("]");
#endif

#if HAVE_GETPID
	srand48(getpid());
#endif

	extern unsigned int error_count;
	extern cmdnode* the_pgm;

	// Create some files
	// Standard input
	extern vector<bool>   file_input;
	extern vector<string> file_handle;
	extern vector<string> file_name;
	extern vector<FILE*>  file_pointer;
	file_input.push_back(true);
	file_handle.push_back("STDIN");
	file_pointer.push_back(stdin);
	file_name.push_back("stdin");
	// Standard output
	file_input.push_back(false);
	file_handle.push_back("STDOUT");
	file_pointer.push_back(stdout);
	file_name.push_back("stdout");
	// Standard error
	file_input.push_back(false);
	file_handle.push_back("STDERR");
	file_pointer.push_back(stderr);
	file_name.push_back("stderr");
    
	_nword = 0;
	_gre_debug = debug_requested;

	initialize_variables();
	gr_begin(1);

	// Order of naming precedence: 
	//   if user set with -L, use that.  Otherwise, use
	//   value in DEFAULT_GRE_DIR.  NOTE: use *last* value of -L,
	//   if multiples given.
	string grecmd_name;

	grecmd_name = lib_dir;
	grecmd_name.append("/gre.cmd");

	//printf("DEBUG(gre.cpp): lib_dir is '%s'\n", lib_dir.c_str());

	// Process startup file, unless told not to
	if (!no_startup) {
		FILE *startup_file = fopen(grecmd_name.c_str(), "r");
		if (startup_file) {
			push_cmdfile("gre.cmd", startup_file);
			while (!feof(startup_file))
				yyparse();
			evaluate_cmd(the_pgm);
			pop_cmdfile();      // need to, since loop prevents seeing EOF
			fclose(startup_file);
		} else {
			fprintf(stderr, "gre: cannot locate startup file %s\n", grecmd_name.c_str());
			gre_exit(1);
		}
	}
    
	// Process ~/.grerc file, if it exists, unless told not to
	if (!no_grerc) {
		string filename(egetenv("HOME"));
		filename.append("/.grerc");
		FILE *grerc_file = fopen(filename.c_str(), "r");
		if (grerc_file) {
			push_cmdfile(".grerc", grerc_file);
			while (!feof(grerc_file))
				yyparse();
			pop_cmdfile();      // need to, since loop prevents seeing EOF
			evaluate_cmd(the_pgm);
			fclose(grerc_file);
		}
	}

	//
	// Possibly process file of -e commands
	if (gave_e_arguments) {
		printf("DEBUG: should now do the e_arguments\n");
		e_file = fopen(e_file_name, "r");
		if (!e_file) {
			fprintf(stderr, "Cannot reopen e_file `%s'\n",e_file_name);
			gre_exit(1);
		}
		push_cmdfile(e_file_name, e_file);
		while (!feof(e_file))
			yyparse();
		pop_cmdfile();      // need to, since loop prevents seeing EOF
		fclose(e_file);
		evaluate_cmd(the_pgm);
		call_OS_unlink(e_file_name);
		//end_up();
		printf("DEBUG: ... done with the e_arguments\n");
		//gre_exit(0);
	}
	// Process command file.
	push_cmdfile(cmd_file_name, cmd_file);
    
	if (_gre_debug & DEBUG_YACC_PARSING) printf("Parsing file `%s' ...\n", cmd_file_name);

	// BUG -- might not work, since cmd_file could call several modules??

	while (!feof(cmd_file))
		yyparse();
	pop_cmdfile();      // need to, since loop prevents seeing EOF

	start_up();
	if (_gre_debug & DEBUG_YACC_PARSING) printf("Executing `%s' ...\n\n", cmd_file_name);
	if (error_count) {
		printf("Execution of %s aborted due to compilation errors.\n", cmd_file_name);
	} else {
		if (_gre_debug & DEBUG_TEMPORARY) {
			printf("Exiting at end of parse, because '-d 128' given\n");
			gre_exit(0);
		}
		evaluate_cmd(the_pgm);
	}
	end_up();
	gre_exit(0);
	return 0;			// never executed
}

bool word_is(int w, const char *s)
{
	int which = get_vector_var_index("$_");
	if (which < 0) {
		fprintf(stderr, "INTERNAL ERROR: somehow $_ became undefined\n");
		gre_exit(1);
	}
	return !strcmp(s, vector_vars[which].get_text_vector(w));
}

// *** put stuff from 'startup.cc:set_defaults' here, and not it
// NOTE: *must* have definitions of $_ and $_[] at start
void initialize_variables()
{
	put_var("$_", "");		// scalar
	printf("\nAAA %s:%d AAA\n",__FILE__,__LINE__);
	put_var("$_arrow_size", ARROWSIZE_DEFAULT); // REMOVE LATER
	put_var("$_exit_status", 0.0); // REMOVE LATER
	printf("\nBBB %s:%d BBB\n",__FILE__,__LINE__);
	put_var("$_", "", -1);	// vector
	printf("\nCCC %s:%d CCC\n",__FILE__,__LINE__);
	// put_var("$_", "", 1);	// vector  -- BUG why isn't above enough?
	put_var("$_x", 0.0, -1);	// the -1 means don't fill in
	printf("\nDDD %s:%d DDD\n",__FILE__,__LINE__);
	put_var("$_y", 0.0, -1);
	put_var("$_z", 0.0, -1);
	put_var("$_w", 0.0, -1);
	put_var("$_u", 0.0, -1);
	put_var("$_v", 0.0, -1);
	put_var("$_arrow_size", ARROWSIZE_DEFAULT);
	put_var("$_exit_status", 0.0);
	put_var("$_font_size", FONTSIZE_PT_DEFAULT);
	gr_setfontsize_pt(FONTSIZE_PT_DEFAULT);
	gr_setfont(FONT_DEFAULT);
	put_var("$_graylevel", 0.0);
	gr_setmissingvalue(MISSING_VALUE);
	put_var("$_missing_value", gr_currentmissingvalue());
	put_var("$_symbol_size", SYMBOLSIZE_DEFAULT);
	put_var("$_linewidth", LINEWIDTH_DEFAULT);
	put_var("$_linewidth_axis", LINEWIDTHAXIS_DEFAULT);
	put_var("$_linewidth_symbol", LINEWIDTHSYMBOL_DEFAULT);
	put_var("$_xmargin", XMARGIN_DEFAULT);
	put_var("$_xsize", XSIZE_DEFAULT);
	put_var("$_xleft", 0.0);
	put_var("$_xright", XSIZE_DEFAULT);
	put_var("$_ymargin", YMARGIN_DEFAULT);
	put_var("$_ytop", XSIZE_DEFAULT);
	put_var("$_ybottom", 0.0);
	put_var("$_ysize", XSIZE_DEFAULT);
	put_var("$_tic_direction", 0.0); // out
	put_var("$_tic_size", TICSIZE_DEFAULT);
	put_var("$_red", 0.0);
	put_var("$_green", 0.0);
	put_var("$_blue", 0.0);

	put_var("$_eof", 0.0);
	put_var("$_version", gre_version);
	extern char* cmd_file_name;
	put_var("$_command_file", cmd_file_name);

	put_var("$_pi", 2.0 * asin(1.0));
	put_var("$_e",  exp(1.0));

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
	string the_time(asctime(localtime(&sec)));
	the_time[the_time.size() - 1] = '\0';
	put_var("$_time", the_time.c_str());

	// from 'set_defaults', no longer used
	_contourFmt.assign(CONTOUR_FMT_DEFAULT);
	_current_directory.assign(".");
	_axesStyle = 0;
	_contourFmt.assign(CONTOUR_FMT_DEFAULT);
	_current_directory.assign(".");
	// Set up various other defaults
	_xFmt.assign(X_FMT_DEFAULT);// commented out
	_yFmt.assign(Y_FMT_DEFAULT);
	_chatty = 1;
	create_builtin_colors();
	int argv_list_len = int(argv_list.size());
	put_var("$ARGV", "", -1);
	for (int i = 0; i < argv_list_len; i++) {
		if (debug_requested & (unsigned int)256)	printf("storing $ARGV[%d] as '%s'\n", i, argv_list[i]);
		put_var("$ARGV", (const char*)argv_list[i], i);
	}
}


static char*
make_in_local_directory(char *fn)
{
	unsigned int l = strlen(fn);
	int i;
	for (i = l - 1; i > -1; i--)
		if (fn[i] == '/')
			return fn + i + 1;
	return fn;
}

