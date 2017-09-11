<?php
class SetPassword extends AerospikeTestCommon
{
    protected function setUp() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $res = $this->db->createUser("example", "example@123", array("read"));
    }

    protected function tearDown() {
        $res = $this->db->dropUser("example");
        $this->db->close();
    }
    /**
     * @test
     * Basic Set Password without parameter
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
    function testSetPasswordNoParameter()
    {
        try {
            return($this->db->setPassword());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic Set Password positive
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
    function testSetPasswordPositive()
    {
	    $res = $this->db->setPassword("example", "john@123");
	    if ($res == Aerospike::OK) {
    	    return Aerospike::OK;
        } else {
    	    return $this->db->error();
	    }
    }
    /**
     * @test
     * Basic Set Password correct timeout
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
    function testSetPasswordCorrectTimeout()
    {
	    $res = $this->db->setPassword("example", "john@123", array(Aerospike::OPT_WRITE_TIMEOUT => 20000));
	    if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
    	    return $this->db->errorno();
	    }
    }
    /**
     * @test
     * Basic Set Password incorrect timeout
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
    function testSetPasswordIncorrectTimeout()
    {
        $res = $this->db->setPassword("example", "john@123", array(Aerospike::OPT_WRITE_TIMEOUT => 0.2));
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic Set Password non-existent user
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
    function testSetPasswordNonExistentUser()
    {
        $res = $this->db->setPassword("abcdefgh", "john@123");
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic Set Password user as NULL
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
    function testSetPasswordNullUser()
    {
        $res = $this->db->setPassword(NULL, "example@123");
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic Set Password password as NULL
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
    function testSetPasswordNullPassword()
    {
        $res = $this->db->setPassword("example", NULL);
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic Set Password password is very long
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
    function testSetPasswordVeryLong()
    {
	    $pass = str_repeat("pass", 100);
        $res = $this->db->setPassword("example", $pass);
        if ($res == Aerospike::OK) {
	        return Aerospike::OK;
        } else {
		    return $this->db->errorno();
        }
    }
}
