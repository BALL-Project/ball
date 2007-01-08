#! /usr/bin/perl -s

#zum rekursiven ergänzen aller tab-with angaben wird diese script so aufgerufen:
#find . -name "*.h" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv
#find . -name "*.C" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv
#find . -name "*.iC" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv

#filename.pl gibt den filename des resten Arguments aus, wenn "; tab-width: 2;" nicht drin ist
#lines.pl kopiert das erste Argument in eine Temporärdatei und schreibt die tab-width angaben davor. dann gibt es temporärdateiname und dateiname aus

open(INFILE,"$ARGV[0]") or die "SOURCEFILE: $!";
open(OUTFILE,">$ARGV[0].linesout") or die "DESTFILE: $!";

print OUTFILE "// -*- Mode: C++; tab-width: 2; -*-\n";
print OUTFILE "// vi: set ts=2:\n";
print OUTFILE "//\n";  


while (<INFILE>){
	print OUTFILE $_;}

close (INFILE);
close (OUTFILE);

print "$ARGV[0].linesout\n$ARGV[0]\n";
