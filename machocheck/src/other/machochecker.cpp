/* -*- mode: C++; c-basic-offset: 4; tab-width: 4 -*-
 *
 * Copyright (c) 2006-2010 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <vector>
#include <set>

#ifndef _GLIBCXX_PERMIT_BACKWARD_HASH
# define _GLIBCXX_PERMIT_BACKWARD_HASH 1
#endif /* !_GLIBCXX_PERMIT_BACKWARD_HASH */
#include <ext/hash_set>
/* FIXME: check for this header with autoconf and/or get the correct includes
 * for it: */
#if defined(__cplusplus) && defined(__GNUC__) && defined(__GNUC_MINOR__)
# if ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3))) || \
     (defined(__clang__) && defined(__llvm__) && !defined(__clang_analyzer__))
#  if (__cplusplus >= 201103L)
#   include <unordered_set>
#  endif /* c++11 */
# endif /* gcc 4.3+ || clang, but not the static analyzer */
#endif /* __cplusplus && __GNUC__ && __GNUC_MINOR__ */

#include "configure.h"

#include "MachOFileAbstraction.hpp"
#include "Architectures.hpp"

#if !defined(__has_include)
# define __has_include(foo) 0
#endif /* !__has_include */

#if defined(HAVE_LIBGEN_H) || (defined(_XOPEN4) && defined(_XOPEN4UX)) || \
    (defined(_XOPEN_VERSION) && (_XOPEN_VERSION >= 4)) || \
    (defined(_XOPEN_SOURCE) && defined(_XOPEN_SOURCE_EXTENDED)) || \
    __has_include(<libgen.h>)
# include <libgen.h> /* for basename() */
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__) && defined(emacs) && \
     (defined(DEBUG) || defined(lint))
#  warning "machochecker.cpp expects <libgen.h> to be included for basename()."
# endif /* __GNUC__ && !__STRICT_ANSI__ && emacs && (DEBUG || lint) */
#endif /* HAVE_LIBGEN_H || xpg4.2 */

#if defined(HAVE_SYS_SYSLIMITS_H) || __has_include(<sys/syslimits.h>)
# include <sys/syslimits.h>
#endif /* HAVE_SYS_SYSLIMITS_H */

// Specification for main function in this file, and throwf(), too:
#include "machochecker.h"

/* */
__attribute__((noreturn))
void throwf(const char* format, ...)
{
	va_list	list;
	char*	p;
	va_start(list, format);
	vasprintf(&p, format, list);
	va_end(list);

	const char*	t = p;
	throw t;
}

/* */
static uint64_t read_uleb128(const uint8_t*& p, const uint8_t* end)
{
	uint64_t result = 0;
	int bit = 0;
	do {
		if (p == end)
			throwf("malformed uleb128");

		uint64_t slice = (*p & 0x7f);

		if ((bit >= 64) || ((slice << bit >> bit) != slice))
			throwf("uleb128 too big");
		else {
			result |= (slice << bit);
			bit += 7;
		}
	} while (*p++ & 0x80);
	return result;
}

#if defined(__GNUC__) && (__GNUC__ >= 6)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wshift-negative-value"
#endif /* gcc 6+ */
static int64_t read_sleb128(const uint8_t*& p, const uint8_t* end)
{
	int64_t result = 0;
	int bit = 0;
	uint8_t byte;
	do {
		if (p == end)
			throwf("malformed sleb128");
		byte = *p++;
		result |= ((byte & 0x7f) << bit);
		bit += 7;
	} while (byte & 0x80);
	// sign extend negative numbers:
	if ((byte & 0x40) != 0)
		result |= ((-1LL) << bit);
	return result;
}
/* keep condition the same as where we push: */
#if defined(__GNUC__) && (__GNUC__ >= 6)
# pragma GCC diagnostic pop
#endif /* gcc 6+ */

template <typename A>
class MachOChecker
{
public:
	static bool								validFile(const uint8_t* fileContent);
	static MachOChecker<A>*					make(const uint8_t* fileContent, uint32_t fileLength, const char* path, const char* verifierDstRoot)
													{ return new MachOChecker<A>(fileContent, fileLength, path, verifierDstRoot); }
	virtual									~MachOChecker() {}


private:
	typedef typename A::P					P;
	typedef typename A::P::E				E;
	typedef typename A::P::uint_t			pint_t;

    // utility classes for using std::unordered_map with c-strings:
    struct CStringHash {
        size_t operator()(const char* __s) const {
     		size_t __h = 0UL;
            for (int i = 0; *__s; ++__s) {
                __h = ((5UL * __h) + (size_t)*__s);
                if (i >= 0) {
                    i++;
                }
            }
            return __h;
        }
    };
	class CStringEquals
	{
	public:
		bool operator()(const char* left, const char* right) const { return (strcmp(left, right) == 0); }
	};
    struct CStringEquals_s
 	{
 		bool operator()(const char* left, const char* right) const { return (strcmp(left, right) == 0); }
 	};


	typedef __gnu_cxx::hash_set<const char*, __gnu_cxx::hash<const char*>, CStringEquals>  GnuStringSet;
#if defined(__cplusplus) && (__cplusplus >= 201103L) && !defined(__clang_analyzer__)
    typedef std::unordered_set<const char*, CStringHash, CStringEquals_s>  StdStringSet;
    typedef StdStringSet StringSet;
#else
    typedef GnuStringSet StringSet;
#endif /* c++11, but not the clang static analyzer */

												MachOChecker(const uint8_t* fileContent, uint32_t fileLength, const char* path, const char* verifierDstRoot);
	void										checkMachHeader();
	void										checkLoadCommands();
	void										checkSection(const macho_segment_command<P>* segCmd, const macho_section<P>* sect);
	uint8_t										loadCommandSizeMask();
	void										checkSymbolTable();
	void										checkInitTerms();
    void										checkThreadedRebaseBind();
	void										checkIndirectSymbolTable();
	void										checkRelocations();
	void										checkExternalRelocation(const macho_relocation_info<P>* reloc);
	void										checkLocalRelocation(const macho_relocation_info<P>* reloc);
    void										verify();
	void										verifyInstallName();
	void										verifyNoRpaths();
	void										verifyNoFlatLookups();
	void										verifyiOSMac();
	pint_t										relocBase();
	bool										addressInWritableSegment(pint_t address);
	bool										hasTextRelocInRange(pint_t start, pint_t end);
	pint_t										segStartAddress(uint8_t segIndex);
	bool										addressIsRebaseSite(pint_t addr, pint_t& pointeeAddr);
	bool										addressIsBindingSite(pint_t addr);
	pint_t										getInitialStackPointer(const macho_thread_command<P>*);
	pint_t										getEntryPoint(const macho_thread_command<P>*);
	const char*									archName();

    //...why the whitespace?
	const char*									fPath;
	const char*									fDstRoot;
	const macho_header<P>*						fHeader;
	uint32_t									fLength;
	const char*									fInstallName;
	const char*									fStrings;
	const char*									fStringsEnd;
	const macho_nlist<P>*						fSymbols;
	uint32_t									fSymbolCount;
	const macho_dysymtab_command<P>*			fDynamicSymbolTable;
	const uint32_t*								fIndirectTable;
	uint32_t									fIndirectTableCount;
	const macho_relocation_info<P>*				fLocalRelocations;
	uint32_t									fLocalRelocationsCount;
	const macho_relocation_info<P>*				fExternalRelocations;
	uint32_t									fExternalRelocationsCount;
	bool										fWriteableSegmentWithAddrOver4G;
	bool										fSlidableImage;
	bool										fHasLC_RPATH;
    bool										fIsDebugVariant;
	uint64_t									fBaseAddress;
	const macho_segment_command<P>*				fFirstSegment;
	const macho_segment_command<P>*				fFirstWritableSegment;
	const macho_segment_command<P>*				fTEXTSegment;
	const macho_dyld_info_command<P>*			fDyldInfo;
	uint32_t									fSectionCount;
	std::vector<const macho_segment_command<P>*>fSegments;
};


//...
template <>
bool MachOChecker<ppc>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC)
		return false;
	if (header->cputype() != CPU_TYPE_POWERPC)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
#ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
#endif /* DEBUG */
            break;
	}
	return false;
}

template <>
bool MachOChecker<ppc64>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC_64)
		return false;
	if (header->cputype() != CPU_TYPE_POWERPC64)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
#ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
#endif /* DEBUG */
            break;
	}
	return false;
}

template <>
bool MachOChecker<x86>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC)
		return false;
	if (header->cputype() != CPU_TYPE_I386)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
#ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
#endif /* DEBUG */
            break;
	}
	return false;
}

template <>
bool MachOChecker<x86_64>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC_64)
		return false;
	if (header->cputype() != CPU_TYPE_X86_64)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
#ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
#endif /* DEBUG */
            break;
	}
	return false;
}

#if defined(SUPPORT_ARCH_arm_any) && SUPPORT_ARCH_arm_any
template <>
bool MachOChecker<arm>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC)
		return false;
	if (header->cputype() != CPU_TYPE_ARM)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
#ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
#endif /* DEBUG */
            break;
	}
	return false;
}
#endif

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <>
bool MachOChecker<arm64>::validFile(const uint8_t* fileContent)
{
	const macho_header<P>* header = (const macho_header<P>*)fileContent;
	if (header->magic() != MH_MAGIC_64)
		return false;
	if (header->cputype() != CPU_TYPE_ARM64)
		return false;
	switch (header->filetype()) {
		case MH_EXECUTE:
		case MH_DYLIB:
		case MH_BUNDLE:
		case MH_DYLINKER:
			return true;
        default:
# ifdef DEBUG
            printf("Unhandled mach header filetype 0x%x.\n", header->filetype());
# endif /* DEBUG */
            break;
	}
	return false;
}
#endif /* SUPPORT_ARCH_arm64 */

template <> __attribute__((const))
uint8_t MachOChecker<ppc>::loadCommandSizeMask()
{
    return (uint8_t)0x03;
}
template <> __attribute__((const))
uint8_t MachOChecker<ppc64>::loadCommandSizeMask()
{
    return (uint8_t)0x07;
}
template <> __attribute__((const))
uint8_t MachOChecker<x86>::loadCommandSizeMask()
{
    return (uint8_t)0x03;
}
template <> __attribute__((const))
uint8_t MachOChecker<x86_64>::loadCommandSizeMask()
{
    return (uint8_t)0x07;
}
template <> __attribute__((const))
uint8_t MachOChecker<arm>::loadCommandSizeMask()
{
    return (uint8_t)0x03;
}
#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <> __attribute__((const))
uint8_t MachOChecker<arm64>::loadCommandSizeMask()
{
    return (uint8_t)0x07;
}
#endif /* SUPPORT_ARCH_arm64 */


template <>
ppc::P::uint_t MachOChecker<ppc>::getInitialStackPointer(const macho_thread_command<ppc::P>* threadInfo)
{
	return (ppc::P::uint_t)threadInfo->thread_register(3);
}

template <>
ppc64::P::uint_t MachOChecker<ppc64>::getInitialStackPointer(const macho_thread_command<ppc64::P>* threadInfo)
{
	return (ppc64::P::uint_t)threadInfo->thread_register(3);
}

template <>
x86::P::uint_t MachOChecker<x86>::getInitialStackPointer(const macho_thread_command<x86::P>* threadInfo)
{
	return (x86::P::uint_t)threadInfo->thread_register(7);
}

template <>
x86_64::P::uint_t MachOChecker<x86_64>::getInitialStackPointer(const macho_thread_command<x86_64::P>* threadInfo)
{
	return (x86_64::P::uint_t)threadInfo->thread_register(7);
}

template <>
arm::P::uint_t MachOChecker<arm>::getInitialStackPointer(const macho_thread_command<arm::P>* threadInfo)
{
	return (arm::P::uint_t)threadInfo->thread_register(13);
}

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <> __attribute__((noreturn))
arm64::P::uint_t MachOChecker<arm64>::getInitialStackPointer(const macho_thread_command<arm64::P>* threadInfo)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (threadInfo)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
	throw "LC_UNIXTHREAD not supported for arm64";
}
#endif /* SUPPORT_ARCH_arm64 */

//...
template <>
ppc::P::uint_t MachOChecker<ppc>::getEntryPoint(const macho_thread_command<ppc::P>* threadInfo)
{
	return (ppc::P::uint_t)threadInfo->thread_register(0);
}

template <>
ppc64::P::uint_t MachOChecker<ppc64>::getEntryPoint(const macho_thread_command<ppc64::P>* threadInfo)
{
	return (ppc64::P::uint_t)threadInfo->thread_register(0);
}

template <>
x86::P::uint_t MachOChecker<x86>::getEntryPoint(const macho_thread_command<x86::P>* threadInfo)
{
	return (x86::P::uint_t)threadInfo->thread_register(10);
}

template <>
x86_64::P::uint_t MachOChecker<x86_64>::getEntryPoint(const macho_thread_command<x86_64::P>* threadInfo)
{
	return (x86_64::P::uint_t)threadInfo->thread_register(16);
}

template <>
arm::P::uint_t MachOChecker<arm>::getEntryPoint(const macho_thread_command<arm::P>* threadInfo)
{
	return (arm::P::uint_t)threadInfo->thread_register(15);
}

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <> __attribute__((noreturn))
arm64::P::uint_t MachOChecker<arm64>::getEntryPoint(const macho_thread_command<arm64::P>* threadInfo)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (threadInfo)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
	throw "LC_UNIXTHREAD not supported for arm64";
}
#endif /* SUPPORT_ARCH_arm64 */

/* */
template <typename A>
const char* MachOChecker<A>::archName()
{
	switch (fHeader->cputype()) {
		case CPU_TYPE_I386:
			return "i386";
		case CPU_TYPE_X86_64:
			if (fHeader->cpusubtype() == CPU_SUBTYPE_X86_64_H)
				return "x86_64h";
			else
				return "x86_64";
		case CPU_TYPE_ARM:
			switch (fHeader->cpusubtype()) {
				case CPU_SUBTYPE_ARM_V7:
					return "armv7";
				case CPU_SUBTYPE_ARM_V7S:
					return "armv7s";
				case CPU_SUBTYPE_ARM_V7K:
					return "armv7k";
     			default:
        			break;
			}
			return "arm";
		case CPU_TYPE_ARM64:
			return "arm64";
   		default:
     		break;
	}
	return "unknown";
}


