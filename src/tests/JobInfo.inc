<?php

require_once 'Common.inc';

class JobInfo extends AerospikeTestCommon
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
        
        $where = $this->db->predicateBetween("salary", 40000, 60000);
        $status = $this->db->queryApply("test", "demo", $where, "query_apply", "update_salary", array(8000), $job_id);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->job_id = $job_id;
    }

    /**
     * @test
     * jobInfo() with correct arguments
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
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->jobInfo($this->job_id, Aerospike::JOB_QUERY, $job_info);
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
     * jobInfo() with an options array
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
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $status = $this->db->jobInfo($this->job_id, Aerospike::JOB_QUERY, $job_info, array(Aerospike::OPT_READ_TIMEOUT=>5822));
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
     * jobInfo() with job_info(out parameter) is passed of type string
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
        do {
            time_nanosleep(0, 5000000); // pause 5ms
            $job_info = "info";
            $status = $this->db->jobInfo($this->job_id, Aerospike::JOB_QUERY, $job_info);
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
     * jobInfo() with non-existent module
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
        $status = $this->db->jobInfo($this->job_id, Aerospike::JOB_QUERY, $job_info);
        if ($status != Aerospike::OK) {
            return Aerospike::ERR_PARAM;
        }
        return $this->db->errorno();
    }

}
?>
