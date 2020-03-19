<?php

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

 /**
 * Determine if two associative arrays are similar
 *
 * Both arrays must have the same indexes with identical values
 * without respect to key ordering 
 * 
 * @param array $a
 * @param array $b
 * @return bool
 */
function arrays_are_similar($a, $b) {
    // if the indexes don't match, return immediately
    if (count(array_diff_assoc($a, $b))) {
      return false;
    }
    // we know that the indexes, but maybe not values, match.
    // compare the values between the two arrays
    foreach($a as $k => $v) {
      if ($v !== $b[$k]) {
        return false;
      }
    }
    // we have identical indexes, and no unequal values
    return true;
  }

final class ListOperationsTest extends TestCase {
    protected $db;
    protected $keys1;
    protected $keys2;

    protected function setUp() : void
    {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $this->keys = [];
        $key = $this->db->initKey("test", "list_ops", "test_rec");
        $this->key= $key;
        $numListBin = array(1, 3, 2);
        $stringListBin = array("alpha", "charlie", "beta");
        $this->db->put($key, ["numList" => $numListBin, "stringList" => $stringListBin]);
        $this->test_rec = $stringListBin;
    }

    protected function tearDown() : void
    {
        $this->db->remove($this->key);
    }

    // START OF LIST OPS

    public function testListAppend() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_APPEND, "bin" => "stringList", "list_policy" => array(AEROSPIKE::OPT_LIST_WRITE_FLAGS => AEROSPIKE::AS_LIST_WRITE_DEFAULT), "val" => "delta"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha", "charlie", "beta", "delta"));
    }

    public function testListMerge() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_MERGE, "bin" => "numList", "list_policy" => array(), "val" => array(4, 5)]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 3, 2, 4, 5));
    }

    public function testListInsert() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_INSERT, "bin" => "stringList", "index" => 1, "list_policy" => array(), "val" => "delta"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha", "delta", "charlie", "beta"));
    }

    public function testListInsertItems() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_INSERT_ITEMS, "bin" => "stringList", "index" => 1, "list_policy" => array(), "val" => ["delta", "echo"]]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha", "delta", "echo", "charlie", "beta"));
    }

    public function testListPop() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_POP, "bin" => "numList", "index" => 1]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 2));
    }

    public function testListPopRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_POP_RANGE, "bin" => "stringList", "index" => 0, "val" => 2]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("beta"));
    }

    public function testListPopRangeFrom() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_POP_RANGE_FROM, "bin" => "stringList", "index" => 1]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha"));
    }

    public function testListRemove() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE, "bin" => "stringList", "index" => 1]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha", "beta"));
    }

    public function testListRemoveRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_RANGE, "bin" => "stringList", "index" => 0, "val" => 2]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("beta"));
    }

    public function testListRemoveByIndex() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_INDEX, "bin" => "stringList", "index" => 0, "return_type" => AEROSPIKE::AS_LIST_RETURN_NONE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("charlie", "beta"));
    }

    public function testListRemoveByIndexRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_INDEX_RANGE, "bin" => "stringList", "index" => 0, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_INDEX]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("stringList" => array(0, 1)));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("beta"));
    }

    public function testListRemoveByIndexRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_INDEX_RANGE_TO_END, "bin" => "stringList", "index" => 1, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("stringList" => array("charlie", "beta")));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["stringList"], array("alpha"));
    }

    public function testListRemoveByRank() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_RANK, "bin" => "numList", "rank" => 1, "return_type" => AEROSPIKE::AS_LIST_RETURN_RANK]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("numList" => 1));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 3));
    }

    public function testListRemoveByRankRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_RANK_RANGE, "bin" => "numList", "rank" => 1, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_COUNT]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("numList" => 2));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1));
    }

    public function testListRemoveByRankRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_RANK_RANGE_TO_END, "bin" => "numList", "rank" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE | AEROSPIKE::AS_LIST_RETURN_INVERTED]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("numList" => array(1, 2)));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(3));
    }

    public function testListRemoveByValue() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_VALUE, "bin" => "numList", "val" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_INDEX]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("numList" => array(2)));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 3));
    }

    public function testListRemoveByValueList() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_VALUE_LIST, "bin" => "numList", "val" => array(1, 3), "return_type" => AEROSPIKE::AS_LIST_RETURN_RANK]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($rec, array("numList" => array(0, 2)));
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(2));
    }

    public function testListRemoveByValueRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_VALUE_RANGE, "bin" => "numList", "range_begin" => 2, "range_end" => 5, "return_type" => AEROSPIKE::AS_LIST_RETURN_COUNT]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => 2));
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1));
    }

    public function testListRemoveByValueRelRankRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_VALUE_REL_RANK_RANGE, "bin" => "numList", "val" => 1, "rank" => 2, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(3)));
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 2));
    }

    public function testListRemoveByValueRelRankRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_BY_VALUE_REL_RANK_RANGE_TO_END, "bin" => "numList", "val" => 1, "rank" => 2, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(3)));
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 2));
    }

    public function testListRemoveRangeFrom() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_REMOVE_RANGE_FROM, "bin" => "numList", "index" => 1]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1));
    }

    public function testListClear() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_CLEAR, "bin" => "numList"]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array());
    }

    public function testListSet() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_SET, "bin" => "numList", "list_policy" => array(AEROSPIKE::OPT_LIST_WRITE_FLAGS => AEROSPIKE::AS_LIST_WRITE_DEFAULT), "index" => 2, "val" => 3 ]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->db->get($key, $record);
        $this->assertEquals($record["bins"]["numList"], array(1, 3, 3));
    }

    public function testListGet() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET, "bin" => "stringList", "index" => 2]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("stringList" => "beta"));
    }

    public function testListGetRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_RANGE, "bin" => "stringList", "index" => 0, "val" => 2]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("stringList" => array("alpha", "charlie")));
    }

    public function testListGetByIndex() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_INDEX, "bin" => "stringList", "index" => 0, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("stringList" => "alpha"));
    }

    public function testListGetByIndexRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_INDEX_RANGE, "bin" => "stringList", "index" => 0, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("stringList" => array("alpha", "charlie")));
    }

    public function testListGetByIndexRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_INDEX_RANGE_TO_END, "bin" => "stringList", "index" => 1, "return_type" => AEROSPIKE::AS_LIST_RETURN_INDEX]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("stringList" => array(1, 2)));
    }

    public function testListGetByRank() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_RANK, "bin" => "numList", "rank" => 1, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => 2));
    }

    public function testListGetByRankRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_RANK_RANGE, "bin" => "numList", "rank" => 1, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(2, 3)));
    }

    public function testListGetByRankRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_RANK_RANGE_TO_END, "bin" => "numList", "rank" => 1, "return_type" => AEROSPIKE::AS_LIST_RETURN_RANK]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(1, 2)));
    }

    public function testListGetByValue() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_VALUE, "bin" => "numList", "val" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(2)));
    }

    public function testListGetByValueList() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_VALUE_LIST, "bin" => "numList", "val" => array(2, 3), "return_type" => AEROSPIKE::AS_LIST_RETURN_INDEX]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(1, 2)));
    }

    public function testListGetByValueRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_VALUE_RANGE, "bin" => "numList", "range_begin" => 1, "range_end" => 3, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(1, 2)));
    }

    public function testListGetByValueRankRange() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_VALUE_REL_RANK_RANGE, "bin" => "numList", "val" => 2, "rank" => 0, "count" => 2, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(2, 3)));
    }

    public function testListGetByValueRankRangeToEnd() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_BY_VALUE_REL_RANK_RANGE_TO_END, "bin" => "numList", "val" => 2, "rank" => 0, "return_type" => AEROSPIKE::AS_LIST_RETURN_VALUE]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(2, 3)));
    }

    public function testListGetRangeFrom() {
        $key = $this->key;
        $ops = [["op" => AEROSPIKE::OP_LIST_GET_RANGE_FROM, "bin" => "numList", "index" => 1]];
        $status = $this->db->operate($key, $ops, $rec);
        $this->assertEquals($status, AEROSPIKE::OK);
        $this->assertEquals($rec, array("numList" => array(3, 2)));
    }
}
?>
