## Process this file with automake to produce Makefile.in
# gre: doc/Makefile.am
             srcdir = @srcdir@
              VPATH = @srcdir@
      info_TEXINFOS = gre.texi
    PACKAGE_VERSION = @PACKAGE_VERSION@
if OS_IS_LINUX_REDHAT
            MAN_DIR = $(DESTDIR)$(prefix)/share/man/man1
           INFO_DIR = $(DESTDIR)$(prefix)/share/info
            DOC_DIR = $(DESTDIR)$(prefix)/share/doc/gre-${PACKAGE_VERSION}
           HTML_DIR = $(DOC_DIR)/html
#       EXAMPLES_DIR = $(DOC_DIR)/examples
else
if OS_IS_LINUX_DEBIAN
            MAN_DIR = $(DESTDIR)$(prefix)/share/man/man1
           INFO_DIR = $(DESTDIR)$(prefix)/share/info
            DOC_DIR = $(DESTDIR)$(prefix)/share/doc/gre
           HTML_DIR = $(DOC_DIR)/html
#       EXAMPLES_DIR = $(DOC_DIR)/examples
else
            MAN_DIR = $(DESTDIR)$(prefix)/share/man/man1
           INFO_DIR = $(DESTDIR)$(prefix)/share/info
            DOC_DIR = $(DESTDIR)$(prefix)/share/gre/doc
           HTML_DIR = $(DOC_DIR)/html
#       EXAMPLES_DIR = $(DOC_DIR)/examples
endif
endif
                 RM = rm -f
     HTML_LONG_NAME = gre-long
#   INFO_DIR_SOLARIS = $(DESTDIR)$(prefix)/info

#SUBDIRS = examples resources screenshots tst_suite
SUBDIRS = tst_suite figures
DIST_SUBDIRS = tst_suite figures
EXTRA_DIST = \
 install-sh\
 gre2texi texinfo2HTML gre2html HTML_subdivide\
 make_html_index make_html_commandindex make_html_builtinindex

html: $(srcdir)/gre.texi
#	cd examples ; make
	cd tst_suite ; make
# 	Make the info page (even if rewritten later) to get indices ...
	$(MAKEINFO) $(AM_MAKEINFOFLAGS) $(MAKEINFOFLAGS) -I$(srcdir) gre.texi
#	... but this is clumsy and should be done with a dependency.
	cat $(srcdir)/gre.texi           	>  TMP
	perl $(srcdir)/make_html_index		>> TMP
	perl $(srcdir)/make_html_commandindex	>> TMP
	perl $(srcdir)/make_html_builtinindex	>> TMP
	cat TMP | perl $(srcdir)/texinfo2HTML	>  $(HTML_LONG_NAME)
	$(RM) TMP
	$(RM) gre[1-9]*.html
	perl $(srcdir)/HTML_subdivide  $(HTML_LONG_NAME) "The Gre graphing language"
	$(RM) $(HTML_LONG_NAME)
	$(RM) *.pass2
	if test ! -f index.html ; then ln -s gre1.html index.html ; fi
	touch html

### html-install:
### # 	NB. Be careful in editing this target.  Things that are
### # 	created during the building process do not have $(srcdir)
### #	in their names; only things in the tarball have that prefix.
### 	${INSTALL} -d                                 $(HTML_DIR)
### 	${INSTALL} -d                                 $(HTML_DIR)/resources
### 	$(INSTALL_DATA) $(srcdir)/resources/*.gif     $(HTML_DIR)/resources
### 	${INSTALL} -d                                 $(HTML_DIR)/tst_suite
### 	$(INSTALL_DATA)           tst_suite/*html     $(HTML_DIR)/tst_suite
### 	$(INSTALL_DATA) $(srcdir)/examples/*.gre      $(HTML_DIR)/
### 	$(INSTALL_DATA)           examples/*.html     $(HTML_DIR)/
### 	$(INSTALL_DATA)           examples/*.png      $(HTML_DIR)/
### 	${INSTALL} -d                                 $(HTML_DIR)/screenshots
### 	$(INSTALL_DATA) $(srcdir)/screenshots/*.png   $(HTML_DIR)/screenshots
### 	$(INSTALL_DATA)           *.html              $(HTML_DIR)
### 	${INSTALL} -d                                 $(EXAMPLES_DIR)
### 	$(INSTALL_DATA) $(srcdir)/examples/model*     $(EXAMPLES_DIR)
### 	$(INSTALL_DATA) $(srcdir)/examples/*.dat      $(EXAMPLES_DIR)
### 	$(INSTALL_DATA) $(srcdir)/examples/*.gre      $(EXAMPLES_DIR)
### 	$(INSTALL_DATA) $(srcdir)/examples/e*.ps      $(EXAMPLES_DIR)
### 	$(INSTALL_PROGRAM) $(srcdir)/examples/FEM.pl  $(EXAMPLES_DIR)
### 	(cd $(HTML_DIR); rm -rf examples ; ln -sf ../examples)

gre.ps: gre.texi
#	cd examples    ; make eps
#	cd screenshots ; make eps
	cd tst_suite   ; make texi
	tex gre.texi
	texindex gre.cp
	texindex gre.fn
	texindex gre.ky
	texindex gre.pg
	texindex gre.tp
	texindex gre.vr
	tex gre.texi
	dvips -o gre.ps -t letter gre.dvi

gre.pdf: gre.texi
#	cd examples    ; make pdf
#	cd screenshots ; make pdf
	cd tst_suite   ; make texi
	pdftex gre.texi
	texindex gre.cp
	texindex gre.fn
	texindex gre.ky
	texindex gre.pg
	texindex gre.tp
	texindex gre.vr
	pdftex gre.texi

# Add to some of the automake-created targets.
all-local:
	make html
if OS_IS_LINUX_REDHAT
gre_manpage_name = gre-manpage-redhat.1
else
gre_manpage_name = gre-manpage.1
endif

install-data-local:
	make html-install
	$(INSTALL) -m 644 $(srcdir)/../license.txt         $(DOC_DIR)
	mkdir -m 755 -p                                    $(MAN_DIR)
	cat $(srcdir)/../doc/$(gre_manpage_name) | sed -e s,VERSION,${PACKAGE_VERSION}, > tmp
	$(INSTALL_DATA)    tmp                             $(MAN_DIR)/gre.1
	$(RM) tmp
#	Install manpages manually; man_MANS puts them in /usr/man/man1, which is not where
#	they are supposed to go, at least on linux/redhat systems [2003-jun-8 Dan Kelley]
	mkdir -m 755 -p                                    $(INFO_DIR)
	$(INSTALL_DATA)    $(srcdir)/../doc/gre.info*      $(INFO_DIR)
if !OS_IS_LINUX_REDHAT
#	This is handled by gre.spec [2003-may-31 Dan Kelley]
	cd  $(INFO_DIR) ; gzip -f --best gre.info*
endif


uninstall-local:
#	$(RM) `ls ../*ps ../license.txt *.html gre.info* $(HTML_DIR)/resources/* $(HTML_DIR)/tst_suite/* $(HTML_DIR)/*`
	$(RM) `ls ../*ps ../license.txt *.html gre.info* $(HTML_DIR)/tst_suite/* $(HTML_DIR)/*`
	$(RM) html
	$(RM) $(DOC_DIR)/license.txt
	$(RM) $(MAN_DIR)/gre.*
	$(RM) $(INFO_DIR)/gre.info*
clean-local:
	rm gre.info*

