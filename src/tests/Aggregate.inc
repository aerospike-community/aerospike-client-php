<?php
require_once 'Common.inc';
/**

 *Basic Aggregate tests

 */
class Aggregate extends AerospikeTestCommon
{

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key,array("first_name"=>"john", "age"=>29));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key1");
        $this->db->put($key,array("first_name"=>"smith", "age"=>27));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key2");
        $this->db->put($key,array("first_name"=>"adam", "age"=>22));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key3");
        $this->db->put($key,array("first_name"=>"ellie", "age"=>32));
        $this->keys[] = $key;
        $this->ensureIndex('test', 'demo', 'age', 'demo_age_idx', Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);
    }

    /**
     * @test
     * Aggregate - first name of emp where age in between
     * 20 and 29.
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
    function testAggregatePositive()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $returned);
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        if (array_key_exists("john", $returned[0]) && array_key_exists("smith", $returned[0]) &&
            array_key_exists("adam", $returned[0]))
        {
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }

    /**
     * @test
     * Scan Aggregate - empty predicate
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testScanAggregation)
     *
     * @test_plans{1.1}
     */
    function testScanAggregation()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = array();
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $returned);
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        if (isset($returned) && is_array($returned[0])) {
                if (array_key_exists("john", $returned[0]) &&
                    array_key_exists("smith", $returned[0]) &&
                    array_key_exists("ellie", $returned[0]) &&
                    array_key_exists("adam", $returned[0])) {
                        return Aerospike::OK;
                    } else {
                        return Aerospike::ERR_CLIENT;
                    }
            }
    }

    /**
     * @test
     * Aggregate - first name of emp where age in between
     * 20 and 29 with policy
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
    function testAggregatePositiveWithPolicy()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $returned, array(Aerospike::OPT_READ_TIMEOUT=>1000));
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        if (array_key_exists("john", $returned[0]) && array_key_exists("smith", $returned[0]) &&
            array_key_exists("adam", $returned[0])) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
    }

    /**
     * @test
     * Aggregate - Applying aggregation with where predicate containing a
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
    function testAggregateNegativeSecondaryIndexNotFound()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateEquals("first_nameaa", "raunak");
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Aggregate - Where predicate not satisfied by any record.
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
    function testAggregatePositiveEmptyAggregationResult()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateEquals("age", 1000);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($names["bins"])) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Aggregate - namespace and set are incorrect.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateUnknownNamespaceAndSetNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateUnknownNamespaceAndSetNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("t", "d", $where, "test_stream", "group", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Aggregate - Module name is incorrect.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateUnknownModuleNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateUnknownModuleNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "agg", "group_count", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Aggregate - Unknown function passed.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateUnknownFunctionNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateUnknownFunctionNegative()
    {
        gc_enable();
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Aggregate - Empty namespace and set.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateEmptyNamespaceSetNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateEmptyNamespaceSetNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("", "", $where, "test_stream", "group", array("first_name"), $names);
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        return($status);
    }

    /**
     * @test
     * Aggregate - Empty module and function
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateEmptyFunctionUDFNameNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateEmptyFunctionUDFNameNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "", "", array("first_name"), $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Aggregate - String variable is passed as argument to UDF.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateArgumentIsVariableNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateArgumentIsVariableNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $arg = "args";
        $where = $this->db->predicateBetween("age", 20, 29);
        try{
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group", $arg, $names);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        }catch(Exception $e) {
        	return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Aggregate - Empty argument passed to UDF.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAggregateArgsEmptyNegative)
     *
     * @test_plans{1.1}
     */

    function testAggregateArgsEmptyNegative()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $arg = "args";
        $where = $this->db->predicateBetween("age", 20, 29);
        try {
			$status = $this->db->aggregate("test", "demo", $where, "test_stream", "group", "", $names);
			if ($status !== Aerospike::OK) {
				return $this->db->errorno();
			}
        } catch(Exception $e) {
        	return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Aggregate - Extra parameter in call to lua function
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
    function testAggregateExtraParameter()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name", "last_name"), $returned);
        if (array_key_exists("john", $returned[0]) && array_key_exists("smith", $returned[0]) &&
            array_key_exists("adam", $returned[0])) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
    }
    /**
     * @test
     * Aggregate - Lua function definition contains extra parameter
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
    function testAggregateLuaExtraParameter()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_extra_parameter.lua", "test_aggregate_extra_parameter.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_aggregate_extra_parameter", "group_count", array("first_name"), $returned);
        if (array_key_exists("john", $returned[0]) && array_key_exists("smith", $returned[0]) &&
            array_key_exists("adam", $returned[0])) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
    }
    /**
     * @test
     * Aggregate - Lua function definition contains less parameters
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
    function testAggregateLuaLessParameter()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_less_parameter.lua", "test_aggregate_less_parameter.lua");
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->aggregate("test", "demo", $where, "test_aggregate_less_parameter", "group_count", array("first_name"), $returned);
        if ($status != Aerospike::OK) {
            return($status);
        }
        if (array_key_exists("john", $returned[0]) && array_key_exists("smith", $returned[0]) &&
            array_key_exists("adam", $returned[0])) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
    }
    /**
     * @test
     * Basic Aggregate with where missing
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
    function testAggregateWithWhereParameterMissing()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_less_parameter.lua", "test_aggregate_less_parameter.lua");
        try {
            $status = $this->db->aggregate("test", "demo", "test_aggregate_less_parameter", "group_count", array("first_name"), $returned);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }

    /**
     * @test
     * Aggregate with where containing string value
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
    function testAggregateWithWhereContainingStringValue()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_less_parameter.lua", "test_aggregate_less_parameter.lua");
        try {
            $where = $this->db->predicateBetween("age","21","ages");
            $status = $this->db->aggregate("test", "demo", $where, "test_aggregate_less_parameter", "group_count", array("first_name"), $returned);
            var_dump("status is", $status);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * Aggregate with where containing null string
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
    function testAggregateWithWhereContainingNullString()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_less_parameter.lua", "test_aggregate_less_parameter.lua");
        $status = $this->db->aggregate("test", "demo", array(""), "test_aggregate_less_parameter", "group_count", array("first_name"), $returned);
        return $status;
    }
    /**
     * @test
     * Aggregate with where containing bool value
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
    function testAggregateWithWhereContainingBoolValue()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", true, true);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "group_count", array("first_name"), $returned);
        if (count($returned) == 0) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
    }
    /**
     * @test
     * Aggregate with where containing array value
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
    function testAggregateWithWhereContainingArrayValue()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate_less_parameter.lua", "test_aggregate_less_parameter.lua");
        try {
            $where = $this->db->predicateBetween("age", array(""), array(""));
            $status = $this->db->aggregate("test", "demo", $where, "test_aggregate_less_parameter","group_count", array("first_name"), $names);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * Aggregate returning multiple records
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     *
     *
     */
    function testAggregateReturnMultipleRecords()
    {
        $this->ensureUdfModule("tests/lua/test_stream.lua", "test_stream.lua");
        $where = $this->db->predicateBetween("age", 0, 50);
        $status = $this->db->aggregate("test", "demo", $where, "test_stream", "test_aggregate", array("first_name"), $returned);
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        $iRec = 0;
        for ($i = 0; $i < count($returned); $i++)
        {
            if (in_array("john", $returned[$i]) && in_array("29", $returned[$i]))
            {
                $iRec = $iRec + 1;
            } else if (in_array("ellie", $returned[$i]) && in_array("32", $returned[$i]))
            {
                $iRec = $iRec + 1;
            } else if (in_array("adam", $returned[$i]) && in_array("22", $returned[$i]))
            {
                $iRec = $iRec + 1;
            } else  if (in_array("smith", $returned[$i]) && in_array("27", $returned[$i]))
            {
                $iRec = $iRec + 1;
            }
        }

        if ($iRec == 4)
        {
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }
    /**
     * @test
     * Aggregate returning multiple float records
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     *
     * @remark
     *
     *
     */
    function normal_001()
    {
        $this->ensureUdfModule("tests/lua/test_aggregate.lua", "test_aggregate.lua");
        $where = $this->db->predicateBetween("age", 0, 50);
        $status = $this->db->aggregate("test", "demo", $where, "test_aggregate", "double_group_count", array("first_name", "age", 2), $returned);
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }
        return Aerospike::OK;
    }
}

?>
