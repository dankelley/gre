#include <string>
#include <stdio.h>
#include <ctype.h>
#include "private.h"
#include "extern.h"
#include "debug.h"
#include "parser.h"		// for cmdnode


using namespace std;

static void
nonopen_filehandle_error(const char *cmd, int which_handle, const char*file, int line)
{
	string msg("`");
	msg += cmd;
	msg += "' filehandle `";
	msg += file_handle[which_handle].c_str();
	msg += "' is not open";
	if (file == NULL)
		err(msg.c_str());
	else
		fatal_error(file, line, msg.c_str());
}



double 
rewind_data_file(int which_handle)
{
	if (file_pointer[which_handle] == NULL) {
		nonopen_filehandle_error("rewind", which_handle, NULL, 0);
	} else {
		rewind(file_pointer[which_handle]);
		return 1.0;
	}
	return 0.0;
}

// return index of file, if it exists, or -1 otherwise.
int
file_exists(const char* n)
{
	for (unsigned int i = 0; i < file_name.size(); i++) {
		//printf("[%s]  [%s]  %d\n", n, file_name[i].c_str(), n == file_name[i]);
		if (n == file_name[i])
			return i;
	}
	return -1;
}

double
open_data_file(const char *filename, int which_handle)
{
	//printf("DEBUG:%s:%d open_data_file(name= '%s', handle-number= %d)\n", __FILE__,__LINE__,filename, which_handle);

	string fname(filename);
	// trim starting or ending whitespace from filename
	while (isspace(fname[0]))
		fname.STRING_ERASE(0, 1);
	while (isspace(fname[fname.size() - 1]))
		fname.STRING_ERASE(fname.size() - 1, 1);

	FILE *fp;
	extern vector<bool> file_input;
	unsigned int len = strlen(filename);

	// Check, before opening, whether it's 
	//   (a) compressed with unix-compress
	//   (b) compressed with GNU gzip
	//   (c) a URL
	// and if any of the above, open a pipe.  Otherwise, open
	// a normal file.
	if (len > 2 && !strncmp(filename + len - 2, ".Z", 2)) {
		string cmd = "uncompress -c ";
		cmd += filename;
		cmd += "> ";
		string tmp_name = tmp_file_name();
		cmd += tmp_name;
		call_the_OS(cmd.c_str());
		file_input.push_back(true);
		file_name.push_back(filename);
		fp = fopen(tmp_name.c_str(), "r");
		fname.STRING_ERASE(fname.size() - 2, 2);
		//printf("IT IS COMPRESSED.  fname [%s]\n",fname.c_str());
	} else if (len > 3 && !strncmp(filename + len - 3, ".gz", 3)) {
		string cmd = "gunzip -c ";
		cmd += filename;
		cmd += "> ";
		string tmp_name = tmp_file_name();
		cmd += tmp_name;
		call_the_OS(cmd.c_str());
		file_input.push_back(true);
		file_name.push_back(filename);
		fp = fopen(tmp_name.c_str(), "r");
		fname.STRING_ERASE(fname.size() - 3, 3);
		//printf("IT IS GZIPPED.  fname [%s]\n",fname.c_str());
	} else if (!strncmp(filename, "http://", 7)
		   || !strncmp(filename, "file://", 7)
		   || !strncmp(filename, "ftp://", 6)) {
		// See if URL
		string tmp_name = tmp_file_name();
		pipe_tmpfile.push_back(tmp_name);
		string cmd = "lynx -dump '";
		cmd += filename;
		cmd += "' > ";
		cmd += tmp_name;
		call_the_OS(cmd.c_str());
		file_input.push_back(true);
		file_name.push_back(filename);
		fp = fopen(tmp_name.c_str(), "r");
	} else {
		// ... check to see if it's a pipe ...
		//printf("some kinda normal\n");
		if (fname[fname.size() - 1] == '|') {	// pipe
			fname.STRING_ERASE(fname.size() - 1, 1);
			string cmd = fname; // not really a filename, of course
			string tmp_name = tmp_file_name();
			pipe_tmpfile.push_back(tmp_name);
			cmd += " > ";
			cmd += tmp_name;
			call_the_OS(cmd.c_str());
			file_input.push_back(true);
			file_name.push_back(fname.c_str()); // weird
			fp = fopen(tmp_name.c_str(), "r");
		} else {
			// ... normal file -- see if input, output, or i/o ...
			int existing;
			if (fname[0] == '<') {
				existing = file_exists(filename + 1);
				file_input.push_back(true);
				file_name.push_back(filename + 1);
				fp = fopen(filename + 1, "r");
			} else if (fname[0] == '>') {
				existing = file_exists(filename + 1);
				file_input.push_back(false);
				file_name.push_back(filename + 1);
				fp = fopen(filename + 1, "w");
			} else {
				existing = file_exists(filename);
				file_input.push_back(true); // default
				file_name.push_back(filename);
				fp = fopen(filename, "r");
			}
		}
	}
	extern vector<FILE*> file_pointer;
	if (!fp) {
		if (_gre_debug & DEBUG_OS_CALLS) {
			string msg("`open' cannot open file `");
			msg += filename;
			msg += "'\n";
			err(msg.c_str());
		}
		return 0.0;
	}
    
	file_pointer[which_handle] = fp;

	return (fp != NULL ? 1.0 : 0.0);
}

