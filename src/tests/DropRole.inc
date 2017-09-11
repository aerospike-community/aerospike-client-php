<?php
class DropRole extends AerospikeTestCommon
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
     * Basic DropRole without parameter
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
    function testDropRoleNoParameter()
    {
        try {
            return($this->db->dropRole());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic DropRole positive
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
    function testDropRolePositive()
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
     * Basic DropRole positive with policy
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
    function testDropRolePositivePolicy()
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
						$this->db->dropRole("examples_role", $policy);
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
     * Basic DropRole non-existing rolename
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
    function testDropRoleNonExisting()
    {
        try {
            return($this->db->dropRole("examples_role2"));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic DropRole role name NULL
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
    function testDropRoleNull()
    {
        try {
            return($this->db->dropRole(NULL));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic DropRole incorrect policy
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
    function testDropRoleIncorrectPolicy()
    {
        $privileges = array(array("code" => Aerospike::PRIV_READ), array("code" => Aerospike::PRIV_READ_WRITE, "ns" => "test", "set" => "demo"));
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
        $status = $this->db->createRole("examples_role", $privileges, $policy);
		sleep(1);
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 0.5);
		$this->db->dropRole("examples_role", $policy);
		$error = $this->db->errorno();
		$this->db->dropRole("examples_role");
		return($error);
	}
}
?>
