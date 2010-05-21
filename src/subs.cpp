#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>

//#define NDEBUG			// uncomment for debugging
#include <assert.h>

#include "parser.h"
#include "subs.h"


#define PT_PER_CM       28.45
#define DEG_PER_RAD	57.29577951		/* degrees per radian */
#define MATH_E		2.7182818284590452354	/* e */

//double deg_per_rad = DEG_PER_RAD;
double deg_per_rad = 1.0;	// for radian units



double rand_gaussian();		// zero-mean, unit standard deviation
double proj_lonlat_to_x(double lat, double lon);
double proj_lonlat_to_y(double lat, double lon);
bool proj_lonlat_to_xy(double lat[], double lon[], unsigned int num, double x[], double y[]);


static double proj_fwd_old_lat = 0.0;
static double proj_fwd_old_lon = 0.0;
static double proj_fwd_old_x   = 0.0;
static double proj_fwd_old_y   = 0.0;

double
proj_lonlat_to_x(double lat, double lon)
{
	if (lat == proj_fwd_old_lat && lon == proj_fwd_old_lon)
		return  proj_fwd_old_x;
	double x, y;
	if (proj_lonlat_to_xy(&lat, &lon, 1, &x, &y)) {
		proj_fwd_old_lat = lat;
		proj_fwd_old_lon = lon;
		proj_fwd_old_x = x;
		proj_fwd_old_y = y;
		return x;
	}
	return 0.0;
}

double
proj_lonlat_to_y(double lat, double lon)
{
	if (lat == proj_fwd_old_lat && lon == proj_fwd_old_lon)
		return  proj_fwd_old_x;
	double x, y;
	if (proj_lonlat_to_xy(&lat, &lon, 1, &x, &y)) {
		proj_fwd_old_lat = lat;
		proj_fwd_old_lon = lon;
		proj_fwd_old_x = x;
		proj_fwd_old_y = y;
		return y;
	}
	return 0.0;
}

double text_ascent(const char *c)
{
	extern void gr_stringwidth(const char *s, double *w, double *a, double *d);
	double width, ascent, descent;
	gr_stringwidth(c, &width, &ascent, &descent);
	return ascent;
}
double text_descent(const char *c)
{
	extern void gr_stringwidth(const char *s, double *w, double *a, double *d);
	double width, ascent, descent;
	gr_stringwidth(c, &width, &ascent, &descent);
	return descent;
}
double text_width(const char *c)
{
	extern void gr_stringwidth(const char *s, double *w, double *a, double *d);
	double width, ascent, descent;
	gr_stringwidth(c, &width, &ascent, &descent);
	return width;
}
double string_length0()
{
	return 111.111;
}
double string_length1(const char *s)
{
	return double(strlen(s));
}
double tagged_value(const char *tag, const char *source)
{
	printf("%s:%d in tagged_value(%s,%s)\n",__FILE__,__LINE__,tag,source);
	return 1.0;
}

static double gsrand(double s)
{
#if defined(HAVE_DRAND48)
	srand48(int(floor(s+0.5)));
#else
	srand(int(floor(s+0.5)));
#endif
	return 1.0;
}
static double grand()
{
#if defined(HAVE_DRAND48)
	return drand48();
#else
	// Below assumes 32-bit machine since number is 2^31.
	return rand() / 2147483647.0;
#endif
}

double gint(double a)	{return double(int(a));}
double gmax(double a, double b) {return (a > b ? a : b);}
double gmin(double a, double b) {return (a < b ? a : b);}
double gsin(double a)	{return sin(a / deg_per_rad);}
double gasin(double a)	{return deg_per_rad * asin(a);}
double gcos(double a)	{return cos(a / deg_per_rad);}
double gacos(double a)	{return deg_per_rad * acos(a);}
double gtan(double a)	{return tan(a / deg_per_rad);}
double gatan(double a)	{return deg_per_rad * atan(a);}
double gatan2(double a, double b) {return deg_per_rad * atan2(a, b);}
double gexp(double a)	{return pow(MATH_E, a);}
double gexp10(double a)	{return pow(10.0, a);}
double pt_to_cm(double a) {return a / PT_PER_CM;}

