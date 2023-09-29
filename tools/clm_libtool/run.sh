#!/usr/bin/env bash

ACTION=$1
LIBNAME=$2

prep() {
    local name="$1";
    poetry run python src/clm_libtool.py --root_path "../../." --action "prepare" --name $name
}

clean() {
    local name="$1";
    poetry run python src/clm_libtool.py --root_path "../../." --action "clean" --name $name
}

[ $# -eq 0 ] && { echo "Usage: $0 name of library"; exit 1; }

case $ACTION in 
    "prepare") prep $LIBNAME;;
    "clean") clean $LIBNAME;;
    "*") echo "unrecognized command"; exit 1;;
esac
