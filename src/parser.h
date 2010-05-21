// -*- Mode: c++ -*-

#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>		/* for c++ string class */
#include <string.h>		/* for e.g. strdup (Fujitsu needs) */
#include <math.h>
//#include <malloc.h>

// Removed 19990720 (1999 Jul 20)
//#if defined(_MSC_VER)
//#include <iostream>		/* Q: DO I NEED THIS? */
//#else
//#include <stream>		/* Q: DO I NEED THIS? */
//#endif

#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
#include "private.h"
#include "subs.h"
#include "scalar.h"
#include "debug.h"
#include "macro.h"

enum cmd_eval_code {rval_error, rval_okay, rval_last, rval_return, rval_next};

int next_char();
int last_char();
void pop_char();
char * interpolate_var_in_string(const char* string);

using namespace std;

void runtime_warning(const char *msg);
void runtime_warning(const char *f, unsigned int l, const char *msg);
void compile_warning(const char *s, ...);
int yyerror(const char *s);
void increase_block_level();
void reduce_block_level();



/*
 * var -- numeric or text variables
 */
class var {
public:
	/* Text */
	var(const char *n, const char *v) {
		if (_gre_debug & DEBUG_VARIABLES) printf("var(%s,%s) START    [%s:%d]\n",n,v,__FILE__,__LINE__);
		is_vector = false;
		scalar s(v);
		val.push_back(s);
		name.assign(n);
		references = 0;
		hash_value = hash(n);
		if (_gre_debug & DEBUG_VARIABLES) printf("  END this at %lx  [%s:%d]\n",(unsigned long)this,__FILE__,__LINE__);
	}
	/* Numeric */
	var(const char *n, double d) {
		if (_gre_debug & DEBUG_VARIABLES) printf("var(%s,%f) START    [%s:%d]\n",n,d,__FILE__,__LINE__);
		name.assign(n);
		scalar s(d);
		val.push_back(s);
		is_vector = false;
		references = 0;
		hash_value = hash(n);
		if (_gre_debug & DEBUG_VARIABLES) printf("  END this at %lx  [%s:%d]\n",(unsigned long)this,__FILE__,__LINE__);
	}
	/* Scalar numeric */
	void set_value(double d) {
		is_vector = false;	/* make vector */
		if (val.size() > 1)
			val.erase(val.begin() + 1, val.end());
		val[0].set_numeric(d);
	}
	/* Scalar text */
	void set_value(const char* t){ 
		is_vector = false;	/* make vector */
		if (val.size() > 1)	
			val.erase(val.begin() + 1, val.end());
		val[0].set_text(t);
	}
	/* Vector numeric */
	void set_value(double v, int index) {
		is_vector = true;
		if (index < 0) {
			val.erase(val.begin(), val.end()); // clear it
			if (_gre_debug & DEBUG_VARIABLES) printf("var::set_value() cleared vector %s[]\n",name.c_str());
		} else {
			unsigned int present_size = val.size();
			if (index < int(present_size)) {
				val[index].set_numeric(v);
			} else {
				scalar s(0.0);
				for (int i = int(present_size); i < index; i++) {
					val.push_back(s);
				}
				s.set_numeric(v);
				val.push_back(s);
				if (_gre_debug & DEBUG_VARIABLES) printf("var::set_value() set %s[%d] to %f.  new size=%d\n",name.c_str(),index,v,int(val.size()));
			}
		}
	}
	/* Vector text */
	void set_value(const char *v, int index) {
		is_vector = true;
		if (index < 0) {
			val.erase(val.begin(), val.end()); // clear it
			if (_gre_debug & DEBUG_VARIABLES) printf("var::set_value() cleared vector %s[]\n",name.c_str());
		} else {
			unsigned int present_size = val.size();
			if (index < int(present_size)) {
				val[index].set_text(v);
			} else {
				scalar s(0.0);
				for (int i = int(present_size); i < index; i++) {
					val.push_back(s);
				}
				s.set_text(v);
				val.push_back(s);
				if (_gre_debug & DEBUG_VARIABLES) printf("var::set_value() set %s[%d] to '%s'.  new size=%d\n",name.c_str(),index,v,int(val.size()));
			}
		}
	}
	const scalar& get_at_index(int i) const {
		//printf("scalar::get_at_index(%d)\n", i);fflush(stdout);
		//printf("scalar::get_at_index(%d).  val.size is %d\n", i, val.size());fflush(stdout);
		if (i == 0)		// common case: skip size() call
			return val[0];
		if (0 < i && i < int(val.size()))
			return val[i];
		// Return a zeroed-out dummy
		scalar *r = new scalar("");
		return *r;
	}
	bool get_type(int i) const	{ // 1=is_numeric
		int n = val.size();
		if (i < 0)
			i += n;	// negative index
		if (i > n)
			return false;	// out of bounds
		return val[i].is_numeric();
	}
	unsigned int get_size() const { return val.size(); }
	int get_end_index()         const  {
		if (!is_vector)
			return -1;
		else
			return val.size() - 1;
	}
	void resize(int newsize) {
		if (newsize < 1) {
			val.erase(val.begin(), val.end());
		} else {
			int present_size = val.size();
			if (newsize > present_size) {
				/* fill with zeros */
				scalar s(0.0);
				for (int i = int(present_size); i < newsize; i++) {
					val.push_back(s);
				}
			} else {
				val.erase(val.begin() + newsize, val.begin() + present_size);
			}
		}
	}
	unsigned int get_hash() const { return hash_value;} 
	const string& get_name_string() const {return name;}
	const char *get_name()	const { printf("var/get_name...\n"); printf("  var/get_name returning '%s'\n",name.c_str()); return name.c_str(); }
	double get_numeric_scalar () const	{ 
		if (val[0].is_numeric())
			return val[0].get_numeric();
		else
			return 0.0;		/* BUG */
	}
	bool get_text_scalar (string& result) const { 
		if (!val[0].is_numeric())
			result.assign(val[0].get_text());
		else {
			char msg[100];	// hope long enough
			sprintf(msg, "%f", val[0].get_numeric());
			result.assign(msg);
		}
		return true;
	}
	double get_numeric_vector (unsigned int i) const { 
		if (val[i].is_numeric())
			return val[i].get_numeric();
		else
			return 0.0;		/* BUG */
	}
	const char* get_text_vector (unsigned int i) const { 
		if (!val[i].is_numeric())
			return val[i].get_text();
		else
			return "";		/* BUG */
	}
	void clear_vector() {
		if (_gre_debug & DEBUG_VARIABLES) printf("    doing a clear_vector()  [%s:%d]\n",__FILE__,__LINE__);
		val.erase(val.begin(), val.end());
	}
	void erase_name() {
		if (_gre_debug & DEBUG_VARIABLES) printf("erasing name of variable, previously '%s'\n", name.c_str());
		name = "";
	}
	unsigned int get_references() const { return references; }
	void increment_references() {references++;}
	// -- Need the items below for STL --
	bool operator<  (const var& ignored) const {return false;} // Need for STL
	bool operator== (const var& ignored) const {return false;} // Need for STL
	~var() {		// Need for STL
		if(_gre_debug & DEBUG_VARIABLES) printf("~var() erasing at %lx (name='%s'  var @ %lx)   [%s:%d]\n",(unsigned long)this,name.c_str(),(unsigned long)&val,__FILE__,__LINE__);
		val.erase(val.begin(), val.end());
		name.~string(); // BUG: do we need this???
	}
	var() {			// Need for STL
		if (_gre_debug & DEBUG_VARIABLES) printf("var() START    [%s:%d]\n",__FILE__,__LINE__);
		is_vector = false;
		scalar s(0.0);
		val.push_back(s);
		name.assign("");
		references = 0;
		hash_value = hash("");
		if (_gre_debug & DEBUG_VARIABLES) printf("  END var() val@ %lx this@ %lx  [%s:%d]\n",(unsigned long)this,(unsigned long)&val,__FILE__,__LINE__);
	}
	var(const var& n) {	// need for STL
		if (_gre_debug & DEBUG_VARIABLES) printf("var(var at %lx) START (name is '%s') (size is %d)    [%s:%d]\n", (unsigned long)(&n), n.get_name(),n.get_size(),__FILE__,__LINE__);
		is_vector = n.get_end_index() == -1 ? false : true;
		printf("    1. about to get the name of copied var\n");
		const char* dummy=n.get_name();
		printf("    1.1.1 got the name...\n");
		const char* dummy2=n.get_name();
		printf("    1.1.2 got the name...\n");
		string nn(n.get_name());
		printf("    1.2 got the name...\n");
		//name.assign(n.get_name());
		name = nn;
		printf("    2. got the var name ok\n");
		unsigned int l = n.get_size();
		clear_vector();
		for (unsigned int i = 0; i < l; i++) {
			val.push_back(n.get_at_index(i));
		}
		references = n.get_references();
		hash_value = n.get_hash();
		printf("  var(var) END parser.h ~246\n");
		if (_gre_debug & DEBUG_VARIABLES) printf("  END var(var) returning this at %lx\n",(unsigned long)this);
	}
	var& operator=(const var& n) { // need for STL
		if (_gre_debug & DEBUG_VARIABLES) printf("var operator=(var at %lx) START    [%s:%d]\n",(unsigned long)(&n),__FILE__,__LINE__);
		is_vector = n.get_end_index() == -1 ? false : true;
		name.assign(n.get_name());
		unsigned int l = n.get_size();
		clear_vector();
		for (unsigned int i = 0; i < l; i++) {
			val.push_back(n.get_at_index(i));
		}
		references = n.get_references();
		hash_value = n.get_hash();
		if (_gre_debug & DEBUG_VARIABLES) printf("  END var operator= returning this at %lx\n",(unsigned long)this);
		return *this;
	}
private:
	bool		is_vector;
	vector<scalar>  val;
	string		name;
	unsigned int    references;
	unsigned int    hash_value;
};				// class var

