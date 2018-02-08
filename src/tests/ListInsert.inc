<?php
require_once 'Common.inc';

/**
 * List size operation
*/
class Employee
{
    public $desg = 'Manager';
}

class ListInsert extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "list_insert");
        $this->put_record = array("John", 15, array(25, 45), array("name"=>"Dan"), 45.8, True);
        $this->db->put($key,array("binA"=>100, "binB" => $this->put_record));
        $this->keys[] = $key;
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
     * Insert an integer to a list with all positive arguments
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
        $status = $this->db->listInsert($this->keys[0], "binB", 2, 123,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("John", 15, 123, array(25, 45), array("name"=>"Dan"), 45.8, True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * Insert string to a list with all positive arguments
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
        $status = $this->db->listInsert($this->keys[0], "binB", 0, "string",
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("string", "John", 15, array(25, 45), array("name"=>"Dan"),
            45.8, True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Insert float value to a list
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
        $status = $this->db->listInsert($this->keys[0], "binB", 6, 3.14);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("John", 15, array(25, 45), array("name"=>"Dan"),
            45.8, True, 3.14);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Insert boolean value to a list
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
        $status = $this->db->listInsert($this->keys[0], "binB", 0, True);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array(True, "John", 15, array(25, 45), array("name"=>"Dan"), 45.8, True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * Insert object value to a list
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_005)
     *
     * @test_plans{1.1}
     */
    function normal_005() {
        $eobj = new Employee();
        $status = $this->db->listInsert($this->keys[0], "binB", 2, $eobj);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("John", 15, $eobj, array(25, 45), array("name"=>"Dan"), 45.8, True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Insert list to a list with all positive arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_006)
     *
     * @test_plans{1.1}
     */
    function normal_006() {
        $status = $this->db->listInsert($this->keys[0], "binB", 5, array("Harry", 50000, 12.3),
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("John", 15, array(25, 45), array("name"=>"Dan"), 45.8,
            array("Harry", 50000, 12.3), True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * Insert map to a list with all positive arguments
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_007)
     *
     * @test_plans{1.1}
     */
    function normal_007() {
        $status = $this->db->listInsert($this->keys[0], "binB", 6, array("city"=>"Pune"),
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array("John", 15, array(25, 45), array("name"=>"Dan"), 45.8,
            True, array("city"=>"Pune"));
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Insert empty list to a list
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_008)
     *
     * @test_plans{1.1}
     */
    function normal_008() {
        $value = array();
        $status = $this->db->listInsert($this->keys[0], "binB", 0, $value);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record, array("binB"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $out_array = array(array(), "John", 15, array(25, 45), array("name"=>"Dan"),
            45.8, True);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * Insert value with non-existent bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_009)
     *
     * @test_plans{1.1}
     */
    function normal_009() {
        $bin = $this->generateRandomString();
        $value = array("Aerospike", "Casandra", "Redis");
        $status = $this->db->listInsert($this->keys[0], $bin, 0, $value);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        $status = $this->db->get($this->keys[0], $get_record, array($bin));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        $out_array = array($value);
        $comp_res = array_diff_assoc_recursive($get_record["bins"][$bin], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * Insert value with non-existent key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_010)
     *
     * @test_plans{1.1}
     */
    function normal_010() {
        $value = array("Aerospike", "Casandra", "Redis");
        $key = $this->db->initKey("test", "demo", $this->generateRandomString());
        $this->keys[] = $key;
        $status = $this->db->listInsert($key, "binC", 0, $value);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        $status = $this->db->get($key, $get_record, array("binC"));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        $out_array = array($value);
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binC"], $out_array);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List Insert operation with key parameter is of type an integer.
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
        $value = array("Aerospike", "Casandra", "Redis");
        try {
			$status = $this->db->listInsert(200, "binB", 0, $value);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}
			return Aerospike::ERR_CLIENT;
        } catch(ErrorException $e) {
        	return $this->db->errorno();
        }
    }
    
    /**
     * @test
     * List Insert operation with bin is of type an array.
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
        $value = array("Aerospike", "Casandra", "Redis");
        try {
            $status = $this->db->listInsert($this->keys[0], array(599),
                0, $value);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List Insert operation with options parameter is of type string.
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
    function edge_003() {
        $value = array("Aerospike", "Casandra", "Redis");
        try {
            $status = $this->db->listInsert($this->keys[0], "binB", 0,
                $value, "abcdef");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
}
?>
