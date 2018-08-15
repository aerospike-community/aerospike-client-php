<?php
require_once 'Common.inc';
/**

 *Basic Aggregate tests

*/
class ScanInfo extends AerospikeTestCommon
{
    private $myscan_id = 0;

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $this->ensureUdfModule("tests/lua/test_transform.lua", "test_transform.lua");
        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key,array("first_name"=>"john", "age"=>29));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key1");
        $this->db->put($key,array("first_name"=>"smith", "age"=>27));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "Get_key2");
        $this->db->put($key,array("first_name"=>"adam", "age"=>22));
        $this->keys[] = $key;
	    $key = $this->db->initKey("test", "demo", "Get_key3");
        $this->db->put($key,array("first_name"=>"ellie", "age"=>32));
        $this->keys[] = $key;
    }
	/**
	 * @test
     * ScanInfo - All correct arguments
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoPositive()
    {
        $info = array();
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->myscan_id = $scan_id;
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($this->myscan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        if ($info['progress_pct'] != 100) {
            return $this->db->errorno();
        }
        return $status;
    }
	/**
	 * @test
     * ScanInfo - Scan id is string
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoScanIdString()
    {
        try {
            $status = $this->db->scanInfo("", $info);
        } catch (Exception $e) {
            return $this->db->errorno();
        }
    }
	/**
	 * @test
     * ScanInfo - Scan id is array
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoScanIdArray()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->myscan_id = $scan_id;
        try {
            $status = $this->db->scanInfo(array($this->myscan_id), $info);
        } catch (Exception $e) {
              return $this->db->errorno();
        }
    }
	/**
	 * @test
     * ScanInfo - Scan id is negative
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoScanIdNegative()
    {
        $status = $this->db->scanInfo(-1, $info);
	    if($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
	/**
	 * @test
     * ScanInfo - Info parameter is string
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoInfoIsString()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->myscan_id = $scan_id;
        $scan_info = "age";
        $status = $this->db->scanInfo($this->myscan_id, $scan_info);
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($this->myscan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        if ($info['progress_pct'] != 100) {
            return $this->db->errorno();
        }
        return $status;
    }
	/**
	 * @test
     * ScanInfo - All correct arguments with optionals
	 *
	 * @pre
	 * Connect using aerospike object to the specified node
	 *
	 * @post
	 * newly initialized Aerospike objects
	 *
	 * @remark
	 * Variants: OO (testAggregatePositive)
	 *
	 * @test_plans{1.1}
	 */
	function testScanInfoWithOptionals()
    {
        $status = $this->db->scanApply("test", "demo", "test_transform", "mytransform", array(20), $scan_id);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->myscan_id = $scan_id;
        $status = $this->db->scanInfo($this->myscan_id, $info, array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        $info = array();
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->scanInfo($this->myscan_id, $info);
            if ($status != Aerospike::OK) {
                return $this->db->errorno();
            }
        } while($info['status'] != Aerospike::SCAN_STATUS_COMPLETED);
        if ($info['progress_pct'] != 100) {
            return $this->db->errorno();
        }
        return $status;
    }
}
?>
