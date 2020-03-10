<?php

require_once("Common.inc");
require_once(realpath(__DIR__ . '/../../autoload.php'));
class Scan extends AerospikeTestCommon
{
    static $put_ldt_rec = array("LDTCONTROLBIN" => NULL, "triangular"=>array(19));
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key, array("email"=>"john"));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key1");
        $this->db->put($key, array("email"=>"smith"));
        $this->keys[] = $key;
    }

    /**
     * @test
     * Basic SCAN without parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testScanNoParameter()
    {
        try {
            $this->db->scan();
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic SCAN Check set pnd ns parameter value non exist in database
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckSetAndNsValueNotExistInDB()
    {
        $processed = 0;
        return($this->db->scan("r1", "rr", function ($record) use (&$processed) {
            if (array_key_exists("email", $record["bins"]) &&
                !is_null($record["bins"]["email"]))
                echo $record["bins"]["email"]."\n";
            if ($processed++ > 19) return false;
        }, array("email")));
    }
    /**
     * @test
     * Basic SCAN Check set pnd ns parameter value empty string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckSetAndNsValueNull()
    {
        $processed = 0;
        try {
            return($this->db->scan("", "", function ($record) use (&$processed) {
            if (array_key_exists("email", $record["bins"]) &&
                !is_null($record["bins"]["email"])) {
                    echo $record["bins"]["email"]."\n";
                }
            if ($processed++ > 19) {
                return false;
            }
            }, array("email")));
        } catch(Exception $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Scan with correct arguments and set NULL
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testScanWithCorrectArgumentsAndSetNull)
     *
     * @test_plans{1.1}
     */
    function testScanWithCorrectArgumentsAndSetNull()
    {
        $processed = 0;
        $status = $this->db->scan( "test", null, function ($record) use (&$processed, &$db) {
            $processed++;
        }, array("email"), array(Aerospike::OPT_READ_TIMEOUT=>0, Aerospike::OPT_SCAN_RPS_LIMIT=>5));
        if ($status !== Aerospike::OK || $processed < 1) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic SCAN Check incorrect name of bins
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckIncorrectNameOfBins()
    {
        $processed = 0;
        $db= $this->db;
        $this->db->scan("test", "demo", function ($record) use (&$processed, &$db) {
            if (array_key_exists("account", $record["bins"]) &&
                !is_null($record["bins"]["account"])) {
                    echo $record["bins"]["account"]."\n";
                }
            else {
                return Aerospike::ERR_CLIENT;
            }
            $key = $db->initKey("test", "demo", "testing");
            $db->put($key, array("bin1"=>10));
            if ($processed++ > 19) {
                return false;
            }
        }, array("account"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(array_key_exists('bins',$record)) {
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic SCAN Check wrong bin name in callback
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckIncorrectBinNameInCallback()
    {
        $processed = 0;
        $db= $this->db;
        $this->db->scan("test", "demo", function ($record) use (&$processed, &$db) {
            if (array_key_exists("account", $record["bins"]) &&
                !is_null($record["bins"]["account"]))
                echo $record["bins"]["email"]."\n";
            if (array_key_exists("email", $record["bins"]) && ((!strcmp($record["bins"]["email"], "smith")) ||
                (!strcmp($record["bins"]["email"], "john")))) {
                    $processed++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
            if ($processed == 2) {
                $key = $db->initKey("test", "demo", "testing");
                $db->put($key, array("bin1"=>10));
                return Aerospike::OK;
            } else if($processed > 2) {
                return Aerospike::ERR_CLIENT;
            } 
        }, array("email"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic SCAN Check for empty bin value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForEmptyBinValue()
    {
        $processed = 0;
        try {
            $status =  $this->db->scan("test", "demo", function ($record) use (&$processed) {
                if (array_key_exists("email", $record["bins"]) &&
                    !is_null($record["bins"]["email"])) {
                        echo $record["bins"]["email"]."\n";
                    }
                if ($processed++ > 19) {
                    return false;
                }
            }, "");
        } catch (exception $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic SCAN Check incorrect value for options
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckIncorrectValueForOptions()
    {
        $status = $this->db->scan( "test", "demo", function ($record) use (&$processed) {
            if (array_key_exists("account", $record["bins"]) &&
                !is_null($record["bins"]["account"])) {
                    echo $record["bins"]["email"]."\n";
                }
            if ($processed++ > 19) {
                return false;
            }
        }, array("email"), array(Aerospike::OPT_SCAN_NOBINS=>false, Aerospike::OPT_READ_TIMEOUT=>"pqr"));
        if ($status !== aerospike::OK) {
            return $this->db->errorno();
        }
        return aerospike::OK;
    }
    /**
     * @test
     * Basic SCAN Check for callback parameter missing
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForCallbackParameterMissing()
    {
        try {
            $this->db->scan( "test", "demo", array("email"));
        } catch(ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic SCAN Check with correct arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForCorrectArguments()
    {
        $processed = 0;
        $db= $this->db;
        $status = $this->db->scan( "test", "demo", function ($record) use (&$processed, &$db) {
            if ((strcmp("test", $record["key"]["ns"]) == 0) && (strcmp("demo",
                $record["key"]["set"]) == 0)) {
                    if ($processed < 2) {
                        if (array_key_exists("email", $record["bins"]) &&
                            !is_null($record["bins"]["email"])) {
                                if ((!strcmp($record["bins"]["email"], "smith")) ||
                                    (!strcmp($record["bins"]["email"], "john"))) {
                                        $processed++;
                                    }
                            }
                    }
                    if ($processed == 2) {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                } else {
                    return AEROSPIKE::ERR_CLIENT;
                }
        }, array("email"), array(Aerospike::OPT_READ_TIMEOUT=>2000,Aerospike::OPT_SCAN_RPS_LIMIT=>3700));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Scan - positive with all arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregatePositive)
     *
     * @test_plans{1.1}
     */
    function testScanPositiveWithAllArguments()
    {
        $processed = 0;
        $db = $this->db;
        $status = $this->db->scan( "test", "demo", function ($record) use (&$processed, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if ($processed < 2) {
                        if (array_key_exists("email", $record["bins"]) &&
                            !is_null($record["bins"]["email"])) {
                            if ((!strcmp($record["bins"]["email"], "smith")) ||
                                (!strcmp($record["bins"]["email"], "john"))) {
                                    $processed++;
                                }
                        }
                    }
                    if ($processed == 2) {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                } else {
                    return AEROSPIKE::ERR_CLIENT;
                }
        }, array("email"), array(Aerospike::OPT_READ_TIMEOUT=>2000, Aerospike::OPT_SCAN_CONCURRENTLY=>true));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic SCAN Check without optional arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForCorrectArgumentsWithoutOptionals()
    {
        $processed = 0;
        $db= $this->db;
        $status = $this->db->scan( "test", "demo", function ($record) use (&$processed, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if ($processed < 2) {
                        if (array_key_exists("email", $record["bins"]) &&
                            !is_null($record["bins"]["email"])) {
                            if ((!strcmp($record["bins"]["email"], "smith")) ||
                                (!strcmp($record["bins"]["email"], "john"))) {
                                    $processed++;
                                }
                        }
                    }
                    if ($processed == 2) {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                } else {
                    return AEROSPIKE::ERR_CLIENT;
                }
        });
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic scan check with NO_BINS scan option.
     *
     * @pre
     * Connect using aerospike object to the specified node.
     *
     * @post
     * Newly initialized Aerospike objects.
     *
     * @remark
     * Variants: OO (testScanWithNoBinsArgument).
     *
     * @test_plans{1.1}
     */
    function testScanWithNoBinsArgument()
    {
        $failed = FALSE;
        $callback = function ($record) use (&$failed) {
            if($record["bins"] != array()) {
                $failed = TRUE;
            }
        };
        $status = $this->db->scan("test", "demo", $callback,
            array( "email", "account"), array(Aerospike::OPT_READ_TIMEOUT=>2000, Aerospike::OPT_SCAN_NOBINS=>TRUE));
        if($failed) {
            $this->fail("Record returned bins.");
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic SCAN Check with correct arguments and one optional argument
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForOneOptionalArgument()
    {
        $processed = 0;
        $db= $this->db;
        $status = $this->db->scan( "test", "demo", function ($record) use (&$processed, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if ($processed < 2) {
                        if (array_key_exists("email", $record["bins"]) &&
                            !is_null($record["bins"]["email"])) {
                            if ((!strcmp($record["bins"]["email"], "smith")) ||
                                (!strcmp($record["bins"]["email"], "john"))) {
                                    $processed++;
                                }
                        }
                    }
                    if ($processed == 2) {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                } else {
                    return AEROSPIKE::ERR_CLIENT;
                }
        }, array("email"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic SCAN Check with correct arguments and empty callback function
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetValueNotExistInDB)
     *
     * @test_plans{1.1}
     */
    function testCheckForEmptyCallbackFunction()
    {
        $status = $this->db->scan("test", "demo", function ($record) {},
         array("email"), array(Aerospike::OPT_SCAN_PRIORITY=>Aerospike::SCAN_PRIORITY_AUTO,
            Aerospike::OPT_READ_TIMEOUT=>2000));
        if ($status !== AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

}
?>