// Returns random variable obeying gaussian distribution,
// with zero mean and standard deviation of 1.
//
// Code is based on NR gasdev(), section 7.2, but I use
// drand48() instead of their routine.  You may, of course
// seed this using srand48().
//
// Code tested 5 Nov 1996 Dan Kelley -- produces correct
// mean and std-dev, and graph looks symmetric and gaussian
// by eye.  So can't be any large errors from code in book.
double
rand_gaussian()		
{
	static int iset = 0;
	static double gset;
	double fac, rsq, v1, v2;
	if (iset == 0) {
		do {
#if defined(HAVE_DRAND48)
			v1 = 2 * drand48() - 1;
			v2 = 2 * drand48() - 1;
#else
			v1 = 2 * rand() - 1;
			v2 = 2 * rand() - 1;
#endif
			rsq = v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2 * log(rsq) / rsq);
		gset = v1 * fac;
		iset = 1;
		return v2 * fac;
	} else {
		iset = 0;
		return gset;
	}
}

double user_to_cm_x(double xuser) 
{
	void gr_usertocm(double x, double y, double *x_cm, double *y_cm);
	double xcm, ycm;
	gr_usertocm(xuser, 1.0, &xcm, &ycm);
	return xcm;
}
double user_to_cm_y(double yuser)
{
	void gr_usertocm(double x, double y, double *x_cm, double *y_cm);
	double xcm, ycm;
	gr_usertocm(1.0, yuser, &xcm, &ycm);
	return ycm;
}
double cm_to_pt(double a) {return a * PT_PER_CM;}

/* sw_freezing_temperature (S, p) -- freezing temperature of seawater
   UNITS         S in psu; p in dbar
   RETURN VALUE  Freezing temperature in degC
   ACCURACY      Gill quotes 0.004 degrees
   CHECK         freeze (40 psu, 500 dbar) = -2.588567 degC
   REFERENCE     A.E. Gill, 1982. "Atmosphere-Ocean Dynamics". Academic Press.
   (Section A3.7, page 602), or UNESCO technical papers in marine science,
   number 44 (1983)
*/
double
sw_freezing_temperature(double S /* PSU */, double p /* dbar */)
{
	return (-0.0575 * S + 1.710523e-3 * S * sqrt (S) - 2.154996e-4 * S * S -
		7.53e-4 * p);
}

/*
  sw_salinity() -- Salinity of seawater, given Conductivity ratio, Temp, Press
  SYNTAX        double salinity (double C, double T, double p)
  UNITS         C = C(S,T,p)/C(35,15,0); T in-situ degC; p dbar
  RETURN VALUE  S in p.s.u.
  CHECK         R    T/degC   p/dbar       --> S
  1        15        0          35
  1.2      20     2000   37.245628
  0.65      5     1500   27.995347
  REFERENCE     UNESCO technical papers in marine science, number 44 (1983).
  page 6-7

*/
double
sw_salinity (double C, double T, double p)
{
	const double a0 = 0.008;
	const double a1 = -0.1692;
	const double a2 = 25.3851;
	const double a3 = 14.0941;
	const double a4 = -7.0261;
	const double a5 = 2.7081;
	const double b0 = 0.0005;
	const double b1 = -0.0056;
	const double b2 = -0.0066;
	const double b3 = -0.0375;
	const double b4 = 0.0636;
	const double b5 = -0.0144;
	const double c0 = 0.6766097;
	const double c1 = 2.00564e-2;
	const double c2 = 1.104259e-4;
	const double c3 = -6.9698e-7;
	const double c4 = 1.0031e-9;
	const double d1 = 3.426e-2;
	const double d2 = 4.464e-4;
	const double d3 = 4.215e-1;
	const double d4 = -3.107e-3;
	const double e1 = 2.070e-5;
	const double e2 = -6.370e-10;
	const double e3 = 3.989e-15;
	const double k = 0.0162;
	double deltaS, rt, Rp, Rt, Rt2, T15;
	rt = c0 + T *
		(c1 + T *
		 (c2 + T *
		  (c3 + T *
		   c4)));
	Rp = 1.0
		+
		p *
		(e1 + p *
		 (e2 + p *
		  e3))
		/
		(1.0 + T *
		 (d1 + T *
		  d2) +
		 C * (d3 + T * d4));
	Rt = C / Rp / rt;
	Rt2 = sqrt (Rt);
	T15 = T - 15.0;
	deltaS = T15
		/
		(1.0 + k * T15)
		*
		(b0 + Rt2 *
		 (b1 + Rt2 *
		  (b2 + Rt2 *
		   (b3 + Rt2 *
		    (b4 + Rt2 *
		     b5)))));
	return (double)
		a0 + Rt2 *
		(a1 + Rt2 *
		 (a2 + Rt2 *
		  (a3 + Rt2 *
		   (a4 + Rt2 *
		    (a5)))))
		+
		deltaS;
}

