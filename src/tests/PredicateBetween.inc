<?php

require_once 'Common.inc';
/**
 *Basic Get tests
 */

class PredicateBetween extends AerospikeTestCommon
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
     * Basic Predicate Between without parameter
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
    function testPredicateBetweenNoParameter()
    {
        try {
            $this->db->predicateBetween();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
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
    function testPredicateBetweenValueIsBool()
    {
        try {
            $ans = $this->db->predicateBetween($this->bin, true, true);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        $op = array_diff_assoc_recursive($ans, array("bin"=>"age", "op"=>"BETWEEN", "val"=>array(1, 1)));
        if(!empty($op)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
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
    function testPredicateBetweenValueIsArray()
    {
        try {
            $this->db->predicateBetween($this->bin, array("age", "email"), array(2, 3));
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic PredicateBetween has only one argument
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
    function testPredicateBetweenValueHasOneArgument()
    {
        try {
            $this->db->predicateBetween($this->bin);
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
    function testPredicateBetween()
    {
        try {
            $ans = $this->db->predicateBetween($this->bin, 30, 40);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        $op = array_diff_assoc_recursive($ans, array("bin"=>"age", "op"=>"BETWEEN", "val"=>array(30, 40)));
        if(!empty($op)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
}
?>