template <typename A>
MachOChecker<A>::MachOChecker(const uint8_t* fileContent, uint32_t fileLength, const char* path, const char* verifierDstRoot)
 : fHeader(NULL), fLength(fileLength), fInstallName(NULL), fStrings(NULL), fSymbols(NULL), fSymbolCount(0), fDynamicSymbolTable(NULL), fIndirectTableCount(0),
 fLocalRelocations(NULL), fLocalRelocationsCount(0), fExternalRelocations(NULL),  fExternalRelocationsCount(0),
 fWriteableSegmentWithAddrOver4G(false), fSlidableImage(false), fHasLC_RPATH(false), fIsDebugVariant(false), fBaseAddress(0), fFirstSegment(NULL), fFirstWritableSegment(NULL),
 fTEXTSegment(NULL), fDyldInfo(NULL), fSectionCount(0)
{
	// sanity check:
	if (! validFile(fileContent))
		throw "not a mach-o file that can be checked";

	fPath = strdup(path);
	fDstRoot = verifierDstRoot ? strdup(verifierDstRoot) : NULL;
	fHeader = (const macho_header<P>*)fileContent;

	// sanity check header:
	checkMachHeader();

	// check load commands:
	checkLoadCommands();

	checkIndirectSymbolTable();

	checkRelocations();

	checkSymbolTable();

	checkInitTerms();

	checkThreadedRebaseBind();

    if (verifierDstRoot != NULL)
		verify();
    else
    	printf("skipping '-verifier_dstroot' checks...\n");
}


template <typename A>
void MachOChecker<A>::checkMachHeader()
{
#ifdef DEBUG
    printf("Checking mach header...\n");
#endif /* DEBUG */

	if ((fHeader->sizeofcmds() + sizeof(macho_header<P>)) > fLength)
		throw "sizeofcmds in mach_header is larger than file";

	uint32_t flags = fHeader->flags();
#ifdef DEBUG
	printf("Header flags are 0x%x.\n", flags);
#endif /* DEBUG */
#if defined(LD64_VERSION_NUM) && (LD64_VERSION_NUM == 236)
# define MH_INVALID_MASK 0xFE000000
#elif defined(LD64_VERSION_NUM) && (LD64_VERSION_NUM == 274)
# define MH_INVALID_MASK 0xFC000000
#else
# define MH_INVALID_MASK 0xF8000000
#endif /* OLD_MH_INVALID_BITS && LD64_VERSION_NUM */
	const uint32_t invalidBits = (MH_INCRLINK | MH_LAZY_INIT | MH_INVALID_MASK);
	if (flags & invalidBits)
		throw "invalid bits in mach_header flags";
	if ((flags & MH_NO_REEXPORTED_DYLIBS) && (fHeader->filetype() != MH_DYLIB))
		throw "MH_NO_REEXPORTED_DYLIBS bit of mach_header flags only valid for dylibs";

	switch (fHeader->filetype()) {
		case MH_EXECUTE:
			fSlidableImage = (flags & MH_PIE);
			break;
		case MH_DYLIB:
		case MH_BUNDLE:
			fSlidableImage = true;
			break;
		default:
			throw "not a mach-o file type supported by this tool";
	}

#ifdef DEBUG
    printf("Done checking mach header.\n");
#endif /* DEBUG */
}

#if !defined(LC_ENCRYPTION_INFO_64) && !defined(_MACHO_LOADER_H_)
# define LC_ENCRYPTION_INFO_64 0x2C /* 64-bit encrypted segment information */
#endif /* !LC_ENCRYPTION_INFO_64 && !_MACHO_LOADER_H_ */

template <typename A>
void MachOChecker<A>::checkLoadCommands()
{
#ifdef DEBUG
    printf("Checking load commands...\n");
#endif /* DEBUG */

	// check that all load commands fit within the load command space file:
	const macho_encryption_info_command<P>* encryption_info = NULL;
	const macho_thread_command<P>* threadInfo = NULL;
	const macho_entry_point_command<P>* entryPoint = NULL;
	const uint8_t* const endOfFile = ((uint8_t*)fHeader + fLength);
	const uint8_t* const endOfLoadCommands = ((uint8_t*)fHeader + sizeof(macho_header<P>) + fHeader->sizeofcmds());
	const uint32_t cmd_count = fHeader->ncmds();
	const macho_load_command<P>* const cmds = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
	const macho_load_command<P>* cmd = cmds;
	const macho_dylib_command<P>* dylibID;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		uint32_t size = cmd->cmdsize();
		if ((size & this->loadCommandSizeMask()) != 0)
			throwf("load command #%d has a unaligned size", i);
		const uint8_t* endOfCmd = (((uint8_t*)cmd) + cmd->cmdsize());
		if (endOfCmd > endOfLoadCommands)
			throwf("load command #%d extends beyond the end of the load commands", i);
		if (endOfCmd > endOfFile)
			throwf("load command #%d extends beyond the end of the file", i);
		switch (cmd->cmd()) {
			case macho_segment_command<P>::CMD:
			case LC_SYMTAB:
			case LC_DYSYMTAB:
			case LC_LOAD_DYLIB:
			case LC_ID_DYLINKER:
			case LC_LOAD_DYLINKER:
			case macho_routines_command<P>::CMD:
			case LC_SUB_FRAMEWORK:
			case LC_SUB_CLIENT:
			case LC_TWOLEVEL_HINTS:
			case LC_PREBIND_CKSUM:
			case LC_LOAD_WEAK_DYLIB:
			case LC_LAZY_LOAD_DYLIB:
			case LC_UUID:
			case LC_REEXPORT_DYLIB:
			case LC_SEGMENT_SPLIT_INFO:
			case LC_CODE_SIGNATURE:
			case LC_LOAD_UPWARD_DYLIB:
			case LC_VERSION_MIN_MACOSX:
			case LC_VERSION_MIN_IPHONEOS:
   			case LC_VERSION_MIN_TVOS:
			case LC_VERSION_MIN_WATCHOS:
			case LC_FUNCTION_STARTS:
			case LC_DYLD_ENVIRONMENT:
			case LC_DATA_IN_CODE:
			case LC_DYLIB_CODE_SIGN_DRS:
			case LC_SOURCE_VERSION:
   			case LC_NOTE:
			case LC_BUILD_VERSION:
				break;
            case LC_RPATH:
				fHasLC_RPATH = true;
				break;
			case LC_ID_DYLIB:
				dylibID = (macho_dylib_command<P>*)cmd;
				if ( dylibID->name_offset() > size )
					throwf("malformed mach-o: LC_ID_DYLIB load command has offset (%u) outside its size (%u)", dylibID->name_offset(), size);
				if ( (dylibID->name_offset() + strlen(dylibID->name()) + 1) > size )
					throwf("malformed mach-o: LC_ID_DYLIB load command string extends beyond end of load command");
				fInstallName = dylibID->name();
				break;
			case LC_DYLD_INFO:
			case LC_DYLD_INFO_ONLY:
				fDyldInfo = (macho_dyld_info_command<P>*)cmd;
				break;
			case LC_ENCRYPTION_INFO:
#ifdef LC_ENCRYPTION_INFO_64
            case LC_ENCRYPTION_INFO_64:
#endif /* LC_ENCRYPTION_INFO_64 */
				encryption_info = (macho_encryption_info_command<P>*)cmd;
				break;
			case LC_SUB_UMBRELLA:
			case LC_SUB_LIBRARY:
				if (fHeader->flags() & MH_NO_REEXPORTED_DYLIBS)
					throw "MH_NO_REEXPORTED_DYLIBS bit of mach_header flags should not be set in an image with LC_SUB_LIBRARY or LC_SUB_UMBRELLA";
				break;
			case LC_MAIN:
				if (fHeader->filetype() != MH_EXECUTE)
					throw "LC_MAIN can only be used in MH_EXECUTE file types";
				entryPoint =  (macho_entry_point_command<P>*)cmd;
				break;
			case LC_UNIXTHREAD:
				if (fHeader->filetype() != MH_EXECUTE)
					throw "LC_UNIXTHREAD can only be used in MH_EXECUTE file types";
				threadInfo = (macho_thread_command<P>*)cmd;
				break;
			default:
#if defined(__PRETTY_FUNCTION__) && !defined(__STRICT_ANSI__)
				throwf("%s, %s (line %d): load command #%d is an unknown kind 0x%X",
				       __FILE__, __PRETTY_FUNCTION__, __LINE__, i, cmd->cmd());
#else
				throwf("%s, line %d: load command #%d is an unknown kind 0x%X",
				       __FILE__, __LINE__, i, cmd->cmd());
#endif /* __PRETTY_FUNCTION && !__STRICT_ANSI__ */
		}
		cmd = (const macho_load_command<P>*)endOfCmd;
	}

	// check segments:
	cmd = cmds;
	std::vector<std::pair<pint_t, pint_t> > segmentAddressRanges;
	std::vector<std::pair<pint_t, pint_t> > segmentFileOffsetRanges;
	const macho_segment_command<P>* linkEditSegment = NULL;
	const macho_segment_command<P>* stackSegment = NULL;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		if (cmd->cmd() == macho_segment_command<P>::CMD) {
			const macho_segment_command<P>* segCmd = (const macho_segment_command<P>*)cmd;
			fSegments.push_back(segCmd);
			if (segCmd->cmdsize() != (sizeof(macho_segment_command<P>) + segCmd->nsects() * sizeof(macho_section_content<P>)))
				throw "invalid segment load command size";

			// see if this overlaps another segment address range:
			uint64_t startAddr = segCmd->vmaddr();
			uint64_t endAddr = (startAddr + segCmd->vmsize());
			for (typename std::vector<std::pair<pint_t, pint_t> >::iterator it = segmentAddressRanges.begin(); it != segmentAddressRanges.end(); ++it) {
				if (it->first < startAddr) {
					if (it->second > startAddr) {
						throw "overlapping segment vm addresses";
                    }
				} else if (it->first > startAddr) {
					if (it->first < endAddr) {
						throw "overlapping segment vm addresses";
                    }
				} else {
					throw "overlapping segment vm addresses";
				}
				segmentAddressRanges.push_back(std::make_pair<pint_t, pint_t>((pint_t)startAddr, (pint_t)endAddr));
			}
			// see if this overlaps another segment file offset range:
			uint64_t startOffset = segCmd->fileoff();
			uint64_t endOffset = startOffset + segCmd->filesize();
			for (typename std::vector<std::pair<pint_t, pint_t> >::iterator it = segmentFileOffsetRanges.begin(); it != segmentFileOffsetRanges.end(); ++it) {
				if (it->first < startOffset) {
					if (it->second > startOffset)
						throw "overlapping segment file data";
				} else if (it->first > startOffset) {
					if (it->first < endOffset) {
						throw "overlapping segment file data";
                    }
				} else {
					throw "overlapping segment file data";
				}
				segmentFileOffsetRanges.push_back(std::make_pair<pint_t, pint_t>((pint_t)startOffset, (pint_t)endOffset));
				// check that it is within file bounds:
				if ((startOffset > fLength) || (endOffset > fLength))
					throw "segment file data is past end of file";
			}
			// verify it fits in file:
			if (startOffset > fLength)
				throw "segment fileoff does not fit in file";
			if (endOffset > fLength)
				throw "segment fileoff+filesize does not fit in file";

			// record special segments:
			if (strcmp(segCmd->segname(), "__LINKEDIT") == 0)
				linkEditSegment = segCmd;
			else if (strcmp(segCmd->segname(), "__UNIXSTACK") == 0)
				stackSegment = segCmd;

			// cache interesting segments:
			if (fFirstSegment == NULL)
				fFirstSegment = segCmd;
			if ((fTEXTSegment == NULL) && (strcmp(segCmd->segname(), "__TEXT") == 0))
				fTEXTSegment = segCmd;
			if ((segCmd->initprot() & VM_PROT_WRITE) != 0) {
				if (fFirstWritableSegment == NULL)
					fFirstWritableSegment = segCmd;
				if (segCmd->vmaddr() > 0x100000000ULL)
					fWriteableSegmentWithAddrOver4G = true;
			}
   			if ((segCmd->fileoff() == 0) && (segCmd->filesize() != 0))
				fBaseAddress = segCmd->vmaddr();

			// check section ranges:
#if defined(DEBUG) && DEBUG
            printf("Checking section ranges for segment number %u named %s...\n",
                   i, segCmd->segname());
#endif /* DEBUG */
 			const macho_section<P>* const sectionsStart = (macho_section<P>*)((char*)segCmd + sizeof(macho_segment_command<P>));
			const macho_section<P>* const sectionsEnd = &sectionsStart[segCmd->nsects()];
			for (const macho_section<P>* sect = sectionsStart; sect < sectionsEnd; ++sect) {
				// check all non-zero sized sections are within segment:
				if (sect->addr() < startAddr)
					throwf("section %s vm address not within segment",
                           sect->sectname());
				if ((sect->addr() + sect->size()) > endAddr)
					throwf("section %s vm address not within segment",
                           sect->sectname());
				if (((sect->flags() & SECTION_TYPE) != S_ZEROFILL)
					&& ((sect->flags() & SECTION_TYPE) != S_THREAD_LOCAL_ZEROFILL)
					&& (segCmd->filesize() != 0)
					&& (sect->size() != 0)) {
					if (sect->offset() < startOffset)
						throwf("section %s file offset not within segment",
                               sect->sectname());
					if ((sect->offset() + sect->size()) > endOffset)
						throwf("section %s file offset not within segment",
                               sect->sectname());
				}
				checkSection(segCmd, sect);
				++fSectionCount;
			}
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd) + cmd->cmdsize());
	}

	// verify there was a LINKEDIT segment:
	if (linkEditSegment == NULL)
		throw "no __LINKEDIT segment";

	// verify there was an executable __TEXT segment and load commands are in it:
	if (fTEXTSegment == NULL)
		throw "no __TEXT segment";
	if (fTEXTSegment->initprot() != (VM_PROT_READ | VM_PROT_EXECUTE))
		throw "__TEXT segment does not have r-x init permissions";
