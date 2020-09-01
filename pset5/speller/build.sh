#!/usr/bin/env bash

TEXT='texts/holmes.txt'

if make speller 1>/dev/null; then
	case $1 in
		s|S) ./speller dictionaries/small $TEXT;;
		* ) ./speller $TEXT;;
	esac
fi

rm *.o 2>/dev/null
rm speller 2>/dev/null
