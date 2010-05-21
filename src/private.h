// -*- mode: c++ -*-

#if !defined(_private_)
#define		_private_

#include        <vector>
#include	<stdio.h>

#if 0
#include	<stdlib.h>
#include        <stdarg.h>
#include        "gr.hh"
#include        "gr_coll.hh"
#include        "errors.hh"
#include        "files.hh"
#endif

#include "types.h"

#if 0
#include        "GriTimer.hh"
#define         cmd_being_done_LEN       100	// cmd stack
#define		LineLength	       32768	// max length of line
#define		LineLength_1	       32767	// -1 + max length of line
#define		MAX_cmd_word		8192	// words/command
#define		MAX_nword		4096	// words/line
#define		_num_dstackMAX		  50	// length of dstack
#endif


// Image storage.
#define		_imageBLANK		 255	// blank images with white
typedef struct {
	int             ras_magic;	// magic number
	unsigned int    ras_width;	// width (pixels) of image
	unsigned int    ras_height;	// height (pixels) of image
	unsigned int    ras_depth;	// depth (1, 8, or 24 bits) of pixel
	unsigned int    ras_length;	// length (bytes) of image
	int             ras_type;	// type of file; see RT_* below
	int             ras_maptype;// type of colormap; see RMT_* below
	int             ras_maplength; // length (bytes) of following map
	unsigned char *map;	   // map
	unsigned char *image;	   // image
}               IMAGE;

// From Sun's /usr/include/rasterfile.h
#define	RAS_MAGIC	0x59a66a95
#define RT_STANDARD	1
#define RMT_NONE        0
#define RMT_EQUAL_RGB   1
#define RMT_RAW         2

bool            allocate_grid_storage(int nx, int ny);
bool            allocate_image_storage(int nx, int ny);
bool            allocate_imageMask_storage(int nx, int ny);
#if 0
bool            assign_synonym(void);
bool            batch(void);
#endif
bool            blank_image(void);
bool            blank_imageMask(void);
#if 0
int             block_level(void);
char           *block_source_file(void);
unsigned int    block_source_line(void);
unsigned int    block_offset_line(void);
void            bounding_box_display(const char *msg);
#endif
void            bounding_box_update(const rectangle& box);
char*           call_getenv(const char* e);
int             call_the_OS(const char* the_cmd);
int             call_OS_unlink(const char* file);
FILE*           call_popen(const char* the_cmd, const char* the_mode);
bool            calculate_image_histogram(void);
void            check_psfile(void);
void            chop_into_words(char *s, char **w, int *nw, int max);

#if 0
void            clear_eof_flag_on_data_file(void);
#endif
void            close_data_files();

unsigned int cmd_parameters();
bool cmd_parameter_is_n(unsigned int i);
double cmd_parameter_n(unsigned int i);
char* cmd_parameter_s(unsigned int i);

#if 0
int             cmd_being_done(void);
char           *complete_filename(const char *name);
#endif

void            create_builtin_colors(void);

#if 0
bool            create_commands(const char *filename);
bool            create_color(const char *name, double r, double g, double b);
bool            create_new_command(FILE * fp, char *line);
bool            create_synonym(const char *name, const char *value);
bool            create_variable(const char *name, double value);
#endif

bool            create_x_scale(void);
bool            create_y_scale(void);

#if 0
char           *cshell_name(const char *s);
int             data_file_index(const char *name);
#endif

bool            define_image_scales(double llx, double lly, double urx, double ury);
#if 0
bool            delete_file(const char *filename);
bool            delete_syn(const char *name);
bool            delete_var(const char *name);
#endif
void            demonstrate_command_usage(void);
#if 0
void            display_cmd_being_done_stack();
void            display_cmd_stack(const char *s);
void            display_data_stack(const char *s);
void            display_unused_var(void);
void            display_unused_syn(void);
bool            do_command_line(void);
#endif

bool            draw_axes(int type, double loc, gr_axis_properties side, bool allow_offset);
bool            draw_axes_if_needed(void);

#if 0
bool            draw_gri_logo(void);
#endif

char           *egetenv(const char *s);
void            end_up(void);

#if 0
void            err(const char *string, ...); // COMMENTED OUT
void            expand_blanks(char *cmdline); // COMMENTED OUT
int             ExtractQuote(char *sout, const char *s); // COMMENTED OUT
#endif

bool            extremal_values(int vector_index, double *min, double *max);


#if 0
char           *file_in_list(const char *name, bool show_nonlocal_files, bool show_local_files);
const char     *filename_sans_dir(const char *fullfilename);
bool            find_min_max(double *data, int num, double *min, double *max);
bool            find_min_max_v(void);
bool            find_min_max_x(void);
bool            find_min_max_y(void);
bool            find_min_max_z(void);
#endif

char            first_unwhite(const char *s);
bool            intersection(double x0, double y0, double x1, double y1,
			     double X0, double Y0, double X1, double Y1,
			     double *x, double *y);
char            last_unwhite(const char *s);
char*           tmp_file_name(void);

