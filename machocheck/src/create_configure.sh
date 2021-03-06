#!/bin/bash

echo "" > ${DERIVED_FILE_DIR}/configure.h

if [ -n "${IPHONEOS_DEPLOYMENT_TARGET}" ]; then
	echo "#define DEFAULT_IPHONEOS_MIN_VERSION \"${IPHONEOS_DEPLOYMENT_TARGET}\"" >> ${DERIVED_FILE_DIR}/configure.h
else
  if [ -n "${MACOSX_DEPLOYMENT_TARGET}" ]; then
	echo "#define DEFAULT_MACOSX_MIN_VERSION \"${MACOSX_DEPLOYMENT_TARGET}\"" >> ${DERIVED_FILE_DIR}/configure.h
  fi
fi

if [ -z "${RC_SUPPORTED_ARCHS}" ]; then
	RC_SUPPORTED_ARCHS="i386 x86_64 armv7 armv7s"
fi

for ANARCH in ${RC_SUPPORTED_ARCHS}
do
	KNOWN_ARCHS=",armv4t,armv5,armv6,armv7,armv7f,armv7k,armv7s,i386,x86_64,"
	FOUND=`echo "$KNOWN_ARCHS" | grep ",$ANARCH,"`
	if [ $FOUND ]; then
		echo "#define SUPPORT_ARCH_$ANARCH  1" >> ${DERIVED_FILE_DIR}/configure.h
	else
		echo "#error uknown architecture: $ANARCH" >> ${DERIVED_FILE_DIR}/configure.h
	fi
done

echo "#define ALL_SUPPORTED_ARCHS  \"${RC_SUPPORTED_ARCHS}\"" >> ${DERIVED_FILE_DIR}/configure.h


# <rdar://problem/10897631> ld64 hardcodes a reference to /Developer/usr/lib/libLTO.dylib
if [ -n "${DT_TOOLCHAIN_DIR}" ]
then
	echo "-Wl,-lazy_library,${DT_TOOLCHAIN_DIR}/usr/lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
else
	if [ -e "/Developer/usr/lib/libLTO.dylib" ]
	then
		echo "-Wl,-lazy_library,/Developer/usr/lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
	else
		echo "-Wl,-lazy_library,${BUILT_PRODUCTS_DIR}/../lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
	fi
fi

# EOF
