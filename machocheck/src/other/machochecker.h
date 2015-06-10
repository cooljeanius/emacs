/*
 *  machochecker.h
 *  emacs
 *
 *  Created by Eric Gallager on 6/4/15.
 */

#ifndef MACHOCHECKER_H
#define MACHOCHECKER_H 1

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

__attribute__((noreturn)) void throwf(const char* format, ...);
int machocheck_main(int argc, const char* argv[]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MACHOCHECKER_H */

/* EOF */
