SUMMARY = "Fast open source processor emulator"
HOMEPAGE = "http://qemu.org"
LICENSE = "GPLv2 & LGPLv2.1"
DEPENDS = "glib-2.0 zlib pixman"
RDEPENDS_${PN}_class-target += "bash"

require qemu-targets.inc
inherit autotools pkgconfig bluetooth
BBCLASSEXTEND = "native nativesdk"

# If SRCREV equals '${AUTOREV}', then specify 'branch=hpsc'
# in SRC_URI, else specify 'nobranch=1'
SRC_URI = "${@ "git://github.com/ISI-apex/qemu.git;protocol=git;branch=hpsc" if (d.getVar('SRCREV') == '${AUTOREV}') else "git://github.com/ISI-apex/qemu.git;protocol=git;nobranch=1" }"

# QEMU_TARGETS is overridable variable
QEMU_TARGETS ?= "aarch64"

EXTRA_OECONF = " \
    --prefix=${prefix} \
    --bindir=${bindir} \
    --includedir=${includedir} \
    --libdir=${libdir} \
    --mandir=${mandir} \
    --datadir=${datadir} \
    --docdir=${docdir}/${BPN} \
    --sysconfdir=${sysconfdir} \
    --libexecdir=${libexecdir} \
    --localstatedir=${localstatedir} \
    --with-confsuffix=/${BPN} \
    --disable-strip \
    --disable-werror \
    --target-list=${@get_qemu_target_list(d)} \
    --extra-cflags='${CFLAGS}' \
    "
EXTRA_OECONF_append_class-native = " --python=${USRBINPATH}/python2.7"

EXTRA_OEMAKE_append_class-native = " LD='${LD}' AR='${AR}' OBJCOPY='${OBJCOPY}' LDFLAGS='${LDFLAGS}'"

LDFLAGS_append_class-native = " -fuse-ld=bfd"

export LIBTOOL="${HOST_SYS}-libtool"

S = "${WORKDIR}/git"

do_configure() {
    ${S}/configure --target-list="aarch64-softmmu" --enable-fdt --disable-kvm --disable-xen
}

INSANE_SKIP_nativesdk-qemu = "already-stripped"
FILES_${PN} += "/usr/*"