bool
execute_read_columns(cmdnode *c)
{
	int rows;
	if (c->start == NULL)
		rows = -1;		// means all columns
	else 
		rows = int(c->start->eval().get_numeric());

	FILE *fp = file_pointer[c->handle_index];
	if (fp == NULL) {
		nonopen_filehandle_error("read", c->handle_index, c->file_name, c->line);
	} else {
		int xind = get_vector_var_index("$_x");
		int yind = get_vector_var_index("$_y");
		int zind = get_vector_var_index("$_z");
		int uind = get_vector_var_index("$_u");
		int vind = get_vector_var_index("$_v");
		clear_var_vector_at(xind);
		clear_var_vector_at(yind);
		clear_var_vector_at(zind);
		clear_var_vector_at(uind);
		clear_var_vector_at(vind);
		int xcol = -1, ycol = -1, zcol = -1, ucol = -1, vcol = -1;
		int maxcol = -1; // us to see if dataline long enough
		int line_number = 0;
		if (c->column_place[0]) {
			xcol = NEAREST_INT(c->column_place[0]->eval().get_numeric());
			if (xcol > maxcol)			maxcol = xcol;
		}
		if (c->column_place[1]) {
			ycol = NEAREST_INT(c->column_place[1]->eval().get_numeric());
			if (ycol > maxcol)			maxcol = ycol;
		}
		if (c->column_place[2]) {
			zcol = NEAREST_INT(c->column_place[2]->eval().get_numeric());
			if (zcol > maxcol)			maxcol = zcol;
		}
		if (c->column_place[3]) {
			ucol = NEAREST_INT(c->column_place[3]->eval().get_numeric());
			if (ucol > maxcol)			maxcol = ucol;
		}
		if (c->column_place[4]) {
			vcol = NEAREST_INT(c->column_place[4]->eval().get_numeric());
			if (vcol > maxcol)			maxcol = vcol;
		}
		char buf[1024];	// BUG: fixed size
		char *words[100]; // BUG: fixed size
		int row = 0;
		while (rows < 0 || row < rows) {
			fgets(buf, 1024, fp);
			if (feof(fp))
				break;
			line_number++;
			unsigned int len = strlen(buf);
			gre_assert(buf[-1 + len] == '\n', "buffer (size 1024 bytes) is too small.\n");
			if (len == 1) {
				//printf("len=1 so breaking now\n");
				break;
			}
			//printf("row %d: buf = %s", row, buf);
			int nw;
			chop_into_words(buf, words, &nw, 100);
			if (!nw) {
				//printf("nw=0 so breaking now\n");
				break;
			}
			if (nw < maxcol) {
				char msg[256];
				sprintf(msg, "file %s:%d has too few columns (has %d; want %d)\n", 
					file_handle[c->handle_index].c_str(),
					line_number,
					nw, maxcol);
				fatal_error(c->file_name, c->line, msg);
			}

			//printf(" ... this is %d words '%s' '%s'\n", nw, words[0],words[1]);
			//printf("xcol=%d ycol=%d\n",xcol,ycol);
			double the_value;
			double missing = gr_currentmissingvalue();
			if (xcol > 0) {
				if (getdnum(words[xcol-1], &the_value))
					put_var_vector_numeric_at(xind, row, the_value);
				else
					put_var_vector_numeric_at(xind, row, missing); 
			}
			if (ycol > 0) {
				if (getdnum(words[ycol-1], &the_value))
					put_var_vector_numeric_at(yind, row, the_value);
				else
					put_var_vector_numeric_at(yind, row, missing); 
			}
			if (zcol > 0) {
				if (getdnum(words[zcol-1], &the_value))
					put_var_vector_numeric_at(zind, row, the_value);
				else
					put_var_vector_numeric_at(zind, row, missing); 
			}
			if (ucol > 0) {
				if (getdnum(words[ucol-1], &the_value))
					put_var_vector_numeric_at(uind, row, the_value);
				else
					put_var_vector_numeric_at(uind, row, missing); 
			}
			if (vcol > 0) {
				if (getdnum(words[vcol-1], &the_value))
					put_var_vector_numeric_at(vind, row, the_value);
				else
					put_var_vector_numeric_at(vind, row, missing); 
			}
			row++;
		}
		put_var("$_num_col_data", double(row));

#if 0                           // 990117 - now do in 'scales_defined()'
		extern bool _columns_exist;
		extern bool _xscale_exists;
		extern bool _yscale_exists;
		if (xcol > 0) {
			_columns_exist = true;
			if (!_xscale_exists) {
				create_x_scale();
			}
		}
		if (ycol > 0) {
			_columns_exist = true;
			if (!_yscale_exists) {
				create_y_scale();
			}
		}
#endif // 990117

	}
	return true;
}

