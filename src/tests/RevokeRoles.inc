<?php
class RevokeRoles extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        $res = $this->db->createUser("example", "example@123", array("read","read-write","sys-admin"));
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
     * Basic RevokeRoles without parameter
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
    function testRevokeRolesNoParameter()
    {
        try {
            return($this->db->revokeRoles());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic RevokeRoles positive
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
    function testRevokeRolesPositive()
    {
        $status = $this->db->revokeRoles("example", array("read", "read-write"));
        sleep(2);
        $status = $this->db->queryUser("example", $roles);
        $result = array_diff_assoc_recursive($roles, array("sys-admin"));
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic RevokeRoles positive with policy
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
    function testRevokeRolesPositivePolicy()
    {
        $this->db->revokeRoles("example", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 1000));
        sleep(2);
        $this->db->queryUser("example", $roles);
        $result = array_diff_assoc_recursive($roles, array("sys-admin"));
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic RevokeRoles positive with policy incorrect
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
    function testRevokeRolesPolicyIncorrect()
    {
        return($this->db->revokeRoles("example", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 0.5)));
    }
    /**
     * @test
     * Basic RevokeRoles with NULL username
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
    function testRevokeRolesUserIsNull()
    {
        return($this->db->revokeRoles(NULL, array("read", "read-write")));
    }
    /**
     * @test
     * Basic RevokeRoles non-existent user
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
    function testRevokeRolesNonExistentUser()
    {
        return($this->db->revokeRoles("abcdefgh", array("read", "read-write")));
    }
    /**
     * @test
     * Basic RevokeRoles empty roles
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
    function testRevokeRolesEmptyRoles()
    {
        return($this->db->revokeRoles("example", array()));
    }
    /**
     * @test
     * Basic RevokeRoles all roles
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
    function testRevokeRolesAllRoles()
    {
        $this->db->revokeRoles("example", array("read", "read-write", "sys-admin"));
        sleep(2);
        $this->db->queryUser("example", $roles);
        $result = array_diff_assoc_recursive($roles, array());
        if(empty($result)) {
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }
    /**
     * @test
     * Basic RevokeRoles non-possessed role
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
    function testRevokeRolesNonpossessedRoles()
    {
        $res = $this->db->createUser("john-nonpossessed", "john@123", array("read","read-write"));
        sleep(2);
        $this->db->revokeRoles("john-nonpossessed", array("sys-admin"));
        sleep(2);
        $this->db->queryUser("john-nonpossessed", $roles);
        $result = array_diff_assoc_recursive($roles, array("read", "read-write"));
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        } else {
            $res = $this->db->dropUser("john-nonpossessed");
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic RevokeRoles invalid role list
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
    function testRevokeRolesInvalidRoleList()
    {
        return($this->db->revokeRoles("example", array("viewer")));
    }
    /**
     * @test
     * Basic RevokeRoles read role
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
    function testRevokeRolesReadRole()
    {
        $this->db->revokeRoles("example", array("read", "read-write"));
        sleep(2);
        $key = $this->db->initKey("test", "demo", 1);
        return ($this->db->put($key, array("age" => 1)));
    }
}
?>
