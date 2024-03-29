#!/usr/bin/sh

expected="$1"
cc -o tmp tmp.s
./tmp
actual="$?"
if [ "$actual" = "$expected" ]; then
  echo "[OK] $actual"
else
  echo "[FAILED] Expected: $expected | Actual: $actual"
  exit 1
fi
