<?php
class CreateRole extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }

    protected function tearDown() {
        $this->db->close();
    }
    /**
     * @test
     * Basic CreateRole without parameter
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
    function testCreateRoleNoParameter()
    {
        try {
            return($this->db->createRole());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic CreateRole positive
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testCreateRolePositive()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
        $testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
        $status = $this->db->createRole("examples_role", $privileges);
		sleep(1);
		if($status == AEROSPIKE::OK ) {
			$status = $this->db->queryRole("examples_role", $ret_privileges);
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				$status = $this->db->createUser("testcreaterole", "createrole", array("examples_role"));
				sleep(1);
				if($status == AEROSPIKE::OK) {
					$status = $this->db->queryUser("testcreaterole", $ret_roles);
					$result = array_diff_assoc_recursive($ret_roles, array("examples_role"));
					if(empty($result)) {
						$this->db->dropRole("examples_role");
						sleep(1);
						$this->db->queryUser("testcreaterole", $ret_roles);
						$result = array_diff_assoc_recursive($ret_roles, array());
						if(empty($result)) {
							$this->db->dropUser("testcreaterole");
							return Aerospike::OK;
						} else {
							return Aerospike::ERR_CLIENT;
						}
					} else {
						return Aeropsike::ERR_CLIENT;
					}
				} else {
					return $status;
				}
			} else {
				return Aerospike::ERR_CLIENT;
			}
		} else {
			return $status;
		}
	}
    /**
     * @test
     * Basic CreateRole positive with policy
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testCreateRolePositivePolicy()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
        $status = $this->db->createRole("examples_role", $privileges, $policy);
		sleep(1);
		if($status == AEROSPIKE::OK ) {
			$status = $this->db->queryRole("examples_role", $ret_privileges);
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				$status = $this->db->createUser("testcreaterole", "createrole", array("examples_role"));
				sleep(1);
				if($status == AEROSPIKE::OK) {
					$status = $this->db->queryUser("testcreaterole", $ret_roles);
					$result = array_diff_assoc_recursive($ret_roles, array("examples_role"));
					if(empty($result)) {
						$this->db->dropRole("examples_role");
						sleep(1);
						$this->db->queryUser("testcreaterole", $ret_roles);
						$result = array_diff_assoc_recursive($ret_roles, array());
						if(empty($result)) {
							$this->db->dropUser("testcreaterole");
							return Aerospike::OK;
						} else {
							return Aerospike::ERR_CLIENT;
						}
					} else {
						return Aeropsike::ERR_CLIENT;
					}
				} else {
					return $status;
				}
			} else {
				return Aerospike::ERR_CLIENT;
			}
		} else {
			return $status;
		}
	}
    /**
     * @test
     * Basic CreateRole invalid role type
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
    function testCreateRoleInvalidRoleType()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
        try {
            return($this->db->createRole(["role_type"], $privileges));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic CreateRole invalid privileges type
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
    function testCreateRoleInvalidPrivilegesType()
    {
        try {
            return($this->db->createRole("examples_role", "invalidtype"));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic CreateRole existing role
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testCreateRoleExistingRole()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
        $status = $this->db->createRole("examples_role", $privileges, $policy);
		sleep(1);
		if($status == AEROSPIKE::OK) {
			$status = $this->db->createRole("examples_role", $privileges);
			$error = $this->db->errorno();
			$this->db->dropRole("examples_role");
			return($error);
		} else {
			return(Aerospike::ERR_CLIENT);
		}
	}
    /**
     * @test
     * Basic CreateRole positive with special characters in role_name
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testCreateRolePositiveSpecialCharactersRoleName()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
		$role_name = "!#Q#AEQ@#$%&^*((^&*~~~````";
        $status = $this->db->createRole($role_name, $privileges, $policy);
		sleep(1);
		if($status == AEROSPIKE::OK ) {
			$status = $this->db->queryRole($role_name, $ret_privileges);
			$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
			if(empty($result)) {
				$status = $this->db->createUser("testcreaterole", "createrole", array($role_name));
				sleep(1);
				if($status == AEROSPIKE::OK) {
					$status = $this->db->queryUser("testcreaterole", $ret_roles);
					$result = array_diff_assoc_recursive($ret_roles, array($role_name));
					if(empty($result)) {
						$this->db->dropRole($role_name);
						sleep(1);
						$this->db->queryUser("testcreaterole", $ret_roles);
						$result = array_diff_assoc_recursive($ret_roles, array());
						if(empty($result)) {
							$this->db->dropUser("testcreaterole");
							return Aerospike::OK;
						} else {
							return Aerospike::ERR_CLIENT;
						}
					} else {
						return Aeropsike::ERR_CLIENT;
					}
				} else {
					return $status;
				}
			} else {
				return Aerospike::ERR_CLIENT;
			}
		} else {
			return $status;
		}
	}
    /**
     * @test
     * Basic CreateRole too long role name
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testCreateRoleTooLongRoleName()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
		$role_name = "example";
		for($i = 0; $i< 1000; $i++) {
			$role_name = $role_name."example";
		}
		$status = $this->db->createRole($role_name, $privileges, $policy);
		return $this->db->errorno();
	}
}
?>
