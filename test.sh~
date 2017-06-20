#!/bin/bash
# script to ins and remove PUF kernel Module and logging into file.
P1=".txt"
P2="empty1"
FILENAME=$P2$P1
#Errors in using while loop 
#doesnt change for change in conditions


while true; do
		
   		if [ -s ${FILENAME} ]; then
		echo "didnt wrote"
        	break
  		else
        	#If empty goes to this condition
	    	sleep 1	
	    	echo "some data for the file" >> ${FILENAME};
		echo "wrote"	
   		fi
done
