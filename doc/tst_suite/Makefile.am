## Process this file with automake to produce Makefile.in
# gre/doc/tst_suite/Makefile.am
srcdir = @srcdir@
VPATH = @srcdir@
EXTRA_DIST = tst-variables.gre tst-variables2.gre tst-match.gre\
 tst-math.gre tst-string-comp.gre tst-substitute.gre\
 tst-random.gre tst-builtin.gre tst-sw.gre tst-if.gre\
 tst-while.gre tst-do-while.gre tst-for.gre tst-foreach.gre\
 tst-nested-cmd.gre tst-new-cmd.gre tst-die.gre tst-last.gre\
 tst-my.gre tst-print.gre tst-read.gre tst-sub.gre
DISTCLEANFILES = $(the_texi) $(the_html)
the_texi = tst-variables.texi tst-variables2.texi tst-match.texi\
 tst-math.texi tst-string-comp.texi tst-substitute.texi\
 tst-random.texi tst-builtin.texi tst-sw.texi tst-if.texi\
 tst-while.texi tst-do-while.texi tst-for.texi tst-foreach.texi\
 tst-nested-cmd.texi tst-new-cmd.texi tst-die.texi tst-last.texi\
 tst-my.texi tst-print.texi tst-read.texi tst-sub.texi
the_html = tst-variables.html tst-variables2.html tst-match.html\
 tst-math.html tst-string-comp.html tst-substitute.html\
 tst-random.html tst-builtin.html tst-sw.html tst-if.html\
 tst-while.html tst-do-while.html tst-for.html tst-foreach.html\
 tst-nested-cmd.html tst-new-cmd.html tst-die.html tst-last.html\
 tst-my.html tst-print.html tst-read.html tst-sub.html
%.texi : %.gre
	perl $(srcdir)/../gre2texi $< $@
%.html : %.gre
	perl $(srcdir)/../gre2html $< $@

clean-local:
	rm -f $(the_texi) $(the_html)
all-local: texi html
texi: $(the_texi)
html: $(the_html)

### 
GRE=gre
later-make-this-check-local:
	@$(GRE) tst-nested-cmd.gre
	@$(GRE) tst-sub.gre
	@$(GRE) tst-my.gre
	@$(GRE) tst-for.gre
	@$(GRE) tst-foreach.gre
	@$(GRE) tst-do-while.gre
	@$(GRE) tst-last.gre
	@$(GRE) tst-variables.gre
	@$(GRE) tst-variables2.gre
	@$(GRE) tst-builtin.gre
	@$(GRE) tst-match.gre
	@$(GRE) tst-math.gre
	@$(GRE) tst-if.gre
	@$(GRE) tst-read.gre
	@$(GRE) tst-new-cmd.gre
	@$(GRE) tst-while.gre
	@$(GRE) tst-string-comp.gre
	@$(GRE) tst-substitute.gre
