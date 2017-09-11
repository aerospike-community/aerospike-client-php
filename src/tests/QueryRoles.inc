<?php
class QueryRoles extends AerospikeTestCommon
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
     * Basic QueryRoles positive
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
    function testQueryRolesPositive()
	{
    	$this->db->queryRoles($ret_privileges);
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$result = array_diff_assoc_recursive($testprivileges, $ret_privileges['examples_role']);
		if(empty($result)) {
			return AEROSPIKE::OK;
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic QueryRoles positive with policy
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
    function testQueryRolesPositivePolicy()
    {
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 2000);
    	$this->db->queryRoles($ret_privileges, $policy);
		$testprivileges = array(array("code" => 10), array("code" => 11, "ns" => "test", "set" => "demo"));
		$result = array_diff_assoc_recursive($testprivileges, $ret_privileges['examples_role']);
		if(empty($result)) {
			return AEROSPIKE::OK;
		} else {
			return AEROSPIKE::ERR_CLIENT;
		}
	}
    /**
     * @test
     * Basic QueryRoles with incorrect policy
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
    function testQueryRolesIncorrectPolicy()
    {
		$policy = array(Aerospike::OPT_WRITE_TIMEOUT => 0.5);
    	return($this->db->queryRoles($ret_privileges, $policy));
	}
}
?>
