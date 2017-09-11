<?php
class QueryUser extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $res = $this->db->createUser("example", "example@123", array("read"));
    }

    protected function tearDown() {
        $res = $this->db->dropUser("example");
        $this->db->close();
    }
    /**
     * @test
     * Basic Query User without parameter
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
    function testQueryUserNoParameter()
    {
        try {
            return($this->db->queryUser());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic Query User positive
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
    function testQueryUserPositive()
    {
        $test = array("read");
        sleep(2);
        $this->db->queryUser("example", $roles);	
        $result = array_diff_assoc_recursive($test, $roles);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic Query User positive with policy
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
    function testQueryUserPositiveWithPolicy()
    {
        $test = array("read");
        sleep(2);
        $this->db->queryUser("example", $roles, array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        $result = array_diff_assoc_recursive($test, $roles);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic Query User with incorrect policy
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
    function testQueryUserIncorrectPolicy()
    {
        $test = array("read");
        $result = $this->db->queryUser("example", $roles, array("OPT_WRITE_TIMEOUT" => 0.5));
        $result = array_diff_assoc_recursive($test, $roles);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic Query User username is null
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
    function testQueryUserUserIsNull()
    {
        $test = array("read");
        $status = $this->db->queryUser(NULL, $roles);	
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        } else {
            $result = array_diff_assoc_recursive($test, $roles);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        }
    }
    /**
     * @test
     * Basic Query User non existent user
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
    function testQueryUserNonexistentUser()
    {
        $test = array("read");
        $status = $this->db->queryUser("abcasfdgasddas", $roles);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Basic Query User with no roles
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
    function testQueryUserNoRoles()
    {
        $test = array();
        if ($this->db->isConnected()) {
            $this->db->revokeRoles("example", array("read"));
            sleep(2);
            $this->db->queryUser("example", $roles);	
            $result = array_diff_assoc_recursive($test, $roles);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        }
    }
    /**
     * @test
     * Basic Query User with policy as string
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
    function testQueryUserStringPolicy()
    {
        $test = array("read");
        try {
            $this->db->queryUser("example", $roles, array("OPT_READ_TIMEOUT" => "incorrect"));	
            $result = array_diff_assoc_recursive($test, $roles);
            if(!empty($result)) {
                return Aerospike::ERR_CLIENT;
            } else {
                return Aerospike::OK;
            }
        } catch (ErrorExcpetion $e) {
            return $db->errorno();
        }
    }
    /**
     * @test
     * Basic Query User with extra parameter
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
    function testQueryUsesExtraParameter()
    {
        $test = array("read");
        try {
            $this->db->queryUser("example", $roles, array("OPT_READ_TIMEOUT" => 1000), "");
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
