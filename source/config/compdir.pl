#!/usr/local/bin/perl
use Cwd;

sub allfiles{
	@allfiles = ();
	
	$olddir = cwd();
	chdir $_[0];
	@dirs = (".");
	while ($#dirs > -1){
		DIR: foreach $d (@dirs){
			opendir THISDIR, $d;
			@dirfiles = grep !/^\.\.?$/, readdir THISDIR;
			closedir THISDIR;
			FILE: foreach $f (@dirfiles){
				@allfiles = (@allfiles, "$d/$f");
				next FILE unless -d "$d/$f";
				@newdirs = (@newdirs, "$d/$f");	
			}
		}
		@dirs = (@newdirs);
		@newdirs = ();
	}
	chdir $olddir;
	return @allfiles;
}

print "$#ARGV\n";
if ($#ARGV != 1) {
	print "usage: compdir.pl <dir1> <dir2>\n";
	print "       will create a list of changes neccessary to change <dir1> into <dir2>.";
	exit;
}

print "scanning files in $ARGV[0]...\n";
@filesA = allfiles($ARGV[0]);
print "scanning files in $ARGV[1]...\n";
@filesB = allfiles($ARGV[1]);
print "comparing trees...\n";

@in_A_not_B = ();
@in_B_not_A = ();
@in_A_and_B = ();

%filehash = ();
foreach $file (@filesB){
	$filehash{$file} = $file;
}

while ($#filesA != -1){
	$file = pop(@filesA);
	if (defined $filehash{$file}) {
		delete $filehash{$file};
		@in_A_and_B = (@in_A_and_B, $file);
	} else {
		@in_A_not_B = (@in_A_not_B, $file);
	}
}

foreach $key (keys %filehash){
	@in_B_not_A = (@in_B_not_A, $key);
}

print "Add the following files to $ARGV[0]:\n";
foreach $file (@in_B_not_A){
	print "  $file\n";
}
print "Add the following files to $ARGV[1]:\n";
foreach $file (@in_A_not_B){
	print "  $file\n";
}
