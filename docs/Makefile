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

rus: rusfaq.koi8r rusfaq.utf8 notework.koi8r notework.utf8
rusfaq.koi8r: rusfaq.txt
	iconv -f cp866 -t koi8-r -o rusfaq.koi8r rusfaq.txt
rusfaq.utf8: rusfaq.txt
	iconv -f cp866 -t utf8 -o rusfaq.utf8 rusfaq.txt
notework.koi8r: notework.rus
	iconv -f cp866 -t koi8-r -o notework.koi8r notework.rus
notework.utf8: notework.rus
	iconv -f cp866 -t utf8 -o notework.utf8 notework.rus

%.html: %.1
	groff -Thtml -mman $< >$@

%.txt: %.1
	groff -Tascii  -mman $< | sed s/\\[[0-9][0-9]*m//g >$@

clean:
	-rm $(HTMLS) $(TXTS)

tokentpl.txt: ../golded3/gemsgs.cpp
	echo '* Golded+ macroses list.' > $@
	echo '==========================================================================' >> $@
	echo '* These macroses may be used in:' >> $@
	echo '* template file, externutil command line, tearline and tagline.' >> $@
	echo '==========================================================================' >> $@
	grep tokenxchg $< | sed -ne 's/^.*"\(@\w*\)".*$$/\1/p' | sort -u >> $@
	@echo '==========================================================================' >> $@
	echo '* These macroses is line prefixes for the template file.' >> $@
	@echo '==========================================================================' >> $@
	cat ../golded3/getpls.cpp | sed -ne cat ../golded3/getpls.cpp \
	  | sed -ne '/ \{0,1\}{ CSTR_COMMA_SIZEOF_CSTR("/s/.\{1,\}"\(\w\{1,\}\)".\{1,\}/@\1/p' \
	  | sort -u >> $@
	@echo '==========================================================================' >> $@

#    { CSTR_COMMA_SIZEOF_CSTR("write")        },
#    { CSTR_COMMA_SIZEOF_CSTR("header")       }

tokencfg.txt: ../golded3/gccfgg.h
	@echo '* Golded+ config file tokens list.' > $@
	@echo '==========================================================================' >> $@
	grep CRC_ $< | sed -e '/CRC_LAST_CRC/d' -e 's/^.* CRC_\(\w*\).*$$/\1/' | sort -u >> $@
