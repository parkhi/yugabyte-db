/*
 *	string.h
 *		string handling helpers
 *
 *	Portions Copyright (c) 1996-2018, PostgreSQL Global Development Group
 *	Portions Copyright (c) 1994, Regents of the University of California
 *
 *	src/include/common/string.h
 */
#ifndef COMMON_STRING_H
#define COMMON_STRING_H

/* functions in src/common/string.c */
extern bool pg_str_endswith(const char *str, const char *end);
extern int strtoint(const char *pg_restrict str, char **pg_restrict endptr,
		 int base);

extern int	pg_strip_crlf(char *str);

/* functions in src/common/pg_get_line.c */
extern char *pg_get_line(FILE *stream);

/* functions in src/common/sprompt.c */
extern char *simple_prompt(const char *prompt, bool echo);

#endif							/* COMMON_STRING_H */
