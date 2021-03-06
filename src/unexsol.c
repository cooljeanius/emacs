/* unexsol.c: Trivial unexec for Solaris.  */

#include <config.h>
#include "unexec.h"

#include <dlfcn.h>

#include "lisp.h"
#include "character.h"
#include "buffer.h"
#include "charset.h"
#include "coding.h"

#ifndef RTLD_MEMORY
# define RTLD_MEMORY 0x01000
#endif /* !RTLD_MEMORY */

void
unexec (const char *new_name, const char *old_name)
{
  Lisp_Object data;
  Lisp_Object errstring;

  /* The fact that Solaris has a dldump() function that does exactly what Emacs
   * needs here almost makes me want to get a Solaris system: */
  if (! dldump (0, new_name, RTLD_MEMORY))
    return;

  data = list1 (build_string (new_name));
  synchronize_system_messages_locale ();
  errstring = code_convert_string_norecord (build_string (dlerror ()),
					    Vlocale_coding_system, 0);

  xsignal (Qfile_error,
	   Fcons (build_string ("Cannot unexec"), Fcons (errstring, data)));
}

/* EOF */
