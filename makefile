######################*license start*###################################
# OCTEON SDK
# 
# Copyright (c) 2003-2005 Cavium Networks. All rights reserved.
# 
# This file, which is part of the OCTEON SDK from Cavium Networks,
# contains proprietary and confidential information of Cavium Networks and
# its suppliers.
# 
# Any licensed reproduction, distribution, modification, or other use of
# this file or the confidential information or patented inventions
# embodied in this file is subject to your license agreement with Cavium
# Networks. Unless you and Cavium Networks have agreed otherwise in
# writing, the applicable license terms can be found at:
# licenses/cavium-license-type2.txt
# 
# All other use and disclosure is prohibited.
# 
# Contact Cavium Networks at info@caviumnetworks.com for more information.
#####################*license end*#####################################/

#
# File version info: $Id: Makefile 26394 2007-07-25 02:27:17Z rfranz $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

#  standard common Makefile fragment
                                                                                
include $(OCTEON_ROOT)/common.mk

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter -I$(OCTEON_ROOT)/target/include
CFLAGS = -g -Wall 

CFLAGS_HILI = $(CFLAGS)  -I$(OCTEON_ROOT)/executive/ -DUSE_RUNTIME_MODEL_CHECKS=1 -DCVMX_ENABLE_PARAMETER_CHECKING=0 -DCVMX_ENABLE_CSR_ADDRESS_CHECKING=0 -DCVMX_ENABLE_POW_CHECKS=0  -DOCTEON_MODEL=OCTEON_CN58XX -DOCTEON_TARGET=cvmx_64 -g -O2 -W -Wall -Wno-unused-parameter -I$(OCTEON_ROOT)/bootloader/u-boot/include/ -MD -Wall -Wa,-a=$@.list -Wno-unused-parameter -O2 -g -fno-strict-aliasing  -DTEMP_SDK_BUILD_NUMBER=`oct-version | sed -n "s/.* build \(.*\)/\1/p"` -DGCC_VERSION=\""mipsisa64-octeon-elf-gcc (GCC) 4.1.2 (Cavium Networks Version: 1_7_3, build 58)\"" -DSDK_VERSION=\""`oct-version`\""

TARGET = main

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
all:
	echo "hello: This example doesn't support Linux"
else
all: $(TARGET)
endif

OBJS = main.o
$(TARGET): $(OBJS)
	mipsisa64-octeon-elf-gcc -o $(TARGET) $(OBJS)
run: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -numcores=1

run4: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -numcores=4
hili:
	mipsisa64-octeon-elf-gcc $(CFLAGS_HILI) -c main.c
	mipsisa64-octeon-elf-gcc -o main.o
clean:
	rm -f  $(TARGET) 
	rm -rf  $(OBJS)
