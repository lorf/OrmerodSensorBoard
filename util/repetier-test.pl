#!/usr/bin/perl

use strict;
use warnings;
use Getopt::Std;
use Device::SerialPort;

sub usage {
    print "$0 [-b baudrate] serial_port [repetitions]\n";
    exit 0;
}

getopts('-b:', \my %opts);

my $portname = shift || &usage;
my $repetitions = shift || 16;
my $baudrate = $opts{b} || 115200;

tie *PO, 'Device::Serialport'
my $po = new Device::SerialPort $portname
    or die "Can't open $portname: $!\n";
$po->baudrate($baudrate);
$po->databits(8);
$po->parity("none");
$po->stopbits(1);
$po->handshake("none");
$po->write_settings
    or die "Can' set port parameters: $!\n";

my @values;
for (1..$repetitions) {
    $po->write("G30\n");
    my $buffer = "";
    do {
        my ($nread, $data) = $po->read(255);
        $buffer .= $data;
        if ($buffer =~ /^Z-probe:([\d\.]+)\s/ms) {
            push @values, $1;
            last;
        }
    } while(1);
}

$po->close;
undef $po
untie *PO;

my ($min, $max, $sum, $mean, $stddev) = (0, 0, 0, 0, 0);
map { $min and $min < $_ or $min = $_ } @values;
map { $max and $max > $_ or $max = $_ } @values;
map { $sum += $_ } @values;
$mean = $sum / scalar @values;
my $variance = 0;
map { $variance += ($_ - $mean)**2 } @values;
$variance /= scalar @values;
$stddev = sqrt($variance);

printf "Num Values: %d, Min: %.02f, Max: %.02f, Mean: %.04f, Stddev: %.04f\n",
    scalar @values, $min, $max, $mean, $stddev;

$" = ", ";
print "Values: @values\n";

$" = ",";
print "http://www.wolframalpha.com/input/?i={@values}\n";