#if defined(DEBUG) && (DEBUG > 1)
	if (fTEXTSegment->maxprot() != (VM_PROT_READ | VM_PROT_EXECUTE | VM_PROT_WRITE))
		throwf("__TEXT segment does not have rwx max permissions (instead they are 0%o)",
		       fTEXTSegment->maxprot());
#endif /* DEBUG */
	if (fTEXTSegment->fileoff() != 0)
		throw "__TEXT segment does not start at mach_header";
	if (fTEXTSegment->filesize() < (sizeof(macho_header<P>) + fHeader->sizeofcmds()))
		throw "__TEXT segment smaller than load commands";

	// verify if custom stack used, that stack is in __UNIXSTACK segment:
	if (threadInfo != NULL) {
		pint_t initialSP = getInitialStackPointer(threadInfo);
		if (initialSP != 0) {
			if (stackSegment == NULL)
				throw "LC_UNIXTHREAD specifies a custom initial stack pointer, but no __UNIXSTACK segment";
			if ((initialSP < stackSegment->vmaddr()) || (initialSP > (stackSegment->vmaddr() + stackSegment->vmsize())))
				throw "LC_UNIXTHREAD specifies a custom initial stack pointer which does not point into __UNIXSTACK segment";
		}
	}

	// verify __UNIXSTACK is zero fill:
	if (stackSegment != NULL) {
		 if ((stackSegment->filesize() != 0) || (stackSegment->fileoff() != 0))
			throw "__UNIXSTACK is not a zero-fill segment";
		 if (stackSegment->vmsize() < 4096)
			throw "__UNIXSTACK segment is too small";
	}

	// verify entry point is in __TEXT segment:
	if (threadInfo != NULL) {
		pint_t initialPC = getEntryPoint(threadInfo);
		if ((initialPC < fTEXTSegment->vmaddr()) || (initialPC >= (fTEXTSegment->vmaddr() + fTEXTSegment->vmsize()))) {
			throwf("entry point 0x%0llX is outside __TEXT segment", (long long)initialPC);
        }
	} else if (entryPoint != NULL) {
		pint_t initialOffset = (pint_t)entryPoint->entryoff();
		if ((initialOffset < fTEXTSegment->fileoff()) || (initialOffset >= (fTEXTSegment->fileoff() + fTEXTSegment->filesize()))) {
			throwf("entry point 0x%0llX is outside __TEXT segment", (long long)initialOffset);
        }
	}

	// checks for executables:
	bool isStaticExecutable = false;
	if (fHeader->filetype() == MH_EXECUTE) {
		isStaticExecutable = true;
		cmd = cmds;
		for (uint32_t i = 0U; i < cmd_count; ++i) {
			switch (cmd->cmd()) {
				case LC_LOAD_DYLINKER:
					// the existence of a dyld load command makes a executable dynamic:
					isStaticExecutable = false;
					break;
                default:
                    break;
			}
			cmd = (const macho_load_command<P>*)(((uint8_t*)cmd) + cmd->cmdsize());
		}
		if (isStaticExecutable) {
			if ((fHeader->flags() != MH_NOUNDEFS) && (fHeader->flags() != (MH_NOUNDEFS | MH_PIE)))
				throw "invalid bits in mach_header flags for static executable";
		}
	}

	// verify encryption info:
	if (encryption_info != NULL) {
		switch ( fHeader->filetype() ) {
			case MH_EXECUTE: case MH_DYLIB: case MH_BUNDLE:
				break;  // okay
			default:
				throwf("LC_ENCRYPTION_INFO load command is not allowed in this file type (0x%x).\n",
				       fHeader->filetype());
		}
		if (encryption_info->cryptoff() <  (sizeof(macho_header<P>) + fHeader->sizeofcmds()))
			throw "LC_ENCRYPTION_INFO load command has cryptoff covers some load commands";
		if ((encryption_info->cryptoff() % 4096) != 0)
			throw "LC_ENCRYPTION_INFO load command has cryptoff which is not page aligned";
		if ((encryption_info->cryptsize() % 4096) != 0)
			throw "LC_ENCRYPTION_INFO load command has cryptsize which is not page sized";
		for (typename std::vector<std::pair<pint_t, pint_t> >::iterator it = segmentFileOffsetRanges.begin();
																it != segmentFileOffsetRanges.end(); ++it) {
			if ((it->first <= encryption_info->cryptoff()) && (encryption_info->cryptoff() < it->second)) {
				if ((encryption_info->cryptoff() + encryption_info->cryptsize()) > it->second)
					throw "LC_ENCRYPTION_INFO load command is not contained within one segment";
			}
		}
	}

    // verify dylib has LC_ID_DYLIB
	if ( fHeader->filetype() == MH_DYLIB ) {
		if ( fInstallName == NULL )
			throw "MH_DYLIB missing LC_ID_DYLIB";
	}
	else {
		if ( fInstallName != NULL )
			throw "LC_ID_DYLIB found but file type is not MH_DYLIB";
	}

	// check LC_SYMTAB, LC_DYSYMTAB, and LC_SEGMENT_SPLIT_INFO:
	cmd = cmds;
	bool foundDynamicSymTab = false;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		switch (cmd->cmd()) {
			case LC_SYMTAB:
				{
					const macho_symtab_command<P>* symtab = (macho_symtab_command<P>*)cmd;
					fSymbolCount = symtab->nsyms();
					fSymbols = (const macho_nlist<P>*)((char*)fHeader + symtab->symoff());
					if (symtab->symoff() < linkEditSegment->fileoff())
						throw "symbol table not in __LINKEDIT";
					if ((symtab->symoff() + (fSymbolCount * sizeof(macho_nlist<P>*))) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "symbol table end not in __LINKEDIT";
                    if ((symtab->symoff() + (fSymbolCount * sizeof(macho_nlist<P>*))) > symtab->stroff())
						throw "symbol table overlaps string pool";
					if ((symtab->symoff() % sizeof(pint_t)) != 0)
						throw "symbol table start not pointer aligned";
					fStrings = ((char*)fHeader + symtab->stroff());
					fStringsEnd = (fStrings + symtab->strsize());
					if (symtab->stroff() < linkEditSegment->fileoff())
						throw "string pool not in __LINKEDIT";
					if ((symtab->stroff() + symtab->strsize()) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "string pool extends beyond __LINKEDIT";
					if ((symtab->stroff() % 4) != 0) // work around until rdar://problem/4737991 is fixed
						throw "string pool start not pointer aligned";
					if ((symtab->strsize() % sizeof(pint_t)) != 0)
						throw "string pool size not a multiple of pointer size";
				}
				break;
			case LC_DYSYMTAB:
				{
					if (isStaticExecutable &&! fSlidableImage)
						throw "LC_DYSYMTAB should not be used in static executable";
					foundDynamicSymTab = true;
					fDynamicSymbolTable = (macho_dysymtab_command<P>*)cmd;
					fIndirectTable = (uint32_t*)((char*)fHeader + fDynamicSymbolTable->indirectsymoff());
					fIndirectTableCount = fDynamicSymbolTable->nindirectsyms();
					if (fIndirectTableCount != 0) {
						if (fDynamicSymbolTable->indirectsymoff() < linkEditSegment->fileoff())
							throw "indirect symbol table not in __LINKEDIT";
						if ((fDynamicSymbolTable->indirectsymoff() + fIndirectTableCount * 8) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
							throw "indirect symbol table not in __LINKEDIT";
						if ((fDynamicSymbolTable->indirectsymoff() % sizeof(pint_t)) != 0)
							throw "indirect symbol table not pointer aligned";
					}
					fLocalRelocationsCount = fDynamicSymbolTable->nlocrel();
					if (fLocalRelocationsCount != 0) {
						fLocalRelocations = (const macho_relocation_info<P>*)((char*)fHeader + fDynamicSymbolTable->locreloff());
						if (fDynamicSymbolTable->locreloff() < linkEditSegment->fileoff())
							throw "local relocations not in __LINKEDIT";
						if ((fDynamicSymbolTable->locreloff() + fLocalRelocationsCount * sizeof(macho_relocation_info<P>)) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
							throw "local relocations not in __LINKEDIT";
						if ((fDynamicSymbolTable->locreloff() % sizeof(pint_t)) != 0)
							throw "local relocations table not pointer aligned";
					}
					fExternalRelocationsCount = fDynamicSymbolTable->nextrel();
					if (fExternalRelocationsCount != 0) {
						fExternalRelocations = (const macho_relocation_info<P>*)((char*)fHeader + fDynamicSymbolTable->extreloff());
						if (fDynamicSymbolTable->extreloff() < linkEditSegment->fileoff())
							throw "external relocations not in __LINKEDIT";
						if ((fDynamicSymbolTable->extreloff() + fExternalRelocationsCount * sizeof(macho_relocation_info<P>)) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
							throw "external relocations not in __LINKEDIT";
						if ((fDynamicSymbolTable->extreloff() % sizeof(pint_t)) != 0)
							throw "external relocations table not pointer aligned";
					}
				}
				break;
			case LC_SEGMENT_SPLIT_INFO:
				{
					if (isStaticExecutable)
						throw "LC_SEGMENT_SPLIT_INFO should not be used in static executable";
					const macho_linkedit_data_command<P>* info = (macho_linkedit_data_command<P>*)cmd;
					if (info->dataoff() < linkEditSegment->fileoff())
						throw "split seg info not in __LINKEDIT";
					if ((info->dataoff() + info->datasize()) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "split seg info not in __LINKEDIT";
					if ((info->dataoff() % sizeof(pint_t)) != 0)
						throw "split seg info table not pointer aligned";
					if ((info->datasize() % sizeof(pint_t)) != 0)
						throw "split seg info size not a multiple of pointer size";
				}
				break;
			case LC_FUNCTION_STARTS:
				{
					const macho_linkedit_data_command<P>* info = (macho_linkedit_data_command<P>*)cmd;
					if (info->dataoff() < linkEditSegment->fileoff())
						throw "function starts data not in __LINKEDIT";
					if ((info->dataoff() + info->datasize()) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "function starts data not in __LINKEDIT";
					if ((info->dataoff() % sizeof(pint_t)) != 0)
						throw "function starts data table not pointer aligned";
					if ((info->datasize() % sizeof(pint_t)) != 0)
						throw "function starts data size not a multiple of pointer size";
				}
				break;
			case LC_DATA_IN_CODE:
				{
					const macho_linkedit_data_command<P>* info = (macho_linkedit_data_command<P>*)cmd;
					if (info->dataoff() < linkEditSegment->fileoff())
						throw "data-in-code data not in __LINKEDIT";
					if ((info->dataoff() + info->datasize()) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "data-in-code data not in __LINKEDIT";
					if ((info->dataoff() % sizeof(pint_t)) != 0)
						throw "data-in-code data table not pointer aligned";
					if ((info->datasize() % sizeof(pint_t)) != 0)
						throw "data-in-code data size not a multiple of pointer size";
				}
				break;
			case LC_DYLIB_CODE_SIGN_DRS:
				{
					const macho_linkedit_data_command<P>* info = (macho_linkedit_data_command<P>*)cmd;
					if (info->dataoff() < linkEditSegment->fileoff())
						throw "dependent dylib DR data not in __LINKEDIT";
					if ((info->dataoff() + info->datasize()) > (linkEditSegment->fileoff() + linkEditSegment->filesize()))
						throw "dependent dylib DR  data not in __LINKEDIT";
					if ((info->dataoff() % sizeof(pint_t)) != 0)
						throw "dependent dylib DR  data table not pointer aligned";
					if ((info->datasize() % sizeof(pint_t)) != 0)
						throw "dependent dylib DR  data size not a multiple of pointer size";
				}
				break;
            default:
                break;
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd) + cmd->cmdsize());
	}
	if (!isStaticExecutable && !foundDynamicSymTab)
		throw "missing dynamic symbol table";
	if (fStrings == NULL)
		throw "missing symbol table";

#ifdef DEBUG
    printf("Done checking load commands.\n");
#endif /* DEBUG */
}

#ifdef emacs
dumpingness related_to_dump_p = UNRELATED_TO_DUMPING;
#endif /* emacs */

/* */
template <typename A>
void MachOChecker<A>::checkSection(const macho_segment_command<P>* segCmd, const macho_section<P>* sect)
{
	uint8_t sectionType = (sect->flags() & SECTION_TYPE);
	if (sectionType == S_ZEROFILL) {
		if (sect->offset() != 0)
			throwf("section offset should be 0 for zero-fill section %s",
                   sect->sectname());
	}

	// check section's segment name matches segment:
#ifdef DEBUG
	if (strncmp(sect->segname(), segCmd->segname(), 16) != 0)
		throwf("section %s in segment %s has wrong segment name",
               sect->sectname(), segCmd->segname());
#else
	(void)segCmd;
#endif /* DEBUG */

	// more section tests here...
#ifdef emacs
    /* TODO: things unexmacosx.c says about the sections it dumps that we
     * should probably check:
     * - only one __DATA segment in the input file (i.e. temacs)
     *   (it will create more of them in the output file, bootstrap-emacs, though)
     * - this __DATA segment has sections __data, __bss, __common,
     *   __la_symbol_ptr, __nl_symbol_ptr, and __dyld.
     * - data block for the segment must start at the region boundary.
     * - file offset fields should be adjusted for subsequently dumped load commands.
     */
	if ((strncmp(sect->sectname(), "__bss", 16UL) == 0)
		|| (strncmp(sect->sectname(), "__common", 16UL) == 0)
		|| (strstr(sect->sectname(), "__bss") != NULL)
		|| (strstr(sect->sectname(), "__pu_bss") != NULL)) {
		if (related_to_dump_p == INPUT_FILE_TO_BE_DUMPED) {
			/* TODO: also check that section has no data */
			if ((sect->flags() & SECTION_TYPE) != S_ZEROFILL) {
				fprintf(stderr, "section %s is non-zerofill.\n",
						sect->sectname());
			}
		} else if (related_to_dump_p == OUTPUT_FILE_OF_DUMPING) {
			if ((sect->flags() & SECTION_TYPE) != S_REGULAR) {
				fprintf(stderr, "section %s is non-regular.\n",
						sect->sectname());
			}
		}
	} else if (strncmp(sect->sectname(), "__text", 16UL) == 0) {
		; /* TODO: check for bad symbols */
	}
# if defined(EMACSDEBUG)
	else {
		printf("No special emacs checks for section %s.\n", sect->sectname());
	}
# endif /* EMACSDEBUG */
#endif /* emacs */

#ifdef DEBUG
# if (DEBUG > 1)
    /* instead of actually doing the checks mentioned previously, though,
     * just print all of the fields for now: */
    printf("Section %s has addr %llu, size %llu, offset %u, align %u, "
           "reloff %u, nreloc %u, flags %u, reserved1 %u, and reserved2 %u.\n",
           sect->sectname(), sect->addr(), sect->size(), sect->offset(),
           sect->align(), sect->reloff(), sect->nreloc(), sect->flags(),
           sect->reserved1(), sect->reserved2());
# endif /* (DEBUG > 1) */
    fprintf(stderr, "Done checking section %s in segment %s.\n",
            sect->sectname(), segCmd->segname());
#endif /* DEBUG */
}

static bool endsWith(const char* str, const char* suffix)
{
	size_t suffixLen = strlen(suffix);
	size_t strLen = strlen(str);
	if (strLen > suffixLen) {
		return (strcmp(&str[strLen - suffixLen], suffix) == 0);
	}
	return false;
}


static bool sharedCacheEligiblePath(const char* path)
{
	return ((strncmp(path, "/usr/lib/", 9UL) == 0)
			|| (strncmp(path, "/System/Library/", 16UL) == 0)
			|| (strncmp(path, "/System/iOSSupport/usr/lib/", 27UL) == 0)
			|| (strncmp(path, "/System/iOSSupport/System/Library/", 34UL) == 0)
			|| (strncmp(path, "/PureDarwin/System/Library/", 27UL) == 0));
}

template <typename A>
void MachOChecker<A>::verify()
{
#define NUM_DEBUG_SUFFIXES 12
	static const char* debugSuffixes[NUM_DEBUG_SUFFIXES] = {
		"_asan.dylib", "_asan", "_debug.dylib", "_debug", "_profile", "_profile.dylib",
        "_trace", "_trace.dylib", "_tsan", "_tsan.dylib", "_ubsan" , "_ubsan.dylib"
    };
#if defined(__cplusplus) && (__cplusplus >= 201103L)
	for (const char* suffix : debugSuffixes) {
		if (endsWith(fPath, suffix))
			fIsDebugVariant = true;
	}
#else
	for (int i = 0; i < NUM_DEBUG_SUFFIXES; i++) {
		if (endsWith(fPath, debugSuffixes[i]))
  			fIsDebugVariant = true;
    }
#endif /* c++11 or not */

	if ((fDstRoot != NULL) && (strlen(fDstRoot) < strlen(fPath))) {
		const char* installLocationInDstRoot = &fPath[strlen(fDstRoot)];
		if (installLocationInDstRoot[0] != '/')
			--installLocationInDstRoot;
		if (sharedCacheEligiblePath(installLocationInDstRoot)) {
			if (!fIsDebugVariant && (strstr(fPath, ".app/") == NULL)) {
				verifyInstallName();
				verifyNoRpaths();
			}
		}
	}
	verifyNoFlatLookups();
	verifyiOSMac();
}

#if !defined(PATH_MAX) && !defined(_SYS_SYSLIMITS_H_)
# define PATH_MAX 1024
#endif /* !PATH_MAX && !_SYS_SYSLIMITS_H_ */

template <typename A>
void MachOChecker<A>::verifyInstallName()
{
	// Do NOT allow @rpath to be used as -install_name for OS dylibs:
	if (strncmp(fInstallName, "@rpath/", 7UL) == 0) {
		printf("os_dylib_rpath_install_name\tfatal\t-install_name uses @rpath in arch %s\n", archName());
	} else {
		// Verify -install_name match actual path of dylib:
		const char* installPathWithinDstRoot = &fPath[strlen(fDstRoot)];
		if (strcmp(installPathWithinDstRoot, fInstallName) != 0) {
			// see if install name is a symlink to actual file:
			bool symlinkToDylib = false;
			char absDstPath[PATH_MAX];
			if (realpath(fDstRoot, absDstPath) != NULL) {
				char fullInstallNamePath[PATH_MAX];
				strlcpy(fullInstallNamePath, absDstPath, PATH_MAX);
				strlcat(fullInstallNamePath, fInstallName, PATH_MAX);
				char absInstallNamePath[PATH_MAX];
				if (realpath(fullInstallNamePath, absInstallNamePath) != NULL) {
					char absFPath[PATH_MAX];
					if (realpath(fPath, absFPath) != NULL) {
						if (strcmp(absInstallNamePath, absFPath) == 0)
							symlinkToDylib = true;
					}
				}
			}
			if (!symlinkToDylib)
				printf("os_dylib_bad_install_name\twarn\t-install_name does not match install location in arch %s\n", archName());
		}
	}

}

template <typename A>
void MachOChecker<A>::verifyNoRpaths()
{
	// Do NOT allow OS dylibs to add rpaths:
	if (fHasLC_RPATH) {
		printf("os_dylib_rpath\twarn\tcontains LC_RPATH load command in arch %s\n", archName());
	}
}


template <typename A>
void MachOChecker<A>::verifyNoFlatLookups()
{
	if ((fHeader->flags() & MH_TWOLEVEL) == 0) {
		printf("os_dylib_flat_namespace\twarn\tbuilt with -flat_namespace in arch %s\n", archName());
		return;
	}

	if (fDynamicSymbolTable != NULL) {
		const macho_nlist<P>* const	undefinesStart = &fSymbols[fDynamicSymbolTable->iundefsym()];
		const macho_nlist<P>* const undefinesEnd = &undefinesStart[fDynamicSymbolTable->nundefsym()];
		for (const macho_nlist<P>* sym = undefinesStart; sym < undefinesEnd; ++sym) {
#ifdef DEBUG
			printf("0x%04X %s\n", sym->n_desc(), &fStrings[sym->n_strx()]);
#endif
			if (GET_LIBRARY_ORDINAL(sym->n_desc()) == DYNAMIC_LOOKUP_ORDINAL) {
				const char* symName = &fStrings[sym->n_strx()];
				printf("os_dylib_undefined_dynamic_lookup\twarn\tbuilt with -undefined dynamic_lookup for symbol %s in arch %s\n", symName, archName());
			}
		}
	}
}

template <typename A>
void MachOChecker<A>::verifyiOSMac()
{
	const char* fileLocationWithinDstRoot = &fPath[strlen(fDstRoot)];
	if ( strncmp(fileLocationWithinDstRoot, "/System/iOSSupport/", 19) == 0 ) {
		// everything in /System/iOSSupport/ should be iOSMac only
		bool bad = false;
		const uint32_t cmd_count = fHeader->ncmds();
		const macho_load_command<P>* cmd = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
		for (uint32_t i = 0; i < cmd_count; ++i) {
			const macho_build_version_command<P>* buildVersCmd;
			switch (cmd->cmd()) {
				case LC_VERSION_MIN_MACOSX:
				case LC_VERSION_MIN_IPHONEOS:
				case LC_VERSION_MIN_TVOS:
				case LC_VERSION_MIN_WATCHOS:
					bad = true;
					break;
				case LC_BUILD_VERSION:
					buildVersCmd = (macho_build_version_command<P>*)cmd;
					if (buildVersCmd->platform() != PLATFORM_IOSMAC)
						bad = true;
					break;
     			default:
        			printf("Unhandled LC\n");
           			break;
			}
			cmd = (const macho_load_command<P>*)(((uint8_t*)cmd)+cmd->cmdsize());
		}
		if (bad)
			printf("macos_in_ios_support\twarn\tnon-iOSMac in /System/iOSSupport/ in arch %s\n", archName());
	} else {
		; // TODO: maybe someday warn about iOSMac only stuff not in /System/iOSSupport/
	}
}

//...
template <typename A>
void MachOChecker<A>::checkIndirectSymbolTable()
{
#ifdef DEBUG
    printf("Checking indirect symbol table...\n");
#endif /* DEBUG */

	// static executables do NOT have indirect symbol table:
	if (fDynamicSymbolTable == NULL)
		return;
	const macho_load_command<P>* const cmds = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
	const uint32_t cmd_count = fHeader->ncmds();
	const macho_load_command<P>* cmd = cmds;
#ifdef DEBUG
	printf("There are %u commands to process here.\n", cmd_count);
#endif /* DEBUG */
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		if (cmd->cmd() == macho_segment_command<P>::CMD) {
			const macho_segment_command<P>* segCmd = (const macho_segment_command<P>*)cmd;
			const macho_section<P>* const sectionsStart = (macho_section<P>*)((char*)segCmd + sizeof(macho_segment_command<P>));
			const macho_section<P>* const sectionsEnd = &sectionsStart[segCmd->nsects()];
			for (const macho_section<P>* sect = sectionsStart; sect < sectionsEnd; ++sect) {
				// make sure all magic sections that use indirect symbol table fit within it:
				uint32_t start = 0U;
				uint32_t elementSize = 0U;
				switch (sect->flags() & SECTION_TYPE) {
					case S_SYMBOL_STUBS:
						elementSize = sect->reserved2();
						start = sect->reserved1();
						break;
					case S_LAZY_SYMBOL_POINTERS:
					case S_NON_LAZY_SYMBOL_POINTERS:
						elementSize = sizeof(pint_t);
						start = sect->reserved1();
						break;
                    default:
                        break;
				}
				if (elementSize != 0) {
					uint32_t count = (uint32_t)(sect->size() / elementSize);
					if ((count * elementSize) != sect->size())
						throwf("%s section size is not an even multiple of element size", sect->sectname());
					if ((start + count) > fIndirectTableCount) {
						throwf("%s section references beyond end of indirect symbol table (%d > %d)",
                               sect->sectname(), (start + count), fIndirectTableCount);
                    }
				}
			}
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd) + cmd->cmdsize());
	}

	if (fDynamicSymbolTable->ilocalsym() != 0) {
		throwf("start of local symbols (%d) not at start of symbol table", fDynamicSymbolTable->ilocalsym());
	}
	if (fDynamicSymbolTable->ilocalsym() > fSymbolCount) {
		throwf("start of local symbols out of range (%d > %d) in indirect symbol table", fDynamicSymbolTable->ilocalsym(), fSymbolCount);
  	}
	if ((fDynamicSymbolTable->ilocalsym() + fDynamicSymbolTable->nlocalsym()) > fSymbolCount) {
		throwf("local symbols out of range (%d+%d > %d) in indirect symbol table",
			fDynamicSymbolTable->ilocalsym(), fDynamicSymbolTable->nlocalsym(), fSymbolCount);
	}

	if (fDynamicSymbolTable->iextdefsym() > fSymbolCount) {
		throwf("start of extern symbols out of range (%d > %d) in indirect symbol table", fDynamicSymbolTable->iextdefsym(), fSymbolCount);
  	}
	if (fDynamicSymbolTable->iextdefsym() != (fDynamicSymbolTable->ilocalsym() + fDynamicSymbolTable->nlocalsym())) {
		throwf("start of extern symbols (%d) not contiguous to local symbols (%d+%d) in indirect symbol table",
			fDynamicSymbolTable->iextdefsym(), fDynamicSymbolTable->ilocalsym(), fDynamicSymbolTable->nlocalsym());
	}
	if ((fDynamicSymbolTable->iextdefsym() + fDynamicSymbolTable->nextdefsym()) > fSymbolCount) {
		throwf("extern symbols out of range (%d+%d > %d) in indirect symbol table",
			fDynamicSymbolTable->iextdefsym(), fDynamicSymbolTable->nextdefsym(), fSymbolCount);
	}

	if (fDynamicSymbolTable->iundefsym() > fSymbolCount) {
		throwf("start of undefined symbols out of range (%d > %d) in indirect symbol table", fDynamicSymbolTable->iundefsym(), fSymbolCount);
	}
	if (fDynamicSymbolTable->iundefsym() != (fDynamicSymbolTable->iextdefsym() + fDynamicSymbolTable->nextdefsym())) {
		throwf("start of undefined symbols (%d) not contiguous to extern symbols (%d+%d) in indirect symbol table",
			fDynamicSymbolTable->iundefsym(), fDynamicSymbolTable->iextdefsym(), fDynamicSymbolTable->nextdefsym());
	}
	if ((fDynamicSymbolTable->iundefsym() + fDynamicSymbolTable->nundefsym()) > fSymbolCount) {
		throwf("undefined symbols out of range (%d+%d > %d) in indirect symbol table",
			fDynamicSymbolTable->iundefsym(), fDynamicSymbolTable->nundefsym(), fSymbolCount);
	}

	if ((fDynamicSymbolTable->iundefsym() + fDynamicSymbolTable->nundefsym()) != fSymbolCount) {
		throwf("end undefined symbols (%d+%d) not at end of all symbols (%d) in indirect symbol table",
				fDynamicSymbolTable->iundefsym(), fDynamicSymbolTable->nundefsym(),  fSymbolCount);
	}

#ifdef DEBUG
    printf("Done checking indirect symbol table.\n");
#endif /* DEBUG */
}

//...
template <typename A>
void MachOChecker<A>::checkSymbolTable()
{
#ifdef DEBUG
    printf("Checking symbol table...\n");
#endif /* DEBUG */

	// verify no duplicate external symbol names:
	if (fDynamicSymbolTable != NULL) {
		StringSet externalNames;
		const macho_nlist<P>* const	exportedStart = &fSymbols[fDynamicSymbolTable->iextdefsym()];
		const macho_nlist<P>* const exportedEnd = &exportedStart[fDynamicSymbolTable->nextdefsym()];
		int i = (int)fDynamicSymbolTable->iextdefsym();
#ifdef DEBUG
        fprintf(stderr, "External symbols start at %d (a.k.a. 0x%x).\n", i,
				fDynamicSymbolTable->iextdefsym());
#endif /* DEBUG */
		for (const macho_nlist<P>* p = exportedStart; p < exportedEnd; ++p, ++i) {
			const char* symName = &fStrings[p->n_strx()];
			if (symName > fStringsEnd)
				throw "string index out of range";
#if defined(DEBUG) && (DEBUG > 1)
			fprintf(stderr, "sym[%d] = %s\n", i, symName);
#endif /* DEBUG > 1 */
			if (externalNames.find(symName) != externalNames.end())
				throwf("duplicate external symbol: %s", symName);
			if ((p->n_type() & N_EXT) == 0)
				throwf("non-external symbol in external symbol range: %s", symName);
			// do NOT add N_INDR to externalNames because there is likely an undefine with same name:
			if ((p->n_type() & N_INDR) == 0)
				externalNames.insert(symName);
		}
		// verify no undefines with same name as an external symbol:
		const macho_nlist<P>* const	undefinesStart = &fSymbols[fDynamicSymbolTable->iundefsym()];
		const macho_nlist<P>* const undefinesEnd = &undefinesStart[fDynamicSymbolTable->nundefsym()];
		for(const macho_nlist<P>* p = undefinesStart; p < undefinesEnd; ++p) {
			const char* symName = &fStrings[p->n_strx()];
			if (symName > fStringsEnd)
				throw "string index out of range";
			if (externalNames.find(symName) != externalNames.end())
				throwf("undefine with same name as external symbol: %s", symName);
		}
		// verify all N_SECT values are valid:
		for (const macho_nlist<P>* p = fSymbols; p < &fSymbols[fSymbolCount]; ++p) {
			uint8_t type = p->n_type();
			if (((type & N_STAB) == 0) && ((type & N_TYPE) == N_SECT)) {
				if (p->n_sect() > fSectionCount) {
					throwf("symbol '%s' has n_sect=%d which is too large",
                           &fStrings[p->n_strx()], p->n_sect());
				}
			}
		}
	}

#ifdef DEBUG
    printf("Done checking symbol table.\n");
#endif /* DEBUG */
}

//...
template <typename A>
void MachOChecker<A>::checkInitTerms()
{
#ifdef DEBUG
    printf("Checking inits and terms...\n");
#endif /* DEBUG */

	const macho_load_command<P>* const cmds = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
	const uint32_t cmd_count = fHeader->ncmds();
	const macho_load_command<P>* cmd = cmds;
	int inits_and_terms_checked = 0;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		if (cmd->cmd() == macho_segment_command<P>::CMD) {
			const macho_segment_command<P>* segCmd = (const macho_segment_command<P>*)cmd;
			const macho_section<P>* const sectionsStart = (macho_section<P>*)((char*)segCmd + sizeof(macho_segment_command<P>));
			const macho_section<P>* const sectionsEnd = &sectionsStart[segCmd->nsects()];
			for(const macho_section<P>* sect = sectionsStart; sect < sectionsEnd; ++sect) {
				// make sure all magic sections that use indirect symbol table fit within it:
				uint32_t count;
				pint_t* arrayStart;
				pint_t* arrayEnd;
				const char* kind = "initializer";
				uint32_t sect_type_flags = (sect->flags() & SECTION_TYPE);
				switch (sect_type_flags) {
					case S_MOD_TERM_FUNC_POINTERS:
						kind = "terminator";
						inits_and_terms_checked++;
						// fall through:
						ATTRIBUTE_FALLTHROUGH;
					case S_MOD_INIT_FUNC_POINTERS:
						count = (uint32_t)(sect->size() / sizeof(pint_t));
						if ((count * sizeof(pint_t)) != sect->size())
							throwf("%s section size is not an even multiple of element size", sect->sectname());
						if ((sect->addr() % sizeof(pint_t)) != 0)
							throwf("%s section size is not pointer size aligned", sect->sectname());
						// check each pointer in array points within TEXT:
						arrayStart = (pint_t*)((char*)fHeader + sect->offset());
						arrayEnd = (pint_t*)((char*)fHeader + sect->offset() + sect->size());
						for (pint_t* p = arrayStart; p < arrayEnd; ++p) {
							pint_t pointer = (pint_t)P::getP(*p);
							if ((pointer < fTEXTSegment->vmaddr()) ||  (pointer >= (fTEXTSegment->vmaddr()+fTEXTSegment->vmsize())))
								throwf("%s 0x%08llX points outside __TEXT segment", kind, (long long)pointer);
						}
						// check each pointer in array will be rebased and not bound:
						if (fSlidableImage) {
							pint_t sectionBeginAddr = (pint_t)sect->addr();
							pint_t sectionEndddr = (pint_t)(sect->addr() + sect->size());
							for (pint_t addr = sectionBeginAddr, *p = arrayStart; addr < sectionEndddr; addr += (pint_t)sizeof(pint_t), ++p) {
								if (addressIsBindingSite(addr))
									throwf("%s at 0x%0llX has binding to external symbol", kind, (long long)addr);
								pint_t pointer = (pint_t)P::getP(*p);
								if (! addressIsRebaseSite(addr, pointer))
									throwf("%s at 0x%0llX is not rebased", kind, (long long)addr);
                                // check each pointer in array points within TEXT
								if ((pointer < fTEXTSegment->vmaddr()) ||  (pointer >= (fTEXTSegment->vmaddr() + fTEXTSegment->vmsize())))
									throwf("%s 0x%08llX points outside __TEXT segment", kind, (long long)pointer);
							}
						} else {
							// check each pointer in array points within TEXT
							for (pint_t* p = arrayStart; p < arrayEnd; ++p) {
								pint_t pointer = (pint_t)P::getP(*p);
								if ((pointer < fTEXTSegment->vmaddr()) ||  (pointer >= (fTEXTSegment->vmaddr()+fTEXTSegment->vmsize())))
									throwf("%s 0x%08llX 3 points outside __TEXT segment", kind, (long long)pointer);
							}
						}
						inits_and_terms_checked++;
						break;
                    default:
#if defined(DEBUG) && defined(S_LAZY_DYLIB_SYMBOL_POINTERS) && defined(stderr)
						if (sect_type_flags > S_LAZY_DYLIB_SYMBOL_POINTERS) {
							fprintf(stderr, "Unhandled section type: 0x%x.\n",
									sect_type_flags);
						}
#else
						break;
#endif /* DEBUG && S_LAZY_DYLIB_SYMBOL_POINTERS && stderr */
				}
			}
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd) + cmd->cmdsize());
	}

#ifdef DEBUG
    printf("Done checking inits and terms; checked %d total.\n",
		   inits_and_terms_checked);
#endif /* DEBUG */
}

template <typename A>
void MachOChecker<A>::checkThreadedRebaseBind()
{
#ifdef DEBUG
	printf("Checking threaded rebase bind...\n");
#endif
	// look bind info
	if (fDyldInfo != NULL) {
		const uint8_t* p = (uint8_t*)fHeader + fDyldInfo->bind_off();
		const uint8_t* end = &p[fDyldInfo->bind_size()];

		uint8_t type = 0;
		uint64_t segOffset = 0;
		uint64_t count;
		uint64_t skip;
		const char* symbolName = NULL;
		uint64_t libraryOrdinal = 0;
		uint8_t segIndex = 0;
		int64_t addend = 0;
		pint_t segStartAddr = 0;
		uint64_t ordinalTableSize = 0;
		bool useThreadedRebaseBind = false;
		bool done = false;
		while (!done && (p < end)) {
			uint8_t immediate = *p & BIND_IMMEDIATE_MASK;
			uint8_t opcode = *p & BIND_OPCODE_MASK;
			++p;
			switch (opcode) {
				case BIND_OPCODE_DONE:
					done = true;
					break;
				case BIND_OPCODE_SET_DYLIB_ORDINAL_IMM:
					libraryOrdinal = immediate;
					break;
				case BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB:
					libraryOrdinal = read_uleb128(p, end);
					break;
				case BIND_OPCODE_SET_DYLIB_SPECIAL_IMM:
					// the special ordinals are negative numbers
					if (immediate == 0)
						libraryOrdinal = 0;
					else {
						int8_t signExtended = BIND_OPCODE_MASK | immediate;
						libraryOrdinal = signExtended;
					}
					break;
				case BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM:
					symbolName = (char*)p;
     				if (symbolName != NULL) {
         				; // ???
             		}
					while (*p != '\0')
						++p;
					++p;
					break;
				case BIND_OPCODE_SET_TYPE_IMM:
					type = immediate;
     				if (type) {
         				; // ???
             		}
					break;
				case BIND_OPCODE_SET_ADDEND_SLEB:
					addend = read_sleb128(p, end);
     				if (addend) {
         				; // ???
         			}
					break;
				case BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
					segIndex = immediate;
					segStartAddr = segStartAddress(segIndex);
					segOffset = read_uleb128(p, end);
					break;
				case BIND_OPCODE_ADD_ADDR_ULEB:
					segOffset += read_uleb128(p, end);
					break;
				case BIND_OPCODE_DO_BIND:
					segOffset += sizeof(pint_t);
					break;
				case BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB:
					segOffset += read_uleb128(p, end) + sizeof(pint_t);
					break;
				case BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED:
					segOffset += (immediate * sizeof(pint_t)) + sizeof(pint_t);
					break;
				case BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB:
					count = read_uleb128(p, end);
					skip = read_uleb128(p, end);
					for (uint32_t i = 0; i < count; ++i) {
						segOffset += skip + sizeof(pint_t);
					}
					break;
				case BIND_OPCODE_THREADED:
					// Note the immediate is a sub opcode
					switch (immediate) {
						case BIND_SUBOPCODE_THREADED_SET_BIND_ORDINAL_TABLE_SIZE_ULEB:
							ordinalTableSize = read_uleb128(p, end);
							useThreadedRebaseBind = true;
							break;
						case BIND_SUBOPCODE_THREADED_APPLY: {
							if (!useThreadedRebaseBind) {
								throwf("BIND_SUBOPCODE_THREADED_APPLY without ordinal table");
							}
							uint64_t delta = 0;
							do {
								const uint8_t* pointerLocation = ((uint8_t*)fHeader + fSegments[segIndex]->fileoff() + segOffset);
								uint64_t value =
                                  P::getP((Pointer32<LittleEndian>::uint_t)*(uint64_t*)pointerLocation);
								bool isRebase = (value & (1ULL << 62)) == 0;

								if (isRebase) {
#ifdef DEBUG
# if defined(HAVE_SEGNAME) && defined(HAVE_SECTIONNAME) && defined(HAVE_TYPENAME)
									printf("(rebase): %-7s %-16s 0x%08llX  %s\n",
									       segName,
                                           sectionName(segIndex,
                                                       (segStartAddr + segOffset)),
                                           (segStartAddr + segOffset), typeName);
# else
									printf("(rebase): 0x%08llX  \n",
                                           (segStartAddr + segOffset));
# endif /* HAVE_SEGNAME && HAVE_SECTIONNAME && HAVE_TYPENAME */
#endif /* DEBUG */
								} else {
									// the ordinal is bits [0..15]
									uint16_t ordinal = value & 0xFFFF;
									if (ordinal >= ordinalTableSize) {
										throwf("bind ordinal is out of range");
									}
								}

								// The delta is bits [51..61]
								// And bit 62 is to tell us if we are a rebase (0) or bind (1)
								value &= ~(1ULL << 62);
								delta = (value & 0x3FF8000000000000) >> 51;
								segOffset += delta * sizeof(pint_t);
							} while ( delta != 0);
							break;
						}
						default:
							throwf("unknown threaded bind subopcode %d", immediate);
					}
					break;
				default:
					throwf("bad bind opcode %d", *p);
			}
		}
  		if (libraryOrdinal) {
#ifdef DEBUG
			printf("libraryOrdinal is %d.\n", (int)libraryOrdinal);
#else
			(void)libraryOrdinal;
#endif
    	}
	}
#ifdef DEBUG
	printf("Done checking threaded rebase bind.\n");
#endif /* DEBUG */
}

//...
template <>
ppc::P::uint_t MachOChecker<ppc>::relocBase()
{
	if (fHeader->flags() & MH_SPLIT_SEGS)
		return (ppc::P::uint_t)fFirstWritableSegment->vmaddr();
	else
		return (ppc::P::uint_t)fFirstSegment->vmaddr();
}

template <>
ppc64::P::uint_t MachOChecker<ppc64>::relocBase()
{
	if (fWriteableSegmentWithAddrOver4G)
		return (ppc64::P::uint_t)fFirstWritableSegment->vmaddr();
	else
		return (ppc64::P::uint_t)fFirstSegment->vmaddr();
}

template <>
x86::P::uint_t MachOChecker<x86>::relocBase()
{
	if (fHeader->flags() & MH_SPLIT_SEGS)
		return (x86::P::uint_t)fFirstWritableSegment->vmaddr();
	else
		return (x86::P::uint_t)fFirstSegment->vmaddr();
}

template <>
x86_64::P::uint_t MachOChecker<x86_64>::relocBase()
{
	// check for split-seg:
	return (x86_64::P::uint_t)fFirstWritableSegment->vmaddr();
}

template <>
arm::P::uint_t MachOChecker<arm>::relocBase()
{
	if (fHeader->flags() & MH_SPLIT_SEGS)
		return (arm::P::uint_t)fFirstWritableSegment->vmaddr();
	else
		return (arm::P::uint_t)fFirstSegment->vmaddr();
}

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <>
arm64::P::uint_t MachOChecker<arm64>::relocBase()
{
	return fFirstWritableSegment->vmaddr();
}
#endif /* SUPPORT_ARCH_arm64 */

//...
template <typename A>
bool MachOChecker<A>::addressInWritableSegment(pint_t address)
{
	const macho_load_command<P>* const cmds = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
	const uint32_t cmd_count = fHeader->ncmds();
	const macho_load_command<P>* cmd = cmds;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		if (cmd->cmd() == macho_segment_command<P>::CMD) {
			const macho_segment_command<P>* segCmd = (const macho_segment_command<P>*)cmd;
			if ((address >= segCmd->vmaddr()) && (address < (segCmd->vmaddr() + segCmd->vmsize()))) {
				// if segment is writable, then we are fine:
				if ((segCmd->initprot() & VM_PROT_WRITE) != 0)
					return true;
				// could be a text reloc, make sure section bit is set:
				const macho_section<P>* const sectionsStart = (macho_section<P>*)((char*)segCmd + sizeof(macho_segment_command<P>));
				const macho_section<P>* const sectionsEnd = &sectionsStart[segCmd->nsects()];
				for(const macho_section<P>* sect = sectionsStart; sect < sectionsEnd; ++sect) {
					if ((sect->addr() <= address) && (address < (sect->addr()+sect->size()))) {
						// found section for this address, if has relocs we are fine:
						return ((sect->flags() & (S_ATTR_EXT_RELOC|S_ATTR_LOC_RELOC)) != 0);
					}
				}
			}
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd)+cmd->cmdsize());
	}
	return false;
}

