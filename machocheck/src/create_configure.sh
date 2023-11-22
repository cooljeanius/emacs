#!/bin/bash

if [ -z "${DERIVED_SOURCES_DIR}" ]; then
	echo "error: create_configure.sh expects DERIVED_SOURCES_DIR to be set!" >&2
	exit 1
fi
if [ -z "${DERIVED_FILE_DIR}" ]; then
	echo "error: create_configure.sh expects DERIVED_FILE_DIR to be set!" >&2
	exit 1
fi

echo "/* configure.h is generated from create_configure.sh */" > ${DERIVED_FILE_DIR}/configure.h

if [ -n "${IPHONEOS_DEPLOYMENT_TARGET}" ]; then
	echo "#define DEFAULT_IPHONEOS_MIN_VERSION \"${IPHONEOS_DEPLOYMENT_TARGET}\"" >> ${DERIVED_FILE_DIR}/configure.h
else
  if [ -n "${MACOSX_DEPLOYMENT_TARGET}" ]; then
	echo "#define DEFAULT_MACOSX_MIN_VERSION \"${MACOSX_DEPLOYMENT_TARGET}\"" >> ${DERIVED_FILE_DIR}/configure.h
  fi
fi

if [ -z "${RC_SUPPORTED_ARCHS}" ]; then
	RC_SUPPORTED_ARCHS="i386 x86_64 x86_64h armv6 armv7 armv7s armv7m armv7k arm64"
fi

for ANARCH in ${RC_SUPPORTED_ARCHS}
do
	KNOWN_ARCHS=",armv4t,armv5,armv6,armv7,armv7f,armv7k,armv7s,armv6m,armv7m,armv7em,armv8,arm64,arm64v8,i386,x86_64,x86_64h,"
	FOUND=`echo "$KNOWN_ARCHS" | grep ",$ANARCH,"`
	if [ $FOUND ]; then
		echo "#define SUPPORT_ARCH_$ANARCH  1" >> ${DERIVED_FILE_DIR}/configure.h
	else
		echo "#error uknown architecture: $ANARCH" >> ${DERIVED_FILE_DIR}/configure.h
	fi
done

if [ -n "${RC_HIDE_TIDE}" ]; then
	echo "#define ALL_SUPPORTED_ARCHS  \"${RC_SUPPORTED_ARCHS}\"" >> ${DERIVED_FILE_DIR}/configure.h
	echo "#define SUPPORT_APPLE_TV 0" >> ${DERIVED_FILE_DIR}/configure.h
else
	if [ -n "${DT_VARIANT}" -a "${DT_VARIANT}" != "PONDEROSA" ]; then
		echo "#define ALL_SUPPORTED_ARCHS  \"${RC_SUPPORTED_ARCHS}\"" >> ${DERIVED_FILE_DIR}/configure.h
		echo "#define SUPPORT_APPLE_TV 0" >> ${DERIVED_FILE_DIR}/configure.h
	else
		echo "#define ALL_SUPPORTED_ARCHS  \"${RC_SUPPORTED_ARCHS} (tvOS)\"" >> ${DERIVED_FILE_DIR}/configure.h
		echo "#define SUPPORT_APPLE_TV 1" >> ${DERIVED_FILE_DIR}/configure.h
	fi
fi


# <rdar://problem/10897631> ld64 hardcodes a reference to /Developer/usr/lib/libLTO.dylib
if [ -n "${DT_TOOLCHAIN_DIR}" ]
then
	echo "-Wl,-lazy_library,${DT_TOOLCHAIN_DIR}/usr/lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
	if [ -f "${DT_TOOLCHAIN_DIR}/usr/lib/libswiftDemangle.dylib" ]; then
		echo "-Wl,-lazy_library,${DT_TOOLCHAIN_DIR}/usr/lib/libswiftDemangle.dylib" >  ${DERIVED_FILE_DIR}/linkExtras
		echo "#define DEMANGLE_SWIFT 1" >> ${DERIVED_FILE_DIR}/configure.h
	else
		echo "" > ${DERIVED_FILE_DIR}/linkExtras
	fi
else
	if [ -e "/Developer/usr/lib/libLTO.dylib" ]
	then
		echo "-Wl,-lazy_library,/Developer/usr/lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
	else
		echo "-Wl,-lazy_library,${BUILT_PRODUCTS_DIR}/../lib/libLTO.dylib" > ${DERIVED_SOURCES_DIR}/LTO_option.txt
	fi
fi

echo "#define BITCODE_XAR_VERSION \"1.0\"" >> ${DERIVED_FILE_DIR}/configure.h

if [ -n "${RC_ProjectSourceVersion}" ]; then
	echo "#define LD64_VERSION_NUM ${RC_ProjectSourceVersion}" >> ${DERIVED_FILE_DIR}/configure.h
else
	echo "#define LD64_VERSION_NUM 0" >> ${DERIVED_FILE_DIR}/configure.h
fi

# EOF