/* Functions */
void show_var_stack();
bool put_var(const char *n, double val, int index);
bool put_var(const char *n, double val);
bool put_var(const char *n, const char *val);
bool put_var(const char *n, const char *val, int index);

/* New access functions, using symbol table */
/* Scalar */
double get_var_scalar_numeric_at(int which);
bool   put_var_scalar_numeric_at(int which, double value);
const char*  get_var_scalar_text_at(int which);
bool         put_var_scalar_text_at(int which, const char* value);
/* Vector: */
bool   clear_var_vector_at(int which);
double get_var_vector_numeric_at(int which, int index);
bool   put_var_vector_numeric_at(int which, int index, double value);
const char* get_var_vector_text_at(int which, int index);
bool        put_var_vector_text_at(int which, int index, const char* value);
double get_var_vector_end(int which);
void put_var_vector_size(int which, int newsize);

int get_scalar_var_index(const char *n);
int get_vector_var_index(const char *n);
double get_var_vector(const char *n, int index);
double get_var_scalar(const char *n);
char* get_var_text(const char *n);
extern vector<var> scalar_vars;
extern vector<var> vector_vars;

extern vector<bool>   file_input;
extern vector<string> file_handle;
extern vector<string> file_name;
extern vector<FILE*>  file_pointer;
extern vector<string> pipe_tmpfile;

int look_up_file_handle(const char *name, bool create_if_not_found);



/*
 * wnode -- word node (variable-name, or raw word, or expression)
 */
struct wnode {		/* word node */
	enum w_type {unknown, vname, word, expr, column_spec};
	char  *w;		/* word */
	class enode *e;		/* expr */
	w_type t;		/* type */
	wnode* next;
};				// wnode