int last_external_reloc_num = 0;

//...
template <>
void MachOChecker<ppc>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 2)
		throw "bad external relocation length";
	if (reloc->r_type() != GENERIC_RELOC_VANILLA)
		throw "unknown external relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad external relocation pc_rel";
	if (reloc->r_extern() == 0)
		throw "local relocation found with external relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "external relocation address not in writable segment";
#ifdef DEBUG
    printf("r_symbolnum for ppc (32-bit) external reloc is %d.\n",
		   reloc->r_symbolnum());
#endif /* DEBUG */
	// FIXME: check r_symbol (huh?)
}

template <>
void MachOChecker<ppc64>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 3)
		throw "bad external relocation length";
	if (reloc->r_type() != GENERIC_RELOC_VANILLA)
		throw "unknown external relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad external relocation pc_rel";
	if (reloc->r_extern() == 0)
		throw "local relocation found with external relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "external relocation address not in writable segment";
#ifdef DEBUG
    printf("r_symbolnum for ppc64 external reloc is %d.\n",
		   reloc->r_symbolnum());
#endif /* DEBUG */
	// FIXME: check r_symbol (huh?)
}

template <>
void MachOChecker<x86>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 2)
		throw "bad external relocation length";
	if (reloc->r_type() != GENERIC_RELOC_VANILLA)
		throw "unknown external relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad external relocation pc_rel";
	if (reloc->r_extern() == 0)
		throw "local relocation found with external relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "external relocation address not in writable segment";
