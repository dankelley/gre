## Process this file with automake to produce Makefile.in
# gre/doc/figures/
     srcdir = @srcdir@
     VPATH = @srcdir@

# BUG: should have  TS-color  but I can't build gre right now to create the .ps
# BUG: should have  TU_defn   but I can't build gre right now to create the .ps

txt_files = clip_to_curve.txt TS.txt\
 diamonds.txt example1.txt hue_demo.txt logo.txt maritimes.txt\
 sound_speed.txt birthday.txt month-series.txt
html_files = clip_to_curve.html TS.html\
 diamonds.html example1.html hue_demo.html logo.html maritimes.html\
 sound_speed.html birthday.html month-series.html
eps_files = clip_to_curve.eps TS.eps\
 diamonds.eps example1.eps hue_demo.eps logo.eps maritimes.eps\
 sound_speed.eps birthday.eps month-series.eps
png_files = clip_to_curve.png TS.png\
 diamonds.png example1.png hue_demo.png logo.png maritimes.png\
 sound_speed.png birthday.png month-series.png
EXTRA_DIST = clip_to_curve.gre TS.gre\
 diamonds.gre example1.gre hue_demo.gre logo.gre maritimes.gre\
 sound_speed.gre birthday.gre month-series.gre\
 clip_to_curve.ps TS.ps\
 diamonds.ps example1.ps hue_demo.ps logo.ps maritimes.ps\
 sound_speed.ps birthday.ps month-series.ps
DISTCLEANFILES = $(png_files) $(eps_files) $(html_files) $(txt_files)
%.txt : %.gre
	echo "$<" | cat >$@
%.eps : %.ps
	cp $< $@
%.png : %.ps
	convert $< $@
%-tiny.png : %.png
	-convert -geometry 90x999 $< $@
%.pdf : %.ps
	ps2pdf $< $@
%.html : %.gre
	perl $(srcdir)/../gre2html $< $@
all: html png eps txt
html: $(html_files)
eps: $(eps_files)
png: $(png_files)
txt: $(txt_files)
