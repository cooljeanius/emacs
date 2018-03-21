/* test_mach_o_binary.c -*- C -*- */

/* This file is a way of testing the various addresses of parts of a
 * mach-o binary.
 * Adapted from the manpages of sbrk(3) and end(3):
 * The sbrk function is a historical curiosity left over from earlier days
 * before the advent of virtual memory management.
 *
 * The current value of the program break (i.e., the lowest address of a
 * process's data segment (uninitialized data)) is reliably returned by
 * ``sbrk(0)''.  The getrlimit(2) system call may be used to determine the
 * maximum permissible size of the data segment; it will not be possible
 * to set the break beyond the rlim_max value returned from a call to
 * getrlimit, e.g.  ``qetext + rlp->rlim_max.''
 * (I assume "qetext" is the same as "get_etext()"?)
 *
 * The get_end(), get_etext(), and get_edata() routines provide a stopgap
 * measure to programs that use the UNIX link-editor defined symbols.
 * Apple very strongly discourages use of these routines.  The problem is
 * that any program that is using UNIX link editor defined symbols
 * (_end, _etext, or _edata) is making assumptions that the program has
 * the memory layout of a UNIX program.
 *
 * This is an incorrect assumption for a program built by the Mach-O link
 * editor.  The reason that these routines are even provided is that, if
 * very minimal assumptions about the layout are used, and the default
 * format and memory layout of the Mach-O link editor is used to build the
 * program, then some things may work by using the values returned by
 * these routines in place of the addresses of their UNIX link-editor
 * defined symbols.  So use at your own risk, and only if you know what
 * you are doing.  Or better yet, convert the program to use the
 * appropriate Mach or Mach-O functions.  If you are trying to find out
 * about your address space, then use vm_region(2), and if you are trying
 * to find out where your program is loaded, then use the dyld(3)
 * functions.
 *
 * The values of the UNIX link-editor defined symbols _etext, _edata, and
 * _end are returned by the routines get_etext(), get_edata(), and
 * get_end() respectively.  In a Mach-O file they have the following
 * values:  get_etext() returns the 1st address after the (__TEXT,__text)
 * section; note that this may or may not be the only section in the
 * __TEXT segment.  get_edata() returns the 1st address after the
 * (__DATA,__data) section; note that this may or may not be the last
 * non-zero fill section in the __DATA segment.  get_end() returns the 1st
 * address after the last segment in the executable; note that a shared
 * library may be loaded at this address.
 */

#include "src/config.h"
#if defined(HAVE_STDALIGN_H) && defined(BUILDING_FROM_XCODE)
# undef HAVE_STDALIGN_H
#endif /* HAVE_STDALIGN_H && BUILDING_FROM_XCODE */
#include <stdio.h> /* for printf() */
#include <unistd.h> /* for sbrk() */
#include <mach-o/getsect.h> /* for get_end() and friends */
#include <sys/resource.h> /* for getrlimit() */
#include <errno.h> /* getrlimit() may set errno */
#include <string.h> /* for strerror() */
#include <assert.h> /* for assert() */
#if !defined(__OPTIMIZE__)
/* FIXME: warning or something */
#endif /* !__OPTIMIZE__ */
#include "src/lisp.h"

/* in case gnulib re-defined this on us: */
#if !defined(__clang_analyzer__) && defined(strerror)
# if defined(_GL_STRING_H) || defined(_GL_ALREADY_INCLUDING_STRING_H)
#  undef strerror
# endif /* _GL_STRING_H || _GL_ALREADY_INCLUDING_STRING_H */
#endif /* !__clang_analyzer__ && strerror */

int main(void)
{
    struct rlimit *rlp = (struct rlimit *)malloc(sizeof(struct rlimit));
    unsigned long long int max_data_seg_size;
    Lisp_Object end;
    int err = getrlimit(RLIMIT_DATA, rlp);
    if (err == -1) {
        fprintf(stderr, "getrlimit() failed w/errno %d (i.e. \"%s\").\n",
                errno, strerror(errno));
        assert((errno == EFAULT) || (errno == EINVAL));
        exit(EXIT_FAILURE);
    }
    max_data_seg_size = (unsigned long long int)((rlim_t)get_etext()
                                                 + rlp->rlim_max);
    XSETINT(end, ((intptr_t)(char *)sbrk(0) / 1024));
    printf("sbrk(0)      returned %lld, aka %p.\n",
           (long long int)(intptr_t)sbrk(0), sbrk(0));
#if !defined(CHECK_LISP_OBJECT_TYPE) && defined(XINT) && defined(XUINT)
    printf("emacs would see that as %lld, aka 0x%llx.\n",
           XINT(end), (unsigned long long int)XUINT(end));
#else
    IF_LINT((void)end);
#endif /* !CHECK_LISP_OBJECT_TYPE && XINT && XUINT */
    printf("get_etext()  returned %lu, aka 0x%lx.\n", get_etext(),
           get_etext());
    printf("get_edata()  returned %lu, aka 0x%lx.\n", get_edata(),
           get_edata());
    printf("get_end()    returned %lu, aka 0x%lx.\n", get_end(),
           get_end());
    printf("max_data_seg_size is %llu, i.e. (%llu + %llu).\n",
           max_data_seg_size, (rlim_t)get_etext(), rlp->rlim_max);
    printf("in hex, that is 0x%llx, i.e. (0x%llx + 0x%llx).\n",
           max_data_seg_size, (rlim_t)get_etext(), rlp->rlim_max);
    free(rlp);
}
