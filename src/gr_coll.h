// -*- mode: c++ -*-

// Some classes for Gri
#if !defined(_gr_coll_hh_)
#define _gr_coll_hh_
//  GriString:          -- string
#include <string>
#include "macro.h"
#include "errors.h"
#include "types.h"
#include "GriColor.h"

class GriString : public string {
public:
	void draw(double xcm, double ycm, gr_textStyle s, double angle) const;
	bool line_from_FILE(FILE *fp);
};

#endif				// ifdef _gr_coll_hh_
