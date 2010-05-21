//#include <string>
#include "gr.h"
#include "extern.h"
#include "private.h"
#include "superus.h"

// 98-aug-2: Do not close files since exit() will do; egcs seg-faults if close multiply-opened files.
// #define CLOSE_THE_FILES

using namespace std;

static void
clean_io_files()
{
	extern vector<string> file_handle;
	extern vector<string> file_name;
	extern vector<bool> file_input;
	extern vector<FILE*> file_pointer;
	unsigned int i;
#if defined(CLOSE_THE_FILES)
	printf("about to close %d i/o files...\n", file_handle.size());
	for (i = 0; i < file_handle.size(); i++) {
		printf(" about to close number %d   %x\n", i, int(file_pointer[i]));
		printf("   handle '%s'\n", file_handle[i].c_str());
		printf("   name   '%s'\n", file_name[i].c_str());
		if (file_pointer[i] != NULL && file_pointer[i] != stdout && file_pointer[i] != stdin && file_pointer[i] != stderr) {
			printf("   really, about to close %x\n", int(file_pointer[i]));
			int status = fclose(file_pointer[i]);
			printf("  status of the fclose = %d\n", status);
			if (status == EOF) {
				printf("ERROR closing file `%s' (handle %s)\n", file_name[i].c_str(), file_handle[i].c_str());
			}
		} else {
			printf("       ... do not close, since builtin\n");
		}
		printf("       done\n");
	}
#endif
	file_input.erase(file_input.begin(), file_input.end());
	file_handle.erase(file_handle.begin(), file_handle.end());
	file_pointer.erase(file_pointer.begin(), file_pointer.end());
	file_name.erase(file_name.begin(), file_name.end());
	for (i = 0; i < pipe_tmpfile.size(); i++)
		call_OS_unlink(pipe_tmpfile[i].c_str());
	pipe_tmpfile.erase(pipe_tmpfile.begin(), pipe_tmpfile.end());
}

void
end_up()
{
	extern bool postscript_clipping_on;
#if 0
	if (_chatty > 0) {
		display_unused_var();
		display_unused_syn();
	}
#endif
	clean_io_files();
	extern FILE *_grPS;
	if (postscript_clipping_on)
		fprintf(_grPS, "Q %% turn clipping off (user forgot to)\n");
	close_data_files();
	gr_end("!");
	if (!_drawingstarted) {
		extern char* ps_file_name;
		call_OS_unlink(ps_file_name);
	}
	warning("\\\\");
}
