Summary: a language for scientific illustration
%define greversion 0.2.3
Name: gre
Version: %{greversion}
Release: 1
Copyright: distributable
Group: Applications/Engineering
Source: ftp://ftp.phys.ocean.dal.ca/users/kelley/gre/gre-%{greversion}.tgz
URL: http://www.phys.ocean.dal.ca/~kelley/gre/index.html
Vendor: Tantramar Software
Packager: Dan E. Kelley <Dan.Kelley@Dal.CA>
BuildRoot: /var/tmp/%{name}-root

%description
Gre is a programming language for scientific graphics.  It can make
x-y graphs, contour-graphs, and image graphs.  Gre has a full suite of
low-level graphical operations and high-level programming capabilities
(patterned on the Perl language) to permit complex customization.

Gre is not point-click.  In some ways it is analogous to TeX:
extensive power rewards tolerance of a modest learning curve.

%prep
%setup -q

%build
%configure
make libdir=/usr/share/gre gre
strip gre

%install
# remove docinst if it is there from a previous build
rm -rf docinst
mkdir docinst
mkdir docinst/html
(cd doc ; make linux_redhat HTML_DIR=../docinst/html)
#cp -rdp doc TMP && mkdir -p docinst && mv -f TMP docinst/html
##(cd docinst/html/          ; rm -rf *[!.html]; rm -f Makefile) || :
##(cd docinst/html/resources ; rm -rf *{.txt,.eps,.ps} Makefile) || :
##(cd docinst/html/examples  ; rm -rf *{.txt,.eps,.ps} Makefile) || :
make DESTDIR=$RPM_BUILD_ROOT install_linux_redhat

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README copyright.txt
%doc docinst/html
/usr/bin/gre
/usr/share/gre
/usr/share/man/man1/gre.*
/usr/share/info/gre.info*
/usr/share/emacs/site-lisp/gre-mode.el

%post
if [ "$1" = 1 ]
then
	if ! grep -i " GRE:" /usr/share/info/dir > /dev/null
	then
                /sbin/install-info --dir-file="/usr/share/info/dir" /usr/share/info/gre.info.gz
	fi
fi

%postun
if [ "$1" = 0 ];
then
	if grep -i " GRE:" /usr/share/info/dir > /dev/null
	then
                /sbin/install-info --dir-file="/usr/share/info/dir" --remove /usr/share/info/gre.info.gz
		chmod a+r /usr/share/info/dir
	fi
fi

%changelog
* Fri Feb 28 2003 Dan Kelley <dan.kelley@dal.ca>
- 0.2.3 change things (gri.spec, Makefiles) so it will rpmbuild
* Mon May  8 2000 Dan Kelley <dan.kelley@dal.ca>
- Changed the files part, so it only specifies gre.1, not e.g. grep,
  in /usr/man/man1.
* Tue Apr 19 2000 Dan Kelley <dan.kelley@dal.ca>
- Changed this spec-file be more like the Gri spec-file, mainly
  by putting things in /usr instead of /opt.  A few changes
  were required, e.g. deleting the %configure line since it
  caused the compiler to use flags that make the EGCS compiler
  generate so-called "internal compiler errors" whilst 
  compiling the set.cpp file.  I am not sure exactly what the 
  flags were that caused the problems, but I do know that "-O2" and
  "-m486", which are result from the presence of the  %configure
  line in the spec file, are not what I have used successfully 
  in the past.

