#!/usr/bin/env php
<?php

$OUTPUT = "phpt";

$lines = file("TEST_CLASSES");

class CBTestFramework { }

system("rm -rf $OUTPUT");
mkdir($OUTPUT);

print "Building phpt's for:\n";

foreach ($lines as $line) {
    $line = chop($line);
    require_once($line . ".inc");
    $clsname = $line;
    $reflector = new ReflectionClass($clsname);
    $methods = $reflector->getMethods();

    $clspath = "$OUTPUT/$clsname";
    mkdir($clspath);

    foreach ($methods as $meth) {
        $name = $meth->getName();
        $matches = array();
        if (! preg_match("/^test(.+)/", $name, $matches)) {
            continue;
        }

        $test_base = $matches[1];

        $fname = "$clspath/$test_base.phpt";
        $fp = fopen($fname, "w");

        $inc_line = 'include dirname(__FILE__).';
        $inc_line .= '"/../../cbtestframework/cbtest-phpt-loader.inc";';

        // Boilerplate:
        fprintf($fp, <<<EOF
--TEST--
$clsname - $test_base

--SKIPIF--
<?php
$inc_line
aerospike_phpt_skipif("$clsname", "$name");

--FILE--
<?php
$inc_line
aerospike_phpt_runtest("$clsname", "$name");
--EXPECT--
AEROSPIKE__OK
EOF
);
	}
	print "  * $line\n";
}

print "done\n";
