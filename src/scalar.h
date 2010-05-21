// -*- mode: c++ -*-
// Store object which may hold either numeric or string value.

#if !defined(_SCALAR_H_)
#define _SCALAR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

class scalar {
private:
	double *n;
	char **s;
	bool *numeric;
	unsigned int len;
	unsigned int capacity;
public:
	scalar();
	scalar(const scalar& scalar_source);
	scalar(const char* s_source);
	scalar(double n_source);
	void expand(unsigned int capacity_new);
	void free_storage();
	~scalar();
	unsigned int get_len() const { return len;}
	unsigned int get_capacity() const { return capacity;}
	bool is_numeric(unsigned int i) const;
	bool is_numeric() const {return is_numeric(0);}
	char *get_text(unsigned int i) const;
	char *get_text() const {return get_text(0);}
	double get_numeric() const {return get_numeric(0);}
	double get_numeric(unsigned int i) const;
	void set_numeric(unsigned int i, double n_source);
	void set_numeric(double n_source) {set_numeric(0, n_source);}
	void set_text(unsigned int i, const char* s_source);
	void set_text(const char* s_source) {set_text(0, s_source);}
	bool operator<  (const scalar& ignored) const { // fake for STL
		return false;
	}
	bool operator== (const scalar& ignored) const { // fake for STL
		return false;
	}
	scalar& operator=(const scalar& new_scalar);
};

#endif // _SCALAR_H_
