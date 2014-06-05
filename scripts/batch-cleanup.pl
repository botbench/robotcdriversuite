#!/usr/bin/perl -w
use strict;
use warnings;

my $inputFile = $ARGV[0]; #read first argument from the commandline as fileName
print "$inputFile\n";
open INPUTFILE, "+<", $inputFile or die $!;    #Open File

my $data = do { local $/ = undef; <INPUTFILE> };
close (INPUTFILE);

# strip the dos end of line character
$data =~ s///sg;

# strip the $Id tags that span multiple lines
$data =~ s/\/[^\/]+\$Id:[^\/]+\///sg;

# Get rid of multiple empty lines
$data =~ s/\n[\s]*\n[\s]*\n/\n\n/sg;

# Remove the empty line at the end of the file
$data =~ s/\n[\s]*\n$/\n/sg;

# Replace the tabs with two spaces
$data =~ s/	/  /sg;

# Add the DOS end of line character again
$data =~ s/\n/\n/sg;

my $outputFile = $inputFile;
open OUTPUTFILE, ">", $outputFile or die $!;    #Open File
print OUTPUTFILE $data;
close (OUTPUTFILE);
