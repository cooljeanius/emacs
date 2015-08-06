# This file is used via the maint.mk-using rule in Makefile.

# This is a gnulib-specific rule to keep non-ascii characters
# from creeping into our .texi files.  There is nothing inherently
# wrong with e.g., UTF-8 characters in texinfo sources, but here,
# we had accumulated some non-ascii apostrophes and hyphens, and
# they were recently replaced with their ascii analogs.  This rule
# should help keep things consistent.
sc_keep_gnulib_texi_files_mostly_ascii:
	@prohibit='[^][	 a-zA-Z0-9'\''`~!\@#$$%^&*(){}=+";:,./<>?|\_-]'	\
	in_vc_files='\.texi$$'						\
	halt='invalid bytes in gnulib .texi file'			\
	  $(_sc_search_regexp)

# gnulib skips:
local-checks-to-skip =			\
  sc_GFDL_version			\
  sc_GPL_version			\
  sc_bindtextdomain			\
  sc_cast_of_alloca_return_value	\
  sc_cast_of_argument_to_free		\
  sc_cast_of_x_alloc_return_value	\
  sc_error_message_period		\
  sc_error_message_uppercase		\
  sc_m4_quote_check			\
  sc_makefile_TAB_only_indentation	\
  sc_makefile_at_at_check		\
  sc_makefile_path_separator_check	\
  sc_obsolete_symbols			\
  sc_program_name			\
  sc_prohibit_HAVE_MBRTOWC		\
  sc_prohibit_S_IS_definition		\
  sc_prohibit_always_true_header_tests	\
  sc_prohibit_atoi_atof			\
  sc_prohibit_c_ctype_without_use	\
  sc_prohibit_dirent_without_use	\
  sc_prohibit_getopt_without_use	\
  sc_prohibit_have_config_h		\
  sc_prohibit_magic_number_exit		\
  sc_prohibit_path_max_allocation	\
  sc_prohibit_signal_without_use	\
  sc_prohibit_strcmp			\
  sc_prohibit_strings_without_use	\
  sc_prohibit_undesirable_word_seq	\
  sc_require_config_h			\
  sc_require_config_h_first		\
  sc_space_tab				\
  sc_texinfo_acronym			\
  sc_two_space_separator_in_usage	\
  sc_unmarked_diagnostics		\
  sc_useless_cpp_parens

# emacs skips:
## catches ms-w32-specific defines; not sure what TRT is for that platform:
local-checks-to-skip += sc_Wundef_boolean
## too many for now, and some are wrong:
local-checks-to-skip += sc_file_system
## too many for now:
local-checks-to-skip += sc_keep_gnulib_texi_files_mostly_ascii
## I disagree:
local-checks-to-skip += sc_prohibit_cvs_keyword
## too many for now:
local-checks-to-skip += sc_prohibit_doubled_word
## too many, and some are binary files, or otherwise uneditable:
local-checks-to-skip += sc_prohibit_empty_lines_at_EOF
## I see including stuff without using it as okay:
local-checks-to-skip += sc_prohibit_intprops_without_use
local-checks-to-skip += sc_prohibit_verify_without_use
## meh:
local-checks-to-skip += sc_prohibit_test_double_equal
local-checks-to-skip += sc_prohibit_test_minus_ao
## The one example here of the thing it catches is bad on purpose:
local-checks-to-skip += sc_redundant_const
## WAY too many:
local-checks-to-skip += sc_trailing_blank

empty-at-EOF = /fr/LC_MESSAGES/test-quotearg\.mo$$|^doc/INSTALL(\.(ISO|UTF-8))?
exclude_file_name_regexp--sc_prohibit_empty_lines_at_EOF = \
  ^modules/|^lib/javaversion\.class$$|$(empty-at-EOF)
exclude_file_name_regexp--sc_prohibit_doubled_word = \
  ^(tests/test-init\.sh|lib/base(32|64)\.c)$$
exclude_file_name_regexp--sc_prohibit_openat_without_use = ^lib/selinux-at\.c$$
exclude_file_name_regexp--sc_trailing_blank = \
  ^(.*\.class|build-aux/texinfo\.tex|doc/Copyright/assign\.future\.manual)$$

# Disable strncpy prohibition completely, for now.
exclude_file_name_regexp--sc_prohibit_strncpy = .*

if_before_free_offenders_ =	\
  clean-temp.c			\
  csharpcomp.c			\
  free.c			\
  fstrcmp.c			\
  gl_carray_list.c		\
  glob.c			\
  glthread/lock.c		\
  propername.c			\
  relocatable.c			\
  vasnprintf.c

if_before_free_basename_re_ = \
  $(shell printf '%s\n' '$(if_before_free_offenders_)' \
    |sed 's|\.c\>||g'|tr -s '[\n[:space:]]' '|')

exclude_file_name_regexp--sc_avoid_if_before_free = \
  ^lib/($(if_before_free_basename_re_))\.c$$

exclude_file_name_regexp--sc_prohibit_defined_have_decl_tests = \
  ^lib/((error|getopt|strtoimax)\.c|(argp-namefrob)\.h)$$
