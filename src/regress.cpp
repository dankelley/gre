static char     sccsid[] = "@(#) regress.cc 1.29 (c) 97/06/16 Dan E. Kelley";
#include	<string>
#include	<math.h>
#include	<stdio.h>
#include	"gr.h"
#include	"extern.h"

using namespace std;

extern char     _grTempString[];
bool            regressCmd(void);
static bool     regress_linear(bool y_vs_x);
static int      fit(double x[], double y[], int ndata, double sig[], int mwt, double *a, double *b, double *siga, double *sigb, double *chi2, double *q);
static double   gammln(double xx);
static double   gammq(double a, double x);
static void     gcf(double *gammcf, double a, double x, double *gln);
static void     gser(double *gamser, double a, double x, double *gln);
static double   R_linear(double x[], double y[], int n);
static double   rms_deviation(double x[], double y[], int n, double a, double b);
static double student_t_025(int nu);

// ($$) { # {y vs x}|{x vs y}
// regressCmd() - handle 'regress' command
bool
regressCmd()
{
	if (cmd_parameter_is_n(0)) {
		err("`regress' expecting word `x' or `y' but got numerical expression \\", dtoa(cmd_parameter_n(0)), " instead", "\\");
		return false;
	}
	bool y_vs_x = true;
	const char *tmp = cmd_parameter_s(0);
	if (strEQ(tmp, "y")) {
		y_vs_x = true;
	} else if (strEQ(tmp, "x")) {
		y_vs_x = false;
	} else {
		err("`regress' expecting word `x' or `y' but found `\\", tmp, "' instead", "\\");
		return false;
	}
	if (cmd_parameter_is_n(1)) {
		err("`regress \\", (y_vs_x ? "y" : "x"), "' expecting word `vs' but got numerical expression \\", dtoa(cmd_parameter_n(1)), " instead", "\\");
		return false;
	}
	tmp = cmd_parameter_s(1);
	if (strNE(tmp, "vs")) {
		err("`regress \\", (y_vs_x ? "y" : "x"), "' expecting word `vs' but got `\\", tmp, "' instead", "\\");
		return false;
	}
	tmp = cmd_parameter_s(2);
	if (y_vs_x) {
		if (strNE(tmp, "x")) {
			err("`regress y vs' expecting `x' but got `\\", tmp, "' instead", "\\");
			return false;
		}
	} else {
		if (strNE(tmp, "y")) {
			err("`regress x vs' expecting `y' but got `\\", tmp, "' instead", "\\");
			return false;
		}
	}
	return regress_linear(y_vs_x);
}

static bool
regress_linear(bool y_vs_x)
{
	int xind = get_vector_var_index("$_x");
	if (xind < 0)
		fatal_error("X column ($_x) has become non-existent");
	int yind = get_vector_var_index("$_y");
	if (yind < 0)
		fatal_error("Y column ($_y) has become non-existent");
	extern vector<var> vector_vars;
	var* xvar = &vector_vars[xind];
	var* yvar = &vector_vars[yind];
	int xlen = 1 + xvar->get_end_index();
	if (xlen < 1)
		fatal_error("`draw curve' noticed that no x data exist");
	int ylen = 1 + yvar->get_end_index();
	if (ylen < 1)
		fatal_error("`draw curve' noticed that no y data exist");
	if (xlen != ylen)
		fatal_error("`draw curve' requires xlen == ylen");
	double *xx = new double[xlen];
	double *yy = new double[xlen];
	double *errx = new double[xlen];
	for (int i = 0; i < xlen; i++) {
		if (y_vs_x) {
			xx[i] = xvar->get_at_index(i).get_numeric();
			yy[i] = yvar->get_at_index(i).get_numeric();
		} else {
			xx[i] = yvar->get_at_index(i).get_numeric();
			yy[i] = xvar->get_at_index(i).get_numeric();
		}
		if (!gr_missing(xx[i]) && !gr_missing(yy[i]))
			errx[i] = 1.0;
		else
			errx[i] = 1.0e10;
	}
	double          a, b, siga, sigb, chi2, q;
	int good = fit(xx, yy, xlen, errx, 1, &a, &b, &siga, &sigb, &chi2, &q);
	double r2 = R_linear(xx, yy, xlen);
	r2 = r2 * r2;
	put_var("$_coeff0", a);
	put_var("$_coeff1", b);
	put_var("$_coeff0_sig", student_t_025(good-2)*siga);
	put_var("$_coeff1_sig", student_t_025(good-2)*sigb);
	put_var("$_R2", r2);
	put_var("$_regression_deviation", rms_deviation(xx, yy, xlen, a, b));
	return true;
}

