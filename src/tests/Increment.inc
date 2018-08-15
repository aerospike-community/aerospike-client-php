<?php
require_once 'Common.inc';

/**
 *Basic Increment opeartion tests
*/

class Increment extends AerospikeTestCommon
{
    function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $key = $this->db->initKey("test", "demo", "increment_test");
        $this->db->put($key, array("bin1"=>1, "bin2"=>"1"));
        $this->keys[] = $key;
   }

    /**
     * @test
     * Basic bin increment operation
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementByOffsetValue)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementByOffsetValue() {
        $this->db->increment($this->keys[0], 'bin1', 4);
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin increment operation with policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementByOffsetValue)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementByOffsetValueWithPolicy() {
        $this->db->increment($this->keys[0], 'bin1', 4,
            array(Aerospike::OPT_WRITE_TIMEOUT=>1000,
            Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin increment operation use initial value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementByOffsetValue)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementByOffsetValueUseInitialValue() {
        $this->db->increment($this->keys[0], 'bin3', 2,
            array(Aerospike::OPT_WRITE_TIMEOUT=>1000));
        $status = $this->db->get($this->keys[0], $get_record, array('bin3'));
        if(2 != $get_record['bins']['bin3']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin increment operation when key not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testKeyRemoveKeyNotExist)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementKeyNotExist() {
        $key1 = array("ns" => "test", "set" => "demo", "key" => "----ssss----");
        $this->db->increment($key1, 'bin1', 4,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_ONCE,
            Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $this->keys[] = $key1;
        $status = $this->db->get($key1, $get_record, array('bin1'));
        if(4 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin increment when bin not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementBinNotExist)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementBinNotExist() {
        $this->db->increment($this->keys[0], 'abc', 4);
        $status = $this->db->get($this->keys[0], $get_record, array('abc'));
        if(4 != $get_record['bins']['abc']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin increment when offset value string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementOffsetValueString)
     *
     * @test_plans{1.1}
     */
    function testBinIncrementOffsetValueString()
    {
        $status = $this->db->increment($this->keys[0], 'bin1', "abc");
        return $status;
    }
    /**
     * @test
     * Basic bin increment when offset value is a numeric string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementOffsetValueNumericString)
     */
    function testBinIncrementOffsetValueNumericString()
    {
        $status = $this->db->increment($this->keys[0], 'bin1', "10");
        return $status;
    }
    /**
     * @test
     * Basic bin increment when offset value is a Negative numeric string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBinIncrementOffsetValueNegativeNumericString)
     */
    function testBinIncrementOffsetValueNegativeNumericString()
    {
        $status = $this->db->increment($this->keys[0], 'bin1', "-10");
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy GEN_GT
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithGenGTPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 10;
        $this->db->increment($this->keys[0], 'bin1', 4, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy GEN_GT value lesser
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithGenGTLesser() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->increment($this->keys[0], 'bin1', 4, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(1 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy GEN_EQ
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithGenEQPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->increment($this->keys[0], 'bin1', 4, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy GEN_EQ not equal
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithGenEQNotEqual() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $this->db->increment($this->keys[0], 'bin1', 4, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(1 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy GEN_EQ_IGNORE
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithGenEQIgnore() {
        $gen_value = 10;
        $this->db->increment($this->keys[0], 'bin1', 4, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment opeartion on with policy key digest
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPrependOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithPolicyKeyDigest() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo",
        		str_repeat("a", 20), true);
        $put_status = $this->db->put($key, array("bin1"=>1), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $this->db->increment($key, 'bin1', 4, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $status = $this->db->get($key, $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic increment operation on with policy key send
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testIncrementOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testIncrementWithPolicyKeySend() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo", 1);
        $put_status = $this->db->put($key, array("bin1"=>1), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $this->db->increment($key, 'bin1', 4, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $status = $this->db->get($key, $get_record, array('bin1'));
        if(5 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
?>
