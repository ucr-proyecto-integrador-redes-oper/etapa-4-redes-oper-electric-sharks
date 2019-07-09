#!/bin/bash

if [ $# -lt 4 ]; then
	echo "$0 <orange_IP_addr> <first_blue_port> <blue_host_IP> <number_of_nodes>"
	exit -1
fi

declare -i PORT=$2

for (( i=0; i<$4; i++ ))
do
	#echo "./blue $1 $PORT $3 &"
	./blue $1 $PORT $3 &
	PORT=$PORT+1
done
