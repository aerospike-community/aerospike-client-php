<?php

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class MapOperationsTest extends TestCase { 
    /*
        These are tests of incorrect usage for each map operation type.
    */
    protected $db;
    protected $keys1;
    protected $keys2;

    protected function setUp()
    {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
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

    protected function tearDown()
    {
        $this->db->remove($this->key);
    }

    /* Set Map Policy */
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

    /* Test Map Clear */
    public function testMapClearNonMapBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR, "bin" => "non_map"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapClearNonExistentBin() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR, "bin" => "nonrealbin"]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_BIN_INCOMPATIBLE_TYPE);
    }

    public function testMapClearNoBinEntry() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::ERR_PARAM);
    }

    /* Test Map Size */
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


    /* Test Map Get By Key */
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

    /* Should this be an error? */
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


    /* Test Map Get By Key Range */
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

/*  I'm right here right now */
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

    /*
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
        HERE
    */
    // public function testMapGetByValueRange() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
    //             "val" => "b",
    //             "range_end" => "e",
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["a", "b", "c"]);
    // }

    // public function testMapGetByIndex() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
    //             "index" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["c", "d"]);
    // }

    // public function testMapGetByIndexRange() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
    //             "index" => 2,
    //             "count" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["c", "d", "d", "e"]);
    // }

    // public function testMapGetByRank() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
    //             "rank" => -1,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["y", "z"]);
    // }

    // public function testMapGetByRankRange() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
    //             "rank" => -2,
    //             "count" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["x", "y", "y", "z"]);
    // }

    // public function testMapPut() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_PUT,
    //             "key" => "aero",
    //             "val" => "spike",
    //             "bin" => "map",
    //             "map_policy" => $this->map_policy]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);

    //     $this->db->get($key, $rec);
    //     $map = $rec["bins"]["map"];
    //     $correct = $this->test_rec;
    //     $correct["aero"] = "spike";
    //     $this->assertEquals($map, $correct);
    // }

    // public function testMapPutItems() {
    //     $key = $this->key;
    //     $items = ["aero" => "spike", 1 => 2];
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
    //             "bin" => "map",
    //             "val" => $items,
    //             "map_policy" => $this->map_policy]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);

    //     $this->db->get($key, $rec);
    //     $map = $rec["bins"]["map"];
    //     $correct = $this->test_rec;
    //     $correct["aero"] = "spike";
    //     $correct[1] = 2;
    //     $this->assertEquals($map, $correct);
    // }

    // public function testMapIncrement() {
    //     $key = $this->key;
    //     $this->db->put($key, ["ints" => ["integer" =>5]]);
    //     $items = ["aero" => "spike", 1 => 2];
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_INCREMENT,
    //             "bin" => "ints",
    //             "val" => 10,
    //             "key" => "integer",
    //             "map_policy" => $this->map_policy]];
    //     $status = $this->db->operate($key, $ops, $rec);

    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["ints"], 15);

    //     $this->db->get($key, $rec);
    //     $map = $rec["bins"]["ints"];
    //     $this->assertEquals($map["integer"], 15);
    // }

    // public function testMapDecrement() {
    //     $key = $this->key;
    //     $this->db->put($key, ["ints" => ["integer" =>5]]);
    //     $items = ["aero" => "spike", 1 => 2];
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_DECREMENT,
    //             "bin" => "ints",
    //             "val" => 3,
    //             "key" => "integer",
    //             "map_policy" => $this->map_policy]];
    //     $status = $this->db->operate($key, $ops, $rec);

    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["ints"], 2);

    //     $this->db->get($key, $rec);
    //     $map = $rec["bins"]["ints"];
    //     $this->assertEquals($map["integer"], 2);
    // }

    // public function testMapRemoveByKey() {
    //     $key = $this->key;
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($rec["map"], ["r", "s"]);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;
    //     unset($correct["r"]);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByKeyList() {
    //     $key = $this->key;
    //     $rem_keys = ["a", "e", "r", "o"];
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], $rem_keys);


    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;

    //     unset($correct["a"]);
    //     unset($correct["e"]);
    //     unset($correct["r"]);
    //     unset($correct["o"]);

    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByKeyRange() {
    //     $key = $this->key;
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE, "key" => "e", "range_end" => "h", "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["e", "f", "g"]);

    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;

    //     unset($correct["e"]);
    //     unset($correct["f"]);
    //     unset($correct["g"]);

    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByValue() {
    //     $key = $this->key;
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($rec["map"], ["r", "s"]);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;
    //     unset($correct["r"]);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByValueRange() {
    //     $key = $this->key;
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE, "val" => "f", "range_end" => "i", "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["e", "f", "g"]);

    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;

    //     unset($correct["e"]);
    //     unset($correct["f"]);
    //     unset($correct["g"]);

    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByValueList() {
    //     $key = $this->key;
    //     $rem_vals = ["b", "f", "s", "p"];
    //     $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["a", "e", "r", "o"]);


    //     $this->db->get($key, $rec);
    //     $correct = $this->test_rec;

    //     unset($correct["a"]);
    //     unset($correct["e"]);
    //     unset($correct["r"]);
    //     unset($correct["o"]);

    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }


    // public function testMapRemoveByIndex() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
    //             "index" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["c", "d"]);

    //     $correct = $this->test_rec;
    //     unset($correct["c"]);

    //     $this->db->get($key, $rec);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByIndexRange() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
    //             "index" => 2,
    //             "count" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["c", "d", "d", "e"]);

    //     $correct = $this->test_rec;
    //     unset($correct["c"]);
    //     unset($correct["d"]);

    //     $this->db->get($key, $rec);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByRank() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
    //             "rank" => -1,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["y", "z"]);

    //     $correct = $this->test_rec;
    //     unset($correct["y"]);

    //     $this->db->get($key, $rec);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }

    // public function testMapRemoveByRankRange() {
    //     $key = $this->key;
    //     $ops = [
    //         [
    //             "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
    //             "rank" => -2,
    //             "count" => 2,
    //             "bin" => "map",
    //             "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
    //     $status = $this->db->operate($key, $ops, $rec);
    //     $this->assertEquals($status, AEROSPIKE::OK);
    //     $this->assertEquals($rec["map"], ["x", "y", "y", "z"]);

    //     $correct = $this->test_rec;
    //     unset($correct["x"]);
    //     unset($correct["y"]);

    //     $this->db->get($key, $rec);
    //     $this->assertEquals($rec["bins"]["map"], $correct);
    // }
}

?>
