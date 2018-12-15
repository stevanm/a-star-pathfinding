#include <stdio.h>
#include <stdlib.h>

#ifndef __CHECK_ERROR_H__
#define __CHECK_ERROR_H__

/*
 * Check error macro
 * */
 
#define check_error(expr, userMsg)	\
	do {\
		if (!(expr)) {	\
			fprintf(stderr, "%s\n", userMsg);\
			exit(EXIT_FAILURE); \
		}	\
	} while (0)


#endif
