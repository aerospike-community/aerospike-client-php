<?php
class DropUser extends AerospikeTestCommon
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
        $this->db->dropUser("examplephp");
        $this->db->close();
    }
    /**
     * @test
     * Basic DropUser without parameter
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
    function testDropUserNoParameter()
    {
        try {
            return($this->db->dropUser());
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic DropUser with user NULL
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
    function testDropUserUserIsNull()
    {
        $user = NULL;
        return ($this->db->dropUser($user));
    }
    /**
     * @test
     * Basic DropUser positive
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
    function testDropUserUserPositive()
    {
        $user = "examplephp";
        $this->db->createUser($user, "example123", array("read"));
        sleep(2);
        $this->db->dropUser($user);
        sleep(2);
        $this->db->queryUser($user, $roles);
        if(empty($roles))
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
        return($this->db->errorno());
    }
    /**
     * @test
     * Basic DropUser with non-existent user
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
    function testDropUserNonExistentUser()
    {
        $user = "abcdefgh";
        try {
            return($this->db->dropUser($user));
        } catch(ErrorException $e) {
            return($this->db->errorno());
        }
    }
    /**
     * @test
     * Basic DropUser positive with policy
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
    function testDropUserUserPositiveWithPolicy()
    {
        $user = "examplephp";
        $this->db->createUser($user, "example123", array("read"));
        sleep(2);
        $this->db->dropUser($user, array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        sleep(2);
        $this->db->queryUser($user, $roles);
        if(empty($roles))
            return Aerospike::OK;
        else
            return Aerospike::ERR_CLIENT;
        return($this->db->errorno());
    }
    /**
     * @test
     * Basic DropUser positive with policy
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
    function testDropUserUserPositiveWithIncorrectPolicy()
    {
        $user = "examplephp";
        $this->db->createUser($user, "example123", array("read"));
        sleep(2);
        return ($this->db->dropUser($user, array(Aerospike::OPT_WRITE_TIMEOUT => 0.5)));
    }
    /**
     * @test
     * Basic DropUser negative with extra argument
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
    function testDropUserUserExtraArgument()
    {
        $user = "examplephp";
        $this->db->createUser($user, "example123", array("read"));
        sleep(2);
        try {
            $this->db->dropUser($user, array(Aerospike::OPT_WRITE_TIMEOUT => 2000), "");
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic DropUser negative with policy null
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
    function testDropUserPolicyNull()
    {
        $user = "examplephp";
        $this->db->createUser($user, "example123", array("read"));
        sleep(2);
        try {
            $this->db->dropUser($user, NULL);
        } catch(ErrorException $e) {
            return $this->db->errorno();
        }
    }
}
?>
