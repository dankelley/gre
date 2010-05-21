$file = "/home/kelley/mail/NET";
open (IN, $file) or die "Cannot open $file";
$i = 0;
while(<IN>) {
    chop;
    # Date: Sat, 17 Jul 1999 15:01:20 -0300
    if (/Date:(.*)/) {			# a date
	@f = split;
	$day = $f[2];
	$hour = $f[5];
	$hour =~ s/:.*//;
	$time = $day + $hour / 24; 
    }
    next if !/Gateway.Phys.OCEAN/;
    print "'$_' -> \n";
    s/[.a-zA-Z0-9() ]//g;
    print "  -> '$_'\n";
    $misses = length($_);
    $percent = 100 * $misses / 3;
    $_x[$i] = $time;
    $_y[$i] = $percent;
    $i++;
}
draw curve;

