<?php
require_once 'Common.inc';

/**
 *Basic Append  opeartion tests
*/

class Append  extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = array("ns" => "test", "set" => "demo","key" =>"Append_key");
        $this->db->put($key,array("Greet"=>"World_end"));
        $this->keys[] = $key;
   }

    /**
     * @test
     * Basic append opeartion on bin value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
     function testAppendOnBinValue() {
        $this->db->append($this->keys[0], 'Greet', ' World');
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if ("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on bin value with policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
     function testAppendOnBinValueWithPolicy() {
         $this->db->append($this->keys[0], 'Greet', ' World',
             array(Aerospike::OPT_WRITE_TIMEOUT=>1000,
             Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin value append operation when key not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendKeyNotExist)
     *
     * @test_plans{1.1}
     */
    function testAppendKeyNotExist() {
        $key = array("ns" => "test", "set" => "demo", "key" => "---------AppendKey-------");
        $this->keys[] = $key;
        return($this->db->append($key, 'Greet', ' World',
                array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE,
                Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST)));
    }
    /**
     * @test
     * Basic bin append operatrion when not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendBinNotExist)
     *
     * @test_plans{1.1}
     */
    function testAppendBinNotExist() {
        $opts = array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE, 
            Aerospike::OPT_WRITE_TIMEOUT=>1000,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_ALL);
        if ($this->db->isConnected()) {
            return($this->db->append($this->keys[0], 'Greet1234', ' World', $opts));
        } else {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic bin value append when append value not string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendValueNotString)
     *
     * @test_plans{1.1}
     */
    function testAppendValueNotString() {
        if ($this->db->isConnected()) {
            return($this->db->append($this->keys[0], 'Greet', 888888));
        } else {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic append opeartion on with policy GEN_GT
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithGenGTPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 10;
        $this->db->append($this->keys[0], 'Greet', ' World', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on with policy GEN_GT value lesser
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithGenGTLesser() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->append($this->keys[0], 'Greet', ' World', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on with policy GEN_EQ
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithGenEQPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->append($this->keys[0], 'Greet', ' World',
            array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ,
            $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on with policy GEN_EQ not equal
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithGenEQNotEqual() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $this->db->append($this->keys[0], 'Greet', ' World', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on with policy GEN_EQ_IGNORE
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithGenEQIgnore() {
        $gen_value = 10;
        $this->db->append($this->keys[0], 'Greet', ' World', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append opeartion on with policy key digest
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithPolicyKeyDigest() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo",
         base64_decode("append_option_policy_key_digest"), true);
        $put_status = $this->db->put($key, array("Greet"=>"World_end"), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $this->db->append($key, 'Greet', ' World', array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $status = $this->db->get($key, $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic append operation on with policy key send
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAppendOnBinValue)
     *
     * @test_plans{1.1}
     */
    function testAppendWithPolicyKeySend() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo", 1);
        $put_status = $this->db->put($key, array("Greet"=>"World_end"), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $this->db->append($key, 'Greet', ' World', array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $status = $this->db->get($key, $get_record, array('Greet'));
        if("World_end World" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
?>
