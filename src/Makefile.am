## Process this file with automake to produce Makefile.in
# gre/src/Makefile.am
srcdir = @srcdir@
VPATH = @srcdir@
SUBDIRS = proj
DIST_SUBDIRS = proj
bin_PROGRAMS = gre
gre_LDADD = @LIBS@ proj/libgreproj.a
#gre_LDADD = @LIBS@ proj/libgreproj.a -ldmalloc
#gre_LDADD = @LIBS@ proj/libgreproj.a
#gre_LDADD = @LIBS@ proj/libgreproj.a -lefence
gredir = $(datadir)/gre

#gri_DATA = gri.cmd
#nodist_gre_SOURCES = startup.debian startup.redhat 

gre_SOURCES = parser.cpp\
 convert.cpp delete.cpp draw.cpp endup.cpp file.cpp graxes.cpp\
 grcntour.cpp gr_coll.cpp gr.cpp gre_main.cpp GriColor.cpp GriPath.cpp\
 GriState.cpp grstring.cpp image.cpp\
 popt.cpp print.cpp read.cpp regress.cpp scalar.cpp\
 scales.cpp set.cpp startup.cpp stats.cpp storage.cpp subs.cpp\
 template.cpp utility.cpp\
 debug.h defaults.h errors.h extern.h files.h GMatrix.h gr_coll.h gr.h\
 GriColor.h GriPath.h GriState.h image_ex.h macro.h parser.h popt.h\
 postscpt.h private.h scalar.h subs.h superus.h tags.h types.h

EXTRA_DIST = parser.y

parser.cpp: parser.y
	bison -v -o parser.cpp parser.y

PACKAGE_VERSION = @PACKAGE_VERSION@

#if OS_IS_LINUX_DEBIAN
#DEFS = -DDEFAULT_GRE_DIR=\"$(prefix)/share/gre/$(PACKAGE_VERSION)/\" @DEFS@
#else
DEFS = -DDEFAULT_GRE_DIR=\"$(prefix)/share/gre/\" @DEFS@
#endif

#if OS_IS_LINUX_DEBIAN
#DGD = $(prefix)/share/gre/$(PACKAGE_VERSION)/
#else
#DGD = $(prefix)/share/gre/
#endif
#DEFS = -DDEFAULT_GRE_DIR=\"$(DGD)\" @DEFS@

EXTRA_CFLAGS_TEMPLATE = @EXTRA_CFLAGS_TEMPLATE@ $(LINUX_EXTRA_CFLAGS)

#### End of system configuration section. ####

# Special rule for templates, so as not to use
# -fno-implicit-templates (g++ weirdness).  This special
# case depends on whether -frepo exists ... and whether the installer
# has temerity to try it!
template.o: template.cpp
	$(CXX) -c $(CXXFLAGS) $(EXTRA_CFLAGS_TEMPLATE) $(DEFS) -I$(srcdir) $<

#if OS_IS_LINUX_DEBIAN
debian_lib = $(debian)/usr/share/gre/$(PACKAGE_VERSION)
install_linux_debian: gre
	@echo "Installing into '$(debian)' directory"
	install -d $(debian_lib)
#	if test -f startup.debian; then sed -e s,VERSION,${PACKAGE_VERSION}, startup.debian > $(debian_lib)/startup.msg; else echo "WARNING: no startup.debian file!"; fi;
	install -d         $(debian)/usr/bin
	install -m 755 gre $(debian)/usr/bin/gre-$(PACKAGE_VERSION)
	(cd $(debian)/usr/bin; ln -fs gre-$(PACKAGE_VERSION) gre)
	install -m 644 gre.cmd  $(debian_lib)/gre.cmd
#	install -m 644 logo.dat $(debian_lib)/logo.dat
	if test -f gre_merge; then install -m 755 gre_merge $(debian)/usr/bin; else echo "WARNING: no gre_merge file!"; fi;	
	if test -f gre_unpage; then install -m 755 gre_unpage $(debian)/usr/bin; else echo "WARNING: no gre_unpage file!";fi;
#	install -d $(debian)/usr/share/emacs/site-lisp
#	install -m 644 gre-mode.el $(debian)/usr/share/emacs/site-lisp/

