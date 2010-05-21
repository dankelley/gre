#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "extern.h"
#include "private.h"
#include "parser.h"
#include "image_ex.h"

using namespace std;

#define GRID_MAX_WIDTH_TO_SHOW 32
#define IMAGE_MAX_WIDTH_TO_SHOW 32
#define MISSING " ---missing--- "


static const string&
do_sprintf_internal(const string& fmt, const wnode* list)
{
	bool in_math = false;
	//if (list == NULL) printf("DEBUG list is EMPTY\n");
	//printf("list is (integer) %d\n", (int)list);
	char buffer[100];
	static string result;	// avoid getting new storage
	result = "";
	const char *f = fmt.c_str();
	char *this_fmt = new char[fmt.size()];
	unsigned int this_fmt_len = 0;
	while (*f) {
		if (*f == '%') {
			this_fmt[this_fmt_len++] = *f++;
			// %f %g %s %d
			// %NN.MMf %NN.MMg %NN.MMs %NN.MMd
			while (*f) {
				if (isdigit(*f) || *f == '.') {
					this_fmt[this_fmt_len++] = *f++;
					this_fmt[this_fmt_len] = '\0';
					//printf("\n\tDEBUG growing this_fmt is now '%s'\n",this_fmt);
				} else if (*f == 's') {
					this_fmt[this_fmt_len++] = *f++;
					this_fmt[this_fmt_len] = '\0';
					//printf("\n\tDEBUG finished this_fmt is now '%s'\n",this_fmt);
					if (list != NULL) {
						sprintf(buffer, this_fmt, interpolate_var_in_string(list->e->eval_as_text().get_text()));
						result.append(buffer);
						list = list->next;
					} else {
						sprintf(buffer, "%s", this_fmt);
						result.append(buffer);
					}
					break;
				} else if (*f == 'd') {
					this_fmt[this_fmt_len++] = *f++;
					this_fmt[this_fmt_len] = '\0';
					//printf("\n\tDEBUG finished this_fmt is now '%s'\n",this_fmt);
					if (list != NULL) {
						//printf("\n\tDEBUG FMT '%s' for INT %d\n", this_fmt, int(list->e->eval().get_numeric()));
						sprintf(buffer, this_fmt, int(list->e->eval().get_numeric()));
						result.append(buffer);
						list = list->next;
					} else {
						sprintf(buffer, "%s", this_fmt);
						result.append(buffer);
					}
					break;
				} else if (*f == 'f' || *f == 'g' || *f == 'e') {
					this_fmt[this_fmt_len++] = *f++;
					this_fmt[this_fmt_len] = '\0';
					//printf("\n\tDEBUG finished this_fmt is now '%s'\n",this_fmt);
					if (list != NULL) {
						//printf("\n\tDEBUG FMT '%s' for FLOAT %f\n", this_fmt, list->e->eval().get_numeric());
						sprintf(buffer, this_fmt, list->e->eval().get_numeric());
						result.append(buffer);
						list = list->next;
					} else {
						sprintf(buffer, "%s", this_fmt);
						result.append(buffer);
					}
					break;
				} else {
					// ISN'T IT AN ERROR TO BE HERE??
					//printf("\n\tDEBUG finished this_fmt is now '%s'\n",this_fmt);
					sprintf(buffer, "%c", *f++);
					result.append(buffer);
					break;
				}
			}
			this_fmt_len = 0;
		} else {
			//printf("Got non-format *f of '%c'.  Now in_math is %d\n", *f, in_math);
			if (*f == '\\') {
				//printf("It '%c' is backslash, next is '%c'\n", *f,*(f+1));
				f++;
				if (*f == '(') {
					in_math = true;
					result.append("\\(");
				} else if (*f == ')') {
					in_math = false;
					result.append("\\)");
				} else if (*f == 'n') {
					if (in_math)
						result.append("\\n");
					else
						result.append("\n");
				} else if (*f == 't') {
					if (in_math)
						result.append("\\t");
					else
						result.append("\t");
				} else {
					sprintf(buffer, "\\%c", *f);
					result.append(buffer);
				}
				f++;
			} else {
				sprintf(buffer, "%c", *f++);
				result.append(buffer);
			}
		}
		// printf("\t<%s>\n", result.c_str()); // *** USEFUL FOR DEBUGGING ***
	}
	delete [] this_fmt;
	//printf("do_sprintf_internal has fmt '%s' will return '%s'\n", fmt.c_str(),result.c_str());
	return result;
}

