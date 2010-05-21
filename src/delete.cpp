#include <string>
#include <stdlib.h>
#include <stddef.h>

#include	"extern.h"

using namespace std;

#if 0
bool            delete_columnsCmd(void);
static void     delete_all_columns(void);
static bool     delete_columns_randomly(void);
bool            delete_columns_where_missing(void);
bool            delete_gridCmd(void);
extern char     _grTempString[];
#endif

// ($;$$) { # {columns [{randomly .fraction.}|{where missing}] }|grid|{[x|y] scale}
bool
deleteCmd()
{
	unsigned int num_params = cmd_parameters();
	if (num_params > 0) {
		expecting_a_keyword("`delete columns where'", 0, "`columns', `grid' or `scale'");
		const char *cmd0 = cmd_parameter_s(0);
		if (strEQ(cmd0, "columns")) {
			if (num_params == 1) {
				// delete columns;
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
			} else if (num_params == 3) { // delete columns ...
				expecting_a_keyword("`delete columns where'", 1, "`randomly' or `where'");
				const char *cmd1 = cmd_parameter_s(1);
				if (strEQ(cmd1, "randomly")) {
					expecting_an_exp("`delete columns randomly'", 2);
					double f = cmd_parameter_n(2);
					if (f > 0.0) {
						printf("SHOULD `delete columns randomly %f' now, but NOT SINCE NOT CODED YET\n", f);
					} else {
						warning("`delete columns randomly' clipping .fraction. to 0 (did no deletions)");
					}
				} else if (strEQ(cmd1, "where")) {
					printf("DEBUG: `delete columns where ...' not coded yet\n");
					expecting_the_keyword("`delete columns where'", 2, "`missing'");
				} else {
					err("`delete columns' expecting keyword `randomly' or `missing' but got `\\", cmd1, "' instead", "\\");
					return false;
				}
			} else {
				err("`delete columns' may take only 2 arguments, not \\", dtoa(num_params - 1), " as given", "\\");
				return false;
			}
		} else if (strEQ(cmd0, "grid")) {
			printf("DEBUG: should delete grid now, but NOT DOING IT YET\n");
		} else if (strEQ(cmd0, "scale")) {
			_xscale_exists = false;
			_need_x_axis = true;
			_user_set_x_axis = false;
			gr_setxtransform(gr_axis_LINEAR);
			_yname = "x";
			_yscale_exists = false;
			_need_y_axis = true;
			_yincreasing = true;
			_user_set_y_axis = false;
			gr_setytransform(gr_axis_LINEAR);
			_yname = "y";
		} else if (strEQ(cmd0, "x")) {
			expecting_the_keyword("`delete x'", 1, "scale");
			_xscale_exists = false;
			_need_x_axis = true;
			_user_set_x_axis = false;
			gr_setxtransform(gr_axis_LINEAR);
			_yname = "x";
		} else if (strEQ(cmd0, "y")) {
			expecting_the_keyword("`delete y'", 1, "scale");
			_yscale_exists = false;
			_need_y_axis = true;
			_yincreasing = true;
			_user_set_y_axis = false;
			gr_setytransform(gr_axis_LINEAR);
			_yname = "y";
		} else {
			err("`delete' expecting keyword `columns', `grid' or `scale' but got `\\", cmd0, "' instead", "\\");
			return false;
		}
	} else {
		printf("\n\n\t**** 'delete' not working yet DEBUG: num_params = %d\n\n\t******\n", num_params);
	}
	return true;
}

#if 0
// `delete columns [{randomly .fraction.}|{where missing}]'

