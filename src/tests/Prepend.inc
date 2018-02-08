<?php
require_once 'Common.inc';

/**
 *Basic Prepend  opeartion tests
*/

class Prepend extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "Prepend_key");
        $this->db->put($key,array("Greet"=>"World_end"));
        $this->keys[] = $key;
    }

    /**
     * @test
     * Basic prepend opeartion on bin value
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
    function testPrependOnBinValue() {
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ');
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on bin value with policy
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
    function testPrependOnBinValueWithPolicy() {
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ',
            array(Aerospike::OPT_WRITE_TIMEOUT=>1000,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_ALL));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic bin value prepend operation when key not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPrependKeyNotExist)
     *
     * @test_plans{1.1}
     */
    function testPrependKeyNotExist() {
        $key1 = $this->db->initKey("test", "demo", "---sss----");
        return $this->db->prepend($key1, 'Greet', ' World',
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
    }
    /**
     * @test
     * Basic bin prepend operatrion when not exist
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPrependBinNotExist)
     *
     * @test_plans{1.1}
     */
        function testPrependBinNotExist() {
            return $this->db->prepend($this->keys[0], 'sssssssssss', ' World', array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_ONCE));
    }
    /**
     * @test
     * Basic bin value prepend when prepend value not string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPrependValueNotString)
     *
     * @test_plans{1.1}
     */
    function testPrependValueNotString() {
        $opts = array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE,
            Aerospike::OPT_WRITE_TIMEOUT=>1000,
            Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND);
        return $this->db->prepend($this->keys[0], 'Greet', 888888, $opts);
    }
    /**
     * @test
     * Basic prepend opeartion on with policy GEN_GT
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
    function testPrependWithGenGTPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 10;
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on with policy GEN_GT value lesser
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
    function testPrependWithGenGTLesser() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on with policy GEN_EQ
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
    function testPrependWithGenEQPositive() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on with policy GEN_EQ not equal
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
    function testPrependWithGenEQNotEqual() {
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on with policy GEN_EQ_IGNORE
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
    function testPrependWithGenEQIgnore() {
        $gen_value = 10;
        $this->db->prepend($this->keys[0], 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, $gen_value)));
        $status = $this->db->get($this->keys[0], $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend opeartion on with policy key digest
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
    function testPrependWithPolicyKeyDigest() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo",
        		str_repeat("a", 20), true);
        $put_status = $this->db->put($key, array("Greet"=>"World_end"), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $this->db->prepend($key, 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $status = $this->db->get($key, $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic prepend operation on with policy key send
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
    function testPrependWithPolicyKeySend() {
        $gen_value = 10;
        $key = $this->db->initKey("test", "demo", 1);
        $put_status = $this->db->put($key, array("Greet"=>"World_end"), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $this->db->prepend($key, 'Greet', 'Hello ', array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $status = $this->db->get($key, $get_record, array('Greet'));
        if("Hello World_end" != $get_record['bins']['Greet']) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
?>
