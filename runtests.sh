#!/bin/bash
clear
echo "-------------------------------START-----------------------------------"
if [ ! -e bin/tests ] ; then  echo "Test directory doesn't exist!" && exit ; fi
for f in bin/tests/* ; do $f ; done
echo "-------------------------------END-------------------------------------"