static bool
delete_columns_randomly()
{
	double fraction;
	if (!getdnum(_word[3], &fraction)) {
		READ_WORD_ERROR(".fraction.");
		return false;
	}
	if (fraction < 0.0) {
		warning("`delete columns randomly' clipping .fraction. to 0 (did no deletions)");
		return true;		// do nothing
	}
	if (fraction > 1.0) {
		warning("`delete columns randomly' clipping .fraction. to 1");
		delete_all_columns();
		return true;
	}
	int length = _colX.size();
	double miss = gr_currentmissingvalue();
	int good = 0;
	vector<int> ok((size_t)length, 0);
	int i;
	for (i = 0; i < length; i++) {
		ok[i] = (char)0;
		if (_colX.size() > 0 && (_colX[i] == miss))
			continue;
		if (_colY.size() > 0 && (_colY[i] == miss))
			continue;
		if (_colZ.size() > 0 && (_colZ[i] == miss))
			continue;
		if (_colU.size() > 0 && (_colU[i] == miss))
			continue;
		if (_colV.size() > 0 && (_colV[i] == miss))
			continue;
		if (_colR.size() > 0 && (_colR[i] == miss))
			continue;
		if (_colTHETA.size() > 0 && (_colTHETA[i] == miss))
			continue;
		ok[i] = (char)1;
		good++;
	}
// Create vector of whether to kill a given index
	vector<int> kill((size_t)length, 0);
// Laborously get correct number of data to discard.  Originally
// I just tried to remove the given number, but that adds
// an extra element of randomness.
	int subset = int(floor(good * fraction + 0.5));
	int collisions = 0;
#if defined(HAVE_DRAND48)
	srand48(getpid());
#else
#if defined(HAVE_GETPID)
	srand(getpid());
#endif
#endif
	for (i = 0; i < subset; i++) {
#if defined(HAVE_DRAND48)	// range is 0 to 1, but do modulus in case
		int index = int(drand48() * length) % length;
#else
		int index = int(rand() % length);
#endif
		if (ok[index]) {
			if (kill[index]) {
				if (collisions++ > length) {
					sprintf(_grTempString, "`delete columns randomly' could only delete %d columns\n", i);
					warning(_grTempString);
					break;
				}
				i--;
			} else {
				kill[index] = 1;
			}
		} else {
			i--;
		}
	}
	for (i = 0; i < length; i++) {
		if (kill[i]) {
			if (_colX.size())
				_colX[i] = miss;
			if (_colY.size())
				_colY[i] = miss;
			if (_colZ.size())
				_colZ[i] = miss;
			if (_colU.size())
				_colU[i] = miss;
			if (_colV.size())
				_colV[i] = miss;
			if (_colR.size())
				_colR[i] = miss;
			if (_colTHETA.size())
				_colTHETA[i] = miss;
		}
	}	
	return true;
}

bool
delete_columns_where_missing()
{
	int haveX, haveY, haveZ, haveU, haveV, haveR, haveTHETA;
	haveX = haveY = haveZ = haveU = haveV = haveR = haveTHETA = 0;
	int length = _colX.size();
	int i;
	for (i = 0; i < length; i++) {
		if (_colX.size()) haveX = 1;
		if (_colY.size()) haveY = 1;
		if (_colZ.size()) haveZ = 1;
		if (_colU.size()) haveU = 1;
		if (_colV.size()) haveV = 1;
		if (_colR.size()) haveR = 1;
		if (_colTHETA.size()) haveTHETA = 1;
	}
	double *xP = _colX.begin();
	double *yP = _colY.begin();
	double *zP = _colZ.begin();
	double *uP = _colU.begin();
	double *vP = _colV.begin();
	double *rP = _colR.begin();
	double *thetaP = _colTHETA.begin();
	vector<int> kill((size_t)length, 0);
	int num_to_kill = 0;
	for (i = 0; i < length; i++) {
		if (haveX && gr_missing(xP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveY && gr_missing(yP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveZ && gr_missing(zP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveU && gr_missing(uP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveV && gr_missing(vP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveR && gr_missing(rP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
		if (haveTHETA && gr_missing(thetaP[i])) {
			kill[i] = 1; num_to_kill++; continue;
		}
	}
	if (!num_to_kill) {
		return true;
	}
	for (i = length - 1; i >= 0; i--) {
		if (kill[i]) {
			if (haveX) _colX.erase(_colX.begin() + i);
			if (haveY) _colY.erase(_colY.begin() + i);
			if (haveZ) _colZ.erase(_colZ.begin() + i);
			if (haveU) _colU.erase(_colU.begin() + i);
			if (haveV) _colV.erase(_colV.begin() + i);
			if (haveR) _colR.erase(_colR.begin() + i);
			if (haveTHETA) _colTHETA.erase(_colTHETA.begin() + i);
		}
	}
	PUT_VAR("..num_col_data..", double(_colX.size()));
	PUT_VAR("..num_col_data_missing..", 0);
	length -= num_to_kill;
	return true;
}

bool
delete_gridCmd()
{
	Require(_nword == 2, err("Must have `delete grid'"));
	if (!strcmp(_word[1], "grid")) {
		_f_xy.set_size(0, 0);
		_legit_xy.set_size(0, 0);
		if (_grid_exists == true) {
			_f_min = _f_max = gr_currentmissingvalue();
			_grid_exists = false;
		}
		if (_xgrid_exists == true) {
#if defined(DEBUG_STORAGE)
			printf("delete clearing _xmatrix=%x\n", _xmatrix);
#endif
			delete [] _xmatrix;
			_xgrid_exists = false;
		}
		if (_ygrid_exists == true) {
#if defined(DEBUG_STORAGE)
			printf("delete clearing _ymatrix=%x\n", _ymatrix);
#endif
			delete [] _ymatrix;
			_ygrid_exists = false;
		}
	} else {
		err("Must have `delete grid'");
		return false;
	}
	return true;
}
#endif
