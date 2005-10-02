# $Id$
# -*- makefile to build some Golded+ documentation -*-
# (c) Stas Degteff 2:5080/102@FIDONET, stas_degteff@users.sourceforge.net

HTMLS= golded.html goldnode.html rddt.html
TXTS=  golded.txt goldnode.txt rddt.txt


all: html-doc txt-doc tokentpl.txt tokencfg.txt

html: html-doc
html-doc: $(HTMLS)

txt: txt-doc
txt-doc:  $(TXTS)


%.html: %.1
	groff -Thtml -mman $< >$@

%.txt: %.1
	groff -Tascii  -mman $< | sed s/\\[[0-9][0-9]*m//g >$@

clean:
	-rm $(HTMLS) $(TXTS)

tokentpl.txt: ../golded3/gemsgs.cpp
	echo '* Golded+ macroses list.' > $@
	echo '* These macroses may be used in:' > $@
	echo '* template file, externutil command line, tearline and tagline.' > $@
	echo '* \$$Id$$' >> $@
	echo '==========================================================================' >> $@
	grep tokenxchg $< | sed -e '/^static bool tokenxchg/d' -e 's/^.*"\(@\w*\)".*$$/\1/' | sort -u >> $@

tokencfg.txt: ../golded3/gccfgg.h
	@echo '* Golded+ config file tokens list.' > $@
	@echo '* \$$Id$$' >> $@
	@echo '==========================================================================' >> $@
	grep CRC_ $< | sed -e '/CRC_LAST_CRC/d' -e 's/^.* CRC_\(\w*\).*$$/\1/' | sort -u >> $@
