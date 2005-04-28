# -*- makefile  to build documentation-*-

all: html-doc

html-doc: golded.html goldnode.html rddt.html

%.html: %.1
	groff -Thtml -mman $< >$@
