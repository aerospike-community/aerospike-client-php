<?php
class RevokePrivileges extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $privileges = array(array("code" => Aerospike::PRIV_SYS_ADMIN), array("code" => Aerospike::PRIV_USER_ADMIN));
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
     * Basic RevokePrivileges no parameter
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
    function testRevokePrivilegesNoParameter()
    {
        try {
            return($this->db->revokePrivileges());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic RevokePrivileges positive
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
    function testRevokePrivilegesPositive()
	{
		$status = $this->db->grantPrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ)));
		if($status == AEROSPIKE::OK) {
    		$this->db->queryRole("examples_role", $ret_privileges);
			$testprivileges = array(array("code" => 0), array("code" => 1), array("code" => 10));
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				$status = $this->db->revokePrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ)));
				if($status == AEROSPIKE::OK) {
    				$this->db->queryRole("examples_role", $ret_privileges);
					$testprivileges = array(array("code" => 0), array("code" => 1));
					$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
					if(empty($result)) {
						return AEROSPIKE::OK;
					} else {
						return AEROSPIKE::ERR_CLIENT;
					}
				}
			} else {
				return AEROSPIKE::ERR_CLIENT;
			}
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic RevokePrivileges positive policy
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
    function testRevokePrivilegesPositivePolicy()
	{
		sleep(2);
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
		$status = $this->db->grantPrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ)), $policy);
		sleep(1);
		if($status == AEROSPIKE::OK) {
    		$this->db->queryRole("examples_role", $ret_privileges);
            $testprivileges = array(array("code" => 0), array("code" => 1), array("code" => 10));
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				$status = $this->db->revokePrivileges("examples_role", array(array("code" => Aerospike::PRIV_READ)));
                if($status == AEROSPIKE::OK) {
    				$this->db->queryRole("examples_role", $ret_privileges);
					$testprivileges = array(array("code" => 0), array("code" => 1));
					$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
					if(empty($result)) {
						return AEROSPIKE::OK;
					} else {
						return AEROSPIKE::ERR_CLIENT;
					}
				}
			} else {
				return AEROSPIKE::ERR_CLIENT;
			}
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic RevokePrivileges incorrect role type
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
    function testRevokePrivilegesIncorrectRoleType()
    {
        try {
            return($this->db->revokePrivileges(["user"], array(array("code" => Aerospike::PRIV_READ))));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic RevokePrivileges unknown role name
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
    function testRevokePrivilegesUnknownRoleName()
    {
        try {
            return($this->db->revokePrivileges("examples_role_unknown", array(array("code" => Aerospike::PRIV_READ))));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic RevokePrivileges incorrect privilege type
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
    function testRevokePrivilegesIncorrectPrivilegeType()
    {
        try {
            return($this->db->revokePrivileges("examples_role", NULL));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic RevokePrivileges empty privilege list
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
    function testRevokePrivilegesEmptyPrivilegeList()
    {
        try {
            return($this->db->revokePrivileges("examples_role", array()));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
}
?>
