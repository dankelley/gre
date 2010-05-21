// define some filenames

#if !defined(_files_h_)
#define _files_h_

#define GRE_TMP_FILE                    "TMP.GRE"
#define GRERC_FILE              	".grerc"
#define GRE_COMMANDS_FILE       	"gre.cmd"

#if !defined(DEFAULT_GRE_DIR)
#if defined(_MSC_VER)
#define DEFAULT_GRE_DIR            	"C:\\gre"
#else
#define DEFAULT_GRE_DIR            	"/opt/gre/lib"
#endif
#endif

#if defined(HAVE_LIBNETCDF)
#include        <netcdf.h>
#endif

#endif				// _files_h_
