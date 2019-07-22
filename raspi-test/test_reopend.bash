#!/bin/bash

case "$1"
in
b) BASHFILE="./test-bed.bash";;
e) BASHFILE="./edel-test.bash";;
*) echo "Argument 'b' or 'e' shold follow for select test bash!" 
	exit 1;;
esac

echo "Now test 200 times for ${BASHFILE}"
for i in {1..200}
do
	echo "=========================================="
	echo "test $i times :"
	source  ${BASHFILE}
	sleep 1
done
