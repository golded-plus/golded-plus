# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

EAPI=2

ECVS_AUTH="pserver"
ECVS_SERVER="golded-plus.cvs.sourceforge.net:/cvsroot/golded-plus"
ECVS_MODULE="${PN}"
ECVS_LOCALNAME="${PN}"

inherit cvs

DESCRIPTION="FTN Editor ${PN}"
SRC_URI=""
HOMEPAGE="http://golded-plus.sourceforge.net"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="spell -doc"
DEPEND=">=sys-libs/ncurses-5.4
	spell? ( app-dicts/myspell-${LINGUAS} )
	doc? ( sys-apps/texinfo )"

S="${WORKDIR}/${ECVS_LOCALNAME}"

src_unpack() {
	cvs_src_unpack
}

src_prepare() {
	cd "${S}"

	# Add header with personal configuration
	cp ${FILESDIR}/mygolded.h ./golded3/mygolded.h

	# Disable MS Office spellchecker support
	sed -i 's/#CPPFLAGS+=-DGCFG_NO_MSSPELL/CPPFLAGS+=-DGCFG_NO_MSSPELL/' ./Config.def

	# Disable HunSpell spellchecker support if need
	use spell || sed -i 's/#CPPFLAGS+=-DGCFG_NO_MYSPELL/CPPFLAGS+=-DGCFG_NO_MYSPELL/' ./Config.def

}

src_compile() {
	cd "${S}"

	emake PLATFORM=lnx 
	use doc && emake docs
}

src_install() {
	cd "${S}"

	dobin bin/gedlnx bin/gnlnx bin/rddtlnx
	doman docs/*.1
	if useq doc ; then
		dodoc docs/*.txt docs/notework.rus
		dodoc manuals/*.txt
		dohtml docs/*.html
	fi
	insinto /usr/share/golded-plus
	doins bin/screenrc_koi8r
	exeinto /etc/ftn/golded.sample
	doexe bin/golded
	insinto /etc/ftn/golded.sample/charset
	doins cfgs/charset/*
	insinto /etc/ftn/golded.sample/colorset
	doins cfgs/colorset/*
	insinto /etc/ftn/golded.sample/config
	doins cfgs/config/*
	insinto /etc/ftn/golded.sample/template
	doins cfgs/template/*
}
