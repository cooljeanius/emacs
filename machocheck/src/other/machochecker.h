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
	
#ifdef emacs
typedef enum relation_to_dumping_possibilities {
	UNRELATED_TO_DUMPING = 0,
	INPUT_FILE_TO_BE_DUMPED = 1,
	OUTPUT_FILE_OF_DUMPING = 2
} dumpingness;
extern dumpingness related_to_dump_p;
#endif /* emacs */

extern int last_external_reloc_num;

__attribute__((noreturn)) void throwf(const char* format, ...);
extern int machocheck_main(int argc, const char* argv[]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MACHOCHECKER_H */

/* EOF */
