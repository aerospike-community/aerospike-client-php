<?php

require_once 'Common.inc';
/**
 *Basic Query tests
 */

class Query extends AerospikeTestCommon
{

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }

        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key,array("email"=>"john", "age"=>29));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key1");
        $this->db->put($key,array("email"=>"smith", "age"=>27));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key2");
        $this->db->put($key,array("email"=>"adam", "age"=>22));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key3");
        $this->db->put($key,array("email"=>"ellie", "age"=>32));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'age', 'demo_age_idx', Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);

        #INDEX on list.
        $key = $this->db->initKey("test", "demo", "index_key1");
        $this->db->put($key,array("name"=>"Dan", "fav_movies"=>array("a", "b", "c")));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key2");
        $this->db->put($key,array("name" => "jennifer", "fav_movies"=>array("x", "a", "b")));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'fav_movies', 'index_list_str', Aerospike::INDEX_TYPE_LIST, Aerospike::INDEX_STRING);

        $key = $this->db->initKey("test", "demo", "index_key3");
        $this->db->put($key,array("numbers"=>array(1, 2, 3, 4, 5)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key4");
        $this->db->put($key,array("numbers"=>array(2, 3, 5)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key44");
        $this->db->put($key,array("numbers"=>array(2, 13, 85)));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'numbers', 'index_list_numeric', Aerospike::INDEX_TYPE_LIST, Aerospike::INDEX_NUMERIC);

        #INDEX map(keys & values)
        $key = $this->db->initKey("test", "demo", "index_key5");
        $this->db->put($key,array("movies_viewed"=>array("aa" => 1, "bb" => 2)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key6");
        $this->db->put($key,array("movies_viewed"=>array("aa" => 5, "zz" => 1)));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'movies_viewed', 'index_mapkeys_str', Aerospike::INDEX_TYPE_MAPKEYS, Aerospike::INDEX_STRING);

        $key = $this->db->initKey("test", "demo", "index_key7");
        $this->db->put($key,array("map_numeric"=>array(20 => 1, 10 => 2)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key77");
        $this->db->put($key,array("map_numeric"=>array(10 => 256)));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'map_numeric', 'index_mapkeys_numeric', Aerospike::INDEX_TYPE_MAPKEYS, Aerospike::INDEX_NUMERIC);

        $key = $this->db->initKey("test", "demo", "index_key8");
        $this->db->put($key,array("mapval_num"=>array("a" => 11, "b" => 22)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "index_key9");
        $this->db->put($key,array("mapval_num"=>array("a" => 15, "z" => 100)));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'mapval_num', 'index_mapval_numeric', Aerospike::INDEX_TYPE_MAPVALUES,
            Aerospike::INDEX_NUMERIC);

        $key = $this->db->initKey("test", "demo", "index_key10");
        $this->db->put($key,array("mapval_str"=>array("aa" => "x", "vv" => "g")));
        $this->keys[] = $key;

        $this->ensureIndex('test', 'demo', 'mapval_str', 'index_mapval_string', Aerospike::INDEX_TYPE_MAPVALUES,
            Aerospike::INDEX_STRING);

        $this->ensureIndex('test', NULL, 'age', 'test_null_age_idx', Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);
    }

    /**
     * @test
     * Basic Query without parameter
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
    function testQueryNoParameter()
    {
        try {
            $this->db->query();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Query with incorrect ns and set
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithIncorrectNsSet()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties) {
            if (array_key_exists("email", $record["bins"]) &&
                !is_null($record["bins"]["email"])) {
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
                }
        };
        $status = $this->db->query("t", "d", $where, $callback,
            array("email","age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Query with empty ns and set
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithEmptyNsSet()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties) {
            if (array_key_exists("email", $record["bins"]) &&
                !is_null($record["bins"]["email"])) {
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
            }
        };
        $status = $this->db->query("", "", $where,
            $callback, array("email","age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Query with incorrect name of bins
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithIncorrectNameOfBins()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $in_thirties = 0;
        $db = $this->db;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            $key = $db->initKey("test", "demo", "testing");
            $db->put($key, array("bin1"=>10));
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        } else {
                            return Aerospike::ERR_CLIENT;
                       }
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        };
        $status = $this->db->query("test", "demo", $where,
            $callback, array("first_name", "last_name"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if (array_key_exists('bins',$record)) {
            if(10 == $record['bins']['bin1']) {
                $this->db->remove($key);
                return Aerospike::OK;
            }
        } else {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with wrong bin name in callback
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithWrongBinNameInCallback()
    {
        $total = 0;
        $in_thirties = 0;
        $db= $this->db;
        $where = $this->db->predicateBetween("age", 30, 39);
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("wrong_bin", $record["bins"]) &&
                        (!is_null($record["bins"]["wrong_bin"]))) {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        $total += (int) $record["bins"]["age"];
                        $in_thirties++;
                    }
                }
        };
        $status = $this->db->query( "test", "demo", $where,
            $callback, array("email","age"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(array_key_exists('bins',$record)) {
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        } else {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Query with wrong bin name in where
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithWrongBinNameInWhere()
    {
        $total = 0;
        $db= $this->db;
        $where = $this->db->predicateBetween("agesdfasdfas", 30, 39);
        $callback = function ($record) use (&$total, &$db) {
            $total++;
        };
        $status=$this->db->query("test", "demo", $where,
            $callback, array("age"));
        return $status;
    }
    /**
     * @test
     * Query with empty bin value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithEmptyBinValue()
    {
        try {
            $where = $this->db->predicateBetween("age", 30, 39);
            $callback = function ($record) {
                // we expect to never get here
                return Aerospike::ERR_CLIENT;
            };
            $status = $this->db->query("test", "demo", $where,
                $callback, "");
        } catch (ErrorException $e) {
            // expected result is an exception for the argument 5 type not being array
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with callback parameter missing
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithCallbackParameterMissing() {
        try {
            $where = $this->db->predicateBetween("age", 30, 39);
            $status = $this->db->query("test", "demo", $where,
                NULL, array("email","age"));
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Query with incorrect value for optionals
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithIncorrectValueForOptionals()
    {
        $total = 0;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        }
                } else {
                    return (AEROSPIKE::ERR_PARAM);
                }
        };
        $status = $this->db->query( "test", "demo", array(), $callback,
            array("email", "age"), array(Aerospike::OPT_READ_TIMEOUT=>""));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Query - Executing a query with where predicate containing a
     * non-indexed bin.
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
    function testQueryNegativeSecondaryIndexNotFound()
    {
        $where = $this->db->predicateEquals("first_nameaa", "raunak");
        $total = 0;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        (!is_null($record["bins"]["email"]))) {
                            if ($record["bins"]["email"] == "ajit") {
                                return Aerospike::OK;
                            }
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        }
                } else {
                    return(AEROSPIKE::ERR_PARAM);
                }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("email", "age"), array(Aerospike::OPT_READ_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Query with correct arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithCorrectArguments()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $db= $this->db;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("email", $record["bins"]) &&
                    (!is_null($record["bins"]["email"]))) {
                    if ($record["bins"]["email"] == "ellie") {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("email", "age"));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(array_key_exists('bins',$record)) {
        if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        }
        return Aerospike::ERR_CLIENT;
        /*if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;*/
    }
    /**
     * @test
     * Query with correct arguments and set NULL
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithCorrectArgumentsAndSetNull()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $db= $this->db;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0) {
                if (array_key_exists("email", $record["bins"]) &&
                    (!is_null($record["bins"]["email"]))) {
                    if ($record["bins"]["email"] == "ellie") {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
                }
            }
        };
        $status = $this->db->query("test", NULL, $where, $callback, array("email", "age"));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if($status == 0) {
            if(array_key_exists('bins',$record)) {
                if(10 == $record['bins']['bin1']) {
                        $this->db->remove($key);
                        return Aerospike::OK;
                }
            }
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with correct arguments and predicateEqual
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithPredicateEqual()
    {
        $where = $this->db->predicateEquals("age", 22);
        $total = 0;
        $db = $this->db;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("email", $record["bins"]) &&
                    (!is_null($record["bins"]["email"]))) {
                    if ($record["bins"]["email"] == "adam") {
                        $key = $db->initKey("test", "demo", "testing");
                        $db->put($key, array("bin1"=>10));
                        return Aerospike::OK;
                    }
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("email", "age"));
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
     * Query with correct arguments and put in callback
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithPutInCallback()
    {
        $total = 0;
        $in_thirties = 0;
        $where = $this->db->predicateBetween("age", 30, 39);
        $db = $this->db;
        $callback = function ($record) use (&$total, &$in_thirties, $db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                        if ($record["bins"]["email"] == "ellie") {
                        $key = $db->initKey("test", "demo", "testKey");
                        $db->put($key, array("name"=>"ellie"));
                    }
                    $total += (int) $record["bins"]["age"];
                    $in_thirties++;
                }
            }
        };
        $status = $this->db->query( "test", "demo", $where, $callback,
            array("email", "age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "testKey");
        $this->db->get($key, $record);
        $this->db->remove($key, array(Aerospike::OPT_READ_TIMEOUT=>3000));
        $result = array_diff_assoc_recursive($record["bins"], array("name"=>"ellie"));
        if (!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Query without optional arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithOptionalArguments()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $db = $this->db;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                        if ($record["bins"]["email"]=="ellie") {
                            ; // do nothing
                            $key = $db->initKey("test", "demo", "testing");
                            $db->put($key, array("bin1"=>10));
                        } else {
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        }
                    }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("email", "age"));
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
     * Query with one optional argument
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithOneOptionalArgument()
    {
        $where = $this->db->predicateBetween("age", 30, 39);
        $total = 0;
        $db= $this->db;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties, &$db) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                        if ($record["bins"]["email"] == "ellie") {
                            // do nothing
                            $key = $db->initKey("test", "demo", "testing");
                            $db->put($key, array("bin1"=>10));
                        } else {
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        }
                    }
               }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array( "email", "age"), array(Aerospike::OPT_READ_TIMEOUT=>2000));
        $key = $this->db->initKey("test", "demo", "testing");
        $this->db->get($key, $record);
        if(array_key_exists('bins',$record)) {
            if(10 == $record['bins']['bin1']) {
            $this->db->remove($key);
            return Aerospike::OK;
        }
        } else {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return Aerospike::ERR_CLIENT;
        /*if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;*/
    }
    /**
     * @test
     * Query with NO_BINS query option.
     *
     * @pre
     * Connect using aerospike object to the specified node.
     *
     * @post
     * Newly initialized Aerospike objects.
     *
     * @remark
     * Variants: OO (testGETTwoParameter).
     *
     * @test_plans{1.1}
     */
    function testQueryWithNoBinsArgument()
    {
        $failed = FALSE;
        $where = $this->db->predicateBetween("age", 10, 39);
        $callback = function ($record) use (&$failed) {
            if($record["bins"] != array()) {
                $failed = TRUE;
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array( "email", "age"), array(Aerospike::OPT_READ_TIMEOUT=>2000, Aerospike::OPT_QUERY_NOBINS=>TRUE));
        if($failed) {
            $this->fail("Record returned bins.");
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Query with empty callback function
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithEmptyCallbackFunction()
    {
        $where = $this->db->predicateBetween("age", 20, 29);
        $callback =  function ($record) { };
        $status = $this->db->query( "test", "demo", $where, $callback,
            array("email", "age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic Query with where parameter missing
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryWithWhereParameterMissing)
     *
     * @test_plans{1.1}
     */
    function testQueryWithWhereParameterMissing()
    {
        try {
            $total = 0;
            $in_thirties = 0;
            $callback = function ($record) use (&$total, &$in_thirties) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                            $total += (int) $record["bins"]["age"];
                            $in_thirties++;
                        }
                }
            };
            $status = $this->db->query("test", "demo", $callback,array( "email", "age"));
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * Query with where containing null value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithWhereContainingNullValue()
    {
        $where = $this->db->predicateEquals("age", "");
        $callback =  function ($record) {
            // we do not expect to be here
            return false;
        };
        try {
            $status = $this->db->query( "test", "demo", $where,
                $callback, array("email", "age"));
        } catch (ErrorException $e) {
            // we expect the query() to throw this exception
            return Aerospike::ERR_PARAM;
        }
        return AEROSPIKE::ERR_PARAM;
    }

    /**
     * @test
     * Query with where containing null string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithWhereContainingNullString()
    {
        $total = 0;
        $in_thirties = 0;
        $callback = function ($record) use (&$total, &$in_thirties) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                        $total += (int) $record["bins"]["age"];
                        $in_thirties++;
                    }
                }
        };
        $status = $this->db->query( "test", "demo", array(""), $callback,
            array( "email", "age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Query without where predicate.(i.e.scan)
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryWithoutPredicate)
     *
     * @test_plans{1.1}
     */
    function testQueryWithoutPredicate()
    {
        $count = 0;
        $callback = function ($record) use (&$count) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                    if (array_key_exists("email", $record["bins"]) &&
                        !is_null($record["bins"]["email"])) {
                            if ($record["bins"]["email"] == "john" ||
                                $record["bins"]["email"] == "smith" ||
                                $record["bins"]["email"] == "adam" ||
                                $record["bins"]["email"] == "ellie") {
                                $count++;
                            }
                        }
                } else {
                    return (AEROSPIKE::ERR_PARAM);
                }
        };
        $status = $this->db->query( "test", "demo", array(), $callback, array("email", "age"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        if ($count < 4) {
            return (AEROSPIKE::ERR_PARAM);
        }
        return $status;
    }

    /**
     * @test
     * Query with callback returning false
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithCallbackReturningFalse()
    {
        $where = $this->db->predicateEquals("age", 29);
        $callback =  function ($record) {
            return false;
        };
        return ($this->db->query("test", "demo", $where,
                $callback));
    }

    /**
     * @test
     * Query with callback returning false
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testQueryWithCallbackReturningFalseWithSelectBins()
    {
        $where = $this->db->predicateEquals("age", 29);
        $callback =  function ($record) {
            return false;
        };
        return ($this->db->query("test", "demo", $where,
                $callback, array("email", "age")));
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnListStringPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnListStringPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("fav_movies", Aerospike::INDEX_TYPE_LIST, "a");
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("fav_movies", $record["bins"]) &&
                    (!is_null($record["bins"]["fav_movies"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("fav_movies"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 2)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnListNumericPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnListNumericPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("numbers", Aerospike::INDEX_TYPE_LIST, 2);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("numbers", $record["bins"]) &&
                    (!is_null($record["bins"]["numbers"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("numbers"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 3)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnListNumericPredicateRange)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnListNumericPredicateRange()
    {
        $total_records = 0;
        $db= $this->db;
        $where = $this->db->predicateRange("numbers", Aerospike::INDEX_TYPE_LIST, 3, 5);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("numbers", $record["bins"]) &&
                    (!is_null($record["bins"]["numbers"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("numbers"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 5)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapKeysStringPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapKeysStringPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("movies_viewed", Aerospike::INDEX_TYPE_MAPKEYS, "aa");
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("movies_viewed", $record["bins"]) &&
                    (!is_null($record["bins"]["movies_viewed"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("movies_viewed"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 2)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapKeysNumericPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapKeysNumericPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("map_numeric", Aerospike::INDEX_TYPE_MAPKEYS, 10);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("map_numeric", $record["bins"]) &&
                    (!is_null($record["bins"]["map_numeric"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("map_numeric"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 2)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapKeysNumericPredicateRange)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapKeysNumericPredicateRange()
    {
        $total_records = 0;
        $db= $this->db;
        $where = $this->db->predicateRange("map_numeric", Aerospike::INDEX_TYPE_MAPKEYS, 15, 20);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("map_numeric", $record["bins"]) &&
                    (!is_null($record["bins"]["map_numeric"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("map_numeric"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 1)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    ///////

    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapValuesStringPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapValuesStringPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("mapval_str", Aerospike::INDEX_TYPE_MAPVALUES, "x");
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("mapval_str", $record["bins"]) &&
                    (!is_null($record["bins"]["mapval_str"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("mapval_str"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 1)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapValuesNumericPredicateContains)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapValuesNumericPredicateContains()
    {
        $total_records = 0;
        $db= $this->db;
        $in_thirties = 0;
        sleep(5);
        $where = $this->db->predicateContains("mapval_num", Aerospike::INDEX_TYPE_MAPVALUES, 100);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("mapval_num", $record["bins"]) &&
                    (!is_null($record["bins"]["mapval_num"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("mapval_num"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 1)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Query with index is created on list of integers.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testQueryIndexOnMapValuesNumericPredicateRange)
     *
     * @test_plans{1.1}
     */
    function testQueryIndexOnMapValuesNumericPredicateRange()
    {
        $total_records = 0;
        $db= $this->db;
        $where = $this->db->predicateRange("mapval_num", Aerospike::INDEX_TYPE_MAPVALUES, 22, 44);
        $callback = function ($record) use (&$total_records) {
            if (strcmp("test", $record["key"]["ns"]) == 0 && strcmp("demo",
                $record["key"]["set"]) == 0) {
                if (array_key_exists("mapval_num", $record["bins"]) &&
                    (!is_null($record["bins"]["mapval_num"]))) {
                        $total_records++;
                }
            }
        };
        $status = $this->db->query("test", "demo", $where, $callback,
            array("mapval_num"));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($total_records == 1)
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
}
?>
