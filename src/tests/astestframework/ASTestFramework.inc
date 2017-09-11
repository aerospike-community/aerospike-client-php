<?php

/**
 * These helper exceptions are thrown when a test declares it is to be skipped
 * or is incomplete
 */
class ASTestFramework_PrematureGracefulException extends Exception {}

class ASTestFramework_TestSkipException extends
    ASTestFramework_PrematureGracefulException { }

class ASTestFramework_TestIncompleteException extends
    ASTestFramework_PrematureGracefulException { }


/**
 * This is a simple API based on (and intended to be forward-compatible with)
 * PHPUnit. This includes minor parts from PHPUnit whose license file is included
 * within this directory.
 */
class ASTestFramework {

    static $verbose = false;

    protected function dieCommon($msg) {
        fprintf(STDERR, "Assertion failed.. dumping backtrace..\n");
        debug_print_backtrace();
        die("[ASTestFramework] $msg");
    }

    public function markTestIncomplete($comment = "") {
        throw new ASTestFramework_TestIncompleteException($comment);
    }

    public function markTestSkipped($comment = "") {
        throw new ASTestFramework_TestSkipException($comment);
    }

    public function assertInternalType($type, $obj, $msg = "") {

        $specialmap = array(
            "int" => "integer",
            "boolean" => "bool",
        );

        $tname = $type;

        if (array_key_exists($type, $specialmap)) {
            $tname = $specialmap[$type];
        }

        $funcname = "is_$tname";

        if (!function_exists($funcname)) {
            $this->dieCommon("No such type '$tname'");
        }

        if (!call_user_func($funcname, $obj)) {
            $this->dieCommon("Expected type '$type'");
        }

        return true;
    }

    public function assertNull($obj, $msg = "") {
        if (!$obj === NULL) {
            $this->dieCommon("Expected 'NULL'");
        }
        return true;
    }

    public function assertNotNull($obj, $msg = "") {
        if ($obj === NULL) {
            $this->dieCommon("Expected not-NULL");
        }
        return true;
    }

    public function assertContains($needle, $haystack, $msg = "") {
        if (!is_string($haystack)) {
            $this->dieCommon("assertContains supports only string searching");
        }

        if (strpos($haystack, $needle) === FALSE) {
            $this->dieCommon("Couldn't find '$needle'");
        }
        return true;
    }

    public function assertInstanceOf($cls, $obj, $msg = "") {
        if (!$obj instanceof $cls) {
            $this->dieCommon("Expected instance of $cls");
        }
        return true;
    }

    public function assertEquals($expected, $actual, $msg = "") {
        if ($expected !== $actual) {
            $this->dieCommon("Not equal!");
        }
        return true;
    }

    public function assertNotEquals($expected, $actual, $msg = "") {
        if ($expected === $actual) {
            $this->dieCommon("Expected not equals!");
        }
        return true;
    }

    public function assertTrue($obj, $msg = "") {
        if ($obj !== TRUE) {
            $this->dieCommon("Expected TRUE");
        }
        return true;
    }

    public function assertFalse($obj, $msg = "") {
        if ($obj !== FALSE) {
            $this->dieCommon("Expected FALSE");
        }
        return true;
    }

    public function assertNotEmpty($obj, $msg = "") {
        if (empty($obj)) {
            $this->dieCommon("Expected not-empty");
        }
        return true;
    }

    public function assertEmpty($obj, $msg = "") {
        if (!empty($obj)) {
            $this->dieCommon("Expected '$obj' to be empty!");
        }
        return true;
    }

    public function assertGreaterThan($gt, $val, $msg = "") {
        if (! ($val > $gt)) {
            $this->dieCommon("Expected value > $gt");
        }
        return true;
    }

    public function assertLessThan($lt, $val, $msg = "") {
        if (!($val < $lt)) {
            $this->dieCommon("Expected value < $lt");
        }
        return true;
    }

    public function assertRegExp($re, $str, $msg = "") {
        if (preg_match($re, $str) < 1) {
            $this->dieCommon("Couldn't match '$re'");
        }
        return true;
    }

    public function assertArrayHasKey($key, $array, $msg = "") {
        if (!array_key_exists($key, $array)) {
            $this->dieCommon("Couldn't assert presence of $key in array");
        }
        return true;
    }

    public function assertCount($exp, $array, $msg = "") {
        if (count(array_keys($array)) != $exp) {
            $this->dieCommon("Couldn't assert count == $exp");
        }
        return true;
    }
	public function assertTypeArray($array, $msg = "") {
        if (!is_array($array)) {
            $this->dieCommon("Not get Array");
        }else if(count($array)==0) {
			$this->dieCommon("No Data.");
		}
        return true;
    }

    public function runSingleTest($name) {
        $reflector = new ReflectionClass($this);
        $meth = $reflector->getMethod($name);
        $rethrow = false;

        if (self::$verbose) {
            fprintf(STDERR, "Running $name..\n");
        }

        $status = $this->setUp();
        if (!is_null($status) && $status !== Aerospike::OK) {
            // error during test set-up. cannot continue
            // to test
        } else {
            try {
                $status = $meth->invoke($this);
            } catch (ASTestFramework_PrematureGracefulException $exc) {
                if (self::$verbose) {
                    printf("\tSkipped.. " . $exc->getMessage());
                }
            } catch (Exception $e) {
                fprintf(STDERR, "=========\n[".$e->getCode()."] ".$e->getMessage()."\n=========\n");
                $rethrow = $e;
                $status = 201;
            }
        }

        $this->tearDown();

        if (self::$verbose) {
            fprintf(STDERR, "\tDone..\n");
        }

        if ($rethrow) {
            throw $rethrow;
        }
        return $status;
    }

    public function runAllTests() {
        $reflector = new ReflectionClass($this);
        $methods = $reflector->getMethods();
        foreach ($methods as $meth) {
            $name = $meth->getName();
            if (preg_match("/^test/", $name) ) {
                $this->runSingleTest($name);
            }
        }
    }

    public function log_error($message) {
        fprintf(STDERR, "$message\n");
    }

    // View tests use this
    static function setUpBeforeClass() {

    }
}

function astestframework_error_handler($errno,
                                      $errstr,
                                      $errfile,
                                      $errline,
                                      $errcontext) {

    throw new ErrorException($errstr, $errno, $errno, $errfile, $errline);
}

set_error_handler("astestframework_error_handler");

?>