/*
 *
 * Calculate spiciness of seawater (JacketMcdougall85),
 * 
 * SYNTAX        double spice_JM85(double S, double T, double p)
 * 
 * UNITS         S in psu; T in degC; p in dbar
 * 
 * RETURN VALUE  spice in kg/(m**3)
 * 
 * TEST VALUE 21C, 35psu -> spice = 27.319 (check p 1202 in JM85 paper)
 *
 * USES          theta(S, T, p);
 */
double
sw_spice(double S, double T, double p)
{
	static double a[5][5] = {
		{ 1.609705e-1,  6.542397e-1,  5.222258e-4, -2.586742e-5,  7.565157e-7},
		{-8.007345e-2,  5.309506e-3, -9.612388e-5,  3.211527e-6, -4.610513e-8},
		{ 1.081912e-2, -1.561608e-4,  3.774240e-6, -1.150394e-7,  1.146084e-9},
		{-1.451748e-4,  3.485063e-6, -1.387056e-7,  3.737360e-9, -2.967108e-11},
		{ 1.219904e-6, -3.591075e-8,  1.953475e-9, -5.279546e-11, 4.227375e-13},
	};
	int i, j;
	double sp = 0.0;
	double t[5], s[5];
	t[0] = 1.0;
	t[1] = sw_potential_temperature(S, T, p, 0.0);
	t[2] = t[1] * t[1];
	t[3] = t[1] * t[2];
	t[4] = t[1] * t[3];
	s[0] = 1.0;
	s[1] = S;
	s[2] = s[1] * s[1];
	s[3] = s[1] * s[2];
	s[4] = s[1] * s[3];
	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			sp += a[i][j] * t[i] * s[j];
	return (sp);
}


double
sw_alpha(double S, double T, double p)
{
	const double dT = 1.0e-5;
	double          rhoMinus = sw_density(S, T - dT, p);
	double          rhoPlus = sw_density(S, T + dT, p);
	return (rhoMinus - rhoPlus) / (rhoMinus + rhoPlus) / dT;
}

double
sw_beta(double S, double T, double p)
{
	const double dS = 1.0e-5;
	double          rhoMinus = sw_density(S - dS, T, p);
	double          rhoPlus = sw_density(S + dS, T, p);
	return (rhoPlus - rhoMinus) / (rhoMinus + rhoPlus) / dS;
}

double
sw_density(double S /* PSU */, double T /* in-situ degC */, double p /* dbar */)
{
	double          rho_w, Kw, Aw, Bw, p1, S12, ro, xkst;
	rho_w = 999.842594 +
		T * (6.793952e-2 +
		     T * (-9.095290e-3 +
			  T * (1.001685e-4 +
			       T * (-1.120083e-6 + T * 6.536332e-9))));
	Kw = 19652.21
		+ T * (148.4206 +
		       T * (-2.327105 +
			    T * (1.360477e-2 - T * 5.155288e-5)));
	Aw = 3.239908 +
		T * (1.43713e-3 +
		     T * (1.16092e-4 -
			  T * 5.77905e-7));
	Bw = 8.50935e-5 +
		T * (-6.12293e-6 +
		     T * 5.2787e-8);
	p1 = 0.1 * p;
	S12 = sqrt(S);
	ro = rho_w +
		S * (8.24493e-1 +
		     T * (-4.0899e-3 +
			  T * (7.6438e-5 +
			       T * (-8.2467e-7 + T * 5.3875e-9))) +
		     S12 * (-5.72466e-3 +
			    T * (1.0227e-4 -
				 T * 1.6546e-6) +
			    S12 * 4.8314e-4));
	xkst = Kw +
		S * (54.6746 +
		     T * (-0.603459 +
			  T * (1.09987e-2 -
			       T * 6.1670e-5)) +
		     S12 * (7.944e-2 +
			    T * (1.6483e-2 +
				 T * (-5.3009e-4)))) +
		p1 * (Aw +
		      S * (2.2838e-3 +
			   T * (-1.0981e-5 +
				T * (-1.6078e-6)) +
			   S12 * (1.91075e-4)) +
		      p1 * (Bw +
			    S * (-9.9348e-7 +
				 T * (2.0816e-8 +
				      T * (9.1697e-10)))));
	return (ro / (1.0 - p1 / xkst));
}
/* From Unesco technical papers in marine science, number 44 (1983).
 * 
 * SYNTAX        double sw_lapse_rate(double S, double t, double p)
 * 
 * UNITS         S in psu; t in degC; p in dbar;
 * 
 * RETURN VALUE  adiabatic lapse rate in degC/dbar
 *
 * Note: used to compute potential temperature.
 */
