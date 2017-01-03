#!/bin/bash

export PYTHONPATH=$PWD/../..:$PYTHONPATH
if [ $# -lt 1 ]; then
  python emulator.py &
fi
if [ $# -eq 1 ]; then
  python emulator.py $1 &
fi
if [ $# -eq 2 ]; then
  python emulator.py $1 $2 &
fi
if [ $# -eq 3 ]; then
  python emulator.py $1 $2 $3 &
fi
E=$!
read -n1
kill $E

