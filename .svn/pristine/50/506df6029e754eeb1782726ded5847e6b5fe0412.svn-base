#!/bin/bash
# script to ins and remove PUF kernel Module and logging into file.
SEC=60
BASENAME="_PUF.log"
DUMPFILE="Mem_dump_file.log"
ENDDUMPFILE="End_dump_file.log"
UN="_"

dmesg -c >> ${DUMPFILE};
echo "Rowhammer PUF Module LOADED";
insmod kernel_panda.ko
for TIME in {1..2}
  do
   DELAY=$((SEC*TIME))
   sleep $DELAY;
   
   FILENAME=$DELAY$UN$BASENAME
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
     echo "PUF TIME COMPLETE :$DELAY";
done
rmmod kernel_panda
sleep 30;
dmesg -c >> ${ENDDUMPFILE}
echo "PUF Module REMOVED:TEST:$TEST"
