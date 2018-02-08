<?php
require_once 'Common.inc';

/**
 * List size operation
*/

class ListRemoveRange extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "list_remrange");
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
     * List remove range  operation with all positive arguments
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
        $status = $this->db->listRemoveRange($this->keys[0], "binB", 3, 3,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array("John", 15, array(25, 45)));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * List remove range operation with index value is negative integer.
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
        $status = $this->db->listRemoveRange($this->keys[0], "binB", -5, 2,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($this->keys[0], $get_record);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($get_record["bins"]["binB"],
            array("John", array("name"=>"Dan"), 45.8, True));
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * List remove range operation with non-existent key
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
        $status = $this->db->listRemoveRange($key, "binB", 0, 3);
        return $this->db->errorno();
    }
    
    /**
     * @test
     * List remove range operation with non-existent bin
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
        $status = $this->db->listRemoveRange($this->keys[0], $this->generateRandomString(),
            0, 3);
        return $status;
    }

    /**
     * @test
     * List remove range operation with non-existent index
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
        $status = $this->db->listRemoveRange($this->keys[0], "binB", 300, 2);
        return $status;
    }

    /**
     * @test
     * List remove range operation with key parameter is of type an integer.
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
			$status = $this->db->listRemoveRange(200, "binB", 0, 3);
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
     * List pop remove operation with bin is of type an array.
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
            $status = $this->db->listRemoveRange($this->keys[0], array(599), 0, 3);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List remove range operation with index parameter is of type string.
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
            $status = $this->db->listRemoveRange($this->keys[0], "binB", "abc", 3);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
    
    /**
     * @test
     * List remove range operation with count parameter is of type string.
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
            $status = $this->db->listRemoveRange($this->keys[0], "binB", 0, "abc");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * List remove range operation with options parameter is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_005)
     *
     * @test_plans{1.1}
     */
    function edge_005() {
        try {
            $status = $this->db->listRemoveRange($this->keys[0], "binB", 0, 3, "abcdef");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }
}
?>
