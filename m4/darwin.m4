# darwin.m4 serial 1                                       -*- Autoconf -*-
dnl# extracted from the libtool required darwin checks

# EMACS_REQUIRED_DARWIN_CHECKS
# -------------------------
AC_DEFUN([EMACS_REQUIRED_DARWIN_CHECKS],[
  dnl# use this instead of requiring, because if we require it too early,
  dnl# it will mess up the flags we pass to it:
  AC_BEFORE([AC_PROG_CC],[$0])dnl

  dnl# this one we can actually require:
  AC_REQUIRE([AC_PROG_GREP])dnl
  
  case ${host_os} in
    rhapsody* | darwin*)
    AC_CHECK_TOOL([DSYMUTIL],[dsymutil],[:])
    AC_CHECK_TOOL([NMEDIT],[nmedit],[:])
    AC_CHECK_TOOL([LIPO],[lipo],[:])
    AC_CHECK_TOOL([OTOOL],[otool],[:])
    if test "x${OTOOL}" = "x:"; then
      AC_CHECK_TOOL([OTOOL64],[otool64],[:])
    fi
    dnl# (libtool decls removed)

    AC_CACHE_CHECK([for -single_module linker flag],
      [emacs_cv_apple_cc_single_mod],
      [emacs_cv_apple_cc_single_mod=no
      if test -z "${EMACS_MULTI_MODULE}"; then
	# By default we will add the -single_module flag. You can override
	# by either setting the environment variable EMACS_MULTI_MODULE
	# non-empty at configure time, or by adding -multi_module to the
	# link flags.
	rm -rf libconftest.dylib*
	echo "int foo(void){return 1;}" > conftest.c
	echo "${CC} ${CFLAGS} ${LDFLAGS} -o libconftest.dylib \
-dynamiclib -Wl,-single_module conftest.c" >&AS_MESSAGE_LOG_FD
	${CC} ${CFLAGS} ${LDFLAGS} -o libconftest.dylib \
	  -dynamiclib -Wl,-single_module conftest.c 2>conftest.err
        _emacs_result=$?
	# If there is a non-empty error log, and "single_module"
	# appears in it, assume the flag caused a linker warning
        if test -s conftest.err && ${GREP} single_module conftest.err; then
	  cat conftest.err >&AS_MESSAGE_LOG_FD
	# Otherwise, if the output was created with a 0 exit code from
	# the compiler, it worked.
	elif test -f libconftest.dylib && test ${_emacs_result} -eq 0; then
	  emacs_cv_apple_cc_single_mod=yes
	else
	  cat conftest.err >&AS_MESSAGE_LOG_FD
	fi
	rm -rf libconftest.dylib*
	rm -f conftest.*
      fi])dnl

    AC_CACHE_CHECK([for -exported_symbols_list linker flag],
      [emacs_cv_ld_exported_symbols_list],
      [emacs_cv_ld_exported_symbols_list=no
      save_LDFLAGS="${LDFLAGS}"
      echo "_main" > conftest.sym
      LDFLAGS="${LDFLAGS} -Wl,-exported_symbols_list,conftest.sym"
      AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[]])],
	[emacs_cv_ld_exported_symbols_list=yes],
	[emacs_cv_ld_exported_symbols_list=no])
	LDFLAGS="${save_LDFLAGS}"
    ])dnl

    AC_REQUIRE([AM_PROG_AR])dnl
    AC_REQUIRE([AC_PROG_RANLIB])dnl

    AC_CACHE_CHECK([for -force_load linker flag],[emacs_cv_ld_force_load],
      [emacs_cv_ld_force_load=no
      cat > conftest.c << _EMACS_EOF
int forced_loaded() { return 2;}
_EMACS_EOF
      echo "${CC} ${CFLAGS} -c -o conftest.o conftest.c" >&AS_MESSAGE_LOG_FD
      ${CC} ${CFLAGS} -c -o conftest.o conftest.c 2>&AS_MESSAGE_LOG_FD
      echo "${AR} cru libconftest.a conftest.o" >&AS_MESSAGE_LOG_FD
      ${AR} cru libconftest.a conftest.o 2>&AS_MESSAGE_LOG_FD
      echo "${RANLIB} libconftest.a" >&AS_MESSAGE_LOG_FD
      ${RANLIB} libconftest.a 2>&AS_MESSAGE_LOG_FD
      cat > conftest.c << _EMACS_EOF
int main() { return 0;}
_EMACS_EOF
      echo "${CC} ${CFLAGS} ${LDFLAGS} -o conftest conftest.c -Wl,-force_load,./libconftest.a" >&AS_MESSAGE_LOG_FD
      ${CC} ${CFLAGS} ${LDFLAGS} -o conftest conftest.c -Wl,-force_load,./libconftest.a 2>conftest.err
      _emacs_result=$?
      if test -s conftest.err && ${GREP} force_load conftest.err; then
	cat conftest.err >&AS_MESSAGE_LOG_FD
      elif test -f conftest && test ${_emacs_result} -eq 0 && ${GREP} forced_load conftest >/dev/null 2>&1; then
	emacs_cv_ld_force_load=yes
      else
	cat conftest.err >&AS_MESSAGE_LOG_FD
      fi
        rm -f conftest.err libconftest.a conftest conftest.c
        rm -rf conftest.dSYM
    ])dnl

    dnl# why do we suddenly switch to using the '${wl}' variable now when
    dnl# we had been hardcoding it before? Anyways, make sure it is set:
    if test "x${wl}" = "x"; then
      test -z "${wl}" && export wl='-Wl,'
    fi
    case ${host_os} in
    rhapsody* | darwin1.[[012]])
      _emacs_dar_allow_undefined='${wl}-undefined ${wl}suppress' ;;
    darwin1.*)
      _emacs_dar_allow_undefined='${wl}-flat_namespace ${wl}-undefined ${wl}suppress' ;;
    darwin*) # darwin 5.x on
      # if running on 10.5 or later, the deployment target defaults
      # to the OS version, if on x86, and 10.4, the deployment
      # target defaults to 10.4. Do you not love it?
      case ${MACOSX_DEPLOYMENT_TARGET-10.0},${host} in
	10.0,*86*-darwin8*|10.0,*-darwin[[91]]*)
	  _emacs_dar_allow_undefined='${wl}-undefined ${wl}dynamic_lookup' ;;
	10.[[012]]*)
	  _emacs_dar_allow_undefined='${wl}-flat_namespace ${wl}-undefined ${wl}suppress' ;;
	10.*)
	  _emacs_dar_allow_undefined='${wl}-undefined ${wl}dynamic_lookup' ;;
      esac
    ;;
  esac
    if test "x${emacs_cv_apple_cc_single_mod}" = "xyes"; then
      _emacs_dar_single_mod='${single_module}'
    fi
    if test "x${emacs_cv_ld_exported_symbols_list}" = "xyes"; then
      _emacs_dar_export_syms=' ${wl}-exported_symbols_list,${output_objdir}/${libname}-symbols.expsym'
    else
      _emacs_dar_export_syms='~${NMEDIT} -s ${output_objdir}/${libname}-symbols.expsym ${lib}'
    fi
    if test "x${DSYMUTIL}" != "x:" && test "x${emacs_cv_ld_force_load}" = "xno"; then
      _emacs_dsymutil='~${DSYMUTIL} ${lib} || echo "dsymutil failure"'
    else
      _emacs_dsymutil=''
    fi
    ;;
  esac
])dnl
