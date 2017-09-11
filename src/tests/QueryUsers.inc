<?php
class QueryUsers extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        $res = $this->db->createUser("example", "example@123", array("read"));
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }

    protected function tearDown() {
        $res = $this->db->dropUser("example");
        $this->db->close();
    }
    /**
     * @test
     * Basic Query Users without parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersNoParameter()
    {
        try {
            return($this->db->queryUsers());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic Query Users positive
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersPositive()
    {
        $test = array("read");
        sleep(2);
        $this->db->queryUsers($roles);
        $res = $roles['example'];
        $result = array_diff_assoc_recursive($test, $res);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic Query Users positive with policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersPositiveWithPolicy()
    {
        sleep(2);
        $test = array("read");
        $this->db->queryUsers($roles, array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        $result = array_diff_assoc_recursive($test, $roles['example']);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic Query Users with incorrect policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersIncorrectPolicy()
    {
        $test = array("read");
        if ($this->db->isConnected()) {
            $result = $this->db->queryUsers($roles, array(Aerospike::OPT_WRITE_TIMEOUT => 0.5));
            return $result;
            $result = array_diff_assoc_recursive($test, $roles);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        }
        else {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic Query Users roles is null
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersRolesIsNull()
    {
        sleep(2);
        $test = array("read");
        $roles = NULL;
        $result = $this->db->queryUsers($roles);
        if ($result != Aerospike::OK) {
            return $this->db->errorno();
        } else {
            $result = array_diff_assoc_recursive($test, $roles['example']);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        }
    }
    /**
     * @test
     * Basic Query Users with policy as string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersStringPolicy()
    {
        $test = array("read");
        try {
            return($this->db->queryUser("example", $roles, array(Aerospike::OPT_WRITE_TIMEOUT => "incorrect")));
        } catch (ErrorExcpetion $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic Query Users with extra parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testQueryUsersExtraParameter()
    {
        $test = array("read");
            try {
            $this->db->queryUsers($roles, array("OPT_READ_TIMEOUT" => 1000), "");
            $result = array_diff_assoc_recursive($test, $roles);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        } catch(ErrorException $e) {
            return $this->db->errorno();
        }
    }
}
