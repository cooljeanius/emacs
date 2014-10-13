# prototypes_extra.m4 serial 1 file starts here
dnl# This was originally in the emacs configure.ac file.   -*- Autoconf -*-

AC_DEFUN([AC_REQUIRE_PROTOTYPES],[
  AC_REQUIRE([AC_C_PROTOTYPES])dnl
  AC_LANG_CASE([C],[
    m4_ifdef([AX_CFLAGS_STRICT_PROTOTYPES],[
      AC_REQUIRE([AX_CFLAGS_STRICT_PROTOTYPES])dnl
    ],[
      :
    ])dnl# end ifdef wrapper
  ],[C++],[
    m4_ifdef([AX_CXXFLAGS_STRICT_PROTOTYPES],[
      AC_REQUIRE([AX_CXXFLAGS_STRICT_PROTOTYPES])dnl
    ],[
      :
    ])dnl# end ifdef wrapper
  ])dnl# end switch and case on language used

  dnl# I would like to use some AX PROTOTYPE checks here, but they seem
  dnl# to be broken...
  
])dnl# end macro

# prototypes_extra.m4 ends here
