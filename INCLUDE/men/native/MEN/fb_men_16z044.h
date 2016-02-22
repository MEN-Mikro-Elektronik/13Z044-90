/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!  
 *        \file  fb_men_16z044.h
 *
 *      \author  thomas.schnuerer@men.de
 *        $Date: 2007/01/08 17:43:21 $
 *    $Revision: 2.1 $
 * 
 *  	 \brief  Framebuffer driver for MEN 16z044 fb unit, first Version.
 * 				 Supported modes:
 * 				 640x400 800x600 1024x768 1280x1024 at 16bpp. 
 *
 *     Switches: -
 *
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: fb_men_16z044.h,v $
 * Revision 2.1  2007/01/08 17:43:21  ts
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2004 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef _MEN_16Z044_FB_
#define _MEN_16Z044_FB_

#include <linux/version.h>	

/* -- MEN 16Z044 Framebuffer,  additional ioctls -- */
#define MEN_16Z044_IOC_MAGIC		'F'
#define MEN_16Z044_IOCBASE      	40
#define FBIO_ENABLE_MEN_16Z044_TEST\
	    _IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	0 )
#define FBIO_DISABLE_MEN_16Z044_TEST\
        _IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	1 )
#define FBIO_ENABLE_75HZ\
        _IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	2 )
#define FBIO_ENABLE_60HZ\
        _IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	3 )
/* offs.  4-7 intentionally left free for Resolution changing */
#define FBIO_MEN_16Z044_BLANK\
		_IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	8 )
#define FBIO_MEN_16Z044_UNBLANK\
		_IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 	9 )
#define FBIO_MEN_16Z044_SWAP_ON\
		_IO(MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 10 )
#define FBIO_MEN_16Z044_SWAP_OFF\
	_IO(  MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 11 )

#define FBIO_MEN_16Z044_SET_SCREEN\
    _IOW( MEN_16Z044_IOC_MAGIC, MEN_16Z044_IOCBASE + 12 , unsigned int)

#endif
