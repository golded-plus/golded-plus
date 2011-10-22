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
	doc? ( sys-apps/texinfo )"
RDEPEND="${DEPEND}"

pdep=""
for l in \
"af" "bg" "ca" "cs" "cy" "da" "de" "de-alt" "el" "en" \
"eo" "es" "et" "fo" "fr" "ga" "gl" "he"     "hr" "hu" \
"ia" "id" "it" "ku" "lt" "lv" "mi" "mk"     "ms" "nb" \
"nl" "nn" "pl" "pt" "ro" "ru" "sk" "sl"     "sv" "sw" \
"tn" "uk" "zu" \
; do
dep="linguas_${l}? ( app-dicts/myspell-${l} )"
[[ -z ${pdep} ]] &&
pdep="${dep}" ||
pdep="${pdep}
${dep}"
IUSE="${IUSE} linguas_${l}"
done

PDEPEND="spell? ( ${pdep} )"

S="${WORKDIR}/${ECVS_LOCALNAME}"

src_unpack() {
	cvs_src_unpack
}

src_prepare() {
	# Add header with personal configuration
	cp ${FILESDIR}/mygolded.h ./golded3/mygolded.h

	# Disable MS Office spellchecker support
	sed -i 's/#CPPFLAGS+=-DGCFG_NO_MSSPELL/CPPFLAGS+=-DGCFG_NO_MSSPELL/' ./Config.def

	# Disable HunSpell spellchecker support if need
	use spell || sed -i 's/#CPPFLAGS+=-DGCFG_NO_MYSPELL/CPPFLAGS+=-DGCFG_NO_MYSPELL/' ./Config.def

}

src_compile() {
	emake PLATFORM=lnx || die "emake failed"
	use doc && emake docs || die "emake docs failed"
}

src_install() {
	dobin bin/gedlnx bin/gnlnx bin/rddtlnx
	doman docs/*.1
	if use doc ; then
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
