#!/usr/bin/perl

my $blanked = 0;
open (IN, "xscreensaver-command -watch |");
while (<IN>) {
    if (m/^(BLANK|LOCK)/) {
        if (!$blanked) {
            system "sydan_screensaver";
            $blanked = 1;
        }
    } elsif (m/^UNBLANK/) {
        system "killall sydan_screensaver";
        $blanked = 0;
    }
}
