#!/bin/sh
gcc -Wall main.c -o main
chmod u+x main
if [ "$#" -ne 1 ]; then
    echo "You must pass exactly 1 command line argument to the program, please try again."
    echo  "For example: ./run.sh file.txt"
fi
if [ "$#" -eq 1 ]; then
    ./main $1
fi





