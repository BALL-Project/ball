#! /usr/bin/perl -s

#zum rekursiven ergänzen aller tab-with angaben wird diese script so aufgerufen:
#find . -name "*.h" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv
#find . -name "*.C" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv
#find . -name "*.iC" | xargs -l1 filename.pl | xargs -l1 lines.pl | xargs -l2 mv

#filename.pl gibt den filename des resten Arguments aus, wenn "; tab-width: 2;" nicht drin ist
#lines.pl kopiert das erste Argument in eine Temporärdatei und schreibt die tab-width angaben davor. dann gibt es temporärdateiname und dateiname aus

#pruefen, ob richtige anzahl argumente
$args=@ARGV;

open(INFILE,"$ARGV[0]") or die "SOURCEFILE: $!";
$matches=0;

while (<INFILE>){
  if ($_ =~ m/; tab-width: 2;/ ){ 
		$matches=1;	
		}
}
close (INFILE);

if ($matches==0){
			print"$ARGV[0]\n";
			}