install_linux_debian_greonly: gre
	@echo "Installing into '$(debian)' directory"
	install -d             $(debian_lib)
	install -m 644 gre.cmd  $(debian_lib)/gre.cmd
#	install -m 644 logo.dat $(debian_lib)/logo.dat
#	if test -f startup.debian; then sed -e s,VERSION,${PACKAGE_VERSION}, startup.debian > $(debian_lib)/startup.msg; else echo "WARNING: no startup.debian file!"; fi;
	install -d         $(debian)/usr/bin
	install -m 755 gre $(debian)/usr/bin/gre-$(PACKAGE_VERSION)
#endif

check:
	@./gre -directory . $(srcdir)/../doc/tst_suite/tst_var_syn.gre 
	@./gre -directory . $(srcdir)/../doc/tst_suite/tst_control.gre
	@./gre -directory . $(srcdir)/../doc/tst_suite/tst_rpn.gre
# 	The next few lines are a kludge since I got bored with trying
# 	to figure out how to redirect things for automake.
	@cp $(srcdir)/../doc/tst_suite/tst_IO.gre   .
	@cp $(srcdir)/../doc/tst_suite/tst_IO_1.dat .
	@cp $(srcdir)/../doc/tst_suite/tst_IO_2.dat .
	@./gre -directory . tst_IO.gre
	@rm -f tst_IO.gre tst_IO_1.dat tst_IO_2.dat
#	@cat $(srcdir)/version.cc    | perl -ane 'if(/_number/)             {$$v=$$F[3]; $$v=~s/[^"]*"//; $$v=~s/".*//; die "ERROR: version number in version.cc is $$v, which disagrees with the version in the Makefile, $(PACKAGE_VERSION).  You MUST fix this.\n" if ("$$v" ne "$(PACKAGE_VERSION)"); print "Version number in version.cc       ... is OK\n";}'
#	@cat $(srcdir)/gre.cmd       | perl -ane 'if(/\(version ([^)]*)\)/) {$$v=$$1; die "ERROR: Version in gre.cmd is $$v, which disagrees with the version in the Makefile, $(PACKAGE_VERSION).  You MUST fix this.\n" if ("$$v" ne "$(PACKAGE_VERSION)"); print "Version number in gre.cmd          ... is OK\n";}'
#	@cat $(srcdir)/../doc/gre.texi  | perl -ane 'if(/GRE-VERSION (.*)/)    {$$v=$$1; die "ERROR: Version in ../doc/gre.texi is $$v, which disagrees with the version in the Makefile, $(PACKAGE_VERSION).  You MUST fix this.\n" if ("$$v" ne "$(PACKAGE_VERSION)"); print "Version number in ../doc/gre.texi    ... is OK\n";}'
	@cat $(srcdir)/../gre.spec      | perl -ane 'if(/greversion (.*)/)     {$$v=$$1; if ("$$v" ne "$(PACKAGE_VERSION)") {warn "ERROR: Version in gre.spec is $$v, which disagrees with the version in the Makefile, $(PACKAGE_VERSION).  You SHOULD fix this.\n"} else { print "Version number in gre.spec         ... is OK\n";}}'
	@cat $(srcdir)/../debian/changelog | perl -ne 'if (($$. == 1)&&(/\(([^-]+)-/)) {if ("$$1" ne "$(PACKAGE_VERSION)") {print "**WARNING: Version in debian/changelog is $$1,\n** which disagrees with the version in the Makefile, $(PACKAGE_VERSION).\n** You should fix this for official releases.\n"} else { print "Version number in debian/changelog ... is OK\n";}}'

# Sun microsystems, sunOS 5.x
# NOTE: this target doesn't depend on 'gre' or 'doc', etc; this
# is to be done manually, the first step perhaps on an 
# entirely different computer!!
SUNOS5_NAME = gre-$(PACKAGE_VERSION)-SunOS5
package-SunOS5:
	-rm -rf          $(SUNOS5_NAME)
	-mkdir -m 755 -p $(SUNOS5_NAME)
	-mkdir -m 755 -p $(SUNOS5_NAME)/bin
	-mkdir -m 755 -p $(SUNOS5_NAME)/lib
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc/html
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc/html/resources
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc/html/screenshots
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc/html/tst_suite
	-mkdir -m 755 -p $(SUNOS5_NAME)/doc/html/examples
	-cp -p gre                                                        $(SUNOS5_NAME)/bin
	-cp  gre.cmd copyright.txt license.txt gre-mode.el       $(SUNOS5_NAME)/lib