bool
execute_read_var_list(cmdnode *c)
{
	if (!file_input[c->handle_index]) {
		string msg("File with handle `");
		msg += file_handle[c->handle_index];
		msg += "' is for output, not input\n";
		fatal_error(c->file_name, c->line, msg.c_str());
		return false;		// never reached
	}
	FILE *fp = file_pointer[c->handle_index];
	if (fp == NULL) {
		nonopen_filehandle_error("read", c->handle_index, c->file_name, c->line);
		return false;		// never reached
	}
	wnode *n = c->first_wnode;
	while (n != NULL) {
		double tmp;
		fscanf(fp, "%lf", &tmp);
		if (feof(fp)) {
			put_var("$_eof", 1);
			put_var(n->w, 0.0);	/* should be MISSING-VALUE I think*/
			break;	/* give up on this list*/
		} else {
			put_var(n->w, tmp);
		}
		n = n->next;
	}
	return true;
}

bool
execute_read_grid_data(cmdnode *c)
{
	int rows, cols;
	wnode *w = c->first_wnode;
	if (w == NULL) {
		rows = _ymatrix.size();
		cols = _xmatrix.size();
	} else {
		rows = int(floor(0.5 + w->e->eval().get_numeric()));
		w = w->next;
		cols = int(floor(0.5 + w->e->eval().get_numeric()));
		if (rows < 1)
			fatal_error(c->file_name, c->line, "`read grid data' cannot have $rows < 1");
		if (cols < 1)
			fatal_error(c->file_name, c->line, "`read grid data' cannot have $cols < 1");
		if (_ymatrix.size() != 0 && int(_ymatrix.size()) != rows) {
			char msg[100];
			sprintf(msg, "`read grid data' given rows=%d but have pre-existing rows=%d", rows, _ymatrix.size());
			fatal_error(c->file_name, c->line, msg);
		}
		if (_xmatrix.size() != 0 && int(_xmatrix.size()) != cols) {
			char msg[100];
			sprintf(msg, "`read grid data' given cols=%d but have pre-existing cols=%d", cols, _xmatrix.size());
			fatal_error(c->file_name, c->line, msg);
		}
	}
	//printf("In 'read grid data' fname '%s' r=%d c=%d\n", file_handle[c->handle_index].c_str(), rows, cols);
	FILE *fp = file_pointer[c->handle_index];
	if (!fp) {
		fatal_error(c->file_name, c->line, "`read grid data' has no open file");
		return false;
	}
	if (!allocate_grid_storage(cols, rows)) {
		fatal_error(c->file_name, c->line, "Insufficient space for matrix");
		return false;
	}
	//printf("rows = %d cols = %d\n", rows, cols);
	for (int row = rows - 1; row > -1; row--) {
		for (int col = 0; col < cols; col++) {
			float tmp;
			if (1 != fscanf(fp, "%f", &tmp)) {
				char msg[200];
				sprintf(msg, "ran out of grid data after having read %d items (need %d)\n", 
					col + (rows - 1 - row) * cols, rows * cols);
				fatal_error(c->file_name, c->line, msg);
				return false;
			}
			_f_xy(col, row) = tmp;
			_legit_xy(col, row) = true;
			//printf("  %f into %d %d\n", tmp, col, row);
		}
	}
	// skip any following whitespace
	int cc;
	while ((cc = getc(fp)) == ' ' || cc == '\t')
		;
	// also skip the newline if that's what we stopped on
	if (cc != '\n')
		ungetc(cc, fp);

	//cc = getc(fp); printf("\nNEXT CHAR '%c'\n", cc); ungetc(cc, fp);

	//void extrema_of_matrix(double *min, double *max);
	extrema_of_matrix(&_f_min, &_f_max);	// Figure grid limits
	_grid_exists = true;
	return true;
}

