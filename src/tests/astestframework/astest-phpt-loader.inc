<?php

require_once 'ASTestFramework.inc';
require_once dirname(__FILE__).'/../Common.inc';
define('Undefined_Error', 2001);

// Mapping the Aerospike error codes to their string names
$ref_class = new ReflectionClass('Aerospike');
$status_codes = array(Aerospike::OK => 'OK', Undefined_Error=>"Undefined_Error");
foreach ($ref_class->getConstants() as $class_constant => $const_val) {
    if (substr($class_constant, 0, 3) == 'ERR') {
        $status_codes[$const_val] = $class_constant;
    }
}

function aerospike_phpt_runtest($clsname, $name) {
    require_once(dirname(__FILE__)."/../$clsname.inc");
    $reflector = new ReflectionClass($clsname);

    if ($reflector->hasMethod("setUpBeforeClass")) {
        $reflector->getMethod("setUpBeforeClass")->invoke(NULL);
    }

    $obj = $reflector->newInstance();

    # If any assertions fail here, PHPUnitCompat will fail the test
    # and complain.
    $rv= $obj->runSingleTest($name);
    global $status_codes;
    printf("%s", ($status_codes[$rv]));
}

function aerospike_phpt_skipif($clsname, $name) {
    require_once(dirname(__FILE__)."/../$clsname.inc");
    $reflector = new ReflectionClass($clsname);
    $obj = $reflector->newInstance();
    $skip_cls = "skipALL";
    $matchlist = array();

    $bname = preg_match("/^test(.+)/", $name, $matchlist);
    $skip_meth = "skip" . $matchlist[1];

    $is_skipped;

    $meths = array($skip_cls, $skip_meth);

    foreach ($meths as $meth) {
        if ($reflector->hasMethod($meth)) {
            try {
                $is_skipped = $obj->$meth();
            } catch (CBTestFramework_TestSkipException $ex) {
                die("skip " . $ex->getMessage());
            }
        }
    }
}
