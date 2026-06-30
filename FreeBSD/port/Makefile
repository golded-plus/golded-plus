PORTNAME=       golded+
PORTVERSION=    1.1.5.20260425
CATEGORIES=     news mail

MAINTAINER=     ports@FreeBSD.org
COMMENT=        GoldEd+ (FTN message editor)
WWW=            https://github.com/golded-plus/golded-plus

# GoldED+ uses the GNU General Public License v2
LICENSE=        GPLv2
LICENSE_FILE=   ${WRKSRC}/docs/copying

USES=           gmake ncurses

USE_GITHUB=     yes
GH_ACCOUNT=     golded-plus
GH_PROJECT=     golded-plus
GH_TAGNAME=     golded-plus-1.1.5-20260425

SNAPDATE=       20260425

DOCS_DOCS=      copying copying.lib license.txt notework.txt rusfaq.txt tips.txt
DOCS_MANUALS=   gold_ref.txt gold_usr.txt
PORTDOCS=       ${DOCS_DOCS} ${DOCS_MANUALS}
PORTEXAMPLES=   *

OPTIONS_DEFINE= DOCS EXAMPLES KOI8
OPTIONS_DEFAULT=DOCS EXAMPLES
KOI8_DESC=      Enable KOI8-R charset support

do-configure:
        @${SED} \
        -e 's,^\(#define __GVER_VENDOR_NAME__\).*,\1 "",' \
        -e 's,^\(#define __GVER_VENDOR_FIDO__\).*,\1 "",' \
        -e 's,^\(#define __GVER_VENDOR_EMAIL__\).*,\1 "${MAINTAINER}",' \
        -e 's,^\(#define __GVER_RELEASENAME__\).*,\1 "Golded+ FreeBSD port",' \
        <${WRKSRC}/golded3/mygolded.__h >${WRKSRC}/golded3/mygolded.h

.include <bsd.port.options.mk>

do-configure-KOI8-on:
        @${REINPLACE_CMD} -e 's,#-DKOI8,-DKOI8,' ${WRKSRC}/GNUmakef.def

do-install:
        ${INSTALL_PROGRAM} ${WRKSRC}/bin/gedlnx ${STAGEDIR}${PREFIX}/bin/golded
        ${INSTALL_PROGRAM} ${WRKSRC}/bin/gnlnx ${STAGEDIR}${PREFIX}/bin/goldnode
        ${INSTALL_PROGRAM} ${WRKSRC}/bin/rddtlnx ${STAGEDIR}${PREFIX}/bin/rddt
        ${INSTALL_MAN} ${WRKSRC}/docs/*.1 ${STAGEDIR}${PREFIX}/share/man/man1

do-install-EXAMPLES-on:
        @${MKDIR} ${STAGEDIR}${EXAMPLESDIR}/charset
        (cd ${WRKSRC}/cfgs/charset && ${COPYTREE_SHARE} . ${STAGEDIR}${EXAMPLESDIR}/charset)

do-install-DOCS-on:
        @${MKDIR} ${STAGEDIR}${DOCSDIR}
        ${INSTALL_DATA} ${DOCS_DOCS:S,^,${WRKSRC}/docs/,} ${STAGEDIR}${DOCSDIR}
        ${INSTALL_DATA} ${DOCS_MANUALS:S,^,${WRKSRC}/manuals/,} ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
