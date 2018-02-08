<?php
require_once 'Common.inc';

/**
 *Basic touch  opeartion tests
*/

class Touch extends AerospikeTestCommon
{

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "touch_key");
        $this->db->put($key, array("Greet"=>"Hello World"));
        $this->keys[] = $key;
    }
    /**
     * @test
     * Basic Touch operation
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBasicTouchOpeartion)
     *
     * @test_plans{1.1}
     */

    function testBasicTouchOperation() {
        $rv = $this->db->put($this->keys[0], array("bin1"=>"Hello World"));
        $this->db->touch($this->keys[0], 10);
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if("Hello World" != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic Touch operation with policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testBasicTouchOpeartion)
     *
     * @test_plans{1.1}
     */

    function testBasicTouchOperationWithPolicy() {
        $rv = $this->db->put($this->keys[0], array("bin1"=>"Hello World"));
        $this->db->touch($this->keys[0], 10,
            array(Aerospike::OPT_WRITE_TIMEOUT=>200,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
        $status = $this->db->get($this->keys[0], $get_record, array('bin1'));
        if("Hello World" != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic Touch operation when key not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOperationKeyNotExist)
     *
     * @test_plans{1.1}
     */
    function testTouchOperationKeyNotExist() {
        $key1 = array("ns" => "test", "set" => "demo", "key" => "-----sshh---");
        return ($this->db->touch($key1, 120, array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE)));
    }
    /**
     * @test
     * Basic touch opeartion on with policy GEN_GT
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithGenGTPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 10;
        $touch_status = $this->db->touch($this->keys[0], 12, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch opeartion on with policy GEN_GT value lesser
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithGenGTLesser() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $touch_status = $this->db->touch($this->keys[0], 120, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch opeartion on with policy GEN_EQ
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithGenEQPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $touch_status = $this->db->touch($this->keys[0], 120, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch opeartion on with policy GEN_EQ not equal
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithGenEQNotEqual() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $touch_status = $this->db->touch($this->keys[0], 120, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch opeartion on with policy GEN_EQ_IGNORE
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithGenEQIgnore() {
        $gen_value = 10;
        $touch_status = $this->db->touch($this->keys[0], 120, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch opeartion on with policy key digest
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithPolicyKeyDigest() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo",
        		str_repeat("a", 20), true);
        $put_status = $this->db->put($key, array("bin1"=>1), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $touch_status = $this->db->touch($key, 120, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $status = $this->db->get($key, $get_record, array('bin1'));
        if(1 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
    /**
     * @test
     * Basic touch operation on with policy key send
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testTouchOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testTouchWithPolicyKeySend() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo", 1);
        $put_status = $this->db->put($key, array("bin1"=>1), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $touch_status = $this->db->touch($key, 120, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $status = $this->db->get($key, $get_record, array('bin1'));
        if(1 != $get_record['bins']['bin1']) {
            return Aerospike::ERR_CLIENT;
        }
        return $touch_status;
    }
}
?>
