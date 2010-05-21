#if !defined(_SUBS_H_)
#define _SUBS_H_

#include <string>
#include "parser.h"

using namespace std;

/*
 * User subroutines
 */
class user_sub
{
 public:
	user_sub() {
		action = NULL;
		min_args = max_args = -1;
	}
	user_sub(const struct wnode* n, char* prototype, struct cmdnode* a);
	user_sub(const user_sub& n) { /* needed for STL */
		name = n.get_name();
		action = n.get_action();
		min_args = n.get_min_args();
		max_args = n.get_max_args();
	};
	bool operator<  (const user_sub& ignored) const {return false;}
	bool operator== (const user_sub& ignored) const {return false;}
	user_sub& operator=(const user_sub& n) { /* needed for STL */
		name   = n.get_name();
		action = n.get_action();
		min_args = n.get_min_args();
		max_args = n.get_max_args();
		return *this;
	}
	~user_sub() {;}
	string   get_name()   const { printf("subs.h/get_name()\n");printf("and the name is '%s'\n",name.c_str());printf("OK?\n");return name;  }
	cmdnode*     get_action()       {return action;}
	cmdnode*     get_action() const {return action;}
	int get_min_args() const { return min_args; } 
	int get_max_args() const { return max_args; } 
 private:
	int min_args, max_args;
	string name;
	cmdnode* action;
};

typedef struct {
	double(*fcn)();
	const char *name;
} SUB0;
typedef struct {
	double(*fcn)(double);
	const char *name;
} SUB1;
typedef struct {
	double(*fcn)();
	const char *name;
} SUB0_TEXT;
typedef struct {
	double(*fcn)(const char *);
	const char *name;
} SUB1_TEXT;
typedef struct {
	double(*fcn)(const char *, const char *);
	const char *name;
} SUB2_TEXT;
typedef struct {
	double(*fcn)(double, double);
	const char *name;
} SUB2;
typedef struct {
	double(*fcn)(double, double, double);
	const char *name;
} SUB3;
typedef struct {
	double(*fcn)(double, double, double, double);
	const char *name;
} SUB4;
#endif /* _SUBS_H_ */
