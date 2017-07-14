/*****************************************************************************
 * Test tool for 256 * 64 * 4 monochrome displays
 * e.g for MP70 or SNY-OBS
 *
 * Author: Christian.Zoz@men.de
 * This tool is based on work from J. Thumshirn found on a SNY-OBS image
 *
 * (c) Copyright 2003-2004 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 *****************************************************************************/
#include <err.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logos.h"

int dbg = 0;
#define dbg_warnx(format, ...) do {               \
	if (dbg)                                  \
		warnx(format, ##  __VA_ARGS__ );  \
	} while(0)

/* TAKE CARE HERE 
 * The functions below use a mix of
 * - these defines and
 * - screensize passed as parameter
 * I would have loved to have it all as parameters but stopped working here
 * due to the usual time constraints. */
#define ROWS 64	
#define COLUMNS_FB 256
#define COLUMNS_BASEPIC 288

/* Creates a base picture of 'waves' (in- and de-creasing values.
   The base picture is wider as the display, to allow a symmetric pattern.
   The pattern is 288 pixels wide. Therefore with COLUMNS_BASEPIC = 288
   all rows are equal --> room for improvement left. */
void create_base_picture(char *picture) {
	int pixel;
	char value = 0;
	char width = 1, __width__ = 1;
	char step = 1;
	for (pixel = 0; pixel < ROWS * COLUMNS_BASEPIC; pixel++) {
		*(picture + pixel) = value;
		if ((pixel + 1) % width == 0)
			value += step;
		if (value > 15) {  /* switch from in- to de-creasing values */
			value = 15;
			step = -1;
		}
		if (value < 0) {   /* switch from de- to in-creasing values */
			value = 0; /* and change the step-width */
			step = 1;  /* widths are: 1, 2, 3, 2, 1 */
			__width__ = (__width__ % 5) + 1;
			if (__width__ < 4)
				width = __width__;
			else
				width = 6 - __width__;
		}
	}
}

/* This takes lines out of the passed base picture, shifts them and fills it
   into a buffer which may then put into frame-buffer.
   There are 3 types of shifting:
   - fixed column-shift (col_shift)
   - fixed row-shift (row_shift)
   - a sinus shift of maximum width (sinus_width)
*/
void prepare_fb(char *picture, char *buffer,
                int sinus_width, int row_shift, int col_shift) {
	int pixel;
	char val16 = 0;
	char val32 = 0;
	int sin_shift;
	int fb_index = 0;

	sin_shift = sinus_width + sinus_width * sin(M_PI * 2 * row_shift / ROWS);
	for (pixel = 0; pixel < ROWS * COLUMNS_BASEPIC; pixel++) {
		if (pixel % COLUMNS_BASEPIC == COLUMNS_FB) {
			pixel += COLUMNS_BASEPIC - COLUMNS_FB;
			sin_shift = sinus_width + sinus_width * sin(M_PI * 2 * (row_shift + pixel / COLUMNS_BASEPIC) / ROWS);
		}
		/* 4 bits/pixel -> 2 pixels/byte */
		val16 = *(picture + (ROWS * COLUMNS_BASEPIC + pixel + sin_shift + col_shift) % (ROWS * COLUMNS_BASEPIC));
		if (pixel % 2) {
			val32 += val16;
			*(buffer + fb_index++) = val32;
		}
		else {
			val32 = val16 << 4;
		}
	}
}

/* Creates a 'movie' out of wave pictures with the help of the fuctions above
   and writes then directly to the framebuffer device. */
static int display_waves(int fd, char *image, unsigned long size, int delay, int iterations) {
	char *picture;
	int i;
	dbg_warnx("Writing moving waves to framebuffer");
	picture = malloc(ROWS * COLUMNS_BASEPIC);
	create_base_picture(picture);
	delay /= 25;
	for (i=0; i<iterations*100; i++) {
		prepare_fb(picture, image, 48, i, -i);
		lseek(fd, 0, SEEK_SET);
		if ((unsigned long) write(fd, image, size) < size) {
			warnx("Writing the buffer failed");
			free(picture);
			return -1;
		}
		usleep(delay);
	}
	free(picture);
	return 0;
}

/* Writes an image to the framebuffer device and waits 'delay' msecs. If
   images point to NULL it creates a image on the fly with all bytes
   set to 'value'. Note that 1 byte is 2 pixels. */
static int display_write(int fd, char *buffer, unsigned long size,
                         const char *image, int value, int delay, char* msg) {
	if (msg)
		dbg_warnx("Writing %s to framebuffer", msg);
	if (!image) {
		memset(buffer, value, size);
		image = buffer;
	}
	lseek(fd, 0, SEEK_SET);
	if ((unsigned long) write(fd, image, size) < size) {
		warnx("Writing the buffer failed");
		return -1;
	}
	usleep(delay);
	return 0;	
}

static int display_logos(int fd, char *buffer, unsigned long size, int delay,
                         int iterations) {
	int i;
	for (i = 0; i < iterations; i++) {
		if (display_write(fd, buffer, size, NULL, 0x0, delay, "all 0x0"))
			return 1;
		if (display_write(fd, buffer, size, sie_logo, 0, delay, "Siemens logo"))
			return 1;
		if (display_write(fd, buffer, size, rect_logo, 0, delay, "rectangle"))
			return 1;
		if (display_write(fd, buffer, size, men_logo, 0, delay, "MEN logo"))
			return 1;
		if (display_write(fd, buffer, size, NULL, 0xFF, delay, "all 0xFF"))
			return 1;
	}
	return 0;
}

