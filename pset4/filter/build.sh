#!/usr/bin/env bash

[ -z "$1" ] &&
	echo "Usage: $0 [FLAG]" && exit 1

case $1 in
	g|r|b|e) flag=$1;;
	debug|d) clang -o filter filter.c helpers.c; exit 0;;
	*) echo -e "$0: invalid option -- '$1'"; exit 2;;
esac

rm images/output.bmp 2>/dev/null
if make filter &>/dev/null; then
	./filter -$1 images/tower.bmp images/output.bmp &&
		sxiv -t images/tower.bmp images/output.bmp
fi
