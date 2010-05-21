// #define DASH

// @(#) GriState.hh 1.21 (c) 97/03/26 Dan E. Kelley
#if !defined(_GriState_h_)
#define _GriState_h_

//#define DEBUG_GRISTATE // Debug

#include <stdio.h>
#include <vector>		// part of STL
#include "defaults.h"
//#include "gr.hh"
#include "GriColor.h"

// The graphics state.  Important global object; see also state.cc.
class GriState
{
 public:
	GriState();
	GriState(const GriState& n);
	~GriState();
	void set_color_line(const GriColor& c);
	void set_color_text(const GriColor& c);
	void set_separate_text_color(bool f)	{is_separate_text_color = f;    }
	void set_fontsize(double fs)		{the_font.size_pt     = fs;	}
	void set_font(gr_font f)		{the_font             = f;	}
	void set_line_cap(int c)		{the_line_cap         = c;      }
	void set_line_join(int j)		{the_line_join        = j;      }
	void set_linewidth_axis(double w)	{the_linewidth_axis   = w;	}
	void set_linewidth_line(double w)	{the_linewidth_line   = w;	}
	void set_linewidth_symbol(double w)	{the_linewidth_symbol = w;	}
	void set_superuser(int s)		{the_superuser	      = s;	}
	void set_trace(bool t)			{the_trace	      = t;	}
	const GriColor& color_line() const 	{return colorLine;		}
	const GriColor& color_text() const	{return colorText;		}
	bool separate_text_color() const	{return is_separate_text_color; }
	gr_font	font() const			{return the_font;		}
	int	line_cap() const		{return the_line_cap;           }
	int	line_join() const		{return the_line_join;          }
	double	linewidth_axis() const		{return the_linewidth_axis;	}
	double	linewidth_line() const		{return the_linewidth_line;	}
	double	linewidth_symbol() const	{return the_linewidth_symbol;	}
	int	superuser() const		{return the_superuser;		}
	bool	trace() const			{return the_trace;		}
#ifdef DASH
	const vector<double>& dash() const       {return *the_dash;}
#endif
	bool operator<  (const GriState& ignored) const {return false;}
	bool operator== (const GriState& ignored) const {return false;}
	GriState& operator= (const GriState& n);
 private:
	double		the_linewidth_axis;
	double		the_linewidth_line;
	double		the_linewidth_symbol;
	int		the_line_cap;
	int		the_line_join;
	gr_font		the_font;
	GriColor	colorLine;
	GriColor	colorText;
	bool		is_separate_text_color;
	bool		the_trace;
	int		the_superuser;
#ifdef DASH
	vector<double> *the_dash;
#endif
	// FOR MORE, SEE extern.hh:21
};

#endif
