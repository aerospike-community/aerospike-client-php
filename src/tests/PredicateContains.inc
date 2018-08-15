<?php

require_once 'Common.inc';
/**
 *Basic Get tests
 */

class PredicateContains extends AerospikeTestCommon
{
    private $bin;

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
		}
		$key = $this->db->initKey("test", "demo", "predicate_contains");
		$this->db->put($key,array("numeric_list"=>array(1, 2, 3, 4, 5)));
		$this->keys[] = $key;
		$this->db->put($key,array("string_list"=>array('a', 'b', 'c')));
		$this->keys[] = $key;
    }

    /**
     * @test
     * Basic Predicate Contains without parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsNoParameter()
    {
        try {
            $this->db->predicateContains();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic Predicate with value parameter is of type array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsValIsArray)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsValIsArray()
    {
		$predicate = $this->db->predicateContains('numeric_list', Aerospike::INDEX_TYPE_LIST, array());
		if (!$predicate) {
			return Aerospike::ERR_PARAM;
		}
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic Predicate with value parameter is NULL
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsValIsNULL)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsValIsNULL()
    {
		$predicate = $this->db->predicateContains('numeric_list', Aerospike::INDEX_TYPE_LIST, NULL);
		if (!$predicate) {
			return Aerospike::ERR_PARAM;
		}
        return Aerospike::OK;
	}
    /**
     * @test
     * Basic Predicate with value is of type integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsIntegerValue)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsIntegerValue()
    {
		$predicate = $this->db->predicateContains('numeric_list', Aerospike::INDEX_TYPE_LIST, 3);
		if (!$predicate) {
			return Aerospike::ERR_PARAM;
		}
        return Aerospike::OK;
	}
    /**
     * @test
     * Basic Predicate with value is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsStringValue)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsStringValue()
    {
		$predicate = $this->db->predicateContains('string_list', Aerospike::INDEX_TYPE_LIST, 'c');
		if (!$predicate) {
			return Aerospike::ERR_PARAM;
		}
        return Aerospike::OK;
	}
    /**
     * @test
     * Basic Predicate with index type is string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsIndexTypeString)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsIndexTypeString()
	{
		try {
			$predicate = $this->db->predicateContains('string_list', 'abcde', 'c');
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
	}
    /**
     * @test
     * Basic Predicate with bin name is of type integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateContainsBinNameInteger)
     *
     * @test_plans{1.1}
     */
    function testPredicateContainsBinNameInteger()
	{
		try {
			$predicate = $this->db->predicateContains(12, 'abcde', 'c');
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
	}
}
?>
