/* -*- mode: c++ -*-*/
%{

#include <string>		/* for c++ string class*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <ctype.h>

#include <stdarg.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_GLOB)
#include <glob.h>
#endif

#include "macro.h"
bool save_string = false;
#include "parser.h"
#include "tags.h"
#include "subs.h"
#include "scalar.h"
#include "private.h"
#include "gr_coll.h"
#include "debug.h"

using namespace std;

bool print_comments=true;
char s_delimiter = '/';

bool in_my = false;
string trace_buffer = "";
// Next three (file_...) linked
vector<bool>   file_input;
vector<string> file_handle;
vector<string> file_name;
vector<FILE*>  file_pointer;
vector<string> pipe_tmpfile;	// separate from above

#define NP 0x00c /* newpage character (control-L) */

// See gre.cc
extern bool _give_warnings;
extern int _optimization;

// Next three used to keep track of possibly nested functions.
vector<int> function_takes_min_arg;
vector<int> function_takes_max_arg;
vector<int> function_which;

// Next are about the input command file
static FILE *cmdfile_fp = NULL;
static vector<FILE*> cmdfile_fp_save;
static char *cmdfile_name = NULL; /* no storage assoc with this*/
static vector<char*> cmdfile_name_save;
static unsigned int cmdfile_line = 1;
static vector<int> cmdfile_line_save;
static int cmdfile_line_position = 0;
static vector<int> cmdfile_line_position_save;
static int cmdfile_parse_where = 0;	// where in the parse-string
static vector<int> cmdfile_parse_where_save;
bool at_eof = true;
static string use_name = "";

/* Next two are shared to C functions, to navigate error messages. */
char *filename_runtime = NULL;	
unsigned int fileline_runtime = 0;


/* Store words on this line.  Use _nword to count number of words, and*/
/* never delete extraneous words.  This prevents time wastage, in removing*/
/* strings and then recreating them, as control moves from short commands*/
/* to long ones.*/
extern int _nword;
vector<string> _word;

static bool expecting_matchstring = false;
static bool expecting_prototype = false;
static bool defining_sub_or_cmd = false;
unsigned int debug = 0;

const unsigned int error_countMAX = 2;

void
fatal_error(const char *msg)
{
    printf("%s at %s line %d.\n", msg, filename_runtime, fileline_runtime);
    gre_exit(1);
}

void
fatal_error(const char *msg, const char *msg2)
{
    printf("%s%s at %s line %d.\n", msg, msg2, filename_runtime, fileline_runtime);
    gre_exit(1);
}

void
fatal_error(const char *msg, const char *msg2, const char *msg3)
{
    printf("%s%s%s at %s line %d.\n", msg, msg2, msg3, filename_runtime, fileline_runtime);
    gre_exit(1);
}

void
fatal_error(const char *msg, const char *msg2, const char *msg3, const char *msg4)
{
    printf("%s%s%s%s at %s line %d.\n", msg, msg2, msg3, msg4, filename_runtime, fileline_runtime);
    gre_exit(1);
}
void
fatal_error(const char *file, unsigned int line, const char *msg)
{
    if (file[0] == '\0')
	printf("%s", msg);
    else
	printf("%s at %s line %d.\n", msg, file, line);
    gre_exit(1);
}
void
runtime_warning(const char *file, unsigned int line, const char *msg)
{
    if (file[0] == '\0')
	printf("%s", msg);
    else
	printf("%s at %s line %d.", msg, file, line);
}
void
runtime_warning(const char *msg)
{
    printf("%s at %s line %d.\n", msg, cmdfile_name, cmdfile_line);
}

// Return name of command.  NOTE: this MUST be kept up-to-date
// if (parser.h:struct cmdnode) is changed.
char*
name_of_cmd(int type)
{
    switch (type) {
    case -1:  return "blank";
    case -2:  return "unkown";
    case -3:  return "assign_text_vector";
    case -4:  return "last_block";
    case -5:  return "next_block";
    case -6:  return "C_command";
    case -7:  return "delete_var_list";
    case -8:  return "do_while_block";
    case -9:  return "my_var_list";
    case -10: return "expression";
    case -11: return "for_block";
    case -12: return "foreach_block";
    case -13: return "if_block";
    case -14: return "user_cmd";
    case -15: return "new_var";
    case -16: return "print";
    case -17: return "printf";
    case -18: return "proj";
    case -19: return "query";
    case -20: return "read_var_list";
    case -21: return "return_block";
    case -22: return "split";
    case -23: return "sub_block";
    case -24: return "while_block";
    case -25: return "chop";
    case -26: return "read_columns";
    case -27: return "read_grid_data";
    case -28: return "read_grid_x";
    case -29: return "read_grid_y";
    case -30: return "package";
    case -31: return "use";
    }
    return "";
}


#define TRACE printf("%s:%d: %s\n\n", $$->file_name, $$->line, trace_buffer.c_str()); trace_buffer = "";



// enode::eval() BEGINS
scalar enode::eval()			// -- evaluate as a number
{
	//printf("debug: eval() called. the_type %d variable=%d match_a_string=%d\n", the_type, variable,match_a_string);
	static scalar rval;
	rval.set_numeric(0.0);		// initially guess numeric zero
	//printf("enode::eval() the_type %d (constant %d)\n",the_type,enode::constant);
	switch (the_type) {
	case assign_list_vector: {// rval is number of items
		//printf("%s:%d: doing assign_list_vector (to %d-th vector)\n",__FILE__,__LINE__,variable_position_vector);
		wnode *list = sub_arg_list;
		int index = 0;
		while (list != NULL) {
			//printf("should assign something\n");
			if (list->e->is_textual()) {
				// Patterned on parser.y:614
				const char *vv = interpolate_var_in_string(list->e->eval_as_text().get_text());
				put_var_vector_text_at(variable_position_vector, index, vv);
			} else {
				put_var_vector_numeric_at(variable_position_vector, index, list->e->eval().get_numeric());
			}
			list = list->next;
			index++;
			//printf("done assigning at index %d\n",index-1);
		}
		rval.set_numeric(index);
	}
		break;
	case variable: // eval()
		//printf("about to eval a variable at %d\n", variable_position_scalar);
		rval = scalar_vars[variable_position_scalar].get_at_index(0);
		//printf("  got rval.n %f  rval.s '%s'\n", rval.n,rval.get_text());
	
		break;
	case scalar_postdec: {	// eval()
		double val = 0.0;	// default for string
		if (scalar_vars[variable_position_scalar].get_at_index(0).is_numeric())
			val = get_var_scalar_numeric_at(variable_position_scalar);
		rval.set_numeric(val);
		val -= 1.0;
		put_var_scalar_numeric_at(variable_position_scalar, val);
	}
	break;
	case scalar_postinc: {	// eval()
		double val = 0.0;	// default for string
		if (scalar_vars[variable_position_scalar].get_at_index(0).is_numeric())
			val = scalar_vars[variable_position_scalar].get_numeric_scalar();
		rval.set_numeric(val);
		val += 1.0;
		scalar_vars[variable_position_scalar].set_value(val);
	}
	break;
	case lt:
		rval.set_numeric(left->eval().get_numeric() < right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case le:
		rval.set_numeric(left->eval().get_numeric() <= right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case gt:
		rval.set_numeric(left->eval().get_numeric() > right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case ge:
		rval.set_numeric(left->eval().get_numeric() >= right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case eq:
		rval.set_numeric(left->eval().get_numeric() == right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case ne:
		rval.set_numeric(left->eval().get_numeric() != right->eval().get_numeric() ? 1.0 : 0.0);
		break;
	case pattern_variable: {
		extern vector<var> pattern_match;
		if ((unsigned int)which_handle >= pattern_match.size())
			rval.set_numeric(0.0);
		else {
			string tmp;
			pattern_match[which_handle].get_text_scalar(tmp);
			rval.set_numeric(atof(tmp.c_str()));
		}
	}
        break;
	case constant:	// eval()
		rval.set_numeric(constant_numerical_value);
		break;
	case text_index: {
		char *str = strdup(interpolate_var_in_string(left->eval_as_text().get_text()));
		char *substr = strdup(interpolate_var_in_string(right->eval_as_text().get_text()));
		int offset=(middle==NULL) ? 0 : int(middle->eval().get_numeric());
		if (offset < 0)
			offset = 0;
		if (offset >= int(strlen(str))) {
			rval.set_numeric(-1.0);
		} else {
			char *loc = strstr(str + offset, substr);
			if (loc == NULL)
				rval.set_numeric(-1.0);
			else
				rval.set_numeric(double(int(loc - str)));
		}
		free(str);
		free(substr);
	}
	break;
	case file_test: {
#if defined(HAVE_LSTAT)
		struct stat buf;
#endif
		rval.set_numeric(0.0);	// done above but what the heck
		// see 'man -s 2 stat'
		// ???? BUG: use stat() or lstat() ???
	
		// S_IFDIR directory
		// S_IFREG regular
		switch (file_test_flag) {
		case 'd':
#if defined(HAVE_LSTAT)
			if (0 == lstat(interpolate_var_in_string(left->eval_as_text().get_text()), &buf)) {
				if ((buf.st_mode & S_IFMT) == S_IFDIR)
					rval.set_numeric(1.0);
			}
#else
			rval.set_numeric(0.0);	// ????? guess
#endif
			break;
		case 'f':
#if defined(HAVE_LSTAT)
			if (0 == lstat(interpolate_var_in_string(left->eval_as_text().get_text()), &buf)) {
				if ((buf.st_mode & S_IFMT) == S_IFREG)
					rval.set_numeric(1.0);
			}
#else
			rval.set_numeric(0.0);	// ????? guess
#endif
			break;
		case 'r':	// ??? BUG ??? what if not owner?
		{
			FILE *fp = fopen(interpolate_var_in_string(left->eval_as_text().get_text()), "r");
			if (fp) {
				rval.set_numeric(1.0);
				fclose(fp);
			}
		}
		break;
		default:
			printf("Internal ERROR in file_test\n");
		}
	}
	break;
	case text_glob: {
#if defined(HAVE_GLOB)          // maybe faster or somehow better
		int glob_return_val;
		glob_t globbuf;
		glob_return_val = glob(interpolate_var_in_string(left->eval_as_text().get_text()), GLOB_ERR | GLOB_MARK, NULL, &globbuf);
		vector<string> items;
		for (unsigned int i = 0; i < globbuf.gl_pathc; i++)
			items.push_back(string(globbuf.gl_pathv[i]));
#else
		string cmd = "ls -c1 ";
		cmd += interpolate_var_in_string(left->eval_as_text().get_text());
		cmd += " > ";
		char* tmpfile = tmp_file_name();
		cmd += tmpfile;
		call_the_OS(cmd.c_str());
		FILE *f = fopen(tmpfile, "r");
		vector<string> items;
		char word[100];
		while (1 == fscanf(f, "%s", word))
			items.push_back(string(word));
		cmd = "rm -f ";
		cmd += tmpfile;
		call_the_OS(cmd.c_str());
#endif
		scalar r;
		//r.free_storage();       // allow for zero size REMOVED 1999-jul-20
		unsigned int len = items.size();
		for (unsigned int i = 0; i < len; i++)
			r.set_text(i, items[i].c_str());
		rval = r;
	}
	break;
	case system_call:
		rval.set_numeric(double(call_the_OS(interpolate_var_in_string(left->eval_as_text().get_text()))));
		break;
	case skip_a_line:
	{
		printf("Doing skip handle= %d (%lx)\n", which_handle, (unsigned long)file_pointer[which_handle]);
		int skip_c = '0';
		while ('\n' != (skip_c = fgetc(file_pointer[which_handle]))) {
			printf(" skipping '%c'\n", skip_c);
		}
		rval.set_numeric(0.0);
	}
	break;
	case rewind_file:
		double rewind_data_file(int which_handle);
		rval.set_numeric(rewind_data_file(which_handle));
		put_var("$_eof", 0.0);
		break;
	case open_file:
		double open_data_file(const char *filename, int which_handle);
		rval.set_numeric(open_data_file(interpolate_var_in_string(left->eval_as_text().get_text()), which_handle));
		put_var("$_eof", 0.0);
		break;
	case close_file:
		//printf("DEBUG: which_handle=%d\n",which_handle); 
		//printf("DEBUG: fp= %x\n",file_pointer[which_handle]); 
		if (file_pointer[which_handle] != NULL
		    && fclose(file_pointer[which_handle]))
			rval.set_numeric(0.0);
		else
			rval.set_numeric(1.0);
		put_var("$_eof", 0.0);
		break;
	case text_chop: {
		char *endchar = strdup(" ");
		char *s = strdup(get_var_scalar_text_at(variable_position_scalar));
		endchar[0] = s[-1 + strlen(s)];
		s[-1 + strlen(s)] = '\0';
		put_var_scalar_text_at(variable_position_scalar, s);
		free(s);
		rval.set_text(endchar);
	}
        break;
	case text_chomp: {
		char *s = strdup(get_var_scalar_text_at(variable_position_scalar));
		//printf("doing NUMERIC text_chomp.  variable_position_scalar=%d.  initial '%s'\n",variable_position_scalar,s);
		int len, l;
		len = strlen(s);
		l = len;
		while (l > 1 && s[l - 1] == '\n')
			l--;
		if (l != len && l > 1) {
			//printf("before trim '%s'  (l=%d)    ", s, l);
			s[l] = '\0';
			//printf("after trim '%s'\n", s);
			put_var_scalar_text_at(variable_position_scalar, s);
		}
		free(s);
		rval.set_numeric(double(len - l));
		//printf(" ... text_chomp returning %d\n", len - l);
	}
        break;
	case die: {
		if (left != NULL) {
			string msg(interpolate_var_in_string(left->eval_as_text().get_text()));
			print_buf(stdout, msg.c_str());
			unsigned int len = msg.length();
			if (!(len > 2 && '\\' == msg[len - 2] && 'n' == msg[len - 1]))
				printf(" at %s line %d.\n",file_name, file_linenumber);
		} else {
			printf("Died at %s line %d.\n", file_name, file_linenumber);
		}
		end_up();
		gre_exit(255);		/* exit code matches perl*/
	}
        break;
	case exit_fcn:
		end_up();
		gre_exit(int(left->eval().get_numeric()));
		break;
	case unlink_files: {
		int files = 0;
		wnode *f = sub_arg_list;
		while (f != NULL) {
			int uval;
			string fname = interpolate_var_in_string(f->e->eval_as_text().get_text());
			//printf("UNLINKING '%s'\n", fname.c_str());
			uval = unlink(fname.c_str());
			f = f->next;
			if (uval == 0)
				files++;
		}
		rval.set_numeric(files);
		break;
	}
	case warn: {
		string msg(interpolate_var_in_string(left->eval().get_text()));
		print_buf(stdout, msg.c_str());
		unsigned int len = msg.length();
		if (!(len > 2 && '\\' == msg[len - 2] && 'n' == msg[len - 1]))
			printf(" at %s line %d.\n",file_name, file_linenumber);
		rval.set_numeric(1.0);
	}
        break;
	case text_eq: {	// eval()
		string s1 = interpolate_var_in_string(left->eval_as_text().get_text());
		string s2 = interpolate_var_in_string(right->eval_as_text().get_text());
		//printf("text_eq: s1 '%s'    s2 '%s'\n",s1.c_str(),s2.c_str());
		rval.set_numeric(double(s1 == s2));
	}
        break;
	case text_ne: {		// eval()
		string s1 = interpolate_var_in_string(left->eval_as_text().get_text());
		string s2 = interpolate_var_in_string(right->eval_as_text().get_text());
		//printf("text_ne: s1 '%s'    s2 '%s'\n",s1.c_str(),s2.c_str());
		rval.set_numeric(double(!(s1 == s2)));
	}
        break;
	case assign_scalar:	// eval()
		//printf("%s:%d doing assign_scalar...\n",__FILE__,__LINE__);
		if (right->is_textual()) { // known as textual at compiletime...
			//printf("DEBUG 1: right textual\n");
			rval.set_text(interpolate_var_in_string(right->eval_as_text().get_text()));
			put_var_scalar_text_at(variable_position_scalar, rval.get_text());
		} else {	// but at runtime still might be textual.
			//printf("DEBUG 2: right not textual\n");
			if (!right->eval().is_numeric()) {
				//printf("DEBUG 2a\n");
				rval.set_text(interpolate_var_in_string(right->eval().get_text()));
				put_var_scalar_text_at(variable_position_scalar, rval.get_text());
				//printf("DEBUG 2aa '%s'\n",right->eval().get_text());
				//put_var_scalar_text_at(variable_position_scalar, "DAN DAN DAN");
			} else {
				//printf("DEBUG 2B: is_numeric? %d\n",right->eval().is_numeric());
				double value = right->eval().get_numeric();
				double old = 0.0;
				if (assignment_op != 0)
					old = get_var_scalar_numeric_at(variable_position_scalar);
				switch (assignment_op) {
				case 0:		// =
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 1:		// +=
					value = old + value;
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 2:		// -=
					value = old - value;
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 3:		// *=
					value = old * value;
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 4:		// /=
					value = old / value;
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 5:		// **=
					value = pow(old, value);
					put_var_scalar_numeric_at(variable_position_scalar, value);
					rval.set_numeric(value);
					break;
				case 6:		//  =~
				{
				//printf("case 6 LHS '%s'\n", get_var_scalar_text_at(variable_position_scalar));
				//printf("right->left '%s'\n", right->left->eval_as_text().get_text());
				//printf("right->right '%s'\n", right->right->eval_as_text().get_text());
#if 1
					char *from = strdup(interpolate_var_in_string(right->left->eval_as_text().get_text()));
					if (!from) OUT_OF_MEMORY;
				//printf("from '%s'\n",from);
					if (strlen(from) > 0) {
						char *to = strdup(interpolate_var_in_string(right->right->eval_as_text().get_text()));
						if (!to) OUT_OF_MEMORY;
						char *to_tmp = strdup(to);
						if (!to_tmp) OUT_OF_MEMORY;
						if (!to_tmp) OUT_OF_MEMORY;
						strcpy(to_tmp, to);
						bool ignore_case = false, repeat = false;
						
						unsigned int len = strlen(to_tmp);
						while (len &&
						       (to_tmp[len - 1] == 'i'
							|| to_tmp[len - 1] == 'g'
							|| to_tmp[len - 1] == '/')) {
							if (to_tmp[len - 1] == 'i')
								ignore_case = true;
							if (to_tmp[len - 1] == 'g')
								repeat = true;
							to_tmp[len - 1] = '\0';
							len--;
						}
						
						string flag("/");
						if (ignore_case)
							flag.append("i");
						string string_to_change = get_var_scalar_text_at(variable_position_scalar);
						bool regexp_match(const char *re, const char *s, const char *f, int subexp_max, regmatch_t *matchptr, int *subexpressions);
						
#define SUBEXP_MAX 100
						regmatch_t matchptr[SUBEXP_MAX];
						int subexpressions = 0;
						
						unsigned int start = 0;
						unsigned int to_len = strlen(to_tmp);
						unsigned int matches = 0;
						do {
							bool match = regexp_match(from, string_to_change.c_str() + start, flag.c_str(), SUBEXP_MAX, matchptr, &subexpressions);
							if (match) {
								int sub_length = matchptr[0].rm_eo - matchptr[0].rm_so;
								string_to_change.replace(matchptr[0].rm_so + start, sub_length, to_tmp);
								start = start + matchptr[0].rm_so + (to_len - sub_length) + 1;
								matches++;
							} else {
								break;
							}
						} while (repeat);
						put_var_scalar_text_at(variable_position_scalar, string_to_change.c_str());
						rval.set_text(string_to_change.c_str());
						//rval.set_numeric(0.0);
						free(from);
						free(to);
						free(to_tmp);
					} else {
						// Nothing to do
						rval.set_text(get_var_scalar_text_at(variable_position_scalar));
						//rval.set_numeric(0);
					}
#endif
				//rval.set_text("TEST");
				}
				break;
				default:
					yyerror("Unknown assignment operator");
				}
			}
		}
		break;
	case resize_vector: {	// eval()
		//printf("eval() resize_vector ...\n");
		if (right->is_textual()) {
			yyerror("INTERNAL ERROR: cannot handle textual RHS to this command\n");
		} else {
			int newsize = 1 + int(floor(0.5 + right->eval().get_numeric()));
			//printf("newsize to be %d\n", newsize);
			if (assignment_op != 0)
				yyerror("INTERNAL error: can only handle op '='\n");
			put_var_vector_size(variable_position_vector, newsize);
			rval.set_numeric(newsize);
		}
	}
	break;
	case assign_vector: {	// eval()
		//printf("eval() assign_vector ...\n");
		int index = int(0.5 + left->eval().get_numeric());
		//printf("eval() assign_vector --- 1\n");
		//printf("\neval() of assign_vector, variable_position_vector = %d, at index %d\n",variable_position_vector, index);
		if (right->is_textual()) {
			const char *vv = interpolate_var_in_string(right->eval_as_text().get_text());
			put_var_vector_text_at(variable_position_vector, index, vv);
			rval.set_text(vv);
			//printf("\t... assigned as text '%s'\n",vv);
		} else {
			double value = right->eval().get_numeric();
			double old = 0.0;
			if (assignment_op != 0)
				old = get_var_vector_numeric_at(variable_position_vector, index);
			switch (assignment_op) {
			case 0:				break; // =
			case 1: value = old + value;	break; // +=
			case 2: value = old - value;    break; // -=
			case 3: value = old * value;    break; // *=
			case 4: value = old / value;    break; // /=
			case 5: value = pow(old, value);break; // **=
			default:
				yyerror("Unknown assignment operator");
			}
			put_var_vector_numeric_at(variable_position_vector, index, value);
			rval.set_numeric(value);
		}
	}
	//printf("-- done with assign to vector\n");
	break;
	case question:
	{
		bool test = middle->eval().is_numeric() ? true : false;
		//printf("TEST result is %d\n", test);
		if (test) {
			//printf("TRUE\n");
			if (left->eval().is_numeric()) {
				//printf("NUMERIC...\n");
				rval.set_numeric(left->eval().get_numeric());
				//printf(" ... NUMERIC value %f\n",rval.n);
			} else {
				//printf("TEXTUAL...\n");
				rval.set_text(left->eval().get_text());
				//printf(" ... TEXTUAL value '%s'\n", rval.get_text());
			}
		} else {
			//printf("FALSE\n");
			if (right->eval().is_numeric()) {
				//printf("NUMERIC...\n");
				rval.set_numeric(right->eval().get_numeric());
				//printf(" ... NUMERIC value %f\n",rval.n);
			} else {
				//printf("TEXTUAL...\n");
				rval.set_text(right->eval().get_text());
				//printf(" ... TEXTUAL value '%s'\n", rval.get_text());
			}
		}
		//if (rval.is_numeric()) printf("set rval to %f\n",rval.n); else printf("set rval to '%s'\n",rval.get_text());
	}
	break;
	case add:
		rval.set_numeric(double(left->eval().get_numeric() + right->eval().get_numeric()));
		break;
	case sub:
		rval.set_numeric(double(left->eval().get_numeric() - right->eval().get_numeric()));
		break;
	case mul:
		rval.set_numeric(double(left->eval().get_numeric() * right->eval().get_numeric()));
		break;
	case div:
		rval.set_numeric(double(left->eval().get_numeric() / right->eval().get_numeric()));
		break;
	case remainder:
		rval.set_numeric(double(fmod(left->eval().get_numeric(), right->eval().get_numeric())));
		break;
	case power:
		rval.set_numeric(double(pow(left->eval().get_numeric(), right->eval().get_numeric())));
		break;
	case function_builtin_numeric_0:
		rval.set_numeric(double(func_of_0()));
		break;
	case function_builtin_numeric_1:
		rval.set_numeric(double(func_of_1(left->eval().get_numeric())));
		break;
	case function_builtin_numeric_2:
		rval.set_numeric(double(func_of_2(left->eval().get_numeric(), right->eval().get_numeric())));
		break;
	case function_builtin_numeric_3:
		rval.set_numeric(double(func_of_3(left->eval().get_numeric(), right->eval().get_numeric(), middle->eval().get_numeric())));
		break;
	case function_builtin_numeric_4:
		rval.set_numeric(double(func_of_4(left->eval().get_numeric(), right->eval().get_numeric(), middle->eval().get_numeric(), extra->eval().get_numeric())));
		break;
	
	
	case function_builtin_text_1:
		rval.set_numeric(double(func_of_1_text(interpolate_var_in_string(left->eval().get_text()))));
		break;

	case function_builtin_text_2:
	{
		string arg1(interpolate_var_in_string(left->eval().get_text()));
		string arg2(interpolate_var_in_string(right->eval().get_text()));
		rval.set_numeric(func_of_2_text(arg1.c_str(), arg2.c_str()));
	}
	break;

	case function_user: {
		extern scalar the_return_value;
		the_return_value.set_numeric(0.0); // default
		increase_block_level();
		wnode *arg;
		arg = sub_arg_list;
		int args = 0;
		while (arg != NULL) {
			if (arg->e->is_textual()) {
				put_var_vector_text_at(0, args, arg->e->eval_as_text().get_text());
			} else {
				put_var_vector_numeric_at(0, args, arg->e->eval().get_numeric());
			}
			args++;
			arg = arg->next;
		}
		extern vector<user_sub> user_sub_stack;
		cmd_eval_code evaluate_cmd(cmdnode *c);
		evaluate_cmd(user_sub_stack[sub_index].get_action());
		bool delete_var(const char *n);
		reduce_block_level();
		//rval = set_numeric(double(the_return_value));
		rval = the_return_value;
		//printf("\n");
		//printf("function_user %s:%d return value numeric %s [%s]\n",__FILE__,__LINE__,rval.is_numeric()?"yes":"no",rval.get_text());
	}
	break;
	case negative:
		rval.set_numeric(double(- left->eval().get_numeric()));
		break;
	case boolean_not:
		rval.set_numeric(double(!left->eval().get_numeric()));
		break;
	case boolean_and:
		rval.set_numeric(double(left->eval().get_numeric() && right->eval().get_numeric()));
		break;
	case boolean_or:
		rval.set_numeric(double(left->eval().get_numeric() || right->eval().get_numeric()));
		break;
	case variable_at_i: {	// eval()
		int index = int(0.5 + left->eval().get_numeric());
		//printf("\n\neval() variable_at_i, index=%d   variable_position_vector=%d\n", index,variable_position_vector);
		rval = vector_vars[variable_position_vector].get_at_index(index);
		//printf("\tVALUE %f '%s'\n",rval.n,rval.get_text()); 
	}
	break;
	case vector_postinc: {
		int index = int(0.5 + left->eval().get_numeric());
		double val = 0.0;	// zero is default for string
		if (vector_vars[variable_position_vector].get_at_index(index).is_numeric())
			val = vector_vars[variable_position_vector].get_at_index(index).get_numeric();
		rval.set_numeric(val);
		val += 1.0;
		vector_vars[variable_position_vector].set_value(val, index);
	}
	break;
	case vector_postdec: {
		int index = int(0.5 + left->eval().get_numeric());
		double val = 0.0;	// zero is default for string
		if (vector_vars[variable_position_vector].get_at_index(index).is_numeric())
			val = vector_vars[variable_position_vector].get_at_index(index).get_numeric();
		rval.set_numeric(val);
		val -= 1.0;
		vector_vars[variable_position_vector].set_value(val, index);
	}
	break;
	case variable_end: /* eval()*/
		//printf("\nin eval for variable_end at position '%d'\n", variable_position_vector);
		if (variable_position_vector >= 0) {
			//printf("\tseemingly vector, value %f.\n",get_var_vector_end(variable_position_vector));
			rval.set_numeric(double(get_var_vector_end(variable_position_vector)));
		} else {
			//printf("\tseemingly scalar\n");
			rval.set_numeric(-1.0);	// scalar
		}
		break;
	case text:	// eval()
		//printf("TRYING TO EVAL() A HANDLE_READ\n");
		if (text_from_file) {
			//printf("doing a text_from_file.  which_handle = %d\n",which_handle);
			extern vector<FILE*> file_pointer;
			FILE *fp = file_pointer[which_handle];
			//printf("\t that handle is '%s'\n", file_handle[which_handle].c_str());
			//printf("\t that fp is %x (%x)\n", file_pointer[which_handle],fp);
			char buffer[1000]; // BUG -- size!
			if (!feof(fp))
				fgets(buffer, 999, fp);
			if (feof(fp))
				buffer[0] = '\0';
			if (text_to_underline) {
				//printf("PUT '%s' into $_\n", buffer);
				put_var_scalar_text_at(0, buffer);
			}
			rval.set_text(buffer);
		} else {
			rval.set_text(textual_value);
		}
		break;
/*
 *  case question:
 *    printf("Evaling a TEXTUAL ?:.  test='%s'   left='%s'  right='%s'\n",middle->eval().s,left->eval().s,right->eval().get_text());
 *    rval.s = middle->eval().s ? left->eval().s : right->eval().s;
 *    rval.is_numeric = false;
 *    break;
 */
	case text_dot: {
		string rr(left->eval_as_text().get_text());
		rr += right->eval_as_text().get_text();
		rval.set_text(rr.c_str());
	}
	break;
	case text_sprintf: {	// called as number
		//printf("DEBUG: executing text_sprintf as a number...\n");
		char* do_sprintf(const wnode* list);
		rval.set_text(do_sprintf(sub_arg_list));
	}
	break;
	case text_substr: {
		//printf("\n\tDEBUG: evaluating text_substr as a number...\n");
		string rr(left->eval_as_text().get_text());
		//printf("\t as text full string is '%s'\n",rr.c_str());
		int length = rr.length();
		int offset = int(right->eval().get_numeric());
		if (offset < 0)
			offset += length;
		//printf("\toffset = %d\n", offset);
		//printf("\tlength = %d\n", length);
		if (offset < length - 1) {
			if (middle) {
				int length = int(middle->eval().get_numeric());
				if (length > 0) {
					rr = rr.substr(offset, length); // Plauger: 17.5.1.54
					rval.set_text(rr.c_str());
				} else {
					rval.set_text("");
				}
			} else {
				rr = rr.substr(offset, rr.length() - offset);
				rval.set_text(rr.c_str());
			}
		} else {
			rval.set_numeric(0.0);
		}
	}
	break;
	case change_a_string: {
		char *from = strdup(interpolate_var_in_string(left->eval_as_text().get_text()));
		if (!from) OUT_OF_MEMORY;
		if (strlen(from) > 0) {
			char *to = strdup(interpolate_var_in_string(right->eval_as_text().get_text()));
			if (!to) OUT_OF_MEMORY;
			char *to_tmp = strdup(to);
			if (!to_tmp) OUT_OF_MEMORY;
			if (!to_tmp) OUT_OF_MEMORY;
			strcpy(to_tmp, to);
			bool ignore_case = false, repeat = false;
            
			unsigned int len = strlen(to_tmp);
			while (len &&
			       (to_tmp[len - 1] == 'i'
				|| to_tmp[len - 1] == 'g'
				|| to_tmp[len - 1] == '/')) {
				if (to_tmp[len - 1] == 'i')
					ignore_case = true;
				if (to_tmp[len - 1] == 'g')
					repeat = true;
				to_tmp[len - 1] = '\0';
				len--;
			}

			string flag("/");
			if (ignore_case)
				flag.append("i");

			//printf("  change_a_string from='%s' to='%s   flag='%s'  repeat=%d ignore_case=%d\n", from, to_tmp, flag.c_str(), repeat, ignore_case);
			string string_to_change = get_var_scalar_text_at(0);
			//printf("  $_ is '%s'\n", string_to_change.c_str());
			bool regexp_match(const char *re, const char *s, const char *f, int subexp_max, regmatch_t *matchptr, int *subexpressions);
            
#define SUBEXP_MAX 100
			regmatch_t matchptr[SUBEXP_MAX];
			int subexpressions = 0;
                          
			unsigned int start = 0;
			unsigned int to_len = strlen(to_tmp);
			unsigned int matches = 0;
			do {
				//printf("\nstart=%d working on '%s'\n", start, string_to_change.c_str() + start);
				bool match = regexp_match(from, string_to_change.c_str() + start, flag.c_str(), SUBEXP_MAX, matchptr, &subexpressions);
				if (match) {
					int sub_length = matchptr[0].rm_eo - matchptr[0].rm_so;

					//printf(" to_len=%d sub_length=%d %d:'%s'(%s) -> ", to_len, sub_length, start,string_to_change.c_str(),string_to_change.c_str()+start);

					string_to_change.replace(matchptr[0].rm_so + start, sub_length, to_tmp);

					start = start + matchptr[0].rm_so + (to_len - sub_length) + 1;

					//printf(" %d:'%s'(%s)\n", start, string_to_change.c_str(), string_to_change.c_str()+start);
					matches++;

				} else {
					break;
				}
			} while (repeat);

			put_var_scalar_text_at(0, string_to_change.c_str());

			rval.set_text("");
			rval.set_numeric(matches);

			free(from);
			free(to);
			free(to_tmp);

		} else {
			// Nothing to do
			rval.set_text("");
			rval.set_numeric(0);
		}
	}
	break;
	case match_a_string: {
		extern vector<var>    pattern_match;
		extern vector<int>    pattern_match_length;
		//printf("match_a_string: the string is '%s'\n", interpolate_var_in_string(left->eval_as_text().get_text()));
		//printf("match_a_string: $_ is '%s'\n", get_var_scalar_text_at(variable_position_scalar));

		bool regexp_match(const char *re, const char *s, const char *f, int subexp_max, regmatch_t *matchptr, int *subexpressions);

#define SUBEXP_MAX 100
		regmatch_t matchptr[SUBEXP_MAX];
		int subexpressions = 0;
		const char *re = interpolate_var_in_string(left->eval_as_text().get_text());
		const char *to_search = get_var_scalar_text_at(variable_position_scalar);

		//printf("match_a_string: re '%s'\n", re);
		//printf("match_a_string: to_search '%s'\n", to_search);
		//printf("match_a_string: variable_position_scalar %d\n", variable_position_scalar);

		if (strlen(to_search) > 0) {
			bool match = regexp_match(re, to_search, variable_name, SUBEXP_MAX, matchptr, &subexpressions);
			// First clear any existing
			if (pattern_match_length.size() > 0) {
				int to_clear = pattern_match_length[-1 + pattern_match_length.size()];
				if (pattern_match.size() < (unsigned int)to_clear) {
					INTERNAL_ERROR("cannot clean out nonexistent match patterns\n");
				}
				pattern_match.erase(pattern_match.end() - to_clear, pattern_match.end());
				pattern_match_length.pop_back();
			}
			if (match) {
				// buffer for subexpressions be large enough to hold all
				char *buf = (char*)malloc(sizeof(char) * strlen(to_search));
				if (!buf) OUT_OF_MEMORY
						  for (int i = 0; i < subexpressions; i++) {
							  char name[10];
							  int sub_length = matchptr[i+1].rm_eo - matchptr[i+1].rm_so;
							  strncpy(buf, to_search + matchptr[i+1].rm_so, sub_length);
							  buf[sub_length] = '\0';
							  //printf("\t\tbuf '%s' starts at '%s' copy %d\n", buf, to_search+matchptr[i+1].rm_so,sub_length);
							  sprintf(name, "%d", i + 1);
							  var v(name, buf);
							  pattern_match.push_back(v);
							  //printf("\t%s is '%s'; pattern_match.size %d\n", name, to_search,pattern_match.size());
							  //printf("    '%s' <- '%s' (%d -- %d)\n", name, buf, matchptr[i+1].rm_so, matchptr[i+1].rm_eo);
						  }
				free(buf);
				pattern_match_length.push_back(subexpressions);

				//printf("match_a_string: $NUMBER list at end ...\n"); for (int vv = 0; vv < pattern_match.size(); vv++) printf("\t$%s \"%s\"\n", pattern_match[vv].get_name(),pattern_match[vv].get_text_scalar());printf("\n");

			} else {
				pattern_match_length.push_back(0);
			}

			rval.set_numeric(double(match));
		} else {
			rval.set_numeric(0.0);
		}
	}
	break;
    
	default: {
		char msg[100];
		sprintf(msg, "ERROR trying to eval() an expression: type=%d Not coded yet\n", the_type);
		yyerror(msg);
		rval.set_numeric(0.0);
	}
	}
	return rval;
}
// enode::eval() ENDS

int				// NOT SURE THIS IS EVER CALLED!!
print_command(FILE *fp, const wnode* arg_list)
{
	if (arg_list == NULL) { 
		printf("BUG: don't know how to print $_ fix this later\n");
		return 0;
	} else {
		while (arg_list != NULL) {
			printf("DEBUG: please email author with this line as subjectline: %s:%d\n",__FILE__,__LINE__);
			if (arg_list->e->get_type() == enode::text
			    || arg_list->e->get_type() == enode::text_dot
			    || arg_list->e->get_type() == enode::text_substr) {
				print_buf(fp, interpolate_var_in_string(arg_list->e->eval().get_text()));
			} else {
				char b[100];
				sprintf(b, "%.14g", arg_list->e->eval().get_numeric());
				print_buf(fp, b);
			}
			arg_list = arg_list->next;
		}
	}
	return 1;
} 

// Info used by the lexer, to see if an item is a WORD or CMDWORD
int cmd_matches_to = -1;
unsigned int cmd_num_matches = 0;
static unsigned int cmd_word_index = 0;
static bool cmd_is_expected_next = true;
void
cmd_expected_next()
{
    cmd_matches_to = -1;
    cmd_num_matches = 0;
    cmd_matches_to = 0;
    cmd_word_index = 0;
    cmd_is_expected_next = true;
    //printf(" ~~~ cmd_expected_next() called; set cmd_word_index=0\n");
}
void
cmd_not_expected_next()
{
    cmd_matches_to = -1;
    cmd_num_matches = 0;
    cmd_is_expected_next = false;
}

// Look up filehandle, returning index.  If not found, and
// if create_if_not_found is true, create the filehandle and
// return index.  If not found but create_if_not_found is false,
// return -1.
int
look_up_file_handle(const char *name, bool create_if_not_found)
{
    unsigned int i;
    extern vector<string> file_handle;
    for (i = 0; i < file_handle.size(); i++)
	if (file_handle[i] == name)
	    break;
    if (i < file_handle.size())
	return i;
    // Didn't find it.  Possibly create new one.
    if (!create_if_not_found)
	return -1;
    file_input.push_back(true); // ??
    file_handle.push_back(name);
    file_pointer.push_back((FILE*)NULL);

    // Warn if not all-caps
    const char *n = name;
    while (*n) {
	if (islower(*n)) {
	    compile_warning("file-handle `", name, "' contains lower-case character(s)", NULL);
	    break;
	}
	n++;
    }
    return i;
} 


vector<var> scalar_vars;
vector<var> vector_vars;
vector<user_sub> user_sub_stack;


void
ensure_variable_known(const char *varname)
{
    int which = get_scalar_var_index(varname);
    if (which < 0) {
	var *v = new var();
	scalar_vars.push_back(*v);
	if (_gre_debug & DEBUG_VARIABLES) printf("DEBUG: ensure_variable_known(%s) stored at index %d\n", varname, get_scalar_var_index(varname));
    } else {
	if (_gre_debug & DEBUG_VARIABLES) printf("DEBUG: ensure_variable_known(%s) noted existing index %d\n", varname, which);
    }
}


void
show_var_stack()
{
	/* Scalars*/
	int len = scalar_vars.size();
	printf("BUG: the following list of variables will be wrong if you use 'my' variables anywhere in your program!!!\n");
	printf("Scalars, numbering %d in total\n-------------------------------\n", len);
	int i;
	for (i = len - 1; i > -1; i--) {
		const char *name = scalar_vars[i].get_name();
		//if (*name == '\0') printf("(%2d) un-named (empty)\n", i+1);
		if (*name != '\0') {
			printf("(%2d) %20s = ", i, name);
			if (!scalar_vars[i].get_at_index(0).is_numeric()) {
				string tmp;
				scalar_vars[i].get_text_scalar(tmp);
				printf("\"%s\"\n", tmp.c_str());
			} else {
				printf("%.14g\n", scalar_vars[i].get_numeric_scalar());
			}
		}
	}
	printf("\nVectors\n-------\n");
	/* Vectors*/
	len = vector_vars.size();
	const int max_to_show = 10;
	for (i = len - 1; i > -1; i--) {
		unsigned int num = vector_vars[i].get_size();
		const char *name = vector_vars[i].get_name();
		if (*name != '\0') {
			string newname(vector_vars[i].get_name());
			newname[0] = '@';
			printf("(%2d) %20s[0..%d] = ", i, newname.c_str(), num - 1);
			if (num == 0) {
				printf("[empty]\n");
			} else {
				printf("[\n");
				for (unsigned int j = 0; j < num; j++) {
					printf("                           ");
					if (!vector_vars[i].get_at_index(j).is_numeric()) {
						printf("\"%s\"\n", vector_vars[i].get_text_vector(j));
					} else {
						printf("%.14g\n", vector_vars[i].get_numeric_vector(j));
					}
					if (j == max_to_show - 1) {
						printf(" ...\n");
						break;
					}
				}
				printf("                       ]\n");
			}
		}
	}
}

bool
put_var(const char *n, double val)
{
	printf("HERE-in-put_var\n");
	printf("%s:%d  put_var(%s, %f) scalar_var/double len=%d\n",__FILE__,__LINE__,n,val,int(scalar_vars.size()));
	if (_gre_debug & DEBUG_VARIABLES) printf("put_var assigned %f to %s\n", val, n);
	extern vector<var> scalar_vars;
	int len = scalar_vars.size();
	/* If existing, replace it*/
	for (int i = len - 1; i > -1; i--) {
		if (!strcmp(n, scalar_vars[i].get_name())) {
			scalar_vars[i].set_value(val);
			return true;
		}
	}
#if 0
	var *v = new var(n, val);
	scalar_vars.push_back(*v);
	delete v;
#else
	printf("at %s:%d\n",__FILE__,__LINE__);
	printf("n='%s'\n",n);
	printf("val=%f ABOUT TO CONSTRUCT A VAR\n",val);
	var v(n,val);
	printf("at %s:%d (constructed v ok.  ABOUT TO PUSH IT TO scalar_vars)\n",__FILE__,__LINE__);
	//scalar_vars.push_back(var(n,val));
	scalar_vars.push_back(v);
	printf("at %s:%d (pushed ok)\n",__FILE__,__LINE__);
#endif
	return true;
}
void
clear_vector_var(const char *n)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("clear_vector_var cleared %s[...]\n", n);
    extern vector<var> vector_vars;
    int len = vector_vars.size();
    for (int i = len - 1; i > -1; i--)
	if (!strcmp(n, vector_vars[i].get_name())) {
	    vector_vars[i].clear_vector();
	    break;
	}
}

/* If index<0, don't fill in value */
bool
put_var(const char *n, double val, int index)
{
	extern vector<var> vector_vars;
	printf("%s:%d put_var(%s, %f, %d) vector_var/double len=%d\n",__FILE__,__LINE__,n,val,index,int(vector_vars.size()));
	if (_gre_debug & DEBUG_VARIABLES) printf("put_var() assigned %f to %s[%d]\n", val, n, index);
	int len = vector_vars.size();
	/* If existing, replace it*/
	//printf("  <%s>\n",vector_vars[0].get_name());
	for (int i = len - 1; i > -1; i--) {
		printf("    i=%d\n",i);
		printf("    check '%s'...\n", vector_vars[i].get_name());
		if (!strcmp(n, vector_vars[i].get_name())) {
			vector_vars[i].set_value(val, index);
			return true;
		}
	}
	printf("1.\n");
	var *v = new var(n, 0.0);	/* the 0 is to get right constructor*/
	printf("2.\n");
	v->set_value(val, index);
	printf("3.\n");
	vector_vars.push_back(*v);
	printf("4.\n");
	delete v;
	printf("5.\n");
	return true;
}

bool
put_var(const char *n, const char *val)
{
	printf("%s:%d put_var(%s,%s) scalar_var/string len=%d\n",__FILE__,__LINE__,n,val,int(scalar_vars.size()));
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var assigned \"%s\" to %s\n", val, n);
    extern vector<var> scalar_vars;
    int len = scalar_vars.size();
    /* If existing, replace it*/
    for (int i = len - 1; i > -1; i--) {
	if (!strcmp(n, scalar_vars[i].get_name())) {
	    scalar_vars[i].set_value(val);
	    printf("  replaced existing at i=%d\n",i);
	    return true;
	}
    }
    printf("ONE\n");
    var *v = new var(n, val);
    printf("TWO\n");
    scalar_vars.push_back(*v);
    printf("THREE\n");
    delete v;
    printf("  created new scalar_var len=%d\n",int(scalar_vars.size()));
    return true;
}

bool
put_var(const char *n, const char *val, int index)
{
	printf("%s:%d put_var(%s,%s,%d) vector_var/string len=%d\n",__FILE__,__LINE__,n,val,index,vector_vars.size());
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var assigned \"%s\" to %s[%d]\n", val, n, index);
    extern vector<var> vector_vars;
    int len = vector_vars.size();
    /* If existing, replace it*/
    //printf("\nSTRING put_var(%s,%s,%d)\n",n,val,index);
    for (int i = len - 1; i > -1; i--) {
	if (!strcmp(n, vector_vars[i].get_name())) {
	    vector_vars[i].set_value(val, index);
	    printf("  replaced existing at i=%d\n",i);
	    return true;
	}
    }
    //printf("\t\tso, created new vector at index %d\n", len);
    var *v = new var(n, "");
    v->set_value(val, index);
    vector_vars.push_back(*v);
    delete v;
    printf("  created new scalar_var len=%d\n",scalar_vars.size());
    return true;
}

/* return index of variable, or -1 if not found*/
int
get_scalar_var_index(const char* n)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("get_scalar_var_index(%s) hash=%d\n", n, hash(n));
    static string nn;
    nn = n;	/* do not delete later*/
    if (nn[1] == '#')
	nn.STRING_ERASE(1, 1);
    if (nn == "$_")
	return 0;		// See gre.cc:initialize
    extern vector<var> scalar_vars;
    int len = scalar_vars.size();

//NOT USED:    unsigned int h = hash(n);
    // 0 used for $_ 
    for (int i = len - 1; i > 0; i--) {
	if (_gre_debug & DEBUG_VARIABLES) printf("  check on %d-th, '%s' hash=%d ... ", i, scalar_vars[i].get_name(), scalar_vars[i].get_hash());
	if (nn == scalar_vars[i].get_name_string()) {
	    if (_gre_debug & DEBUG_VARIABLES) printf(" MATCH\n");
	    return i;
	}
	    if (_gre_debug & DEBUG_VARIABLES) printf(" different\n");
    }
    return -1;
}

/* return index of variable, or -1 if not found*/
int
get_vector_var_index(const char* n)
{
    static string nn;
    nn = n;	/* do not delete later*/
    if (nn[1] == '#')
	nn.STRING_ERASE(1, 1);
    if (nn == "$_")
	return 0;		// See gre.cc:initialize
    extern vector<var> vector_vars;
    int len = vector_vars.size();
    for (int i = len - 1; i > 0; i--) { // 0 used for $_
	if (nn == vector_vars[i].get_name_string()) {
	    return i;
	}
    }
    return -1;
}

double
get_var_vector(const char *n, int index)
{
    static string nn;
    nn = n;
    if (nn[1] == '#')
	nn.STRING_ERASE(1, 1);
    extern vector<var> vector_vars;
    int i, len = vector_vars.size();
    for (i = len - 1; i > -1; i--)
	if (nn == vector_vars[i].get_name_string())
	    return vector_vars[i].get_numeric_vector(index);
    fprintf(stderr, "ERROR: get_var_vector() cannot find variable named `%s'", n);
    return 0.0;
}

double
get_var_scalar(const char *n)
{
    static string nn;
    nn = n;
    if (nn[1] == '#')
	nn.STRING_ERASE(1, 1);
    extern vector<var> scalar_vars;
    int i, len = scalar_vars.size();
    for (i = len - 1; i > -1; i--)
	if (nn == scalar_vars[i].get_name_string())
	    return scalar_vars[i].get_numeric_scalar();
    if (_give_warnings)
	printf("Use of uninitialized variable `%s'; the value 0 use\n", n);
    return 0.0;
}
double
get_var_scalar_numeric_at(int which)
{
    if (which >= 0 && which < (int)scalar_vars.size())
	return scalar_vars[which].get_numeric_scalar();
    else
	return 0.0;
}
const char*
get_var_scalar_text_at(int which)
{
	string res("");
	if (which == 0)		// common case: skip size() call
		scalar_vars[0].get_text_scalar(res);
	else if (which >= 0 && which < (int)scalar_vars.size())
		scalar_vars[which].get_text_scalar(res);
	return res.c_str();
}
double 
get_var_vector_numeric_at(int which, int index)
{
    //printf("get_var_vector_numeric_at(which=%d, index=%d\n",which,index);
    if (which >= 0 && which < (int)vector_vars.size())
	return vector_vars[which].get_numeric_vector(index);
    else
	return 0.0;
}
const char*
get_var_vector_text_at(int which, int index)
{
    if (which >= 0 && which < (int)vector_vars.size())
	return vector_vars[which].get_text_vector(index);
    else
	return "";
}
double 
get_var_vector_end(int which)
{
    if (which >= 0 && which < (int)vector_vars.size())
	return double(vector_vars[which].get_end_index());
    else
	return -1.0;
}
void
put_var_vector_size(int which, int size)
{
    if (which >= 0 && which < (int)vector_vars.size()) {
	vector_vars[which].resize(size);
    }
}
bool
put_var_scalar_numeric_at(int which, double value)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var_scalar_numeric_at(which=%d,value=%f)\n", which, value);
    if (which == 0) {		// common case: skip size() call
	scalar_vars[0].set_value(value);
	return true;
    }
    if (which > 0 && which < (int)scalar_vars.size()) {
	scalar_vars[which].set_value(value);
	return true;
    }
    return false;		/* not used*/
}
bool
put_var_scalar_text_at(int which, const char* value)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var_scalar_text_at(which=%d,value=%s)\n", which, value);
    if (which == 0) {		// common case: skip size() call
	scalar_vars[0].set_value(value);
	return true;
    }
    if (which > 0  && which < (int)scalar_vars.size()) {
	scalar_vars[which].set_value(value);
	return true;
    }
    return false;		/* not used*/
}
bool
clear_var_vector_at(int which)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("clear_var_vector_at(which=%d)\n", which);
    if (which >= 0 && which < (int)vector_vars.size()) {
	vector_vars[which].clear_vector();
	return true;
    }
    return false;
}
bool
put_var_vector_numeric_at(int which, int index, double value)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var_vector_numeric_at(which=%d,index=%d,value=%f)\n", which, index,value);
    if (which >= 0 && which < (int)vector_vars.size()) {
	vector_vars[which].set_value(value, index);
	return true;
    }
    return false;		/* not used*/
}
bool
put_var_vector_text_at(int which, int index, const char* value)
{
    if (_gre_debug & DEBUG_VARIABLES) printf("put_var_vector_text_at assigned \"%s\" to vector #%d, index %d\n", value, which, index);
    if (which >= 0 && which < (int)vector_vars.size()) {
	vector_vars[which].set_value(value, index);
	return true;
    }
    return false;		/* not used*/
}


