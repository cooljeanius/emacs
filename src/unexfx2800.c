/* unexfx2800.o: Unexec for the Alliant FX/2800.  */

#include <stdio.h>

extern int unexec(char *, char *, unsigned int, unsigned int, unsigned int);
int
unexec(char *new_name, char *a_name, unsigned int data_start,
	   unsigned int bss_start, unsigned int entry_address)
{
  int stat_stat = 0;
    
  stat_stat = elf_write_modified_data(a_name, new_name);
  if (stat_stat < 0)
    perror("emacs: elf_write_modified_data");
  else if (stat_stat > 0)
    fprintf(stderr, "Unspecified error from elf_write_modified_data.\n");
  return stat_stat;
}
