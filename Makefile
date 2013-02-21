all::
CROSS_COMPILE=powerpc-wrs-vxworks-

obj-FRC_UserProgram.out = main.o vxworks-util.o vxworks-no-munch.o

TARGETS = FRC_UserProgram.out

include base.mk

export WIND_BASE:=/var/empty
