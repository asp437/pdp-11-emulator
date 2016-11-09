#!/bin/bash
set -e # Exit as soon as any line in the bash script fails

if [ $# -ne 1 ]
then
	echo "ERROR: Usage build asm_file_without_extension"r
else
	./pdp11-asm $1.asm $1.bin
	python3 data_injector.py $1.bin
fi