/*
 * enode -- expression node
 */
class enode {
public:
	enum type {empty,		// 0
		   assign_scalar,	// 1
		   assign_list_vector, // 2
		   assign_vector,	// 3
		   resize_vector,	// 4
		   // Text operations
		   text, text_dot, text_eq, text_ne, text_glob,// 5-9
		   // Numeric comparisons
		   eq, ne, lt, le, ge, gt,// 10-15
		   // Numeric operations
		   add, sub, mul, div, remainder, negative,	power,// 16-22
		   // Logical operations
		   boolean_not, boolean_and, boolean_or,    // 23-25
		   // Functions
		   function_builtin_numeric_0,// 26
		   function_builtin_numeric_1,
		   function_builtin_numeric_2,
		   function_builtin_numeric_3,
		   function_builtin_numeric_4,
		   function_builtin_text_0,   
		   function_builtin_text_1,   
		   function_builtin_text_2, // 33
		   function_user,   // 34
		   // Constants and variables
		   constant,	// 35
		   pattern_variable,
		   variable,	
		   variable_at_i,	
		   variable_end,	
		   scalar_postdec,	
		   scalar_postinc,	
		   vector_postdec,	
		   vector_postinc,	
		   // File test
		   file_test,	
		   // Some commands
		   close_file,
		   die,
		   exit_fcn,
		   warn,
		   open_file,
		   rewind_file,
		   skip_a_line,
		   system_call,
		   // Textual things
		   string_from_file,
		   text_index,
		   text_chomp,
		   text_chop,
		   text_sprintf,
		   text_substr,
		   question,
		   match_a_string,
		   change_a_string,
		   unlink_files
	};
	enode() {
		the_type = empty;
	}

	enode(double the_value) {
		textual_value = variable_name = NULL;
		the_type = constant;
		constant_numerical_value = the_value;
		variable_position_scalar = variable_position_vector = -1;
	}

