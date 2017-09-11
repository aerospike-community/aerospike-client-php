#!/usr/bin/perl
use strict;
use warnings;
use File::Path qw(rmtree mkpath);
use File::Basename qw(basename);

my $PHP_BIN = $ENV{PHP_EXE} || "php";
my $PHPUNIT_BIN = $ENV{PHPUNIT_EXE} || `which phpunit`;
chomp($PHPUNIT_BIN);

my $CBEXT_DIR = $ENV{EXTDIR} or die "EXTDIR must be set!";
my $SYS_EXTDIR = qx($PHP_BIN -r "echo ini_get('extension_dir');");
my $ext = $CBEXT_DIR . "/modules/aerospike.so";
my $tmp_extdir = "_extdir_tmp";

my $ini =  <<EOF;
extension_dir=$tmp_extdir;
extension=aerospike.so
EOF


rmtree($tmp_extdir);
mkpath($tmp_extdir);

my @orig_exts = glob("$SYS_EXTDIR/*");

foreach my $e (@orig_exts) {
    if ($e =~ /aerospike\.so$/) {
        next;
    }
    symlink($e, "$tmp_extdir/".basename($e));
}

symlink($ext, "$tmp_extdir/aerospike.so");

my $tmpini = "$tmp_extdir/ini_tmp";
open my $fh, ">", $tmpini or die "$tmpini: $!";

print $fh $ini;
print $fh "\n";
print $fh "report_memleaks=1\n";

close $fh;

$ENV{PHPRC} = $tmpini;
$ENV{PCBC_PHPUNIT} = 1;

my @cmd = ($PHP_BIN);

if ($PHPUNIT_BIN) {
    push @cmd, $PHPUNIT_BIN;
}

push @cmd, @ARGV;

print "@cmd\n";

if ($ENV{DEBUGGER}) {
    unshift @cmd, split(' ', $ENV{DEBUGGER});
}

exec @cmd;
