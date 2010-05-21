/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     ARRAY = 259,
     PATTERN_VARIABLE = 260,
     VARIABLE = 261,
     VARIABLE_END = 262,
     COLUMN_IDENT = 263,
     ASSIGNOP = 264,
     END_MATCH_STRING = 265,
     BOOL = 266,
     CMD = 267,
     DIE = 268,
     EXIT = 269,
     WARN = 270,
     DO = 271,
     ELSE = 272,
     EXECUTE = 273,
     FOR = 274,
     FOREACH = 275,
     FILE_TEST = 276,
     GLOB = 277,
     IF = 278,
     INDEX = 279,
     LAST = 280,
     NEXT = 281,
     RETURN = 282,
     CHOMP = 283,
     CHOP = 284,
     READ_GRID = 285,
     COLUMNS = 286,
     MY = 287,
     GREPACKAGE = 288,
     USE = 289,
     SKIP = 290,
     SPLIT = 291,
     STRING_EQ = 292,
     STRING_NE = 293,
     SUB = 294,
     SUBSTR = 295,
     SPRINTF = 296,
     SYSTEM = 297,
     UNLESS = 298,
     UNLINK = 299,
     WHILE = 300,
     UNTIL = 301,
     PRINT = 302,
     PRINTF = 303,
     OPEN = 304,
     REWIND = 305,
     CLOSE = 306,
     QUERY = 307,
     READ = 308,
     DOT = 309,
     EQ = 310,
     LE = 311,
     GE = 312,
     NOT = 313,
     AND = 314,
     OR = 315,
     WORD = 316,
     CMDWORD = 317,
     PROTOTYPE = 318,
     FUNCTION_BUILTIN_NUMERIC = 319,
     FUNCTION_BUILTIN_TEXT = 320,
     FUNCTION_USER = 321,
     STRING = 322,
     MATCH_STRING = 323,
     S_FROM = 324,
     S_TO = 325,
     NE = 326,
     MULOP = 327,
     UNARY = 328,
     POWOP = 329,
     POSTDEC = 330,
     POSTINC = 331
   };
#endif
/* Tokens.  */
#define NUM 258
#define ARRAY 259
#define PATTERN_VARIABLE 260
#define VARIABLE 261
#define VARIABLE_END 262
#define COLUMN_IDENT 263
#define ASSIGNOP 264
#define END_MATCH_STRING 265
#define BOOL 266
#define CMD 267
#define DIE 268
#define EXIT 269
#define WARN 270
#define DO 271
#define ELSE 272
#define EXECUTE 273
#define FOR 274
#define FOREACH 275
#define FILE_TEST 276
#define GLOB 277
#define IF 278
#define INDEX 279
#define LAST 280
#define NEXT 281
#define RETURN 282
#define CHOMP 283
#define CHOP 284
#define READ_GRID 285
#define COLUMNS 286
#define MY 287
#define GREPACKAGE 288
#define USE 289
#define SKIP 290
#define SPLIT 291
#define STRING_EQ 292
#define STRING_NE 293
#define SUB 294
#define SUBSTR 295
#define SPRINTF 296
#define SYSTEM 297
#define UNLESS 298
#define UNLINK 299
#define WHILE 300
#define UNTIL 301
#define PRINT 302
#define PRINTF 303
#define OPEN 304
#define REWIND 305
#define CLOSE 306
#define QUERY 307
#define READ 308
#define DOT 309
#define EQ 310
#define LE 311
#define GE 312
#define NOT 313
#define AND 314
#define OR 315
#define WORD 316
#define CMDWORD 317
#define PROTOTYPE 318
#define FUNCTION_BUILTIN_NUMERIC 319
#define FUNCTION_BUILTIN_TEXT 320
#define FUNCTION_USER 321
#define STRING 322
#define MATCH_STRING 323
#define S_FROM 324
#define S_TO 325
#define NE 326
#define MULOP 327
#define UNARY 328
#define POWOP 329
#define POSTDEC 330
#define POSTINC 331




