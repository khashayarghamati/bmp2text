/* bmp2txt v0.1 Copyright (C) 2003 tomasliq <tom@eggdrop.ch>
 * 
 * A very simple bmp to text converter
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdio.h>
#include <stdlib.h>

/* uncomment the following line to enable stupid colors
 * note that this "feature" has errors! */

/* #define COLORS */

int main(int argc, char *argv[])
{
	FILE *file;
	unsigned short int filetype;
	long int offbits;
	int width, height;
	short int planes;
	short int bits;
	unsigned char *data, *linebuf;
	long size;
	int i,l;
	
	if (argc < 2)
	{
		printf("Usage: %s <bitmap>\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "rb")) == NULL)
	{
		printf("Error: Can not open file %s for reading\n", argv[1]);
		exit(1);
	}

	if (!fread(&filetype, sizeof(short int), 1, file))
	{
		printf("Error: Cannot read filetype\n");
		exit(1);
	}

	if (filetype != 19778)
	{
		printf("Error: This is not a bitmap file\n");
		exit(1);
	}
	
	fseek(file, 8, SEEK_CUR);

	if (!fread(&offbits, sizeof(long int), 1, file))
	{
		printf("Error: Cannot read offset\n");
		exit(1);
	}

	fseek(file, 4, SEEK_CUR);

	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);
	
	fread(&planes, sizeof(short int), 1, file);
	
	if (planes != 1)
	{
		printf("Error: Number of planes must be 1\n");
		exit(1);
	}

	if (!fread(&bits, sizeof(short int), 1, file))
	{
		printf("Error: Cannot read bits\n");
	}

	if (bits != 24)
	{
		printf("Error: Number of bits per pixel must be 24\n");
		exit(1);
	}

	size = width*height*3;

	linebuf = (unsigned char *)malloc(width*3);
	
	if ((data = (unsigned char *)malloc(size)) == NULL)
	{
		printf("Error: Cannot allocate memory\n");
		exit(1);
	}

	
	fseek(file, offbits, SEEK_SET);
	
	if (!fread(data, size, 1, file))
	{
		printf("Error: Cannot read bitmap data\n");
		exit(1);
	}

	for (i=size-width*3;i>=0;i-=width*3)
	{
#ifdef COLORS
		printf("\x1b[47m");
#endif
		for (l=0;l<width*3;l+=3)
		{
			int avg;
#ifdef COLORS
			int col=0;
#endif
			avg=(data[i+l]+data[i+l+1]+data[i+l+2])/3;

#ifdef COLORS
			/* make some stupid colors */
			if (data[i+l] > 200) { col = 1; }
			if (data[i+l+1] > 200) { if (col > 0) { col=0; } else { col = 2; } }
			if (data[i+l+2] > 200) { if (col > 0) { col=0; } else { col = 3; } }
			
			if (col == 0) { printf("\x1b[30m"); }
			if (col == 1) { printf("\x1b[32m"); }
			if (col == 2) { printf("\x1b[34m"); }
			if (col == 3) { printf("\x1b[33m"); }
#endif
			
			if (avg >= 205) { printf(" "); }
			if (avg >= 154 && avg <= 204) { printf("."); }
			if (avg >= 103 && avg <= 153) { printf(":"); }
			if (avg >= 52 && avg <= 102) { printf("x"); }
			if (avg <= 51) { printf("*"); }
		}
#ifdef COLORS
		printf("\x1b[0m\n");
#else
		printf("\n");
#endif
	}
	free(data);
	
	return 0;
}
