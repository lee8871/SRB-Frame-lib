#!/bin/bash

echo "Now test 200 times to edel-buzh"

TIME=$(date "+%Y%m%d-%H-%M-%S")
for i in {1..201}
do
	echo ""
	echo ""
	echo "=========================================="
	echo "test NO. $i :"
	~/srb_ws/build/SRB_Frame_test/SRB_Frame_tester -BEdel-Baade -L~/srb_report/srb-log-edel${TIME}.v -SEdel -A2000
	sleep 0.1
done
