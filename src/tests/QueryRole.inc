<?php
class QueryRole extends AerospikeTestCommon
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
     * Basic QueryRole without parameter
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
    function testQueryRoleNoParameter()
    {
        try {
            return($this->db->queryRole());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic QueryRole positive
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
    function testQueryRolePositive()
	{
    	$this->db->queryRole("examples_role", $ret_privileges);
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
		if(empty($result)) {
			return AEROSPIKE::OK;
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic QueryRole positive with policy
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
    function testQueryRolePositivePolicy()
    {
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
    	$this->db->queryRole("examples_role", $ret_privileges, $policy);
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$result = array_diff_assoc_recursive($testprivileges, $ret_privileges);
		if(empty($result)) {
			return AEROSPIKE::OK;
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic QueryRole incorrect role name
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
    function testQueryRoleIncorrectRoleName()
    {
        try {
            return($this->db->queryRole("examples_role_incorrect", $roles));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic QueryRole incorrect role type
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
    function testQueryRoleIncorrectRoleType()
    {
        try {
            return($this->db->queryRole(["role_name"], $roles));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
}
?>