// Compute Pearson correlation coefficient, R.
static double
R_linear(double x[], double y[], int n)
{
	// Use formulae in terms of demeaned variables, 
	// for numerical accuracy.
	int             i, non_missing = 0;
	double          xmean = 0.0, ymean = 0.0;
	for (i = 0; i < n; i++) {
		if (!gr_missing(x[i]) && !gr_missing(y[i])) {
			xmean += x[i];
			ymean += y[i];
			non_missing++;
		}
	}
	if (non_missing == 0)
		return 0.0;
	xmean /= non_missing;
	ymean /= non_missing;
	double          syy = 0.0, sxy = 0.0, sxx = 0.0;
	double          xtmp, ytmp;
	for (i = 0; i < n; i++) {
		if (!gr_missing(x[i]) && !gr_missing(y[i])) {
			xtmp = x[i] - xmean;
			ytmp = y[i] - ymean;
			sxx += xtmp * xtmp;
			syy += ytmp * ytmp;
			sxy += xtmp * ytmp;
		}
	}
	return (sxy / sqrt(sxx * syy));
}

// RMS deviation to model y=a+bx
static double
rms_deviation(double x[], double y[], int n, double a, double b)
{
	int non_missing = 0;
	double sum = 0.0, dev;
	for (int i = 0; i < n; i++) {
		if (!gr_missing(x[i]) && !gr_missing(y[i])) {
			dev = y[i] - a - b * x[i];
			sum += dev * dev;
			non_missing++;
		}
	}
	if (non_missing == 0)
		return gr_currentmissingvalue();
	return sqrt(sum / non_missing);
}

// Returns number good data
static double   sqrarg;
#define SQR(a) (sqrarg=(a),sqrarg*sqrarg)
static int
fit(double x[], double y[], int ndata,
    double sig[], int mwt,
    double *a, double *b,
    double *siga, double *sigb,
    double *chi2, double *q)
{
	int             i;
	int             good = 0;
	double          wt, t, sxoss, sx = 0.0, sy = 0.0, st2 = 0.0, ss, sigdat;
	*b = 0.0;
	if (mwt) {
		ss = 0.0;
		for (i = 0; i < ndata; i++) {
			if (!gr_missing(x[i]) && !gr_missing(y[i]) && !gr_missing(sig[i])) {
				wt = 1.0 / SQR(sig[i]);
				sx += x[i] * wt;
				sy += y[i] * wt;
				ss += wt;
				good++;
			}
		}
	} else {
		ss = 0.0;
		for (i = 0; i < ndata; i++) {
			if (!gr_missing(x[i]) && !gr_missing(y[i])) {
				sx += x[i];
				sy += y[i];
				ss += 1.0;
				good++;
			}
		}
	}
	sxoss = sx / ss;
	if (mwt) {
		for (i = 0; i < ndata; i++) {
			if (!gr_missing(x[i]) && !gr_missing(y[i]) && !gr_missing(sig[i])) {
				t = (x[i] - sxoss) / sig[i];
				st2 += t * t;
				*b += t * y[i] / sig[i];
			}
		}
	} else {
		for (i = 0; i < ndata; i++) {
			if (!gr_missing(x[i]) && !gr_missing(y[i])) {
				t = x[i] - sxoss;
				st2 += t * t;
				*b += t * y[i];
			}
		}
	}
	*b /= st2;
	*a = (sy - sx * (*b)) / ss;
	*siga = sqrt((1.0 + sx * sx / (ss * st2)) / ss);
	*sigb = sqrt(1.0 / st2);
	*chi2 = 0.0;
	if (mwt == 0) {
		for (i = 0; i < ndata; i++)
			if (!gr_missing(x[i]) && !gr_missing(y[i]))
				*chi2 += SQR(y[i] - (*a) - (*b) * x[i]);
		*q = 1.0;
		if (good > 2) {
			sigdat = sqrt((*chi2) / (good - 2));
			*siga *= sigdat;
			*sigb *= sigdat;
		} else {
			*siga = -1.0;
			*sigb = -1.0;
		}
	} else {
		for (i = 0; i < ndata; i++)
			if (!gr_missing(x[i]) && !gr_missing(y[i]) && !gr_missing(sig[i]))
				*chi2 += SQR((y[i] - (*a) - (*b) * x[i]) / sig[i]);
		if (good > 2) {
			*q = gammq(0.5 * (good - 2), 0.5 * (*chi2));
			sigdat = sqrt((*chi2) / (good - 2));
			*siga *= sigdat;
			*sigb *= sigdat;
		} else {
			*q = -1.0;
			*siga = -1.0;
			*sigb = -1.0;
		}
	}
	return good;
}