//* Print text, obeying escape codes \t and \n
void
print_buf(FILE *fp, const char *buf)	/* print with newlines decoded*/
{
	unsigned int len = strlen(buf);
	for (unsigned int i = 0; i < len; i++) {
		if (buf[i] == '\\' && buf[i + 1] == 'n') {
			fputc('\n', fp);
			i++;
		} else if (buf[i] == '\\' && buf[i + 1] == 't') {
			fputc('\t', fp);
			i++;
		} else {
			fputc(buf[i], fp);
		}
	}
}

void
print_image()
{
	printf("\
Image properties\n\
================\n");
	printf("  Width: %d    height: %d\n", _image.ras_width, _image.ras_height);
	printf("  Span of possible values:  %f to %f\n", _image0, _image255);
	printf("  Lower-left corner at (%f, %f) on graph\n", _image_llx, _image_lly);
	printf("  Upper-left corner at (%f, %f) on graph\n", _image_urx, _image_ury);
	printf("  Histogram of values:\n\
     Image #          Value           %%   Each * represents 2%% of pixels\n\
     -------   ------------------   ----  ------------------------------\n");
	calculate_image_histogram();
	double sum = 0.0;
	for (unsigned int i = 0; i < 256; i++) {
		sum += _imageHist[i];
		if (i && (!(i % 16) || i == 255)) {
			printf("    %3d->%3d ", i - 16, i);
			printf("%9g->%9g  %5.1f  ",
			       _image0 + (i - 16) * (_image255 - _image0) / 255.0,
			       _image0 + i * (_image255 - _image0) / 255.0,
			       100.0 * sum);
			if (sum > 0.0) {
				unsigned int num = (unsigned int) floor(50.0 * sum);
				for (unsigned int j = 0; j < num; j++) {
					printf("*");
				}
			}
			sum = 0.0;
			printf("\n");
		}
	}
	if (_image.ras_width < IMAGE_MAX_WIDTH_TO_SHOW
	    && _image.ras_height < IMAGE_MAX_WIDTH_TO_SHOW) {
		int j;
		printf("Image is, in terms of internal values from 0 to 255:\n");
		for (j = int(_image.ras_height - 1); j > -1; j--) {
			for (unsigned int i = 0; i < _image.ras_width; i++) {
				if (!*(_imageMask.image + i * _image.ras_height + j)) {
					printf("%03d ", *(_image.image + i * _image.ras_height + j));
				} else {
					printf("*** ");
				}
			}
			printf("\n");
		}
		printf("\n  Image is, in terms of user values:\n");
		float scale = 255.0 / (_image255 - _image0);
		for (j = int(_image.ras_height - 1); j > -1; j--) {
			for (unsigned int i = 0; i < _image.ras_width; i++) {
				if (!*(_imageMask.image + i * _image.ras_height + j)) {
					int val = *(_image.image + i * _image.ras_height + j);
					float val_float;
					if (_imageTransform == NULL)
						val_float = _image0 + float(val) / scale;
					else 
						val_float = _image0 + _imageTransform[val] / scale;
					printf("%10f ", val_float);
				} else {
					printf("********* ");
				}
			}
			printf("\n");
		}
	} else {
		printf("  The image is too big to display its values here.\n");
	}
}

