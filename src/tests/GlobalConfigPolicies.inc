<?php
ini_set('aerospike.key_policy', '1');
require_once 'Common.inc';
/**
 *Basic Put tests
*/

class GlobalConfigPolicies extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config, true, array(Aerospike::OPT_POLICY_KEY => Aerospike::POLICY_KEY_SEND,
            Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_CREATE,
            Aerospike::OPT_WRITE_TIMEOUT=>8900));
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $this->ensureUdfModule("tests/lua/test_record_udf.lua", "module.lua");
    }

    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Method overrides global config (key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesGlobalKey)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesGlobalKey()
    {
        $key = $this->db->initKey("test", "demo", "put_key");
        $status = $this->db->put($key, array("bin1"=>"Hello World"), 15,
            array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['key']['key'] == "put_key")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Method overrides global config (exists).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesGlobalExists)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesGlobalExists()
    {
        $key = $this->db->initKey("test", "demo", "put_key");
        $status = $this->db->put($key, array("bin1"=>"Hello World"), 15,
            array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_UPDATE));
        return $status;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Global config overrides csdk defaults(key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGlobalOverridesDefaultsKey)
     *
     * @test_plans{1.1}
     */
    function testGlobalOverridesDefaultsKey()
    {
        $key = $this->db->initKey("test", "demo", "put_key");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['key']['key'] == "put_key")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Method overrides csdk defaults(key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesDefaultsKey)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesDefaultsKey()
    {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, true);
        $key = $db->initKey("test", "demo", "put_key");
        $status = $db->put($key, array("bin1"=>"Hello World"), 15,
            array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        $this->keys[] = $key;
        $status = $db->get($key, $returned);
        $db->close();
        if($returned['key']['key'] == "put_key")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Global config overrides csdk defaults(exists).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGlobalOverridesDefaultsExists)
     *
     * @test_plans{1.1}
     */
    function testGlobalOverridesDefaultsExists()
    {
        $key = $this->db->initKey("test", "demo", "put_key");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['bins'] != NULL)
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Method overrides csdk defaults(exists).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesDefaultsExists)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesDefaultsExists()
    {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, true);
        $key = $db->initKey("test", "demo", "put_key_update");
        $status = $db->put($key, array("bin1"=>"Hello World"), 15,
            array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_UPDATE));
        $db->close();
        return $status;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Global config overrides csdk defaults(timeout).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGlobalOverridesDefaultsWriteTimeout)
     *
     * @test_plans{1.1}
     */
    function testGlobalOverridesDefaultsWriteTimeout()
    {
        $key = $this->db->initKey("test", "demo", "put_key");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if ($status != AEROSPIKE::OK) {
            return $status;
        }
        if($returned['bins'] != NULL)
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * UDF - Method overrides global config(key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesGlobalKeyUDF)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesGlobalKeyUDF()
    {
        $key = $this->db->initKey("test","demo", "udf_integer");
        $status = $this->db->put($key,array("bin1"=>10), NULL, array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_IGNORE,
		Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        if ($status != AEROSPIKE::OK) {
            return($status);
        }
	$status = $this->db->get($key, $returned, NULL,  array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
	if ($status != AEROSPIKE::OK) {
            return($status);
	}
	if ($returned['key']['key'] != NULL) {
	    return Aerospike::ERR_CLIENT; 
	}
        $returned = "aa";
        $udf_apply_status = $this->db->apply($key, "module",
            "bin_udf_operation_integer", array("bin1", 2, 20),
            $returned, array(Aerospike::OPT_WRITE_TIMEOUT => 2000, Aerospike::OPT_POLICY_KEY=> Aerospike::POLICY_KEY_SEND));
        $this->keys[] = $key;
        if ($udf_apply_status != Aerospike::OK) {
            return ($this->db->errorno());
        }

	$status = $this->db->get($key, $returned, NULL,  array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
	if ($status != AEROSPIKE::OK) {
            return($status);
	}
	if ($returned['key']['key'] != "udf_integer") {
	    return Aerospike::ERR_CLIENT; 
	}
        $code = array();
        $status = $this->db->getRegistered("module.lua", $code,
            Aerospike::UDF_TYPE_LUA, array(Aerospike::OPT_READ_TIMEOUT => 2000));
        if ($status != Aerospike::OK) {
            return($this->db->errorno());
        }

        return ($status);
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * Append - Method overrides global config(key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMethodOverridesGlobalKeyAppend)
     *
     * @test_plans{1.1}
     */
    function testMethodOverridesGlobalKeyAppend()
    {
        $key = $this->db->initKey("test", "demo", 157);
        $status = $this->db->append($key, 'Greet', ' World',
            array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        if ($status != AEROSPIKE::OK) {
            return $status;
        }
        $this->keys[] = $key;
        $status = $this->db->get($key, $get_record, array('Greet'),
            array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        if ($status != AEROSPIKE::OK) {
            return $status;
        }
        if (NULL != $get_record['key']['key']) {
             return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic Single PUT
     *
     * @pre
     * INI overrides csdk defaults (key).
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testINIOverridesDefaultsKey)
     *
     * @test_plans{1.1}
     */
    function testINIOverridesDefaultsKey()
    {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $db = new Aerospike($config, true);
        $key = $db->initKey("test", "demo", "key_ini_put");
        $status = $db->put($key,array("bin1"=>"Hello Wo"));
        if ($status != AEROSPIKE::OK) {
            return $status;
        }
        $this->keys[] = $key;
        $status = $db->get($key, $returned);
        $db->close();
        if ($status != AEROSPIKE::OK) {
            return $status;
        }
        if($returned['key']['key'] != "key_ini_put") {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
?>