static int display_var_brightness(int fd, char *buffer, unsigned long size,
                                  int delay, int iterations) {
	int i;
	char value;
	dbg_warnx("Writing varying brightness to framebuffer");
	for (i = 0; i < iterations * 32; i++) {
		value  = i % 32;
		if (value > 15)
			value = 31 - value;
		value = (value << 4) + value;
		if (display_write(fd, buffer, size, NULL, value, delay/10, NULL))
			return 1;
	}
	printf("\n");
	return 0;
}

static void usage(char *argv0)
{
	printf("Usage: %s -d <device> [options]\n", argv0);
	printf("  -b           Show varying brightness\n");
	printf("  -d <device>  Framebuffer device\n");
	printf("  -i           Number of iterations\n");
	printf("  -l           Show logos\n");
	printf("  -s           Delay between images in ms (default 500)\n");
	printf("  -v           Verbose output\n");
	printf("  -w           Show moving wave pattern\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int fb_devnode_fd;
	unsigned long screensize;
	int retval = 0;
	char *buffer;

	extern char *optarg;
	int opt;
	char *dev_node = NULL;
	int delay = 500;
	int iterations = 3;
	int show_logos = 0;
	int show_waves = 0;
	int show_brightness = 0;

	/* TODO: better check our rights to read/write to the device node */
	if (getuid())
		errx(1, "Must be root to run this program.");

	while ((opt = getopt(argc, argv, "bd:hi:ls:vw")) != -1) {
		switch (opt) {
		case 'b':
			show_brightness = 1;
			break;
		case 'd':
			dev_node = optarg;
			break;
		case 'i':
			iterations = atoi(optarg);
			break;
		case 'l':
			show_logos = 1;
			break;
		case 's':
			delay = atoi(optarg);
			break;
		case 'v':
			dbg = 1;
			break;
		case 'w':
			show_waves = 1;
			break;
		case 'h':
		default:
			usage(argv[0]);
			break;
		}
	}
	delay = delay * 1000;
	if (!dev_node)
		usage(argv[0]);

	fb_devnode_fd = open(dev_node, O_RDWR);
	if (fb_devnode_fd == -1)
		err(1, "Cannot open framebuffer device.");
	dbg_warnx("Framebuffer %s opened successfully", dev_node);

	screensize = ROWS * COLUMNS_FB / 2; /* 4bpp -> 2p/byte */
	buffer = malloc(screensize);
	if (!buffer)
		err(1, "Cannot allocate image buffer");

	if (show_brightness)
		retval = display_var_brightness(fb_devnode_fd, buffer,
		                                screensize, delay, iterations);

	if (show_logos)
		retval = display_logos(fb_devnode_fd, buffer, screensize,
		                       delay, iterations);

	if (show_waves)
		retval = display_waves(fb_devnode_fd, buffer, screensize,
		                       delay, iterations);

	if (display_write(fb_devnode_fd, buffer, screensize, NULL, 0x03,
	                  delay, "all 0x0"))
		return 1;

	free(buffer);
	close(fb_devnode_fd);

	return retval;
}

#if 0
/* As soon as we have a mmap ioctl in 13z044-90 again we can use this code */
/* (... which has of course to be tweaked a bit ...) */

//#include <linux/fb.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/ioctl.h>
//#include <sys/mman.h>

static int display_mmap(char *mem, unsigned long size, const char *image,
                        int value, int delay, char* msg);
static int display_mmap(char *mem, unsigned long size, const char *image,
                        int value, int delay, char* msg) {
	dbg_warnx("Writing %s to framebuffer", msg);
	if (image)
		memcpy(mem, image, size);
	else
		memset(mem, value, size);
	if (msync(mem, size, MS_SYNC)) {
		warn("Syncing the buffer failed");
		return -1;
	}
	usleep(delay);
	return 0;	
}

/* This is a part from main() to be used with mmapped framebuffer */
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	char *fb_memory;

	if (ioctl(fb_devnode_fd, FBIOGET_FSCREENINFO, &finfo))
		err(1, "Error reading fixed screen information");

	if (ioctl(fb_devnode_fd, FBIOGET_VSCREENINFO, &vinfo))
		err(1, "Error reading variable screen info");
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	dbg_warnx("Screen Resolution: %dx%d, %dbpp",
	          vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	dbg_warnx("Screen Sizei:      %ld bytes.", screensize);

	fb_memory = mmap(0, screensize, PROT_READ | PROT_WRITE,
	                 MAP_SHARED | MAP_FILE, fb_devnode_fd, 0);
	if (fb_memory == MAP_FAILED)
		err(1, "Failed to map framebuffer to memory");
	dbg_warnx("Framebuffer device successfully mapped to memory");

	for (i = 0; i < iterations; i++) {
		if (display_mmap(fb_memory, screensize, NULL, 0x0, delay, "all 0x0"))
			break;
		if (display_mmap(fb_memory, screensize, sie_logo, 0, delay, "Siemens logo"))
			break;
		if (display_mmap(fb_memory, screensize, rect_logo, 0, delay, "rectangle"))
			break;
		if (display_mmap(fb_memory, screensize, men_logo, 0, delay, "MEN logo"))
			break;
		if (display_mmap(fb_memory, screensize, NULL, 0xFF, delay, "all 0xFF"))
			break;
	}
	munmap(fb_memory, screensize);
#endif