bool
delete_scalar_var(const char *n)	/* delete most recently created version*/
{
    extern vector<var> scalar_vars;
    int len = scalar_vars.size();
    for (int i = len - 1; i > -1; i--)
	if (!strcmp(n, scalar_vars[i].get_name())) {
            scalar_vars.erase(scalar_vars.begin() + i,
			      scalar_vars.begin() + i + 1);
	    return true;
	}
    return false;		/* didn't find it*/
}
bool
delete_vector_var(const char *n)	/* delete most recently created version*/
{
    extern vector<var> vector_vars;
    int len = vector_vars.size();
    for (int i = len - 1; i > -1; i--)
	if (!strcmp(n, vector_vars[i].get_name())) {
	    vector_vars.erase(vector_vars.begin() + i,
			      vector_vars.begin() + i + 1);
	    return true;
	}
    return false;		/* didn't find it*/
}
unsigned int 
length_of_wnode_list(wnode* w)
{
    unsigned int ret = 0;
    while (w) {
	ret++;
	w = w->next;
    }
    return ret;
}


/*
 * New commands
 */
class newcommand {
public:
    newcommand() {
	action = NULL;
	number_words_in_syntax = 0;
	syntax = NULL;
	min_args = max_args = -1;
	prototype = "";
    }
    void set_action(cmdnode* a) {action = a;}
    newcommand(wnode* s, char *proto, cmdnode* a) {
	prototype = strdup(proto);
	action = a;
	number_words_in_syntax = length_of_wnode_list(s);
	syntax = s;
	if (!proto) {
	    min_args = 0;
	    max_args = 0; // means don't care
	} else {
	    min_args = max_args = 0;
	    unsigned int len = strlen(proto);
	    bool found_semicolon = false;
	    for (unsigned int i = 0; i < len; i++) {
                // Need only check legal characters since the string
		// was already checked in yylex.
		if (proto[i] == ';') {
		    found_semicolon = true;
		} else if (proto[i] == '$') {
		    if (found_semicolon) {
			max_args++;
		    } else {
			min_args++;
			max_args++;
		    }
		} else if (proto[i] == '@') {
		    if (found_semicolon)
			max_args = -1;
		    else
			min_args = max_args = -1;
		    break;
		}
	    }
	}
	//printf("Newcmd '%s' ... has MIN args = %d; max args = %d\n", s->w, min_args, max_args);
    }
    newcommand(const newcommand& n) {
	action = n.get_action();
	number_words_in_syntax = length_of_wnode_list(n.get_syntax());
	syntax = n.get_syntax();
	min_args = n.get_min_args();
	max_args = n.get_max_args();
	prototype = strdup(n.get_prototype());
    };
    bool operator<  (const newcommand& ignored) const {return false;}
    bool operator== (const newcommand& ignored) const {return false;}
    newcommand& operator=(const newcommand& n) {
	action = n.get_action();
	number_words_in_syntax = n.get_syntax_length();
	syntax = n.get_syntax();
	min_args = n.get_min_args();
	max_args = n.get_max_args();
	prototype = strdup(n.get_prototype());
	return *this;
    }
    ~newcommand() {};		/* should delete stuff ... BUG*/
    int get_min_args() const { return min_args; } 
    int get_max_args() const { return max_args; } 
    unsigned int get_syntax_length() const {return number_words_in_syntax; }
    wnode* get_syntax()         {return syntax;}
    wnode* get_syntax()   const {return syntax;}
    cmdnode* get_action()       {return action;}
    cmdnode* get_action() const {return action;}
    char *get_prototype() const {return prototype;}
    bool empty_action()   const {return (action == NULL) ? true : false;}
private:
    unsigned int number_words_in_syntax;
    int min_args, max_args;
    wnode*   syntax;
    cmdnode* action;
    char*    prototype;
};
vector<newcommand> cmd_stack;
vector<bool>       cmd_may_match;