	enode(type t, const char *s) { /* text (set filename if from file) */
		textual_value = variable_name = NULL;
		variable_position_scalar = variable_position_vector = -1;
		if (t == text) {
			the_type = text;
			textual_value = new char [1 + strlen(s)];
			if (!textual_value) { fprintf(stderr, "Out of storage\n"); exit(1); }
			strcpy(textual_value, s);
			text_from_file = false;
			text_to_underline = false;
		} else {
			fprintf(stderr, "Internal error: unknown type in enode(%d,%s)\n", int(t), s);
			exit(1);
		}
	}
	enode* read_string_from_file(const char *handle) {
		enode* e = new enode();
		e->which_handle = look_up_file_handle(handle, true);
		extern bool save_string;
		e->text_to_underline = save_string;
		e->the_type = text;
		e->text_from_file = true;
		return e;
	}
	enode* question_mark(enode *test, enode *true_part, enode *false_part) {
		enode* e = new enode();
		e->middle = test;
		e->left = true_part;
		e->right = false_part;
		e->the_type = question;
		return e;
	}
	enode* test_a_file(char m, enode *filename) {
		enode* e = new enode();
		e->file_test_flag = m;
		e->left = filename;
		e->the_type = file_test;
		return e;
	}
	enode* call_system(enode *cmd) {
		enode* e = new enode();
		e->left = cmd;
		e->the_type = system_call;
		return e;
	}
	enode* string_change(const char *from, const char *to) {
		enode* e = new enode();
		e->left = new enode(enode::text, from);
		e->right = new enode(enode::text, to);
		e->the_type = change_a_string;
		return e;
	}
	enode* string_match(enode *s, const char* flag) {
		enode* e = new enode();
		e->variable_name = strdup(flag);
		e->left = s;
		e->the_type = match_a_string;
		return e;
	}
	enode* do_chop(const char *var_name, const char* name, unsigned int line) {
		enode* e = new enode();
		if (var_name == NULL) {	// use $_
			//printf("varname is NULL\n");
			e->variable_position_scalar = 0;
		} else {		// use named variable
			//printf("in enode* do_chomp.  var '%s'\n",var_name);
			int which = get_scalar_var_index(var_name);
			//printf("    which = %d\n", which);
			if (which < 0) {
				var *v = new var(var_name, "");
				scalar_vars.push_back(*v);
				e->variable_position_scalar = scalar_vars.size() - 1;
			} else {
				e->variable_position_scalar = which;
			}
		}
		e->the_type = text_chop;
		e->file_name = name;
		e->file_linenumber = line;
		return e;
	}
	enode* do_chomp(const char *var_name, const char* name, unsigned int line) {
		//printf("in enode* do_chomp.\n");
		enode* e = new enode();
		if (var_name == NULL) {	// use $_
			//printf("varname is NULL\n");
			e->variable_position_scalar = 0;
		} else {		// use named variable
			//printf("in enode* do_chomp.  var '%s'\n",var_name);
			int which = get_scalar_var_index(var_name);
			//printf("    which = %d\n", which);
			if (which < 0) {
				var *v = new var(var_name, "");
				scalar_vars.push_back(*v);
				e->variable_position_scalar = scalar_vars.size() - 1;
			} else {
				e->variable_position_scalar = which;
			}
		}
		//printf(" ... nb variable position is %d\n", e->variable_position_scalar);
		e->the_type = text_chomp;
		e->file_name = name;
		e->file_linenumber = line;
		return e;
	}
	enode* do_die(enode *message, const char* name, unsigned int line) {
		enode* e = new enode();
		e->left = message;
		e->the_type = die;
		e->file_name = name;
		e->file_linenumber = line;
		return e;
	}
	enode* do_glob(enode* str) {
		enode *e = new enode();
		e->left = str;
		e->the_type = text_glob;
		return e;
	}
	enode* do_index(enode* str, enode* substr, enode* position) {
		enode *e = new enode();
		e->left = str;
		e->right = substr;
		e->middle = position;
		e->the_type = text_index;
		return e;
	}
	enode* do_exit(enode *message) {
		enode* e = new enode();
		e->left = message;
		e->the_type = exit_fcn;
		return e;
	}
	enode* do_warn(enode *message, const char* name, unsigned int line) {
		enode* e = new enode();
		e->left = message;
		e->the_type = warn;
		e->file_name = name;
		e->file_linenumber = line;
		return e;
	}
	enode* do_unlink(wnode *file_list) { // patterned on e_sprintf
		enode* e = new enode();
		e->sub_arg_list = file_list;
		e->the_type = unlink_files;
		return e;
	}
	enode* substr(enode *the_text, enode *the_offset, enode *the_length) {
		enode* e = new enode();
		e->left = the_text;
		e->right = the_offset;
		e->middle = the_length;
		e->the_type = text_substr;
		return e;
	}
	enode* e_sprintf(wnode *the_list) {
		enode* e = new enode();
		e->sub_arg_list = the_list;
		e->the_type = text_sprintf;
		return e;
	}
	enode* open_a_file(const char *handle, enode *name) {
		//printf("enode open_a_file(%s,%s)\n",handle,name->eval().get_text());
		enode* e = new enode();
		e->left = name;
		e->the_type = open_file;
		e->which_handle = look_up_file_handle(handle, true);
		//printf("'open_a_file' handle '%s'\n", handle);
		//printf("'open_a_file' type %d\n", e->the_type);
		return e;
	}
	enode* skip(const char *handle, enode *num) {
		enode* e = new enode();
		e->which_handle = look_up_file_handle(handle, true);
		e->left = num;
		e->the_type = skip_a_line;
		return e;
	}
	enode* rewind_a_file(const char *handle) {
		//printf("enode rewind_a_file(%s)\n",handle);
		enode* e = new enode();
		e->left = NULL;
		e->the_type = rewind_file;
		e->which_handle = look_up_file_handle(handle, true);
		//printf("\tsaved handle '%s' as index %d\n", handle, e->which_handle);
		return e;
	}
	enode* assign_to_scalar(enode* lvalue, const char *assign_op, enode* rvalue)
		{
			enode* e = new enode();
			e->the_type = assign_scalar;
			int which = lvalue->get_variable_position_scalar();
			if (which < 0) {
				printf("ERROR: no such scalar variable.\n");
				exit(1);
			}
			//printf("parser.h/assign_to_scalar index %d  %s\n", which,lvalue->get_vname());
			e->variable_position_scalar = which;
			scalar_vars[which].increment_references();
			e->variable_name = strdup(lvalue->get_vname());
			if (!e->variable_name) {fprintf(stderr, "Out of storage\n"); exit(1);}
			e->right = rvalue;
			if (!strcmp(assign_op, "=")) e->assignment_op = 0;
			else if (!strcmp(assign_op, "+=")) e->assignment_op = 1;
			else if (!strcmp(assign_op, "-=")) e->assignment_op = 2;
			else if (!strcmp(assign_op, "*=")) e->assignment_op = 3;
			else if (!strcmp(assign_op, "/=")) e->assignment_op = 4;
			else if (!strcmp(assign_op, "**=")) e->assignment_op = 5;
			else if (!strcmp(assign_op, "=~")) e->assignment_op = 6;
			else yyerror("Unknown assignment operator");
			return e;
		}
	enode* assign_to_known_scalar(const char* vname, int which, const char *assign_op, enode* rvalue)
		{
			enode* e = new enode();
			e->the_type = assign_scalar;
			//printf("parser.h/assign_to_known_scalar index %d (%s)\n", which,vname);
			e->variable_position_scalar = which;
			scalar_vars[which].increment_references();
			e->variable_name = strdup(vname);
			if (!e->variable_name)  {fprintf(stderr, "Out of storage\n"); exit(1);}
			e->right = rvalue;
			if (!strcmp(assign_op, "=")) e->assignment_op = 0;
			else if (!strcmp(assign_op, "+=")) e->assignment_op = 1;
			else if (!strcmp(assign_op, "-=")) e->assignment_op = 2;
			else if (!strcmp(assign_op, "*=")) e->assignment_op = 3;
			else if (!strcmp(assign_op, "/=")) e->assignment_op = 4;
			else if (!strcmp(assign_op, "**=")) e->assignment_op = 5;
			else if (!strcmp(assign_op, "=~")) e->assignment_op = 6;
			else yyerror("Unknown assignment operator");
			return e;
		}

	enode* assign_list_to_vector(int which, wnode* list)
		{
			//printf("%s:%d: created enode assign_list_to_vector(which=%d,...)\n",__FILE__,__LINE__,which);
			enode* e = new enode();
			e->the_type = assign_list_vector;
			e->variable_position_vector = which;
			e->sub_arg_list = list;
			return e;
		}

