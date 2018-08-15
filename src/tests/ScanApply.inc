<?php
require_once 'Common.inc';
/**

 *Basic Aggregate tests

 */
class ScanApply extends AerospikeTestCommon
{
    /*
     * TODO: Remove these constants and map with actual Aerospike class
     * constants.
     */
    private $ages = array(29, 27, 22, 32, 35, 32.5);


    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $this->ensureUdfModule("tests/lua/test_transform.lua", "test_transform.lua");
        sleep(2);
        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key, array("first_name"=>"john", "age"=>($this->ages[0])));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key1");
        $this->db->put($key, array("first_name"=>"smith", "age"=>($this->ages[1])));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key2");
        $this->db->put($key, array("first_name"=>"adam", "age"=>($this->ages[2])));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key3");
        $this->db->put($key, array("first_name"=>"ellie", "age"=>($this->ages[3])));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key4");
        $this->db->put($key, array("first_name"=>"mitchell", "age"=>($this->ages[4])));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_Float");
        $this->db->put($key, array("first_name"=>"mitchell", "age"=>($this->ages[5])));
        $this->keys[] = $key;
    }

    /**
     * @test
     * ScanApply - All correct arguments
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
    function testScanApplyPositive()
    {
        $status = $this->db->get($this->keys[2], $get_record, array('age'));
       $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age'));
        if(42 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - namespace and set are incorrect.
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

    function testScanApplyUnknownNamespaceAndSetNegative()
    {
        $status = $this->db->scanApply("t", "d", "test_transform", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - Module name is incorrect.
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

    function testScanApplyUnknownModuleNegative()
    {
        $where = $this->db->predicateBetween("age", 20, 29);
        $status = $this->db->scanApply("test", "demo", "test_transform1", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - Unknown function passed.
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

    function testScanApplyUnknownFunctionNegative()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform1", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[2], $get_record, array('age','first_name'));
        if(22 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - Empty namespace and set.
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
    function testScanApplyEmptyNamespaceSetNegative()
    {
        $status = $this->db->scanApply("", "", "test_transform", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - Empty module and function
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

    function testScanApplyEmptyFunctionUDFNameNegative()
    {
        $status = $this->db->scanApply("test", "demo", "", "", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - String variable is passed as argument to UDF.
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

    function testScanApplyArgumentIsVariableNegative()
    {
        $arg = "args";
        try {
			$status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", "age", $scan_id);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}
			return $status;
        } catch(ErrorException $e) {
        	return $this->db->errorno();
        }
    }

    /**
     * @test
     * ScanApply - Empty argument passed to UDF.
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

    function testScanApplyArgsEmptyNegative()
    {
        $arg = "args";
        try {
			$status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", "", $scan_id);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}
			return $status;
        } catch(ErrorException $e) {
        	return $this->db->errorno();
        }
    }
    /**
     * @test
     * ScanApply - Extra parameter in call to lua function
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
    function testScanApplyExtraParameter()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20, 30), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age'));
        if(42 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - Lua function definition contains extra parameter
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
    function testScanApplyLuaExtraParameter()
    {
        $this->ensureUdfModule("tests/lua/test_transform_extra_parameter.lua", "test_transform_extra_parameter.lua");
        $status = $this->db->scanApply("test", "demo", "test_transform_extra_parameter", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age','first_name'));
        if(22 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }

    /**
     * @test
     * ScanApply - Lua function definition contains less parameters
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
    function testScanApplyLuaLessParameter()
    {
        $this->ensureUdfModule("tests/lua/test_transform_less_parameter.lua", "test_transform_less_parameter.lua");
        $status = $this->db->scanApply("test", "demo", "test_transform_less_parameter", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - positive with all arguments
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
    function testScanApplyPositiveWithAllArguments()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_PERCENTAGE=>100, Aerospike::OPT_SCAN_NOBINS=>false));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age','first_name'));
        if(42 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - null string for scanid
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
    function testScanApplyScanIdIsNull()
    {
        $scan_id = "";
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age','first_name'));
        if(42 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - percent is 80
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
    function testScanApplyPercentIsInt()
    {
        $count = 0;
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_PERCENTAGE=>80));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        for ($i = 0; $i < 5; $i++) {
            $status = $this->db->get($this->keys[$i], $get_record, array('age'));
            if (($this->ages[$i] + 20) == $get_record['bins']['age']) {
                $count++;
            }
        }
        if($count >= 3) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * ScanApply - percent is a string
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
    function testScanApplyPercentIsString()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_PERCENTAGE=>"abc"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - Scan priority is string
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
    function testScanApplyPriorityIsString()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_PRIORITY=>"SCAN_PRIORITY_AUTO"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - conncurent option is integer.
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
    function testScanApplyConcurrentIsInt()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_CONCURRENTLY=>5));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - conncurent option is positive
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
    function testScanApplyConcurrentPositive()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id, array(Aerospike::OPT_SCAN_CONCURRENTLY=>true));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[2], $get_record, array('age'));
        if(42 != $get_record['bins']['age']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * ScanApply - Modify record with float value.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_001)
     *
     * @test_plans{1.1}
     */
    function normal_001()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20.1), $scan_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($scan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        $status = $this->db->get($this->keys[5], $get_record, array('age'));
        return Aerospike::OK;
    }
}
?>
