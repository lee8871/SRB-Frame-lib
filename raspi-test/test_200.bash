#!/bin/bash
echo "Now test 200 times to edel-buzh"

re='^[0-9]+$'
if ! [[ $1 =~ $re ]] ; then
   echo "Accesses in each test is not set, set to 10000"
	 ACS_IN_EACH="10000"
else
   echo "Accesses in each set to $1"
	 ACS_IN_EACH=$1
fi

TIME=$(date "+%Y%m%d-%H-%M-%S")
for i in {1..201}
do
	echo ""
	echo ""
	echo "=========================================="
	echo "test NO. $i :"
	~/srb_ws/SRB-Frame-lib/build/SRB_Frame_test/SRB_Frame_tester -BEdel-Baade -L~/srb_report/srb-log-edel${TIME}.v -SEdel -A${ACS_IN_EACH}
	sleep 0.1
done
