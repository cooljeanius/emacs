#!/bin/sh

# Script to take an undumped emacs and dump it during installation

# What was emacs compiled with
PREFIX=/usr
VERSION=21.2
MINORVERSION=1

DUMPDIR=`/usr/bin/mktemp -d /private/tmp/emacs.XXXXXX`

ln -sf "$PREFIX/share/emacs/$VERSION/etc" "$DUMPDIR/etc"
cp "$DUMPDIR/etc/DOC-$VERSION.$MINORVERSION" "$DUMPDIR/etc/DOC"
mkdir -p "$DUMPDIR/lib-src"
mkdir -p "$DUMPDIR/src"

UNDUMPED="$PREFIX/bin/emacs-undumped"
if [ `lipo -info "$UNDUMPED" | awk '{print $1}'` = "Non-fat" ]
then
	# Thin file, do not lipo
	cp "$UNDUMPED" "$DUMPDIR/src/temacs"
else
	# Fat binary, needs lipo
        lipo -extract_family `arch` "$UNDUMPED" -output "$DUMPDIR/src/temacs"
fi

cd "$DUMPDIR/src"
LC_ALL=C; export LC_ALL
./temacs -batch -l loadup dump
rm temacs
pax -rw . "$PREFIX/bin"

cd /
rm -rf "$DUMPDIR"
