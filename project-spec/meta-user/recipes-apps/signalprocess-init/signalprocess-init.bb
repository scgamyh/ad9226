#this file is the signalprocess-init recipe.
#
SUMMARY = "Simple signalprocess-init application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
 
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
 
SRC_URI = "file://signalprocess-init \
"
 
S = "${WORKDIR}"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
 
inherit update-rc.d
INITSCRIPT_NAME = "signalprocess-init"
INITSCRIPT_PARAMS = "start 99 5 ."
 
do_install() {
    install -d ${D}${sysconfdir}/init.d
    install -m 0755 ${S}/signalprocess-init ${D}${sysconfdir}/init.d/signalprocess-init
}
FILES_${PN} += "${sysconfdir}/*"

