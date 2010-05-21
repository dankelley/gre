#include <string>
#include <algorithm>		// for sort
#include <vector>		// part of STL
#include <math.h>
#include "private.h"

//using namespace std;		// for vector

extern char     _grTempString[];

bool            gr_missing(double x); // in gr.h

#if 0				// 2002-nov-2: not used, it seems
void
moment(double *data, int n, double *ave, double *adev, double *sdev, double *svar, double *skew, double *kurt)
{
	if (n < 2)
		*ave = *adev = *sdev = *svar = *skew = *kurt = 0.0;
	else {
		int             ngood = 0, i;
		double          s = 0.0, p;
		for (i = 0; i < n; i++)
			if (!gr_missing((double) (*(data + i)))) {
				s += *(data + i);
				ngood++;
			}
		*ave = s / ngood;
		*adev = (*svar) = (*skew) = (*kurt) = 0.0;
		for (i = 0; i < n; i++) {
			if (!gr_missing((double) (*(data + i)))) {
				*adev += fabs(s = *(data + i) - (*ave));
				*svar += (p = s * s);
				*skew += (p *= s);
				*kurt += (p *= s);
			}
		}
		*adev /= ngood;
		*svar /= (ngood - 1);
		*sdev = sqrt(*svar);
		if (*svar) {
			*skew /= (ngood * (*svar) * (*sdev));
			*kurt = (*kurt) / (ngood * (*svar) * (*svar)) - 3.0;
		}
	}
}
#endif

double
array_at_i(const std::vector<double> &x, double idouble)
{
	unsigned int i = int(floor(idouble));
	unsigned int n = x.size();
	double rval;
	if (i < 0)
		rval = x[0];
	else if (i >= n)
		rval = x[n - 1];
	else {
		double r = idouble - i;
		rval = (r * x[i + 1] * r + (1.0 - r) * x[i]);
	}
	return rval;
}

// calculate q1, q2 = median, and q3 for n data in x
void
histogram_stats(const std::vector<double> &x, unsigned int n, double *q1, double *q2, double *q3)
{
	if (n < 2)
		*q1 = *q2 = *q3 = 0.0;
	else {
		unsigned int ngood = 0;
		std::vector<double> xcopy;
		for (unsigned int i = 0; i < n; i++)
			if (!gr_missing(x[i]))
				xcopy.push_back(x[i]);
		ngood = xcopy.size();
#if 0
		std::sort(xcopy.begin(), xcopy.end());
#endif
		printf("ERROR ... histogram stats produces wrong results %s:%d\n",__FILE__,__LINE__);
		*q1 = array_at_i(xcopy, 0.25 * (ngood - 1));
		*q2 = array_at_i(xcopy, 0.50 * (ngood - 1));
		*q3 = array_at_i(xcopy, 0.75 * (ngood - 1));
	}
}