#ifdef DEBUG
	int reloc_num = (int)reloc->r_symbolnum();
	if (reloc_num != last_external_reloc_num) {
		last_external_reloc_num = reloc_num;
		printf("r_symbolnum for x86 (32-bit) external reloc is %d.\n",
			   reloc_num);
	}
#endif /* DEBUG */
	// FIXME: check r_symbol (huh?)
}

//...
template <>
void MachOChecker<x86_64>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 3)
		throw "bad external relocation length";
	if (reloc->r_type() != X86_64_RELOC_UNSIGNED)
		throw "unknown external relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad external relocation pc_rel";
	if (reloc->r_extern() == 0)
		throw "local relocation found with external relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "exernal relocation address not in writable segment";
#ifdef DEBUG
    printf("r_symbolnum for x86_64 external reloc is %d.\n",
		   reloc->r_symbolnum());
#endif /* DEBUG */
	// FIXME: check r_symbol (huh?)
}

#if defined(SUPPORT_ARCH_arm_any) && SUPPORT_ARCH_arm_any
template <>
void MachOChecker<arm>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 2)
		throw "bad external relocation length";
	if (reloc->r_type() != ARM_RELOC_VANILLA)
		throw "unknown external relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad external relocation pc_rel";
	if (reloc->r_extern() == 0)
		throw "local relocation found with external relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "external relocation address not in writable segment";
