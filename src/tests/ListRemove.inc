<?php
require_once 'Common.inc';

/**
 * List size operation
*/

class ListRemove extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "list_remove");
        $this->db->put($key,array("binA"=>100, "binB" => array("John", 15, array(25, 45),
            array("name"=>"Dan"), 45.8, True)));
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
     * List remove operation with all positive arguments
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
        $status = $this->db->listRemove($this->keys[0], "binB", 3,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"], array("John", 15,
            array(25, 45), 45.8, True));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List remove operation with index value is negative integer.
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
        $status = $this->db->listRemove($this->keys[0], "binB", -5,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array("John", array(25, 45),array("name"=>"Dan"), 45.8, True));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List remove operation with non-existent key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (error_001)
     *
     * @test_plans{1.1}
     */
    function error_001() {
        $key = $this->db->initKey("test", "demo", $this->generateRandomString());
        $status = $this->db->listRemove($key, "binB", 3);
        return $this->db->errorno();
    }
    
    /**
     * @test
     * List remove operation with non-existent bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (error_002)
     *
     * @test_plans{1.1}
     */
    function error_002() {
        $status = $this->db->listRemove($this->keys[0], $this->generateRandomString(), 3);
        return $status;
    }

    /**
     * @test
     * List remove operation with non-existent index
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (error_003)
     *
     * @test_plans{1.1}
     */
    function error_003() {
        $status = $this->db->listRemove($this->keys[0], "binB", 300);
        return $status;
    }

    /**
     * @test
     * List remove operation with key parameter is of type an integer.
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
    	try {
			$status = $this->db->listRemove(200, "binB", 3);
			return $this->db->errorno();
    	} catch(ErrorException $e) {
    		return $this->db->errorno();
    	}
    }
    
    /**
     * @test
     * List remove operation with bin is of type an array.
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
        try {
            $status = $this->db->listRemove($this->keys[0], array(599), 3);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List remove operation with index parameter is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_003)
     *
     * @test_plans{1.1}
     */
    function edge_003() {
        try {
            $status = $this->db->listRemove($this->keys[0], "binB", "abc");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List remove operation with options parameter is of type string.
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
        try {
            $status = $this->db->listRemove($this->keys[0], "binB", 3, "abcdef");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
}
?>