#if 0
int             get_cmd_values(char **w, int nw, const char *key, int nobjects, double *objects);
bool            get_c_file_name(int old, const char *prompt, const char *name);
bool            get_command_line(void);
#endif
bool            getdnum(const char *string, double *number);
int             get_flag(const char *name);
#if 0
bool            getinum(const char *string, int *number);
//... OLD bool            get_var(const char *name, double *value);
//... OLD bool            get_syn(const char *name, char *value);
#endif
double get_var_scalar(const char *n);
double get_var_vector(const char *n, int index);
#if 0
void            give_help(void);
#endif
void            gr_setmissingvalue(double x);
double          gr_currentmissingvalue(void);
void            gr_textput(const char *s);
void            gri_abort(void);
void            gre_exit(int code);
#if 0
bool            grid_interp(double xx, double yy, double *value);
#endif
bool            grid_exists(void);
#if 0
bool            gr_missing(double x);
bool            handle_if_block(void);
#endif
unsigned int    hash(const char *s);
#if 0
void            highpass_image(void);
#endif
void            histogram_stats(const std::vector<double> &x, unsigned int n, double *q1, double *q2, double *q3);
void            initialize(void);
bool            image_exists(void);
bool            image_mapping_exists(void);
bool            image_range_exists(void);
bool            imageMask_exists(void);
bool            image_scales_defined(void);
double          image_to_value(int c);
bool            inside_box(double x, double y);
bool            look_up_color(const char *name, double *red, double *green, double *blue);
#if 0
void            lowpass_image(void);
bool            massage_command_line(char *cmdline);
int             match_gri_syntax(const char *cmdline, int flag);
bool            mathCmd(void);
#endif

void            extrema_of_matrix(double *min, double *max);
void            extrema_of_xcol(double *min, double *max);
void            extrema_of_ycol(double *min, double *max);

#if 0
void            moment(double *data, int n, double *ave, double *adev, double *sdev, double *svar, double *skew, double *kurt);
void            more_file_to_terminal(const char *filename);
#endif
void            no_scales_error(void);
#if 0
int             number_good_xyz(double x[], double y[], double f[], int n);
unsigned int    number_missing_cols(void);
int             parse_C_commandCmd(const char *s);
bool            perform_block(const char *s, const char *source_file, int source_line);
bool            perform_command_line(FILE * fp, bool is_which);
int             perform_gri_cmd(int cmd);
bool            perform_gri_program(void);
bool            perform_while_block(const char *buffer, const char *test, int lines);
void            pop_command_word_buffer(void);
void            pop_cmd_being_done_stack(void);
void            push_cmd_being_done_stack(int cmd);
bool            push_cmd_file(const char *fname, bool interactive, bool allow_warning, const char *status);
void            push_command_word_buffer(void);
bool            put_var(const char *name, double value, bool replace_existing);
bool            put_syn(const char *name, const char *value, bool replace_existing);
bool            pop_data_file(int file);
#endif
void            print_columns();
void            print_grid();
bool            proj_lonlat_to_xy(double lat[], double lon[], unsigned int num, double x[], double y[]);
#if 0
bool            push_data_file(const char *name, data_file_type type, const char *status, bool delete_when_close);
bool            push_data_file_to_top(const char *name);
#endif
char           *pwd(void);
double          quantize(double x, int levels, double dx);
#if 0
bool            quoted(const char *string);
#endif

bool            start_up();
// Equation of state for seawater
double          sw_freezing_temperature(double S, double p);
double          sw_density(double S, double T, double p);
double          sw_potential_temperature(double S, double t, double p, double pref);
double          sw_lapse_rate(double S, double t, double p);
double          sw_salinity(double S, double t, double p);


#if 0
bool            re_compare(const char *string, const char *pattern);
void            remove_comment(char *cmdline);
void            remove_esc_quotes(char *w);
void            remove_trailing_blanks(char *s);
bool            rpn_create_function(char *name, char **w, unsigned int nw);
bool            same_syntax(const char *cmdline, const char *syntax, int flag);
bool            same_word(const char *cp, const char *sp);
#endif
bool            scales_defined(void);
void print_buf(FILE *fp, const char *buf); // print with newlines decoded
void            set_x_axis_style(int xstyle);
void            set_y_axis_style(int ystyle);
bool            set_environment(void);
bool            set_flagCmd(void);
#if 0
void            set_eof_flag_on_data_file(void);
#endif
void            set_line_width_axis(void);
void            set_line_width_curve(void);
void            set_line_width_symbol(void);
void            set_ps_color(char path_or_text, bool force = false);
#if 0
void            set_up_command_word_buffer(void);
#endif
bool            set_x_scale(void);
bool            set_y_scale(void);
#if 0
bool            show_next_lineCmd(void);
void            show_var_stack(void);
void            show_syn_stack();
int             skip_space(const char *s);
int             skip_nonspace(const char *s);
bool            skipping_through_if(void);
bool            stop_replay_if_error(void);
void            strcat_c(char *s, int c);
bool            string_is_blank(const char *s);
#endif

void            string_to_pager(const char* s);

#if 0
bool            substitute_rpn_expressions(const char *cmdline, char *cmdlinecopy);
bool            substitute_synonyms_cmdline(const char *s, char *sout, bool allow_math);
bool            substitute_synonyms(const char *s, char *sout, bool allow_math);
int             superuser(void);
#endif
void            swap(double& a, double& b);
#if 0
bool            systemCmd(void);
char           *tilde_expand(const char *s);
bool            tracing(void);
bool            update_readfrom_file_name(void);
#endif
bool            using_proj(void);
#if 0
char           *unbackslash(const char *s);
unsigned char value_to_image(double v);
void		vector_reverse(double *x, int n);
#endif
double          vector_min(double *x, unsigned n);
double          vector_max(double *x, unsigned n);

const char* itoa(int i);
const char* dtoa(double d);

#if defined(VMS)
void
warning(va_dcl va_alist);
#else
void            warning(const char *string, ...);
#endif
#if 0
bool            warn_if_slow(GriTimer *t, double fraction_done, const char *cmd);
int             what_line(void);
char           *what_file(void);
#endif
bool            well_ordered(double min, double max, double inc_big, double inc_small);
#if 0
bool            word_is(int i, const char *word);
void            write_prompt(void);

#endif /* debug */

#endif				// not _private_