	enode* assign_to_vector(enode* lvalue, const char *assign_op, enode* rvalue)
		{
			//printf("%s:%d: enode assign_to_vector(*,%s,*)\n",__FILE__,__LINE__,assign_op);
			enode* e = new enode();
			e->the_type = assign_vector;
			int which = lvalue->get_variable_position_vector();
			//printf("%s:%d: lvalue which = %d\n", __FILE__,__LINE__,which);
			if (which < 0) {
				fprintf(stderr, "ERROR:%s:%d no such vector variable.\n",__FILE__,__LINE__);
				exit(1);
			}
			e->variable_position_vector = which;
			vector_vars[which].increment_references();
			e->variable_name = strdup(lvalue->get_vname());
			if (!e->variable_name) {fprintf(stderr, "Out of storage\n"); exit(1);}
			e->left = lvalue->get_left();
			e->right = rvalue;
			if (!strcmp(assign_op, "=")) e->assignment_op = 0;
			else if (!strcmp(assign_op, "+=")) e->assignment_op = 1;
			else if (!strcmp(assign_op, "-=")) e->assignment_op = 2;
			else if (!strcmp(assign_op, "*=")) e->assignment_op = 3;
			else if (!strcmp(assign_op, "/=")) e->assignment_op = 4;
			else if (!strcmp(assign_op, "**=")) e->assignment_op = 5;
			else if (!strcmp(assign_op, "=~")) e->assignment_op = 6;
			else yyerror("Unknown assignment operator");
			return e;
		}
	enode* resize_a_vector(enode* lvalue, const char *assign_op, enode* rvalue)
		{
			//printf("enode resize_vector(*,%s,*)\n",assign_op);
			enode* e = new enode();
			e->the_type = resize_vector;
			int which = lvalue->get_variable_position_vector();
			//printf("the lvalue which = %d\n", which);
			if (which < 0) {
				printf("ERROR: no such vector variable.\n");
				exit(1);
			}
			e->variable_position_vector = which;
			e->variable_name = strdup(lvalue->get_vname());
			if (!e->variable_name) {fprintf(stderr, "Out of storage\n"); exit(1);}
			e->left = lvalue->get_left();
			e->right = rvalue;
			if (!strcmp(assign_op, "=")) e->assignment_op = 0;
			else if (!strcmp(assign_op, "+=")) e->assignment_op = 1;
			else if (!strcmp(assign_op, "-=")) e->assignment_op = 2;
			else if (!strcmp(assign_op, "*=")) e->assignment_op = 3;
			else if (!strcmp(assign_op, "/=")) e->assignment_op = 4;
			else if (!strcmp(assign_op, "**=")) e->assignment_op = 5;
			else if (!strcmp(assign_op, "=~")) e->assignment_op = 6;
			else yyerror("Unknown assignment operator");
			return e;
		}
	enode* pattern_match_variable(const char *name)
		{
			//printf("enode* pattern_match_variable(%s) %d\n", name, atoi(name+1));
			enode* e = new enode();
			e->the_type = pattern_variable;
			e->which_handle = atoi(name+1);
			return e;
		}
	enode* close_a_file(const char *handle) 
		{
			//printf("enode close_a_file() ... SHOULD CLOSE handle '%s'\n", handle);
			enode* e = new enode();
			e->the_type = close_file;
			e->which_handle = look_up_file_handle(handle, true);
			//printf("\tsaved handle '%s' as index %d\n", handle, e->which_handle);
			return e;
		}
	/* Access variable (vector, if index non-NULL, or scalar if index NULL) */
	enode(type t, char *var_name, enode *index) {
		//printf("DEBUG: doing variable-access enode(%d,%s,...)\n",t,var_name); 
		textual_value = variable_name = NULL;
		the_type = t;
		if (the_type == variable
		    || the_type == scalar_postinc
		    || the_type == scalar_postdec) {

			int which = get_scalar_var_index(var_name);
			//printf("enode() %s -> %d\n",var_name,which);

			if (which < 0) {
				//printf("enode for 'variable' '%s' (NEWLY CREATED)\n",var_name);
				var *v = new var(var_name, "");
				scalar_vars.push_back(*v);
				variable_position_scalar = scalar_vars.size() - 1;
			} else {
				scalar_vars[which].increment_references();
				//printf("enode for 'variable' '%s' (which= %d)\n",var_name, which);
				variable_position_scalar = which;
			}
		} else if (the_type == vector_postinc || the_type == vector_postdec) {
			int which = get_vector_var_index(var_name);
			//printf("for this thingee, posn is %d\n",which);
			if (which < 0) {
				var *v = new var(var_name, 0.0);
				vector_vars.push_back(*v);
				delete v;
				variable_position_vector = vector_vars.size() - 1;
			} else {
				vector_vars[which].increment_references();
				variable_position_vector = which;
			}
			left = index;	/* store index */
			//printf("enode for vector++ or vector-- on '%s' (got which=%d)\n",var_name, which);
		} else if (the_type == variable_end) {
			int which = get_vector_var_index(var_name);
			if (which < 0) {
				variable_position_vector = -1;
				which = get_scalar_var_index(var_name);
				if (which < 0) {
					variable_position_scalar = -1;
				} else {
					variable_position_scalar = which;
					scalar_vars[which].increment_references();
				}
			} else {
				variable_position_vector = which;
				vector_vars[which].increment_references();
			}
		} else if (the_type == variable_at_i) {
			int which = get_vector_var_index(var_name);
			if (which < 0) {
				var *v = new var(var_name, 0.0);
				vector_vars.push_back(*v);
				delete v;
				//printf("-- case 1 had to construct\n");
				variable_position_vector = vector_vars.size() - 1;
			} else {
				//printf("-- case 2 already known\n");
				vector_vars[which].increment_references();
				variable_position_vector = which;
			}
			//printf("enode for 'variable_at_i' '%s' variable_position_which=%d\n",var_name,variable_position_vector);
			left = index;	/* store index */
		}
		// BUG: below prob not needed.
		variable_name = strdup(var_name);
		if (!variable_name) {fprintf(stderr, "Out of storage\n"); exit(1);}
	}
	//* Various binary operations
	enode(type t, enode *l, enode *r) {
		textual_value = variable_name = NULL;
		variable_position_scalar = variable_position_vector = -1;
		the_type = t;
		left = l;
		right = r;
	}

