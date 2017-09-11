<?php
class GrantRoles extends AerospikeTestCommon
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
     * Basic GrantRoles without parameter
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
    function testGrantRolesNoParameter()
    {
        try {
            return($this->db->grantRoles());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic GrantRoles positive
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
    function testGrantRolesPositive()
    {
        $this->db->grantRoles("example", array("read", "read-write"));
        sleep(2);
        $status = $this->db->queryUser("example", $roles);
        $result = array_diff_assoc_recursive($roles, array("read", "read-write"));
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic GrantRoles positive with policy
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
    function testGrantRolesPositivePolicy()
    {
        $this->db->grantRoles("example", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 1000));
        sleep(2);
        $status = $this->db->queryUser("example", $roles);
        $result = array_diff_assoc_recursive($roles, array("read", "read-write"));
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic GrantRoles positive with policy incorrect
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
    function testGrantRolesPolicyIncorrect()
    {
        return($this->db->grantRoles("example", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 0.5)));
    }
    /**
     * @test
     * Basic GrantRoles with NULL username
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
    function testGrantRolesUserIsNull()
    {
        return($this->db->grantRoles(NULL, array("read", "read-write")));
    }
    /**
     * @test
     * Basic GrantRoles non-existent user
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
    function testGrantRolesNonExistentUser()
    {
        return($this->db->grantRoles("abcdefg", array("read", "read-write")));
    }
    /**
     * @test
     * Basic GrantRoles empty roles
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
    function testGrantRolesEmptyRoles()
    {
        return($this->db->grantRoles("example", array()));
    }
    /**
     * @test
     * Basic GrantRoles invalid role list
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
    function testGrantRolesInvalidRoleList()
    {
        return($this->db->grantRoles("example", array("viewer")));
    }
}
?>
