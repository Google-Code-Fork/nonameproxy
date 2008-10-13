#!/bin/bash

if [ ! -f gplheader ]
then
        echo "no gpl header file"
        exit 1
fi

for file in `ls *.cpp *.h` 
do
        if [ -z "`grep 'This program is free software' $file`" ]
        then
                echo "adding header to $file"
                mv $file "$file.gpltmp"
                cat gplheader > $file
                cat "$file.gpltmp" >> $file
                rm "$file.gpltmp"
        fi
done

