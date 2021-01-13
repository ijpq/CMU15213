#!/usr/bin/perl
use Getopt::Std;
use Digest::MD5 qw(md5 md5_hex md5_base64);


##############################################################################
#
# This program is used by Autolab to test a malloclab submission for
# correctness and performance.  It calls both mdriver and mdriver-emulate
# and combines their results into a single JSON string for Autolab
#
##############################################################################

sub usage
{
    printf STDERR "$_[0]\n";
    printf STDERR "Usage: $0 [-h] [-c] [-s SECONDS] -p PENALTY\n";
    printf STDERR "Options:\n";
    printf STDERR "  -h              Print this message\n";
    printf STDERR "  -c              Grade checkpoint\n";
    printf STDERR "  -s SECONDS      Set driver timeout\n";
    printf STDERR "  -p PENALTY      Penalty for failing mdriver-emulate\n";
    die "\n" ;
}

# Generic setting
$| = 1;      # Autoflush output on every print statement

# Settings
# Driver timeout
$timeout = 180;
# Penalty for emulation error
$penalty = 30;

getopts('hs:p:c');

if ($opt_h) {
    usage($ARGV[0]);
}

if ($opt_s) {
    $timeout = $opt_s;
}

if ($opt_p) {
    $penalty = $opt_p;
}

$calibration_flags = "";
$driver_flags = "";

$checkpoint = 0;

if ($opt_c) {
    $checkpoint = 1;
    $calibration_flags = "$calibration_flags -c";
    $driver_flags = "$driver_flags -p";
}


# Check Key.txt validity
open(IN,"<", "key.txt") || die "cannot open key.txt:$!";
my $andrew_id = ""; my $git_username = ""; my $submission_code = "";

while (<IN>) {
 if ($. == 1){$andrew_id .= $_;}
 if ($. == 2){$git_username .= $_;}
 if ($. == 3){$submission_code .= $_;}
}

chomp $andrew_id; chomp $git_username; chomp $submission_code;

if($andrew_id eq '<Enter your andrewid here>'){
    die "ERROR: key.txt does not contain your andrewid";
}
if($git_username eq '<Enter your git username here>'){
    die "ERROR: key.txt does not contain your git username";
}
if($submission_code eq '<Enter your unique code here>'){
    die "ERROR: key.txt does not contain your code";
}

# Run calibration program
$calibration_prog = "./calibrate.pl";

if (!-e $calibration_prog) {
    die "Cannot find calibration program $calibration_prog\n";
}

system("$calibration_prog $calibration_flags");


# Which field in the scoreboard string indicates the penalty
$pfield = 1;
# Which field in the scoreboard string indicates utilization
$ufield = 3;


$driver_prog = "mdriver";
$edriver_prog = "mdriver-emulate";

if (!-e $driver_prog) {
    die "Cannot find driver program '$driver_prog'\n";
}

if (!-e $edriver_prog) {
    die "Cannot find driver program '$edriver_prog'\n";
}

print "Running ./$driver_prog -s $timeout $driver_flags\n";

$dstring = `./$driver_prog -A -s $timeout $driver_flags 2>&1` ||
    die "Couldn't run driver $driver_prog\n";

$found_autograde = 0;
$prefix = "";
$score = 0;
$mid = "";
@fields = ();
$suffix = "";

# Extract the autograde JSON from the driver output
for $line (split "\n", $dstring) {
    if ($line =~ /Autograded Score/) {
        $found_autograde = 1;
        $line =~ /^([^0-9]*)([0-9\.]+)([^0-9]*)\[(.*)\](.*)$/;
        $prefix = $1;
        $score = $2;
        $mid = $3;
        $list = $4;
        @fields = split ",[\s]*", $list;
        $suffix = $5;
    } else {
        print "$line\n";
    }

}

if (!$found_autograde) {
    print "Driver $driver_prog failed.  Exiting\n";
    exit(0);
}

if ($score > 0) {
    print "Running ./$edriver_prog -s $timeout $driver_flags\n";

    $penalty_field = -$penalty;
    $persist_penalty = 0;

    $estring = "";

    $estring = `./$edriver_prog -A -s $timeout $driver_flags 2>&1` ||
        print "FAILED\n";

# Extract the autograde JSON from the driver output
    for $line (split "\n", $estring) {
        if ($line =~ /Autograded Score/) {
            $line =~ /^.*\[(.*)\].*$/;
            $list = $1;
            @efields = split ",[\s]*", $list;
            $eutil = $efields[$ufield];
            if ($eutil eq $fields[$ufield] && $eutil > 0) {
                # Driver output is OK.  Remove penalty
                $penalty_field = $persist_penalty;
            } else {
                $util = $fields[$ufield];
            }
        } elsif ($line =~ /error/) {
            $persist_penalty = $penalty_field;
            print "$line\n";
        }else {
            print "$line\n";
        }
    }

# Now replace the penalty field in the driver's output string
    $fields[$pfield] = $penalty_field;
    $score += $penalty_field;
    if ($score < 0) {
        $score = 0;
    }
}

$nlist = join(', ', @fields);
print $prefix . $score . $mid . "[$nlist]" . $suffix . "\n";
