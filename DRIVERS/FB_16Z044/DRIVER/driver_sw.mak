#**************************  M a k e f i l e ********************************
#  
#         Author: ts
#          $Date: 2007/04/13 13:40:23 $
#      $Revision: 1.1 $
#  
#    Description: makefile descriptor for swapped 16Z025 Module
#                      
#-----------------------------------------------------------------------------
#   Copyright (c) 2007-2019, MEN Mikro Elektronik GmbH
#*****************************************************************************

MAK_NAME=lx_z44_sw

MAK_LIBS=

MAK_SWITCH=$(SW_PREFIX)MAC_MEM_MAPPED \
	   $(SW_PREFIX)MAC_BYTESWAP

MAK_INCL=$(MEN_INC_DIR)/../../NATIVE/MEN/men_chameleon.h 

MAK_INP1=fb_men_16z044$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)

