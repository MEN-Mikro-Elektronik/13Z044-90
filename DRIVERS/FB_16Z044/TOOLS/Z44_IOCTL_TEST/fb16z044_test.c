/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!  
 *        \file  fb16z044_test.c
 *
 *      \author  thomas.schnuerer@men.de
 * 
 *  	 \brief  Simple usermode Testprogram to enable and disable
 * 				 16z044 special ioctls and display a base set of colors.
 *
 * 		$CC ./fb16z044_test.c -Wall -o p18test -I$ELINOS_PROJECT/linux/include
 *
 *     Switches: -
 *
 */
/*
 *---------------------------------------------------------------------------
 * Copyright 2003-2019, MEN Mikro Elektronik GmbH
 ****************************************************************************/
/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>		/* VSCREENINFO */
#include "../../INCLUDE/NATIVE/MEN/fb_men_16z044.h"

static int gencolors(int fdes);

const char *G_use="\n"
" fb16z044_test <dev> <IOCTLnr>  calls specified ioctl directly.\n"\
" -------------------------------------------------\n"\
" ioctl numbers:\n"\
" FBIO_DISABLE_MEN_16Z044_TEST    0         show normal screen\n"\
" FBIO_ENABLE_MEN_16Z044_TEST     1         show test pattern\n"\
" FBIO_ENABLE_75HZ                2         set refresh rate to 75 Hz\n"\
" FBIO_ENABLE_60HZ                3         set refresh rate to 60 Hz\n"\
" FBIO_MEN_16Z044_SWAP_ON         4         switch byte swapping on\n"\
" FBIO_MEN_16Z044_SWAP_OFF        5         turn byte swapping off\n"\
" FBIO_MEN_16Z044_BLANK           8         blank screen (all signals idle)\n"\
" FBIO_MEN_16Z044_UNBLANK         9         unblank screen\n"\
" color test (display 7 base colors) c\n\n"\
" example: ./fbtest16z044_test /dev/fb0 c  displays a TV like color map.\n"\
"          ./fbtest16z044_test /dev/fb0 1  shows rectangle on edges\n";


/***********************************************************************/
/*
 * if errornous input was made or when help requested
 *
 */
void usage(void)
{
	
	printf(G_use);
	exit(1);

}

/***********************************************************************/
/*
 * the only main function
 *
 */
int main(int argc, char *argv[]) 
{

	int fd = 0;
	char devnam[16];
	
	if (argc !=3)
		usage();

	strncpy(devnam, argv[1], 15);	
	fprintf(stderr, " open framebuffer device %s  %s\n", argv[1], argv[2]);
	if (!(fd = open( devnam , O_RDWR))){
		fprintf(stderr, "*** cant open framebuffer device %s\n", devnam);
		exit(1);
	}	

	if (! strcmp( "1", argv[2] ))
		ioctl( fd, FBIO_ENABLE_MEN_16Z044_TEST );
	else if (! strcmp( "0", argv[2] ))
		ioctl( fd, FBIO_DISABLE_MEN_16Z044_TEST );
	else if (! strcmp( "2", argv[2] ))
		ioctl( fd, FBIO_ENABLE_75HZ );
	else if (! strcmp( "3", argv[2] ))
		ioctl( fd, FBIO_ENABLE_60HZ );
	else if (! strcmp( "4", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_SWAP_ON );
	else if (! strcmp( "5", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_SWAP_OFF );
	else if (! strcmp( "8", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_BLANK );
	else if (! strcmp( "9", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_UNBLANK );
	else if (! strcmp( "10", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_SWAP_ON );
	else if (! strcmp( "11", argv[2] ))
		ioctl( fd, FBIO_MEN_16Z044_SWAP_OFF );
	else if (! strcmp( "c", argv[2] ))
		gencolors( fd );

	else
		usage();

	close(fd);

	return 0;

};


static int gencolors(int fdes ) 
{
	unsigned int line=0, col=0;

	struct fb_var_screeninfo screeninfo;
	unsigned int index = 0;
	int width;
	int height; 
	int arr_size;
	void *memP = NULL;
	unsigned short *data = NULL;

	/* array of 7 basic colors (appears like TV screen FBAS test) */
	unsigned short color[] ={	
		0x0000,	/* 0000.0000.0000.0000  black	*/
		0xF800,	/* 1111.1000.0000.0000  r 		*/
		0x07E0,	/* 0000.0111.1110.0000  g 		*/
		0x001F,	/* 0000.0000.0001.1111  b		*/
		0xF81F,	/* 1111.1000.0001.1111  r+b 	*/
		0xFFE0,	/* 1111.1111.1110.0000  r+g 	*/
		0x07FF,	/* 0000.0111.1111.1111  b+g		*/
		0xFFFF	/* 1111.1111.1111.1111  white 	*/
	};
	
	if (ioctl(fdes, FBIOGET_VSCREENINFO, &screeninfo)<0){
		perror("ioctl");
		exit(1);
	}

	printf("-------- framebuffer info: ----------\n");
	printf(" xres = %d \n", 			screeninfo.xres	);
	printf(" yres = %d \n", 			screeninfo.yres	);
	printf(" xres_virtual = %d \n", 	screeninfo.xres_virtual );
	printf(" yres_virtual = %d \n", 	screeninfo.yres );
	printf(" xoffset = %d \n", 			screeninfo.xoffset );
	printf(" yoffset = %d \n", 			screeninfo.yoffset );
	printf(" bits_per_pixel = %d \n", 	screeninfo.bits_per_pixel);

	if (screeninfo.bits_per_pixel == 8 || screeninfo.bits_per_pixel == 16 ) {
		/* examine size */
		width 	= screeninfo.xres;
		height 	= screeninfo.yres; 
		arr_size =  sizeof(color) / sizeof(short);

		if ( (memP = mmap(0, 
						  width * height * sizeof(short),
						  PROT_READ|PROT_WRITE, 
						  MAP_SHARED, 
						  fdes, 
						  0)) < 0 ){
			perror("mmap error");
			exit(1);
		} 
		else 
		{
			data = (unsigned short*)memP;
			for( line = 0; line< height; line++ )  
			{
				for( col = 0; col< width; col++ )
				{
					/* avoid int rounding errors (index always 0 ) */
					index = ( col * (arr_size * 1000) / width ) / 1000 ;
					data[(line*width)+col] = color[index];
				}
			};

			munmap(data, width * height * sizeof(short));

		};
	};

	return 0;

};

