#!/bin/bash
{ for I in 10 20 30 40 50 60 70 80 90 \
      80 70 60 50 40 30 20 10 0; do
   echo $I 
   sleep 1
done
echo; } | dialog --guage "A guage demo" 6 70 0
