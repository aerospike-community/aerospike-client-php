<?php

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class MapOperationsTest extends TestCase {
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
        $this->db->put($key, ["map" => $bin]);
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => "map", "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops);
        $this->test_rec = $bin;
    }

    protected function tearDown() : void
    {
        $this->db->remove($this->key);
    }

    /* Positive tests */
    public function testSetMapPolicy() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SET_POLICY, "bin" => "map", "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops);
        $this->assertEquals($status, AEROSPIKE::OK);
    }

    /* Removes all elements from the map */
    public function testMapClear() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_CLEAR, "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $cleared_record);
        $this->assertEquals($cleared_record["bins"]["map"], []);
    }

    public function testMapSize() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_SIZE, "bin" => "map"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], 26);
    }

    public function testMapGetByKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_GET_BY_KEY, "key" => "r",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["r", "s"]);
    }

    public function testMapGetByKeyRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_KEY_RANGE,
                "key" => "b",
                "range_end" => "e",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["b", "c", "c", "d", "d", "e"]);
    }

    public function testMapGetByValue() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE,
                "val" => "a",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["z", "a"]);
    }

    public function testMapGetByValueRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_VALUE_RANGE,
                "val" => "b",
                "range_end" => "e",
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["a", "b", "c"]);
    }

    public function testMapGetByIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX,
                "index" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["c", "d"]);
    }

    public function testMapGetByIndexRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["c", "d", "d", "e"]);
    }

    public function testMapGetByRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK,
                "rank" => -1,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["y", "z"]);
    }

    public function testMapGetByRankRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_GET_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["x", "y", "y", "z"]);
    }

    public function testMapPut() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT,
                "key" => "aero",
                "val" => "spike",
                "bin" => "map",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);

        $this->db->get($key, $rec);
        $map = $rec["bins"]["map"];
        $correct = $this->test_rec;
        $correct["aero"] = "spike";
        $this->assertEquals($map, $correct);
    }

    public function testMapPutItems() {
        $key = $this->key;
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_PUT_ITEMS,
                "bin" => "map",
                "val" => $items,
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);

        $this->db->get($key, $rec);
        $map = $rec["bins"]["map"];
        $correct = $this->test_rec;
        $correct["aero"] = "spike";
        $correct[1] = 2;
        $this->assertEquals($map, $correct);
    }

    public function testMapIncrement() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_INCREMENT,
                "bin" => "ints",
                "val" => 10,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["ints"], 15);

        $this->db->get($key, $rec);
        $map = $rec["bins"]["ints"];
        $this->assertEquals($map["integer"], 15);
    }

    public function testMapDecrement() {
        $key = $this->key;
        $this->db->put($key, ["ints" => ["integer" =>5]]);
        $items = ["aero" => "spike", 1 => 2];
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_DECREMENT,
                "bin" => "ints",
                "val" => 3,
                "key" => "integer",
                "map_policy" => $this->map_policy]];
        $status = $this->db->operate($key, $ops, $rec);

        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["ints"], 2);

        $this->db->get($key, $rec);
        $map = $rec["bins"]["ints"];
        $this->assertEquals($map["integer"], 2);
    }

    public function testMapRemoveByKey() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY, "key" => "r",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec["map"], ["r", "s"]);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $rec);
        $correct = $this->test_rec;
        unset($correct["r"]);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByKeyList() {
        $key = $this->key;
        $rem_keys = ["a", "e", "o", "r"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_LIST, "key" => $rem_keys,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], $rem_keys);


        $this->db->get($key, $rec);
        $correct = $this->test_rec;

        unset($correct["a"]);
        unset($correct["e"]);
        unset($correct["r"]);
        unset($correct["o"]);

        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByKeyRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_KEY_RANGE, "key" => "e", "range_end" => "h", "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["e", "f", "g"]);

        $this->db->get($key, $rec);
        $correct = $this->test_rec;

        unset($correct["e"]);
        unset($correct["f"]);
        unset($correct["g"]);

        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByValue() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE, "val" => "s",  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec["map"], ["r", "s"]);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $rec);
        $correct = $this->test_rec;
        unset($correct["r"]);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByValueRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_RANGE, "val" => "f", "range_end" => "i", "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["e", "f", "g"]);

        $this->db->get($key, $rec);
        $correct = $this->test_rec;

        unset($correct["e"]);
        unset($correct["f"]);
        unset($correct["g"]);

        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByValueList() {
        $key = $this->key;
        $rem_vals = ["b", "f", "s", "p"];
        $ops = [["op" => AEROSPIKE::OP_MAP_REMOVE_BY_VALUE_LIST, "val" => $rem_vals,  "bin" => "map", "return_type" => AEROSPIKE::MAP_RETURN_KEY]];

        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["a", "e", "o", "r"]);


        $this->db->get($key, $rec);
        $correct = $this->test_rec;

        unset($correct["a"]);
        unset($correct["e"]);
        unset($correct["r"]);
        unset($correct["o"]);

        $this->assertEquals($rec["bins"]["map"], $correct);
    }


    public function testMapRemoveByIndex() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX,
                "index" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["c", "d"]);

        $correct = $this->test_rec;
        unset($correct["c"]);

        $this->db->get($key, $rec);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByIndexRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_INDEX_RANGE,
                "index" => 2,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["c", "d", "d", "e"]);

        $correct = $this->test_rec;
        unset($correct["c"]);
        unset($correct["d"]);

        $this->db->get($key, $rec);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByRank() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK,
                "rank" => -1,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["y", "z"]);

        $correct = $this->test_rec;
        unset($correct["y"]);

        $this->db->get($key, $rec);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }

    public function testMapRemoveByRankRange() {
        $key = $this->key;
        $ops = [
            [
                "op" => AEROSPIKE::OP_MAP_REMOVE_BY_RANK_RANGE,
                "rank" => -2,
                "count" => 2,
                "bin" => "map",
                "return_type" => AEROSPIKE::MAP_RETURN_KEY_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec["map"], ["x", "y", "y", "z"]);

        $correct = $this->test_rec;
        unset($correct["x"]);
        unset($correct["y"]);

        $this->db->get($key, $rec);
        $this->assertEquals($rec["bins"]["map"], $correct);
    }
}

?>
