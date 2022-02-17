#!/bin/bash
# $1 - output file name
# $2 - catagog
# $3 - files extension to find
if [ "$#" -ne 3 ]
then
	echo "not enough params"
	echo "param1 - output file name"
	echo "param2 - catagog"
	echo "param3 - files extension to find"
else
	find $2 -type f -name "*.$3" > $1.txt | sort -o $1.txt 2>&1
fi

