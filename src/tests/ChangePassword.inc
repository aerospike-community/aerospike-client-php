<?php
class ChangePassword extends AerospikeTestCommon
{
    static $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
    protected function setUp() {
        $config = array("hosts"=>array(self::$host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $res = $this->db->createUser("testchangepassworduser", "aerospike", array("read"));
    }

    protected function tearDown() {
        $res = $this->db->dropUser("testchangepassworduser");
        $this->db->close();
    }
    /**
     * @test
     * Basic Change Password without parameter
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
    function testChangePasswordNoParameter()
    {
        try {
            return($this->db->changePassword());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic Change Password positive
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
    function testChangePasswordPositive()
    {
        $user = "testchangepassworduser";
        $res = $this->db->changePassword($user, "john@123");
        if ($res == Aerospike::OK) {
            $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"john@123");
            $new_client = new Aerospike($config, false);
            if ($new_client->isConnected()) {
                return AEROSPIKE::OK;
            } else {
                return AEROSPIKE::ERR_CLIENT;
            }
        } else {
    	    return $this->db->errorno();
	    }
    }
    /**
     * @test
     * Basic Change Password correct timeout
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
    function testChangePasswordCorrectTimeout()
    {
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword($user, "john@123", array(Aerospike::OPT_WRITE_TIMEOUT => 20000));
	    if ($res == Aerospike::OK) {
            $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
            $wrongconnection = new Aerospike($config, false);
            if (!$wrongconnection->isConnected()) {
                $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"john@123");
                $rightconnection = new Aerospike($config);
                if ($rightconnection->isConnected()) {
                    $rightconnection->close();
                    $connection->close();
                    return AEROSPIKE::OK;
                }
            } else {
                return AEROSPIKE::ERR_CLIENT;
            }
        } else {
    	    return $connection->errorno();
	    }
    }
    /**
     * @test
     * Basic Change Password incorrect timeout
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
    function testChangePasswordIncorrectTimeout()
    {
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword($user, "john123", array(Aerospike::OPT_WRITE_TIMEOUT => 0.2));
        if ($res == Aerospike::OK) {
            return Aerospike::OK;
        } else {
            return $connection->errorno();
        }
    }
    /**
     * @test
     * Basic Change Password non-existent user
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
    function testChangePasswordNonExistentUser()
    {
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword("abcdefgh", "john@123");
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $connection->errorno();
        }
    }
    /**
     * @test
     * Basic Change Password user as NULL
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
    function testChangePasswordNullUser()
    {
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword(NULL, "example@123");
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $connection->errorno();
        }
    }
    /**
     * @test
     * Basic Change Password password as NULL
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
    function testChangePasswordNullPassword()
    {
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword("example", NULL);
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $connection->errorno();
        }
    }
    /**
     * @test
     * Basic Change Password password is very long
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
    function testChangePasswordVeryLong()
    {
	    $pass = "pass";
	    for ($i=1; $i<10; $i++) {
		    $pass = $pass + "pass";
	    }
        $user = "testchangepassworduser";
        $config = array("hosts"=>array(self::$host), "user"=>$user, "pass"=>"aerospike");
        $connection = new Aerospike($config, false);
        $res = $connection->changePassword($user, $pass);
        if ($res == Aerospike::OK) {
            return Aerospike::OK;
        } else {
    	    return $connection->errorno();
	    }
    }
}