	enode(char flag, unsigned int which, int min_args, int max_args, wnode *function_argument) { // FUNCTION, BUILTIN or USER
		//printf("constructing enode() for fcn\n");
		if (flag != 'b'		// builtin numeric
		    && flag != 'B'	// builtin text
		    && flag != 'u') {	// user
			yyerror("Internal error: function neither builtin nor user");
			return;
		}
		int words_in_args = 0;
		wnode *arg = function_argument;
		while (arg != NULL) {
			arg = arg->next;
			words_in_args++;
		}
		// ******************************************
		// *** BUG -- DO NOT SUPPORT POLYMORPHISM ***
		// ******************************************
	
		// *******************************************************
		// *** BUG -- DO NOT SUPPORT VARIABLE-LENGTH ARG LISTS ***
		// *******************************************************

		if (flag == 'B') {
			extern SUB0_TEXT sub0_text[];
			extern SUB1_TEXT sub1_text[];
			extern SUB2_TEXT sub2_text[];
			//printf("%s:%d wia=%d  which=%d\n",__FILE__,__LINE__, words_in_args,which);
			if (min_args == 0) {
				func_of_0_text = sub0_text[which].fcn;
				the_type = function_builtin_text_0;
				left = NULL;
				function_argument = function_argument->next;
			} else if (min_args == 1) {
				func_of_1_text = sub1_text[which].fcn;
				the_type = function_builtin_text_1;
				left = function_argument->e;
				function_argument = function_argument->next;
			} else if (min_args == 2) {
				func_of_2_text = sub2_text[which].fcn;
				the_type = function_builtin_text_2;
				left = function_argument->e;
				function_argument = function_argument->next;
				right = function_argument->e;
			} else {
				yyerror("Internal error: builtin function must take 0, 1 or 2 args");
			}
		} else if (flag == 'b') {
			extern SUB0 sub0[];
			extern SUB1 sub1[];
			extern SUB2 sub2[];
			extern SUB3 sub3[];
			extern SUB4 sub4[];
			if (words_in_args != min_args) { // max_args will be same for builtin
				string msg("Wrong number of arguments to built-in function `");
				if (min_args == 0) {
					msg += sub0[which].name;
					msg += "', which takes no arguments.";
				} else if (min_args == 1) {
					msg += sub1[which].name;
					msg += "', which takes a single argument.";
				} else if (min_args == 2) {
					msg += sub2[which].name;
					msg += "', which takes two arguments.";
				} else if (min_args == 3) {
					msg += sub3[which].name;
					msg += "', which takes three arguments.";
				} else if (min_args == 4) {
					msg += sub4[which].name;
					msg += "', which takes four arguments.";
				}
				yyerror(msg.c_str());
				return;
			}
			if (min_args == 0) {
				func_of_0 = sub0[which].fcn;
				the_type = function_builtin_numeric_0;
				left = NULL;
			} else if (min_args == 1) {
				func_of_1 = sub1[which].fcn;
				the_type = function_builtin_numeric_1;
				left = function_argument->e;
			} else if (min_args == 2) {
				func_of_2 = sub2[which].fcn;
				the_type = function_builtin_numeric_2;
				left = function_argument->e;
				function_argument = function_argument->next;
				right = function_argument->e;
			} else if (min_args == 3) {
				func_of_3 = sub3[which].fcn;
				the_type = function_builtin_numeric_3;
				left = function_argument->e;
				function_argument = function_argument->next;
				right = function_argument->e;
				function_argument = function_argument->next;
				middle = function_argument->e;
			} else if (min_args == 4) {
				func_of_4 = sub4[which].fcn;
				the_type = function_builtin_numeric_4;
				left = function_argument->e;
				function_argument = function_argument->next;
				right = function_argument->e;
				function_argument = function_argument->next;
				middle = function_argument->e;
				function_argument = function_argument->next;
				extra = function_argument->e;
			} else {
				yyerror("Internal error: builtin function must take 0, 1 or 2 args");
			}
		} else {		// must be 'u' for user
			extern vector<user_sub> user_sub_stack;
			if (min_args > -1 && words_in_args < min_args) {
				string msg("Too few arguments to builtin function `");
				msg += user_sub_stack[which].get_name().c_str();
				msg += "' -- must be given at least ";
				char num[100];
				sprintf(num, "%d.", min_args);
				msg += num;
				yyerror(msg.c_str());
			} else {
				if (max_args > -1 && words_in_args > max_args) {
					string msg("Too many arguments to builtin function `");
					msg += user_sub_stack[which].get_name().c_str();
					msg += "' -- must have at most ";
					char num[100];
					sprintf(num, "%d.", min_args);
					msg += num;
					yyerror(msg.c_str());
				} else {
					the_type = function_user;
					sub_index = which;
					sub_arg_list = function_argument;
				}
			}
		}
	}
	enode* concatenate_text(enode *l, enode *r) {
		enode* e = new enode();
#if 0
		if (!l->is_textual()) {
			yyerror("Cannot do '.' since item to left is not textual.");
			return e;
		}
		if (!r->is_textual()) {
			yyerror("Cannot do '.' since item to right is not textual.");
			return e;
		}
#endif
		e->left = l;
		e->right = r;
		e->the_type = text_dot;
		return e;
	}
	~enode() {
		// BUG -- should clear up space for e.g. filename
		if (textual_value) delete [] textual_value;
		if (variable_name) free(variable_name);
	}

