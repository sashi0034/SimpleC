#!/usr/bin/sh

cc -o tmp tmp.s
./tmp
echo $?
