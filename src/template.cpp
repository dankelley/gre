// Instantiate templates.
// BUG: this code may well need changing as compilers evolve
// BUG: in how they handle templates.  :=(

#include <string>
#include <stdio.h>
#include <vector>
#include "private.h"
#include "types.h"
#include "GriColor.h"
#include "GMatrix.h"
#include "parser.h"

using namespace std;

//#include        "defaults.hh"
//#include        "gr.hh"
//#include        "gr_coll.hh"
//#include        "GriState.hh"


// Instantiate on DEC c++ compiler
#if defined(__DECCXX)
#pragma define_template void  reverse(vector<double>::iterator, vector<double>::iterator);
#pragma define_template void sort(vector<double>::iterator, vector<double>::iterator);
//#pragma define_template vector<CmdFile>
//#pragma define_template vector<DataFile>
#pragma define_template vector<GriNamedColor>
//#pragma define_template vector<GriState>
#pragma define_template vector<bool>
#pragma define_template vector<double>
#pragma define_template vector<FILE*>
#pragma define_template vector<int>
#pragma define_tempate  vector<string>
#pragma define_tempate  vector<char*>
#pragma define_template vector<var>
#pragma define_template GriMatrix<bool>
#pragma define_template GriMatrix<double>
#endif // DEC compiler


// Instantiate on GNU c++ compiler
#if defined(__GNUC__)
#if __GNUC__ >= 3		// avoid deficiency in old compilers
//void std::reverse(std::vector<double>::iterator, std::vector<double>::iterator);
//void std::sort(std::vector<double>::iterator, std::vector<double>::iterator);
#else
template void std::reverse(std::vector<double>::iterator, std::vector<double>::iterator);
template void std::sort(std::vector<double>::iterator, std::vector<double>::iterator);
#endif

//template class vector<CmdFile>;
//template class vector<DataFile>;
template class vector<GriNamedColor>;
//template class vector<GriState>;
template class vector<bool>;
template class vector<double>;
template class vector<FILE*>;
template class vector<int>;
template class vector<string>;
template class vector<char*>;
template class vector<var>;
template class GriMatrix<bool>;
template class GriMatrix<double>;

#endif // new GCC compiler
