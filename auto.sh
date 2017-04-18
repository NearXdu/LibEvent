#! /bin/bash
if [ $# -lt 1 ] ; then 
    echo "USAGE: $0 Description" 
    exit 1; 
fi 

PWD=`pwd`
BINDIR=$PWD/bin
BINNAME=`ls -t $BINDIR |  head -n 1`


for  i in $*;
do
    DESC=$DESC"$i "
done

#remove last space char
DESC=${DESC%?}

echo ">**$BINNAME: $DESC**<br>" >> README.md
