# New ports collection makefile for: GoldED+
# Date created:        4 June 2003
# Whom:                alexander smishlajev
#
# $FreeBSD$
#

PORTNAME=	golded
PORTVERSION=	1.1.5.1.$(PORTSNAPSHOT)
CATEGORIES=	ftn
MASTER_SITES=	http://golded-plus.sourceforge.net
DISTNAME=	gps$(PORTSNAPSHOT)

MAINTAINER=	alex@tycobka.lv
COMMENT=	Full-featured mail/news reader for Fidonet and Internet

PORTSNAPSHOT=	0
USE_BZIP2=	yes

MAN1=		golded.1 goldnode.1 rddt.1
MANCOMPRESSED=	no
USE_GMAKE=	yes
NO_WRKSUBDIR=	yes
USE_REINPLACE=	yes

DOCS=		copying copying.lib license.txt notework.txt rusfaq.txt tips.txt
DOCDIR=		$(PREFIX)/share/golded

do-configure:
	@${SED} \
	  -e 's,^\(#define __GVER_VENDOR_NAME__\).*,\1 "Alexander S. Aganichev",' \
	  -e 's,\(#define __GVER_VENDOR_FIDO__\).*,\1 "2:5020/201.58",' \
	  -e 's,\(#define __GVER_VENDOR_EMAIL__\).*,\1 "asa@users.sf.net",' \
	  -e 's,\(#define __GVER_RELEASENAME__\).*,\1 "FreeBSD port",' \
	 <${WRKSRC}/golded3/mygolded.__h >${WRKSRC}/golded3/mygolded.h
.if defined(USE_KOI8)
	@${REINPLACE_CMD} -e 's,#-DKOI8,-DKOI8,' ${WRKSRC}/GNUmakef.def
.endif

# snapshot distribution uses CRLF line endings but gcc requires LF
# remove CR characters from all sources
# XXX is it possible to match \r?  how?
pre-build:
	@${FIND} ${WRKSRC} -name '*.cpp' -or -name '*.c' -or -name '*.h' \
	  -or -name '*.1' -or -name '*.txt' \
	  -or -name '*.cfg' -or -name '*.tpl' \
	  -or -name '*.chs' -or -name '*.esc' \
	 | ${XARGS} ${REINPLACE_CMD} -e 's,[[:cntrl:]]$$,,'
	@${FIND} ${WRKSRC} -name '*.bak' -delete

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/bin/gedlnx ${PREFIX}/bin/gedbsd
	${INSTALL_PROGRAM} ${WRKSRC}/bin/gnlnx ${PREFIX}/bin/gnbsd
	${INSTALL_PROGRAM} ${WRKSRC}/bin/rddtlnx ${PREFIX}/bin/rddtbsd
	for dir in charset colorset config template; do \
	 ${MKDIR} ${DOCDIR}/$$dir; \
	 for file in `${LS} ${WRKSRC}/cfgs/$$dir|${GREP} -v CVS`; do\
	  ${INSTALL_DATA} ${WRKSRC}/cfgs/$$dir/$$file ${DOCDIR}/$$dir; \
	 done; \
	done
	for f in $(MAN1); do \
	 ${INSTALL_MAN} ${WRKSRC}/docs/$$f ${MAN1PREFIX}/man/man1; \
	done
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCDIR}
	for f in $(DOCS); do \
	 ${INSTALL_MAN} ${WRKSRC}/docs/$$f ${DOCDIR}; \
	done
.endif

.include <bsd.port.mk>
