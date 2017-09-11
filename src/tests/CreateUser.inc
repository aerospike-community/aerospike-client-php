<?php
class CreateUser extends AerospikeTestCommon
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
        $this->db->dropUser("createuser-example");
        $this->db->dropUser("createuser2");
        $this->db->close();
    }
    /**
     * @test
     * Basic CreateUser without parameter
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
    function testCreateUserNoParameter()
    {
        try {
            return($this->db->createUser());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic CreateUser positive
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
    function testCreateUserPositive()
    {
        $test = array("read", "read-write");
        $this->db->createUser("createuser-example", "example123", array("read", "read-write"));
        sleep(2);
        $this->db->queryUser("createuser-example", $roles);
        $this->db->dropUser("createuser-example");
        $result = array_diff_assoc_recursive($test, $roles);
        if(empty($result)) 
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic CreateUser positive with policy
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
    function testCreateUserPositivePolicy()
    {
        $test = array("read", "read-write");
        $this->db->createUser("createuser-example", "example123", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        sleep(2);
        $this->db->queryUser("createuser-example", $roles);
        $this->db->dropUser("createuser-example");
        $result = array_diff_assoc_recursive($test, $roles);
        if(empty($result)) 
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic CreateUser positive with policy incorrect
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
    function testCreateUserPolicyIncorrect()
    {
        $test = array("read", "read-write");
        return($this->db->createUser("createuser-example", "example123", array("read", "read-write"), array(Aerospike::OPT_WRITE_TIMEOUT => 0.5)));
    }
    /**
     * @test
     * Basic CreateUser with password as NULL
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
    function testCreateUserPasswordNull()
    {
        $pass = NULL;
        return ($this->db->createUser("createuser-example", $pass, array("read", "read-write")));
    }
    /**
     * @test
     * Basic CreateUser with username as NULL
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
    function testCreateUserNull()
    {
        $user = NULL;
        return ($this->db->createUser($user, "example123", array("read", "read-write")));
    }
    /**
     * @test
     * Basic CreateUser with roles as NULL
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
    function testCreateUserRolesNull()
    {
        $roles = NULL;
        try {
            return ($this->db->createUser("createuser-example", "example123", $roles));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic CreateUser with roles as empty
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
    function testCreateUserRolesEmpty()
    {
        return $this->db->createUser("createuser-example", "example123", array());
    }
    /**
     * @test
     * Basic CreateUser already existing user
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
    function testCreateUserExisting()
    {
    	$this->db->createUser("createuser-example", "example123", array("read", "read-write"));
        return ($this->db->createUser("createuser-example", "example123", array("read", "read-write")));
    }
    /**
     * @test
     * Basic CreateUser already invalid role
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
    function testCreateUserInvalidRole()
    {
        return ($this->db->createUser("createuser-example", "example123", array("invalid")));
    }
    /**
     * @test
     * Basic CreateUser non-held role
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
    function testCreateUserUnderPrivileged()
    {
        $this->db->createUser("createuser-example", "example123", array("read"));
        sleep(2);
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = ["hosts"=> [$host], "user"=>"createuser-example", "pass"=>"example123"];
        $user_db = new Aerospike($config, false);
        $key = $this->db->initKey("test", "demo", 1);
        $status =  $user_db->put($key, array("age" => 1));
        $user_db->close();
        return $status;
    }
    /**
     * @test
     * Basic CreateUser multiple users with same password
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
    function testCreateUserMultiple()
    {
        $test = array("read", "read-write");
        $this->db->createUser("createuser-example", "example123", array("read", "read-write"));
        sleep(2);
        $this->db->queryUser("createuser-example", $roles);
        $result = $this->db->createUser("createuser2", "example123", array("read", "read-write"));
        sleep(2);
        $this->db->queryUser("createuser2", $roles);
        $this->db->dropUser("createuser2");
        $this->db->dropUser("createuser-example");
        $result = array_diff_assoc_recursive($test, $roles);
        if(empty($result)) 
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic CreateUser with very long username
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
    function testCreateUserLongUsername()
    {
        $user = str_repeat("user", 100);

        $test = array("read", "read-write");
        $res = $this->db->createUser($user, "example123", $test);
        if ($res == Aerospike::OK) {
        	$this->db->dropUser($user);
        	return Aerospike::OK;
        }
        return $res;
    }
    /**
     * @test
     * Basic CreateUser with very special characters in username
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
    function testCreateUserSpecialCharacterUsername()
    {
        $user = "!#Q#AEQ@#$%&^*((^&*~~~````";
        $test = array("read", "read-write");
        $this->db->createUser($user, "example123", array("read", "read-write"));
        if ($this->db->errorno() != 0) {
            sleep(2);
            $this->db->queryUser($user, $roles);
            if ($this->db->errorno() != 0) {
                $this->db->dropUser($user);
                $result = array_diff_assoc_recursive($test, $roles);
                if(empty($result)) 
                    return Aerospike::OK;
                else
                    return Aerospike::ERR_CLIENT;
            }
        }
        return $this->db->errorno();
    }
}
?>