static int block_level = 0;
static vector<string> my_var;           // my $var ...
static vector<int>    my_var_bl;        // blocklevel of above


// Next can't be static since it is mentionned in parser.h
vector<var>    pattern_match;    // $0, $1, ...
//static vector<int>    pattern_match_length; // number in set
vector<int>    pattern_match_length; // number in set

void 
reduce_block_level()
{
    // Clean 'my' variables
    int n = (int)my_var_bl.size();
    for (int i = n - 1; i >= 0; i--) {
	if (my_var_bl[i] == block_level) {
	    if (my_var[i][0] == '$') {
		//printf("reduce_block_level scalar '%s' level %d\n", my_var[i].c_str(), my_var_bl[i]);
		int len = int(scalar_vars.size());
		//printf("parser.y/reduce_block_level scalar_vars.size() %d\n", scalar_vars.size());
		for (int v = len - 1; v > -1; v--) {
		    if (my_var[i] == scalar_vars[v].get_name_string()) {
			//printf(" reduce_block_level (now %d) and clearing %s\n",block_level,my_var[i].c_str());
			my_var.pop_back(); // was below if
			my_var_bl.pop_back(); // was below if
			//printf("Erasing name of scalar_vars[%d] '%s'\n",v,scalar_vars[v].get_name_string().c_str());
			scalar_vars[v].erase_name();
			break;
		    }
		}
	    } else {
		//printf("reduce_block_level vector '%s' level %d\n", my_var[i].c_str(), my_var_bl[i]);
		my_var[i][0] = '$'; // fool name recognizer
		int len = int(vector_vars.size());
		//printf("parser.y/reduce_block_level vector_vars.size() %d\n", vector_vars.size());
		for (int v = len - 1; v > -1; v--) {
		    if (my_var[i] == vector_vars[v].get_name_string()) {
			//printf(" reduce_block_level (now %d) and clearing %s\n",block_level,my_var[i].c_str());
			my_var.pop_back(); // was below if
			my_var_bl.pop_back(); // was below if
			//printf("parser.y:%d Erasing name of vector_vars[%d] '%s'\n",__LINE__,v,vector_vars[v].get_name_string().c_str());

			//printf("BEFORE:\n");for (int i = 0; i < vector_vars.size(); i++) printf("  %d: '%s'\n", i, vector_vars[i].get_name());

			vector_vars[v].erase_name();

			//printf("BEFORE:\n");for (int i = 0; i < vector_vars.size(); i++) printf("  %d: '%s'\n", i, vector_vars[i].get_name());

			break;
		    }
		}
	    }
 	} else {
	    break;
	}
    }
    
#if 0
    // Clean patterns
    int len = int(pattern_match.size());
    printf("...cleaning block level %d; len=%d\n", block_level,len);
    for (int p = len - 1; p > -1; p--) {
        if (pattern_match_bl[p] == block_level) {
            printf(" cleaning pattern p=%d at blocklevel %d\n", p, pattern_match_bl[p]);
            pattern_match.pop_back();
            pattern_match_bl.pop_back();
        } else {
            break;
        }
    }
#endif

    block_level--;
}
void
increase_block_level()
{
    block_level++;
}
int
get_block_level()
{
    return block_level;
}




int yyerror(const char *);
int yylex(void);
string package_name("main");
cmd_eval_code evaluate_cmd(cmdnode *c);
    
enum next_item_expected_to_be {a_word, a_cmdword, a_expr, s_from, s_to};
next_item_expected_to_be expect = a_cmdword;

typedef struct {
    char *word;
    int   len;			// might speed up search a little
    int   code;
} KW_LIST;

/* Reserved keywords (I should reorder in terms of probability) */
KW_LIST kw_list[] = {
    {"cmd",	3, CMD},
    {"and",	3, AND},
    {"die",	3, DIE},
    {"do",	2, DO},
    {"else",	4, ELSE},
    {"execute",	7, EXECUTE},
    {"exit",	4, EXIT},
    {"for",	3, FOR},
    {"foreach",	7, FOREACH},
    {"glob",	4, GLOB},
    {"if",	2, IF},
    {"index",	5, INDEX},
    {"last",	4, LAST},
    {"next",	4, NEXT},
    {"while",	5, WHILE},
    {"or",	2, OR},
    {"print",	5, PRINT},
    {"printf",	6, PRINTF},
    {"my",	2, MY},
    {"not",	3, NOT},
    {"eq",	2, STRING_EQ},
    {"ne",	2, STRING_NE},
    {"return",  6, RETURN},
    {"chomp",	5, CHOMP},
    {"chop",	4, CHOP},
    {"close",	5, CLOSE},
//  {"columns",	7, COLUMNS},
    {"open",	4, OPEN},
    {"package",	7, GREPACKAGE},
    {"use",	3, USE},
    {"query",	5, QUERY},
    {"read",	4, READ},
    {"rewind",	6, REWIND},
    {"skip",	4, SKIP},
    {"split",	5, SPLIT},
    {"sub",	3, SUB},
    {"substr",	6, SUBSTR},
    {"sprintf",	7, SPRINTF},
    {"system",	6, SYSTEM},
    {"warn",	4, WARN},
    {"unless",	6, UNLESS},
    {"unlink",	6, UNLINK},
    {"until",	5, UNTIL},
    {"bool",	4, BOOL},		/* do I even need this?*/
    {NULL,	0, 0}
};


unsigned int error_count = 0;
cmdnode *the_pgm;

%}


%union {
    double   dval;		/* number*/
    char    *cval;		/* character string*/
    enode   *eval;		/* expression*/
    wnode   *wval;		/* word*/
    cmdnode *cmdval;		/* command*/
}

%type  <cmdval> cmd cmds block single_cmd
%type  <cmdval> define_sub
%type  <cmdval> define_cmd
%type  <cmdval> do_while_block
%type  <cmdval> do_until_block
%type  <cmdval> loop
%type  <cmdval> while_block	
%type  <cmdval> until_block	
%type  <cmdval> for_block	
%type  <cmdval> foreach_block	
%type  <cmdval> if_block
%type  <cmdval> trailing_if
%type  <cmdval> trailing_unless
%type  <cmdval> trailing_while
%type  <cmdval> trailing_until
%type  <cmdval> return_cmd	
%type  <cmdval> last_cmd	/* break out of a loop */
%type  <cmdval> next_cmd	/* continue with loop */
%type  <cmdval> expr_cmd
%type  <cmdval> skip_cmd

/* Some commands now*/
%type  <cmdval> C_command	/* call a C function directly*/
%type  <cmdval> my_var_list
%type  <cmdval> package
%type  <cmdval> use
%type  <cmdval> print
%type  <cmdval> printf
%type  <cmdval> query		/* ask a question */
%type  <cmdval> read_columns
%type  <cmdval> read_grid
%type  <cmdval> read_var_list	/* print all variable values */
%type  <cmdval> split		/* split as subset of perl method */
%type  <cmdval> a_user_cmd	/* use a user-defined cmd */

%type  <eval> texpr		/* expression or blank, for loops */
%type  <eval> expr		/* expression */
%type  <wval> read_columns_spec
%type  <wval> read_columns_spec_list
%type  <wval> listexpr		/* comma-separated series of expressions */
%type  <wval> listexpr_nocomma	/* space-separated series of expressions */
%type  <wval> var_list_comma	// comma-separated list of variable names
%type  <wval> var_list_nocomma	// space-separated list of variable names
%type  <wval> words		// whitespace-separated list of words
%type  <wval> cmd_words		// calling a 'cmd'

%token <dval> NUM		/* constant number */
%token <cval> ARRAY		/* array name */
%token <cval> PATTERN_VARIABLE  /* $1, etc -- pattern matches */
%token <cval> VARIABLE		/* variable name */
%token <cval> VARIABLE_END	/* position of the last (end) of array */

%token <cval> COLUMN_IDENT	/* STRING|WORD = expr */
%token <cval> ASSIGNOP		// =   +=   -=   *=   /=   =~
%token <cval> END_MATCH_STRING

/*
 * Keywords (reserved words) in *any* position in a command
 */
%token <cval> BOOL		/* 'bool', for C commands */
%token <cval> CMD
%token <cval> DIE
%token <cval> EXIT
%token <cval> WARN
%token <cval> DO
%token <cval> ELSE
%token <cval> EXECUTE
%token <cval> FOR
%token <cval> FOREACH
%token <cval> FILE_TEST
%token <cval> GLOB
%token <cval> IF
%token <cval> INDEX
%token <cval> LAST
%token <cval> NEXT
%token <cval> RETURN
%token <cval> CHOMP CHOP
%token <cval> READ_GRID		/* only if follows 'read' */
%token <cval> COLUMNS		/* only if follows 'print', 'read' or 'convert' */
%token <cval> MY
%token <cval> GREPACKAGE
%token <cval> USE
%token <cval> SKIP
%token <cval> SPLIT
%token <cval> STRING_EQ
%token <cval> STRING_NE
%token <cval> SUB
%token <cval> SUBSTR
%token <cval> SPRINTF
%token <cval> SYSTEM
%token <cval> UNLESS
%token <cval> UNLINK
%token <cval> WHILE
%token <cval> UNTIL
%token <cval> PRINT
%token <cval> PRINTF
%token <cval> OPEN
%token <cval> REWIND
%token <cval> CLOSE
%token <cval> QUERY
%token <cval> READ

