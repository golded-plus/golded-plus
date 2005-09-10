# $Id$
# -*- makefile to build some Golded+ documentation -*-
# (c) Stas Degteff 2:5080/102@FIDONET, stas_degteff@users.sourceforge.net

HTMLS= golded.html goldnode.html rddt.html
TXTS=  golded.txt goldnode.txt rddt.txt


all: html-doc txt-doc

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
