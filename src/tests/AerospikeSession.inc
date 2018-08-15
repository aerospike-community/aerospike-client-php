<?php

require_once("Common.inc");

class AerospikeSession extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        ini_set('session.save_handler', 'aerospike');
        session_save_path("test|sess|" . AEROSPIKE_CONFIG_NAME . ":" .  AEROSPIKE_CONFIG_PORT);
        session_id("testsession");
        session_start();
    }

    /**
     * @test
     * Basic Session write.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSessionRead)
     *
     * @test_plans{1.1}
     */
    function testSessionAWrite()
    {
        $_SESSION["username"] = "test-student";
        $_SESSION["age"] = 20;
        $grades = array("mathematics" => "AA",
            "data-structures" => "A",
            "operating-systems" => "AA");
        $_SESSION["grades"] = $grades;
        session_write_close();
        $key = $this->db->initKey("test", "sess", "testsession");
        $status = $this->db->exists($key, $metadata);
        return $status;
    }

    /**
     * @test
     * Basic Session read.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSessionWrite)
     *
     * @test_plans{1.1}
     */
    function testSessionBRead()
    {
        $grades = array("mathematics" => "AA",
            "data-structures" => "A",
            "operating-systems" => "AA");
        $same_grades = array_diff_assoc_recursive($_SESSION["grades"], $grades);
        if (isset($_SESSION["username"]) && strcmp($_SESSION["username"], "test-student") == 0 &&
            isset($_SESSION["age"]) && $_SESSION["age"] == 20 &&
            isset($_SESSION["grades"]) &&
            empty($same_grades)) {
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }

    /**
     * @test
     * Basic Session destroy.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSessionWrite)
     *
     * @test_plans{1.1}
     */
    function testSessionZDestroy()
    {
        session_destroy();
        $key = $this->db->initKey("test", "sess", "testsession");
        $status = $this->db->exists($key, $metadata);
        return $status;
    }

    /**
     * @test
     * Session read after destroy.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testSessionRead)
     *
     * @test_plans{1.1}
     */
    function testSessionZReadAfterDestroy()
    {
        if (empty($_SESSION)) {
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }

}
?>
