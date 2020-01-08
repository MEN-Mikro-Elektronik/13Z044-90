# 13z04490

**Linux native framebuffer driver package for MEN 16Z044 IP core**


To build the tools add below .mak files into main MDIS Makefile:

ALL_NATIVE_TOOLS = \\\
   TOOLS/FB_TEST/program.mak \\\
   TOOLS/FB_TEST/fb16z044_256x64_test/program.mak
