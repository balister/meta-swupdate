SUMMARY="Image updater for Yocto projects"
DESCRIPTION = "Application for automatic software update from USB Pen"
SECTION="swupdate"
DEPENDS = "mtd-utils libconfig libarchive openssl lua curl json-c u-boot-fw-utils gnutls"
LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=0636e73ff0215e8d672dc4c32c317bb3"

inherit cml1

SRC_URI = "git://github.com/sbabic/swupdate.git;protocol=git \
     file://defconfig \
     "

PACKAGES =+ "${PN}-www"

FILES_${PN}-www = "/www/*"
FILES_${PN}-dev = "${includedir}"
FILES_${PN}-staticdev = "${libdir}"
FILES_${PN} = "${bindir}/* /etc/init.d"
CONFFILES_${PN} += "${sysconfdir}/init.d/recovery"

S = "${WORKDIR}/git/"

EXTRA_OEMAKE += "V=1 ARCH=${TARGET_ARCH} CROSS_COMPILE=${TARGET_PREFIX} SKIP_STRIP=y"

do_configure () {
  cp ${WORKDIR}/defconfig ${S}/.config
  cml1_do_configure
}

do_install () {
  install -d ${D}${bindir}/
  install -m 0755 swupdate ${D}${bindir}/

  install -m 0755 -d ${D}/www
  install -m 0755 ${S}www/* ${D}/www

  install -d ${D}${libdir}/
  install -d ${D}${includedir}/
  install -m 0644 ${S}include/network_ipc.h ${D}${includedir}
  install -m 0755 ${S}ipc/lib.a ${D}${libdir}/libswupdate.a
}

do_compile() {
  unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
  oe_runmake swupdate_unstripped
  cp swupdate_unstripped swupdate
}
