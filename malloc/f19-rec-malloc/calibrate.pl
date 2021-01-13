#!/usr/bin/perl
use Getopt::Std;

# Calibrate throughput for malloc benchmarks depending on CPU type

sub usage
{
    printf STDERR "$_[0]\n";
    printf STDERR "Usage: $0 [-h] [-v] [-c] [-N REPS] [-e TOL] [-S] [-f]\n";
    printf STDERR "Options:\n";
    printf STDERR "   -h              Print this message\n";
    printf STDERR "   -v              Verbose mode\n";
    printf STDERR "   -c              Measure checkpoint solution\n";
    printf STDERR "   -N REPS         Run benchmark REPS times\n";
    printf STDERR "   -e TOL          Set tolerance for outlier detection\n";
    printf STDERR "   -S              Do NOT save results\n";
    printf STDERR "   -f              Prevent looking up of previous result\n";
    die "\n";
}

$| = 1;       # Autoflush output on every print statement

getopts('hvcN:e:Sf');

if ($opt_h) {
    &usage($ARGV[0]);
}

$verbose = 0;
if ($opt_v) {
    $verbose = 1;
}

# Parameters
# Where to get CPU information
$cpu_file = "/proc/cpuinfo";
$cpu_key = "model name";
# CPU info (extracted from file)
$cpu_info = "";

# Number of measurements
$N = 10;
if ($opt_N) {
    $N = $opt_N;
}

# Outlier deletion criterion
$outlier_threshold = 0.01;

if ($opt_e) {
    $outlier_threshold = $opt_e;
    if ($outlier_threshold >= 0.5) {
        die "Tolerance should be relative to 1.0, not 100\n";
    }
}

# Is this the regular or the checkpoint?
$is_checkpoint = 0;
if ($opt_c) {
    $is_checkpoint = 1;
}

# Regular benchmark program
$benchprog = "./mdriver-ref";
# Checkpoint benchmark program
$benchprog_checkpoint = "./mdriver-cp-ref";

$bench = "regular";

if ($is_checkpoint) {
    $benchprog = $benchprog_checkpoint;
    $bench = "checkpoint";
}

$save_results = 1;
if ($opt_S) {
    $save_results = 0;
}
$save_file = "./throughputs.txt";

$lookup_ok = 1;
if ($opt_f) {
    $lookup_ok = 0;
}


# Get the CPU info
$cmultistring = `grep '$cpu_key' $cpu_file` ||
    die "Couldn't get CPU information (key = '$cpu_key') from '$cpu_file'\n";

# Remove first entry
@cstrings = split "\n", $cmultistring;
$cstring = $cstrings[0];

# Remove all white space
$cstring =~ s/\s//g;

# Get data
@parts = split ":", $cstring;

$cpu_info = $parts[1];

if ($verbose > 0) {
    print "Got CPU info '$cpu_info'\n";
}

# See if can find data
sub read_throughput_file {
    my ($save_file) = @_;
    if (-e $save_file) {
        my $rstring = `cat $save_file`;
        my @entries = split "\n", $rstring;
        for my $e (@entries) {
            my ($fcpu_info, $fbench, $favg) = split ":", $e;
            if ($fcpu_info eq $cpu_info && $fbench eq $bench) {
                return $favg;
            }
        }
    }
    return 0.0;
}

if ($lookup_ok == 1) {
    $avg = read_throughput_file($save_file);
    if ($avg > 0.0) {
        print "Calibration: CPU type $cpu_info, benchmark $bench, throughput $avg\n";
        exit(0);
    }
}

# Run measurements
@values = ();

for (my $i = 0; $i < $N; $i += 1) {
    my $sval = `$benchprog` || die "Couldn't run $benchprog\n";
    chomp($sval);

    $val = $sval * 1.0;
    push (@values, $val);

    if ($verbose > 0) {
        print "$i\t$val\n";
    }
}

# Compute average of array.  Ignore elements with value 0.0
sub array_avg
{
    my $n = 0;
    my $sum = 0.0;
    for my $v (@_) {
        if ($v > 0) {
            $sum += $v;
            $n += 1;
        }
    }
    my $avg = $sum/$n;
    if ($verbose > 0) {
        print "Average $avg over $n elements\n";
    }
    sprintf("%.0f", $avg);
}


# Compute average, while removing outliers from array
sub avg_without_outliers
{
    my @values = @_;
    my $done = 0;
    my $avg = 0.0;
    my $curN = $N;

    while (!$done) {
        $avg = &array_avg(@values);
        $done = 1;
        my $idx = 0;
        for my $v (@values) {
            if ($v > 0 && $v < (1.0 - $outlier_threshold) * $avg) {
                if ($verbose > 0) {
                    print "Deleting\t$idx\t$v\n";
                }
                $values[$idx] = 0.0;
                if ($curN > $N/2) {
                    $curN -= 1;
                    $done = 0;
                }
            }
            $idx += 1;
        }
    }

    return $avg;
}


# Save output to text file
sub save_file_output
{
    my ($save_file, $avg) = @_;
    my @entries = ();

    if (-e $save_file) {
        my $rstring = `cat $save_file`;
        @entries = split "\n", $rstring;
        my $idx = 0;
        my $found = 0;
        for my $e (@entries) {
            my ($fcpu_info, $fbench, $favg) = split ":", $e;
            if ($fcpu_info eq $cpu_info && $fbench eq $bench) {
                if ($verbose > 0) {
                    print "CPU:$cpu_info, benchmark:$bench.  Replacing average $favg with $avg\n";
                }
                $entries[$idx] = "$cpu_info:$bench:$avg";
                $found = 1;
            }
            $idx += 1;
        }
        if ($found == 0) {
            $entries[$idx] = "$cpu_info:$bench:$avg";
            if ($verbose > 0) {
                print "Appending entry $idx: $cpu_info:$bench:$avg\n";
            }
        }
    } else {
        @entries = ("$cpu_info:$bench:$avg");
        if ($verbose > 0) {
            print "Created first entry: $cpu_info:$bench:$avg\n";
        }
    }

    open OUT, ">$save_file";
    for my $e (@entries) {
        print OUT "$e\n";
    }
}


# Now start working on the result
$avg = &avg_without_outliers(@values);

# Save output to file
if ($save_results == 1) {
    &save_file_output($save_file, $avg);
}

print "Calibration: CPU type $cpu_info, benchmark $bench, throughput $avg\n";

exit(0);