# ifdef DEBUG
    printf("r_symbolnum for arm (32-bit) external reloc is %d.\n",
		   reloc->r_symbolnum());
# endif /* DEBUG */
	// FIXME: check r_symbol (huh?)
}
#endif /* SUPPORT_ARCH_arm_any */

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <> __attribute__((noreturn))
void MachOChecker<arm64>::checkExternalRelocation(const macho_relocation_info<P>* reloc)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (reloc)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
    throw "external relocations not used for arm64";
}
#endif /* SUPPORT_ARCH_arm64 */

//...
template <>
void MachOChecker<ppc>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_address() & R_SCATTERED) {
		// scattered:
		const macho_scattered_relocation_info<P>* sreloc = (const macho_scattered_relocation_info<P>*)reloc;
        if (sreloc == (const macho_scattered_relocation_info<P>*)NULL) {
            throw "local reloc requested for checking is null";
        }
	} else {
		// FIXME: ???
		if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
			throwf("local relocation address 0x%08X not in writable segment", reloc->r_address());
	}
}


template <>
void MachOChecker<ppc64>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 3)
		throw "bad local relocation length";
	if (reloc->r_type() != GENERIC_RELOC_VANILLA)
		throw "unknown local relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad local relocation pc_rel";
	if (reloc->r_extern() != 0)
		throw "external relocation found with local relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "local relocation address not in writable segment";
}

template <>
void MachOChecker<x86>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
#if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
# pragma unused (reloc)
#endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
#ifdef DEBUG
	fprintf(stderr, "Checking local relocs for x86 is unimplemented.\n");
#endif /* DEBUG */
	return; // FIXME: ???
}

template <>
void MachOChecker<x86_64>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_length() != 3)
		throw "bad local relocation length";
	if (reloc->r_type() != X86_64_RELOC_UNSIGNED)
		throw "unknown local relocation type";
	if (reloc->r_pcrel() != 0)
		throw "bad local relocation pc_rel";
	if (reloc->r_extern() != 0)
		throw "external relocation found with local relocations";
	if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
		throw "local relocation address not in writable segment";
}

#if defined(SUPPORT_ARCH_arm_any) && SUPPORT_ARCH_arm_any
template <>
void MachOChecker<arm>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
	if (reloc->r_address() & R_SCATTERED) {
		// scattered:
		const macho_scattered_relocation_info<P>* sreloc = (const macho_scattered_relocation_info<P>*)reloc;
		if (sreloc->r_length() != 2)
			throw "bad local scattered relocation length";
		if (sreloc->r_type() != ARM_RELOC_PB_LA_PTR)
			throw "bad local scattered relocation type";
	} else {
		if (reloc->r_length() != 2)
			throw "bad local relocation length";
		if (reloc->r_extern() != 0)
			throw "external relocation found with local relocations";
		if (! this->addressInWritableSegment(reloc->r_address() + this->relocBase()))
			throw "local relocation address not in writable segment";
	}
}
#endif /* SUPPORT_ARCH_arm_any */

#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
template <> __attribute__((noreturn))
void MachOChecker<arm64>::checkLocalRelocation(const macho_relocation_info<P>* reloc)
{
# if (defined(__APPLE__) && defined(__APPLE_CC__)) || defined(__MWERKS__)
#  pragma unused (reloc)
# endif /* (__APPLE__ && __APPLE_CC__) || __MWERKS__ */
    throw "local relocations not used for arm64";
}
#endif /* SUPPORT_ARCH_arm64 */

template <typename A>
void MachOChecker<A>::checkRelocations()
{
#ifdef DEBUG
    printf("Checking relocations...\n");
#endif /* DEBUG */

	// external relocations should be sorted to minimize dyld symbol lookups,
	// therefore every reloc with the same r_symbolnum value should be contiguous:
	std::set<uint32_t> previouslySeenSymbolIndexes;
	uint32_t lastSymbolIndex = 0xFFFFFFFF;
	const macho_relocation_info<P>* const externRelocsEnd = &fExternalRelocations[fExternalRelocationsCount];
	for (const macho_relocation_info<P>* reloc = fExternalRelocations; reloc < externRelocsEnd; ++reloc) {
		this->checkExternalRelocation(reloc);
		if (reloc->r_symbolnum() != lastSymbolIndex) {
			if (previouslySeenSymbolIndexes.count(reloc->r_symbolnum()) != 0)
				throw "external relocations not sorted";
			previouslySeenSymbolIndexes.insert(lastSymbolIndex);
			lastSymbolIndex = reloc->r_symbolnum();
		}
	}

	const macho_relocation_info<P>* const localRelocsEnd = &fLocalRelocations[fLocalRelocationsCount];
	for (const macho_relocation_info<P>* reloc = fLocalRelocations; reloc < localRelocsEnd; ++reloc) {
		this->checkLocalRelocation(reloc);
	}

	// verify any section with S_ATTR_LOC_RELOC bits set actually has text relocs:
	const macho_load_command<P>* const cmds = (macho_load_command<P>*)((uint8_t*)fHeader + sizeof(macho_header<P>));
	const uint32_t cmd_count = fHeader->ncmds();
	const macho_load_command<P>* cmd = cmds;
	for (uint32_t i = 0U; i < cmd_count; ++i) {
		if (cmd->cmd() == macho_segment_command<P>::CMD) {
			const macho_segment_command<P>* segCmd = (const macho_segment_command<P>*)cmd;
			// if segment is writable, we are fine:
			if ((segCmd->initprot() & VM_PROT_WRITE) != 0)
				continue;
			// look at sections that have text reloc bit set:
			const macho_section<P>* const sectionsStart = (macho_section<P>*)((char*)segCmd + sizeof(macho_segment_command<P>));
			const macho_section<P>* const sectionsEnd = &sectionsStart[segCmd->nsects()];
			for (const macho_section<P>* sect = sectionsStart; sect < sectionsEnd; ++sect) {
				if ((sect->flags() & S_ATTR_LOC_RELOC) != 0) {
					if (! hasTextRelocInRange((pint_t)sect->addr(), (pint_t)(sect->addr() + sect->size()))) {
						throwf("section %s has attribute set that it has relocs, but it has none", sect->sectname());
					}
				}
			}
		}
		cmd = (const macho_load_command<P>*)(((uint8_t*)cmd)+cmd->cmdsize());
	}

#ifdef DEBUG
    printf("Done checking relocations.\n");
#endif /* DEBUG */
}

template <typename A>
typename A::P::uint_t MachOChecker<A>::segStartAddress(uint8_t segIndex)
{
	if (segIndex > fSegments.size())
		throw "segment index out of range";
	return (typename A::P::uint_t)fSegments[segIndex]->vmaddr();
}

//not sure where this is actually supposed to come from:
static const char *sectionName(int segIndex, uint64_t segAddrWithOffset)
{
    const size_t retStringSize = (sizeof(segIndex) + sizeof(segAddrWithOffset) + 1UL);
    char *retString = (char *)"";
    /* initialize retString this way, instead of with malloc(), to avoid
     * having to free it: */
    for (size_t i = 0UL; i <= retStringSize; i++)
        retString[i] = '\0';
    /* This is a really bad way to form the string, but it is just a first
     * attempt until I figure out how it is actually supposed to work: */
    snprintf(retString, retStringSize,
             "%i%llu", segIndex, segAddrWithOffset);
    return retString;
}

