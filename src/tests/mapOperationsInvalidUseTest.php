<?php

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class MapOperationsInvalidUseTest extends TestCase {
    /*
        These are tests of incorrect usage for each map operation type.
    */
    protected $db;
    protected $keys1;
    protected $keys2;

    protected function setUp() : void
    {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $this->keys = [];
        $this->map_policy = [ AEROSPIKE::OPT_MAP_ORDER => AEROSPIKE::AS_MAP_KEY_ORDERED];
        $key = $this->db->initKey("test", "map_ops", "test_rec");
        $this->key= $key;
        $alpha = "abcdefghijklmnopqrstuvwxyz";
        $bin = [];
        for ($i = 0; $i < 26; $i++) {
            $bin[$alpha[$i]] = $alpha[($i + 1) % 26];
        }
        $this->db->put($key, ["map" => $bin, "non_map"=>5]);
        $this->test_rec = $bin;
        // Give some time for any clock differences to resolve.
    }

    protected function tearDown() : void
    {
        $this->db->remove($this->key);
    }

    /* Tests for:
    OP_MAP_SET_POLICY
    */
    public function testSetMapPolicyNonArray() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => "map", "map_policy" => "honesty is the best policy"]];
        $status = $this->db->operate($key, $ops);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testSetMapPolicyNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => "non_map", "map_policy" => "honesty is the best policy"]];
        $status = $this->db->operate($key, $ops);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testSetMapPolicyNoPolicy() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => "map"]];
        $status = $this->db->operate($key, $ops);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testSetMapPolicyInvalidEntry() {
        $invalid_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ordered-by-keys"];
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => $invalid_policy]];
        $status = $this->db->operate($key, $ops);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /* Tests for:
    OP_MAP_CLEAR
    */
    public function testMapClearNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR, "bin" => "non_map"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapClearNonExistentBin() {
        /* For server greater than 3.15.2 this is OK */
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR, "bin" => "nonrealbin"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertTrue(($status == AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE) || ($status == AEROSPIKE::OK));
    }

    public function testMapClearNoBinEntry() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    Tests for OP_MAP_SIZE
    */
    public function testMapSizeNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SIZE, "bin" => "non_map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /* LOOK INTO THIS */
    public function testMapSizeNonExistentBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SIZE, "bin" => "nonrealbin"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, []);
    }


    /*
    * Tests for OP_MAP_GET_BY_KEY_RANGE
    */
    public function testMapGetByKeyNoBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "r", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyNoKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyNoReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "r", "bin" => "map"]];
         $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "r",  "bin" => "non_map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByKeyNonExistentBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "r",  "bin" => "nonrealbin", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, []);
    }

    public function testMapGetByKeyNonExistentKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "fake_key",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], []);
    }

    /* as_list is a valid key in an as_map */
    public function testMapGetByKeyListKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => ["k1", "k2"],  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], []);
    }

    /* Works because serializer works. */
    public function testMapGetByKeyNULLKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => NULL,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], []);
    }

    public function testMapGetByKeyInvalidReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => NULL,  "bin" => "map", "return_type" => "nothing"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }


    /*
    * Tests for OP_MAP_GET_BY_KEY_RANGE
    */
    public function testMapGetByKeyRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "b",
                "range_end" => "e",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyRangeNoKey() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "range_end" => "e",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyRangeNoRangeEnd() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "c",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "c",
                "range_end" => "e",
                "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByKeyRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "c",
                "range_end" => "e",
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }


    public function testMapGetByKeyRangeInvalidReturn() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "c",
                "range_end" => "e",
                "bin" => "non_map",
                "return_type" => "return everything"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    Tests for: AEROSPIKE::OP_MAP_GET_BY_VALUE
    */
    public function testMapGetByValueNobin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "val" => "a",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByValueNoVal() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);

    }

    public function testMapGetByValueNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "val" => "a",
                "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByValueNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "val" => "a",
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByValueInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "val" => "a",
                "bin" => "non_map",
                "return_type" => "Nothing"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
        Tests for:
        OP_MAP_GET_BY_VALUE_RANGE
    */
    public function testMapGetByValueRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "range_end" => "e",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByValueRangeNoVal() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "range_end" => "e",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByValueRangeNoRangeEnd() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByValueRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "range_end" => "e",
                "bin" => "map"
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /* Look into this */
    public function testMapGetByValueRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "range_end" => "e",
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByValueRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "range_end" => "e",
                "bin" => "non_map",
                "return_type" => "Nothing"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }
    /*
    Tests for:
    OP_MAP_GET_BY_INDEX
    */
    public function testMapGetByIndexNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexNoIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => 2,
                "bin" => "map"
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }


    public function testMapGetByIndexNonNumericIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => "a",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => 1,
                "bin" => "map",
                "return_type" => "nothing"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }
    /*
    Tests for: OP_MAP_GET_BY_INDEX_RANGE
    */

    public function testMapGetByIndexRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNoIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNoCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByIndexRangeNonIntIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => "two",
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNonIntCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => "two",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeNegativeCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => -2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByIndexRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => -2,
                "bin" => "map",
                "return_type" => "Something"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    tests for: OP_MAP_GET_BY_RANK
    */

    public function testMapGetByRankNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => -1,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankNoRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => -1,
                "bin" => "map",
        ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => -1,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByRankNonIntRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => "last",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => -1,
                "bin" => "map",
                "return_type" => "keys"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    * Tests for: OP_MAP_GET_BY_RANK_RANGE
    */
    public function testMapGetByRankRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeNoRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeNoCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "map",
        ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapGetByRankRangeNonIntRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => "last",
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeNonIntCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => "two",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapGetByRankRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => "two",
                "bin" => "map",
                "return_type" => "Everything"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    Test for: OP_MAP_PUT
    */
    public function testMapPutNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutNoKey() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "val" => "spike",
                "bin" => "map",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutNoVal() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "bin" => "map",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutNoMapPolicy() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "bin" => "non_map",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapPutInvalidMapPolicyEntry() {
        $bad_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ORDERED"];
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "bin" => "map",
                "map_policy" => $bad_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutNonArrayPolicy() {
        $bad_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ORDERED"];
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "bin" => "map",
                "map_policy" => "honesty is the best policy"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
        Test for OP_MAP_PUT_ITEMS
    */

    public function testMapPutItemsNoBin() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "val" => $items,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutItemsNoVal() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutItemsNoMapPolicy() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "val" => $items
        ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutItemsNonHashTableItems() {
        $key = $this->key;
        $items = ["aero", "spike"];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "val" => $items,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutItemsNonMapBin() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "non_map",
                "val" => $items,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapPutItemsNonArrayPolicy() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "val" => $items,
                "map_policy" => "honesty is the best policy"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapPutItemsInvalidPolicyEntries() {
        $key = $this->key;
        $bad_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ORDERED"];

        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "val" => $items,
                "map_policy" => $bad_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    Tests for: OP_MAP_INCREMENT
    */

    public function testMapIncrementNoBin() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "val" => 10,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapIncrementNoVal() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapIncrementNoKey() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => 10,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapIncrementNoMapPolicy() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapIncrementNonMapBin() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "non_map",
                "val" => 10,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapIncrementNonIntegerMapItem() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "map",
                "val" => 10,
                "key" => "a",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertNull($rec);
        $this->assertNotEquals($status, AEROSPIKE::OK);
    }

    public function testMapIncrementNonIntegerVal() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => "Ten",
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        /*Currently this is ERR_REQUEST_INVALID */
        $this->assertNull($rec);
        $this->assertNotEquals($status, AEROSPIKE::OK);
    }

    public function testMapIncrementNonArrayPolicy() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer",
                "map_policy" => "policy"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapIncrementInvalidPolicyEntry() {
        $bad_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ORDERED"];
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer",
                "map_policy" => $bad_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    TESTS FOR OP_MAP_DECREMENT
    */

    public function testMapDecrementNoBin() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "val" => 10,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapDecrementNoVal() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapDecrementNoKey() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => 10,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapDecrementNoMapPolicy() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapDecrementNonMapBin() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "non_map",
                "val" => 10,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapDecrementNonIntegerMapItem() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "map",
                "val" => 10,
                "key" => "a",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertNull($rec);
        $this->assertNotEquals($status, AEROSPIKE::OK);
    }

    public function testMapDecrementNonIntegerVal() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => "Ten",
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        /*Currently this is ERR_REQUEST_INVALID */
        $this->assertNull($rec);
        $this->assertNotEquals($status, AEROSPIKE::OK);
    }

    public function testMapDecrementtNonArrayPolicy() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer",
                "map_policy" => "policy"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapDecrementInvalidPolicyEntry() {
        $bad_policy = [ AEROSPIKE::OPT_MAP_ORDER => "ORDERED"];
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer",
                "map_policy" => $bad_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    * Tests for: OP_MAP_REMOVE_BY_KEY
    */

    public function testMapRemoveByKeyNoBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyNoKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyNoReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r",  "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyInvalidReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r",  "bin" => "map", "return_type" => "return_type"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r",  "bin" => "non_map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    * Tests for: OP_MAP_REMOVE_BY_KEY_LIST
    */
    public function testMapRemoveByKeyListNoBin() {
        $key = $this->key;
        $rem_keys = ["a", "e", "r", "o"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys, "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListNoKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListNoReturnType() {
        $key = $this->key;
        $rem_keys = ["a", "e", "r", "o"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "map"]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListInvalidReturnType() {
        $key = $this->key;
        $rem_keys = ["a", "e", "r", "o"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "map", "return_type" => "return_type"]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListNonListKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => "list_of_keys?",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListHashTableKey() {
        $key = $this->key;
        $rem_keys = ["a" => "e", "r" => "o"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyListNonMapBin() {
        $key = $this->key;
        $rem_keys = ["a", "e", "r", "o"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "non_map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }


    /*
    ** Tests for: OP_MAP_REMOVE_BY_KEY_RANGE
    */
    public function testMapRemoveByKeyRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "key" => "e", "range_end" => "h",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyRangeNoKey() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "range_end" => "h",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyRangeNoRangeEnd() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "key" => "e",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "key" => "e", "range_end" => "h",
                "bin" => "map",
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByKeyRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "key" => "e", "range_end" => "h",
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapRemoveByKeyRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE,
                "key" => "e", "range_end" => "h",
                "bin" => "map",
                "return_type" => "return type"]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_VALUE
    */

    public function testMapRemoveByValueNoBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueNoVal() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueInvalidReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s",  "bin" => "map", "return_type" => "return type"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueNoReturnType() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s",  "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s",  "bin" => "non_map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_VALUE_RANGE
    */

    public function testMapRemoveByValueRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "val" => "f",
                "range_end" => "i",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueRangeNoVal() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "range_end" => "i",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueRangeNoRangeEnd() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "val" => "f",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "val" => "f",
                "range_end" => "i",
                "bin" => "map",
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "val" => "f",
                "range_end" => "i",
                "bin" => "map",
                "return_type" => "return type"
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE,
                "val" => "f",
                "range_end" => "i",
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY
            ]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_VALUE_LIST
    */

    public function testMapRemoveByValueListNoBin() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals, "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListNoVal() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListNoReturnType() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals,  "bin" => "map"]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListNonArrayVal() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => "some values",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListHashTableVals() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => ["a"=>"e","r"=>"o"],  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListInvalidReturnType() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals,  "bin" => "map", "return_type" => "return_type"]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByValueListNonMapBin() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals,  "bin" => "non_map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }


    /*
    ** Tests for: OP_MAP_REMOVE_BY_INDEX
    */

    public function testMapRemoveByIndexNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexNoIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => 2,
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexNonIntegerIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => "two",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => 2,
                "bin" => "map",
                "return_type" => "return some stuff"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_INDEX_RANGE
    */

    public function testMapRemoveByIndexRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNoIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNoCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNonIntegerIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => "two",
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNonIntegerCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => "Two",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNegativeCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => -2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "map",
                "return_type" => "return_type"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByIndexRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_RANK
    */

    public function testMapRemoveByRankNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => -1,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankNoRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => -1,
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankNonIntegerRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => "Last",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => -1,
                "bin" => "map",
                "return_type" => "return"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => -1,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    /*
    ** Tests for: OP_MAP_REMOVE_BY_RANK_RANGE
    */

    public function testMapRemoveByRankRangeNoBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNoRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNoCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNoReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "map",
            ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNonIntegerCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => "two",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNonIntegerRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => "two",
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNegativeCount() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => -2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeInvalidReturnType() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "map",
                "return_type" => "return type"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    public function testMapRemoveByRankRangeNonMapBin() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "non_map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }
}

?>
