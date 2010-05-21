// -*- mode: c++ -*-
  
// Macros to save time
#if !defined(_macro_h_)
#define _macro_h_

#define NEAREST_INT(d) (int(floor((d) + 0.5)))

#define INTERNAL_ERROR(msg) fatal_error(__FILE__, __LINE__, (msg));

#define OUT_OF_MEMORY fatal_error(__FILE__, __LINE__, " out of memory");

#define GRI_ABS(x) ((x) < 0.0 ? (-(x)) : (x))

// If 'condition' is untrue, perform action and return false
#define Require(condition, action_if_not) {			\
    if (!(condition)) {						\
        action_if_not;						\
        return false;						\
    }								\
}
// If 'condition' is untrue, perform action and return void
#define Require2(condition, action_if_not) {			\
    if (!(condition)) {						\
        action_if_not;						\
        return;							\
    }								\
}
#define gre_assert(condition, message) {\
    if (!(condition)) {\
	fprintf(stderr, "INTERNAL ERROR AT %s:%d: %s\n", __FILE__, __LINE__, (message));\
	gre_exit(1);\
    }\
}

#define	ShowStr(x) {						\
    gr_textput ((x));						\
}

#define strEQ(s1, s2) (!strcmp((s1),(s2)))
#define strNE(s1, s2) (strcmp((s1),(s2)))

// Return error if item (indicated by index 0, 1, ...) isn't numeric.
#define expecting_numeric(syntax_so_far, index) {\
    if (!cmd_parameter_is_n((index))) {\
        err("\\", (syntax_so_far), " expecting numerical item but got `",\
            cmd_parameter_s((index)), "'", "\\");\
        return false;\
    }\
}

// Die if cmd parameter at given index is non-string, or if it's
// a string but not equal to the desired keyword.
#define expecting_the_keyword(syntax_so_far, index, keyword) { \
    if (cmd_parameter_is_n((index))) { \
        err("\\", (syntax_so_far), " expecting keyword `", (keyword), "' but got numerical expression ",\
	    dtoa(cmd_parameter_n((index))), " instead", "\\"); \
        return false; \
    } \
    if (strcmp(cmd_parameter_s((index)), keyword)) { \
        err("\\", (syntax_so_far), " expecting keyword `", (keyword), "' but got `",\
	    cmd_parameter_s((index)), "' instead", "\\"); \
        return false; \
    } \
}

// Die if cmd parameter at given index is non-string (use this for cases
// where several different keywords would do).
#define expecting_a_keyword(syntax_so_far, index, keyword) { \
    if (cmd_parameter_is_n((index))) { \
        err("\\", (syntax_so_far), " expecting keyword ", (keyword), " but got numerical expression ",\
	    dtoa(cmd_parameter_n((index))), " instead", "\\"); \
        return false; \
    } \
}

// Die if cmd parameter at given index is non-numerical.
#define expecting_an_exp(syntax_so_far, index) { \
    if (!cmd_parameter_is_n((index))) { \
        err("\\", (syntax_so_far), " expecting numerical expression but got ",\
	    cmd_parameter_s((index)), " instead", "\\"); \
        return false; \
    } \
}

// Take care of the fact that the standard c++ library CHANGED the name
// of the string::remove to string::erase, as evidenced by the change
// in g++ from versions 2.7.x to 2.8.x (early 1998).

#if defined(HAVE_OLD_STRING)
#define STRING_ERASE remove
#define STRING_NPOS NPOS
#else
#define STRING_ERASE erase
#define STRING_NPOS string::npos
#endif

#endif // !defined(_macro_h_)