%token <cval> DOT
%token <cval> EQ LE GE		/* == <= >= */
%token <cval> NOT AND OR	/* !, &&, || */
%token <cval> WORD		// a word
%token <cval> CMDWORD		// a word in a command
%token <cval> PROTOTYPE
%token <cval> FUNCTION_BUILTIN_NUMERIC	// e.g. rand()
%token <cval> FUNCTION_BUILTIN_TEXT	// e.g. open()
%token <cval> FUNCTION_USER	// user-defined
%token <cval> STRING		/* quoted string (") */
%token <cval> MATCH_STRING      /* /.../ */

// Next two take care of the s/from/to/ construct ...
%token <cval> S_FROM            // "from" part of s/from/to/ construct
%token <cval> S_TO              // "to" part of s/from/to/ construct


/* Priorities, in order from lowest to highest;
 * follows conventional pattern, to match expectations
 * based on, say gawk.
 */
%left OR
%left AND
%right ','
%right ASSIGNOP
%right '?' ':'
%nonassoc '<' '>' LE GE EQ NE
%left '+' '-' DOT
%left MULOP '/' '%' STRING_EQ STRING_NE
%right NOT UNARY FILE_TEST
%right POWOP
%left POSTINC POSTDEC
%left '('

%% /* RULES */

start
	: cmds { the_pgm = $1; }
	;


cmds
	: cmd { $$ = $1; }
	| cmds cmd { 
            $$ = append_right($1, $2);
            //printf("'cmds cmd' appended %s (%x)\n",name_of_cmd($2->type),$2);
        }
	| cmds error
	;


