$res = @t=(11,"hello",33);
die "Expected 3" if $res ne 3;
die "Expected 11" if $t[0] ne 11;
die "Expected hello" if $t[1] ne "hello";
die "Expected 33" if $t[2] ne 33;
print "ok";