	scalar eval_as_text() {
		//printf("%s:%d In eval_as_text.  the_type= %d ...\n",__FILE__,__LINE__,the_type);
		static scalar rval;
		switch (the_type) {
		case question:
		{
			bool test = middle->eval().is_numeric() ? true : false;
			if (test) {
				if (left->eval().is_numeric()) {
					rval.set_numeric(left->eval().get_numeric());
				} else {
					rval.set_text(left->eval().get_text());
				}
			} else {
				if (right->eval().is_numeric()) {
					rval.set_numeric(right->eval().get_numeric());
				} else {
					rval.set_text(right->eval().get_text());
				}
			}
		}
		break;

		case match_a_string: {
			printf("\n\nERROR/BUG eval_as_text()/match_a_string textually.\n\n");
			rval.set_text("0");
		}
		break;

		case change_a_string: {
			printf("\n\nERROR/BUG eval_as_text()/change_a_string textually.\n\n");
			rval.set_text("0");
		}
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
		case text_chomp:
			yyerror("internal error - trying to evaluate 'text_chomp' in a textual context\n");
			break;

		case assign_scalar:
			yyerror("INTERNAL error in eval_as_text()\n");
			break;
		case resize_vector: {	// eval()
			yyerror("INTERNAL error in eval_as_text() -- cannot resize vector\n");
		}
		break;
		case assign_vector:
			if (right->is_textual()) {
				string value;
				if (assignment_op == 0 /* "=" */) {
					value = interpolate_var_in_string(right->eval().get_text());
					put_var_vector_text_at(variable_position_vector, int(left->eval().get_numeric()), value.c_str());
					rval.set_text(value.c_str());
				} else if (assignment_op == 6 /* "=~" */) {
					printf("NOTE: cannot yet hande =~ operator\n");
					rval.set_text("cannot yet hande =~ operator");
				} else {
					yyerror("Illegal assignment operator for text");
				}
			} else {
				yyerror("Internal error: trying to assign non-text to text.");
			}
			break;
		case text:
			if (text_from_file) {
				extern vector<FILE*> file_pointer;
				FILE *fp = file_pointer[which_handle];
				char buffer[1000]; // BUG -- size!
				if (!feof(fp))
					fgets(buffer, 999, fp);
				if (feof(fp))
					buffer[0] = '\0';
				if (text_to_underline)
					put_var_scalar_text_at(0, buffer);
				rval.set_text(buffer);
			} else {
				rval.set_text(textual_value);
			}
			break;
		case pattern_variable: {
			extern vector<var> pattern_match;
			if (which_handle >= int(pattern_match.size())) {
				rval.set_text("");
			} else {
				std::string s_tmp;
				pattern_match[which_handle].get_text_scalar(s_tmp);
				rval.set_text(s_tmp.c_str());
			}
		}
		break;
		case variable: {
			if (scalar_vars[variable_position_scalar].get_at_index(0).is_numeric()) {
				char *msg = new char [100];
				sprintf(msg, "%.14g", scalar_vars[variable_position_scalar].get_numeric_scalar());
				rval.set_text(msg);
			} else {
				string tmp;
				scalar_vars[variable_position_scalar].get_text_scalar(tmp);
				rval.set_text(tmp.c_str());
			}
		}
		break;
		case variable_at_i: {
			int index = int(left->eval().get_numeric());
			if (vector_vars[variable_position_vector].get_at_index(index).is_numeric()) {
				char msg[100];
				sprintf(msg, "%.14g", vector_vars[variable_position_vector].get_numeric_vector(index));
				rval.set_text(msg);
			} else {
				rval.set_text(vector_vars[variable_position_vector].get_text_vector(index));
			}
		}
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
				if (loc == NULL) {
					rval.set_numeric(-1.0);
				} else {
					rval.set_numeric(double(int(loc - str)));
				}
			}
			free(str);
			free(substr);
		}
		break;
		case text_dot: {
			string rr(left->eval_as_text().get_text());
			rr += right->eval_as_text().get_text();
			rval.set_text(rr.c_str());
			//printf("eval_as_text().text_dot %s:%d\n",__FILE__,__LINE__);
		}
		break;
		case text_sprintf: {	// called as text
			//printf("DEBUG: executing text_sprintf as text ...\n");
			char* do_sprintf(const wnode* list);
			rval.set_text(do_sprintf(sub_arg_list));
		}
		break;
		case text_substr: {
			string rr(left->eval_as_text().get_text());
			int length = rr.length();
			int offset = int(right->eval().get_numeric());
			if (offset < 0)
				offset += length;
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
				rval.set_text("");
			}
		}
		break;
		default: {		// It's numeric, so write to buffer
			char msg[100];
			scalar the_val = eval();
			if (the_val.is_numeric()) {
				sprintf(msg, "%.14g", the_val.get_numeric());
				rval.set_text(msg);
				//printf("Default valu '%s' used (numeric %d)\n",msg,the_val.is_numeric());
			} else {
				rval.set_text(the_val.get_text());
				//printf("TEXT IS  '%s'\n",the_val.get_text());
			}
		}
		}
		return rval;
	} // eval_as_text

	// ***** EVALULATE AS A NUMBER *****
	scalar eval();		// defined in parser.y as of 98-may-3
	char *get_vname()	            { return variable_name; }
	int  get_variable_position_scalar() { return variable_position_scalar; }
	int  get_variable_position_vector() { return variable_position_vector; }
	type get_type()	                    { return the_type; }
	enode *get_left()	            { return left; } // used by variable_at_i
	bool is_textual() {
		//printf("%s:%d is_textual call. the_type=%d textual_value='%s' (CODES: text=%d)\n",__FILE__,__LINE__,the_type,textual_value,text);
		if (the_type == variable) {
			//printf("%s:%d is_textual call...variable\n",__FILE__,__LINE__);
			if (!scalar_vars[variable_position_scalar].get_at_index(0).is_numeric()) {
				return true;
			} else {
				return false;
			}
		} else if (the_type == text
			   || the_type == text_dot 
			   || the_type == text_substr 
			   || the_type == text_sprintf
			   || the_type == text_chop
			   || the_type == text_glob) {
			//printf("%s:%d is_textual call...text\n",__FILE__,__LINE__);
			return true;
		} else if (the_type == question) {
			//printf("%s:%d is_textual call...question\n",__FILE__,__LINE__);
			bool test = middle->eval().is_numeric() ? true : false;
			if (test) {
				//printf("will return value %d\n", !left->eval().is_numeric());
				return !left->eval().is_numeric();
			} else {
				//printf("will return value %d\n", !right->eval().is_numeric());
				return !right->eval().is_numeric();
			}
		} else if (the_type == variable_at_i){
			//printf("%s:%d is_textual call...variable_at_i\n",__FILE__,__LINE__);
			int index = int(left->eval().get_numeric());
			//printf("parser.h::is_textual() ... variable_at_i   index = %d...\n", index);
			if (!vector_vars[variable_position_vector].get_at_index(index).is_numeric()) {
				//printf("returning true\n");
				return true;
			} else {
				//printf("returning false\n");
				return false;
			}
		} else {
			//printf("%s:%d is_textual call...NOT IN LIST\n",__FILE__,__LINE__);
			return false;
		}
	}