void
print_grid()
{
	if (!_grid_exists && !_xgrid_exists && !_ygrid_exists) {
		ShowStr("No grid data exist yet.\n");
		return;
	}
	char msg[256];
	if (_xgrid_exists) {
		if (_xmatrix.size() < GRID_MAX_WIDTH_TO_SHOW) {
			ShowStr("x-locations of matrix are:\n");
			for (unsigned int i = 0; i < _xmatrix.size(); i++) {
				sprintf(msg, "%f\n", _xmatrix[i]);
				ShowStr(msg);
			}
		} else {
			sprintf(msg, "Grid `x' has %d elements, %g ... %g\n",
				_xmatrix.size(), _xmatrix[0], _xmatrix[_xmatrix.size() - 1]);
			ShowStr(msg);
		}
	} else {
		ShowStr("Grid `x' not defined yet\n");
	}
	if (_ygrid_exists) {
		if (_ymatrix.size() < GRID_MAX_WIDTH_TO_SHOW) {
			ShowStr("y-locations of matrix are:\n");
			unsigned int n = _ymatrix.size();
			for (unsigned int j = n-1; j !=0; j--) {
				sprintf(msg, "%f\n", _ymatrix[j]);
				ShowStr(msg);
			}
		} else {
			sprintf(msg, "Grid `x' has %d elements, %g ... %g\n",
				_ymatrix.size(), _ymatrix[0], _ymatrix[_ymatrix.size() - 1]);
			ShowStr(msg);
		}
	} else {
		ShowStr("Grid `y' not defined yet\n");
	}
	if (!_grid_exists) {
		ShowStr("Grid data don't exist yet\n");
		return;
	}
	if (_xmatrix.size() < GRID_MAX_WIDTH_TO_SHOW
	    && _ymatrix.size() < GRID_MAX_WIDTH_TO_SHOW) {
		if (_f_min == _f_max) {
			if (gr_missing((double) _f_min)) {
				ShowStr("Grid data don't exist yet\n");
				return;
			} else {
				sprintf(msg, "Grid data all equal to %f", _f_min);
				ShowStr(msg);
				return;
			}
		} else {
			ShowStr("Grid data:\n");
			unsigned int j = _ymatrix.size() - 1;
			do {
				for (unsigned int i = 0; i < _xmatrix.size(); i++) {
					if (_legit_xy(i, j) == true) {
						sprintf(msg, "%10f ", _f_xy(i, j));
						ShowStr(msg);
					} else {
						ShowStr("     *     ");
					}
				}
				ShowStr("\n");
			} while (j-- != 0);
		}
	} else {
		ShowStr("Grid `data' matrix is too big to display\n");
	}
	sprintf(msg, 
		"Grid is %d wide and %d tall, with values ranging from %f to %f\n",
		_xmatrix.size(),
		_ymatrix.size(),
		_f_min,
		_f_max);
	ShowStr(msg);
	return;
}
#undef GRID_MAX_WIDTH_TO_SHOW
#undef IMAGE_MAX_WIDTH_TO_SHOW


#define MACRO1(IND, VAR, LEN, NAM) \
    if ((IND)) { \
	VAR = &vector_vars[(IND)]; \
	LEN = 1 + (VAR)->get_end_index(); \
	if (LEN > len) \
	    len = LEN; \
	if (LEN != 0) \
            printf("%15s ", NAM);\
    }

#define MACRO2(VAR, I, LEN)\
    if ((I) < (LEN)) {\
        double v = (VAR)->get_at_index((I)).get_numeric();\
        if (gr_missing(v))\
	    printf("%15s ", MISSING);\
        else\
	    printf("%15g ", v);\
    }
void
print_columns()
{
	extern vector<var> vector_vars;
	int xind = get_vector_var_index("$_x");
	int yind = get_vector_var_index("$_y");
	int zind = get_vector_var_index("$_z");
	int uind = get_vector_var_index("$_u");
	int vind = get_vector_var_index("$_v");
	var* xvar = NULL;
	var* yvar = NULL;
	var* zvar = NULL;
	var* uvar = NULL;
	var* vvar = NULL;
	int xlen = 0, ylen = 0, zlen = 0, ulen = 0, vlen = 0;
	int len = 0;

	MACRO1(xind, xvar, xlen, "x");
	MACRO1(yind, yvar, ylen, "y");
	MACRO1(zind, zvar, zlen, "z");
	MACRO1(uind, uvar, ulen, "u");
	MACRO1(vind, vvar, vlen, "v");
	if (len == 0) {
		printf("Columns are empty\n");
	} else {
		printf("\n");
		for (int i = 0; i < len; i++) {
			MACRO2(xvar, i, xlen);
			MACRO2(yvar, i, ylen);
			MACRO2(zvar, i, zlen);
			MACRO2(uvar, i, ulen);
			MACRO2(vvar, i, vlen);
			printf("\n");
		}
	}
}
#undef MACRO1
#undef MACRO2