double
sw_lapse_rate(double S, double t, double p)
{
	const double a0 = 3.5803e-5;
	const double a1 = 8.5258e-6;
	const double a2 = -6.8360e-8;
	const double a3 = 6.6228e-10;
	const double b0 = 1.8932e-6;
	const double b1 = -4.2393e-8;
	const double c0 = 1.8741e-8;
	const double c1 = -6.7795e-10;
	const double c2 = 8.7330e-12;
	const double c3 = -5.4481e-14;
	const double d0 = -1.1351e-10;
	const double d1 = 2.7759e-12;
	const double e0 = -4.6206e-13;
	const double e1 = 1.8676e-14;
	const double e2 = -2.1687e-16;
    
	double Gamma;

	Gamma = a0 + t * (a1 + t * (a2 + t * a3))
		+ (S - 35.) * (b0 + t * b1)
		+ p * (c0 + t * (c1 + t * (c2 + t * c3))
		       + (S - 35.) * (d0 + t * d1)
		       + p * (e0 + t * (e1 + t * e2)));
    
	return Gamma;
}
double
sw_potential_temperature(double S, double t, double p, double pref)
{
	double dp, sq2;
	double dt1, t1, q1;
	double dt2, t2, q2;
	double dt3, t3, q3;
	double dt4, t4;

	dp = pref - p;
	sq2 = sqrt(2.);
    
	dt1 = dp * sw_lapse_rate(S, t, p);
	q1 = dt1;
	t1 = t + 0.5 * dt1;

	dt2 = dp * sw_lapse_rate(S, t1, p + 0.5 * dp);
	q2 = (2. - sq2) * dt2 + (-2. + 3. / sq2) * q1;
	t2 = t1 + (1. - 1. / sq2) * (dt2 - q1);

	dt3 = dp * sw_lapse_rate(S, t2, p + 0.5*dp);
	q3 = (2 + sq2) * dt3 + (-2. - 3. / sq2) * q2;
	t3 = t2 + (1. + 1. / sq2) * (dt3 - q2);

	dt4 = dp * sw_lapse_rate(S, t3, p + dp);
	t4 = t3 + 1. / 6. * (dt4 - 2. * q3);

	return t4;
}

double
pgm_version()
{
	/* Return version of form AA.BBCC where AA is major version,
	 * BB is minor and CC is subminor */
	extern char *gre_version;
	char major[10], minor[10], subminor[10];
	strcpy(major, gre_version);
	unsigned int i, l = strlen(gre_version);
	for (i = 0; i < l; i++)
		if (major[i] == '.') {
			major[i] = '\0';
			break;
		}
	strcpy(minor, gre_version + i + 1);
	l = strlen(minor);
	for (i = 0; i < l; i++)
		if (minor[i] == '.') {
			minor[i] = '\0';
			break;
		}
	strcpy(subminor, minor + i + 1);
	return atof(major) + 1e-2*atof(minor) + 1e-4*atof(subminor);
}

SUB0_TEXT sub0_text[] = {
	{0,              ""             }
}; 

SUB1_TEXT sub1_text[] = {
	{text_width,    "text_width"    },
	{text_ascent,   "text_ascent"   },
	{text_descent,  "text_descent"  },
	{string_length1,"length"        },
	{0,		    ""		    }
};
SUB2_TEXT sub2_text[] = {
	{tagged_value,  "tagged_value"  },
	{0,		    ""		    }
};

