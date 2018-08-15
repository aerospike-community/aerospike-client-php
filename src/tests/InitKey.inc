<?php
require_once 'Common.inc';

/**
 * initKey helper function tests
*/

class InitKey extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic initKey operation
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testInitKeyHelper)
     *
     * @test_plans{1.1}
     */
    function testInitKeyHelper() {
        $key = $this->db->initKey("demo", "test", 10000);
        if (!$key) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic initKey namespace value integer
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testNameSpaceValue)
     *
     * @test_plans{1.1}
     */
    function testNameSpaceValueInt() {
        $key = $this->db->initKey(2312312, "test", 10000);
        if (!$key) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic initKey set value integer
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSetValueInt)
     *
     * @test_plans{1.1}
     */
    function testSetValueInt() {
        $key = $this->db->initKey("demo", 12312312, 10000);
        if (!$key) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic initKey with digest
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSetValueInt)
     *
     * @test_plans{1.1}
     */
    function testInitKeyDigest() {
        $key = $this->db->initKey("test", "demo",
            str_repeat("a", 20), true);
        if (!$key) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic initKey with digest exceeding max length
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSetValueInt)
     *
     * @test_plans{1.1}
     */
    function testInitKeyDigestExceedingMaxLen() {
        $key = $this->db->initKey("test", "demo",
            base64_decode("test_init_key_with_exceeding_max_length"), true);
        if (!$key) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
}
?>