/* Copy the first part of user declarations.  */
#line 2 "parser.y"


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
    var *v = new var(n, val);
    scalar_vars.push_back(*v);
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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 1776 "parser.y"
{
    double   dval;		/* number*/
    char    *cval;		/* character string*/
    enode   *eval;		/* expression*/
    wnode   *wval;		/* word*/
    cmdnode *cmdval;		/* command*/
}
/* Line 193 of yacc.c.  */
#line 2029 "parser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 2042 "parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  165
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2509

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  93
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  196
/* YYNRULES -- Number of states.  */
#define YYNSTATES  372

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   331

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    80,     2,     2,
      86,    87,     2,    77,    71,    78,     2,    79,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,    90,
      74,     2,    75,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    91,     2,    92,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    88,     2,    89,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    76,    81,    82,    83,
      84,    85
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    15,    18,    21,
      24,    27,    30,    32,    34,    36,    38,    40,    42,    44,
      46,    48,    50,    52,    54,    56,    58,    60,    62,    64,
      66,    68,    70,    72,    74,    76,    78,    80,    83,    85,
      88,    90,    93,   100,   102,   106,   108,   111,   114,   117,
     122,   125,   130,   133,   136,   139,   143,   146,   149,   151,
     154,   158,   161,   165,   172,   174,   177,   181,   185,   187,
     191,   196,   201,   207,   211,   216,   221,   227,   231,   234,
     236,   240,   242,   244,   246,   248,   250,   252,   258,   262,
     268,   272,   276,   278,   280,   284,   288,   292,   296,   297,
     302,   307,   313,   319,   329,   336,   343,   347,   354,   361,
     367,   369,   372,   374,   377,   379,   384,   390,   391,   393,
     395,   398,   399,   404,   406,   409,   414,   416,   419,   424,
     426,   429,   432,   437,   444,   453,   456,   459,   463,   466,
     471,   480,   487,   492,   496,   498,   501,   506,   509,   512,
     519,   521,   524,   529,   531,   533,   535,   541,   545,   550,
     555,   557,   561,   565,   569,   573,   577,   581,   585,   589,
     593,   597,   601,   607,   611,   615,   619,   623,   627,   631,
     634,   638,   642,   647,   651,   656,   661,   668,   671,   674,
     677,   680,   686,   692,   694,   696,   698
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      94,     0,    -1,    95,    -1,    96,    -1,    95,    96,    -1,
      95,     1,    -1,   144,    -1,    97,   144,    -1,   124,   144,
      -1,   125,   144,    -1,   126,   144,    -1,   127,   144,    -1,
     120,    -1,   119,    -1,   135,    -1,   136,    -1,    98,    -1,
     101,    -1,   139,    -1,   100,    -1,    99,    -1,   104,    -1,
     107,    -1,   108,    -1,   110,    -1,   111,    -1,   112,    -1,
     116,    -1,   115,    -1,   117,    -1,   118,    -1,   109,    -1,
     137,    -1,   141,    -1,    25,    -1,    26,    -1,    27,    -1,
      27,   141,    -1,    61,    -1,   102,    61,    -1,    62,    -1,
     103,    62,    -1,    18,    67,    11,    67,    86,   143,    -1,
       6,    -1,   105,    71,     6,    -1,     6,    -1,   106,     6,
      -1,    32,     6,    -1,    32,   105,    -1,    32,    86,   105,
      87,    -1,    32,     4,    -1,    32,     6,     9,   141,    -1,
      33,    62,    -1,    34,    62,    -1,    47,   145,    -1,    47,
      62,   145,    -1,    47,    31,    -1,    47,   103,    -1,    47,
      -1,    48,   145,    -1,    48,    62,   145,    -1,    52,     6,
      -1,    52,     6,    67,    -1,    52,     6,    67,    86,   145,
     143,    -1,   114,    -1,   113,   114,    -1,    61,     8,   141,
      -1,    67,     8,   141,    -1,    61,    -1,    53,    31,   113,
      -1,    53,    31,   113,   141,    -1,    53,    31,    61,   113,
      -1,    53,    31,    61,   113,   141,    -1,    53,    30,    61,
      -1,    53,    30,    61,   138,    -1,    53,    30,    61,   102,
      -1,    53,    30,    61,   102,   138,    -1,    53,    61,   106,
      -1,    53,   106,    -1,    36,    -1,     4,     9,    36,    -1,
     131,    -1,   132,    -1,   129,    -1,   130,    -1,   133,    -1,
     134,    -1,    23,   141,   121,    17,   121,    -1,    23,   141,
     121,    -1,    43,   141,   121,    17,   121,    -1,    43,   141,
     121,    -1,   122,    95,   123,    -1,    88,    -1,    89,    -1,
      97,    23,   141,    -1,    97,    43,   141,    -1,    97,    45,
     141,    -1,    97,    46,   141,    -1,    -1,    45,   128,   140,
     121,    -1,    46,   128,   140,   121,    -1,    16,   121,    45,
     140,   144,    -1,    16,   121,    46,   140,   144,    -1,    19,
      86,   141,    90,   140,    90,   141,   143,   121,    -1,    20,
       6,    86,   145,   143,   121,    -1,    20,     6,    86,     4,
     143,   121,    -1,    39,   102,   121,    -1,    39,   102,    86,
      63,   143,   121,    -1,    12,   102,    86,    63,   143,   121,
      -1,    12,   102,    86,    63,   143,    -1,   103,    -1,   103,
     138,    -1,   141,    -1,   138,   141,    -1,    35,    -1,    35,
      86,    61,    87,    -1,    35,    86,    61,   141,    87,    -1,
      -1,   141,    -1,     3,    -1,    69,    70,    -1,    -1,    79,
     142,    68,    10,    -1,    29,    -1,    29,     6,    -1,    29,
      86,     6,    87,    -1,    28,    -1,    28,     6,    -1,    28,
      86,     6,    87,    -1,    13,    -1,    13,   141,    -1,    14,
     141,    -1,    22,    86,   141,    87,    -1,    24,    86,   141,
      71,   141,    87,    -1,    24,    86,   141,    71,   141,    71,
     141,    87,    -1,    15,   141,    -1,    74,    75,    -1,    74,
     102,    75,    -1,    21,   141,    -1,    42,    86,   141,    87,
      -1,    40,    86,   141,    71,   141,    71,   141,    87,    -1,
      40,    86,   141,    71,   141,    87,    -1,    41,    86,   145,
      87,    -1,    50,    86,    87,    -1,    50,    -1,    50,    61,
      -1,    50,    86,    61,    87,    -1,    49,    61,    -1,    49,
     141,    -1,    49,    86,    61,    71,   141,    87,    -1,    51,
      -1,    51,    61,    -1,    51,    86,    61,    87,    -1,     6,
      -1,     7,    -1,     5,    -1,     4,     9,    86,   145,    87,
      -1,   141,     9,   141,    -1,    44,    86,   145,    87,    -1,
       6,    91,   141,    92,    -1,    67,    -1,   141,    54,   141,
      -1,   141,    37,   141,    -1,   141,    38,   141,    -1,   141,
      74,   141,    -1,   141,    56,   141,    -1,   141,    75,   141,
      -1,   141,    55,   141,    -1,   141,    76,   141,    -1,   141,
      57,   141,    -1,   141,    60,   141,    -1,   141,    59,   141,
      -1,   141,    72,   141,    73,   141,    -1,   141,    77,   141,
      -1,   141,    78,   141,    -1,   141,    81,   141,    -1,   141,
      79,   141,    -1,   141,    80,   141,    -1,   141,    83,   141,
      -1,    58,   141,    -1,    86,   141,   143,    -1,    66,    86,
      87,    -1,    66,    86,   145,    87,    -1,    64,    86,    87,
      -1,    64,    86,   145,    87,    -1,    65,    86,   141,    87,
      -1,    65,    86,   141,    71,   141,    87,    -1,    78,   141,
      -1,    77,   141,    -1,     6,    84,    -1,     6,    85,    -1,
       6,    91,   141,    92,    84,    -1,     6,    91,   141,    92,
      85,    -1,    87,    -1,    90,    -1,   141,    -1,   145,    71,
     141,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1923,  1923,  1928,  1929,  1933,  1938,  1939,  2008,  2009,
    2010,  2011,  2012,  2013,  2014,  2015,  2019,  2026,  2032,  2038,
    2044,  2050,  2056,  2062,  2068,  2074,  2080,  2086,  2092,  2098,
    2104,  2110,  2116,  2126,  2130,  2136,  2142,  2145,  2150,  2151,
    2155,  2163,  2174,  2207,  2210,  2216,  2219,  2225,  2231,  2241,
    2251,  2257,  2274,  2282,  2291,  2295,  2299,  2303,  2324,  2335,
    2339,  2346,  2355,  2365,  2378,  2379,  2384,  2389,  2394,  2403,
    2406,  2409,  2412,  2422,  2426,  2430,  2436,  2445,  2449,  2456,
    2461,  2486,  2487,  2488,  2489,  2490,  2491,  2496,  2500,  2504,
    2508,  2515,  2520,  2527,  2533,  2539,  2545,  2551,  2557,  2560,
    2566,  2572,  2578,  2584,  2591,  2596,  2604,  2615,  2629,  2637,
    2648,  2652,  2659,  2660,  2664,  2670,  2676,  2684,  2685,  2689,
    2690,  2698,  2698,  2705,  2709,  2713,  2717,  2721,  2725,  2729,
    2733,  2737,  2743,  2747,  2751,  2755,  2759,  2763,  2769,  2773,
    2777,  2781,  2785,  2790,  2794,  2798,  2802,  2806,  2810,  2814,
    2837,  2838,  2839,  2840,  2841,  2842,  2843,  2866,  2884,  2888,
    2891,  2892,  2905,  2906,  2908,  2909,  2910,  2911,  2912,  2913,
    2914,  2915,  2916,  2920,  2929,  2938,  2947,  2956,  2965,  2974,
    2975,  2976,  2982,  2988,  2994,  3000,  3008,  3017,  3018,  3019,
    3020,  3021,  3024,  3030,  3034,  3039,  3040
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "ARRAY", "PATTERN_VARIABLE",
  "VARIABLE", "VARIABLE_END", "COLUMN_IDENT", "ASSIGNOP",
  "END_MATCH_STRING", "BOOL", "CMD", "DIE", "EXIT", "WARN", "DO", "ELSE",
  "EXECUTE", "FOR", "FOREACH", "FILE_TEST", "GLOB", "IF", "INDEX", "LAST",
  "NEXT", "RETURN", "CHOMP", "CHOP", "READ_GRID", "COLUMNS", "MY",
  "GREPACKAGE", "USE", "SKIP", "SPLIT", "STRING_EQ", "STRING_NE", "SUB",
  "SUBSTR", "SPRINTF", "SYSTEM", "UNLESS", "UNLINK", "WHILE", "UNTIL",
  "PRINT", "PRINTF", "OPEN", "REWIND", "CLOSE", "QUERY", "READ", "DOT",
  "EQ", "LE", "GE", "NOT", "AND", "OR", "WORD", "CMDWORD", "PROTOTYPE",
  "FUNCTION_BUILTIN_NUMERIC", "FUNCTION_BUILTIN_TEXT", "FUNCTION_USER",
  "STRING", "MATCH_STRING", "S_FROM", "S_TO", "','", "'?'", "':'", "'<'",
  "'>'", "NE", "'+'", "'-'", "'/'", "'%'", "MULOP", "UNARY", "POWOP",
  "POSTDEC", "POSTINC", "'('", "')'", "'{'", "'}'", "';'", "'['", "']'",
  "$accept", "start", "cmds", "cmd", "single_cmd", "expr_cmd", "last_cmd",
  "next_cmd", "return_cmd", "words", "cmd_words", "C_command",
  "var_list_comma", "var_list_nocomma", "my_var_list", "package", "use",
  "print", "printf", "query", "read_columns_spec_list",
  "read_columns_spec", "read_columns", "read_grid", "read_var_list",
  "split", "loop", "if_block", "block", "start_of_block", "end_of_block",
  "trailing_if", "trailing_unless", "trailing_while", "trailing_until",
  "while_context", "while_block", "until_block", "do_while_block",
  "do_until_block", "for_block", "foreach_block", "define_sub",
  "define_cmd", "a_user_cmd", "listexpr_nocomma", "skip_cmd", "texpr",
  "expr", "@1", "r_paren", "SC", "listexpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,    44,    63,    58,    60,    62,   326,    43,    45,    47,
      37,   327,   328,   329,   330,   331,    40,    41,   123,   125,
      59,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    93,    94,    95,    95,    95,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    98,    99,   100,   101,   101,   102,   102,
     103,   103,   104,   105,   105,   106,   106,   107,   107,   107,
     107,   107,   108,   109,   110,   110,   110,   110,   110,   111,
     111,   112,   112,   112,   113,   113,   114,   114,   114,   115,
     115,   115,   115,   116,   116,   116,   116,   117,   117,   118,
     118,   119,   119,   119,   119,   119,   119,   120,   120,   120,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   134,   135,   135,   136,   136,
     137,   137,   138,   138,   139,   139,   139,   140,   140,   141,
     141,   142,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   143,   144,   145,   145
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     2,     6,     1,     3,     1,     2,     2,     2,     4,
       2,     4,     2,     2,     2,     3,     2,     2,     1,     2,
       3,     2,     3,     6,     1,     2,     3,     3,     1,     3,
       4,     4,     5,     3,     4,     4,     5,     3,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     5,     3,     5,
       3,     3,     1,     1,     3,     3,     3,     3,     0,     4,
       4,     5,     5,     9,     6,     6,     3,     6,     6,     5,
       1,     2,     1,     2,     1,     4,     5,     0,     1,     1,
       2,     0,     4,     1,     2,     4,     1,     2,     4,     1,
       2,     2,     4,     6,     8,     2,     2,     3,     2,     4,
       8,     6,     4,     3,     1,     2,     4,     2,     2,     6,
       1,     2,     4,     1,     1,     1,     5,     3,     4,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     5,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     4,     3,     4,     4,     6,     2,     2,     2,
       2,     5,     5,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   119,     0,   155,   153,   154,     0,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    35,
      36,   126,   123,     0,     0,     0,   114,    79,     0,     0,
       0,     0,     0,     0,    98,    98,    58,     0,     0,   144,
     150,     0,     0,     0,    40,     0,     0,     0,   160,     0,
       0,     0,     0,   121,     0,   194,     0,     0,     3,     0,
      16,    20,    19,    17,   110,    21,    22,    23,    31,    24,
      25,    26,    28,    27,    29,    30,    13,    12,     0,     0,
       0,     0,    83,    84,    81,    82,    85,    86,    14,    15,
      32,    18,    33,     6,     0,   189,   190,     0,    38,     0,
       0,   130,   131,   135,    92,     0,     0,     0,     0,     0,
     138,     0,     0,     0,    37,   127,     0,   124,     0,    50,
      43,     0,    48,    52,    53,     0,     0,     0,     0,     0,
       0,     0,   117,   117,    56,    40,    57,   195,    54,     0,
      59,   147,     0,   148,   145,     0,   151,     0,    61,    45,
       0,     0,     0,    78,   179,     0,     0,     0,   120,   136,
       0,   188,   187,     0,     0,     1,     5,     4,     0,     0,
       0,     0,     7,    41,   111,   112,     8,     9,    10,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,     0,    39,     0,     0,   117,   117,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,    43,     0,     0,
       0,     0,   106,     0,     0,     0,    90,     0,     0,   118,
       0,    55,     0,    60,     0,     0,   143,     0,    62,    73,
      68,     0,    69,    64,    77,    46,   183,     0,     0,   181,
       0,   137,     0,   193,   180,    94,    95,    96,    97,   113,
     157,   162,   163,   161,   167,   165,   169,   171,   170,     0,
     164,   166,   168,   173,   174,   176,   177,   175,   178,     0,
     159,     0,     0,     0,    93,    91,     0,   117,     0,     0,
     132,     0,     0,   128,   125,    51,    49,    44,   115,     0,
       0,     0,   142,   139,     0,   158,    99,   100,   196,     0,
     146,   152,     0,    75,    74,     0,    68,    71,     0,   160,
      65,    70,   184,     0,   185,   182,   122,     0,   156,   191,
     192,   109,   101,   102,     0,     0,     0,     0,    87,     0,
     116,     0,     0,    89,     0,     0,    76,    66,    72,    67,
       0,   172,   108,    42,     0,   105,   104,     0,   133,   107,
       0,   141,   149,    63,   186,     0,     0,     0,     0,   134,
     140,   103
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    56,    57,    58,    59,    60,    61,    62,    63,    99,
      64,    65,   122,   153,    66,    67,    68,    69,    70,    71,
     242,   243,    72,    73,    74,    75,    76,    77,   105,   106,
     285,    78,    79,    80,    81,   132,    82,    83,    84,    85,
      86,    87,    88,    89,    90,   174,    91,   228,   137,   163,
     254,    93,   138
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -248
static const yytype_int16 yypact[] =
{
     534,  -248,    26,  -248,    32,  -248,   -13,  1459,  1459,  1459,
     -27,     1,   -20,    64,  1459,   -11,  1459,    33,  -248,  -248,
    1459,    -1,     5,     6,    16,    18,    35,  -248,   -13,    45,
      51,    53,  1459,    55,  -248,  -248,   843,   920,   997,   -24,
      21,    84,    15,  1459,  -248,    57,    58,    59,  -248,    66,
     -31,  1459,  1459,  -248,  1459,  -248,   146,   358,  -248,     4,
    -248,  -248,  -248,  -248,  1074,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,    60,    60,
      60,    60,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  2306,  -248,   -17,  -248,  -248,  1459,  -248,    23,
     138,  2366,  2366,  2366,  -248,    79,   534,   137,  1459,    63,
      68,  1459,  1614,  1459,  2306,  -248,   148,  -248,   151,  -248,
     143,   152,    88,  -248,  -248,    99,   -30,  1459,  1459,  1459,
    1614,  1459,  1459,  1459,  -248,  1459,   100,  2306,    90,  1459,
      90,  -248,  1151,  2366,  -248,   -32,  -248,   102,    97,  -248,
     105,   -28,   161,   166,    68,   612,  1459,   689,  -248,  -248,
      11,    68,    68,   108,  1800,  -248,  -248,  -248,  1459,  1459,
    1459,  1459,  -248,  -248,  1459,  2306,  -248,  -248,  -248,  -248,
    1459,  1459,  1459,  1459,  1459,  1459,  1459,  1459,  1459,  1459,
    1459,  1459,  1459,  1459,  1459,  1459,  1459,  1459,  1459,  -248,
    1459,  1552,  -248,   110,    91,  1459,  1459,   446,   111,  1582,
    1536,  1852,   162,  2216,   112,   113,  1459,  -248,    -8,   196,
     766,   140,  -248,  2246,    -6,  1904,   187,    -4,   -27,  2306,
     -27,    90,  1459,    90,   139,   122,  -248,   124,   126,  1228,
      10,   205,  1305,  -248,   166,  -248,  -248,    28,  1644,  -248,
      39,  -248,   208,  -248,  -248,  2306,  2306,  2306,  2306,   199,
    2366,    68,    68,    22,  2426,  2426,  2426,  2366,  2336,  2276,
    2426,  2426,  2426,    22,    22,    68,    68,    68,    68,    41,
      50,   128,    60,    60,  -248,  -248,   133,  1459,     8,    43,
    -248,   -27,  1459,  -248,  -248,  2306,  -248,  -248,  -248,  1956,
     128,  1459,  -248,  -248,   -27,  -248,  -248,  -248,  2306,  1459,
    -248,  -248,  1459,  1382,  1459,  1459,   213,  1305,  1459,   205,
    -248,  2306,  -248,  1459,  -248,  -248,  -248,  1459,  -248,  -248,
    -248,   -27,  -248,  -248,   128,   132,   -27,   -27,  -248,  1696,
    -248,   -27,  1748,  -248,  2008,    43,  1459,  2306,  2306,  2306,
    2060,  2396,  -248,  -248,  1459,  -248,  -248,  1459,  -248,  -248,
    1459,  -248,  -248,  -248,  -248,  1800,  2112,  2164,   -27,  -248,
    -248,  -248
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -248,  -248,   117,   -54,  -248,  -248,  -248,  -248,  -248,   -22,
     188,  -248,   104,    74,  -248,  -248,  -248,  -248,  -248,  -248,
     -10,  -229,  -248,  -248,  -248,  -248,  -248,  -248,   -90,  -248,
    -248,  -248,  -248,  -248,  -248,   194,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -224,  -248,  -132,     0,  -248,
    -247,   -55,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -3
static const yytype_int16 yytable[] =
{
      92,   230,   140,   167,   172,   115,   126,   101,   102,   103,
     119,   117,   120,   320,   110,   314,   112,   204,   315,   199,
     114,   149,   212,   176,   177,   178,   179,   168,   160,   235,
      98,   202,   130,   240,   331,    94,   222,   144,   143,   241,
     226,   336,   337,   154,   159,   150,   151,   169,    98,   170,
     171,   161,   162,   341,   164,   236,   221,    92,   104,   181,
     182,   104,   145,   219,   175,   232,   108,   232,   107,   200,
     109,   316,   202,   282,   283,   111,   152,   241,   123,   296,
     124,   302,   146,   305,   202,   116,   251,   353,   320,   346,
     148,   118,   121,   224,    55,   253,   227,   201,   363,   232,
     231,   195,   196,   197,   233,   198,    92,   147,   209,   203,
     232,   211,   232,   213,   232,   322,    95,    96,   368,   113,
     247,   125,   250,    97,   205,   206,   325,   223,   328,   225,
     253,   127,   229,   229,   329,   330,   158,   128,   306,   129,
     307,   131,   164,   155,   156,   157,   165,   204,   208,   210,
      55,   198,   216,   167,   214,   335,   248,   215,   217,   219,
     220,   232,   173,   237,   238,   279,   239,   149,   255,   256,
     257,   258,   245,   281,   259,   289,   252,   200,   286,   291,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   293,
     294,   338,   297,   300,   304,   229,   229,    92,   180,   310,
     309,   311,   312,   318,   343,   253,   295,   313,   326,   334,
     299,   315,   354,   207,   136,   218,   244,   332,   333,   133,
     317,     0,   308,     0,     0,     0,   181,   182,     0,   175,
       0,   352,   321,     0,     0,     0,   355,   356,     0,     0,
       0,   359,     0,   183,   184,   185,   186,     0,   187,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,     0,   191,   192,     0,   345,   371,   196,
     197,     0,   198,     0,     0,     0,     0,   229,     0,     0,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,   344,
       0,     0,     0,   175,   259,   347,     0,   348,   349,     0,
       0,     0,     0,   350,     0,     0,     0,   351,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,   365,     0,     0,   366,    -2,   166,
     367,     1,     2,     3,     4,     5,     0,     0,     0,     0,
       6,     7,     8,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     0,     0,
      23,    24,    25,    26,    27,     0,     0,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,    43,     0,     0,     0,
      44,     0,    45,    46,    47,    48,     0,    49,     0,     0,
       0,     0,    50,     0,     0,    51,    52,    53,     0,     0,
       0,     0,     0,     0,    54,     0,     0,   166,    55,     1,
       2,     3,     4,     5,     0,     0,     0,     0,     6,     7,
       8,     9,    10,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,     0,    23,    24,
      25,    26,    27,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,    43,     0,     0,     0,    44,     0,
      45,    46,    47,    48,     0,    49,     0,     0,     0,     0,
      50,     0,     0,    51,    52,    53,     0,     0,     0,     0,
       0,     0,    54,     0,     0,   284,    55,     1,     2,     3,
       4,     5,     0,     0,     0,     0,     6,     7,     8,     9,
      10,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     0,     0,    23,    24,    25,    26,
      27,     0,     0,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,     0,     0,
       0,     0,    43,     0,     0,     0,    44,     0,    45,    46,
      47,    48,     0,    49,     0,     0,     0,     0,    50,     0,
       0,    51,    52,    53,     0,     1,   100,     3,     4,     5,
      54,     0,     0,     0,    55,     7,     8,     9,     0,     0,
       0,     0,     0,    14,    15,     0,    17,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     0,    33,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,    45,    46,    47,    48,
       0,    49,     0,     0,     0,     0,    50,     0,     0,    51,
      52,    53,     1,   100,     3,     4,     5,     0,    54,   246,
       0,     0,     7,     8,     9,     0,     0,     0,     0,     0,
      14,    15,     0,    17,     0,     0,     0,    21,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,    33,     0,     0,     0,     0,    38,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,    49,     0,
       0,     0,     0,    50,     0,     0,    51,    52,    53,     1,
     100,     3,     4,     5,     0,    54,   249,     0,     0,     7,
       8,     9,     0,     0,     0,     0,     0,    14,    15,     0,
      17,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    31,     0,
      33,     0,     0,     0,     0,    38,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,    49,     0,     0,     0,     0,
      50,     0,     0,    51,    52,    53,     1,   100,     3,     4,
       5,     0,    54,   298,     0,     0,     7,     8,     9,     0,
       0,     0,     0,     0,    14,    15,     0,    17,     0,     0,
       0,    21,    22,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,     0,    33,     0,     0,
       0,     0,    38,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,     0,     0,   135,     0,    45,    46,    47,
      48,     0,    49,     0,     0,     0,     0,    50,     0,     0,
      51,    52,    53,     1,   100,     3,     4,     5,     0,    54,
       0,     0,     0,     7,     8,     9,     0,     0,     0,     0,
       0,    14,    15,     0,    17,     0,     0,     0,    21,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31,     0,    33,     0,     0,     0,     0,    38,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,   139,     0,    45,    46,    47,    48,     0,    49,
       0,     0,     0,     0,    50,     0,     0,    51,    52,    53,
       1,   100,     3,     4,     5,     0,    54,     0,     0,     0,
       7,     8,     9,     0,     0,     0,     0,     0,    14,    15,
       0,    17,     0,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
       0,    33,     0,     0,     0,     0,    38,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,     0,   141,     0,
       0,    45,    46,    47,    48,     0,    49,     0,     0,     0,
       0,    50,     0,     0,    51,    52,    53,     1,   100,     3,
       4,     5,     0,   142,     0,     0,     0,     7,     8,     9,
       0,     0,     0,     0,     0,    14,    15,     0,    17,     0,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,    31,     0,    33,     0,
       0,     0,     0,    38,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,   173,     0,    45,    46,
      47,    48,     0,    49,     0,     0,     0,     0,    50,     0,
       0,    51,    52,    53,     1,   100,     3,     4,     5,     0,
      54,     0,     0,     0,     7,     8,     9,     0,     0,     0,
       0,     0,    14,    15,     0,    17,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    29,    30,    31,     0,    33,     0,     0,     0,     0,
      38,    39,    40,     0,     0,     0,     0,     0,     0,    43,
       0,     0,   234,     0,     0,    45,    46,    47,    48,     0,
      49,     0,     0,     0,     0,    50,     0,     0,    51,    52,
      53,     1,   100,     3,     4,     5,     0,    54,     0,     0,
       0,     7,     8,     9,     0,     0,     0,     0,     0,    14,
      15,     0,    17,     0,     0,     0,    21,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    29,    30,
      31,     0,    33,     0,     0,     0,     0,    38,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    98,
       0,     0,    45,    46,    47,    48,     0,    49,     0,     0,
       0,     0,    50,     0,     0,    51,    52,    53,     1,   100,
       3,     4,     5,     0,    54,     0,     0,     0,     7,     8,
       9,     0,     0,     0,     0,     0,    14,    15,     0,    17,
       0,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,     0,    33,
       0,     0,     0,     0,    38,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,     0,   316,     0,     0,    45,
      46,    47,   319,     0,    49,     0,     0,     0,     0,    50,
       0,     0,    51,    52,    53,     1,   100,     3,     4,     5,
       0,    54,     0,     0,     0,     7,     8,     9,     0,     0,
       0,     0,     0,    14,    15,     0,    17,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    29,    30,    31,     0,    33,     0,     0,     0,
       0,    38,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,     0,   202,     0,     0,    45,    46,    47,    48,
       0,    49,     0,     0,     0,     0,    50,     0,     0,    51,
      52,    53,     1,   100,     3,     4,     5,     0,    54,     0,
       0,     0,     7,     8,     9,     0,     0,     0,     0,     0,
      14,    15,     0,    17,     0,     0,     0,    21,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,     0,    33,     0,     0,     0,     0,    38,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,    45,    46,    47,    48,     0,    49,     0,
       0,     0,     0,    50,     0,     0,    51,    52,    53,     1,
     288,     3,     4,     5,     0,    54,     0,     0,     0,     7,
       8,     9,     0,     0,     0,     0,     0,    14,    15,     0,
      17,   180,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    31,     0,
      33,     0,     0,     0,     0,    38,    39,    40,     0,   181,
     182,   180,     0,     0,    43,     0,     0,     0,     0,     0,
      45,    46,    47,    48,     0,    49,   183,   184,   185,   186,
      50,   187,   188,    51,    52,    53,     0,     0,     0,   181,
     182,     0,    54,   180,   189,     0,   190,   191,   192,   193,
     194,   195,   196,   197,     0,   198,   183,   184,   185,   186,
       0,   187,   188,     0,   280,     0,     0,     0,     0,     0,
       0,   181,   182,   180,   189,     0,   190,   191,   192,   193,
     194,   195,   196,   197,     0,   198,     0,     0,   183,   184,
     185,   186,   287,   187,   188,     0,     0,     0,     0,     0,
       0,   181,   182,     0,     0,     0,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,     0,   198,   183,   184,
     185,   186,   104,   187,   188,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,     0,   198,     0,     0,
       0,   324,     0,   181,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   184,   185,   186,     0,   187,   188,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   357,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
       0,     0,     0,   358,     0,   181,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   184,   185,   186,     0,   187,   188,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   360,
     189,     0,   190,   191,   192,   193,   194,   195,   196,   197,
       0,   198,     0,     0,     0,   361,     0,   181,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   184,   185,   186,     0,   187,
     188,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,   190,   191,   192,   193,   194,   195,
     196,   197,     0,   198,     0,     0,     0,   253,     0,   181,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   184,   185,   186,
       0,   187,   188,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,   190,   191,   192,   193,
     194,   195,   196,   197,     0,   198,     0,     0,     0,   290,
       0,   181,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,   184,
     185,   186,     0,   187,   188,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,     0,   198,     0,     0,
       0,   303,     0,   181,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   184,   185,   186,     0,   187,   188,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
       0,     0,     0,   340,     0,   181,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   184,   185,   186,     0,   187,   188,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,   190,   191,   192,   193,   194,   195,   196,   197,
       0,   198,     0,     0,     0,   362,     0,   181,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   184,   185,   186,     0,   187,
     188,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,   190,   191,   192,   193,   194,   195,
     196,   197,     0,   198,     0,     0,     0,   364,     0,   181,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   184,   185,   186,
       0,   187,   188,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,   190,   191,   192,   193,
     194,   195,   196,   197,     0,   198,     0,     0,     0,   369,
       0,   181,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,   184,
     185,   186,     0,   187,   188,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,     0,   190,   191,
     192,   193,   194,   195,   196,   197,     0,   198,     0,     0,
       0,   370,     0,   181,   182,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   184,   185,   186,     0,   187,   188,     0,     0,     0,
       0,     0,     0,   181,   182,   180,     0,   292,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,   187,   188,     0,     0,     0,
       0,     0,     0,   181,   182,   180,     0,   301,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,   187,   188,     0,     0,     0,
       0,     0,     0,   181,   182,   180,     0,     0,   189,   327,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,   187,   188,     0,     0,     0,
       0,     0,     0,   181,   182,   180,     0,     0,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,   187,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,   184,   185,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   182,     0,     0,     0,   189,     0,
     190,   191,   192,   193,   194,   195,   196,   197,     0,   198,
     183,    -3,    -3,    -3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      -3,    -3,    -3,   193,   194,   195,   196,   197,     0,   198
};

static const yytype_int16 yycheck[] =
{
       0,   133,    37,    57,    59,     6,    28,     7,     8,     9,
       4,     6,     6,   242,    14,   239,    16,     9,     8,    36,
      20,     6,   112,    78,    79,    80,    81,    23,    50,    61,
      61,    61,    32,    61,   281,     9,   126,    61,    38,    67,
     130,   288,   289,    43,    75,    30,    31,    43,    61,    45,
      46,    51,    52,   300,    54,    87,    86,    57,    88,    37,
      38,    88,    86,    71,    64,    71,    86,    71,    67,    86,
       6,    61,    61,   205,   206,    86,    61,    67,    62,    87,
      62,    87,    61,    87,    61,    86,    75,   334,   317,   313,
       6,    86,    86,   128,    90,    87,   131,    97,   345,    71,
     135,    79,    80,    81,   139,    83,   106,    86,   108,    86,
      71,   111,    71,   113,    71,    87,    84,    85,   365,    86,
     155,    86,   157,    91,    45,    46,    87,   127,    87,   129,
      87,    86,   132,   133,    84,    85,    70,    86,   228,    86,
     230,    86,   142,    86,    86,    86,     0,     9,    11,    86,
      90,    83,     9,   207,     6,   287,   156,     6,     6,    71,
      61,    71,    62,    61,    67,   200,    61,     6,   168,   169,
     170,   171,     6,    63,   174,   210,    68,    86,    67,    17,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,    87,
      87,   291,     6,    63,    17,   205,   206,   207,     9,    87,
      71,    87,    86,     8,   304,    87,   216,   239,    10,    86,
     220,     8,    90,   106,    36,   121,   152,   282,   283,    35,
     240,    -1,   232,    -1,    -1,    -1,    37,    38,    -1,   239,
      -1,   331,   242,    -1,    -1,    -1,   336,   337,    -1,    -1,
      -1,   341,    -1,    54,    55,    56,    57,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    75,    76,    -1,   312,   368,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,   287,    -1,    -1,
      -1,    -1,   292,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,
      -1,    -1,    -1,   313,   314,   315,    -1,   317,   318,    -1,
      -1,    -1,    -1,   323,    -1,    -1,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   346,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,    -1,    -1,   357,     0,     1,
     360,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      62,    -1,    64,    65,    66,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,     1,    90,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    62,    -1,
      64,    65,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    89,    90,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      36,    -1,    -1,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    62,    -1,    64,    65,
      66,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    77,    78,    79,    -1,     3,     4,     5,     6,     7,
      86,    -1,    -1,    -1,    90,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    24,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,
      78,    79,     3,     4,     5,     6,     7,    -1,    86,    87,
      -1,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    24,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    -1,    44,    -1,    -1,    -1,    -1,    49,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,     3,
       4,     5,     6,     7,    -1,    86,    87,    -1,    -1,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      24,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    77,    78,    79,     3,     4,     5,     6,
       7,    -1,    86,    87,    -1,    -1,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    24,    -1,    -1,
      -1,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    62,    -1,    64,    65,    66,
      67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      77,    78,    79,     3,     4,     5,     6,     7,    -1,    86,
      -1,    -1,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    24,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,    49,
      50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    62,    -1,    64,    65,    66,    67,    -1,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,
       3,     4,     5,     6,     7,    -1,    86,    -1,    -1,    -1,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    24,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      -1,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    61,    -1,
      -1,    64,    65,    66,    67,    -1,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    77,    78,    79,     3,     4,     5,
       6,     7,    -1,    86,    -1,    -1,    -1,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    24,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    62,    -1,    64,    65,
      66,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    77,    78,    79,     3,     4,     5,     6,     7,    -1,
      86,    -1,    -1,    -1,    13,    14,    15,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    24,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    61,    -1,    -1,    64,    65,    66,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,
      79,     3,     4,     5,     6,     7,    -1,    86,    -1,    -1,
      -1,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    24,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,
      42,    -1,    44,    -1,    -1,    -1,    -1,    49,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    61,
      -1,    -1,    64,    65,    66,    67,    -1,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,    78,    79,     3,     4,
       5,     6,     7,    -1,    86,    -1,    -1,    -1,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    24,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    61,    -1,    -1,    64,
      65,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    77,    78,    79,     3,     4,     5,     6,     7,
      -1,    86,    -1,    -1,    -1,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    24,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    61,    -1,    -1,    64,    65,    66,    67,
      -1,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,
      78,    79,     3,     4,     5,     6,     7,    -1,    86,    -1,
      -1,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    24,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    -1,    44,    -1,    -1,    -1,    -1,    49,    50,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    -1,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    77,    78,    79,     3,
       4,     5,     6,     7,    -1,    86,    -1,    -1,    -1,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      24,     9,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    37,
      38,     9,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    69,    54,    55,    56,    57,
      74,    59,    60,    77,    78,    79,    -1,    -1,    -1,    37,
      38,    -1,    86,     9,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    54,    55,    56,    57,
      -1,    59,    60,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,     9,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    54,    55,
      56,    57,    90,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    -1,    -1,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    54,    55,
      56,    57,    88,    59,    60,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      -1,    87,    -1,    37,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    59,    60,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    -1,    87,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    59,    60,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    -1,    87,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    59,
      60,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    87,    -1,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    59,    60,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    -1,    87,
      -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    59,    60,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      -1,    87,    -1,    37,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    59,    60,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    -1,    87,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    59,    60,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    -1,    87,    -1,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    59,
      60,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    87,    -1,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    59,    60,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    -1,    87,
      -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    59,    60,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      -1,    87,    -1,    37,    38,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,     9,    -1,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,     9,    -1,    71,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,     9,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,     9,    -1,    -1,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    72,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    12,    13,    14,    15,
      16,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    32,    33,    34,    35,    36,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    58,    62,    64,    65,    66,    67,    69,
      74,    77,    78,    79,    86,    90,    94,    95,    96,    97,
      98,    99,   100,   101,   103,   104,   107,   108,   109,   110,
     111,   112,   115,   116,   117,   118,   119,   120,   124,   125,
     126,   127,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   139,   141,   144,     9,    84,    85,    91,    61,   102,
       4,   141,   141,   141,    88,   121,   122,    67,    86,     6,
     141,    86,   141,    86,   141,     6,    86,     6,    86,     4,
       6,    86,   105,    62,    62,    86,   102,    86,    86,    86,
     141,    86,   128,   128,    31,    62,   103,   141,   145,    62,
     145,    61,    86,   141,    61,    86,    61,    86,     6,     6,
      30,    31,    61,   106,   141,    86,    86,    86,    70,    75,
     102,   141,   141,   142,   141,     0,     1,    96,    23,    43,
      45,    46,   144,    62,   138,   141,   144,   144,   144,   144,
       9,    37,    38,    54,    55,    56,    57,    59,    60,    72,
      74,    75,    76,    77,    78,    79,    80,    81,    83,    36,
      86,   141,    61,    86,     9,    45,    46,    95,    11,   141,
      86,   141,   121,   141,     6,     6,     9,     6,   105,    71,
      61,    86,   121,   141,   145,   141,   121,   145,   140,   141,
     140,   145,    71,   145,    61,    61,    87,    61,    67,    61,
      61,    67,   113,   114,   106,     6,    87,   145,   141,    87,
     145,    75,    68,    87,   143,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   145,
      92,    63,   140,   140,    89,   123,    67,    90,     4,   145,
      87,    17,    71,    87,    87,   141,    87,     6,    87,   141,
      63,    71,    87,    87,    17,    87,   121,   121,   141,    71,
      87,    87,    86,   102,   138,     8,    61,   113,     8,    67,
     114,   141,    87,    71,    87,    87,    10,    73,    87,    84,
      85,   143,   144,   144,    86,   140,   143,   143,   121,   141,
      87,   143,   141,   121,   141,   145,   138,   141,   141,   141,
     141,   141,   121,   143,    90,   121,   121,    71,    87,   121,
      71,    87,    87,   143,    87,   141,   141,   141,   143,    87,
      87,   121
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1923 "parser.y"
    { the_pgm = (yyvsp[(1) - (1)].cmdval); ;}
    break;

  case 3:
#line 1928 "parser.y"
    { (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval); ;}
    break;

  case 4:
#line 1929 "parser.y"
    { 
            (yyval.cmdval) = append_right((yyvsp[(1) - (2)].cmdval), (yyvsp[(2) - (2)].cmdval));
            //printf("'cmds cmd' appended %s (%x)\n",name_of_cmd($2->type),$2);
        ;}
    break;

  case 6:
#line 1938 "parser.y"
    {(yyval.cmdval) = new_cmdnode();;}
    break;

  case 7:
#line 1939 "parser.y"
    {
            if (_gre_debug & DEBUG_TRACE) {
                printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; 
                if ((yyval.cmdval)->type == cmdnode::user_cmd) 
                    printf("# user_cmd, index=%d\n", (yyval.cmdval)->the_cmd_index);
                else
                    printf("# cmd of type %d (%s)\n", (yyval.cmdval)->type, name_of_cmd((yyval.cmdval)->type));
	    }
            if ((yyval.cmdval)->type == cmdnode::use) {
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
	;}
    break;

  case 8:
#line 2008 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `if'\n"); };}
    break;

  case 9:
#line 2009 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `unless'\n"); };}
    break;

  case 10:
#line 2010 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `while'\n"); };}
    break;

  case 11:
#line 2011 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# trailing `until'\n"); };}
    break;

  case 12:
#line 2012 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of if-statment\n"); };}
    break;

  case 13:
#line 2013 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of loop\n"); };}
    break;

  case 14:
#line 2014 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of defininition of a new sub\n"); };}
    break;

  case 15:
#line 2015 "parser.y"
    { if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of definition of a new cmd\n"); };}
    break;

  case 16:
#line 2019 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::expression;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
            //printf("single_cmd %x is a cmdnode::expression\n", $$);
	;}
    break;

  case 17:
#line 2026 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::return_block;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 18:
#line 2032 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::return_block;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 19:
#line 2038 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::next_block;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 20:
#line 2044 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::last_block;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 21:
#line 2050 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::C_command;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 22:
#line 2056 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::my_var_list;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 23:
#line 2062 "parser.y"
    {
            (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
            (yyval.cmdval)->type = cmdnode::package;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
        ;}
    break;

  case 24:
#line 2068 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::print;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 25:
#line 2074 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::printf;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 26:
#line 2080 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::query;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 27:
#line 2086 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    /* Do not set type; it's already set */
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 28:
#line 2092 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::read_columns;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 29:
#line 2098 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::read_var_list;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 30:
#line 2104 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::split;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 31:
#line 2110 "parser.y"
    {
            (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
            (yyval.cmdval)->type = cmdnode::use;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
        ;}
    break;

  case 32:
#line 2116 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(1) - (1)].cmdval);
	    (yyval.cmdval)->type = cmdnode::user_cmd;
	    (yyval.cmdval)->line = cmdfile_line;
	    (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 33:
#line 2126 "parser.y"
    { (yyval.cmdval) = new_cmdnode(new_wnode_expr((yyvsp[(1) - (1)].eval))); ;}
    break;

  case 34:
#line 2130 "parser.y"
    {
	    wnode *n = new_wnode_string((yyvsp[(1) - (1)].cval));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 35:
#line 2136 "parser.y"
    {
	    wnode *n = new_wnode_string((yyvsp[(1) - (1)].cval));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 36:
#line 2142 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode(new_wnode_expr(new enode(0.0)));
	;}
    break;

  case 37:
#line 2145 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode(new_wnode_expr((yyvsp[(2) - (2)].eval)));
	;}
    break;

  case 38:
#line 2150 "parser.y"
    { (yyval.wval) = new_wnode_string((yyvsp[(1) - (1)].cval));;}
    break;

  case 39:
#line 2151 "parser.y"
    { (yyval.wval) = append_right((yyvsp[(1) - (2)].wval), new_wnode_string((yyvsp[(2) - (2)].cval)));;}
    break;

  case 40:
#line 2155 "parser.y"
    {
	    (yyval.wval) = new_wnode_string((yyvsp[(1) - (1)].cval));
	    //printf("* START WITH '%s'\n", $1);
	    if (cmd_num_matches == 1) {
		//printf("\t--- exact match to cmd %d\n",cmd_matches_to);
		expect = a_expr;
	    }
	;}
    break;

  case 41:
#line 2163 "parser.y"
    {
	    (yyval.wval) = append_right((yyvsp[(1) - (2)].wval), new_wnode_string((yyvsp[(2) - (2)].cval)));
	    //printf("* APPEND '%s' %s\n", $2, cmd_num_matches==1 ? "DONE" : "NOT DONE");
	    if (cmd_num_matches == 1) {
		//printf("\t--- exact match to cmd %d\n",cmd_matches_to);
		expect = a_expr;
	    }
	;}
    break;

  case 42:
#line 2174 "parser.y"
    {
	    if (strcmp((yyvsp[(2) - (6)].cval), "C")) {
		char msg[100];
		sprintf(msg, "Internal error: can only execute \"C\" code, not \"%s\" code", (yyvsp[(2) - (6)].cval));
		yyerror(msg);
		(yyval.cmdval) = NULL;	/* ? not sure if this is proper ?*/
	    } else {
		int i = 0;
		/* 
		 * Search functions linearly -- don't worry about speed since
		 * this only done once.
		 */
		bool found = false;
		while(c_fcn[i].fcn != 0) {
		    if (!strcmp((yyvsp[(4) - (6)].cval)/* ->w */, c_fcn[i].name)) {
			found = true;
			(yyval.cmdval) = new_cmdnode(c_fcn[i].fcn);
			break;
		    }
		    i++;
		}
		if (!found) {
		    string msg("Internal error: no builtin C function named `");
		    msg += (yyvsp[(4) - (6)].cval)/* ->w */;
		    msg += "'";
		    yyerror(msg.c_str());
		    (yyval.cmdval) = NULL;	// BUG -- not sure if this is proper
		}
	    }
	;}
    break;

  case 43:
#line 2207 "parser.y"
    {
		(yyval.wval) = new_wnode_vname((yyvsp[(1) - (1)].cval));
	;}
    break;

  case 44:
#line 2210 "parser.y"
    {
		(yyval.wval) = append_right((yyvsp[(1) - (3)].wval), new_wnode_vname((yyvsp[(3) - (3)].cval)));
	;}
    break;

  case 45:
#line 2216 "parser.y"
    {
	    (yyval.wval) = new_wnode_vname((yyvsp[(1) - (1)].cval));
	;}
    break;

  case 46:
#line 2219 "parser.y"
    {
	    (yyval.wval) = append_right((yyvsp[(1) - (2)].wval), new_wnode_vname((yyvsp[(2) - (2)].cval)));
	;}
    break;

  case 47:
#line 2225 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode(new_wnode_vname((yyvsp[(2) - (2)].cval)));
	    my_var.push_back((yyvsp[(2) - (2)].cval));
	    my_var_bl.push_back(get_block_level());
            (yyval.cmdval)->first_wnode = NULL;
	;}
    break;

  case 48:
#line 2231 "parser.y"
    {
	    wnode *p = (yyvsp[(2) - (2)].wval);
	    while (p != NULL) {
		    my_var.push_back(p->w);
		    my_var_bl.push_back(get_block_level());
		    p = p->next;
	    }
	    (yyval.cmdval) = new_cmdnode(new_wnode_vname((yyvsp[(2) - (2)].wval)->w));
	    (yyval.cmdval)->first_wnode = NULL;
	;}
    break;

  case 49:
#line 2241 "parser.y"
    {
	    wnode *p = (yyvsp[(3) - (4)].wval);
	    while (p != NULL) {
		    my_var.push_back(p->w);
		    my_var_bl.push_back(get_block_level());
		    p = p->next;
	    }
	    (yyval.cmdval) = new_cmdnode(new_wnode_vname((yyvsp[(3) - (4)].wval)->w));
	    (yyval.cmdval)->first_wnode = NULL;
	;}
    break;

  case 50:
#line 2251 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode(new_wnode_vname((yyvsp[(2) - (2)].cval)));
	    my_var.push_back((yyvsp[(2) - (2)].cval));
	    my_var_bl.push_back(get_block_level());
            (yyval.cmdval)->first_wnode = NULL;
	;}
    break;

  case 51:
#line 2257 "parser.y"
    {
	    if (strNE("=", (yyvsp[(3) - (4)].cval))) {
		yyerror("Only allowable operator in `my' command is `='");
	    }
	    (yyval.cmdval) = new_cmdnode(new_wnode_vname((yyvsp[(2) - (4)].cval)));
	    my_var.push_back((yyvsp[(2) - (4)].cval));
	    my_var_bl.push_back(get_block_level());
            enode *ev = new enode(enode::variable, (yyvsp[(2) - (4)].cval), NULL); // BUG: not used
            enode* e = new enode();
	    int which = get_scalar_var_index((yyvsp[(2) - (4)].cval));
	    //printf("MY VARIABLE ASSIGNOP expr ... which = %d\n", which);
            wnode *to_execute = new_wnode_expr(e->assign_to_known_scalar((yyvsp[(2) - (4)].cval), which, (yyvsp[(3) - (4)].cval), (yyvsp[(4) - (4)].eval)));
            (yyval.cmdval)->first_wnode = to_execute;
	;}
    break;

  case 52:
#line 2274 "parser.y"
    {
            cmdnode* new_cmdnode_package(const char* package);
    	    (yyval.cmdval) = new_cmdnode_package((yyvsp[(2) - (2)].cval));
        ;}
    break;

  case 53:
#line 2282 "parser.y"
    {
            use_name.assign((yyvsp[(2) - (2)].cval));
            cmdnode* new_cmdnode_use();
    	    (yyval.cmdval) = new_cmdnode_use();
            //printf("\nBUG: 'use' doesn't search files right.  file='%s'\n\n",use_name.c_str());
        ;}
    break;

  case 54:
#line 2291 "parser.y"
    {
    	    (yyval.cmdval) = new_cmdnode((yyvsp[(2) - (2)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	;}
    break;

  case 55:
#line 2295 "parser.y"
    {
    	    (yyval.cmdval) = new_cmdnode((yyvsp[(3) - (3)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle((yyvsp[(2) - (3)].cval), true);
	;}
    break;

  case 56:
#line 2299 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode(new_wnode_string("columns"));
	    (yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	;}
    break;

  case 57:
#line 2303 "parser.y"
    {
	    if ((yyvsp[(2) - (2)].wval)->next != NULL)
		yyerror("Cannot have more than 1 word in `print' file-handle name.");
	    //printf("DEBUG 1.  word is '%s'\n",$2->w);
	    if (!strcmp((yyvsp[(2) - (2)].wval)->w, "variables")) {
		//printf("DEBUG 1.1 -- seen as 'print variables'\n");
		(yyval.cmdval) = new_cmdnode(new_wnode_string("variables"));
		(yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	    } else if (!strcmp((yyvsp[(2) - (2)].wval)->w, "grid")) {
		//printf("DEBUG 1.2 -- seen as 'print grid'\n");
		(yyval.cmdval) = new_cmdnode(new_wnode_string("grid"));
		(yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	    } else if (!strcmp((yyvsp[(2) - (2)].wval)->w, "image")) {
		(yyval.cmdval) = new_cmdnode(new_wnode_string("image"));
		(yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	    } else {
		//printf("DEBUG 1.3 -- seen as 'print ???' so think handle\n");
		(yyval.cmdval) = new_cmdnode((wnode*)NULL);
		(yyval.cmdval)->handle_index = look_up_file_handle((yyvsp[(2) - (2)].wval)->w, true);
	    }
	;}
    break;

  case 58:
#line 2324 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode((wnode*)NULL);
	    (yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	;}
    break;

  case 59:
#line 2335 "parser.y"
    {
    	    (yyval.cmdval) = new_cmdnode((yyvsp[(2) - (2)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle("STDOUT", true);
	;}
    break;

  case 60:
#line 2339 "parser.y"
    {
    	    (yyval.cmdval) = new_cmdnode((yyvsp[(3) - (3)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle((yyvsp[(2) - (3)].cval), true);
	;}
    break;

  case 61:
#line 2346 "parser.y"
    {
	    int which = get_scalar_var_index((yyvsp[(2) - (2)].cval));
	    if (which < 0) {
		var *v = new var((yyvsp[(2) - (2)].cval), "");
		scalar_vars.push_back(*v);
	    }
	    wnode *n = new_wnode_string((yyvsp[(2) - (2)].cval));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 62:
#line 2355 "parser.y"
    {
	    int which = get_scalar_var_index((yyvsp[(2) - (3)].cval));
	    if (which < 0) {
		var *v = new var((yyvsp[(2) - (3)].cval), "");
		scalar_vars.push_back(*v);
	    }
	    wnode *n = new_wnode_string((yyvsp[(2) - (3)].cval));
	    n = append_right(n, new_wnode_expr(new enode(enode::text, (yyvsp[(3) - (3)].cval))));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 63:
#line 2365 "parser.y"
    {
	    ensure_variable_known((yyvsp[(2) - (6)].cval));
	    wnode *n = new_wnode_string((yyvsp[(2) - (6)].cval));
	    n = append_right(n, new_wnode_expr(new enode(enode::text, (yyvsp[(3) - (6)].cval))));
	    n = append_right(n, (yyvsp[(5) - (6)].wval));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 64:
#line 2378 "parser.y"
    { (yyval.wval) = (yyvsp[(1) - (1)].wval); ;}
    break;

  case 65:
#line 2379 "parser.y"
    { (yyval.wval) = append_right((yyvsp[(1) - (2)].wval), (yyvsp[(2) - (2)].wval)); ;}
    break;

  case 66:
#line 2384 "parser.y"
    {
            //printf("read_columns_spec type 1\n");
            //ALWAYS TRUE: if (strNE($2, "=")) { yyerror("expecting `='"); }
	    (yyval.wval) = new_wnode_columns_spec((yyvsp[(1) - (3)].cval), (yyvsp[(3) - (3)].eval));
	;}
    break;

  case 67:
#line 2389 "parser.y"
    {
            //printf("read_columns_spec type 2\n");
	    //ALWAYS TRUE: if (strNE($2, "=")) { yyerror("expecting `='"); }
	    (yyval.wval) = new_wnode_columns_spec((yyvsp[(1) - (3)].cval), (yyvsp[(3) - (3)].eval));
	;}
    break;

  case 68:
#line 2394 "parser.y"
    {
            //printf("read_columns_spec type 3\n");
	    (yyval.wval) = new_wnode_columns_spec((yyvsp[(1) - (1)].cval), NULL);
	    expect = a_word;	// new
	;}
    break;

  case 69:
#line 2403 "parser.y"
    {
	    (yyval.cmdval) = new_read_columns_cmd("ANONYMOUS", (yyvsp[(3) - (3)].wval), NULL);
	;}
    break;

  case 70:
#line 2406 "parser.y"
    {
	    (yyval.cmdval) = new_read_columns_cmd("ANONYMOUS", (yyvsp[(3) - (4)].wval), (yyvsp[(4) - (4)].eval));
	;}
    break;

  case 71:
#line 2409 "parser.y"
    {
	    (yyval.cmdval) = new_read_columns_cmd((yyvsp[(3) - (4)].cval), (yyvsp[(4) - (4)].wval), NULL);
	;}
    break;

  case 72:
#line 2412 "parser.y"
    {
	    (yyval.cmdval) = new_read_columns_cmd((yyvsp[(3) - (5)].cval), (yyvsp[(4) - (5)].wval), (yyvsp[(5) - (5)].eval));
	;}
    break;

  case 73:
#line 2422 "parser.y"
    { /* 'read grid data|x|y' */
	    //printf("'READ GRID %s' parsed\n", $3);
	    (yyval.cmdval) = new_read_grid_cmd((yyvsp[(3) - (3)].cval), "ANONYMOUS", NULL);
	;}
    break;

  case 74:
#line 2426 "parser.y"
    { /* 'read grid data|x|y ...' */
	    //printf("'READ GRID %s ...' parsed\n", $3);
	    (yyval.cmdval) = new_read_grid_cmd((yyvsp[(3) - (4)].cval), "ANONYMOUS", (yyvsp[(4) - (4)].wval));
	;}
    break;

  case 75:
#line 2430 "parser.y"
    { /* with handle */
	    //printf("'READ GRID %s (handle)' parsed\n", $3);
	    if ((yyvsp[(4) - (4)].wval)->next != NULL)
		yyerror("`read grid data|x|y' cannot have more than 1 handle");
	    (yyval.cmdval) = new_read_grid_cmd((yyvsp[(3) - (4)].cval), (yyvsp[(4) - (4)].wval)->w, NULL);
	;}
    break;

  case 76:
#line 2436 "parser.y"
    { /* with handle */
	    //printf("'READ GRID %s (handle) ...' parsed\n", $3);
	    if ((yyvsp[(4) - (5)].wval)->next != NULL)
		yyerror("`read grid data|x|y' cannot have more than 1 handle");
	    (yyval.cmdval) = new_read_grid_cmd((yyvsp[(3) - (5)].cval), (yyvsp[(4) - (5)].wval)->w, (yyvsp[(5) - (5)].wval));
	;}
    break;

  case 77:
#line 2445 "parser.y"
    { /* not same as 'read columns' */
	    (yyval.cmdval) = new_cmdnode((yyvsp[(3) - (3)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle((yyvsp[(2) - (3)].cval), true);
	;}
    break;

  case 78:
#line 2449 "parser.y"
    {
	    (yyval.cmdval) = new_cmdnode((yyvsp[(2) - (2)].wval));
	    (yyval.cmdval)->handle_index = look_up_file_handle("ANONYMOUS", true);
	;}
    break;

  case 79:
#line 2456 "parser.y"
    {
	    wnode *n = new_wnode_string("");
	    (yyval.cmdval) = new_cmdnode(n);
	    (yyval.cmdval)->array_index = 0;
	;}
    break;

  case 80:
#line 2461 "parser.y"
    {
	    if (strNE("=", (yyvsp[(2) - (3)].cval))) {
		yyerror("Only allowable operator in `split' command is `='");
	    }
	    wnode *n = new_wnode_string(""); /* BUG - don't need this (?) */ 
	    (yyval.cmdval) = new_cmdnode(n);
	    char *name = strdup((yyvsp[(1) - (3)].cval));
	    name[0] = '$';	// fool name recognizer
	    int which = get_vector_var_index(name);
	    if (which < 0) {
		var *v = new var(name, 0.0);
		vector_vars.push_back(*v);
		delete v;
		//printf("SPLIT case 1 -- had to create\n");
		(yyval.cmdval)->array_index = vector_vars.size() - 1;
	    } else {
		//printf("SPLIT case 2 -- already known\n");
		(yyval.cmdval)->array_index = which;
	    }
	    //printf("SPLIT lvalue '%s'-->'%s' --> %d\n",$1,name,$$->array_index);
	    free(name);
	;}
    break;

  case 87:
#line 2496 "parser.y"
    {
	    (yyval.cmdval) = new_if_block((yyvsp[(2) - (5)].eval), (yyvsp[(3) - (5)].cmdval), (yyvsp[(5) - (5)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 88:
#line 2500 "parser.y"
    {
	    (yyval.cmdval) = new_if_block((yyvsp[(2) - (3)].eval), (yyvsp[(3) - (3)].cmdval), NULL);
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 89:
#line 2504 "parser.y"
    {
	    (yyval.cmdval) = new_if_block(logical_negation((yyvsp[(2) - (5)].eval)), (yyvsp[(3) - (5)].cmdval), (yyvsp[(5) - (5)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 90:
#line 2508 "parser.y"
    {
	    (yyval.cmdval) = new_if_block(logical_negation((yyvsp[(2) - (3)].eval)), (yyvsp[(3) - (3)].cmdval), NULL);
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 91:
#line 2515 "parser.y"
    {
	    (yyval.cmdval) = (yyvsp[(2) - (3)].cmdval);
	;}
    break;

  case 92:
#line 2520 "parser.y"
    {
            increase_block_level();
	    cmd_expected_next();
	;}
    break;

  case 93:
#line 2527 "parser.y"
    {
            yyerrok;
	    reduce_block_level();
	;}
    break;

  case 94:
#line 2533 "parser.y"
    {
	    (yyval.cmdval) = new_if_block((yyvsp[(3) - (3)].eval), (yyvsp[(1) - (3)].cmdval), NULL);
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 95:
#line 2539 "parser.y"
    {
	    (yyval.cmdval) = new_if_block(logical_negation((yyvsp[(3) - (3)].eval)), (yyvsp[(1) - (3)].cmdval), NULL);
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 96:
#line 2545 "parser.y"
    {
	    (yyval.cmdval) = new_do_while_block((yyvsp[(3) - (3)].eval), (yyvsp[(1) - (3)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 97:
#line 2551 "parser.y"
    {
	    (yyval.cmdval) = new_do_while_block(logical_negation((yyvsp[(3) - (3)].eval)), (yyvsp[(1) - (3)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 98:
#line 2557 "parser.y"
    { save_string = true; ;}
    break;

  case 99:
#line 2560 "parser.y"
    {
	    (yyval.cmdval) = new_while_block((yyvsp[(3) - (4)].eval), (yyvsp[(4) - (4)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 100:
#line 2566 "parser.y"
    {
	    (yyval.cmdval) = new_while_block(logical_negation((yyvsp[(3) - (4)].eval)), (yyvsp[(4) - (4)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 101:
#line 2572 "parser.y"
    {
	    (yyval.cmdval) = new_do_while_block((yyvsp[(4) - (5)].eval), (yyvsp[(2) - (5)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 102:
#line 2578 "parser.y"
    {
	    (yyval.cmdval) = new_do_while_block(logical_negation((yyvsp[(4) - (5)].eval)), (yyvsp[(2) - (5)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 103:
#line 2584 "parser.y"
    {
	    (yyval.cmdval) = new_for_block((yyvsp[(3) - (9)].eval), (yyvsp[(5) - (9)].eval), (yyvsp[(7) - (9)].eval), (yyvsp[(9) - (9)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 104:
#line 2591 "parser.y"
    { /* Like perl, but no commas*/
	    ensure_variable_known((yyvsp[(2) - (6)].cval));
	    (yyval.cmdval) = new_foreach_block_list((yyvsp[(2) - (6)].cval), (yyvsp[(4) - (6)].wval), (yyvsp[(6) - (6)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 105:
#line 2596 "parser.y"
    {
	    ensure_variable_known((yyvsp[(2) - (6)].cval));
	    (yyval.cmdval) = new_foreach_block_array((yyvsp[(2) - (6)].cval), (yyvsp[(4) - (6)].cval), (yyvsp[(6) - (6)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;	    
	;}
    break;

  case 106:
#line 2604 "parser.y"
    {	/* Catch common error for perl users */ 
	    string msg("Must supply a prototype for sub named `");
	    msg += (yyvsp[(2) - (3)].wval)->w;
	    msg += "'.";
	    yyerror(msg.c_str());
	    if ((yyvsp[(2) - (3)].wval)->next != NULL) {
		yyerror("Cannot have more than 1 word in `sub' name; error detected at end of the 'sub' definition");
	    }
	    (yyval.cmdval) = new_cmdnode();
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 107:
#line 2615 "parser.y"
    {
	    if ((yyvsp[(2) - (6)].wval)->next != NULL) {
		yyerror("Cannot have more than 1 word in `sub' name; error detected at end of the 'sub' definition");
	    }
	    extern vector<user_sub> user_sub_stack;
	    user_sub *s = new user_sub((yyvsp[(2) - (6)].wval), (yyvsp[(4) - (6)].cval), (yyvsp[(6) - (6)].cmdval));
	    user_sub_stack.push_back(*s);
 	    delete s;
	    (yyval.cmdval) = new_cmdnode();	// blank
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	;}
    break;

  case 108:
#line 2629 "parser.y"
    {
	    int create_a_new_cmd(wnode* w, char* p, cmdnode *b);
	    (yyval.cmdval) = new_cmdnode();	// blank
	    (yyval.cmdval)->the_cmd_index = create_a_new_cmd((yyvsp[(2) - (6)].wval), (yyvsp[(4) - (6)].cval), (yyvsp[(6) - (6)].cmdval));
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	    if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of defininition of a new user_cmd, index %d\n", (yyval.cmdval)->the_cmd_index); }
	    expecting_prototype = defining_sub_or_cmd = false;
	;}
    break;

  case 109:
#line 2637 "parser.y"
    {
	    int create_a_new_cmd(wnode* w, char* p, cmdnode *b);
	    (yyval.cmdval) = new_cmdnode();	// blank
	    (yyval.cmdval)->the_cmd_index = create_a_new_cmd((yyvsp[(2) - (5)].wval), (yyvsp[(4) - (5)].cval), NULL);
	    (yyval.cmdval)->line = cmdfile_line; (yyval.cmdval)->file_name = cmdfile_name;
	    if (_gre_debug & DEBUG_TRACE) {printf("%s\n", trace_buffer.c_str()); trace_buffer = ""; printf("# end of prototype of a new user_cmd, index %d\n", (yyval.cmdval)->the_cmd_index); }
	    expecting_prototype = defining_sub_or_cmd = false;
	;}
    break;

  case 110:
#line 2648 "parser.y"
    {
            (yyval.cmdval) = new_user_cmd((yyvsp[(1) - (1)].wval),cmd_matches_to,cmd_num_matches,(wnode*)NULL);
	    (yyval.cmdval)->the_cmd_index = cmd_matches_to;
	;}
    break;

  case 111:
#line 2652 "parser.y"
    {
	    (yyval.cmdval) = new_user_cmd((yyvsp[(1) - (2)].wval), cmd_matches_to, cmd_num_matches, (yyvsp[(2) - (2)].wval));
	    (yyval.cmdval)->the_cmd_index = cmd_matches_to;
	;}
    break;

  case 112:
#line 2659 "parser.y"
    {(yyval.wval) = new_wnode_expr((yyvsp[(1) - (1)].eval));;}
    break;

  case 113:
#line 2660 "parser.y"
    {(yyval.wval) = append_right((yyvsp[(1) - (2)].wval), new_wnode_expr((yyvsp[(2) - (2)].eval)));;}
    break;

  case 114:
#line 2664 "parser.y"
    {
	    printf("parsing SKIP\n");
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip("ANONYMOUS", new enode(1.0)));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 115:
#line 2670 "parser.y"
    {		// handle
	    printf("parsing SKIP WORD\n");
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip((yyvsp[(3) - (4)].cval), new enode(1.0)));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 116:
#line 2676 "parser.y"
    {	// handle, number-lines
	    enode *e;
	    wnode *n = new_wnode_expr(e->skip((yyvsp[(3) - (5)].cval), (yyvsp[(4) - (5)].eval)));
	    (yyval.cmdval) = new_cmdnode(n);
	;}
    break;

  case 117:
#line 2684 "parser.y"
    { (yyval.eval) = new enode(1.0);;}
    break;

  case 119:
#line 2689 "parser.y"
    { (yyval.eval) = new enode((yyvsp[(1) - (1)].dval)); ;}
    break;

  case 120:
#line 2690 "parser.y"
    { // s/from/to/
	//printf("'string_change' $1 '%s'   $2 '%s'\n",$1,$2);
		if (strlen((yyvsp[(1) - (2)].cval)) < 1) {
			yyerror("Substitution replacement cannot have empty pattern.");
		}
		enode *e;
		(yyval.eval) = e->string_change((yyvsp[(1) - (2)].cval), (yyvsp[(2) - (2)].cval));
        ;}
    break;

  case 121:
#line 2698 "parser.y"
    {expecting_matchstring = true;;}
    break;

  case 122:
#line 2698 "parser.y"
    {
            //printf("parser got match item '%s'.  flag='%s'\n", $3, $4);
            enode *e;
            enode *s = new enode(enode::text, (yyvsp[(3) - (4)].cval));
	    (yyval.eval) = e->string_match(s, (yyvsp[(4) - (4)].cval));
            expecting_matchstring = false;
        ;}
    break;

  case 123:
#line 2705 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chop(NULL, cmdfile_name, cmdfile_line);
	;}
    break;

  case 124:
#line 2709 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chop((yyvsp[(2) - (2)].cval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 125:
#line 2713 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chop((yyvsp[(3) - (4)].cval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 126:
#line 2717 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chomp((char*)NULL, cmdfile_name, cmdfile_line);
	;}
    break;

  case 127:
#line 2721 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chomp((yyvsp[(2) - (2)].cval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 128:
#line 2725 "parser.y"
    {
            enode *e;
	    (yyval.eval) = e->do_chomp((yyvsp[(3) - (4)].cval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 129:
#line 2729 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->do_die((enode *)NULL, cmdfile_name, cmdfile_line);
	;}
    break;

  case 130:
#line 2733 "parser.y"
    { 
	    enode *e;
	    (yyval.eval) = e->do_die((yyvsp[(2) - (2)].eval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 131:
#line 2737 "parser.y"
    {
	    if ((yyvsp[(2) - (2)].eval)->is_textual())
		yyerror("Cannot `exit' with textual return code.");
	    enode *e;
	    (yyval.eval) = e->do_exit((yyvsp[(2) - (2)].eval));
	;}
    break;

  case 132:
#line 2743 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->do_glob((yyvsp[(3) - (4)].eval));
	;}
    break;

  case 133:
#line 2747 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->do_index((yyvsp[(3) - (6)].eval)/*str*/, (yyvsp[(5) - (6)].eval) /*substr*/, NULL /*position*/);
	;}
    break;

  case 134:
#line 2751 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->do_index((yyvsp[(3) - (8)].eval)/*str*/, (yyvsp[(5) - (8)].eval) /*substr*/, (yyvsp[(7) - (8)].eval) /*position*/);
	;}
    break;

  case 135:
#line 2755 "parser.y"
    { 
	    enode *e;
	    (yyval.eval) = e->do_warn((yyvsp[(2) - (2)].eval), cmdfile_name, cmdfile_line);
	;}
    break;

  case 136:
#line 2759 "parser.y"
    { 
	    enode *e;
	    (yyval.eval) = e->read_string_from_file("STDIN");
	;}
    break;

  case 137:
#line 2763 "parser.y"
    {
	    if ((yyvsp[(2) - (3)].wval)->next != NULL)
		yyerror("Cannot have more than 1 word in file-handle name.");
	    enode *e;
	    (yyval.eval) = e->read_string_from_file((yyvsp[(2) - (3)].wval)->w);
	;}
    break;

  case 138:
#line 2769 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->test_a_file(*((yyvsp[(1) - (2)].cval) + 1), (yyvsp[(2) - (2)].eval));
	;}
    break;

  case 139:
#line 2773 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->call_system((yyvsp[(3) - (4)].eval));
	;}
    break;

  case 140:
#line 2777 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->substr((yyvsp[(3) - (8)].eval)/*string*/, (yyvsp[(5) - (8)].eval) /*offset*/, (yyvsp[(7) - (8)].eval) /*length*/);
	;}
    break;

  case 141:
#line 2781 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->substr((yyvsp[(3) - (6)].eval)/*string*/, (yyvsp[(5) - (6)].eval) /*offset*/, NULL /*length*/);
	;}
    break;

  case 142:
#line 2785 "parser.y"
    {
	    //printf("\n\nABOUT to create enode for sprintf (paren type)\n\n");
	    enode *e;
	    (yyval.eval) = e->e_sprintf((yyvsp[(3) - (4)].wval));
	;}
    break;

  case 143:
#line 2790 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->rewind_a_file("ANONYMOUS");
	;}
    break;

  case 144:
#line 2794 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->rewind_a_file("ANONYMOUS");
	;}
    break;

  case 145:
#line 2798 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->rewind_a_file((yyvsp[(2) - (2)].cval));
	;}
    break;

  case 146:
#line 2802 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->rewind_a_file((yyvsp[(3) - (4)].cval));
	;}
    break;

  case 147:
#line 2806 "parser.y"
    {		/* works even if dot in filename */ 
	    enode *e;
	    (yyval.eval) = e->open_a_file("ANONYMOUS", new enode(enode::text, (yyvsp[(2) - (2)].cval)));
	;}
    break;

  case 148:
#line 2810 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->open_a_file("ANONYMOUS", (yyvsp[(2) - (2)].eval));
	;}
    break;

  case 149:
#line 2814 "parser.y"
    {
	    char *filehandle = (yyvsp[(3) - (6)].cval);
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
		msg += (yyvsp[(3) - (6)].cval);
		msg += "' must be all upper case.";
		yyerror(msg.c_str());
	    }
#if 1
	    enode *e;
#else
	    enode *e = new enode();	    
#endif
	    (yyval.eval) = e->open_a_file((yyvsp[(3) - (6)].cval) /*handle*/, (yyvsp[(5) - (6)].eval) /*name*/);
	;}
    break;

  case 150:
#line 2837 "parser.y"
    { enode *e; (yyval.eval) = e->close_a_file("ANONYMOUS"); ;}
    break;

  case 151:
#line 2838 "parser.y"
    { enode *e; (yyval.eval) = e->close_a_file((yyvsp[(2) - (2)].cval));          ;}
    break;

  case 152:
#line 2839 "parser.y"
    { enode *e; (yyval.eval) = e->close_a_file((yyvsp[(3) - (4)].cval));          ;}
    break;

  case 153:
#line 2840 "parser.y"
    { (yyval.eval) = new enode(enode::variable, (yyvsp[(1) - (1)].cval), NULL); ;}
    break;

  case 154:
#line 2841 "parser.y"
    { (yyval.eval) = new enode(enode::variable_end, (yyvsp[(1) - (1)].cval), NULL); ;}
    break;

  case 155:
#line 2842 "parser.y"
    { enode *e; (yyval.eval) = e->pattern_match_variable((yyvsp[(1) - (1)].cval));;}
    break;

  case 156:
#line 2843 "parser.y"
    {
		if (strNE("=", (yyvsp[(2) - (5)].cval))) {
			string msg("Expecting `=' but got `");
			msg.append((yyvsp[(2) - (5)].cval));
			msg.append("'.");
			yyerror(msg.c_str());
		}
		enode *e;
		//printf("assigning list\n");
		// Pattern on line parser.y:2399 'ARRAY ASSIGNOP SPLIT'
		char *name = strdup((yyvsp[(1) - (5)].cval));
		name[0] = '$';	// fool name recognizer
		int which = get_vector_var_index(name);
		if (which < 0) {
			var *v = new var(name, 0.0);
			vector_vars.push_back(*v);
			delete v;
			which = vector_vars.size() - 1;
		}
		free(name);
		(yyval.eval) = e->assign_list_to_vector(which, (yyvsp[(4) - (5)].wval));
	;}
    break;

  case 157:
#line 2866 "parser.y"
    { 
	    //printf("expr ASSIGNOP expr ... RHS type is %d (e.g. variable=%d text=%d mul=%d)\n", $3->get_type(), enode::variable, enode::text,enode::mul);
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::variable) {
                //printf("assigning to variable\n");
		enode *e;
		(yyval.eval) = e->assign_to_scalar((yyvsp[(1) - (3)].eval), (yyvsp[(2) - (3)].cval), (yyvsp[(3) - (3)].eval));
	    } else if ((yyvsp[(1) - (3)].eval)->get_type() == enode::variable_at_i) {
                //printf("%s:%d: assigning to variable_at_i\n",__FILE__,__LINE__);
		enode *e;
		(yyval.eval) = e->assign_to_vector((yyvsp[(1) - (3)].eval), (yyvsp[(2) - (3)].cval), (yyvsp[(3) - (3)].eval));
	    } else if ((yyvsp[(1) - (3)].eval)->get_type() == enode::variable_end) {
		// resize a vector
		enode *e;
		(yyval.eval) = e->resize_a_vector((yyvsp[(1) - (3)].eval), (yyvsp[(2) - (3)].cval), (yyvsp[(3) - (3)].eval));
	    } else {
		yyerror("Can't modify constant item");
	    }
	;}
    break;

  case 158:
#line 2884 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->do_unlink((yyvsp[(3) - (4)].wval));
	;}
    break;

  case 159:
#line 2888 "parser.y"
    {
	    (yyval.eval) = new enode(enode::variable_at_i, (yyvsp[(1) - (4)].cval), (yyvsp[(3) - (4)].eval)); 
	;}
    break;

  case 160:
#line 2891 "parser.y"
    { (yyval.eval) = new enode(enode::text, (yyvsp[(1) - (1)].cval)); ;}
    break;

  case 161:
#line 2892 "parser.y"
    {
	    if ((yyvsp[(1) - (3)].eval)->get_type()==enode::text && (yyvsp[(3) - (3)].eval)->get_type() == enode::text) {
		string rr((yyvsp[(1) - (3)].eval)->eval_as_text().get_text());
		rr += (yyvsp[(3) - (3)].eval)->eval_as_text().get_text();
		rr.c_str();
		(yyval.eval) = new enode(enode::text, rr.c_str());
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		enode *e;
		(yyval.eval) = e->concatenate_text((yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 162:
#line 2905 "parser.y"
    { (yyval.eval) = new enode(enode::text_eq, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));;}
    break;

  case 163:
#line 2906 "parser.y"
    { (yyval.eval) = new enode(enode::text_ne, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));;}
    break;

  case 164:
#line 2908 "parser.y"
    { (yyval.eval) = new enode(enode::lt,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 165:
#line 2909 "parser.y"
    { (yyval.eval) = new enode(enode::le,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 166:
#line 2910 "parser.y"
    { (yyval.eval) = new enode(enode::gt,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 167:
#line 2911 "parser.y"
    { (yyval.eval) = new enode(enode::eq,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 168:
#line 2912 "parser.y"
    { (yyval.eval) = new enode(enode::ne,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 169:
#line 2913 "parser.y"
    { (yyval.eval) = new enode(enode::ge,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 170:
#line 2914 "parser.y"
    { (yyval.eval) = new enode(enode::boolean_or,  (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 171:
#line 2915 "parser.y"
    { (yyval.eval) = new enode(enode::boolean_and, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval)); ;}
    break;

  case 172:
#line 2916 "parser.y"
    {
	    enode *e;
	    (yyval.eval) = e->question_mark((yyvsp[(1) - (5)].eval), (yyvsp[(3) - (5)].eval), (yyvsp[(5) - (5)].eval));
	;}
    break;

  case 173:
#line 2920 "parser.y"
    {
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode((yyvsp[(1) - (3)].eval)->eval().get_numeric() + (yyvsp[(3) - (3)].eval)->eval().get_numeric());
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::add, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 174:
#line 2929 "parser.y"
    {
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode((yyvsp[(1) - (3)].eval)->eval().get_numeric() - (yyvsp[(3) - (3)].eval)->eval().get_numeric());
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::sub, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 175:
#line 2938 "parser.y"
    { 
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode((yyvsp[(1) - (3)].eval)->eval().get_numeric() * (yyvsp[(3) - (3)].eval)->eval().get_numeric());
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::mul, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 176:
#line 2947 "parser.y"
    {
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode((yyvsp[(1) - (3)].eval)->eval().get_numeric() / (yyvsp[(3) - (3)].eval)->eval().get_numeric());
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::div, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 177:
#line 2956 "parser.y"
    {
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode(fmod((yyvsp[(1) - (3)].eval)->eval().get_numeric(), (yyvsp[(3) - (3)].eval)->eval().get_numeric()));
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::remainder, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 178:
#line 2965 "parser.y"
    { 
	    if ((yyvsp[(1) - (3)].eval)->get_type() == enode::constant && (yyvsp[(3) - (3)].eval)->get_type() == enode::constant) {
		(yyval.eval) = new enode(pow((yyvsp[(1) - (3)].eval)->eval().get_numeric(), (yyvsp[(3) - (3)].eval)->eval().get_numeric()));
		(yyvsp[(1) - (3)].eval)->~enode();
		(yyvsp[(3) - (3)].eval)->~enode();
	    } else {
		(yyval.eval) = new enode(enode::power, (yyvsp[(1) - (3)].eval), (yyvsp[(3) - (3)].eval));
	    }
	;}
    break;

  case 179:
#line 2974 "parser.y"
    { (yyval.eval) = new enode(enode::boolean_not, (yyvsp[(2) - (2)].eval), NULL);;}
    break;

  case 180:
#line 2975 "parser.y"
    { (yyval.eval) = (yyvsp[(2) - (3)].eval);;}
    break;

  case 181:
#line 2976 "parser.y"
    { // USER FUNCTION
	    (yyval.eval) = new enode('u', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), NULL);
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 182:
#line 2982 "parser.y"
    { // USER FUNCTION
	    (yyval.eval) = new enode('u', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), (yyvsp[(3) - (4)].wval));
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 183:
#line 2988 "parser.y"
    { // BUILTIN FUNCTION
	    (yyval.eval) = new enode('b', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), NULL); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 184:
#line 2994 "parser.y"
    { // BUILTIN FUNCTION
	    (yyval.eval) = new enode('b', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), (yyvsp[(3) - (4)].wval)); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 185:
#line 3000 "parser.y"
    { // BUILTIN FUNCTION
	    // I think this matches 'close(HANDLE)'
	    wnode *w = new_wnode_expr((yyvsp[(3) - (4)].eval));
	    (yyval.eval) = new enode('B', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), w); 
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 186:
#line 3008 "parser.y"
    { // BUILTIN FUNCTION
	    // I think this matches 'open(HANDLE,"a")'
	    wnode *w = new_wnode_expr((yyvsp[(3) - (6)].eval));
	    w = append_right(w, new_wnode_expr((yyvsp[(5) - (6)].eval)));
	    (yyval.eval) = new enode('B', function_which.back(), function_takes_min_arg.back(), function_takes_max_arg.back(), w);
	    function_which.pop_back();
	    function_takes_min_arg.pop_back();
	    function_takes_max_arg.pop_back();
	;}
    break;

  case 187:
#line 3017 "parser.y"
    { (yyval.eval) = new enode(enode::negative, (yyvsp[(2) - (2)].eval), NULL);;}
    break;

  case 188:
#line 3018 "parser.y"
    { (yyval.eval) = (yyvsp[(2) - (2)].eval);;}
    break;

  case 189:
#line 3019 "parser.y"
    { (yyval.eval) = new enode(enode::scalar_postdec, (yyvsp[(1) - (2)].cval), NULL);;}
    break;

  case 190:
#line 3020 "parser.y"
    { (yyval.eval) = new enode(enode::scalar_postinc, (yyvsp[(1) - (2)].cval), NULL);;}
    break;

  case 191:
#line 3021 "parser.y"
    { 
	    (yyval.eval) = new enode(enode::vector_postdec, (yyvsp[(1) - (5)].cval), (yyvsp[(3) - (5)].eval));
	;}
    break;

  case 192:
#line 3024 "parser.y"
    { 
	    (yyval.eval) = new enode(enode::vector_postinc, (yyvsp[(1) - (5)].cval), (yyvsp[(3) - (5)].eval));
	;}
    break;

  case 193:
#line 3030 "parser.y"
    { yyerrok; save_string = false; ;}
    break;

  case 194:
#line 3034 "parser.y"
    { yyerrok; cmd_expected_next(); in_my = false;;}
    break;

  case 195:
#line 3039 "parser.y"
    { (yyval.wval) = new_wnode_expr((yyvsp[(1) - (1)].eval)); ;}
    break;

  case 196:
#line 3040 "parser.y"
    { (yyval.wval) = append_right((yyvsp[(1) - (3)].wval), new_wnode_expr((yyvsp[(3) - (3)].eval))); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 5729 "parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 3045 "parser.y"



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