cmd
	: /* NULL */ SC {$$ = new_cmdnode();}
	| single_cmd SC {
            if (_gre_debug & DEBUG_TRACE) {
                printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; 
                if ($$->type == cmdnode::user_cmd) 
                    printf("# user_cmd, index=%d\n", $$->the_cmd_index);
                else
                    printf("# cmd of type %d (%s)\n", $$->type, name_of_cmd($$->type));
	    }
            if ($$->type == cmdnode::use) {
                // Somewhat complicated searching method -- see manual
                FILE *fp;
                string path;
                path.assign("./");
                path.append(use_name);
                path.append(".gre");
                // First, try locally ...
                fp = fopen(path.c_str(), "r");
                //printf("\n");
                //printf("try %s\n",path.c_str());
                if (fp == NULL) {
                    // If not local, look in default gre directory
                    path.assign(DEFAULT_GRE_DIR);
                    path.append("/");
                    path.append(use_name);
                    path.append(".gre");
                    //printf("%s not local; trying %s\n", use_name.c_str(), path.c_str());
                    fp = fopen(path.c_str(), "r"); // no; try GRE_DIR ...
                    if (fp == NULL) {
                        // If not there either, look in GREPATH list
                        path.assign(call_getenv("GREPATH"));
                        //printf("\nNot there either!.  Now try '%s'\n", path.c_str());
                        unsigned int start = 0;
                        unsigned int stop = path.size();
                        // substr(start,stop)
                        string::size_type colon;
                        while (start < stop) {
                            colon = path.find_first_of(':', start);
                            //printf("colon at %d;  start %d\n",colon,start);
                            string sub;
                            if (colon != STRING_NPOS) {
                                sub = path.substr(start, colon - start);
                                start = colon + 1;
                            } else {
                                sub = path.substr(start, stop - start);
                                start = stop;
                            }
                            sub.append("/");
                            sub.append(use_name);
                            sub.append(".gre");
                            //printf("'use' trying for module named %s\n", sub.c_str());
                            fp = fopen(sub.c_str(), "r");
                            if (fp != NULL) {
                                //printf(" == got it!\n");
                                break;
                            }
                            //printf(" -- no luck still\n");
                        }
                    }
                }

                if (fp == NULL) {
                    string msg("Can't locate ");
                    msg = msg + use_name.c_str();
                    yyerror(msg.c_str());
                }
                push_cmdfile(strdup(use_name.c_str()), fp);
                use_name.assign("");
            }
	}
	| trailing_if SC     { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `if'\n"); }}
	| trailing_unless SC { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `unless'\n"); }}
	| trailing_while SC  { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `while'\n"); }}
	| trailing_until SC  { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `until'\n"); }}
	| if_block           { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of if-statment\n"); }}
	| loop               { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of loop\n"); }}
	| define_sub         { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of defininition of a new sub\n"); }}
	| define_cmd         { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of definition of a new cmd\n"); }}
	;

single_cmd
	: expr_cmd {
	    $$ = $1;
	    $$->type = cmdnode::expression;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
            //printf("single_cmd %x is a cmdnode::expression\n", $$);
	}
	| return_cmd {
	    $$ = $1;
	    $$->type = cmdnode::return_block;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| skip_cmd {
	    $$ = $1;
	    $$->type = cmdnode::return_block;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| next_cmd {
	    $$ = $1;
	    $$->type = cmdnode::next_block;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| last_cmd {
	    $$ = $1;
	    $$->type = cmdnode::last_block;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| C_command {
	    $$ = $1;
	    $$->type = cmdnode::C_command;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| my_var_list {
	    $$ = $1;
	    $$->type = cmdnode::my_var_list;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
        | package {
            $$ = $1;
            $$->type = cmdnode::package;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
        }
	| print {
	    $$ = $1;
	    $$->type = cmdnode::print;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| printf {
	    $$ = $1;
	    $$->type = cmdnode::printf;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| query {
	    $$ = $1;
	    $$->type = cmdnode::query;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| read_grid {
	    $$ = $1;
	    /* Do not set type; it's already set */
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| read_columns {
	    $$ = $1;
	    $$->type = cmdnode::read_columns;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| read_var_list {
	    $$ = $1;
	    $$->type = cmdnode::read_var_list;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	| split {
	    $$ = $1;
	    $$->type = cmdnode::split;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
        | use {
            $$ = $1;
            $$->type = cmdnode::use;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
        }
	| a_user_cmd {
	    $$ = $1;
	    $$->type = cmdnode::user_cmd;
	    $$->line = cmdfile_line;
	    $$->file_name = cmdfile_name;
	}
	;


expr_cmd
	: expr { $$ = new_cmdnode(new_wnode_expr($1)); }
	;

last_cmd
	: LAST {
	    wnode *n = new_wnode_string($1);
	    $$ = new_cmdnode(n);
	}
	;
next_cmd
	: NEXT {
	    wnode *n = new_wnode_string($1);
	    $$ = new_cmdnode(n);
	}
	;
return_cmd
	: RETURN {
	    $$ = new_cmdnode(new_wnode_expr(new enode(0.0)));
	}
	| RETURN expr {
	    $$ = new_cmdnode(new_wnode_expr($2));
	}
	;
words
	: WORD { $$ = new_wnode_string($1);}
	| words WORD { $$ = append_right($1, new_wnode_string($2));}
	;

cmd_words
	: CMDWORD {
	    $$ = new_wnode_string($1);
	    //printf("* START WITH '%s'\n", $1);
	    if (cmd_num_matches == 1) {
		//printf("\t--- exact match to cmd %d\n",cmd_matches_to);
		expect = a_expr;
	    }
	}
	| cmd_words CMDWORD {
	    $$ = append_right($1, new_wnode_string($2));
	    //printf("* APPEND '%s' %s\n", $2, cmd_num_matches==1 ? "DONE" : "NOT DONE");
	    if (cmd_num_matches == 1) {
		//printf("\t--- exact match to cmd %d\n",cmd_matches_to);
		expect = a_expr;
	    }
	}
	;

C_command
	: EXECUTE STRING BOOL STRING '(' r_paren  {
	    if (strcmp($2, "C")) {
		char msg[100];
		sprintf(msg, "Internal error: can only execute \"C\" code, not \"%s\" code", $2);
		yyerror(msg);
		$$ = NULL;	/* ? not sure if this is proper ?*/
	    } else {
		int i = 0;
		/* 
		 * Search functions linearly -- don't worry about speed since
		 * this only done once.
		 */
		bool found = false;
		while(c_fcn[i].fcn != 0) {
		    if (!strcmp($4/* ->w */, c_fcn[i].name)) {
			found = true;
			$$ = new_cmdnode(c_fcn[i].fcn);
			break;
		    }
		    i++;
		}
		if (!found) {
		    string msg("Internal error: no builtin C function named `");
		    msg += $4/* ->w */;
		    msg += "'";
		    yyerror(msg.c_str());
		    $$ = NULL;	// BUG -- not sure if this is proper
		}
	    }
	}
	;

var_list_comma
	: VARIABLE {
		$$ = new_wnode_vname($1);
	}
	| var_list_comma ',' VARIABLE {
		$$ = append_right($1, new_wnode_vname($3));
	}
	;

var_list_nocomma
	: VARIABLE {
	    $$ = new_wnode_vname($1);
	}
	| var_list_nocomma VARIABLE {
	    $$ = append_right($1, new_wnode_vname($2));
	}
	;

my_var_list
	: MY VARIABLE {
	    $$ = new_cmdnode(new_wnode_vname($2));
	    my_var.push_back($2);
	    my_var_bl.push_back(get_block_level());
            $$->first_wnode = NULL;
	}
	| MY var_list_comma {
	    wnode *p = $2;
	    while (p != NULL) {
		    my_var.push_back(p->w);
		    my_var_bl.push_back(get_block_level());
		    p = p->next;
	    }
	    $$ = new_cmdnode(new_wnode_vname($2->w));
	    $$->first_wnode = NULL;
	}
	| MY '(' var_list_comma ')' {
	    wnode *p = $3;
	    while (p != NULL) {
		    my_var.push_back(p->w);
		    my_var_bl.push_back(get_block_level());
		    p = p->next;
	    }
	    $$ = new_cmdnode(new_wnode_vname($3->w));
	    $$->first_wnode = NULL;
	}
	| MY ARRAY {
	    $$ = new_cmdnode(new_wnode_vname($2));
	    my_var.push_back($2);
	    my_var_bl.push_back(get_block_level());
            $$->first_wnode = NULL;
	}
	| MY VARIABLE ASSIGNOP expr {
	    if (strNE("=", $3)) {
		yyerror("Only allowable operator in `my' command is `='");
	    }
	    $$ = new_cmdnode(new_wnode_vname($2));
	    my_var.push_back($2);
	    my_var_bl.push_back(get_block_level());
            enode *ev = new enode(enode::variable, $2, NULL); // BUG: not used
            enode* e = new enode();
	    int which = get_scalar_var_index($2);
	    //printf("MY VARIABLE ASSIGNOP expr ... which = %d\n", which);
            wnode *to_execute = new_wnode_expr(e->assign_to_known_scalar($2, which, $3, $4));
            $$->first_wnode = to_execute;
	}
	;

package
        : GREPACKAGE CMDWORD {
            cmdnode* new_cmdnode_package(const char* package);
    	    $$ = new_cmdnode_package($2);
        }
        ;

// HERE HERE HERE HERE HERE
use
        : USE CMDWORD {
            use_name.assign($2);
            cmdnode* new_cmdnode_use();
    	    $$ = new_cmdnode_use();
            //printf("\nBUG: 'use' doesn't search files right.  file='%s'\n\n",use_name.c_str());
        }
        ;

print
	: PRINT listexpr {
    	    $$ = new_cmdnode($2);
	    $$->handle_index = look_up_file_handle("STDOUT", true);
	}
	| PRINT CMDWORD listexpr {
    	    $$ = new_cmdnode($3);
	    $$->handle_index = look_up_file_handle($2, true);
	}
	| PRINT COLUMNS {
	    $$ = new_cmdnode(new_wnode_string("columns"));
	    $$->handle_index = look_up_file_handle("STDOUT", true);
	}
	| PRINT cmd_words {
	    if ($2->next != NULL)
		yyerror("Cannot have more than 1 word in `print' file-handle name.");
	    //printf("DEBUG 1.  word is '%s'\n",$2->w);
	    if (!strcmp($2->w, "variables")) {
		//printf("DEBUG 1.1 -- seen as 'print variables'\n");
		$$ = new_cmdnode(new_wnode_string("variables"));
		$$->handle_index = look_up_file_handle("STDOUT", true);
	    } else if (!strcmp($2->w, "grid")) {
		//printf("DEBUG 1.2 -- seen as 'print grid'\n");
		$$ = new_cmdnode(new_wnode_string("grid"));
		$$->handle_index = look_up_file_handle("STDOUT", true);
	    } else if (!strcmp($2->w, "image")) {
		$$ = new_cmdnode(new_wnode_string("image"));
		$$->handle_index = look_up_file_handle("STDOUT", true);
	    } else {
		//printf("DEBUG 1.3 -- seen as 'print ???' so think handle\n");
		$$ = new_cmdnode((wnode*)NULL);
		$$->handle_index = look_up_file_handle($2->w, true);
	    }
	}
	| PRINT {
	    $$ = new_cmdnode((wnode*)NULL);
	    $$->handle_index = look_up_file_handle("STDOUT", true);
	}
	;

/*
 * PRINTF FORMAT LIST
 * PRINTF FILEHANDLE FORMAT LIST
 */
printf
	: PRINTF listexpr {
    	    $$ = new_cmdnode($2);
	    $$->handle_index = look_up_file_handle("STDOUT", true);
	}
	| PRINTF CMDWORD listexpr {
    	    $$ = new_cmdnode($3);
	    $$->handle_index = look_up_file_handle($2, true);
	}
	;

query
	: QUERY VARIABLE {
	    int which = get_scalar_var_index($2);
	    if (which < 0) {
		var *v = new var($2, "");
		scalar_vars.push_back(*v);
	    }
	    wnode *n = new_wnode_string($2);
	    $$ = new_cmdnode(n);
	}
	| QUERY VARIABLE STRING {
	    int which = get_scalar_var_index($2);
	    if (which < 0) {
		var *v = new var($2, "");
		scalar_vars.push_back(*v);
	    }
	    wnode *n = new_wnode_string($2);
	    n = append_right(n, new_wnode_expr(new enode(enode::text, $3)));
	    $$ = new_cmdnode(n);
	}
	| QUERY VARIABLE STRING '(' listexpr r_paren {
	    ensure_variable_known($2);
	    wnode *n = new_wnode_string($2);
	    n = append_right(n, new_wnode_expr(new enode(enode::text, $3)));
	    n = append_right(n, $5);
	    $$ = new_cmdnode(n);
	}
	;




read_columns_spec_list
	: read_columns_spec { $$ = $1; }
	| read_columns_spec_list read_columns_spec { $$ = append_right($1, $2); }
	;

/* Take as both WORD (first in list) or STRING (later in list) */
read_columns_spec
	: WORD COLUMN_IDENT expr {
            //printf("read_columns_spec type 1\n");
            //ALWAYS TRUE: if (strNE($2, "=")) { yyerror("expecting `='"); }
	    $$ = new_wnode_columns_spec($1, $3);
	}
	| STRING COLUMN_IDENT expr {
            //printf("read_columns_spec type 2\n");
	    //ALWAYS TRUE: if (strNE($2, "=")) { yyerror("expecting `='"); }
	    $$ = new_wnode_columns_spec($1, $3);
	}
	| WORD {
            //printf("read_columns_spec type 3\n");
	    $$ = new_wnode_columns_spec($1, NULL);
	    expect = a_word;	// new
	}
	;

/* `read columns [HANDLE] ...' where ... contains items from `x', `y', etc */
read_columns
	: READ COLUMNS read_columns_spec_list {
	    $$ = new_read_columns_cmd("ANONYMOUS", $3, NULL);
	}
	| READ COLUMNS read_columns_spec_list expr {
	    $$ = new_read_columns_cmd("ANONYMOUS", $3, $4);
	}
	| READ COLUMNS WORD /*handle*/ read_columns_spec_list {
	    $$ = new_read_columns_cmd($3, $4, NULL);
	}
	| READ COLUMNS WORD /*handle*/ read_columns_spec_list expr {
	    $$ = new_read_columns_cmd($3, $4, $5);
	}
	;





read_grid
	: READ READ_GRID WORD { /* 'read grid data|x|y' */
	    //printf("'READ GRID %s' parsed\n", $3);
	    $$ = new_read_grid_cmd($3, "ANONYMOUS", NULL);
	}
	| READ READ_GRID WORD listexpr_nocomma { /* 'read grid data|x|y ...' */
	    //printf("'READ GRID %s ...' parsed\n", $3);
	    $$ = new_read_grid_cmd($3, "ANONYMOUS", $4);
	}
	| READ READ_GRID WORD words { /* with handle */
	    //printf("'READ GRID %s (handle)' parsed\n", $3);
	    if ($4->next != NULL)
		yyerror("`read grid data|x|y' cannot have more than 1 handle");
	    $$ = new_read_grid_cmd($3, $4->w, NULL);
	}
	| READ READ_GRID WORD words listexpr_nocomma { /* with handle */
	    //printf("'READ GRID %s (handle) ...' parsed\n", $3);
	    if ($4->next != NULL)
		yyerror("`read grid data|x|y' cannot have more than 1 handle");
	    $$ = new_read_grid_cmd($3, $4->w, $5);
	}
	;

read_var_list
	: READ WORD var_list_nocomma { /* not same as 'read columns' */
	    $$ = new_cmdnode($3);
	    $$->handle_index = look_up_file_handle($2, true);
	}
	| READ var_list_nocomma {
	    $$ = new_cmdnode($2);
	    $$->handle_index = look_up_file_handle("ANONYMOUS", true);
	}
	;

split
	: SPLIT {
	    wnode *n = new_wnode_string("");
	    $$ = new_cmdnode(n);
	    $$->array_index = 0;
	}
	| ARRAY ASSIGNOP SPLIT {
	    if (strNE("=", $2)) {
		yyerror("Only allowable operator in `split' command is `='");
	    }
	    wnode *n = new_wnode_string(""); /* BUG - don't need this (?) */ 
	    $$ = new_cmdnode(n);
	    char *name = strdup($1);
	    name[0] = '$';	// fool name recognizer
	    int which = get_vector_var_index(name);
	    if (which < 0) {
		var *v = new var(name, 0.0);
		vector_vars.push_back(*v);
		delete v;
		//printf("SPLIT case 1 -- had to create\n");
		$$->array_index = vector_vars.size() - 1;
	    } else {
		//printf("SPLIT case 2 -- already known\n");
		$$->array_index = which;
	    }
	    //printf("SPLIT lvalue '%s'-->'%s' --> %d\n",$1,name,$$->array_index);
	    free(name);
	}
	;

loop
	: do_while_block
	| do_until_block
	| while_block
	| until_block
	| for_block
	| foreach_block
	;


if_block
	: IF expr block ELSE block {
	    $$ = new_if_block($2, $3, $5);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	| IF expr block {
	    $$ = new_if_block($2, $3, NULL);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	| UNLESS expr block ELSE block {
	    $$ = new_if_block(logical_negation($2), $3, $5);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	| UNLESS expr block {
	    $$ = new_if_block(logical_negation($2), $3, NULL);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;

block
	: start_of_block cmds end_of_block {
	    $$ = $2;
	}
	;
start_of_block
	: '{' {
            increase_block_level();
	    cmd_expected_next();
	}
	;

end_of_block
	: '}' {
            yyerrok;
	    reduce_block_level();
	}
	;
trailing_if
	: single_cmd IF expr {
	    $$ = new_if_block($3, $1, NULL);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
trailing_unless
	: single_cmd UNLESS expr {
	    $$ = new_if_block(logical_negation($3), $1, NULL);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
trailing_while
	: single_cmd WHILE expr {
	    $$ = new_do_while_block($3, $1);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
trailing_until
	: single_cmd UNTIL expr {
	    $$ = new_do_while_block(logical_negation($3), $1);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
while_context
	: { save_string = true; }
	;
while_block
	: WHILE while_context texpr block {
	    $$ = new_while_block($3, $4);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
until_block
	: UNTIL while_context texpr block {
	    $$ = new_while_block(logical_negation($3), $4);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
do_while_block
	: DO block WHILE texpr SC{
	    $$ = new_do_while_block($4, $2);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
do_until_block
	: DO block UNTIL texpr SC{
	    $$ = new_do_while_block(logical_negation($4), $2);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;
for_block
	: FOR '(' expr ';' texpr ';' expr r_paren block {
	    $$ = new_for_block($3, $5, $7, $9);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;

foreach_block
	: FOREACH VARIABLE '(' listexpr r_paren block { /* Like perl, but no commas*/
	    ensure_variable_known($2);
	    $$ = new_foreach_block_list($2, $4, $6);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	| FOREACH VARIABLE '(' ARRAY r_paren block {
	    ensure_variable_known($2);
	    $$ = new_foreach_block_array($2, $4, $6);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;	    
	}
	;

define_sub
	: SUB words block {	/* Catch common error for perl users */ 
	    string msg("Must supply a prototype for sub named `");
	    msg += $2->w;
	    msg += "'.";
	    yyerror(msg.c_str());
	    if ($2->next != NULL) {
		yyerror("Cannot have more than 1 word in `sub' name; error detected at end of the 'sub' definition");
	    }
	    $$ = new_cmdnode();
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	| SUB words '(' PROTOTYPE r_paren block {
	    if ($2->next != NULL) {
		yyerror("Cannot have more than 1 word in `sub' name; error detected at end of the 'sub' definition");
	    }
	    extern vector<user_sub> user_sub_stack;
	    user_sub *s = new user_sub($2, $4, $6);
	    user_sub_stack.push_back(*s);
 	    delete s;
	    $$ = new_cmdnode();	// blank
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	}
	;

define_cmd
	: CMD words '(' PROTOTYPE r_paren block {
	    int create_a_new_cmd(wnode* w, char* p, cmdnode *b);
	    $$ = new_cmdnode();	// blank
	    $$->the_cmd_index = create_a_new_cmd($2, $4, $6);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	    if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of defininition of a new user_cmd, index %d\n", $$->the_cmd_index); }
	    expecting_prototype = defining_sub_or_cmd = false;
	}
	| CMD words '(' PROTOTYPE r_paren {
	    int create_a_new_cmd(wnode* w, char* p, cmdnode *b);
	    $$ = new_cmdnode();	// blank
	    $$->the_cmd_index = create_a_new_cmd($2, $4, NULL);
	    $$->line = cmdfile_line; $$->file_name = cmdfile_name;
	    if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of prototype of a new user_cmd, index %d\n", $$->the_cmd_index); }
	    expecting_prototype = defining_sub_or_cmd = false;
	}
	;

a_user_cmd
	: cmd_words {
            $$ = new_user_cmd($1,cmd_matches_to,cmd_num_matches,(wnode*)NULL);
	    $$->the_cmd_index = cmd_matches_to;
	}
	| cmd_words listexpr_nocomma {
	    $$ = new_user_cmd($1, cmd_matches_to, cmd_num_matches, $2);
	    $$->the_cmd_index = cmd_matches_to;
	}
	;

listexpr_nocomma
	: expr {$$ = new_wnode_expr($1);}
	| listexpr_nocomma expr %prec UNARY {$$ = append_right($1, new_wnode_expr($2));}
	;

skip_cmd
	: SKIP {
	    printf("parsing SKIP\n");
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip("ANONYMOUS", new enode(1.0)));
	    $$ = new_cmdnode(n);
	}
	| SKIP '(' WORD ')' {		// handle
	    printf("parsing SKIP WORD\n");
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip($3, new enode(1.0)));
	    $$ = new_cmdnode(n);
	}
	| SKIP '(' WORD expr ')' {	// handle, number-lines
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip($3, $4));
	    $$ = new_cmdnode(n);
	}
	;

texpr
	: /* NULL */ { $$ = new enode(1.0);}
	| expr
	;

expr
	: NUM		{ $$ = new enode($1); }
        | S_FROM S_TO { // s/from/to/
	//printf("'string_change' $1 '%s'   $2 '%s'\n",$1,$2);
		if (strlen($1) < 1) {
			yyerror("Substitution replacement cannot have empty pattern.");
		}
		enode *e;
		$$ = e->string_change($1, $2);
        }
        | '/' {expecting_matchstring = true;} MATCH_STRING END_MATCH_STRING {
            //printf("parser got match item '%s'.  flag='%s'\n", $3, $4);
            enode *e;
            enode *s = new enode(enode::text, $3);
	    $$ = e->string_match(s, $4);
            expecting_matchstring = false;
        }
	| CHOP %prec ',' {
            enode *e;
	    $$ = e->do_chop(NULL, cmdfile_name, cmdfile_line);
	}
	| CHOP VARIABLE {
            enode *e;
	    $$ = e->do_chop($2, cmdfile_name, cmdfile_line);
	}
	| CHOP '(' VARIABLE ')' {
            enode *e;
	    $$ = e->do_chop($3, cmdfile_name, cmdfile_line);
	}
	| CHOMP %prec ',' {
            enode *e;
	    $$ = e->do_chomp((char*)NULL, cmdfile_name, cmdfile_line);
	}
	| CHOMP VARIABLE {
            enode *e;
	    $$ = e->do_chomp($2, cmdfile_name, cmdfile_line);
	}
	| CHOMP '(' VARIABLE ')' {
            enode *e;
	    $$ = e->do_chomp($3, cmdfile_name, cmdfile_line);
	}
	| DIE %prec ',' {
	    enode *e;
	    $$ = e->do_die((enode *)NULL, cmdfile_name, cmdfile_line);
	}
	| DIE expr %prec ',' { 
	    enode *e;
	    $$ = e->do_die($2, cmdfile_name, cmdfile_line);
	}
	| EXIT expr %prec ',' {
	    if ($2->is_textual())
		yyerror("Cannot `exit' with textual return code.");
	    enode *e;
	    $$ = e->do_exit($2);
	}
	| GLOB '(' expr ')' {
	    enode *e;
	    $$ = e->do_glob($3);
	}
	| INDEX '(' expr ',' expr ')' {
	    enode *e;
	    $$ = e->do_index($3/*str*/, $5 /*substr*/, NULL /*position*/);
	}
	| INDEX '(' expr ',' expr ',' expr ')' {
	    enode *e;
	    $$ = e->do_index($3/*str*/, $5 /*substr*/, $7 /*position*/);
	}
	| WARN expr %prec ',' { 
	    enode *e;
	    $$ = e->do_warn($2, cmdfile_name, cmdfile_line);
	}
	| '<' '>'  { 
	    enode *e;
	    $$ = e->read_string_from_file("STDIN");
	}
	| '<' words '>'  {
	    if ($2->next != NULL)
		yyerror("Cannot have more than 1 word in file-handle name.");
	    enode *e;
	    $$ = e->read_string_from_file($2->w);
	}
	| FILE_TEST expr {
	    enode *e;
	    $$ = e->test_a_file(*($1 + 1), $2);
	}
	| SYSTEM '(' expr ')' {
	    enode *e;
	    $$ = e->call_system($3);
	}
	| SUBSTR '(' expr ',' expr ',' expr ')' {
	    enode *e;
	    $$ = e->substr($3/*string*/, $5 /*offset*/, $7 /*length*/);
	}
	| SUBSTR '(' expr ',' expr ')' {
	    enode *e;
	    $$ = e->substr($3/*string*/, $5 /*offset*/, NULL /*length*/);
	}
	| SPRINTF '(' listexpr ')' {
	    //printf("\n\nABOUT to create enode for sprintf (paren type)\n\n");
	    enode *e;
	    $$ = e->e_sprintf($3);
	}
        | REWIND '(' ')' {
	    enode *e;
	    $$ = e->rewind_a_file("ANONYMOUS");
	}
        | REWIND {
	    enode *e;
	    $$ = e->rewind_a_file("ANONYMOUS");
	}
        | REWIND WORD {
	    enode *e;
	    $$ = e->rewind_a_file($2);
	}
        | REWIND '(' WORD ')' {
	    enode *e;
	    $$ = e->rewind_a_file($3);
	}
	| OPEN WORD {		/* works even if dot in filename */ 
	    enode *e;
	    $$ = e->open_a_file("ANONYMOUS", new enode(enode::text, $2));
	}
	| OPEN expr %prec ',' {
	    enode *e;
	    $$ = e->open_a_file("ANONYMOUS", $2);
	}
	| OPEN '(' WORD ',' expr ')' {
	    char *filehandle = $3;
	    /* Demand that handle has no upper-case letters */
	    while (filehandle) {
		if (!(isupper(*filehandle) 
		      || isdigit(*filehandle)
		      || *filehandle == '_'))
		    break;
		filehandle++;
	    }
	    if (*filehandle) {
		string msg("File handle `");
		msg += $3;
		msg += "' must be all upper case.";
		yyerror(msg.c_str());
	    }
#if 1
	    enode *e;
#else
	    enode *e = new enode();	    
#endif
	    $$ = e->open_a_file($3 /*handle*/, $5 /*name*/);
	}
	| CLOSE              { enode *e; $$ = e->close_a_file("ANONYMOUS"); }
	| CLOSE WORD         { enode *e; $$ = e->close_a_file($2);          }
	| CLOSE '(' WORD ')' { enode *e; $$ = e->close_a_file($3);          }
	| VARIABLE	     { $$ = new enode(enode::variable, $1, NULL); }
	| VARIABLE_END	{ $$ = new enode(enode::variable_end, $1, NULL); }
        | PATTERN_VARIABLE   { enode *e; $$ = e->pattern_match_variable($1);}
	| ARRAY ASSIGNOP '(' listexpr ')' {
		if (strNE("=", $2)) {
			string msg("Expecting `=' but got `");
			msg.append($2);
			msg.append("'.");
			yyerror(msg.c_str());
		}
		enode *e;
		//printf("assigning list\n");
		// Pattern on line parser.y:2399 'ARRAY ASSIGNOP SPLIT'
		char *name = strdup($1);
		name[0] = '$';	// fool name recognizer
		int which = get_vector_var_index(name);
		if (which < 0) {
			var *v = new var(name, 0.0);
			vector_vars.push_back(*v);
			delete v;
			which = vector_vars.size() - 1;
		}
		free(name);
		$$ = e->assign_list_to_vector(which, $4);
	}
        /* type variable_name char* expression */
	| expr ASSIGNOP expr { 
	    //printf("expr ASSIGNOP expr ... RHS type is %d (e.g. variable=%d text=%d mul=%d)\n", $3->get_type(), enode::variable, enode::text,enode::mul);
	    if ($1->get_type() == enode::variable) {
                //printf("assigning to variable\n");
		enode *e;
		$$ = e->assign_to_scalar($1, $2, $3);
	    } else if ($1->get_type() == enode::variable_at_i) {
                //printf("%s:%d: assigning to variable_at_i\n",__FILE__,__LINE__);
		enode *e;
		$$ = e->assign_to_vector($1, $2, $3);
	    } else if ($1->get_type() == enode::variable_end) {
		// resize a vector
		enode *e;
		$$ = e->resize_a_vector($1, $2, $3);
	    } else {
		yyerror("Can't modify constant item");
	    }
	}
	| UNLINK '(' listexpr ')' {
	    enode *e;
	    $$ = e->do_unlink($3);
	}
	| VARIABLE '[' expr ']' %prec '(' {
	    $$ = new enode(enode::variable_at_i, $1, $3); 
	}
	| STRING { $$ = new enode(enode::text, $1); }
	| expr DOT expr	        {
	    if ($1->get_type()==enode::text && $3->get_type() == enode::text) {
		string rr($1->eval_as_text().get_text());
		rr += $3->eval_as_text().get_text();
		rr.c_str();
		$$ = new enode(enode::text, rr.c_str());
		$1->~enode();
		$3->~enode();
	    } else {
		enode *e;
		$$ = e->concatenate_text($1, $3);
	    }
	}
	| expr STRING_EQ expr   { $$ = new enode(enode::text_eq, $1, $3);}
	| expr STRING_NE expr   { $$ = new enode(enode::text_ne, $1, $3);}
        /* Add var comparison etc (3 more cases) */
	| expr '<' expr	{ $$ = new enode(enode::lt,  $1, $3); }
	| expr LE expr	{ $$ = new enode(enode::le,  $1, $3); }
	| expr '>' expr	{ $$ = new enode(enode::gt,  $1, $3); }
	| expr EQ expr	{ $$ = new enode(enode::eq,  $1, $3); }
	| expr NE expr	{ $$ = new enode(enode::ne,  $1, $3); }
	| expr GE expr	{ $$ = new enode(enode::ge,  $1, $3); }
        | expr OR  expr	{ $$ = new enode(enode::boolean_or,  $1, $3); }
	| expr AND expr	{ $$ = new enode(enode::boolean_and, $1, $3); }
	| expr '?' expr ':' expr {
	    enode *e;
	    $$ = e->question_mark($1, $3, $5);
	}
	| expr '+' expr	{
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode($1->eval().get_numeric() + $3->eval().get_numeric());
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::add, $1, $3);
	    }
	}
	| expr '-' expr	{
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode($1->eval().get_numeric() - $3->eval().get_numeric());
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::sub, $1, $3);
	    }
	}
	| expr MULOP expr	{ 
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode($1->eval().get_numeric() * $3->eval().get_numeric());
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::mul, $1, $3);
	    }
	}
	| expr '/' expr	{
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode($1->eval().get_numeric() / $3->eval().get_numeric());
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::div, $1, $3);
	    }
	}
	| expr '%' expr	{
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode(fmod($1->eval().get_numeric(), $3->eval().get_numeric()));
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::remainder, $1, $3);
	    }
	}
	| expr POWOP expr	{ 
	    if ($1->get_type() == enode::constant && $3->get_type() == enode::constant) {
		$$ = new enode(pow($1->eval().get_numeric(), $3->eval().get_numeric()));
		$1->~enode();
		$3->~enode();
	    } else {
		$$ = new enode(enode::power, $1, $3);
	    }
	}
	| NOT expr %prec UNARY		{ $$ = new enode(enode::boolean_not, $2, NULL);}
	| '(' expr r_paren		{ $$ = $2;}
	| FUNCTION_USER '(' ')' { // USER FUNCTION
	    $$ = new enode('u', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), NULL);
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| FUNCTION_USER '(' listexpr ')' { // USER FUNCTION
	    $$ = new enode('u', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), $3);
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| FUNCTION_BUILTIN_NUMERIC '(' ')' { // BUILTIN FUNCTION
	    $$ = new enode('b', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), NULL); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| FUNCTION_BUILTIN_NUMERIC '(' listexpr ')' { // BUILTIN FUNCTION
	    $$ = new enode('b', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), $3); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| FUNCTION_BUILTIN_TEXT '(' expr ')' { // BUILTIN FUNCTION
	    // I think this matches 'close(HANDLE)'
	    wnode *w = new_wnode_expr($3);
	    $$ = new enode('B', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), w); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| FUNCTION_BUILTIN_TEXT '(' expr ',' expr ')' { // BUILTIN FUNCTION
	    // I think this matches 'open(HANDLE,"a")'
	    wnode *w = new_wnode_expr($3);
	    w = append_right(w, new_wnode_expr($5));
	    $$ = new enode('B', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), w);
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	}
	| '-' expr %prec UNARY { $$ = new enode(enode::negative, $2, NULL);}
	| '+' expr %prec UNARY { $$ = $2;}
	| VARIABLE POSTDEC  { $$ = new enode(enode::scalar_postdec, $1, NULL);}
	| VARIABLE POSTINC  { $$ = new enode(enode::scalar_postinc, $1, NULL);}
	| VARIABLE '[' expr ']' POSTDEC  { 
	    $$ = new enode(enode::vector_postdec, $1, $3);
	}
	| VARIABLE '[' expr ']' POSTINC  { 
	    $$ = new enode(enode::vector_postinc, $1, $3);
	}
	;

r_paren
	: ')' { yyerrok; save_string = false; }
	;

SC
	: ';' { yyerrok; cmd_expected_next(); in_my = false;}
	;


listexpr
	: expr { $$ = new_wnode_expr($1); }
	| listexpr ',' expr { $$ = append_right($1, new_wnode_expr($3)); }
	;



%%


GriString line_buffer;
int
next_char()			// Get next character, from string or from file
{
    if (cmdfile_fp == NULL) {	// get from string
	if (cmdfile_parse_where == (int)line_buffer.size()) {
            //printf("DEBUG: next_char() detected 'EOF' of string\n");
            at_eof = true;
            pop_cmdfile();
	    return EOF;
        }
	if (line_buffer[cmdfile_parse_where] == '\n') {
	    // Note: cmdfile_line incremented by yylex
	    cmdfile_line_position = 0;
	}
	cmdfile_line_position++;
	if (_gre_debug & DEBUG_TRACE && (trace_buffer.size() > 0 || line_buffer[cmdfile_parse_where] != '\n')) trace_buffer += line_buffer[cmdfile_parse_where];
        at_eof = false;
	return line_buffer[cmdfile_parse_where++];
    } else {
	if (cmdfile_parse_where == (int)line_buffer.size()) {
	    if (!line_buffer.line_from_FILE(cmdfile_fp)) {
//              cmdfile_line_position = 0;
//		cmdfile_line = 1;
//		cmdfile_parse_where = 0;
                //printf("DEBUG: next_char() detected EOF\n");
                at_eof = true;
                int files_left = pop_cmdfile();
                if (files_left > 0) {
                    //printf("\n**** EOF, but not end!!\n");
                    return next_char();
                } else {
                    //printf("\n**** EOF ... THE END\n");
                    return EOF;
                }
	    }
	    //printf("\nnext_char() got another line, length=%d: '%s",line_buffer.size(), line_buffer.c_str());
            cmdfile_line_position = 0;
	    cmdfile_parse_where = 0;
	}
        cmdfile_line_position++;
	if (_gre_debug & DEBUG_TRACE && (trace_buffer.size() > 0 || line_buffer[cmdfile_parse_where] != '\n')) trace_buffer += line_buffer[cmdfile_parse_where];
//	printf("next_char() returning line_buffer[%d] = '%c'\n", cmdfile_parse_where, line_buffer[cmdfile_parse_where]);
        at_eof = false;
	return line_buffer[cmdfile_parse_where++];
    }
}
int
last_char()
{
    if (cmdfile_parse_where > 0) {
	//printf("last_char returning '%c'\n", line_buffer[cmdfile_parse_where-1]);
	return line_buffer[cmdfile_parse_where - 1];
    } else {
	//printf("last_char returning NULL\n");
	return '\0';
    }
}
void
pop_char()
{
    //printf("pop_char entered with cmdfile_parse_where=%d   cmdfile_line_position=%d\n", cmdfile_parse_where,cmdfile_line_position);
    if (_gre_debug & DEBUG_TRACE) trace_buffer.STRING_ERASE(trace_buffer.size()-1, trace_buffer.size());
    if (cmdfile_parse_where > 0)
	cmdfile_parse_where--;
    if (cmdfile_line_position > 0)
        cmdfile_line_position--;
}

int
bad_char_in_prototype(int c)	// make error, flush to r_paren
{
    string msg("Illegal character `");
    msg += char(c);
    msg += "' in prototype; only '$', '@' and ';' allowed";
    yyerror(msg.c_str());
    yylval.cval = "";
    while (')' != (c = next_char())) // flush to r_paren
	;
    if (c == ')')
	pop_char();
    expecting_prototype = false;
    defining_sub_or_cmd = false;
    return PROTOTYPE;
}

int
yylex()
{
	static int last_token = 0;
	static string buf, sbuf;
	int c;
	
	// Special care must be taken for pattern-matching.
	if (expecting_matchstring) {
		if ((c = next_char()) == '/') {
			string the_end = "/";
			do {                // loop lets me add other options
				c = next_char();
				switch (c) {
				case 'i':
					the_end += c;
					break;
				default:
					pop_char();
					break;
				}
			} while (c == 'i');
			//printf("match end is '%s'\n", the_end.c_str());
			yylval.cval = strdup(the_end.c_str());
			return last_token = END_MATCH_STRING;
		}
		pop_char();             // put back the first character
		string sbuf = "";
		while ((c = next_char()) != '/') {
			sbuf += c;
			//printf("'%c'  ->  '%s'\n", c, sbuf.c_str());
		}
		if (sbuf.size() > 0)    // put the '/' back
			pop_char();
		string insert_variable_links(string s);
		string sbuf2 = insert_variable_links(sbuf);
		yylval.cval = strdup(sbuf2.c_str());
		expect = a_expr;
		//printf("yylex returned MATCH_STRING.  '%s'\n", yylval.cval);
		return last_token = MATCH_STRING;
	} // if (expecting_matchstring)
	
	// Skip any whitespace at start
	while ((c = next_char()) == ' ' || c == '\t' || c == '\n' || c == NP) {
		if (c == '\n') {
			cmdfile_line++;
		}
	}
	
	// Check for "s/" part of "s/from/to/" ...
	if (c == 's') {
		int cc = next_char();
		if (ispunct(cc)) {
			s_delimiter = cc;
			string from = "";
			c = next_char();
			if (c == EOF) {
				fatal_error("Substitution replacement not terminated.");
				gre_exit(1);
			}
			from += c;
			//printf("DEBUG 'from starts at '%s'\n",from.c_str());
			while (1) {
				c = next_char();
				//printf("DEBUG  got '%c' (%s)\n", c, from.c_str());
				if (c == EOF) {
					fatal_error("Substitution replacement not terminated");
					gre_exit(1);
				}
				if (c == s_delimiter) {
					yylval.cval = strdup(from.c_str());
					expect = s_to;
				        //printf("DEBUG: S_FROM '%s'\n", yylval.cval);
					return last_token = S_FROM;
				}
				from += c;
				//printf("DEBUG 'from' is now '%s'\n",from.c_str());
			}
			fatal_error(cmdfile_name, cmdfile_line, "Internal error #1 in s///.");
		}
		pop_char();
	}
	// ... check for "to" part of "s/from/to/"
	if (expect == s_to) {
		//printf("\n... in S_TO (c='%c') ...\n", c);
		string to = "";
		// First check for empty string ...
		if (c == s_delimiter) {
			expect = a_expr;
			int cc;
			while (EOF != (cc = next_char())) {
				if (cc == 'g')
					to += "g";
				else if (cc == 'i')
					to += "i";
				else {
					pop_char();
					break;
				}
			}
			yylval.cval = strdup(to.c_str());
			//printf("FINALLY, S_TO is '%s'\n", yylval.cval);
			return last_token = S_TO;
		}
		// ... not empty, so better accumulate!
		to += c;
		while (1) {
			c = next_char();
			//printf(" S_TO  got '%c' (%s)\n", c, to.c_str());
			if (c == EOF) {
				fatal_error(cmdfile_name, cmdfile_line, "Substitution replacement not terminated");
				gre_exit(1);
			}
			if (c == s_delimiter) {
				int cc;
				while (EOF != (cc = next_char())) {
					if (cc == 'g')
						to += "g";
					else if (cc == 'i')
						to += "i";
					else {
						pop_char();
						break;
					}
				}
				yylval.cval = strdup(to.c_str());
				expect = a_expr;
				//printf("    debug: S_TO '%s'\n", yylval.cval);
				return last_token = S_TO;
			}
			to += c;
		}
		fatal_error(cmdfile_name, cmdfile_line, "Internal error #2 in s///");
	}
	
	
	// Check for # comment.  If so, skip to end of line, then 
	// skip any white-space at start of next line.  Also, skip
	// multiple comments even if they are separated by white-space
	// lines.
	while (c == '#') {
		if (_gre_debug & DEBUG_PARSE_COMMENT) {
			if ((c = next_char()) == '#') {
				pop_char();
				putc('#', stdout);
				while ((c = next_char()) != '\n')
					putc(c, stdout);
				putc('\n', stdout);
			} else {
				pop_char();
				while ((c = next_char()) != '\n')
					;
				cmdfile_line++;
			}
			cmdfile_line++;
		} else {
			while ((c = next_char()) != '\n')
				;
			cmdfile_line++;
		}
		
		// Skip any whitespace at start of line following comment
		while ((c = next_char()) == ' ' || c == '\t' || c == '\n' || c == NP) {
			if (c == '\n') {
				cmdfile_line++;
			}
		}
	}
	
	if (c == ';' && !expecting_prototype) {
		yylval.cval = ";";
		expect = a_cmdword;
		//printf("\n ; ");
		return last_token = ';';
	}
	if (c == '{') {
		yylval.cval = "{";
		expect = a_cmdword;
		return last_token = '{';
	}
	if (c == '}') {
		yylval.cval = "}";
		return last_token = '}';
	}
	if (c == '!') {
		if ((c = next_char()) == '=') {
			yylval.cval = "!="; 
			return last_token = NE;
		} else {
			pop_char();
			return last_token = NOT;
		}
	}
	if (c == '(') {
		if (defining_sub_or_cmd) {
			//printf("Got '(' ... and defining_sub_or_cmd.\n");
			expecting_prototype = true;
		} else {
			//printf("Got '(' ... but NOT defining_sub_or_cmd.\n");
		}
		return last_token = '(';
	}
	if (c == ')') {
		if (expecting_prototype) {
			yylval.cval = "";
			pop_char();
			expecting_prototype = false;
			defining_sub_or_cmd = false;
			//printf("Got ')' to complete prototype\n");
			return last_token = PROTOTYPE;
		}
		return last_token = ')';
	}
	if (c == '<') {
		if ((c = next_char()) == '=') {
			yylval.cval = "<=";
			return last_token = LE;
		} else {
			pop_char();
			expect = a_word;
			cmd_not_expected_next();
			return last_token = '<';
		}
	}
	if (c == '>') {
		if ((c = next_char()) == '=') {
			yylval.cval = ">=";
			return last_token = GE;
		} else {
			pop_char();
			expect = a_expr;
			return last_token = '>';
		}
	}
	if (c == '&') {		/* logical and */
		if ((c = next_char()) == '&') {
			yylval.cval = "&&"; 
			return last_token = AND;
		} else {
			pop_char();
			return last_token = '&';
		}
	}
	if (c == '|') {		/* logical and */
		if ((c = next_char()) == '|') {
			yylval.cval = "||"; 
			return last_token = OR;
		} else {
			pop_char();
			return last_token = '|';
		}
	}
	if (c == EOF) {		/* prob in wrong place */
		//printf("yylex: encountered EOF\n");
		if (pop_cmdfile() > 0)
			return last_token = yylex();
		else
			return last_token = 0;
	}
	if (c == '=') {
		if (in_my)		// my refs 2804 3166 3333 3468
			in_my = false;
		if ((c = next_char()) == '=') {
			yylval.cval = "==";
			return last_token = EQ;
		} else if (c == '~') {
			yylval.cval = "=~";
			//printf("Got a =~\n");
			return last_token = ASSIGNOP;
		} else {
			pop_char();
			if (last_token == WORD || last_token == STRING) {
				//printf("\n\t      =     with last a WORD or STRING\n");
				yylval.cval = "=";
				return last_token = COLUMN_IDENT;
			} else {
				yylval.cval = "=";
				return last_token = ASSIGNOP;
			}
		}
	}
	if (c == '%') {
		return last_token = '%';
	}
	if (c == '+') {
		if ((c = next_char()) == '=') {
			yylval.cval = "+=";
			return last_token = ASSIGNOP;
		} else {
			pop_char();
			if ((c = next_char()) == '+') {
				yylval.cval = "++";
				return last_token = POSTINC; 
			}
			pop_char();
			return last_token = '+';
		}
	}
	if (c == '-') {
		if ((c = next_char()) == 'f') {
			yylval.cval = "-f";
			return last_token = FILE_TEST;
		} else if (c == 'd') {
			yylval.cval = "-d";
			return last_token = FILE_TEST;
		} else if (c == 'r') {
			yylval.cval = "-r";
			return last_token = FILE_TEST;
		} else if (c == '=') {
			yylval.cval = "-=";
			return last_token = ASSIGNOP; 
		} else {
			pop_char();
			if ((c = next_char()) == '-') {
				yylval.cval = "--";
				return last_token = POSTDEC; 
			}
			pop_char();
			return last_token = '-';
		}
	}
	/*
	 * Check for / (division or beginning of match string),
	 * or /= (divide-assign).  Note: the end of match string
	 * is handled separately, at token END_MATCH_STRING.
	 */
	if (c == '/') {
		if ((c = next_char()) == '=') {
			yylval.cval = "/=";
			return last_token = ASSIGNOP;
		} else {
			pop_char();
			return last_token = '/';
		}
	}
	if (c == '*') {
		if ((c = next_char()) == '=') {
			yylval.cval = "*=";
			return last_token = ASSIGNOP; 
		} else if (c == '*') {
			if ((c = next_char() == '=')) {
				yylval.cval = "**=";
				return last_token = ASSIGNOP;
			} else {
				pop_char();
				yylval.cval = "**";
				return last_token = POWOP;
			}
		} else {
			pop_char();
			return last_token = MULOP;
		}
	}
	if (c == '"') {		/* string is given as 1 word */
		//unsigned int i = 0;	/* in buf */
		//unsigned max = 100;	/* BUG */
		sbuf = "";
		char last_c = '\0';
		while ((c = next_char()) != EOF) {
			sbuf += char(c);
			if (c == '"') {
				//printf("got a quote.  now have '%s' and last_c='%c'\n", sbuf.c_str(),last_c);
				if (last_c == '\\') {
					// Remove the backslash, by taking off 2 characters
					sbuf.replace(sbuf.length() - 2, 2, "\"");
					//printf("last was backslash.  -> '%s'\n", sbuf.c_str());
				} else {
					//printf("last was NOT backslash\n");
					break;	// end of string
				}
			}
			last_c = c;
		}
		if (c == EOF) {
			yyerror("End of file before end of string");
			pop_cmdfile();
			return last_token = 0;
		}
		if (sbuf.size() > 0 && sbuf[sbuf.size() - 1] == '"')
			sbuf.STRING_ERASE(sbuf.size()-1, 1);
		string insert_variable_links(string s);
		string sbuf2 = insert_variable_links(sbuf);
		yylval.cval = strdup(sbuf2.c_str());
		expect = a_expr;
		return last_token = STRING;
	}
	buf = "";			// clear it out
	// Prototype for 'cmd'
	if (expecting_prototype) {
		if (c != '$' && c != '@' && c != ';')
			return last_token = bad_char_in_prototype(c);
		buf += char(c);
		while ((c = next_char())!=' ' && c!='\t' && c!='\n' && c!=NP && c!=')') {
			buf += c;
			if (c != '$' && c != '@' && c != ';')
				return last_token = bad_char_in_prototype(c);
		}
		yylval.cval = strdup(buf.c_str());
		pop_char();
		expecting_prototype = false;
		defining_sub_or_cmd = false;
		//printf("   ... got prototype '(%s)'\n", yylval.cval);
		return last_token = PROTOTYPE;
	}

	/* Variable name*/
	if (c == '$') {
		if (last_char() == '\\') {
			yyerror("BUG: cannot handle \\$ yet");
			return last_token = 0;
		}
		buf += char(c);
		bool want_end = false;
		c = next_char();
		if (c == '#')
			want_end = true;
		else
			buf += char(c);
		while ((c = next_char()) != '\n' 
		       && (isalpha(c) || isdigit(c) || c == '_')) {
			buf += char(c);
		}
		yylval.cval = strdup(buf.c_str());
		pop_char();
		expect = a_expr;

		if (in_my) {
			// my refs 2804 3166 3333 3468
			var *v = new var(yylval.cval, "");
			scalar_vars.push_back(*v);
			//printf("3329: created var '%s' at index %d (since in_my)\n", yylval.cval,scalar_vars.size()-1);
		} else {
			int which = get_scalar_var_index(yylval.cval);
			if (which < 0) {
				var *v = new var(yylval.cval, "");
				scalar_vars.push_back(*v);
				if (_gre_debug & DEBUG_VARIABLES) printf("DEBUG: new variable '%s' at index %d\n", yylval.cval, get_scalar_var_index(yylval.cval));
			} else {
				if (_gre_debug & DEBUG_VARIABLES) printf("DEBUG: existing variable '%s' at index %d\n", yylval.cval, which);
			}
		}
	    
		//printf("yylex() parsed '$%s::%s'\n", package_name.c_str(), 1+yylval.cval);
		if (want_end) {
			//printf("returning VARIABLE_END '%s'\n", yylval.cval);
			return last_token = VARIABLE_END;
		} else {
			if (isdigit(*(yylval.cval + 1))) {
				//printf("PATTERN VAR...");
				//printf("returning VARIABLE '%s'\n", yylval.cval);
				return last_token = PATTERN_VARIABLE;
			} else {
				return last_token = VARIABLE;
			}
		}
	}
	/* Array name*/
	if (c == '@') {
		if (last_char() == '\\') {
			yyerror("BUG: cannot handle \\@ yet");
			return last_token = 0;
		}
		buf += char(c);
		while ((c = next_char()) != '\n' 
		       && (isalpha(c) || isdigit(c) || c == '_' || c == '#')) {
			buf += char(c);
		}
		yylval.cval = strdup(buf.c_str());
		pop_char();
		expect = a_expr;
		if (in_my) {
			// my refs 2804 3166 3333 3468
			char *name = strdup(yylval.cval);
			name[0] = '$';	// fool name recognizer
			var *v = new var(name, "");
			vector_vars.push_back(*v);
			//printf("%s:%d: created vector var '%s' at index %d (since in_my)\n",__FILE__,__LINE__, yylval.cval,vector_vars.size()-1);
			//for (int i = 0; i < vector_vars.size(); i++) printf("  %d: '%s'\n", i, vector_vars[i].get_name());
			free(name);
		}
		return last_token = ARRAY;
	}

	// Check for DOT
	if (c == '.') {
		if (isdigit(next_char())) {
			pop_char();
		} else {
			yylval.cval = ".";
			return last_token = DOT;
		}
	}

	// Check for number.
	if (isdigit(c) || c == '.') { /* NUM */
		int chars = 0;
		int last_underline = 0;
		buf += char(c);
		while ((c = next_char()) != '\n' && 
		       (c == '_'
			|| c == '0' || c == '1' || c == '2' || c == '3'
			|| c == '4' || c == '5' || c == '6' || c == '7'
			|| c == '8' || c == '9'
			|| c == '.'
			|| c == 'e' || c == 'E'
			|| c == 'd' || c == 'D')) {
			chars++;
			if (c == 'E' || c == 'd' || c == 'D') /* convert to one method */
				c = 'e';
			if (c == '_') {
				if (last_underline && chars - last_underline != 4)
					compile_warning("misplaced _ in constant", NULL);
				last_underline = chars;
			} else {
				buf += char(c);
			}
			if (c == 'e') {
				c = next_char(); // will allow '+' or '-'
				if (!(c == '0' || c == '1' || c == '2' || c == '3'
				      || c == '4' || c == '5' || c == '6' || c == '7'
				      || c == '8' || c == '9'
				      || c == '-' || c == '+')) {
					yyerror("Illegal character after 'e' or 'E' in number");
					return last_token = 0;
				}
				buf += char(c);
			}
		}
		if (last_underline && chars - last_underline != 3)
			compile_warning("misplaced _ in constant", NULL);

		pop_char();
		sscanf(buf.c_str(), "%lf", &(yylval.dval));

		double the_value;
		char *ptr = NULL;
		the_value = strtod(buf.c_str(), &ptr);
		if (*ptr == '\0') {
			/* normal number*/
			yylval.dval = the_value;
			expect = a_expr;
			return last_token = NUM;
		}
		yyerror("Cannot decode token as a numerical value");
		return last_token = 0;
	} 
	/* 
	 * Word-type tokens end on whitespace or on punctuation.
	 */
	if (isalpha(c)) {
		string word_buf = "";
		word_buf += char(c);
		while ((c = next_char()) != '\n' && c != ' ' && c != NP && c != '\t' && (isalnum(c) || c == '_' || c == '.'))
			word_buf += char(c);
		pop_char();

#if 0	
		// 19990807 1999-Aug-7: GIVE UP ON THIS FOR NOW.

		// 19990716 1999-Jul-16: DOES THIS WORK??

		// Recognize special case of `log' as in 
		// `set x type log', as opposed to `$x = log(10)',
		// by checking whether expecting a word or not.

		if (!strcmp(word_buf.c_str(), "log")) {
			yylval.cval = "log";
			if (defining_sub_or_cmd) {
				printf("%s:%d recognized 'log' and made it WORD\n",__FILE__,__LINE__);
				return last_token = WORD;
			} else {
				printf("%s:%d recognized 'log' and made it CMDWORD\n",__FILE__,__LINE__);
				cmd_word_index++;
				return last_token = CMDWORD;
			}
		}
#endif


		// Recognize special case of `grid' following `read', and return
		// token if so ... otherwise `grid' is just a normal word.
		if (!strcmp(word_buf.c_str(), "grid") && last_token == READ) {
			//printf("... returning 'READ_GRID'\n");
			yylval.cval = strdup(word_buf.c_str());
			return last_token = READ_GRID;
		}

		if (!strcmp(word_buf.c_str(), "columns") 
		    && (last_token == READ || last_token == PRINT)) {
			yylval.cval = strdup(word_buf.c_str());
			expect = a_word;
			return last_token = COLUMNS;
		}
		if (!strcmp(word_buf.c_str(), "my")) {
			in_my = true;	// cancelled if hit '=' or ';'
		}
		if (!strcmp(word_buf.c_str(), "cmd")) {
			defining_sub_or_cmd = true;
			yylval.cval = strdup(word_buf.c_str());
			expect = a_word;
			return last_token = CMD;
		}
		if (!strcmp(word_buf.c_str(), "sub")) {
			defining_sub_or_cmd = true;
			yylval.cval = strdup(word_buf.c_str());
			expect = a_word;
			return last_token = SUB;
		}

		// Check keyword list
		int kw_index = -1;
		int kw = 0;
		const char *the_word_buf = word_buf.c_str();
		unsigned int the_word_buf_len = word_buf.size();
		do {
			if (the_word_buf_len == kw_list[kw].len
			    && !strcmp(the_word_buf, kw_list[kw].word)) {
				kw_index = kw_list[kw].code;
				//printf(" %d '%s'\n",kw,kw_list[kw].word);
				break;
			}
		} while (kw_list[++kw].word != NULL);
		if (kw_index != -1) {
			yylval.cval = new char[1 + the_word_buf_len];
			strcpy(yylval.cval, the_word_buf);
			//if (kw_index == CMD) printf("\t... yylex CMD\n");
			//if (kw_index == SUB) printf("\t... yylex SUB\n");
			if (kw_index == CMD 
			    || kw_index == SUB 
			    || kw_index == OPEN
			    || kw_index == READ
			    || kw_index == REWIND
			    || kw_index == CLOSE
			    //|| kw_index == WHILE // probably can remove soon
			    || kw_index == FOR
			    || kw_index == FOREACH
			    || kw_index == EXECUTE) {
				expect = a_word;
				cmd_not_expected_next();
			}
			return last_token = kw_index;
		}
	
		// Check against built-in functions, first textual then numeric
		unsigned int i;

		// ********************************************
		// *** BUG -- DOES NOT SUPPORT POLYMORPHISM ***
		// ********************************************

		// Check functions of 0 text arguments
		extern SUB0_TEXT sub0_text[];
		i = 0;
		while (sub0_text[i].fcn) {
			if (word_buf == sub0_text[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(0);
				function_takes_max_arg.push_back(0);
				return last_token = FUNCTION_BUILTIN_TEXT;
			}
			i++;
		}
		// Check functions of 1 text argument.
		extern SUB1_TEXT sub1_text[];
		i = 0;
		while (sub1_text[i].fcn) {
			if (word_buf == sub1_text[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(1);
				function_takes_max_arg.push_back(1);
				return last_token = FUNCTION_BUILTIN_TEXT;
			}
			i++;
		}
		// Check functions of 2 text arguments.
		extern SUB2_TEXT sub2_text[];
		i = 0;
		while (sub2_text[i].fcn) {
			if (word_buf == sub2_text[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(2);
				function_takes_max_arg.push_back(2);
				return last_token = FUNCTION_BUILTIN_TEXT;
			}
			i++;
		}
		// Check if it's a builtin function
		extern SUB0 sub0[];
		i = 0;
		while (sub0[i].fcn) {
			if (word_buf == sub0[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(0);
				function_takes_max_arg.push_back(0);
				return last_token = FUNCTION_BUILTIN_NUMERIC;
			}
			i++;
		}
		/* Check functions of 1 argument -- numeric */
		extern SUB1 sub1[];
		i = 0;
		while (sub1[i].fcn) {
			if (word_buf == sub1[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(1);
				function_takes_max_arg.push_back(1);
				return last_token = FUNCTION_BUILTIN_NUMERIC;
			}
			i++;
		}
		// Check functions of 2 numeric arguments.
		extern SUB2 sub2[];
		i = 0;
		while (sub2[i].fcn) {
			if (word_buf == sub2[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(2);
				function_takes_max_arg.push_back(2);
				return last_token = FUNCTION_BUILTIN_NUMERIC;
			}
			i++;
		}
		// Check functions of 3 numeric arguments.
		extern SUB3 sub3[];
		i = 0;
		while (sub3[i].fcn) {
			if (word_buf == sub3[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(3);
				function_takes_max_arg.push_back(3);
				return last_token = FUNCTION_BUILTIN_NUMERIC;
			}
			i++;
		}
		// Check functions of 4 numeric arguments.
		extern SUB4 sub4[];
		i = 0;
		while (sub4[i].fcn) {
			if (word_buf == sub4[i].name) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(4);
				function_takes_max_arg.push_back(4);
				return last_token = FUNCTION_BUILTIN_NUMERIC;
			}
			i++;
		}
		//printf("Check word '%s' against user-defined functions (numbering %d) ...\n", word_buf.c_str(),user_sub_stack.size());
		// Check against user-defined functions
		extern vector<user_sub> user_sub_stack;
		for (i = 0; i < user_sub_stack.size(); i++) {
			//printf("\tcheck if present (%s) matches sub %d (%s)\n",word_buf.c_str(),i,user_sub_stack[i].get_name().c_str());
			if (word_buf == user_sub_stack[i].get_name()) {
				function_which.push_back(i);
				function_takes_min_arg.push_back(user_sub_stack[i].get_min_args());
				function_takes_max_arg.push_back(user_sub_stack[i].get_max_args());
				yylval.cval = strdup(word_buf.c_str());
				return last_token = FUNCTION_USER;
			}
		}
		/* Just a word, unknown at this time */
		yylval.cval = strdup(word_buf.c_str());
		//printf("\n\n\t~~~ yylex about to return WORD '%s' cmd_word_index = %d\n",yylval.cval,cmd_word_index);

#if 1
		if (expect == a_cmdword) {
			cmd_num_matches = 0; // reset
			unsigned int nc;
			if (cmd_word_index == 0)
				for (nc = 0; nc < cmd_may_match.size(); nc++)
					cmd_may_match[nc] = true;
			for (nc = 0; nc < cmd_stack.size(); nc++) {
				if (cmd_may_match[nc]) {
					wnode *nc_w = cmd_stack[nc].get_syntax();
					//printf("***examine cmd %d, target '%s' ... cmd first word '%s'; must now skip %d words ...\n",nc,yylval.cval,nc_w->w,cmd_word_index);
					// skip to corresponding word of cmd
					for (unsigned int count = 0; count < cmd_word_index; count++) {
						//printf("\tskipping '%s'\n", nc_w->w);
						nc_w = nc_w->next;
						if (nc_w == NULL) {
							cmd_may_match[nc] = false;
							//printf("\tCANNOT be a match\n");
							break;
						}
					}
					//printf("Command %d - must check target '%s' need_to_check_word=%d\n", nc, yylval.cval,cmd_may_match[nc]);
					if (cmd_may_match[nc]) {
						if (!strcmp(nc_w->w, yylval.cval)) {
							//printf("'%s' matches so far to cmd %d\n",yylval.cval,nc);
							if (cmd_stack[nc].get_syntax_length() == 1 + cmd_word_index) {
				//printf("'%s' is PRECISE match to cmd %d\n",yylval.cval,nc);
								cmd_matches_to = nc;
								cmd_num_matches++;
							}
						} else {
							cmd_may_match[nc] = false;
						}
					}
				}
			}
			//printf("This word (%s), and previous, match %d commands, most recently cmd number %d\n",yylval.cval,cmd_num_matches,cmd_matches_to);
			cmd_word_index++;
			//printf("\tyylex CMDWORD `%s' expecting=%d\n",yylval.cval,expect);
			return last_token = CMDWORD;
		}
		if (expect == a_expr) {
			//printf("\n=== returning '%s' as a STRING\n", yylval.cval);
			return last_token = STRING;
		}
		if (expect == a_word) {
			//printf("\tyylex WORD `%s' reported, since expecting=%d\n",yylval.cval,expect);
			return last_token = WORD;
		}
		printf("\n\n*** ERROR (INTERNAL): `expect' is MESSED UP\n");
#endif
	}
	return last_token = c;
}

/* Scan string for variable names, making substitutions like:
 *
 * "... $foo ... $3 ..." -> "... $_10 ... $^3 ..."
 *
 * where $_10 indicates that the variable '$foo' is the 10-th
 * item in the 'scalar_vars' list (or the 'vector_vars' list),
 * and $^3 indicates that '$3' is the 3-rd item in the 'pattern_match'
 * list.  The pattern_match list corresponds to selected items
 * in a regular expression.
 *
 * If the $ is preceded by a \ character, then this interpolation
 * is not done, and the $ is passed into the string, e.g.
 *
 * open(IN, "gawk '{print \$1,\$2}' test.dat |") or die;
 * while(<IN>){ print; }
 *
 * Following Perl, a special case is that '$0' is just the program name;
 * it is not a pattern-match.
 *
 * CROSS-REFERENCE: the links made here are used by
 * parser.y:interpolate_var_in_string().
 */
string
insert_variable_links(string s)
{
    string rval;
    //printf("insert_variable_links(%s)\n",s.c_str());
    int l = s.size();
    for (int i = 0; i < l; i++) {
	if (s[i] == '$') {
            if (i > 0 && s[i-1] == '\\') {
                rval += s[i];
                continue;
            }
	    bool is_length = false;
            //printf("got '$' next is '%c'\n", s[i+1]);
	    if (s[i + 1] == '#') {
		printf("BUG -- cannot do '$#variable' inside strings.\n");
		is_length = true;
		i += 2;
	    } else {
		i += 1;
	    }

	    string vname("$");

            // Check for '$0', '$1', etc. 
            if (isdigit(s[i])) {
                vname += "^";
                while (s[i] != '\0' && isdigit(s[i])) {
                    vname += s[i];
                    i++;
                }
                i--;            // so can catch this character next time
                rval += vname;
                //printf(" -- is numbered vname '%s' ... rval is now '%s'\n", vname.c_str(),rval.c_str());
                continue;
            }
	    int j;
	    for (j = i; j <= l; j++) {
		char index[20]; 
		char c = s[j];
		if (c == '[') {
		    int which = get_vector_var_index(vname.c_str());
		    //printf("-- is vector '%s' which=%d\n",vname.c_str(),which);
		    rval += is_length ? "$#_" : "$_";
		    sprintf(index, "%d", which);
		    rval += index;
		    break;
		} else if (j == l || c == '\n' || !(isalpha(c) || isdigit(c) || c == '_')) {
		    int which = get_scalar_var_index(vname.c_str());
		    //printf("-- is scalar '%s' which=%d\n",vname.c_str(),which);
		    if (which >= 0) {
			rval += is_length ? "$#_" : "$_";
			sprintf(index, "%d", which);
			rval += index;
		    } else {
 			rval += vname.c_str();
		    }
		    break;
		}
		vname += c;
	    }
	    i = j - 1;
	} else {
	    rval += s[i];
	}
    }
    //printf("\tinsert_variable_links created '%s'\n",rval.c_str());
    return rval;
}

int
yyerror(const char *s)
{
    extern unsigned int error_count;
    printf("%s at %s line %d.\n", s, cmdfile_name, cmdfile_line);
    printf("%s", line_buffer.c_str());
    printf("%*c%c\n", cmdfile_line_position - 1, ' ', '^');
    switch (expect) {
    case a_word: 
	printf(" ... was expecting a word (e.g. a handle name or a column name)\n\n");
	break;
    case a_cmdword: 
	// Commented-out since it never seemed to help the author!!
	//printf("... was expecting a word of a `cmd'.\n\n");
	break;
    case a_expr:
	// Commented-out since it never seemed to help the author!!
	//printf("The parser was expecting an expression.\n\n");
	break;
    case s_from:
	printf("... was expecting the start of a string-change (e.g. 's/' from 's/from/to/').\n\n");
	break;
    case s_to:
	printf("... was expecting the end of a string-change (e.g. '/to/' from 's/from/to/').\n\n");
	break;
    }
    error_count++;
    if (error_count > error_countMAX) {
        printf("Giving up; %d errors found.\n", error_countMAX);
        gre_exit(1);
    }
    return 0;
}
void
compile_warning(const char *s, ...) // end with NULL argument
{
    va_list ap;
    va_start(ap, s);
    while (s != NULL) {
	printf("'%s'", s);
	s = va_arg(ap, const char *);
    }
    va_end(ap);
    printf("\n%s", line_buffer.c_str());
    printf("%*c%c\n", cmdfile_line_position - 1, ' ', '^');
    printf("At %s line %d.", cmdfile_name, cmdfile_line);
}

enode*
logical_negation(enode *e)
{
    return (new enode(enode::boolean_not, e, NULL));
}
wnode*
new_wnode_string(const char *s)
{
    wnode *n = (wnode *) malloc(sizeof(wnode));
    n->w = (char *)malloc(1 + strlen(s));
    strcpy(n->w, s);
    n->t = wnode::word;
    n->next = NULL;
    return n;
}
wnode*
new_wnode_vname(const char *s)
{
    wnode *n = (wnode *) malloc(sizeof(wnode));
    n->w = (char *)malloc(1 + strlen(s));
    strcpy(n->w, s);
    n->t = wnode::vname;
    n->next = NULL;
    return n;
}
wnode*
new_wnode_expr(enode *new_expr)
{
    wnode *n = (wnode *) malloc(sizeof(wnode));
    n->e = new_expr;
    n->t = wnode::expr;
    n->next = NULL;
    return n;
}
wnode*
new_wnode_columns_spec(const char *col_name, enode *col_expr)
{
    wnode *n = (wnode *) malloc(sizeof(wnode));
    n->w = strdup(col_name);	// Q: need to ask for storage??
    n->e = col_expr;
    n->t = wnode::column_spec;
    n->next = NULL;
    return n;
}


/*
 * append 'n' to the right-hand-end of 'list', returning
 * 'n' back again.  Thus, can use as
 *
 *   wnode *n = new_wnode_string("hi");
 *   n = append_right(n, new_wnode_string("Dan");
 *   n = append_right(n, new_wnode_string("ole'");
 *   n = append_right(n, new_wnode_string("buddy");
 */
wnode*
append_right(wnode* list, wnode* n)
{
    register wnode *oldlist;
    static wnode *savefront = NULL;
    static wnode *savetail  = NULL;
    oldlist = list;
    if (savefront == oldlist) {
	savetail = savetail->next = n;
	return oldlist;
    } else
	savefront = oldlist;
    while (list->next != NULL)
	list = list->next;
    savetail = list->next = n;
    return oldlist;
}

cmdnode*
new_cmdnode_package(const char* package)
{
    //printf("new_cmdnode_package ... '%s'\n", package);
    cmdnode *n = (cmdnode *)malloc(sizeof(cmdnode));
    n->type = cmdnode::package;
    package_name = package;
    return n;
}
cmdnode*
new_cmdnode_use()
{
    //printf("new_cmdnode_use()\n");
    cmdnode *n = (cmdnode *)malloc(sizeof(cmdnode));
    n->type = cmdnode::use;
    n->next = NULL;
    return n;
}

cmdnode*
new_user_cmd(wnode* words, int which_cmd, unsigned int num_matching_cmds, wnode* expressions)
{
    if (num_matching_cmds != 1) {
	string msg("Unknown command `");
	wnode *w = words;
	while (w) {
	    msg += w->w;
	    w = w->next;
	    if (w)
		msg += " ";
	}
	msg += "'";
	yyerror(msg.c_str());
	return new_cmdnode(); // NB: with no action
    }
    //NOT USED unsigned number_words = length_of_wnode_list(words);
    unsigned int nc = which_cmd;
    unsigned int num_expr = length_of_wnode_list(expressions);
    if (cmd_stack[nc].get_min_args() > -1 && num_expr < cmd_stack[nc].get_min_args()) {
	string msg("Too few arguments to `");
	wnode *w = words;
	while (w) {
	    msg += w->w;
	    w = w->next;
	    if (w) 
		msg += " ";
	}
	int min = cmd_stack[nc].get_min_args();
	int max = cmd_stack[nc].get_max_args();
	msg += (min == max) ? "' -- must have " : "' -- must have at least ";
	msg += dtoa(min);
	msg += " arguments, but found ";
	msg += dtoa(num_expr);
	yyerror(msg.c_str());
	return new_cmdnode(); // NB: with no action
    }
    if (cmd_stack[nc].get_max_args() > -1 && num_expr > cmd_stack[nc].get_max_args()) {
	string msg("Too many arguments to cmd `");
	wnode *w = words;
	while (w) {
	    msg += w->w;
	    w = w->next;
	    if (w) 
		msg += " ";
	}
	int min = cmd_stack[nc].get_min_args();
	int max = cmd_stack[nc].get_max_args();
	msg += (min == max) ? "' -- must have " : "' -- must have at most ";
	msg += dtoa(max);
	msg += ", but found ";
	msg += dtoa(num_expr);
	yyerror(msg.c_str());
	return new_cmdnode(); // NB: with no action
    }
    return new_newcommand(cmd_stack[nc].get_action(), expressions);
}

cmdnode*
new_if_block(enode* test, cmdnode* true_part, cmdnode* false_part)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::if_block;
    n->test = test;
    n->left = true_part;
    n->right = false_part;
    n->next = NULL;
    return n;
}
cmdnode*
new_do_while_block(enode* test, cmdnode* block)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::do_while_block;
    n->test = test;
    n->left = block;
    n->next = NULL;
    return n;
}
cmdnode*
new_while_block(enode* test, cmdnode* block)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::while_block;
    n->test = test;
    n->left = block;
    n->next = NULL;
    return n;
}

cmdnode*			// call a 'cmd'
new_newcommand(cmdnode* action, wnode *args)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::user_cmd;
#if 1				// oct97: messing around, don't think I need the junk
    n->first_wnode = args;
#else
    printf("\n\nin new_newcommand.  args at addresses...\n");
    wnode *args_traverse = args;
    wnode *copy = new_wnode_expr(args_traverse->e);
    while (args_traverse) {
	printf("\t%d %s\n", int(args_traverse),args_traverse->e->is_textual()? "textual" : "numeric");
	copy = append_right(copy, new_wnode_expr(args_traverse->e));
	args_traverse = args_traverse->next;
    }
    n->first_wnode = copy;
#endif
    n->left = action;
    n->next = NULL;
    return n;
}

cmdnode*
new_read_grid_cmd(const char *item, const char *handle, wnode *args) 
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    int num_args = length_of_wnode_list(args);
    //printf("'read grid' thinks have %d args\n", num_args);
    if (!strcmp(item, "data")) {
	n->type = cmdnode::read_grid_data;
	if (num_args != 2 && num_args != 0)
	    yyerror("`read grid data' must have 0 or 2 arguments");
    } else if (!strcmp(item, "x")) {
	n->type = cmdnode::read_grid_x;
	if (num_args != 1 && num_args != 0) 
	    yyerror("`read grid x' must have 0 or 1 argument");
    } else if (!strcmp(item, "y")) {
	n->type = cmdnode::read_grid_y;
	if (num_args != 1 && num_args != 0) 
	    yyerror("`read grid y' must have 0 or 1 argument");
    } else {
	char msg[200];
	sprintf(msg, "Unknown `read grid' item `%s' -- allowed are `data', `x', and `y'", item);
	yyerror(msg);
	return n;
    }
    n->handle_index = look_up_file_handle(handle, true);
    n->first_wnode = args;
    n->left = NULL;
    n->right = NULL;
    n->next = NULL;
    return n;
}

cmdnode*
new_read_columns_cmd(const char *handle, wnode* columns_spec_list, enode *rows)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::read_columns;
    n->handle_index = look_up_file_handle(handle, true);
    n->left = NULL;
    n->right = NULL;
    n->next = NULL;
    n->start = rows;
    n->column_place[0] = n->column_place[1] = n->column_place[2] = n->column_place[3] = n->column_place[4] = NULL;
    wnode *cs = columns_spec_list;
    int count = 1;
    while (cs) {
	if (!strcmp(cs->w, "x")) {
	    if (n->column_place[0] != NULL) 
		yyerror("may not redefine `x' column place.");
	    else {
		if (cs->e == NULL) 
		    n->column_place[0] = new enode(double(count));
		else 
		    n->column_place[0] = cs->e;
	    }
	} else if (!strcmp(cs->w, "y")) {
	    if (n->column_place[1] != NULL) 
		yyerror("may not redefine `y' column place.");
	    else {
		if (cs->e == NULL) 
		    n->column_place[1] = new enode(double(count));
		else 
		    n->column_place[1] = cs->e;
	    }
	} else if (!strcmp(cs->w, "z")) {
	    if (n->column_place[2] != NULL) 
		yyerror("may not redefine `z' column place.");
	    else {
		if (cs->e == NULL) 
		    n->column_place[2] = new enode(double(count));
		else 
		    n->column_place[2] = cs->e;
	    }
	} else if (!strcmp(cs->w, "u")) {
	    if (n->column_place[3] != NULL) 
		yyerror("may not redefine `u' column place.");
	    else {
		if (cs->e == NULL) 
		    n->column_place[3] = new enode(double(count));
		else 
		    n->column_place[3] = cs->e;
	    }
	} else if (!strcmp(cs->w, "v")) {
	    if (n->column_place[4] != NULL) 
		yyerror("may not redefine `v' column place.");
	    else {
		if (cs->e == NULL) 
		    n->column_place[4] = new enode(double(count));
		else 
		    n->column_place[4] = cs->e;
	    }
	} else {
	    string msg("illegal column name `");
	    msg += cs->w;
	    msg += "'.  Allowed: `x', `y', `z', `u', and `v'.";
	    yyerror(msg.c_str());
	}
	count++;
	cs = cs->next;
    }
    return n;
}

cmdnode*
new_cmdnode()			/* empty, for newcommands */
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::blank;
    n->first_wnode = NULL;
    n->left = NULL;
    n->right = NULL;
    n->next = NULL;
    return n;
}

cmdnode*
new_cmdnode(bool (*fcn)())
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::blank;
    n->first_wnode = NULL;
    n->left = NULL;
    n->right = NULL;
    n->next = NULL;
    n->c_function = fcn;
    return n;
}

cmdnode*
new_cmdnode(wnode *first_w)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::unknown;
    n->first_wnode = first_w;
    n->left = NULL;		/* not required */
    n->right = NULL;		/* not required */
    n->next = NULL;
    return n;
}

cmdnode*
append_right(cmdnode* list, cmdnode* n)
{
    //printf("    doing append_right for cmdnode\n");
    if (n == NULL)		/* skip lines that are comment-only*/
	return list;
    if (n->type == cmdnode::blank)
	return list;		/* skip blank line*/
    if (list == NULL)		/* allow 1st lines of file to be comment*/
	return list = n;
    register cmdnode *oldlist;
    static cmdnode *savefront = NULL;
    static cmdnode *savetail  = NULL;
    oldlist = list;
    if (savefront == oldlist) {
	savetail = savetail->next = n;
        //printf("    ... type 1 return\n");
	return oldlist;
    } else
	savefront = oldlist;
    while (list->next != NULL)
	list = list->next;
    savetail = list->next = n;
    //printf("    ... type 2 return\n");
    return oldlist;
}

scalar the_return_value;

bool				/* 0 if blank or decode to numeric zero*/
string_as_boolean(const char *s)
{
    if (s[0] == '\0')
	return 0;
    char *ptr = NULL;
    double val = strtod(s, &ptr);
    if (*ptr == '\0' && val == 0.0)
	return 0;
    return 1;
}

// Evaluate cmd list (sometimes by calling this fcn recursively.)
// 0->error, 1->OK, 2->last 3->return 4->next
// Note: must check for a break within if-statements
cmd_eval_code
evaluate_cmd(cmdnode *c)
{
    cmd_eval_code return_value = rval_okay;
    if (c == NULL)
	return return_value;
    return_value = rval_error;	// just to be safe
    extern bool _error_in_cmd;
    do {
#if 0                           // 990116 - move to end of loop
	if (_error_in_cmd) {
            printf(" at %s line %d.\n", filename_runtime, fileline_runtime);
	    end_up();
	    gre_exit(255);		/* exit code matches perl*/
	}
#endif
	//printf("\n\t*** in evaluate_cmd.  type=%d\n", c->type);
	//unsigned int words_in_command = 0;  NOT USED
	// Q: is a switch faster??
	if (c->type == cmdnode::if_block) {
	    if (c->test->eval().get_numeric())
		return_value = evaluate_cmd(c->left);
	    else
		return_value = evaluate_cmd(c->right);
	    if (return_value == rval_last)
		return return_value;
	    if (return_value == rval_return)
		return return_value;
	    if (return_value == rval_next)
		return return_value;
	} else if (c->type == cmdnode::while_block) {
	    if (c->test->is_textual()) {
		while (string_as_boolean(c->test->eval().get_text())) {
		    int return_value = evaluate_cmd(c->left);
		    if (return_value == rval_last)
			break;
		    if (return_value == rval_return)
			break;
		    if (return_value == rval_next)
			continue;
		}
	    } else {
		while (c->test->eval().get_numeric()) {
		    int return_value = evaluate_cmd(c->left);
		    if (return_value == rval_last)
			break;
		    if (return_value == rval_return)
			break;
		    if (return_value == rval_next)
			continue;
		}
	    }
	} else if (c->type == cmdnode::do_while_block) {
	    if (c->test->is_textual()) {
		do {
		    int return_value = evaluate_cmd(c->left);
		    if (return_value == rval_last)
			break;
		    if (return_value == rval_return)
			break;
		    if (return_value == rval_next)
			continue;
		} while (string_as_boolean(c->test->eval().get_text()));
	    } else {
		do {
		    int return_value = evaluate_cmd(c->left);
		    if (return_value == rval_last)
			break;
		    if (return_value == rval_return)
			break;
		    if (return_value == rval_next)
			continue;
		} while (c->test->eval().get_numeric());
	    }
	} else if (c->type == cmdnode::for_block) {
	    c->start->eval();
	    while (c->test->eval().get_numeric()) {
		int return_value = evaluate_cmd(c->left);
		c->each->eval();
		if (return_value == rval_last)
		    break;
		if (return_value == rval_return)
		    break;
		if (return_value == rval_next)
		    continue;
	    }
	} else if (c->type == cmdnode::foreach_block) {
	    wnode *n = c->first_wnode;
	    if (n == NULL) { 
		unsigned int which = c->array_index;
		unsigned int num = vector_vars[which].get_size();
		for (unsigned int a = 0; a < num; a++) {
		    if (vector_vars[which].get_at_index(a).is_numeric())
			put_var(c->name, vector_vars[which].get_numeric_vector(a));
		    else
			put_var(c->name, vector_vars[which].get_text_vector(a));
		    cmd_eval_code return_value = evaluate_cmd(c->left);
		    if (return_value == rval_last)
			break;
		    if (return_value == rval_return)
			break;
		    if (return_value == rval_next)
			continue;
		}
	    } else {
		while(n != NULL) {
		    if (n->t != wnode::expr) {
			fatal_error(c->file_name, c->line, "foreach block has invalid type");
			gre_exit(1);
		    } else {
			scalar *f = new scalar(n->e->eval());
                        unsigned int len = f->get_len();
                        for (unsigned int i = 0; i < len; i++) {
                            if (f->is_numeric(i))
                                put_var(c->name, f->get_numeric(i));
                            else
                                put_var(c->name, f->get_text(i));
                            cmd_eval_code return_value = evaluate_cmd(c->left);
                            if (return_value == rval_last)
                                break;
                            if (return_value == rval_return)
                                break;
                            if (return_value == rval_next)
                                break; // caught as 'continue' below
                        }
                        if (return_value == rval_last)
                            break;
                        if (return_value == rval_return)
                            break;
                        if (return_value == rval_next)
                            continue;
                        n = n->next;
		    }
		}
	    }
	} else if (c->type == cmdnode::last_block) {
	    return rval_last;
	} else if (c->type == cmdnode::next_block) {
	    return rval_next;
	} else if (c->type == cmdnode::return_block) {
		//the_return_value = c->first_wnode->e->eval().get_numeric();
	    the_return_value = c->first_wnode->e->eval();
	    return rval_return;
	} else if (c->type == cmdnode::expression) {
	    enode::type t = c->first_wnode->e->get_type();
	    if (t == enode::assign_scalar) {
		c->first_wnode->e->eval();
	    } else if (t == enode::assign_vector) {
		c->first_wnode->e->eval();
	    } else {
    		c->first_wnode->e->eval();
	    }
        } else if (c->type == cmdnode::package) {
            printf("FYI: 'package' command ignored\n");
        } else if (c->type == cmdnode::use) {
            ;
	} else if (c->type == cmdnode::print) {
	    void do_print_command(cmdnode *c);
	    do_print_command(c);
	} else if (c->type == cmdnode::printf) {
	    void do_printf_command(cmdnode *c);
	    do_printf_command(c);
	} else if (c->type == cmdnode::read_grid_data) {
	    bool execute_read_grid_data(cmdnode *c);
	    execute_read_grid_data(c);
	} else if (c->type == cmdnode::read_grid_x) {
	    bool execute_read_grid_xy(char xory, cmdnode *c);
	    execute_read_grid_xy('x', c);
	} else if (c->type == cmdnode::read_grid_y) {
	    bool execute_read_grid_xy(char xory, cmdnode *c);
	    execute_read_grid_xy('y', c);
	} else if (c->type == cmdnode::read_columns) {
	    bool execute_read_columns(cmdnode *c);
	    execute_read_columns(c);
	} else if (c->type == cmdnode::read_var_list) {
	    bool execute_read_var_list(cmdnode *c);
	    execute_read_var_list(c);
	} else if (c->type == cmdnode::split) {
	    /*
	     * Determine target array name
	     */
	    //printf("evaling split.  array index = %d\n", c->array_index);
	    /*
	     * Dump into array, if non-blank
	     */
	    clear_var_vector_at(c->array_index);
	    string s_str;
	    scalar_vars[0].get_text_scalar(s_str);
	    char *s = strdup(s_str.c_str());
	    char *s_to_free = s;
	    if (strlen(s) > 0) {
		unsigned int p = 0;
		char *stop = " \t\n";
		s = strtok(s, stop);
		do {
		    char *ptr = NULL;
		    double the_value = strtod(s, &ptr);
		    if (*ptr == '\0') {
			//printf("  split string '%s' -> number %f\n", s, the_value);
			put_var_vector_numeric_at(c->array_index, p, the_value);
		    } else {
			//printf("  split string '%s'\n", s);
			put_var_vector_text_at(c->array_index, p, s);
		    }
		    p++;
		} while ((s = strtok(NULL, stop)));
	    }
	    free(s_to_free);
	} else if (c->type == cmdnode::my_var_list) {
            //printf("Executing my_var_list.\n");
            wnode *to_execute = c->first_wnode;
            while (to_execute != NULL) {
                //printf(" >>> executing\n");
                to_execute->e->eval();
                to_execute = to_execute->next;
            }
	} else if (c->type == cmdnode::query) {
	    wnode *n = c->first_wnode; /* vname */
	    char *name = n->w;	/* var name */
	    n = n->next;
	    if (n == NULL) {
		printf("Please type value for variable named '%s':  ", name);
	    } else {
		string prompt = interpolate_var_in_string(n->e->eval().get_text());
		n = n->next;
		if (n == NULL) {
		    printf("%s", prompt.c_str());
		} else {
		    printf("BUG: ignoring the `query' restriction list\n");
		    printf("%s", prompt.c_str());
		}
	    }

	    char buf[1024];
	    fgets(buf, 1024, stdin);
	    if (buf[strlen(buf) - 1] == '\n')
		buf[strlen(buf) - 1] = '\0';

	    char *ptr = NULL;
	    double the_value = strtod(buf, &ptr);
	    if (*ptr == '\0') {
		//printf("query takes as numeric '%f'\n", the_value);
		put_var(name, the_value);
	    } else {
		//printf("query takes as text '%s'\n", buf);
		put_var(name, buf);
	    }

	} else if (c->type == cmdnode::C_command) {
	    filename_runtime = c->file_name;
	    fileline_runtime = c->line;
	    // Execute pre-defined C function listed in tags.h file
	    (c->c_function)();
	} else if (c->type == cmdnode::blank) {
	    ;			// empty
	} else if (c->type == cmdnode::user_cmd) {
	    wnode *tmp = c->first_wnode;
	    
	    // Save existing $_[] into old_vector_at_0 so can restore at end
	    static var old_vector_at_0;
            old_vector_at_0 = vector_vars[0];

	    // Must pass through all the args, evaluating all
            // the expressions, before can dump results 
            // into $_[].  So use next var as a temporary store.
	    static var new_vector_at_0;
            new_vector_at_0.clear_vector();

            //printf("\nEvaluating a cmd ... about to dump expressions ...\n");
	    unsigned int arg_index = 0;
	    while (tmp) {
		if (tmp->t == wnode::expr) {
		    if (tmp->e->is_textual()) {
			string the_text = interpolate_var_in_string(tmp->e->eval().get_text());
			//printf("\targ %d (input address %d) -- textual   value '%s'\n",arg_index,int(tmp),the_text.c_str());
			new_vector_at_0.set_value(the_text.c_str(), arg_index);
		    } else {
                        // must be numeric
			    // NOT USED char t[100];
			double value = tmp->e->eval().get_numeric();
			new_vector_at_0.set_value(value, arg_index);
			//printf("\targ %d (input address %d) -- numerical value %f\n",arg_index,int(tmp),value);
			scalar check = new_vector_at_0.get_at_index(arg_index);
			// if (check.is_numeric()) printf(" = %f\n", check.get_numeric()); else printf(" = '%s'\n", check.get_text());

		    }
		} else {
		    char msg[100];
		    sprintf(msg, "INTERNAL ERROR: cannot handle the type, %d", tmp->t);
		    fatal_error(c->file_name, c->line, msg);
		}
		tmp = tmp->next;
		arg_index++;
	    }
	    vector_vars[0].clear_vector(); // clear in case length change

	    // Now that all expressions calculated, put results
            // back into $_[] and call the cmd.
            vector_vars[0] = new_vector_at_0;

	    //printf("about to eval the cmd.  $_ end index is %d\n",vector_vars[0].get_end_index());
	    evaluate_cmd(c->left);
	    vector_vars[0] = old_vector_at_0;
	}
	c = c->next;
        // 990116: moved test moved here, from top of loop
	if (_error_in_cmd) {
            //printf(" at %s line %d.\n", filename_runtime, fileline_runtime);
            printf(" at %s line %d.\n", cmdfile_name, cmdfile_line);
	    end_up();
	    gre_exit(255);		/* exit code matches perl*/
	}
    } while (c != NULL);
    return rval_okay;
}

cmdnode*
new_for_block(enode* start, enode* test, enode* each, cmdnode* block)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::for_block;
    n->start = start;
    n->test  = test;
    n->each  = each;	
    n->left  = block;
    n->next  = NULL;
    return n;
}

cmdnode*
new_foreach_block_list(const char* var_name, wnode* list, cmdnode* block)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::foreach_block;
    n->name = (char *) malloc(1 + strlen(var_name));
    strcpy(n->name, var_name);
    n->first_wnode = list;
    n->left = block;
    n->next = NULL;
    return n;
}
cmdnode*
new_foreach_block_array(const char* var_name, const char* array_name, cmdnode* block)
{
    cmdnode *n = (cmdnode *) malloc(sizeof(cmdnode));
    n->type = cmdnode::foreach_block;
    n->name = (char *) malloc(1 + strlen(var_name));
    strcpy(n->name, var_name);

    char *name = strdup(array_name);
    name[0] = '$';	/* fool name recognizer */
    int which = get_vector_var_index(name);
    if (which < 0) {
	var *v = new var(name, 0.0);
	vector_vars.push_back(*v);
	delete v;
	n->array_index = vector_vars.size() - 1;
	//printf("FOREACH BLOCK case 1 -- had to create for '%s' (index is %d)\n", name, n->array_index);
    } else {
	n->array_index = which;
	//printf("FOREACH BLOCK case 1 -- already had index for '%s' (index is %d)\n", name, n->array_index);
    }
    free(name);
    n->first_wnode = NULL;	// indicates using array
    n->left = block;
    n->next = NULL;
    return n;
}

/*
 * Interpolate variables into string, using codes set up
 * by parser.y:insert_variable_links().
 *
 * NOTE: return value is OVERWRITTEN on each call
 * NOTE: only scalar variables, and variables indexed
 *       by a constant or a scalar, are interpolated.
 */
#define BUFLEN 2048
char buf[BUFLEN];
char *
interpolate_var_in_string(const char* s)
{
	char var_buf[200];
	//printf("\ninterpolate_var_in_string(%s)\n",s);
	unsigned int i = 0, ii = 0;
	if (!s)
		return "";
	do {
		if (s[i] == '\0')
			break;
		// Normal characters are just copied ...
		if (s[i] != '$') {
			buf[ii++] = s[i++];
			continue;
		}
		// OK, now s[i] is '$'.  First, let's see if it is escaped ...
		if (i>0 && s[i-1] == '\\') {
			// The $ is escaped, so gobble '\' and don't interpolate
			ii--;
			buf[ii++] = '$';
			i++;
			continue;
		}

		// BUG
		// BUG -- I've no idea if next if-block is used, or useful.
		// BUG

		// ... no, not escaped.  OK, is it an unknown thing?
		//printf("  char at i=%d,i+1,i+2 = '%c%c%c'\n",i,s[i],s[i+1],s[i+2]);
		if (s[i+1] != '_' && s[i+1] != '^') {
			// Of form "$[a-zA-Z0-9]*" -- just leave it in place.
			//printf("s[%d]='%c' NE _\n", i+1, s[i+1]);
			while (isalnum(s[i]) || s[i] == '$') {
				//printf("        Tacked '%c' to buf\n",s[i]);
				buf[ii++] = s[i++];
				if (s[i] == '\0')
					break;
			}
			continue;
		}

		// No, not unknown.  It is of form "$_N" where N is characters 
		// for index of variable in vector<var>scalar_vars, 
		// or "$^N" where N is index in vector<var>pattern_match.

		// Handle $^DIGIT(s) first.
		if (s[i + 1] == '^') {
			//printf("DIGITS i=%d s+i+2='%s'\n", i, s+i+2);

                        // Next 2 lines skip the '$^' part.
			if (s[i++] == '\0') continue;
			if (s[i++] == '\0') continue;

			char snum[30];       // jeeze, should be long enough!
			int j = 0;
			while (isdigit(s[i])) {
				snum[j++] = s[i++];
				if (j >= 30) {
					INTERNAL_ERROR("$^... buffer exceeded\n");
				}
			}
			snum[j] = '\0';
			int num = atoi(snum);
			if (num < (int)pattern_match.size()) {
				string pm_str;
				pattern_match[num].get_text_scalar(pm_str);
				const char *pm = pm_str.c_str();
				//printf("--- pattern_match[%d] '%s'\n", num, pm);
				while (*pm != '\0')
					buf[ii++] = *pm++;
			}
			continue;
		}

		// Skip next 2 characters.
		if (s[i++] == '\0') continue;
		if (s[i++] == '\0') continue;

		unsigned int n = 0;
		char name[100];
		name[n++] = s[i++];
		if (s[i] == '\0')
			continue;
		while (s[i] == '-' || isdigit(s[i]))
			name[n++] = s[i++];
		name[n] = '\0';
		int index = int(atof(name));
		//printf("    Will interpolate var `%s' (index %d)\n", name, index);
	
		// Catch SIMPLE indexing, only constant allowed now (BUG)
		bool is_vector = false;
		int the_index = -1;
		if (s[i] == '[') {
			is_vector = true;
			i++;
			if (isdigit(s[i])) { // e.g. "$var[0]"
				string sindex;
				while(isdigit(s[i]))
					sindex += s[i++];
				the_index = atoi(sindex.c_str());
				i++;	// skip the ']'
				//printf("DEBUG: constant-indexed vector encountered.  index=%d\n",the_index);
			} else if (s[i] == '$') { // e.g. "$var[$i]"
				i += 2;		// skip the "$_"
				string sindex;
				while(isdigit(s[i]))
					sindex += s[i++];
				the_index = atoi(sindex.c_str());
				//printf("DEBUG: variable-indexed vector encountered. varindex=%d\n",the_index);
				the_index = int(scalar_vars[the_index].get_numeric_scalar());
				//printf("       -> subscript value %d\n", the_index);
				i++;	// skip the ']'
			} else if (s[i] == '\0')
				continue;
		}

		/* Want highest index of item */
		/********* BUG -- this is wrong **********/
		if (name[1] == '#') {
			int which = get_vector_var_index(name);
			if (which == -1) {
				which = get_scalar_var_index(name);
				if (which == -1) {
					fprintf(stderr, "Variable '%s' unknown\n", name);
					gre_exit(1);
				} else {
					strcpy(var_buf, "-1");
				}
			} else {
				extern vector<var> vector_vars;
				sprintf(var_buf, "%d", vector_vars[which].get_end_index());
			}
			unsigned int l = strlen(var_buf);
			for (n = 0; n < l; n++)
				buf[ii++] = var_buf[n];
			continue;
		}

		/* See if it's a scalar */
		if (!is_vector) {
			if (index < 0) {
				fprintf(stderr, "FATAL ERROR - unknown (negative) index\n");
				gre_exit(1);
			}
			extern vector<var> scalar_vars;
			if (scalar_vars[index].get_at_index(0).is_numeric()) {
				double tmp = scalar_vars[index].get_numeric_scalar();
				sprintf(var_buf, "%.14g", tmp);
				unsigned int l = strlen(var_buf);
				for (n = 0; n < l; n++)
					buf[ii++] = var_buf[n];
				//printf("\t*** NUMERIC %f (%s)\n",tmp,var_buf);
			} else {
				const char *var_buf;
				string var_buf_str;
				scalar_vars[index].get_text_scalar(var_buf_str);
				var_buf = var_buf_str.c_str();
				unsigned int l = strlen(var_buf);
				for (n = 0; n < l; n++)
					buf[ii++] = var_buf[n];
				//printf("\t*** STRING (%s)\n",var_buf);
			}
			continue;
		}

		/* It must be a vector */
		//printf("in interpolate_var_in_string(%s) index=%d\n",s,index);
		if (index != -1) {
			extern vector<var> vector_vars;
			if (vector_vars[index].get_at_index(0).is_numeric()) {
				double tmp = vector_vars[index].get_numeric_vector(the_index);
				sprintf(var_buf, "%.14g", tmp);
				unsigned int l = strlen(var_buf);
				for (n = 0; n < l; n++)
					buf[ii++] = var_buf[n];
			} else {
				const char *var_buf;
				var_buf = vector_vars[index].get_text_vector(the_index);
				unsigned int l = strlen(var_buf);
				for (n = 0; n < l; n++)
					buf[ii++] = var_buf[n];
			}
		}
	} while (s[i] != '\0');
	buf[ii] = '\0';

	static string with_sys;	// save memory allocations
	with_sys = "";
	for (i = 0; i < ii; i++) {
		if (buf[i] == '`' && (i==0 || buf[i-1] != '\\')) {
			static string the_cmd; // save memory allocations
			the_cmd = "";
			//printf("got a backtic at position i=%d, (%s)\n", i,buf+i);
			for (++i; i < ii; i++) {
				if (buf[i] == '`' && (i && buf[i-1] != '\\')) {
					//printf("backtic ends at position i=%d, remaining (%s)\n", i,buf+i);
					break;
				}
				the_cmd += buf[i];
			}
			FILE *pipefile = call_popen(the_cmd.c_str(), "r");
			if (!pipefile) {
				runtime_warning("Cannot send backtic expression to OS\n");
				with_sys += "`"; with_sys += the_cmd; with_sys += "`";
			} else {
				string result;
				result = "";
				int cc;
				while (1) {
					if (EOF == (cc = getc(pipefile)))
						break;
					result += char(cc);
				}
				// Trim trailing newline
				if (result[result.size() - 1] == '\n')
					result.STRING_ERASE(result.size() - 1, 1);
				fclose(pipefile);
				with_sys += result;
			}
			//printf("\nHave sent this to OS (%s)\n", the_cmd.c_str());
		} else {
			with_sys += buf[i];
		}
	}
	//printf("\nwith_sys (%s)\n\n", with_sys.c_str());
	if (with_sys.size() < BUFLEN) 
		strcpy(buf, with_sys.c_str());
	//printf("RETURNING '%s'\n", buf);
	return buf;
}

void
push_cmdfile(char *name, FILE* fp)
{
#if 0                           // debugging
    printf("push_cmdfile(%s,%d)\n",name,int(fp));
    printf("  cmdfile_name,cmdfile_line,cmdfile_line_position,cmdfile_parse_where,at_eof: '%s':%d:%d:%d at_eof=%d\n",cmdfile_name,cmdfile_line,cmdfile_line_position,cmdfile_parse_where,at_eof);
#endif
    var v("0", name);
    if (pattern_match.size() == 0)
        pattern_match.push_back(v);
    else
        pattern_match[0] = v;

    // Save existing state (if one exists)
    if (cmdfile_fp != NULL && !at_eof) {
        cmdfile_fp_save.push_back(cmdfile_fp);
        cmdfile_name_save.push_back(cmdfile_name);
        cmdfile_line_save.push_back(cmdfile_line);
        cmdfile_line_position_save.push_back(cmdfile_line_position);
        cmdfile_parse_where_save.push_back(cmdfile_parse_where);
    }
    line_buffer.assign("");           // forces next_char() to read a line

#if 0                           // debugging
    for (unsigned int i = 0; i < cmdfile_name_save.size(); i++) {
        if (i==0) printf("    AT EXIT, STACK IS:\n");
        printf("      %d -- '%s':%d:%d:%d\n",i,cmdfile_name_save[i],cmdfile_line_save[i],cmdfile_line_position_save[i],cmdfile_parse_where_save[i]);
    }
#endif

    // yylval.cval = "";           // clear token ????

    cmdfile_fp = fp;
    cmdfile_name = name;
    cmdfile_line = 1;
    cmdfile_line_position = 0;
    cmdfile_parse_where = 0;
}

int
pop_cmdfile()
{
#if 0                           // debugging
    printf("\npop_cmdfile()\n");
    for (unsigned int i = 0; i < cmdfile_name_save.size(); i++) {
        if (i==0) printf("  STACK:\n");
        printf("    %d -- '%s':%d:%d:%d\n",i,cmdfile_name_save[i],cmdfile_line_save[i],cmdfile_line_position_save[i],cmdfile_parse_where_save[i]);
    }
#endif
    unsigned int stacksize = cmdfile_name_save.size();

    if (stacksize > 0) {
        cmdfile_fp = cmdfile_fp_save.back();
        cmdfile_name = cmdfile_name_save.back();
        cmdfile_line = cmdfile_line_save.back();
        cmdfile_line_position = cmdfile_line_position_save.back();
        cmdfile_parse_where = cmdfile_parse_where_save.back();

        cmdfile_fp_save.pop_back();
        cmdfile_name_save.pop_back();
        cmdfile_line_save.pop_back();
        cmdfile_line_position_save.pop_back();
        cmdfile_parse_where_save.pop_back();

        //printf("pop_cmdfile_name: set to %s:%d:%d:%d  fp %d\n", cmdfile_name,cmdfile_line,cmdfile_line_position,cmdfile_parse_where,int(cmdfile_fp));

    }
    // Make sure it knows that we're to start a new line.
    cmdfile_parse_where = 0;
    line_buffer.assign("");           // ???????
    yylval.cval = "";           // ????????
    //printf("  pop_cmdfile yylval='%s'\n", yylval.cval);
    //printf("pop_cmdfile RETURNING (%d) %d\n", stacksize,cmdfile_name_save.size());
    return stacksize;
}

int
create_a_new_cmd(wnode* w, char* p, cmdnode *a)
{
    extern vector<newcommand> cmd_stack;
    for (unsigned int i = 0; i < cmd_stack.size(); i++) {
	wnode *this_w = w;
	wnode *cmd_w = cmd_stack[i].get_syntax();
	bool match = true;
	while (cmd_w && this_w) {
	    if (strNE(this_w->w, cmd_w->w)) {
		match = false;
		break;
	    }
	    cmd_w = cmd_w->next;
	    this_w = this_w->next;
	}
	if (match && cmd_w == NULL && this_w == NULL) {
	    // Require that prototype match
	    if (strNE(cmd_stack[i].get_prototype(), p)) {
		string msg("prototype doesn't match existing, for cmd `");
		this_w = w;
		while (this_w) { 
		    msg += this_w->w;
		    msg += " ";
		    this_w = this_w->next;
		}
		msg += "' with new prototype";
		yyerror(msg.c_str());
		return -1;
	    }
	    // If action given, require that pre-existing action is empty
	    if (a != NULL && cmd_stack[i].empty_action() == false) {
		string msg("cannot redefine existing cmd `");
		this_w = w;
		while (this_w) {
		    msg += this_w->w;
		    msg += " ";
		    this_w = this_w->next;
		}
		msg.replace(-1 + msg.size(), 1, "'");
		yyerror(msg.c_str());
		return -1;
	    }
	    //printf("debug: create_a_new_cmd() saving action to known cmd number %d\n", i);
	    cmd_stack[i].set_action(a);
	    return i;
	}
    }
    //printf("debug: create_a_new_cmd() saving action to new cmd number %d\n", cmd_stack.size());
    newcommand *nc = new newcommand(w, p, a);
    cmd_stack.push_back(*nc);
    cmd_may_match.push_back(true); // dummy value
    delete nc;
    //printf("MADE new cmd, index of %d\n", (cmd_stack.size() - 1)); 
    return (cmd_stack.size() - 1);
}