void
do_printf_command(cmdnode *c)
{
	extern vector<string> file_handle;
	wnode *n = c->first_wnode;
	FILE *fp = file_pointer[c->handle_index];
	if (fp == NULL) {
		char msg[256];
		sprintf(msg, "%s:%d: warning -- filehandle `%s' not open; using STDOUT instead",
			c->file_name, c->line, file_handle[c->handle_index].c_str());
		warning(msg);
		fp = stdout;
	}
	fflush(fp);
	if (n == NULL) {
		fatal_error(c->file_name, c->line, "`printf' needs a format");
		return;			// never actually done
	}
	if (n->t == wnode::expr) {
		enode *e = n->e;
		if (!e->is_textual()) {
			fatal_error(c->file_name, c->line, "`printf' needs a TEXTUAL format");
			return;			// never actually done
		}
	} else {
		fatal_error(c->file_name, c->line, "`printf' cannot understand item in list");
		return;			// never actually done

	}
	string fmt(interpolate_var_in_string(n->e->eval_as_text().get_text()));
	string result = do_sprintf_internal(fmt, n->next);
	fprintf(fp, "%s", result.c_str());
}

void
do_print_command(cmdnode *c)
{
	extern vector<string> file_handle;
	wnode *n = c->first_wnode;
	FILE *fp = file_pointer[c->handle_index];
	if (fp == NULL) {
		char msg[256];
		sprintf(msg, "%s:%d: warning -- filehandle `%s' not open; using STDOUT instead",
			c->file_name, c->line, file_handle[c->handle_index].c_str());
		warning(msg);
		fp = stdout;
	}
	fflush(fp);
	if (n == NULL) {
		fprintf(fp, "%s", get_var_scalar_text_at(0));
	} else {
		if (n->t == wnode::word) {
			if (!strcmp(n->w, "variables")) {
				show_var_stack();
			} else if (!strcmp(n->w, "columns")) {
				print_columns();
			} else if (!strcmp(n->w, "grid")) {
				print_grid();
			} else if (!strcmp(n->w, "image")) {
				print_image();
			} else {
				fatal_error(c->file_name, c->line, "`print' doesn't understand bare word");
			}
		} else if (n->t == wnode::expr) {
			static string s;
			while (n) {
				enode *e = n->e;
				enode::type expr_type = e->get_type();
				//printf("print wnode::expr type=%d.  is_textual=%d\n", expr_type,e->is_textual());
				if (e->is_textual()) {
					//printf("DEBUG 1\n");
					// 666666 memoryleak.gre ????
					// 666666 problem is in eval_as_text.
#if 1
					//printf("DEBUG:print.cpp...\n");
					//printf("\t'%s'\n", e->eval_as_text().get_text());
					//printf("\t'%s'\n", interpolate_var_in_string(e->eval_as_text().get_text()));
					s = interpolate_var_in_string(e->eval_as_text().get_text());
					//printf("VAL <%s>\n",s.c_str());
					print_buf(fp, s.c_str()); // BUG: wrong file
#endif
				} else if (expr_type == enode::variable) {
					static scalar ee;
					ee = e->eval();
					if (ee.is_numeric())
						fprintf(fp, "%.14g", ee.get_numeric());
					else
						print_buf(fp, e->eval_as_text().get_text());
				} else if (expr_type == enode::variable_at_i) {
					static scalar ee;
					ee = e->eval();
					if (ee.is_numeric())
						fprintf(fp, "%.14g", ee.get_numeric());
					else
						print_buf(fp, e->eval_as_text().get_text());
				} else {
					//printf("\nDUNNO DUNNO\n");
					//fprintf(fp, "%.14g", e->eval().get_numeric());
					fprintf(fp, "%s", e->eval_as_text().get_text());
				}
				n = n->next;
			}
		} else {
			fatal_error(c->file_name, c->line, "`print' doesn't understand item in list");
		}
	}
}

char*
do_sprintf(const wnode* list)
{
	const wnode *n = list;
	if (n->t == wnode::expr) {
		enode *e = n->e;
		if (!e->is_textual()) {
			fprintf(stderr, "Error: `sprintf' requires format to be textual, not numeric.\n");
			gre_exit(1);
			return "";		// never actually done
		}
	} else {
		fprintf(stderr, "`sprintf' cannot understand item in list\n");
		gre_exit(1);
		return "";			// never actually done
	}
	string fmt(interpolate_var_in_string(n->e->eval_as_text().get_text()));
	n = n->next;
	string result = do_sprintf_internal(fmt, n);
	//printf("DEBUG(do_sprintf()): got result '%s')...\n",result.c_str());
	return strdup(result.c_str());
}

