#!/bin/bash
# Check for parameter
if [ $# -eq 0 ]
then
    echo "Usage: ./CodeToHTML.sh <path to base code directory>"
	exit
fi
DIRECTORY=$1 # Path to directory to recursively check for .h's and .c's

# Start GET parameters
printf 'code=' > out1.txt #beginning of GET

# Print each file to out1.txt
for f in $(find "${DIRECTORY}" -name '*.c' -or -name '*.h') 
do
	# Add file header
	printf '/****************************/\r\n/* %s */\r\n/***************************/\r\n' $f >> out1.txt
	# Add file contents
	cat $f >> out1.txt
	# Add file seperator
	printf '\r\n\r\n\r\n\r\n\r\n' >> out1.txt
done

# Replace all &'s with %26 for GET call
cat out1.txt | sed -r 's/&/\%26/g' > out2.txt
# Finish off parameters
printf '&lexer=c&options=&style=friendly&linenos=1&divstyles=' >> out2.txt

# Send GET request to hilite.me for a formatted HTML file
curl --data-binary @out2.txt http://hilite.me/api > submission.html

# Cleanup
rm out1.txt
rm out2.txt