SUB0 sub0[] = {
#if defined(HAVE_DRAND48)
	{drand48,	    "rand"	    },
#else
	{grand,	    "rand"	    },
#endif
	{rand_gaussian, "rand_gaussian" },
	{pgm_version,   "version"	    },
	{0,             ""		    }
};
SUB1 sub1[] = {
#if defined(HAVE_ERF)
	{erf,           "erf"           },
#endif
#if defined(HAVE_ERFC)
	{erfc,          "erfc"          },
#endif
	{pt_to_cm,	    "pt_to_cm"	    },
	{cm_to_pt,	    "cm_to_pt"	    },
	{user_to_cm_x,  "user_to_cm_x"  },
	{user_to_cm_y,  "user_to_cm_y"  },
	{gint,	    "int"           },
	{gsin,          "sin"	    },
	{gcos,	    "cos"	    },
	{gtan,	    "tan"	    },
	{gasin,	    "asin"	    },
	{gacos,	    "acos"	    },
	{gatan,	    "atan"	    },
	{sinh,	    "sinh"	    },
	{cosh,	    "cosh"	    },
	{tanh,	    "tanh"	    },
	{sqrt,	    "sqrt"	    },
	{log10,	    "log10"	    },
	{log,	    "ln"	    },
	{gexp,	    "exp"	    },
	{gexp10,	    "exp10"	    },
	{fabs,	    "abs"	    },
	{ceil,	    "ceil"	    },
	{floor,	    "floor"	    },
	{gsrand,	    "srand"	    },
#if defined(HAVE_ACOSH)
	{asinh,	    "asinh"	    },
	{acosh,	    "acosh"	    },
	{atanh,	    "atanh"	    },
#endif
	{0,		    ""		    }
};
SUB2 sub2[] = {
	{pow,	              "pow"		        },
	{gmax,	              "max"		        },
	{gmin,	              "min"		        },
	{gatan2,	              "atan2"		        },
	{sw_freezing_temperature, "sw_freezing_temperature" },
	{proj_lonlat_to_x,        "proj_lonlat_to_x"        },
	{proj_lonlat_to_y,        "proj_lonlat_to_y"        },
	{0,		              ""		        }
};
SUB3 sub3[] = {
	{sw_alpha,      "sw_alpha"      },
	{sw_beta,       "sw_beta"       },
	{sw_density,    "sw_density"    },
	{sw_lapse_rate, "sw_lapse_rate" },
	{sw_salinity,   "sw_salinity"   },
	{sw_spice,      "sw_spice"      },
	{0,		    ""		    }
};
SUB4 sub4[] = {
	{sw_potential_temperature, "sw_potential_temperature"},
	{0,		""		}
};

// Called near parser.y:1047
user_sub::user_sub(const struct wnode* the_name, char* prototype, struct cmdnode* the_block)
{
	if (the_name == NULL) {
		yyerror("Internal error in `sub WORD'");
		name = "";
		action = NULL;
		return;
	}
	bool duplicate = false;
	//DANDAN extern SUB0 sub0[];
	unsigned int i = 0;
	while (sub0[i].fcn && !duplicate)
		if (!strcmp(the_name->w, sub0[i++].name))
			duplicate = true;
	if (!duplicate) {
		//DANDAN extern SUB1 sub1[];
		i = 0;
		while (sub1[i].fcn && !duplicate)
			if (!strcmp(the_name->w, sub1[i++].name))
				duplicate = true;
	}
	if (!duplicate) {
//DANDAN 	extern SUB2 sub2[];
		i = 0;
		while (sub2[i].fcn)
			if (!strcmp(the_name->w, sub2[i++].name))
				duplicate = true;
	}
	if (!duplicate) {
//DANDAN 	extern SUB3 sub3[];
		i = 0;
		while (sub3[i].fcn)
			if (!strcmp(the_name->w, sub3[i++].name))
				duplicate = true;
	}
	if (!duplicate) {
//DANDAN 	extern SUB4 sub4[];
		i = 0;
		while (sub4[i].fcn)
			if (!strcmp(the_name->w, sub4[i++].name))
				duplicate = true;
	}
	if (duplicate) {
		string msg("Cannot re-define builtin function `");
		msg += the_name->w;
		msg += "'";
		yyerror(msg.c_str());
		action = NULL;	// not sure if needed
		return;
	} 
	name = strdup(the_name->w);
	action = the_block;
	if (!prototype) {
		min_args = 0;
		max_args = 0; // means don't care
	} else {
		min_args = max_args = 0;
		unsigned int len = strlen(prototype);
		bool found_semicolon = false;
		unsigned int i;
		for (i = 0; i < len; i++) {
			// Need only check legal characters since the string
			// was already checked in yylex.
			if (prototype[i] == ';') {
				found_semicolon = true;
			} else if (prototype[i] == '$') {
				if (found_semicolon) {
					max_args++;
				} else {
					min_args++;
					max_args++;
				}
			} else if (prototype[i] == '@') {
				if (found_semicolon)
					max_args = -1;
				else
					min_args = max_args = -1;
				break;
			}
		}
	}
	//printf("Newcmd '%s' ... has MIN args = %d; max args = %d\n", s->w, min_args, max_args);
}
