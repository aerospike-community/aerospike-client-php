<?php

require_once 'Common.inc';

class QueryApply extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }

        $key = $this->db->initKey("test", "demo", "emp1");
        $this->db->put($key,array("name" => "john", "salary"=> 29000));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "emp2");
        $this->db->put($key,array("name" => "Dan", "salary"=> 55000));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "emp3");
        $this->db->put($key,array("name" => "Harry", "salary"=> 45000));
        $this->keys[] = $key;

        $this->ensureUdfModule("tests/lua/query_apply.lua", "query_apply.lua");
        $this->ensureIndex('test', 'demo', 'salary', 'demo_sal_idx', Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);
    }

    /**
     * @test
     * queryApply() with correct arguments
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
    function normal_001()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->jobInfo($job_id, Aerospike::JOB_QUERY, $job_info);
            if ($status != Aerospike::OK) {
                return Aerospike::ERR_CLIENT;
            }
        } while($job_info['status'] != Aerospike::JOB_STATUS_COMPLETED);

        $this->db->get($this->keys[1], $get_record1);
        $this->db->get($this->keys[2], $get_record2);
        if ($get_record1["bins"]["salary"] != 63000 && $get_record2["bins"]["salary"] != 53000) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * queryApply() with an options array
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
    function normal_002()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", array(8000),
            $job_id, array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->jobInfo($job_id, Aerospike::JOB_QUERY, $job_info);
            if ($status != Aerospike::OK) {
                return Aerospike::ERR_CLIENT;
            }
        } while($job_info['status'] != Aerospike::JOB_STATUS_COMPLETED);
        
        $this->db->get($this->keys[1], $get_record1);
        $this->db->get($this->keys[2], $get_record2);
        if ($get_record1["bins"]["salary"] != 63000 && $get_record2["bins"]["salary"] != 53000) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * queryApply() with job_id(out parameter) is passed of type string
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
    function normal_003()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $job_id = "str";
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->jobInfo($job_id, Aerospike::JOB_QUERY,$job_info);
            if ($status != Aerospike::OK) {
                return Aerospike::ERR_CLIENT;
            }
        } while($job_info['status'] != Aerospike::JOB_STATUS_COMPLETED);
        $this->db->get($this->keys[1], $get_record1);
        $this->db->get($this->keys[2], $get_record2);
        if ($get_record1["bins"]["salary"] != 63000 && $get_record2["bins"]["salary"] != 53000) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    
    /**
     * @test
     * queryApply() with non-existent namespace and set
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
    function error_001()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $status = $this->db->queryApply("t", "d", $where, "query_apply", "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * queryApply() without any parameters
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
    function edge_001()
    {
        try {
            $status = $this->db->queryApply();
        } catch (ErrorException $e) {
            return Aerospike::OK;
        }
        return $status;
    }
    
    /**
     * @test
     * queryApply() with namespace and set are of array type
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
    function edge_002()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        try {
        $status = $this->db->queryApply(["ns"], 20, $where, "query_apply", "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
        } catch (Exception $e) {
        	return $this->db->errorno();
        }
    }
    
    /**
     * @test
     * queryApply() with where parameter is of type integer
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
    function edge_003()
    {
    	try {
			$status = $this->db->queryApply("test", "demo", 56, "query_apply", "update_salary", array(8000), $job_id);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}
			return $status;
    	} catch(ErrorException $e) {
    		return $this->db->errorno();
    	}
    }
    
    /**
     * @test
     * queryApply() with module is of type array(expected string)
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
    function edge_004()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        try {
        $status = $this->db->queryApply("test", "demo", $where, ["Query_apply"], "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
        } catch (Exception $e) {
        	return $this->db->errorno();
        }
    }
    
    /**
     * @test
     * queryApply() with UDF name is of type array (expected string)
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
    function edge_005()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        try {
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", ["udf_name"], array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
        } catch(Exception $e) {
        	return $this->db->errorno();
        }
    }
    
    /**
     * @test
     * queryApply() with LUA arguments are not passed as an array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_006)
     *
     * @test_plans{1.1}
     */
    function edge_006()
    {
    	try {
			$where = $this->db->predicateBetween("salary", 40000, 60000);
			$status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", "aa", $job_id);
			if ($status != Aerospike::OK) {
				return $this->db->errorno();
			}
			return $status;
    	} catch(ErrorException $e) {
    		return $this->db->errorno();
    	}
    }
    
    /**
     * @test
     * queryApply() with options parameter is of type string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_007)
     *
     * @test_plans{1.1}
     */
    function edge_007()
    {
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", array("aa"),
            $job_id, "aa");
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
}
?>
