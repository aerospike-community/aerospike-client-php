<?php
class GrantPrivileges extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
        $status = $this->db->createRole("examples_role", $privileges);
        sleep(1);
    }

    protected function tearDown() {
        sleep(1);
		$this->db->dropRole("examples_role");
        $this->db->close();
	}
    /**
     * @test
     * Basic GrantPrivileges no parameter
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
    function testGrantPrivilegesNoParameter()
    {
        try {
            return($this->db->grantPrivileges());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic GrantPrivileges positive
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
    function testGrantPrivilegesPositive()
	{
		$status = $this->db->grantPrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ_WRITE_UDF)));
		sleep(1);
		if($status == AEROSPIKE::OK) {
    		$this->db->queryRole("examples_role", $ret_privileges);
			$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"), array("code" => 12));
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				return AEROSPIKE::OK;
			} else {
				return AEROSPIKE::ERR_CLIENT;
			}
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic GrantPrivileges positive policy
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
    function testGrantPrivilegesPositivePolicy()
	{
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
		$status = $this->db->grantPrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ_WRITE_UDF)), $policy);
		sleep(1);
		if($status == AEROSPIKE::OK) {
    		$this->db->queryRole("examples_role", $ret_privileges);
			$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"), array("code" => 12));
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				return AEROSPIKE::OK;
			} else {
				return AEROSPIKE::ERR_CLIENT;
			}
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic GrantPrivileges existing privilege
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
    function testGrantPrivilegesExistingPrivilege()
	{
		$status = $this->db->grantPrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ)));
		sleep(1);
		if($status == AEROSPIKE::OK) {
    		$this->db->queryRole("examples_role", $ret_privileges);
			$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				return AEROSPIKE::OK;
			} else {
				return AEROSPIKE::ERR_CLIENT;
			}
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic GrantPrivileges incorrect role type
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
    function testGrantPrivilegesIncorrectRoleType()
    {
        try {
            return($this->db->grantPrivileges(["user"], array(array("code" => Aerospike::PRIV_READ))));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic GrantPrivileges unknown role name
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
    function testGrantPrivilegesUnknownRoleName()
    {
        try {
            return($this->db->grantPrivileges("examples_role_unknown", array(array("code" => Aerospike::PRIV_READ))));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic GrantPrivileges incorrect privilege type
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
    function testGrantPrivilegesIncorrectPrivilegeType()
    {
        try {
            return($this->db->grantPrivileges("examples_role", NULL));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic GrantPrivileges empty privilege list
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
    function testGrantPrivilegesEmptyPrivilegeList()
    {
        try {
            return($this->db->grantPrivileges("examples_role", array()));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
}
?>
