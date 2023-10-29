#
# This file is the signalprocess recipe.
#

SUMMARY = "Simple signalprocess application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://signalprocess.c \
	   file://cqueue.c \
	   file://cqueue.h \
	   file://networkcqueue.c \
	   file://networkcqueue.h \
           file://global.h \
	   file://threadwork.c \
	   file://threadwork.h \
	   file://Makefile \
	   file://axidma_ioctl.h \
	   file://conversion.h \
	   file://libaxidma.c \
	   file://libaxidma.h \
	   file://util.c \
	   file://util.h \
	   file://threadpool.c \
	   file://threadpool.h \
	   file://fft.c \
	   file://fft.h \
	   file://ADF4351.h \
	   file://ADF4351.c \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 signalprocess ${D}${bindir}
}
