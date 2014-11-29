# blocks.m4 serial 1                                       -*- Autoconf -*-
# originally taken from libdispatch, the entire project of which was
# released with the following license:
# Copyright (C) 2008-2012 Apple, Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
# (The Apache-2 license is compatible with GPL-3, so it should be okay
# to use it here)

AC_DEFUN([DISPATCH_C_BLOCKS],[
#
# Allow configure to be passed a path to the directory where it should look
# for the Blocks runtime library, if any.
#
AC_ARG_WITH([blocks-runtime],
  [AS_HELP_STRING([--with-blocks-runtime],
    [Specify path to the blocks runtime])],
  [blocks_runtime=${withval}
    LIBS="${LIBS} -L${blocks_runtime}"])dnl

#
# Detect compiler support for Blocks; perhaps someday -fblocks will NOT be
# required, in which case we will need to change this.
#
AC_CACHE_CHECK([for C Blocks support],[dispatch_cv_cblocks],[
  saveCFLAGS="${CFLAGS}"
  CFLAGS="${CFLAGS} -fblocks"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[(void)^{int i; i = 0; }();])],[
    CFLAGS="${saveCFLAGS}"
    dispatch_cv_cblocks="-fblocks"
  ],[
    CFLAGS="${saveCFLAGS}"
    dispatch_cv_cblocks="no"
  ])dnl
])dnl

AS_IF([test "x${dispatch_cv_cblocks}" != "xno"],[
    export CBLOCKS_FLAGS="${dispatch_cv_cblocks}"

    #
    # It may be necessary to directly link the Blocks runtime on some
    # systems, so give it a try if we cannot link a C program that uses
    # Blocks. We will want to remove this at somepoint, as really -fblocks
    # should force that linkage already.
    #
    saveCFLAGS="${CFLAGS}"
    CFLAGS="${CFLAGS} -fblocks -O0"
    AC_MSG_CHECKING([whether additional libraries are required for the Blocks runtime])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[
	^{ int j; j=0; }();
    ]])],[
      AC_MSG_RESULT([no])
    ],[
      saveLIBS="${LIBS}"
      LIBS="${LIBS} -lBlocksRuntime"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[
	^{ int k; k=0; }();
      ]])],[
	AC_MSG_RESULT([-lBlocksRuntime])
      ],[
	AC_MSG_ERROR([cannot find Blocks runtime])
      ])
    ])
    CFLAGS="${saveCFLAGS}"
    have_cblocks=true
    AC_DEFINE([CAN_USE_CBLOCKS],[1],
              [Define to 1 if the Apple "Blocks" extension works in C])
],[
    CBLOCKS_FLAGS=""
    have_cblocks=false
])
AM_CONDITIONAL([HAVE_CBLOCKS],[${have_cblocks}])
AC_SUBST([CBLOCKS_FLAGS])dnl

#
# Because a different C++ compiler may be specified than C compiler,
# we have to do it again for C++.
#
AC_LANG_PUSH([C++])
AC_CACHE_CHECK([for C++ Blocks support],[dispatch_cv_cxxblocks],[
  saveCXXFLAGS="${CXXFLAGS}"
  CXXFLAGS="${CXXFLAGS} -fblocks"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],[(void)^{int i; i = 0; }();])],[
    CXXFLAGS="${saveCXXFLAGS}"
    dispatch_cv_cxxblocks="-fblocks"
  ],[
    CXXFLAGS="${saveCXXFLAGS}"
    dispatch_cv_cxxblocks="no"
  ])dnl
])dnl

AS_IF([test "x${dispatch_cv_cxxblocks}" != "xno"],[
    export CXXBLOCKS_FLAGS="${dispatch_cv_cxxblocks}"

    saveCXXFLAGS="${CXXFLAGS}"
    CXXFLAGS="${CXXFLAGS} -fblocks -O0"
    AC_MSG_CHECKING([whether additional libraries are required for the Blocks runtime])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[
	^{ int j; j=0; }();
    ]])],[
      AC_MSG_RESULT([no])
    ],[
      saveLIBS="${LIBS}"
      LIBS="${LIBS} -lBlocksRuntime"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[
	^{ int k; k=0; }();
      ]])],[
	AC_MSG_RESULT([-lBlocksRuntime])
      ],[
	AC_MSG_ERROR([cannot find Blocks runtime])
      ])
    ])
    CXXFLAGS="${saveCXXFLAGS}"
    have_cxxblocks=true
    AC_DEFINE([CAN_USE_CXXBLOCKS],[1],
              [Define to 1 if the Apple "Blocks" extension works in C++])
],[
    CXXBLOCKS_FLAGS=""
    have_cxxblocks=false
])
AC_LANG_POP
AM_CONDITIONAL([HAVE_CXXBLOCKS],[${have_cxxblocks}])
AC_SUBST([CXXBLOCKS_FLAGS])dnl
])dnl
