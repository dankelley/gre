// -*- mode: c++ -*-
// 'scalar' (old name!!) stores list of numeric/string values

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "scalar.h"

void gre_exit(int code);

void scalar::free_storage()
{
	printf("scalar::free_storage() has capacity=%d   s@%lx\n",capacity,long(s));
	if (capacity != 0) {
		for (unsigned int i = 0; i < capacity; i++)
			free(s[i]);
		free(n);
		free(s);
		free(numeric);
		len = 0;
		capacity = 0;
	}
}

scalar::~scalar()
{
	free_storage();
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::~scalar() at %lx  [%s:%d]\n", (unsigned long)this,__FILE__,__LINE__);
}


// resize to 'capacity_new-1' (or more) items
void scalar::expand(unsigned int capacity_new) 
{
	printf("!!!! Doing a scalar::expand(%d) this=%lx     [%s:%d]\n",capacity_new,(unsigned long)this,__FILE__,__LINE__);
	unsigned int capacity_old = capacity;

	gre_assert(capacity >= 1, "scalar::expand() detected existing capacity of 0");

	// Increase by tens instead of doubling, since if it's not the
	// usual case of 1, it may be some tens of items.
	while (capacity < capacity_new)
		capacity = capacity + 10;
	bool *numeric_new = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric_new, "scalar::expand() is out of memory");
	double *n_new = (double*)malloc(capacity * sizeof(double));
	gre_assert(n_new, "scalar::expand() is out of memory");
	char **s_new = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s_new, "scalar::expand() is out of memory");

	for (unsigned int i = 0; i < capacity; i++) {
		// copy existing data, and blank out the extra capacity
		if (i < len) {
			s_new[i] = (char*)malloc(sizeof(char) * (1 + strlen(s[i]))); 
			gre_assert(s_new[i], "scalar::expand() is out of memory");
			strcpy(s_new[i], s[i]);
			n_new[i] = n[i];
			numeric_new[i] = numeric[i];
		} else {
			s_new[i] = (char*)malloc(sizeof(char));
			gre_assert(s_new[i], "scalar::expand() is out of memory");
			strcpy(s_new[i], "");
			n_new[i] = 0.0;
			numeric_new[i] = true;
		}
	}
	unsigned int capacity_keep = capacity;

	capacity = capacity_old;
	printf("scalar:expand about to free_storage; s@%lx   snew@%lx\n",(unsigned long)s, (unsigned long)s_new);
	free_storage();
	s = s_new;
	n = n_new;
	numeric = numeric_new;
	capacity = capacity_keep;
	printf("scalar::expand() set capacity=%d   s@%lx\n",capacity,(unsigned long)s);
}

scalar::scalar()  
{ 
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::scalar() at %lx  [%s:%d]\n", (unsigned long)this,__FILE__,__LINE__);
	capacity = 1;
	len = 1;
	numeric = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric, "scalar::scalar() is out of memory");
	n = (double*)malloc(capacity * sizeof(double));
	gre_assert(n, "scalar::scalar() is out of memory");
	s = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s, "scalar::scalar() is out of memory");
	s[0] = (char*)malloc(sizeof(char));
	gre_assert(s[0], "scalar::scalar() is out of memory");
	n[0] = 0.0;
	strcpy(s[0], "");
	numeric[0] = true;
	printf("scalar::scalar() set capacity=%d   s@%lx\n",capacity,(unsigned long)s);
}

scalar::scalar(double n_source)
{ 
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::scalar(%f) at %lx  [%s:%d]\n", n_source, (unsigned long)this,__FILE__,__LINE__);
	len = 1;
	capacity = 1;
	numeric = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric, "scalar::scalar(double) is out of memory");
	n = (double*)malloc(capacity * sizeof(double));
	gre_assert(n, "scalar::scalar(double) is out of memory");
	s = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s, "scalar::scalar(double) is out of memory");
	s[0] = (char*)malloc(sizeof(char));
	gre_assert(s[0], "scalar::scalar(double) is out of memory");
	n[0] = n_source;
	strcpy(s[0], "");
	numeric[0] = true;
	printf("scalar::scalar(double) set capacity=%d   s@%lx\n",capacity,(unsigned long)s);
}

scalar::scalar(const char* s_source)
{ 
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::scalar(%s) at %lx  [%s:%d]\n", s_source, (unsigned long)this,__FILE__,__LINE__);
	len = 1;
	capacity = 1;
	numeric = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric, "scalar::scalar(char*) is out of memory");
	n = (double*)malloc(capacity * sizeof(double));
	gre_assert(n, "scalar::scalar(char*) is out of memory");
	s = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s, "scalar::scalar(char*) is out of memory");
	s[0] = (char*)malloc(sizeof(char)*(1 + strlen(s_source)));
	gre_assert(s[0], "scalar::scalar(char*) is out of memory");
	strcpy(s[0], s_source);
	n[0] = 0.0;
	numeric[0] = false;
	printf("scalar::scalar(char*) set capacity=%d   s@%lx\n",capacity,(unsigned long)s);
}

