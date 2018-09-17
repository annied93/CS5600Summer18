#!/bin/bash

# get the file name to process from command line, otherwise prompt for user input
if [ -n "$1" ]; then
	FILE="$1"
else
	echo -n "Enter file name to process: "
	read FILE
fi
echo -n "Enter separator of source file: "
read SEPARATOR

# get the desired output file name from command line, otherwise prompt for user input
if [ -n "$2" ]; then
	OUTPUT="$2"
else
	echo -n "Enter desired output file name: "
	read OUTPUT
fi

# create the output file
touch "$OUTPUT"
touch temp.csv

# copy the header line from the source file into the output file
head -n 1 "$FILE" > "$OUTPUT"

# prompt for user option to see if they want to filter the input file
echo -n "Do you want to filter the file? (y/n): "
read fFLAG
if [ "$fFLAG" == "y" ]; then
	echo -n "Enter column for filter: "
	read COLFILTER
	echo -n "Enter parameter name for filter: "
	read PARAMETER
	awk -F"$SEPARATOR" -v col="$COLFILTER" -v par="$PARAMETER" '$col==par {print $0}' "$FILE" > temp.csv
	echo -n "Do you want to sort the output file? (y/n): "
	read sFLAG
	if [ "$sFLAG" == "y" ]; then
        	echo -n "Enter column for sorting of output: "
        	read COLSORT
        	sort -k "$COLSORT" -t "$SEPARATOR" -o temp.csv temp.csv
	fi
else
	# prompt for user option to see if they want to sort the original
	echo -n "Do you want to sort the input file? (y/n): "
	read s2FLAG
	if [ "$s2FLAG" == "y" ]; then
		echo -n "Enter column for sorting of output: "
		read COLSORT2
		sort -k "$COLSORT2" -t "$SEPARATOR" -o temp.csv temp.csv
	fi
fi
cat temp.csv >> "$OUTPUT"
rm temp.csv

echo "Completed."