template <typename A>
bool MachOChecker<A>::hasTextRelocInRange(pint_t rangeStart, pint_t rangeEnd)
{
    const char *segName = "seg"; //placeholder
    const char *typeName = "type"; //placeholder

	// look at local relocs:
	const macho_relocation_info<P>* const localRelocsEnd = &fLocalRelocations[fLocalRelocationsCount];
	for (const macho_relocation_info<P>* reloc = fLocalRelocations; reloc < localRelocsEnd; ++reloc) {
		pint_t relocAddress = (reloc->r_address() + this->relocBase());
		if ((rangeStart <= relocAddress) && (relocAddress < rangeEnd))
			return true;
	}
	// look rebase info:
	if (fDyldInfo != NULL) {
		const uint8_t *p = (uint8_t*)fHeader + fDyldInfo->rebase_off();
		const uint8_t *end = &p[fDyldInfo->rebase_size()];

		uint8_t type = 0U;
		uint64_t segOffset = 0UL;
		uint32_t count;
		uint32_t skip;
		int segIndex = 0;
		pint_t segStartAddr = 0;
		pint_t addr;
		bool done = false;
		while (!done && (p < end)) {
			uint8_t immediate = (*p & REBASE_IMMEDIATE_MASK);
			uint8_t opcode = (*p & REBASE_OPCODE_MASK);
			++p;
			switch (opcode) {
				case REBASE_OPCODE_DONE:
					done = true;
					break;
				case REBASE_OPCODE_SET_TYPE_IMM:
                    if (type != immediate) {
                        type = immediate;
                    }
					break;
				case REBASE_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
					segIndex = immediate;
					segStartAddr = segStartAddress((uint8_t)segIndex);
					segOffset = read_uleb128(p, end);
					break;
				case REBASE_OPCODE_ADD_ADDR_ULEB:
					segOffset += read_uleb128(p, end);
					break;
				case REBASE_OPCODE_ADD_ADDR_IMM_SCALED:
					segOffset += (immediate * sizeof(pint_t));
					break;
				case REBASE_OPCODE_DO_REBASE_IMM_TIMES:
					for (int i = 0; i < immediate; ++i) {
						addr = (pint_t)(segStartAddr + segOffset);
						if ((rangeStart <= addr) && (addr < rangeEnd))
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(segIndex, (segStartAddr + segOffset)),
                               (segStartAddr + segOffset), typeName);
#endif /* DEBUG */
						segOffset += sizeof(pint_t);
					}
					break;
				case REBASE_OPCODE_DO_REBASE_ULEB_TIMES:
					count = (uint32_t)read_uleb128(p, end);
					for (uint32_t i = 0U; i < count; ++i) {
						addr = (pint_t)(segStartAddr + segOffset);
						if ((rangeStart <= addr) && (addr < rangeEnd))
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(segIndex, (segStartAddr + segOffset)),
                               (segStartAddr + segOffset), typeName);
#endif /* DEBUG */
						segOffset += sizeof(pint_t);
					}
					break;
				case REBASE_OPCODE_DO_REBASE_ADD_ADDR_ULEB:
					addr = (pint_t)(segStartAddr + segOffset);
					if ((rangeStart <= addr) && (addr < rangeEnd))
						return true;
#ifdef DEBUG
					printf("%-7s %-16s 0x%08llX  %s\n", segName,
                           sectionName(segIndex, (segStartAddr + segOffset)),
                           (segStartAddr + segOffset), typeName);
#endif /* DEBUG */
					segOffset += (read_uleb128(p, end) + sizeof(pint_t));
					break;
				case REBASE_OPCODE_DO_REBASE_ULEB_TIMES_SKIPPING_ULEB:
					count = (uint32_t)read_uleb128(p, end);
					skip = (uint32_t)read_uleb128(p, end);
					for (uint32_t i = 0U; i < count; ++i) {
						addr = (pint_t)(segStartAddr + segOffset);
						if ((rangeStart <= addr) && (addr < rangeEnd))
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(segIndex, (segStartAddr + segOffset)),
                               (segStartAddr + segOffset), typeName);
#endif /* DEBUG */
						segOffset += (skip + sizeof(pint_t));
					}
					break;
				default:
					throwf("bad rebase opcode %d", *p);
			}
		}
	}
	return false;
}

template <typename A>
bool MachOChecker<A>::addressIsRebaseSite(pint_t targetAddr, pint_t& pointeeAddr)
{
    const char *segName = "seg"; //placeholder
    const char *typeName = "type"; //placeholder

	// look at local relocs:
	const macho_relocation_info<P>* const localRelocsEnd = &fLocalRelocations[fLocalRelocationsCount];
	for (const macho_relocation_info<P>* reloc = fLocalRelocations; reloc < localRelocsEnd; ++reloc) {
		pint_t relocAddress = (reloc->r_address() + this->relocBase());
		if (relocAddress == targetAddr)
			return true;
	}
	// look rebase info:
	if (fDyldInfo != NULL) {
		const uint8_t* p0 = (uint8_t*)fHeader + fDyldInfo->rebase_off();
		const uint8_t* end0 = &p0[fDyldInfo->rebase_size()];

		uint8_t type = 0U;
		uint64_t segOffset0 = 0UL;
		uint32_t count0;
		uint32_t skip0;
		int ssegIndex = 0;
		pint_t segStartAddr0 = 0;
		pint_t addr;
		bool outerDone = false;
		while (!outerDone && (p0 < end0)) {
			uint8_t immediate = (*p0 & REBASE_IMMEDIATE_MASK);
			uint8_t opcode = (*p0 & REBASE_OPCODE_MASK);
			++p0;
			switch (opcode) {
				case REBASE_OPCODE_DONE:
					outerDone = true;
					break;
				case REBASE_OPCODE_SET_TYPE_IMM:
                    if (type != immediate) {
                        type = immediate;
                    }
					break;
				case REBASE_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
					ssegIndex = immediate;
					segStartAddr0 = segStartAddress((uint8_t)ssegIndex);
					segOffset0 = read_uleb128(p0, end0);
					break;
				case REBASE_OPCODE_ADD_ADDR_ULEB:
					segOffset0 += read_uleb128(p0, end0);
					break;
				case REBASE_OPCODE_ADD_ADDR_IMM_SCALED:
					segOffset0 += (immediate * sizeof(pint_t));
					break;
				case REBASE_OPCODE_DO_REBASE_IMM_TIMES:
					for (int i = 0; i < immediate; ++i) {
						addr = (pint_t)(segStartAddr0 + segOffset0);
						if (addr == targetAddr)
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(ssegIndex, (segStartAddr0 + segOffset0)),
                               (segStartAddr0 + segOffset0), typeName);
#endif /* DEBUG */
						segOffset0 += sizeof(pint_t);
					}
					break;
				case REBASE_OPCODE_DO_REBASE_ULEB_TIMES:
					count0 = (uint32_t)read_uleb128(p0, end0);
					for (uint32_t i = 0U; i < count0; ++i) {
						addr = (pint_t)(segStartAddr0 + segOffset0);
						if (addr == targetAddr)
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(ssegIndex, (segStartAddr0 + segOffset0)),
                               (segStartAddr0 + segOffset0), typeName);
#endif /* DEBUG */
						segOffset0 += sizeof(pint_t);
					}
					break;
				case REBASE_OPCODE_DO_REBASE_ADD_ADDR_ULEB:
					addr = (pint_t)(segStartAddr0 + segOffset0);
					if (addr == targetAddr)
						return true;
#ifdef DEBUG
					printf("%-7s %-16s 0x%08llX  %s\n", segName,
                           sectionName(ssegIndex, (segStartAddr0 + segOffset0)),
                           (segStartAddr0 + segOffset0), typeName);
#endif /* DEBUG */
					segOffset0 += (read_uleb128(p0, end0) + sizeof(pint_t));
					break;
				case REBASE_OPCODE_DO_REBASE_ULEB_TIMES_SKIPPING_ULEB:
					count0 = (uint32_t)read_uleb128(p0, end0);
					skip0 = (uint32_t)read_uleb128(p0, end0);
					for (uint32_t i = 0U; i < count0; ++i) {
						addr = (pint_t)(segStartAddr0 + segOffset0);
						if (addr == targetAddr)
							return true;
#ifdef DEBUG
						printf("%-7s %-16s 0x%08llX  %s\n", segName,
                               sectionName(ssegIndex, (segStartAddr0 + segOffset0)),
                               (segStartAddr0 + segOffset0), typeName);
#endif /* DEBUG */
 						segOffset0 += (skip0 + sizeof(pint_t));
					}
					break;
				default:
					throwf("bad rebase opcode %d", *p0);
			}
		}

		// If we have no rebase opcodes, then we may be using the threaded rebase/bind combined
		// format and need to parse the bind opcodes instead.
		if ((fDyldInfo->rebase_size() == 0) && (fDyldInfo->bind_size() != 0)) {
			const uint8_t* p1 = (uint8_t*)fHeader + fDyldInfo->bind_off();
			const uint8_t* end1 = &p1[fDyldInfo->bind_size()];

			uint8_t usegIndex = 0;
			uint64_t segOffset1 = 0;
			uint64_t count1;
			uint64_t skip1;
			pint_t segStartAddr1 = 0;
			bool innerDone = false;
			while (!innerDone && (p1 < end1)) {
				uint8_t immediate = *p1 & BIND_IMMEDIATE_MASK;
				uint8_t opcode = *p1 & BIND_OPCODE_MASK;
				++p1;
				switch (opcode) {
					case BIND_OPCODE_DONE:
						innerDone = true;
						break;
					case BIND_OPCODE_SET_DYLIB_ORDINAL_IMM:
						break;
					case BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB:
						read_uleb128(p1, end1);
						break;
					case BIND_OPCODE_SET_DYLIB_SPECIAL_IMM:
						break;
					case BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM:
						while (*p1 != '\0')
							++p1;
						++p1;
						break;
					case BIND_OPCODE_SET_TYPE_IMM:
						break;
					case BIND_OPCODE_SET_ADDEND_SLEB:
						read_sleb128(p1, end1);
						break;
					case BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
						usegIndex = immediate;
						segStartAddr1 = segStartAddress(usegIndex);
						segOffset1 = read_uleb128(p1, end1);
						break;
					case BIND_OPCODE_ADD_ADDR_ULEB:
						segOffset1 += read_uleb128(p1, end1);
						break;
					case BIND_OPCODE_DO_BIND:
						break;
					case BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB:
						segOffset1 += read_uleb128(p1, end1) + sizeof(pint_t);
						break;
					case BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED:
						segOffset1 += (immediate * sizeof(pint_t)) + sizeof(pint_t);
						break;
					case BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB:
						count1 = read_uleb128(p1, end1);
						skip1 = read_uleb128(p1, end1);
						for (uint32_t i = 0; i < count1; ++i) {
							segOffset1 += skip1 + sizeof(pint_t);
						}
						break;
					case BIND_OPCODE_THREADED:
						// Note the immediate is a sub opcode
						switch (immediate) {
							case BIND_SUBOPCODE_THREADED_SET_BIND_ORDINAL_TABLE_SIZE_ULEB:
								count1 = read_uleb128(p1, end1);
								break;
							case BIND_SUBOPCODE_THREADED_APPLY: {
								uint64_t delta = 0;
								do {
									uint8_t* pointerLocation = (uint8_t*)fHeader + fSegments[usegIndex]->fileoff() + segOffset1;
									uint64_t value =
                                      P::getP((Pointer32<LittleEndian>::uint_t)*(uint64_t*)pointerLocation);
#if defined(SUPPORT_ARCH_arm64e) && SUPPORT_ARCH_arm64e
									bool isAuthenticated = (value & (1ULL << 63)) != 0;
#endif /* SUPPORT_ARCH_arm64e */
									bool isRebase = (value & (1ULL << 62)) == 0;
									if (isRebase && ((segStartAddr1 + segOffset1) == targetAddr)) {

#if defined(SUPPORT_ARCH_arm64e) && SUPPORT_ARCH_arm64e
										if (isAuthenticated) {
											uint64_t targetValue = value & 0xFFFFFFFFULL;
											targetValue += fBaseAddress;
											pointeeAddr = (pint_t)targetValue;
										} else
#endif /* SUPPORT_ARCH_arm64e */
										{
											// Regular pointer which needs to fit in 51-bits of value.
											// C++ RTTI uses the top bit, so we'll allow the whole top-byte
											// and the signed-extended bottom 43-bits to be fit in to 51-bits.
											uint64_t top8Bits = value & 0x0007F80000000000ULL;
											uint64_t bottom43Bits = value & 0x000007FFFFFFFFFFULL;
											uint64_t targetValue = ( top8Bits << 13 ) | (((intptr_t)(bottom43Bits << 21) >> 21) & 0x00FFFFFFFFFFFFFF);
											pointeeAddr = (pint_t)targetValue;
										}
										return true;
									}

									// The delta is bits [51..61]
									// And bit 62 is to tell us if we are a rebase (0) or bind (1)
									value &= ~(1ULL << 62);
									delta = (value & 0x3FF8000000000000) >> 51;
									segOffset1 += delta * sizeof(pint_t);
								} while (delta != 0);
								break;
							}
							default:
								throwf("unknown threaded bind subopcode %d", immediate);
						}
						break;
					default:
						throwf("bad bind opcode %d", *p1);
				}
			}
		}
	}
	return false;
}

//...
template <typename A>
bool MachOChecker<A>::addressIsBindingSite(pint_t targetAddr)
{
	// look at external relocs:
	const macho_relocation_info<P>* const externRelocsEnd = &fExternalRelocations[fExternalRelocationsCount];
	for (const macho_relocation_info<P>* reloc = fExternalRelocations; reloc < externRelocsEnd; ++reloc) {
		pint_t relocAddress = (reloc->r_address() + this->relocBase());
		if (relocAddress == targetAddr)
			return true;
	}
	// look bind info:
	if (fDyldInfo != NULL) {
		const uint8_t *p = ((uint8_t *)fHeader + fDyldInfo->bind_off());
		const uint8_t *end = &p[fDyldInfo->bind_size()];

		uint8_t type = 0U;
		uint64_t segOffset = 0UL;
		uint64_t count;
		uint32_t skip;
		uint8_t flags = 0U;
		const char *symbolName = NULL;
		int libraryOrdinal = 0;
		int segIndex = 0;
		int64_t addend = 0L;
		pint_t segStartAddr = 0;
		pint_t addr = 0;
		bool done = false;

        //dummy condition to use some unused variables:
        if (flags == addr) {
            ; //I would print them if I knew what they were supposed to be, but as I do NOT know that, do nothing instead...
        }
		while (!done && (p < end)) {
			uint8_t immediate = (*p & BIND_IMMEDIATE_MASK);
			uint8_t opcode = (*p & BIND_OPCODE_MASK);
			++p;
			switch (opcode) {
				case BIND_OPCODE_DONE:
					done = true;
					break;
				case BIND_OPCODE_SET_DYLIB_ORDINAL_IMM:
                    if (libraryOrdinal != (int)immediate) {
                        libraryOrdinal = (int)immediate;
                    }
					break;
				case BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB:
					libraryOrdinal = (int)read_uleb128(p, end);
					break;
				case BIND_OPCODE_SET_DYLIB_SPECIAL_IMM:
					// the special ordinals are negative numbers:
					if (immediate == 0)
						libraryOrdinal = 0;
					else {
						int8_t signExtended = (int8_t)(BIND_OPCODE_MASK | immediate);
						libraryOrdinal = signExtended;
					}
					break;
				case BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM:
					symbolName = (const char *)p;
					while (*p != '\0')
						++p;
					++p;
					break;
				case BIND_OPCODE_SET_TYPE_IMM:
                    if (type != immediate) {
                        type = immediate;
                    }
					break;
				case BIND_OPCODE_SET_ADDEND_SLEB:
					addend = read_sleb128(p, end);
					break;
				case BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB:
					segIndex = immediate;
					segStartAddr = segStartAddress((uint8_t)segIndex);
					segOffset = read_uleb128(p, end);
					break;
				case BIND_OPCODE_ADD_ADDR_ULEB:
					segOffset += read_uleb128(p, end);
					break;
				case BIND_OPCODE_DO_BIND:
					if ((segStartAddr + segOffset) == targetAddr)
						return true;
					segOffset += sizeof(pint_t);
					break;
				case BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB:
					if ((segStartAddr + segOffset) == targetAddr)
						return true;
					segOffset += read_uleb128(p, end) + sizeof(pint_t);
					break;
				case BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED:
					if ((segStartAddr + segOffset) == targetAddr)
						return true;
					segOffset += ((immediate * sizeof(pint_t)) + sizeof(pint_t));
					break;
				case BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB:
					count = (uint32_t)read_uleb128(p, end);
					skip = (uint32_t)read_uleb128(p, end);
					for (uint32_t i = 0U; i < count; ++i) {
						if ((segStartAddr + segOffset) == targetAddr)
							return true;
						segOffset += (skip + sizeof(pint_t));
					}
					break;
                case BIND_OPCODE_THREADED:
					// Note the immediate is a sub opcode
					switch (immediate) {
						case BIND_SUBOPCODE_THREADED_SET_BIND_ORDINAL_TABLE_SIZE_ULEB:
							count = read_uleb128(p, end);
							break;
						case BIND_SUBOPCODE_THREADED_APPLY: {
							uint64_t delta = 0;
							do {
								uint8_t* pointerLocation = (uint8_t*)fHeader + fSegments[segIndex]->fileoff() + segOffset;
								uint64_t value =
                                  P::getP((Pointer32<LittleEndian>::uint_t)*(uint64_t*)pointerLocation);
								bool isRebase = (value & (1ULL << 62)) == 0;
								if (!isRebase) {
									if ((segStartAddr + segOffset) == targetAddr)
										return true;
								}

								// The delta is bits [51..61]
								// And bit 62 is to tell us if we are a rebase (0) or bind (1)
								value &= ~(1ULL << 62);
								delta = (value & 0x3FF8000000000000) >> 51;
								segOffset += delta * sizeof(pint_t);
							} while (delta != 0);
							break;
						}
						default:
							throwf("unknown threaded bind subopcode %d", immediate);
					}
					break;
				default:
					throwf("bad bind opcode %d", *p);
			}
		}
        if ((addend > 0L) || (symbolName == (const char *)NULL)) {
#ifdef DEBUG
            printf("addend is %lld.\n", addend);
#else
            printf("\n");
#endif /* DEBUG */
        }
	}
	return false;
}

