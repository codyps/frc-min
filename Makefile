all::
CROSS_COMPILE=powerpc-wrs-vxworks-

obj-FRC_UserProgram.out = main.o

TARGETS = FRC_UserProgram.out

include base.mk

export WIND_BASE:=$(dir $(shell which $(CC)))../powerpc-wrx-vxworks/sys-include/
