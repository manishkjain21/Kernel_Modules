#!/bin/bash
# script to ins and remove PUF kernel Module and logging into file.
SEC=60
BASENAME="_PUF.log"
DUMPFILE="Mem_dump_file.log"
ENDDUMPFILE="End_dump_file.log"
UN="_"

dmesg -c >> ${DUMPFILE};
echo "PUF Module LOADED:TEST:$TEST with decay DELAY:$DELAY";
insmod kernel_panda.ko mystring="all"
for TEST in {1..36}
do
 for LEN in {1..20}
 do
  for TIME in {1..2}
  do
   DELAY=$((SEC*TIME))
   sleep $DELAY;
   
   FILENAME=$DELAY$UN$LEN$UN$TEST$BASENAME
   dmesg -c >> ${FILENAME};
   #Check if the written file is empty 	
   while true; do
		
   		if [ -s ${FILENAME} ]; then
        	break
  		else
        	#If empty goes to this condition
	    	sleep 1	
	    	dmesg -c >> ${FILENAME};	
   		fi

	done
	
    echo "PUF TIME COMPLETE :$DELAY$UN$LEN$UN$TEST";
  done
 done
done
rmmod kernel_panda.ko;
sleep 30;
dmesg -c >> ${ENDDUMPFILE}
echo "PUF Module REMOVED:TEST:$TEST"