#	-cat startup.msg | sed -e s,VERSION,${PACKAGE_VERSION}, | sed -e "s/DATE/`date`/" > tmp
#	-cp tmp                                                           $(SUNOS5_NAME)/lib/startup.msg
	-cat install-SunOS5 | sed -e s,VERSION,${PACKAGE_VERSION}, > tmp
	-cp tmp                                                           $(SUNOS5_NAME)/install
	-chmod a+rx                                                       $(SUNOS5_NAME)/install
	-cp README-SunOS5                                                 $(SUNOS5_NAME)/README
	-cat ../doc/gre-manpage-SunOS5.1 | sed -e s,VERSION,${PACKAGE_VERSION}, >tmp
	-cp tmp                                                           $(SUNOS5_NAME)/doc/gre.1
	-rm -f tmp
#       Next should really be done by "cd doc; make something" etc.
	-cp ../doc/*html                                                     $(SUNOS5_NAME)/doc/html
	-cp ../doc/*png                                                      $(SUNOS5_NAME)/doc/html
	-cp ../doc/resources/*gif                                            $(SUNOS5_NAME)/doc/html/resources
	-cp ../doc/screenshots/*png                                          $(SUNOS5_NAME)/doc/html/screenshots
	-cp ../doc/tst_suite/*dat ../doc/tst_suite/*gre ../doc/tst_suite/*html     $(SUNOS5_NAME)/doc/html/tst_suite
	-cp ../doc/examples/*dat ../doc/examples/*gre ../doc/examples/*html        $(SUNOS5_NAME)/doc/html/examples
	-tar chof $(SUNOS5_NAME).tar $(SUNOS5_NAME)
	-rm -rf $(SUNOS5_NAME)
	-gzip $(SUNOS5_NAME).tar

# The '-local' part means that these are appended after the automake-created
# targets of names 'all', etc.
#if OS_IS_LINUX_REDHAT
#the_startup_file = $(srcdir)/startup.redhat
#else
#the_startup_file = $(srcdir)/startup.msg
#endif

#gre.cmd: gre.cmd-skel
#	cat $(srcdir)/gre.cmd-skel | sed -e s,VERSION,${PACKAGE_VERSION}, > gre.cmd
#startup.msg-tmp: $(the_startup_file)
#	cat $(the_startup_file)    | sed -e s,VERSION,${PACKAGE_VERSION}, -e s,PREFIX,$(prefix), > startup.msg-tmp

#all-local: gre.cmd startup.msg-tmp
all-local:

install-exec-local:
#	mkdir -m 755 -p                                  $(DESTDIR)$(prefix)/bin
#	$(INSTALL_PROGRAM) $(srcdir)/gre_unpage          $(DESTDIR)$(prefix)/bin/gre_unpage
#	$(INSTALL_PROGRAM) $(srcdir)/gre_unpage          $(DESTDIR)$(prefix)/bin/gre_merge
install-data-local:
	mkdir -m 755 -p                                  $(DESTDIR)$(prefix)/share/emacs/site-lisp
	$(INSTALL_DATA)    $(srcdir)/gre-mode.el         $(DESTDIR)$(prefix)/share/emacs/site-lisp/gre-mode.el
	mkdir -m 755 -p                                  $(DESTDIR)$(prefix)/share/gre
#	$(INSTALL_DATA)    startup.msg-tmp               $(DESTDIR)$(prefix)/share/gre/startup.msg
	$(INSTALL_DATA)    gre.cmd                       $(DESTDIR)$(prefix)/share/gre/gre.cmd
uninstall-local:
#	rm -f $(DESTDIR)$(prefix)/bin/gre_unpage
#	rm -f $(DESTDIR)$(prefix)/bin/gre_merge
	rm -f $(DESTDIR)$(prefix)/share/emacs/site-lisp/gre-mode.el
#	rm -f $(DESTDIR)$(prefix)/share/gre/startup.msg
	rm -f $(DESTDIR)$(prefix)/share/gre/gre.cmd
#	rm -f startup.msg-tmp
	rm -f gre.cmd
