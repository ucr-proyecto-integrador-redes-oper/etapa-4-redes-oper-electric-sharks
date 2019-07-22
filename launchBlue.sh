#!/bin/bash

if [ $# -lt 5 ]; then
	echo "$0 <orange_IP_addr> <first_blue_port> <first_orange_port> <blue_host_IP> <number_of_nodes>"
	exit -1
fi

declare -i BLUE_PORT=$2
declare -i ORANGE_PORT=$3
declare -i TOTAL=$5
TOTAL=$TOTAL+1


for (( i=1; i<$TOTAL; i++ ))
do
	./blue $1 $BLUE_PORT $ORANGE_PORT $4 $i &
	#echo ./blue $1 $BLUE_PORT $ORANGE_PORT $4 $i &
	BLUE_PORT=$BLUE_PORT+1
	ORANGE_PORT=$ORANGE_PORT+1
done