#undef SQR

static double
gammln(double xx)
{
	double          x, tmp, ser;
	static double   cof[6] =
	{76.18009173, -86.50532033, 24.01409822,
	 -1.231739516, 0.120858003e-2, -0.536382e-5};
	int             j;
	x = xx - 1.0;
	tmp = x + 5.5;
	tmp -= (x + 0.5) * log(tmp);
	ser = 1.0;
	for (j = 0; j <= 5; j++) {
		x += 1.0;
		ser += cof[j] / x;
	}
	return -tmp + log(2.50662827465 * ser);
}

static double
gammq(double a, double x)
{
	double          gamser, gammcf, gln;
	if (x < 0.0 || a <= 0.0) {
		err("regress: Invalid arguments in routine GAMMQ");
		return 0;
	}
	if (x < (a + 1.0)) {
		gser(&gamser, a, x, &gln);
		return 1.0 - gamser;
	} else {
		gcf(&gammcf, a, x, &gln);
		return gammcf;
	}
}

#define ITMAX 100
#define EPS 3.0e-7

static void
gcf(double *gammcf, double a, double x, double *gln)
{
	int             n;
	double          gold = 0.0, g, fac = 1.0, b1 = 1.0;
	double          b0 = 0.0, anf, ana, an, a1, a0 = 1.0;
	*gln = gammln(a);
	a1 = x;
	for (n = 0; n < ITMAX; n++) {
		an = (double) n;
		ana = an - a;
		a0 = (a1 + a0 * ana) * fac;
		b0 = (b1 + b0 * ana) * fac;
		anf = an * fac;
		a1 = x * a0 + anf * a1;
		b1 = x * b0 + anf * b1;
		if (a1) {
			fac = 1.0 / a1;
			g = b1 * fac;
			if (fabs((g - gold) / g) < EPS) {
				*gammcf = exp(-x + a * log(x) - (*gln)) * g;
				return;
			}
			gold = g;
		}
	}
	err("regress: a too large, ITMAX too small in routine GCF");
	return;
}

#undef ITMAX
#undef EPS

#define ITMAX 100
#define EPS 3.0e-7
static void
gser(double *gamser, double a, double x, double *gln)
{
	int             n;
	double          sum, del, ap;
	*gln = gammln(a);
	if (x <= 0.0) {
		if (x < 0.0) {
			err("regress:x less than 0 in routine GSER");
			return;
		}
		*gamser = 0.0;
		return;
	} else {
		ap = a;
		sum = 1.0 / a;
		del = sum;
		for (n = 0; n < ITMAX; n++) {
			ap += 1.0;
			del *= x / ap;
			sum += del;
			if (fabs(del) < fabs(sum) * EPS) {
				*gamser = sum * exp(-x + a * log(x) - (*gln));
				return;
			}
		}
		err("regress:a too large, ITMAX too small in routine GSER");
		return;
	}
}

#undef ITMAX
#undef EPS

// From table in a book.
double
student_t_025(int nu)
{
	static double t_025[30] = {
		12.706,			// for nu=1
		4.303,			// for nu=2
		3.182,
		2.776,

		2.571,
		2.447,
		2.365,
		2.306,
		2.262,

		2.228,
		2.201,
		2.179,
		2.160,
		2.145,
	
		2.131,
		2.120,
		2.110,
		2.101,
		2.093,
	
		2.086,
		2.080,
		2.074,
		2.069,
		2.064,
	
		2.060,
		2.056,
		2.052,
		2.048,
		2.045,

		2.042			// for nu=30
	};
	if (nu < 1)
		return t_025[0];	// dunno what to do
	else if (nu <= 30)
		return t_025[nu - 1];
	else if (nu <= 40)
		return 2.021;
	else if (nu <= 60)
		return 2.000;
	else if (nu <= 120)
		return 1.98;
	else
		return 1.96;
}
