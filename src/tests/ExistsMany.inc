<?php
require_once 'Common.inc';

/**
 *Basic Key Exists/getMetaData opeartion tests
*/

class ExistsMany extends AerospikeTestCommon
{

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "existsMany1");
        $this->db->put($key, array("binA"=>10));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "existsMany2");
        $this->db->put($key, array("binB"=>20));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "existsMany3");
        $this->db->put($key, array("binC"=>30));
        $this->keys[] = $key;
    }

    /**
     * @test
     * Basic existsMany operation with all arguments.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyPositive)
     *
     * @test_plans{1.1}
     */
    function testExistsManyPositive() {
        $status = $this->db->existsMany($this->keys, $metadata, array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany operation with all arguments without options and filter
     * bins but with one non-existent key.
     * Basic existsMany operation incorrect key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyPositive)
     *
     * @test_plans{1.1}
     */
    function testExistsManyWithNonExistentKeyPositive() {
        $my_keys = $this->keys;
        $key4 = array($this->db->initKey("test", "demo", "existsMany5"));
        array_splice($my_keys, 1, 0, $key4);
        $status = $this->db->existsMany($my_keys, $metadata);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }

        $i = 0;
        foreach ($metadata as $metadata_key=>$value) {
            if (strcmp($my_keys[$i]["key"], $metadata_key) || ($i == 1 && $value)) {
                if($metadata[$i]["metadata"] == NULL)
                {
                    if(strcmp($value["key"]["key"],$my_keys[1]["key"]))
                    {
                        return Aerospike::ERR_CLIENT;
                    }
                }
            }
            $i++;
        }
        return $status;
    }

    /**
     * @test
     * Basic existsMany operation incorrect key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyPositive)
     *
     * @test_plans{1.1}
     */
    function testExistsManyPositiveIncorrectKey() {
        $key = $this->db->initKey("test", "demo", "existsMany4");
        $this->db->put($key, array("binA"=>10));
        $status = $this->db->existsMany($this->keys, $metadata, array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany operation with empty key array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyEmptyKeyArrayNegative)
     *
     * @test_plans{1.1}
     */
    function testExistsManyEmptyKeyArrayNegative() {
        $status = $this->db->existsMany(array(), $metadata);
        if (!empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany operation with empty string passed 
       in place of key array .
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyKeyEmptyStringNegative)
     *
     * @test_plans{1.1}
     */
    function testExistsManyKeyEmptyStringNegative() {
        try {
            $status = $this->db->existsMany("", $metadata);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany operation with no arguments.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyNoArgumentsNegative)
     *
     * @test_plans{1.1}
     */
    function testExistsManyNoArgumentsNegative() {
        try {
            $status = $this->db->existsMany();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany, Metadata is empty string variable.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyMetadataEmptyStringPositive)
     *
     * @test_plans{1.1}
     */
    function testExistsManyMetadataEmptyStringPositive() {
        $metadta = "";
        $status = $this->db->existsMany($this->keys, $metadata);
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany, Metadata is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyMetadataStringVariablePositive)
     *
     * @test_plans{1.1}
     */
    function testExistsManyMetadataStringVariablePositive() {
        $metadata = "abc";
        $status = $this->db->existsMany($this->keys, $metadata);
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany, Options is of type integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyOptionsIntegerNegative)
     *
     * @test_plans{1.1}
     */
    function testExistsManyOptionsIntegerNegative() {
        try {
            $status = $this->db->existsMany($this->keys, $metadata, 20);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    /**
     * @test
     * Basic existsMany, initKey with digest.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testExistsManyInitKeyWithDigest)
     *
     * @test_plans{1.1}
     */
    function testExistsManyInitKeyWithDigest() {
        $key1 = $this->db->initKey("test", "demo", str_repeat("a", 20), true);
        $this->db->put($key1, array("binA"=>10));
        $this->keys[] = $key1;
        $key2 = $this->db->initKey("test", "demo", str_repeat("b", 20), true);
        $this->db->put($key2, array("binB"=>20));
        $this->keys[] = $key2;

        $keys = array($key1, $key2);
        $status = $this->db->existsMany($keys, $metadata, array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if (empty($metadata)) {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
