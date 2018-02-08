<?php
require_once 'Common.inc';

/**
 * initKey helper function tests
*/

class GetKeyDigest extends AerospikeTestCommon
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
     * Basic getKeyDigest operation with Integer Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestStringPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestIntegerPositive() {
        $key = $this->db->initKey("test", "demo", 10000);
        $digest = $this->db->getKeyDigest("test", "demo", 10000);
        $this->db->put($key, array('test-bin'=>'test-value'));
        $this->keys[] = $key;
        $this->db->get($key, $rec);
        if (array_key_exists("key", $rec) && array_key_exists("digest",
            $rec["key"]) && $digest !== $rec["key"]["digest"]) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic getKeyDigest operation with String Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestIntegerPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestStringPositive() {
        $key = $this->db->initKey("test", "demo", "test-key");
        $digest = $this->db->getKeyDigest("test", "demo", "test-key");
        $this->db->put($key, array('test-bin'=>'test-value'));
        $this->keys[] = $key;
        $this->db->get($key, $rec);
        if (array_key_exists("key", $rec) && array_key_exists("digest",
            $rec["key"]) && $digest !== $rec["key"]["digest"]) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic getKeyDigest with namespace value integer
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestIntegerPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestNameSpaceValueIntPositive() {
        $digest = $this->db->getKeyDigest(2312312, "test", 10000);
        if (!$digest) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Basic getKeyDigest with set value integer
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestIntegerPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestSetValueIntPositive() {
        $digest = $this->db->getKeyDigest("test", 12312312, 10000);
        if (!$digest) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Negative getKeyDigest operation with List Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestStringPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestListNegative() {
        $digest = $this->db->getKeyDigest("test", "demo", array(1,2,3));
        if (!$digest) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Negative getKeyDigest operation with Map Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestStringPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestMapNegative() {
        $digest = $this->db->getKeyDigest("test", "demo", array("a" => 1, "b" => 2,
            "c" => 3));
        if (!$digest) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Negative getKeyDigest operation with NULL key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetKeyDigestStringPositive)
     *
     * @test_plans{1.1}
     */
    function testGetKeyDigestNullNegative() {
        try {
            $digest = $this->db->getKeyDigest("test", "demo", NULL);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        if (!$digest) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }

}
?>
