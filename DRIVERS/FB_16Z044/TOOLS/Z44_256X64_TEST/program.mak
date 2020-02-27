#**************************  M a k e f i l e ********************************
#   Description: makefile for framebuffer fb16z044_256x64_test
#-----------------------------------------------------------------------------
#   Copyright 2019, MEN Mikro Elektronik GmbH
#*****************************************************************************
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

MAK_NAME=fb16z044_256x64_test
# the next line is updated during the MDIS installation
STAMPED_REVISION="13Z044-90_01_08-18-g6ebc5a9_2020-01-08"

DEF_REVISION=MAK_REVISION=$(STAMPED_REVISION)
MAK_SWITCH=$(SW_PREFIX)$(DEF_REVISION)
MAK_INCL=$(MEN_LIN_DIR)/DRIVERS/FB_16Z044/TOOLS/Z44_256X64_TEST/logos.h
MAK_INP1=$(MAK_NAME)$(INP_SUFFIX)
MAK_INP=$(MAK_INP1)
