<?php

require_once 'Common.inc';
/**
 *Basic connection tests
*/

class Connection extends AerospikeTestCommon
{

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Basic Connection Test
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testConnect)
     *
     * @test_plans{1.1}
     */
    function testConnect() {
        $status = $this->db->errorno();
        return $status;
    }
    /**
     * @test
     * Config Argument Not An Array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testConfigNotArray)
     *
     * @test_plans{1.1}
     */
    function testConfigNotArray() {
        try {
            $config = "";
            $db = new Aerospike($config);
            return($db->errorno());
        } catch (Exception $e) {
            return Aerospike::ERR_PARAM;
        }
    }
    /**
     * @test
     * Config Argument Is An Empty Array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testEmptyArray)
     *
     * @test_plans{1.1}
     */
    function testEmptyArray() {
        $config = array("hosts"=>array(array("addr"=>"", "port"=>"")));
        try {
        $db = new Aerospike($config);
        return($db->errorno());
        }catch (Exception $e) {
        	return Aerospike::ERR_CLIENT;
        }
    }
    /**
     * @test
     * Config Array Missing The Host Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMissingHostKeyFromConfigArray)
     *
     * @test_plans{1.1}
     */
    function testMissingHostsKeyFromConfigArray() {
    	try {
			$config = array(array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
			$db = new Aerospike($config);
			return ($db->errorno());
    	} catch (Exception $e) {
    		return Aerospike::ERR_PARAM;
    	}
    }
    /**
     * @test
     * Config Array Missing Addr Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMissingAddrKeyFromConfigArray)
     *
     * @test_plans{1.1}
     */
    function testMissingAddrKeyFromConfigArray() {
        $config = array("hosts"=>array(array("0"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        try {
        $db = new Aerospike($config);
        return ($db->errorno());
        } catch (Exception $e) {
        	return Aerospike::ERR_PARAM;
        }
    }
    /**
     * @test
     * Config Array Missing Port Key
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMissingPortKeyFromConfigArray)
     *
     * @test_plans{1.1}
     */
    function testMissingPortKeyFromConfigArray() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "qw"=>AEROSPIKE_CONFIG_PORT)));
        try {
			$db = new Aerospike($config);
			return ($db->errorno());
        } catch(Exception $e) {
        	return Aerospike::ERR_PARAM;
        }
    }
    /**
     * @test
     * Multiple Hosts
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMultipleHostsArray)
     *
     * @test_plans{1.1}
     */
    function testMultipleHostsArray() {
        $config = array("hosts"=>array(
                    array("addr"=>AEROSPIKE_CONFIG_NAME,"port"=>AEROSPIKE_CONFIG_PORT),
                    array("addr"=>AEROSPIKE_CONFIG_NAME2,"port"=>AEROSPIKE_CONFIG_PORT2)));
        $db = new Aerospike($config);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Port value is string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     * The constructor should fail, return aerospike::ok if it does.
     * @remark
     * Variants: OO (testPortValueIsString)
     *
     * @test_plans{1.1}
     */
    function testPortValueIsString() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>"3000")));
        try {
			$db = new Aerospike($config);
			$status = $db->errorno();
			if($db->isConnected()) {
				$db->close();
				return $status;
			}
			return Aerospike::ERR_CLIENT;
        }catch(Exception $e) {
        	return Aerospike::OK;
        }
    }
    /**
     * @test
     * Test With Null Persistence Alias
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testNullAlias)
     *
     * @test_plans{1.1}
     */
    function testNullAlias() {
        $config = array("hosts"=>array(array("port"=>AEROSPIKE_CONFIG_PORT,"addr"=>AEROSPIKE_CONFIG_NAME)));
        $db = new Aerospike($config, NULL);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return $status;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Alias not passed and different config
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testNamePortSequenceInConfig)
     *
     * @test_plans{1.1}
     */
    function testNoAliasDiffrentConfig() {
        $test1 = false;
        $test2 = false;
        $config1 = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,"port"=>AEROSPIKE_CONFIG_PORT)));
        $db1 = new Aerospike($config1);
        $config2 = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME2,"port"=>AEROSPIKE_CONFIG_PORT2)));
        $db2 = new Aerospike($config2);
        if (!$db1->isConnected() || !$db2->isConnected()) {
            return Aerospike::OK;
        }
        if ($db1->isConnected()) {
            $test1 = true;
            $db1->close();
        }
        if ($db2->isConnected()) {
            $test2 = true;
            $db2->close();
        }
        if ($test1 && $test2) {
            return Aerospike::OK;
        } elseif (!$test1) {
            return $db1->errorno();
        } else {
            return $db2->errorno();
        }
    }


    /**
     * @test
     * Basic Connection
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testConnect)
     *
     * @test_plans{1.1}
     */
    function testConnectWithOption() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $opts = array(Aerospike::OPT_CONNECT_TIMEOUT => 1250,
            Aerospike::OPT_WRITE_TIMEOUT => 1500);
        $db = new Aerospike($config, NULL, $opts);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }

    function testConnectWithPersistFlagFalse() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, false);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }

    function testConnectWithPersistFlagPassedAsStr() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, "persist");
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }

    function testConnectWithPersistFlagPassedAsInt() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, 11111222);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }

    function testConnectWithPersistFlagTrue() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, true);
        $status = $db->errorno();
        if($db->isConnected()) {
        $db->close();
        return ($status);
        }
        return Aerospike::ERR_CLIENT;
    }

    function testConnectWithSameConfigPersistAsTrue() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $config1 = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME,
            "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, true);
        if (!$db->isConnected()) {
            $db->close();
            return Aerospike::ERR_CLIENT;
        }
        $db1 = new Aerospike($config1, true);
        if (!$db1->isConnected()) {
            $db1->close();
            return Aerospike::OK;
        }
        $db1->close();
        $db->close();
        return Aerospike::OK;
    }

    /**
     * @test
     * connect() with use_shm is disabled.
     * For this test-case to pass you need to set use_shm to false
     * through ini.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_001)
     *
     * @test_plans{1.1}
     */
    function normal_001() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config);
        if ($db->errorno() != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }
        $shared_memory_key = $db->shmKey();
        if ($shared_memory_key == NULL) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * connect() with use_shm enabled. shm_key is given by user.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_002)
     *
     * @test_plans{1.1}
     */
    function normal_002() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME2, "port"=>AEROSPIKE_CONFIG_PORT2)),
            "shm"=>array("shm_key"=>123));
        $db = new Aerospike($config);
        if ($db->errorno() != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }
        $shared_memory_key = $db->shmKey();
        if ($shared_memory_key == 123) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * connect() with use_shm enabled. shm_key is not passed by user.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_003)
     *
     * @test_plans{1.1}
     */
    function normal_003() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME2, "port"=>AEROSPIKE_CONFIG_PORT2)),
            "shm"=>array());
        $db = new Aerospike($config);
        if ($db->errorno() != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }
        $shared_memory_key = $db->shmKey();
        if ($shared_memory_key == 2768240641) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * connect() with use_shm enabled and shm_key is passed by user is already used.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_004)
     *
     * @test_plans{1.1}
     */
    function normal_004() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME2, "port"=>AEROSPIKE_CONFIG_PORT2)),
            "shm"=>array("shm_key"=>0xA5000000));
        $db = new Aerospike($config);
        if ($db->errorno() != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }
        $shared_memory_key = $db->shmKey();
        if ($shared_memory_key == 2768240641) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * connect() with COMPRESSION_THRESHOLD option passed.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (bug_CLIENT_627)
     *
     * @test_plans{1.1}
     */
    function bug_CLIENT_627() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $options = array(Aerospike::COMPRESSION_THRESHOLD=>8);
        $db = new Aerospike($config, true, $options);
        if ($db->errorno() != Aerospike::OK) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
} 
?>