// check for approximately equal adjacent values
static double
vector_repeats(vector<double> &v)
{
	int n = v.size();
	if (n <= 1)
		return 0.0;
	double range = fabs(v[0] - v[n - 1]) / 1.0e10;
	if (range == 0.0)
		return v[0];
	for (int i = 1; i < n; i++)
		if (fabs(v[i] - v[i - 1]) < range)
			return v[i];
	return 0.0;
}

bool
execute_read_grid_xy(char xory, cmdnode *c)
{
	unsigned int existing_size = (xory == 'x') ? _xmatrix.size() : _ymatrix.size();
	int number_to_read = -1;
	bool number_specified = false;
	wnode *w = c->first_wnode;
	if (w != NULL) {
		number_to_read = int(floor(0.5 + w->e->eval().get_numeric()));
		if (number_to_read < 1) {
			fatal_error(c->file_name, c->line, "`read grid x|y' cannot have $num < 1");
			return false;		// never reached
		}
		if (_grid_exists && (unsigned int)number_to_read != existing_size) {
			sprintf(_errorMsg, "Grid width %d disagrees with existing x-grid (%d); first `delete grid'", number_to_read, existing_size);
			fatal_error(_errorMsg);
			return false;	// never reached
		}
		number_specified = true;
	}
	//printf("In 'read grid x' fname '%s' num=%d\n", file_handle[c->handle_index].c_str(), number_to_read);
	if (xory == 'x')
		_xmatrix.erase(_xmatrix.begin(), _xmatrix.end());
	else
		_ymatrix.erase(_ymatrix.begin(), _ymatrix.end());
	FILE *fp = file_pointer[c->handle_index];
	if (fp == NULL) {
		nonopen_filehandle_error("read", c->handle_index, c->file_name, c->line);
		return false;		// never reached
	}
	int i = 0;
	GriString line;
	while (!number_specified || i < number_to_read) {
		if (!line.line_from_FILE(fp))
			break;
		double tmp;
		if (1 != sscanf(line.c_str(), "%lf", &tmp))
			break;
		if (xory == 'x')
			_xmatrix.push_back(tmp);
		else
			_ymatrix.push_back(tmp);
		i++;
	}
	// Swap y so it reads as on page
	if (xory == 'y') {
		unsigned int n = _ymatrix.size();
		for (unsigned int i = 0; i < n / 2; i++) {
			double tmp = _ymatrix[i];
			_ymatrix[i] = _ymatrix[n-i-1];
			_ymatrix[n-i-1] = tmp;
		}
	}
	if (number_specified && i != number_to_read) {
		if (xory == 'x')
			sprintf(_errorMsg, "only found %d x-grid data, not %d as suggested\n", i, number_to_read);
		else
			sprintf(_errorMsg, "only found %d y-grid data, not %d as suggested\n", i, number_to_read);
		runtime_warning(c->file_name, c->line, _errorMsg);
	}
	if (i <= 0) {
		err("`read grid x|y' found no data");
		return false;
	}
	// All done reading.
	if (xory == 'x') {
		if (!_xscale_exists)
			if (!create_x_scale()) {
				runtime_warning(c->file_name, c->line, "Problem autoscaling -- no data or all x values equal");
			}
		_xgrid_exists = true;
		if (_xmatrix[1] > _xmatrix[0])
			_xgrid_increasing = true;
		else
			_xgrid_increasing = false;
		double repeat;
		if (0 != (repeat = vector_repeats(_xmatrix))) {
			sprintf(_errorMsg, "x-grid has some adjacent values equal (e.g., value %f)", repeat);
			runtime_warning(c->file_name, c->line, _errorMsg);
		}
	} else {
		if (!_yscale_exists)
			if (!create_y_scale()) {
				runtime_warning(c->file_name, c->line, "Problem autoscaling -- no data or all y values equal");
			}
		_ygrid_exists = true;
		if (_ymatrix[1] > _ymatrix[0])
			_ygrid_increasing = true;
		else
			_ygrid_increasing = false;
		double repeat;
		if (0 != (repeat = vector_repeats(_ymatrix))) {
			sprintf(_errorMsg, "y-grid has some adjacent values equal (e.g., value %f)", repeat);
			runtime_warning(c->file_name, c->line, _errorMsg);
		}
	}
	return true;
}