private:			/* of enode */
#if 0                           // causes segfaults
	regex_t *regex;
	regmatch_t *regex_matchptr;
#endif
	bool text_to_underline;
	bool text_from_file;
	char file_test_flag;	// for test_a_file
	unsigned int assignment_op;	// 0 1 2 3 4 5 6 for = += -= *= /= **= =~
	char *variable_name;	/* for variable (also flag in string_match) */
	int which_handle;		// index for file_handle, name, pointer (-1 if missing)
	int variable_position_scalar; // -1 if not existent yet
	int variable_position_vector; // -1 if not existent yet
	double constant_numerical_value;	/* for numerical scalar */
	char *textual_value;	/* for quoted text of various types */
	type the_type;
	enode *left;		/* left */
	enode *right;		/* right */
	enode *middle;		// used by substr, text_index, func of 3 thing
	enode *extra;		// used by func of 4 things
	int sub_index;
	wnode *sub_arg_list;	// used several ways
	// I think the below should be a union, to save space
	double (*func_of_0)(void);	/* function */
	double (*func_of_1)(double); /* function */
	double (*func_of_2)(double, double); /* function */
	double (*func_of_3)(double, double, double); /* function */
	double (*func_of_4)(double, double, double, double); /* function */
	double (*func_of_0_text)();	/* function */
	double (*func_of_1_text)(const char*);  /* function */
	double (*func_of_2_text)(const char*, const char*);  /* function */
	const char *file_name;	// for 'die' and 'warn'
	unsigned int file_linenumber; // for 'die' and 'warn'
};				// enode

/*
 * cmdnode -- command node
 */
struct cmdnode {		/* cmd node */
	// type_val is value of 'type' for internal cmds; for new cmds
	// it is positive.  NOTE: see parser.y:name_of_cmd(), which
	// MUST be kept up-to-date to match the below.
	enum type_val {blank		 = -1,
		       unknown		 = -2,
		       assign_text_vector    = -3,
		       last_block		 = -4,
		       next_block		 = -5,
		       C_command             = -6,
		       delete_var_list       = -7,
		       do_while_block	 = -8,
		       my_var_list           = -9,
		       expression            = -10,
		       for_block		 = -11,
		       foreach_block         = -12,
		       if_block              = -13,
		       user_cmd     	 = -14,
		       new_var               = -15,
		       print                 = -16,
		       printf                = -17,
		       proj    		 = -18,
		       query		 = -19,
		       read_var_list	 = -20,
		       return_block  	 = -21,
		       skip                  = -22,
		       split                 = -23,
		       sub_block             = -24,
		       while_block		 = -25,
		       chop                  = -26,
		       read_columns          = -27,
		       read_grid_data        = -28,
		       read_grid_x           = -29,
		       read_grid_y           = -30,
		       package               = -31,
		       use                   = -32
	};                          // !!!CROSS_REFERENCE to parser.y:name_of_cmd()
	int type;
	int array_index;		/* for 'split', 'foreach', etc */
	int handle_index;		/* for 'print' */
	int the_cmd_index;		/* which 'cmd' if is a user_cmd */
	char *file_name;		/* pointer to filename */
	unsigned line;		/* line in file */
	wnode* first_wnode;		/* used variously */
	bool (*c_function)(void);
	cmdnode* left;
	cmdnode* right;
	cmdnode* next;
	enode* start;
	enode* test;
	enode* each;
	char* name;			// in 'foreach' loops only
	enode* column_place[5];	// for xyzuv (r, theta only if code)
};



/* Various functions */
enode* logical_negation(enode *e);
wnode* append_right(wnode* list, wnode* n);
cmdnode* append_right(cmdnode* list, cmdnode* n);
wnode* new_wnode_word(const char *s);
wnode* new_wnode_string(const char *s);
wnode* new_wnode_vname(const char *s);
wnode* new_wnode_expr(enode *new_expr);
wnode* new_wnode_columns_spec(const char *col_name, enode *col_expr);

cmdnode* new_user_cmd(wnode* words, int which_cmd, unsigned int num_matching_cmds, wnode* expressions);
cmdnode* new_cmdnode();
cmdnode* new_cmdnode(wnode *first_word);
//cmdnode* new_cmdnode(bool (fcn)());
cmdnode* new_cmdnode(bool (*fcn)());
cmdnode* new_if_block(enode* test, cmdnode* true_part, cmdnode* false_part);
cmdnode* new_do_while_block(enode* test, cmdnode* block);
cmdnode* new_while_block(enode* test, cmdnode* block);
cmdnode* new_for_block(enode* start, enode* test, enode* each, cmdnode* block);
cmdnode* new_foreach_block_list(const char *var, wnode* list, cmdnode* block);
cmdnode* new_foreach_block_array(const char *var, const char *array_name, cmdnode* block);
cmdnode* new_newcommand(cmdnode* block, wnode *args);
cmdnode* new_read_columns_cmd(const char *handle, wnode* columns_spec_list, enode* rows);
cmdnode* new_read_grid_cmd(const char *item, const char *handle, wnode *args);

cmd_eval_code evaluate_cmd(cmdnode *c);

int find_type(cmdnode *c);
void push_cmdfile(char *n, FILE* fp);
int pop_cmdfile();             // returns length of remaining stack

#endif /* PARSER_H_ */
