<?php
require_once 'Common.inc';

class OperateOrdered extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }

    public function generateRandomString($length = 10) {
        $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        $charactersLength = strlen($characters);
        $randomString = '';
        for ($i = 0; $i < $length; $i++) {
            $randomString .= $characters[rand(0, $charactersLength - 1)];
        }
        return $randomString;
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with policy options.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_001)
     *
     * @test_plans{1.1}
     */
    function normal_001() {
        $key = $this->db->initKey("test", "demo", "pk458");
        $this->keys[] = $key;
        $operations = array(
            array("op" => Aerospike::OP_LIST_APPEND, "bin" => "age", "val"=>49),
            array("op" => Aerospike::OP_LIST_GET, "bin" => "age", "index"=>0)
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        if ($status != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }

        $exp_out = array(array("age", 1), array("age", 49));
        $diff = array_diff_assoc_recursive($returned, $exp_out);
		if (!empty($comp_res)) {
			return Aerospike::ERR_CLIENT;
		}

        return $status;
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with returned(out parameter) contains string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_002)
     *
     * @test_plans{1.1}
     */
    function normal_002() {
        $key = $this->db->initKey("test", "demo", "pk458");
        $this->keys[] = $key;
        $operations = array(
            array("op" => Aerospike::OP_LIST_APPEND, "bin" => "age", "val"=>"name"),
            array("op" => Aerospike::OP_LIST_GET, "bin" => "age", "index"=>0)
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        if ($status != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }

        $exp_out = array(array("age", 1), array("age", "name"));
        $diff = array_diff_assoc_recursive($returned, $exp_out);
		if (!empty($comp_res)) {
			return Aerospike::ERR_CLIENT;
		}
        return $status;
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with OP_LIST_POP and OP_LIST_GET on the same bin.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_003)
     *
     * @test_plans{1.1}
     */
    function normal_003() {
        $key = $this->db->initKey("test", "demo", "pk458");
        $this->keys[] = $key;
        $operations = array(
            array("op" => Aerospike::OP_LIST_APPEND, "bin" => "age", "val"=>49),
            array("op" => Aerospike::OP_LIST_GET, "bin" => "age", "index"=>0),
            array("op" => Aerospike::OP_LIST_POP, "bin" => "age", "index"=>0)
        );
        $returned = "output value";
        $status = $this->db->operateOrdered($key, $operations, $returned);

        $exp_out = array(array("age", 1), array("age", 49), array("age", 49));
        $diff = array_diff_assoc_recursive($returned, $exp_out);

        if ($status != Aerospike::OK) {
        	return $status;
        }
        if (empty($diff)) {
        	return Aerospike::OK;
        } else {
        	return Aerospike::ERR_CLIENT;
        }
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with OPERATOR_READ on non-existent bin.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_004)
     *
     * @test_plans{1.1}
     */
    function normal_004() {
        $key = $this->db->initKey("test", "demo", "pk458");
        $this->keys[] = $key;
        $bin = $this->generateRandomString();
        $operations = array(
            array("op" => Aerospike::OPERATOR_WRITE, "bin" => "write_bin", "val" => 49),
            array("op" => Aerospike::OPERATOR_READ, "bin" => $bin)
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        if ($status != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }

        $exp_out = array(array("write_bin", NULL), array($bin, NULL));
        $diff = array_diff_assoc_recursive($returned, $exp_out);
        if (empty($diff)) {
        	return Aerospike::OK;
        }

        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with both write and read operations.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_004)
     *
     * @test_plans{1.1}
     */
    function normal_005() {
        $key = $this->db->initKey("test", "demo", "pk458");
        $this->keys[] = $key;
        $bin = $this->generateRandomString();
        $operations = array(
            array("op" => Aerospike::OPERATOR_WRITE, "bin" => "write_bin", "val" => 49),
            array("op" => Aerospike::OP_LIST_APPEND, "bin" => "append_bin", "val" => 125),
            array("op" => Aerospike::OP_LIST_INSERT, "bin" => "append_bin", "index" => 3, "val" => 500),
            array("op" => Aerospike::OP_LIST_GET_RANGE, "bin" => "append_bin", "index" => 0, "val" => 4)
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        if ($status != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }

        //list append and list_insert return list size after insert
        $exp_out = array(array("append_bin", 1), array("append_bin", 4),
            array("append_bin", array(125, NULL, NULL, 500)));
        $diff = array_diff_assoc_recursive($returned, $exp_out);

        if (empty($diff)) {
        	return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Positive scenario : operateOrdered() with read and delete operations.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_004)
     *
     * @test_plans{1.1}
     */
    function normal_006() {
        $key = $this->db->initKey("test", "demo", "pk459");
        $this->keys[] = $key;
        $status = $this->db->put($key, ["for_del_bin" => "testVal"]);
        if ($status != Aerospike::OK)
            return $status;
        $operations = array(
            array("op" => Aerospike::OPERATOR_READ, "bin" => "for_del_bin"),
            array("op" => Aerospike::OPERATOR_DELETE),
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        if ($status != Aerospike::OK) {
            return $status;
        }

        if (!$returned || $returned[0][0] != "for_del_bin" || $returned[0][1] != "testVal") {
            return Aerospike::ERR_CLIENT;
        }

        $status = $this->db->get($key, $rec);
        if ($status == Aerospike::ERR_RECORD_NOT_FOUND)
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Negative scenario : operateOrdered() with string value is passed in place of an keys array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_001)
     *
     * @test_plans{1.1}
     */
    function edge_001() {
        $key = $this->db->initKey("test", "demo", "pk1");
        $operations = array(
            array("op" => Aerospike::OPERATOR_WRITE, "bin" => "age", "val" => 55),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "age")
        );
        try {
			$status = $this->db->operateOrdered("random", $operations, $returned);
			if ($status != Aerospike::OK) {
				return Aerospike::ERR_PARAM;
			}
			return $status;
        } catch (Exception $e) {
        	return $this->db->errorno();
        }
    }

    /**
     * @test
     * Negative scenario : operateOrdered() with an integer value is passed in place of an operations array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_002)
     *
     * @test_plans{1.1}
     */
    function edge_002() {
        $key = $this->db->initKey("test", "demo", "pk1");
        try {
            $status = $this->db->operateOrdered($key, 100, $returned);
        } catch (Exception $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * Negative scenario : operateOrdered() with boolean value is passed in place of an options array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_002)
     *
     * @test_plans{1.1}
     */
    function edge_003() {
        $key = $this->db->initKey("test", "demo", "pk1");
        $operations = array(
            array("op" => Aerospike::OPERATOR_WRITE, "bin" => "age", "val" => 55),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "age")
        );
        try {
            $status = $this->db->operateOrdered($key, $operations, $returned, True);
        } catch (Exception $e){
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /*
     * This test case should return an error. As val parameter is missing for OPERATOR_WRITE
     */
    /**
     * @test
     * Negative scenario : operateOrdered() with val parameter is missing for OPERATOR_WRITE.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_004)
     *
     * @test_plans{1.1}
     */
    function edge_004() {
        $key = $this->db->initKey("test", "demo", "pk1");
        $operations = array(
            array("op" => Aerospike::OPERATOR_WRITE, "bin" => "age"),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "age")
        );
        $status = $this->db->operateOrdered($key, $operations, $returned);
        return $status;
    }

}
?>

