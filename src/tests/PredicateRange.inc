<?php

require_once 'Common.inc';
/**
 *Basic Get tests
 */

class PredicateRange extends AerospikeTestCommon
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
     * Variants: OO (testPredicateRangeNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeNoParameter()
    {
        try {
            $this->db->PredicateRange();
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
     * Variants: OO (testPredicateRangeMinValIsArray)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeMinValIsArray()
    {
    	try{
			$predicate = $this->db->PredicateRange('numeric_list',
				Aerospike::INDEX_TYPE_LIST, array(), 25);
			if (!$predicate) {
				return Aerospike::ERR_PARAM;
			}
			return Aerospike::OK;
    	}catch (Exception $e) {
    		return Aerospike::ERR_PARAM;
    	}
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
     * Variants: OO (testPredicateRangeMaxValIsNULL)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeMaxValIsArray()
    {
    	try {
		$predicate = $this->db->PredicateRange('numeric_list', Aerospike::INDEX_TYPE_LIST, 2, []);
			if (!$predicate) {
				return Aerospike::ERR_PARAM;
			}
			return Aerospike::OK;
    	}catch (Exception $e) {
    		return Aerospike::ERR_PARAM;
    	}
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
     * Variants: OO (testPredicateRangeIntegerValue)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeIntegerValue()
    {
		$predicate = $this->db->PredicateRange('numeric_list', Aerospike::INDEX_TYPE_LIST, 1, 4);
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
     * Variants: OO (testPredicateRangeStringValue)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeStringValue()
    {
    	try {
			$predicate = $this->db->PredicateRange('string_list', Aerospike::INDEX_TYPE_LIST, 'b', 'd');
			if (!$predicate) {
				return Aerospike::ERR_PARAM;
			}
			return Aerospike::OK;
    	} catch (Exception $e) {
    		return Aerospike::ERR_PARAM;
    	}
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
     * Variants: OO (testPredicateRangeIndexTypeString)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeIndexTypeString()
	{
		try {
			$predicate = $this->db->PredicateRange('string_list', 'abcde', 'c', 'd');
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
     * Variants: OO (testPredicateRangeBinNameInteger)
     *
     * @test_plans{1.1}
     */
    function testPredicateRangeBinNameInteger()
	{
		try {
			$predicate = $this->db->PredicateRange(12, 'abcde', 'c', 'd');
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
	}
}
?>
