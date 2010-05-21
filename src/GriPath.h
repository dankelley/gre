// @(#) GriPath.hh 1.16 (c) 97/05/17 Dan E. Kelley
// GriPath -- store PostScript path
#if !defined(_GriPath_hh_)
#define _GriPath_hh_
#include "types.h"
class GriPath
{
 public:
	enum type {moveto, lineto};
	GriPath();
	GriPath(unsigned capacity);
	~GriPath();
	void closepath() { closed = true;} // Close the path
	void clear();		// Allows reuse without realloc
	void expand();		// Get more space
	void push_back(double xx, double yy, char aa); // Append at end
	unsigned size();	// Return length of path
	unsigned int trim();	// remove extraneous moveto; return new length
	void stroke(units u, double width = -1); // Stroke the path
	void stroke_or_fill(char s_or_f, units u, double width = -1); // Stroke or fill
	void fill(units u);	// Fill path in
	void print();		// Mostly for debugging
	void translate(double dx, double dy); // Only makes sense if units_cm
	void scale(double enlargement); // Only makes sense if units_cm
	void rotate(double degrees); // Rotate anticlockwise
	double get_x(unsigned offset) {return x[offset];} // bug: no checking
	double get_y(unsigned offset) {return y[offset];} // no checking
	rectangle bounding_box(units u);
 private:
	unsigned int depth;	// Length of path
	unsigned int capacity;	// Max elements in path; see expand\(\)
	bool closed;		// a closed path (in postscript sense)?
	// Next three are vector-like, storing values
	double *x;		// Data
	double *y;		// Data
	type *action;		// 'm' or 'l'
};
#endif
