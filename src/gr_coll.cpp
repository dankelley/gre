// Classes for Gri.  See gr_coll.h for docs

//#define DEBUG_GR_COLL 1		// uncomment to debug
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "gr.h"
#include "extern.h"
#include "gr_coll.h"



// Read line from file, enlarging space if needed.  Leave newline
// at end; if file ends before newline, tack one on anyhow.
//
// RETURN false -- if file at EOF before reading any data
bool
GriString::line_from_FILE(FILE *fp)
{
	if (feof(fp))
		return false;
	char c[2];
	c[0] = c[1] = '\0';
	this->assign(c);
	bool first = true;
	do {
		*c = getc(fp);
		if (*c == EOF) {
			this->append("\n");
			//printf("GriString::line_from_FILE() got EOF, first=%d  '%s'\n", first,this->c_str());
			if (first)
				return false;
			else
				return true;
		} else {
			this->append(c);
			if (*c == '\n') {
				//printf("GriString::line_from_FILE() got newline '%s'\n", this->c_str());		
				return true;
			}
		}
		first = false;
	} while (1);
}

void
GriString::draw(double xcm, double ycm, gr_textStyle s, double angle) const
{
	if (strlen(this->c_str()) == 0)
		return;
	gr_show_at(this->c_str(), xcm, ycm, s, angle);
	// Figure bounding box
	double width, ascent, descent;
	gr_stringwidth(this->c_str(), &width, &ascent, &descent);

	//printf("GriString::draw on `%s' xcm=%.1f ycm=%.1f width=%.1f ascent=%.1f descent=%.1f angle=%.1f\n", this->c_str(), xcm,ycm,width,ascent, descent, angle);

	double tmpx[4], tmpy[4];	// 0123 from lower-left anticlockwise
	switch (s) {
	case TEXT_LJUST:
		gr_rotate_xy(     0.0, -descent, angle, tmpx + 0, tmpy + 0);
		gr_rotate_xy(   width, -descent, angle, tmpx + 1, tmpy + 1);
		gr_rotate_xy(   width,   ascent, angle, tmpx + 2, tmpy + 2);
		gr_rotate_xy(     0.0,   ascent, angle, tmpx + 3, tmpy + 3);
		break;
	case TEXT_RJUST:
		gr_rotate_xy(  -width, -descent, angle, tmpx + 0, tmpy + 0);
		gr_rotate_xy(     0.0, -descent, angle, tmpx + 1, tmpy + 1);
		gr_rotate_xy(     0.0,   ascent, angle, tmpx + 2, tmpy + 2);
		gr_rotate_xy(  -width,   ascent, angle, tmpx + 3, tmpy + 3);
		break;
	case TEXT_CENTERED:
		gr_rotate_xy(-width/2, -descent, angle, tmpx + 0, tmpy + 0);
		gr_rotate_xy( width/2, -descent, angle, tmpx + 1, tmpy + 1);
		gr_rotate_xy( width/2,   ascent, angle, tmpx + 2, tmpy + 2);
		gr_rotate_xy(-width/2,   ascent, angle, tmpx + 3, tmpy + 3);
		break;
	}
	tmpx[0] += xcm, tmpy[0] += ycm;
	tmpx[1] += xcm, tmpy[1] += ycm;
	tmpx[2] += xcm, tmpy[2] += ycm;
	tmpx[3] += xcm, tmpy[3] += ycm;
	rectangle box(vector_min(tmpx, 4), vector_min(tmpy, 4),
		      vector_max(tmpx, 4), vector_max(tmpy, 4));
	bounding_box_update(box);
}
