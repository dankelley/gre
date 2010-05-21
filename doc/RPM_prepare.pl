#!/usr/bin/perl -w

print "#\n";
print "# The following list of html sources is prepared by\n";
print "# the gre/src/doc/RPM_prepare.pl script.\n";
print "# IMPORTANT: must delete duplicates from this program:\n";
print "#   RPM_prepare.pl | sort | uniq\n";

$dir = "/opt/gre/doc/html";
open(IN, "gre.texim") or die "cannot open gre.texim";
print "$dir/gre1.html\n";
print "$dir/index.html\n";
while(<IN>) {
    if (/^\@c .* newfile ([^ ]*)/) {
	print "$dir/$1\n";
    }
}
close(IN);
open(IN, "ls -c1 figures/*Makefile figures/*html figures/*gif figures/*gre figures/*dat |") or die "cannot open examples";
while(<IN>) {
    print "$dir/$_";
}
close(IN);
open(IN, "ls -c1 resources/*gif |") or die "cannot open resources";
while(<IN>) {
    print "$dir/$_";
}

$dir = "/opt/gre/tst_suite";
open(IN, "ls -c1 ../tst_suite/Makefile ../tst_suite/*.html ../tst_suite/*.gre ../tst_suite/*.dat |") or die "cannot open ../tst_suite/...";
while(<IN>) {
    s:.*/::;
    print "$dir/$_";
}
