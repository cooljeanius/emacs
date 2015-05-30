# acl_old.m4 serial 1                                      -*- Autoconf -*-
# tests removed from the current acl.m4, preserved here just in case.

AC_DEFUN([gl_FILE_HAS_XATTR_FOR_ACL],[
  AC_REQUIRE([AC_TYPE_SSIZE_T])dnl
  AC_REQUIRE([gl_FUNC_ACL_ARG])dnl
  ## if that worked, then:
  if test "x${enable_acl}" != "xno"; then
    AC_CACHE_CHECK([for getxattr with XATTR_NAME_POSIX_ACL macros],
      [gl_cv_getxattr_with_posix_acls],
      [gl_cv_getxattr_with_posix_acls=no
       AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/types.h>
#include <sys/xattr.h>
#include <linux/xattr.h>
            ]],[[
  ssize_t a = getxattr(".", XATTR_NAME_POSIX_ACL_ACCESS, 0, 0);
  ssize_t b = getxattr(".", XATTR_NAME_POSIX_ACL_DEFAULT, 0, 0);
  return (a < 0) || (b < 0);
            ]])],
         [gl_cv_getxattr_with_posix_acls=yes])])dnl
    ## ok, that should be enough of that.
  fi
  if test "x${gl_cv_getxattr_with_posix_acls}" = "xyes"; then
    LIB_HAS_XATTR_FOR_ACL=""
    AC_DEFINE([GETXATTR_WITH_POSIX_ACLS],[1],
      [Define to 1 if getxattr works with XATTR_NAME_POSIX_ACL_ACCESS
       and XATTR_NAME_POSIX_ACL_DEFAULT.])dnl
    ## end corresponding define
  else
    dnl# Set gl_need_lib_has_xattr_for_acl to a nonempty value, so that any
    dnl# later gl_FUNC_ACL call will set LIB_HAS_XATTR_FOR_ACL="${LIB_ACL}"
    gl_need_lib_has_xattr_for_acl=1
    LIB_HAS_XATTR_FOR_ACL="${LIB_ACL}"
  fi
  AC_SUBST([LIB_HAS_XATTR_FOR_ACL])dnl
])dnl
