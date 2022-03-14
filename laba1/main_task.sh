#!/bin/bash
min=$1
max=$2
direct=$3
find ${direct} -type f -size +"${min}"c -size -"${max}"c -printf '|%sB\t|\t%p\n' |sort -n -r