scalar::scalar(const scalar& scalar_source)
{
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::scalar(scalar at %lx) at %lx  [%s:%d]\n", (unsigned long)(&scalar_source), (unsigned long)this,__FILE__,__LINE__);
//	free_storage();		// added 2003-jul-13
	len = scalar_source.get_len();
	capacity = scalar_source.get_capacity();
	printf("%s:%d scalar::scalar(scalar) copying scalar of size %d  this %lx   source %lx\n",__FILE__,__LINE__,capacity, (unsigned long)this, (unsigned long)&scalar_source);
	numeric = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric, "scalar::scalar(scalar) is out of memory");
	n = (double*)malloc(capacity * sizeof(double));
	gre_assert(n, "scalar::scalar(scalar) is out of memory");
	s = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s, "scalar::scalar(scalar) is out of memory");
	for (unsigned int i = 0; i < capacity; i++) {
		s[i] = (char*)malloc(sizeof(char) * (1 + strlen(scalar_source.get_text(i))));
		gre_assert(s[i], "scalar::scalar(scalar) is out of memory");
		n[i] = scalar_source.get_numeric(i);
		strcpy(s[i], scalar_source.get_text(i));
		numeric[i] = scalar_source.is_numeric(i);
	}
	printf("scalar::scalar COPY set capacity=%d   s@%lx\n",capacity,(unsigned long)s);

}

scalar& scalar::operator=(const scalar& scalar_source)
{
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::operator=(scalar at %lx) at %lx  [%s:%d]\n", (unsigned long)(&scalar_source), (unsigned long)this,__FILE__,__LINE__);
//	free_storage();
	len = scalar_source.get_len();
	capacity = scalar_source.get_capacity();
	//printf("%s:%d scalar::operator= copying scalar of size %d\n",__FILE__,__LINE__,capacity);
	numeric = (bool*)malloc(capacity * sizeof(bool));
	gre_assert(numeric, "scalar::operator=() is out of memory");
	n = (double*)malloc(capacity * sizeof(double));
	gre_assert(n, "scalar:::operator=() is out of memory");
	s = (char**)malloc(capacity * sizeof(char*));
	gre_assert(s, "scalar:::operator=() is out of memory");
	for (unsigned int i = 0; i < capacity; i++) {
		s[i] = (char*)malloc(sizeof(char) * (1 + strlen(scalar_source.get_text(i))));
		gre_assert(s[i], "scalar:::operator=() is out of memory");
		n[i] = scalar_source.get_numeric(i);
		strcpy(s[i], scalar_source.get_text(i));
		numeric[i] = scalar_source.is_numeric(i);
	}
	printf("scalar::scalar ASSIGN set capacity=%d   s@%lx\n",capacity,(unsigned long)s);
	return *this;
}

void scalar::set_numeric(unsigned int i, double n_source) {
	if (i >= len)
		expand(i + 1);
	n[i] = n_source;
	strcpy(s[i], "");
	numeric[i] = true;
	if (i + 1 > len)
		len = i + 1;
	if (_gre_debug & DEBUG_VARIABLES) printf("Set scalar this=%lx to numeric %f\n",(unsigned long)this,n_source);
}

void scalar::set_text(unsigned int i, const char* s_source) 
{
	if (i >= len)
		expand(i + 1);
	if (strlen(s_source) > strlen(s[i])) {
		free(s[i]);
		s[i] = (char*)malloc(sizeof(char) * (1 + strlen(s_source)));
		gre_assert(s[i], "scalar:::set_text() is out of memory");
	}
	strcpy(s[i], s_source); 
	numeric[i] = false;
	n[i] = 0.0;
	if (i + 1 > len)
		len = i + 1;
}

char* scalar::get_text(unsigned int i) const 
{
	if (i < len) 
		return s[i];
	else
		return "";
}

double scalar::get_numeric(unsigned int i = 0) const
{
	if (i < len) {
		if (!numeric[i]) {
			double nn = atof(s[i]);
			return nn;
		} else {
			return n[i];
		}
	} else {
		return 0.0;
	}
}

bool scalar::is_numeric(unsigned int i = 0) const
{
	if (_gre_debug & DEBUG_VARIABLES) printf("    scalar::is_numeric this=\%lx (i=%d  len=%d) numeric=%d  [%s:%d]\n",(unsigned long)this,i,len,numeric[i],__FILE__,__LINE__);
	if (i < len)
		return numeric[i];
	else
		return false;
}
