#!/usr/bin/perl
use Getopt::Std;

##############################################################################
#
# This program checks a file for uses of the #define preprocessing directive.
# It flags any use in which a list of parameters is provided to the macro
#
##############################################################################

sub usage 
{
    printf STDERR "$_[0]\n";
    printf STDERR "Usage: $0 [-h] [-i IGNORE_LIST] -f INFILE\n";
    printf STDERR "Options:\n";
    printf STDERR "  -h               Print this message\n";
    printf STDERR "  -i IGNORE_PREFIX Prefix for allowed macros\n";
    printf STDERR "  -f INFILE        Specify input file\n";
    die "\n" ;
}


# The following is a list of macros that are provided as part of the sample code.
# These are the only allowed ones
$ok_prefix = "dbg_";

getopts('hi:f:');
    
if ($opt_h) {
    usage($ARGV[0]);
}

$infile = STDIN;
$fwarning = "";
if ($opt_f) {
    open($infile, "<", $opt_f) || die "Couldn't open input file '$opt_f'\n";
    $fwarning = " File $opt_f.";
}

if ($opt_i) {
    $ok_prefix = $opt_i;
}

$lineno = 1;
$startlineno = 1;
$line = "";

while (<$infile>) {
    $nextline = $_;
    chomp($nextline);
    # See if this is line has a continuation
    if ($nextline =~ /^(.*)\\[\s]*$/) {
	$prefix = $1;
	$line = "$line$prefix";
    } else {
	$line = "$line$nextline";
	if ($line =~ /^[\s]*#[\s]*define[\s]+([\w]+)\(/) {
	    $macname = $1;
	    if ($lineno == $startlineno) {
		$linemsg = "Line $lineno contains";
	    } else {
		$linemsg = "Lines $startlineno-$lineno contain";
	    }
	    if ($macname !~ /^$ok_prefix/) {
		print "WARNING:$fwarning $linemsg disallowed definition of macro '$macname'\n";
	    }
	}
	$line = "";
	$startlineno = $lineno+1;
    }
    $lineno++;
}
