<?php

require_once 'Common.inc';
/**
 *Basic Get tests
 */

class PredicateEquals extends AerospikeTestCommon
{
    private $bin;

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $this->bin = "age";
    }

    /**
     * @test
     * Basic Predicate Equals without parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsNoParameter()
    {
        try {
            $status = $this->db->predicateEquals();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return($status);
    }
    /**
     * @test
     * Basic Predicate value is Bool
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueIsBool()
    {
        try {
            $ans = $this->db->predicateEquals("age", true);
            if ($ans == NULL) {
                return Aerospike::OK;
            }
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Predicate Value is Array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueIsArray()
    {
        try {
            $ans = $this->db->predicateEquals("age", array("age", "email"));
            if ($ans == NULL) {
                return Aerospike::OK;
            }
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Predicate Value is same as bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueIsSame()
    {
        try {
            $res = $this->db->predicateEquals(1, 1);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic PredicateEquals has only one argument
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueHasOneArgument()
    {
        try {
            $this->db->predicateEquals("age");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic Predicate with correct arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPredicateEquals()
    {
        try {
            $ans = $this->db->predicateEquals("age", 30);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
        $op = array_diff_assoc_recursive($ans, array("bin"=>"age", "op"=>"=", "val"=>30));
        if(!empty($op)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
}
?>
