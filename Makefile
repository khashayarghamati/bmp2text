# Makefile for bmp2txt

all:		bmp2txt

bmp2txt:	bmp2txt.c
		gcc -Wall -ansi -pedantic -o bmp2txt bmp2txt.c

install:
		cp -i bmp2txt /usr/local/bin/bmp2txt

uninstall:	
		rm -f /usr/local/bin/bmp2txt

clean:		
		rm -f bmp2txt
