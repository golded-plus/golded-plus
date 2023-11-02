# Distributed under the terms of the GNU General Public License v2

EAPI=8

inherit git-r3

DESCRIPTION="FTN Editor ${PN}"
EGIT_REPO_URI="https://github.com/golded-plus/golded-plus.git"
HOMEPAGE="http://golded-plus.sourceforge.net"

LICENSE="LGPL-2"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="spell -iconv -old_shift_fn -doc"

RDEPEND=">=sys-libs/ncurses-5.4
	spell? ( app-text/hunspell )
	iconv? ( virtual/libiconv )"
DEPEND="${RDEPEND}"
BDEPEND="doc? ( sys-apps/texinfo )"

src_prepare() {
	use iconv && ewarn "Use iconv library for recoding text. Experimental! Do not set this if you are unsure!"

	# Add header with personal configuration
	cp ${FILESDIR}/mygolded.h ./golded3/mygolded.h

	# Disable MS Office spellchecker support
	sed -i 's/#CPPFLAGS+=-DGCFG_NO_MSSPELL/CPPFLAGS+=-DGCFG_NO_MSSPELL/' ./Config.def

	# Disable Hunspell spellchecker support if need
	use !spell && sed -i 's/#CPPFLAGS+=-DGCFG_NO_MYSPELL/CPPFLAGS+=-DGCFG_NO_MYSPELL/' ./Config.def

	# Patch for dynamic linking of hunspell
	if use spell ; then
		eapply -p0 ${FILESDIR}/hunspell-dynlib.patch || die "eapply failed"
		rm -rf ./goldlib/hunspell
		HUNSPELL_LIB_VERSION=$(ver_cut 1-2 \
		$( echo $( best_version app-text/hunspell ) | sed -e "s~app-text/hunspell~~" ) )
		sed -i "s/STDLIBS+=-lhunspell/STDLIBS+=-lhunspell-${HUNSPELL_LIB_VERSION}/" ./golded3/Makefile
	fi

	# Fix linking error with libtinfo
	eapply -p0 ${FILESDIR}/libtinfo.patch || die "eapply failed"

	eapply_user
}

src_compile() {
	emake PLATFORM=lnx ICONV=$(usex iconv 1 0) OLD_SHIFT_FN=$(usex old_shift_fn 1 0) || die "emake failed"
	use doc && (emake docs || die "emake docs failed")
}

src_install() {
	dobin bin/gedlnx bin/gnlnx bin/rddtlnx
	doman docs/*.1
	use doc && \
		DOCS="docs/*.txt docs/notework.rus manuals/*.txt" \
		HTML_DOCS="docs/*.html" \
		einstalldocs
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
