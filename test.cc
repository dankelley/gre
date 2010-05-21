#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define search_for "^d"
#define N 2
char *pattern[N] = {
	"dan",
	"adan"
};

int
main()
{
	regex_t compiled;
	int compilation_result = regcomp(&compiled, search_for, REG_EXTENDED);
	if (compilation_result != 0) {
		perror("Cannot compile expression\n");
	}
	for (unsigned int i = 0; i < N; i++) {
		int match_result = regexec(&compiled, pattern[i], 0, 0, 0);
		printf("Does '%s' match to '%s'?  %s\n",
		       search_for, pattern[i], match_result==0 ? "yes" : "no");
	}
	return 0;
}
