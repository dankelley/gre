// -*- mode: c++ -*-

// Sample use:
//        if (_gre_debug & DEBUG_OS_CALLS) { ... }
//
//       `1' -> print information about commands as they are parsed
//
//       `2' -> print info when values are assigned to variables
//
//       `4' -> print info when the OS is used, e.g. to create temporary
//              files
//
//       `8' -> while parsing, print any comments that start with ##
//
//     `128' -> for author only -- usage changes without warning
//
//     `256' -> output 'yacc' info during parsing; only the `gre'
//              developer will find this useful.

#if !defined(_debug_)

#define _debug_
#define DEBUG_TRACE         ((unsigned int)   1)
#define DEBUG_VARIABLES     ((unsigned int)   2)
#define DEBUG_OS_CALLS      ((unsigned int)   4)
#define DEBUG_PARSE_COMMENT ((unsigned int)   8)
#define DEBUG_TEMPORARY     ((unsigned int) 128)
#define DEBUG_YACC_PARSING  ((unsigned int) 256)

extern unsigned int _gre_debug;	// defined in parser.y
#endif				/* _debug_ */