// just in case, for example, <AssertMacros.h> somehow wound up getting included:
#ifdef check
# undef check
#endif /* check */

static void check(const char* path, const char* verifierDstRoot)
{
	struct stat stat_buf;

#ifdef emacs
	char *exe_name = basename((char *)path);
	if (exe_name == NULL) {
		/* Just ignore failures (e.g. ENAMETOOLONG) for now: */
		exe_name = (char *)".";
	}
	/* FIXME: be more flexible about possible names here: */
	if (strncmp(exe_name, "temacs", 16UL) == 0) {
# ifdef DEBUG
		printf("Treating '%s' as the input file for an emacs dump.\n",
			   exe_name);
# endif /* DEBUG */
		related_to_dump_p = INPUT_FILE_TO_BE_DUMPED;
	} else if (strncmp(exe_name, "bootstrap-emacs", 32UL) == 0) {
# ifdef DEBUG
		printf("Treating '%s' as the output result of an emacs dump.\n",
			   exe_name);
# endif /* DEBUG */
		related_to_dump_p = OUTPUT_FILE_OF_DUMPING;
	}
#endif /* emacs */

	try {
		int fd = ::open(path, O_RDONLY, 0);
		if (fd == -1)
			throwf("cannot open file, errno=%d (i.e. \"%s\")\n",
                   errno, strerror(errno));
		if (::fstat(fd, &stat_buf) != 0)
			throwf("fstat(%s) failed, errno=%d\n", path, errno);
		uint32_t length = (uint32_t)stat_buf.st_size;
		uint8_t* p = (uint8_t*)::mmap(NULL, (size_t)stat_buf.st_size,
                                      PROT_READ, (MAP_FILE | MAP_PRIVATE),
                                      fd, (off_t)0UL);
		if (p == ((uint8_t*)(-1)))
			throw "cannot map file";
		::close(fd);
		const mach_header* mh = (mach_header*)p;
		if (mh->magic == OSSwapBigToHostInt32(FAT_MAGIC)) {
			const struct fat_header* fh = (struct fat_header*)p;
			const struct fat_arch* archs = (struct fat_arch*)(p + sizeof(struct fat_header));
			/* FIXME: the clang static analyzer finds a potential memory leak in this loop: */
			for (unsigned long i = 0UL; i < OSSwapBigToHostInt32(fh->nfat_arch); ++i) {
				size_t offset = OSSwapBigToHostInt32(archs[i].offset);
				size_t size = OSSwapBigToHostInt32(archs[i].size);
				unsigned int cputype = OSSwapBigToHostInt32(archs[i].cputype);

				switch (cputype) {
				case CPU_TYPE_POWERPC:
                    if (MachOChecker<ppc>::validFile(p + offset)) {
						MachOChecker<ppc>::make((p + offset),
                                                (uint32_t)size, path,
                                                verifierDstRoot);
					} else {
						throw "in universal file, ppc slice does not contain ppc mach-o";
                    }
					break;
				case CPU_TYPE_I386:
                    if (MachOChecker<x86>::validFile(p + offset)) {
						MachOChecker<x86>::make((p + offset),
                                                (uint32_t)size, path,
                                                verifierDstRoot);
					} else {
						throw "in universal file, i386 slice does not contain i386 mach-o";
                    }
					break;
				case CPU_TYPE_POWERPC64:
                    if (MachOChecker<ppc64>::validFile(p + offset)) {
						MachOChecker<ppc64>::make((p + offset),
                                                  (uint32_t)size, path,
                                                  verifierDstRoot);
					} else {
						throw "in universal file, ppc64 slice does not contain ppc64 mach-o";
                    }
					break;
				case CPU_TYPE_X86_64:
                    if (MachOChecker<x86_64>::validFile(p + offset)) {
						MachOChecker<x86_64>::make((p + offset),
                                                   (uint32_t)size, path,
                                                   verifierDstRoot);
					} else {
						throw "in universal file, x86_64 slice does not contain x86_64 mach-o";
                    }
					break;
#if defined(SUPPORT_ARCH_arm_any) && SUPPORT_ARCH_arm_any
				case CPU_TYPE_ARM:
                    if (MachOChecker<arm>::validFile(p + offset)) {
						/* FIXME: leaks: */
						MachOChecker<arm>::make((p + offset),
                                                (uint32_t)size, path,
                                                verifierDstRoot);
					} else {
						throw "in universal file, arm slice does not contain arm mach-o";
                    }
					break;
#endif /* SUPPORT_ARCH_arm_any */
#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
				case CPU_TYPE_ARM64:
					if (MachOChecker<arm64>::validFile(p + offset)) {
						MachOChecker<arm64>::make((p + offset),
                                                  (uint32_t)size, path,
                                                  verifierDstRoot);
					} else {
						throw "in universal file, arm64 slice does not contain arm mach-o";
					}
					break;
#endif
				default:
                    throwf("in universal file, unknown architecture slice 0x%x\n", cputype);
				}
			}
		} else if (MachOChecker<x86>::validFile(p)) {
			/* FIXME: leaks: */
			MachOChecker<x86>::make(p, length, path, verifierDstRoot);
		} else if (MachOChecker<ppc>::validFile(p)) {
			MachOChecker<ppc>::make(p, length, path, verifierDstRoot);
		} else if (MachOChecker<ppc64>::validFile(p)) {
			MachOChecker<ppc64>::make(p, length, path, verifierDstRoot);
		} else if (MachOChecker<x86_64>::validFile(p)) {
			MachOChecker<x86_64>::make(p, length, path, verifierDstRoot);
		}
#if defined(SUPPORT_ARCH_arm_any) && SUPPORT_ARCH_arm_any
		else if (MachOChecker<arm>::validFile(p)) {
			MachOChecker<arm>::make(p, length, path, verifierDstRoot);
		}
#endif /* SUPPORT_ARCH_arm_any */
#if defined(SUPPORT_ARCH_arm64) && SUPPORT_ARCH_arm64
		else if (MachOChecker<arm64>::validFile(p)) {
			MachOChecker<arm64>::make(p, length, path, verifierDstRoot);
		}
#endif /* SUPPORT_ARCH_arm64 */
		else {
			throwf("not a known file type (%p).\n", p);
		}
	}
	catch (const char* msg) {
		throwf("%s in %s", msg, path);
	}
	/* FIXME: the clang static analyzer detects a potential memory leak at the
	 * end of this function */
}

// might be worth moving this higher up in the program later, but as it is,
// it is not needed until here:
#ifndef _PROGNAME
# define _PROGNAME "machocheck"
#endif /* !_PROGNAME */

static void print_machocheck_usage()
{
    const int n = 4; // 'n' for "Number of allowed arguments"
    const char *allowed_args[n] = {
        "-progress", "-help", "-verifier_dstroot", "-verifier_error_list"
    };
    fprintf(stderr, "Usage: %s ", _PROGNAME);
    for (int i = 0; i < n; i++) {
        fprintf(stderr, "[%s] ", allowed_args[i]);
    }
    fprintf(stderr, "<mach-o file> ...\n");
    return;
}

int machocheck_main(int argc, const char* argv[])
{
	bool progress = false;
	const char* verifierDstRoot = NULL;
	int result = 0;
#ifdef DEBUG
    printf("This program is running from path '%s' with '%i' additional argument(s).\n",
           argv[0], (argc - 1));
#endif /* DEBUG */
    if (argc <= 1) {
        fprintf(stderr, "Error: %s requires at least 1 additional argument to be passed to it.\n",
                _PROGNAME);
        print_machocheck_usage();
        return 0; /* FIXME: this return value implies success... */
    }
	for (int i = 1; (i <= argc) && (argv[i] != NULL); i++) {
#ifdef DEBUG
        printf("Checking argument number '%i' in argv.\n", i);
#endif /* DEBUG */
		const char *arg = argv[i];
#ifdef DEBUG
        printf("Argument number '%i' in argv is '%s'.\n", i, arg);
#endif /* DEBUG */
		if (arg[0] == '-') {
			if (strncmp(arg, "-progress", strlen(arg)) == 0) {
#ifdef DEBUG
                printf("'%s' is a recognized argument.\n", arg);
#endif /* DEBUG */
				progress = true;
                if ((i + 1) == argc) {
                    fprintf(stderr, "Error: %s requires at least 1 additional argument to be passed to it (besides '%s').\n",
                            _PROGNAME, arg);
                    return 0;
                } else {
#ifdef DEBUG
                    printf("We should continue to next arg here...\n");
#endif /* DEBUG */
                    continue;
                }
            } else if (strncmp(arg, "-help", strlen(arg)) == 0) {
#ifdef DEBUG
                printf("'%s' is a recognized argument.\n", arg);
#endif /* DEBUG */
                print_machocheck_usage();
                return 0;
			} else if (strncmp(arg, "-verifier_dstroot", strlen(arg)) == 0) {
				verifierDstRoot = argv[++i];
			} else if (strncmp(arg, "-verifier_error_list", strlen(arg)) == 0) {
				printf("os_dylib_rpath_install_name\tOS dylibs (those in /usr/lib/ or /System/Library/) must be built with -install_name that is an absolute path - not an @rpath\n");
				printf("os_dylib_bad_install_name\tOS dylibs (those in /usr/lib/ or /System/Library/) must be built with -install_name matching their file system location\n");
				printf("os_dylib_rpath\tOS dylibs should not contain LC_RPATH load commands (from -rpath linker option)\n");
				printf("os_dylib_flat_namespace\tOS dylibs should not be built with -flat_namespace\n");
				printf("os_dylib_undefined_dynamic_lookup\tOS dylibs should not be built with -undefined dynamic_lookup\n");
				printf("os_dylib_malformed\tThe mach-o is malformed\n");
				return 0;
			} else {
#if defined(DEBUG) && !defined(__cplusplus)
                fprintf(stderr, "unknown option: %s; going to try to throw this as a C++-style exception next...\n",
                        arg);
#endif /* DEBUG && !__cplusplus */
                // this is so stupid; I hate C++:
                try {
                    throwf("unknown option: %s\n", arg);
                }
                catch (const char* msg) {
                    fprintf(stderr, "%s failed: %s: %s\n",
                            _PROGNAME, arg, msg);
                    result = 1;
                }
                /* (if this were plain-C, I would just return or exit or
                 *  break, but no, C++ has to use this dumb try-catch
                 *  exception handling construct instead...) */
			}
		} else {
			bool success = true;
			try {
#ifdef DEBUG
                printf("Checking '%s'...\n", arg);
#endif /* DEBUG */
				check(arg, verifierDstRoot);
			}
			catch (const char* msg) {
   				if ( verifierDstRoot ) {
					printf("os_dylib_malformed\twarn\t%s\n", msg);
				} else {
					fprintf(stderr, "%s failed: %s: %s\n",
					        _PROGNAME, arg, msg);
					result = 1;
					success = false;
				}
			}
			if (success && progress) {
				printf("ok: %s\n", arg);
            }
		}
        if (i < (argc - 1)) {
#ifdef DEBUG
            printf("There remain arguments to process ('%i' < '%i').\n",
                   i, (argc - 1));
#endif /* DEBUG */
            continue;
        }
	}

	return result;
}

#ifdef _GLIBCXX_PERMIT_BACKWARD_HASH
# undef _GLIBCXX_PERMIT_BACKWARD_HASH
#endif /* _GLIBCXX_PERMIT_BACKWARD_HASH */

/* EOF */
