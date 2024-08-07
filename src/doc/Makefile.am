#
# avrdude - A Downloader/Uploader for AVR device programmers
# Copyright (C) 2003 Theodore A. Roth <troth@openavr.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

CLEANFILES =
DISTCLEANFILES =
EXTRA_DIST =

AM_MAKEINFOFLAGS =
AM_MAKEINFOHTMLFLAGS =

# This does NOT create texi2html compatible HTML file names
# AM_MAKEINFOHTMLFLAGS += --number-sections

EXTRA_DIST           += avrdude.css
AM_MAKEINFOHTMLFLAGS += --css-include=$(srcdir)/avrdude.css

AM_MAKEINFOHTMLFLAGS += --split=node

info_TEXINFOS = avrdude.texi
CLEANFILES += avrdude.info
# DISTCLEANFILES += avrdude.info
avrdude_TEXINFOS  =


if ENABLED_DOC
all-local: html pdf
install-data-local: install-html install-pdf
all-local: thtml
install-data-local: install-thtml
endif # ENABLED_DOC


########################################################################
# Use texi2html to produce HTML output (like cmake build)
#
# The texi2html output looks very different from the makeinfo HTML
# output and uses very different file names, so this emulates what the
# cmake build does.
########################################################################

TEXI2HTML = texi2html
TEXI2HTMLFLAGS =
TEXI2HTMLFLAGS += --split=node
TEXI2HTMLFLAGS += --css-include=$(srcdir)/avrdude.css

thtml: avrdude.thtml
avrdude.thtml: avrdude.texi $(srcdir)/version.texi $(srcdir)/avrdude.css $(avrdude_TEXINFOS)
	$(AM_V_at)rm -rf $(@:.thtml=.tmphtml)
	$(AM_V_at)if $(TEXI2HTML) -o avrdude.tmphtml $(TEXI2HTMLFLAGS) `test -f 'avrdude.texi' || echo '$(srcdir)/'`avrdude.texi; then \
	  rm -rf $@ && mv $(@:.thtml=.tmphtml) $@; \
	else \
	  rm -rf $(@:.thtml=.tmphtml); exit 1; \
	fi

install-thtml: avrdude.thtml
	$(MKDIR_P) $(DESTDIR)$(htmldir)
	$(MKDIR_P) $(DESTDIR)$(htmldir)/avrdude.thtml
	$(INSTALL_DATA) avrdude.thtml/* $(DESTDIR)$(htmldir)/avrdude.thtml


########################################################################
# Run the recently built avrdude to produce documentation files
########################################################################

# if it does not exist make this first
../avrdude$(EXEEXT):
	$(MAKE) -C .. avrdude$(EXEEXT)

EXTRA_DIST       += programmers.sed
CLEANFILES       += $(builddir)/programmers.texi
avrdude_TEXINFOS += $(builddir)/programmers.texi
$(builddir)/programmers.texi: ../avrdude$(EXEEXT) ../avrdude.conf $(srcdir)/programmers.sed Makefile
	../avrdude$(EXEEXT) -C ../avrdude.conf -c \? 2>&1 \
	|  grep = | sed -f $(srcdir)/programmers.sed \
	>programmers.texi

CLEANFILES       += $(builddir)/programmer_types.texi
avrdude_TEXINFOS += $(builddir)/programmer_types.texi
$(builddir)/programmer_types.texi: ../avrdude$(EXEEXT) ../avrdude.conf Makefile
	../avrdude$(EXEEXT) -C ../avrdude.conf -c \?type 2>&1 \
	| $(AWK) '$$2 ~ /^=$$/ {printf("@item @code{%s} @tab %s\n",$$1,gensub("[^=]+=[ \t]*","",1))}' \
	| sed "s#<\?\(http://[^ \t,>]*\)>\?#@url{\1}#g" \
	>programmer_types.texi

EXTRA_DIST       += parts.sed
CLEANFILES       += $(builddir)/parts.texi
avrdude_TEXINFOS += $(builddir)/parts.texi
$(builddir)/parts.texi: ../avrdude$(EXEEXT) ../avrdude.conf $(srcdir)/parts.sed Makefile
	../avrdude$(EXEEXT) -C ../avrdude.conf -p \? 2>&1 \
	| grep = | sed -f $(srcdir)/parts.sed \
	>parts.texi

CLEANFILES       += $(builddir)/avrstats.texi
avrdude_TEXINFOS += $(builddir)/avrstats.texi
$(builddir)/avrstats.texi: ../avrdude$(EXEEXT) ../avrdude.conf Makefile
	: > avrstats.texi
	echo -n "@set NUMPARTS " >> avrstats.texi
	../avrdude$(EXEEXT) -C ../avrdude.conf -p \? 2>&1 | grep = | wc -l >> avrstats.texi
	echo -n "@set NUMPROGRAMMERS " >> avrstats.texi
	../avrdude$(EXEEXT) -C ../avrdude.conf -c \? 2>&1 | grep = | wc -l >> avrstats.texi

# FIXME: Why does this have no effect on avrdude.info?
dist-hook:
	cd "$(distdir)" && rm -f $(DISTCLEANFILES) $(CLEANFILES)
