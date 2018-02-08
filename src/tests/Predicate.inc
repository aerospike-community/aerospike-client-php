<?php
require_once 'Common.inc';

/**
 * predicate helper functions tests
*/

class Predicate extends AerospikeTestCommon
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
     * predicateEquals helper should return a specific associative array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateEqualsHelper)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsHelper() {
        $predicate = $this->db->predicateEquals("a", "test");
        $this->assertArrayHasKey('op', $predicate, $msg = "Predicate missing 'op' key.");
        $this->assertArrayHasKey('bin', $predicate, $msg = "Predicate missing 'bin' key.");
        $this->assertArrayHasKey('val', $predicate, $msg = "Predicate missing 'val' key.");
        return Aerospike::OK;
    }

    /**
     * @test
     * Test predicateEquals with integer value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateEqualsValueInt)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueInt() {
        $predicate = $this->db->predicateEquals("a", 1);
        $this->assertInternalType('integer', $predicate['val'], "Expected integer value");
        $this->assertEquals(1, $predicate['val'], "Expected a value of 1");
        return Aerospike::OK;
    }

    /**
     * @test
     * Test predicateEquals with string value
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateEqualsValueString)
     *
     * @test_plans{1.1}
     */
    function testPredicateEqualsValueString() {
        $predicate = $this->db->predicateEquals("a", "1");
        $this->assertInternalType('string', $predicate['val'], "Expected integer value");
        $this->assertEquals("1", $predicate['val'], "Expected a value of '1'");
        return Aerospike::OK;
    }

    /**
     * @test
     * predicateBetween helper should return a specific associative array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateBetweenHelper)
     *
     * @test_plans{1.1}
     */
    function testPredicateBetweenHelper() {
        $predicate = $this->db->predicateBetween("c", 1, 10);
        $this->assertArrayHasKey('op', $predicate, $msg = "Predicate missing 'op' key.");
        $this->assertArrayHasKey('bin', $predicate, $msg = "Predicate missing 'bin' key.");
        $this->assertArrayHasKey('val', $predicate, $msg = "Predicate missing 'val' key.");
        return Aerospike::OK;
    }

    /**
     * @test
     * Test the predicateBetween value to be an array of (min, max) integers
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateBetweenArrayValueWithInts)
     *
     * @test_plans{1.1}
     */
    function testPredicateBetweenArrayValueWithInts() {
        $predicate = $this->db->predicateBetween("c", 1, 10);
        $this->assertInternalType('array', $predicate['val'], "Expected array value.");
        $this->assertEquals(array(1, 10), $predicate['val'], "Expected an array value of (1, 10)");
        return Aerospike::OK;
    }

    /**
     * @test
     * Test the predicateBetween value to be an array of (min, max) integers
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPredicateBetweenArrayValueWithStrings)
     *
     * @test_plans{1.1}
     */
    function testPredicateBetweenArrayValueWithStrings() {
        $predicate = $this->db->predicateBetween("c", "2", "24");
        $this->assertInternalType('array', $predicate['val'], "Expected array value.");
        $this->assertEquals(array(2, 24), $predicate['val'], "Expected an array value of (2, 24)");
        return Aerospike::OK;
    }

}
?>
