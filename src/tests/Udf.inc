<?php
require_once 'Common.inc';

/**
 *Basic Increment opeartion tests
 */

class Udf extends AerospikeTestCommon
{
    protected function setUp() {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $this->ensureUdfModule("tests/lua/test_record_udf.lua", "module.lua");
        $this->ensureUdfModule("tests/lua/sample.lua", "sample.lua");
    }

    /**
     * @test
     * Registers UDF Module.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveRegisterModule)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveRegisterModule() {
        $status = $this->db->listRegistered($modules);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $is_registered = false;
        foreach($modules as $module) {
            if ($module['name'] == 'module.lua') {
                $is_registered = true;
                break;
            }
        }
        if ($is_registered) return Aerospike::OK;
        else return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Registers UDF Module with policy
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveRegisterModule)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveRegisterModuleWithPolicy() {
        $status = $this->db->register("tests/lua/test_record_udf.lua", "module.lua", Aerospike::UDF_TYPE_LUA, array(Aerospike::OPT_WRITE_TIMEOUT=>1000));
        sleep(1);
        $status = $this->db->listRegistered($modules);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $is_registered = false;
        foreach($modules as $module) {
            if ($module['name'] == 'module.lua') {
                $is_registered = true;
                break;
            }
        }
        if ($is_registered) return Aerospike::OK;
        else return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Register UDF module which is not present.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfRegisterUnknownModule)
     *
     * @test_plans{1.1}
     */
    function testUdfNegativeRegisterUnknownModule() {
        $status = $this->db->register("tests/lua/unknown.lua", "unknown.lua");
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Register empty string as a UDF module.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfRegisterEmptyStringAsModule)
     *
     * @test_plans{1.1}
     */
    function testUdfNegativeRegisterEmptyStringAsModule() {
        $status = $this->db->register("", "");
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Deregisters a UDF module which is registered
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfNegativeDeregisterUnknownModule)
     *
     * @test_plans{1.1}
     */
    function testUdfDeregisterModulePositive() {
        $status = $this->db->deregister("module.lua");
        sleep(1);
        $status = $this->db->listRegistered($modules);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $is_registered = false;
        foreach($modules as $module) {
            if ($module['name'] == 'module.lua') {
                $is_registered = true;
                break;
            }
        }
        if($is_registered)
            return Aerospike::ERR_CLIENT;
        return $status;
    }
    /**
     * @test
     * Deregisters a UDF module which is registered with policy
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfNegativeDeregisterUnknownModule)
     *
     * @test_plans{1.1}
     */
    function testUdfDeregisterModulePositivePolicy() {
        $status = $this->db->deregister("module.lua", array(Aerospike::OPT_WRITE_TIMEOUT=>1000));
        sleep(1);
        $status = $this->db->listRegistered($modules);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $is_registered = false;
        foreach($modules as $module) {
            if ($module['name'] == 'module.lua') {
                $is_registered = true;
                break;
            }
        }
        if($is_registered)
            return Aerospike::ERR_CLIENT;
        return $status;
    }
    /**
     * @test
     * Deregisters a UDF module which is not registered.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfNegativeDeregisterUnknownModule)
     *
     * @test_plans{1.1}
     */
    function testUdfNegativeDeregisterUnknownModule() {
        $status = $this->db->deregister("unknown_foo.udf");
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic Udf operation on int.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyOnInteger)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyOnInteger()
    {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test","demo", "udf_integer");
        if ($this->db->isConnected()) {
            $status = $this->db->put($key,array("bin1"=>10));
        } else {
            $this->db->close();
            return($this->db->errorno());
        }
        $returned = "aa";
        $status = $this->db->apply($key, "module",
            "bin_udf_operation_integer", array("bin1", 2, 20), $returned,
            array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        if ($returned !== 32) {
            return "Value returned from apply() is not int(32)";
        }
        return ($status);
    }

    /**
     * @test
     * Basic Udf operation on String.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyOnString)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyOnString() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test","demo", "udf_for_string");
        if ($this->db->isConnected()) {
            $status = $this->db->put($key,array("bin1"=>"hello"));
        } else {
            $this->db->close();
            return($this->db->errorno());
        }
        $status = $this->db->apply($key, "module", "bin_udf_operation_string", array("bin1",
            "world"), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        if($returned == "helloworld") {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Basic Udf operation with parameter as NULL
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyParameterNull)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyParameterNull() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test","demo", "udf_parameter_null");
        if ($this->db->isConnected()) {
            $status = $this->db->put($key,array("first_name"=>array("smith"), "age"=>27));
        } else {
            $this->db->close();
            return($this->db->errorno());
        }
        $status = $this->db->apply($key, "sample", "list_append", array("first_name",
            NULL), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->get($key, $returned);
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $result = array_diff_assoc_recursive($returned["bins"]["first_name"], array("smith", NULL));
        if(empty($result)) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Apply UDF on a record containing List.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyOnList)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyOnList() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test", "demo", "list_iterate");
        $list = array(94, 5, 25);
        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>$list), NULL);
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $returned = array();
        $status = $this->db->apply($key, "module", "list_iterate", array("bin1", 2),
            $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $mystatus = $this->db->get($key, $returned);
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $testList = array("bin1" => array(94, 222, 25, 58));
        $result = array_diff_assoc_recursive($testList, $returned["bins"]);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Apply UDF on a record containing List and UDF returns a List.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyOnListReturnsList)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyOnListReturnsList() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $returned = array();
        $key = $this->db->initKey("test", "demo", "list_iterate");
        $list = array(94, 5, 25);
        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>$list), NULL);
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $returned = array();
        $status = $this->db->apply($key, "module", "list_iterate_returns_list",
            array("bin1", 2), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $testList = array(94, 222, 25, 58);
        $result = array_diff_assoc_recursive($testList, $returned);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }
    /**
     * @test
     * Apply UDF on record containing Map.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyOnMap)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyOnMap() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $returned = array();
        $key = $this->db->initKey("test", "demo", "test_map");
        $map = array("k30"=>6, "k20"=>5, "k10"=>1);
        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>$map), NULL);
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $status = $this->db->apply($key, "module", "map_iterate",
            array("bin1", 55555), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $mystatus = $this->db->get($key, $returned);
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $testMap = array("bin1"=>array("k30"=>55555, "k20"=>55555, "k10"=>55555));
        $result = array_diff_assoc_recursive($returned["bins"], $testMap);
        if (!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Apply UDF on a record and UDF returns a Map.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyReturnsMap)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyReturnsMap() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = array("ns" => "test", "set" => "demo", "key" => "test_map");
        $map = array("k30"=>6, "k20"=>5, "k10"=>1);
        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>$map), NULL);
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $returned = array();
        $status = $this->db->apply($key, "module", "map_iterate_returns_map",
            array("bin1", 85), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $testMap = array("k30"=>85, "k20"=>85, "k10"=>85);
        $result = array_diff_assoc_recursive($testMap, $returned);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Apply UDF on a record and UDF returns a Map(Array) but we are passing variable.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyReturnsMapPassingVariable)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyReturnsMapPassingVariable() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test", "demo", "test_map");
        $map = array("k30"=>6, "k20"=>5, "k10"=>1);
        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>$map), NULL);
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $ret_val = "aa";

        $status = $this->db->apply($key, "module", "map_iterate_returns_map",
            array("bin1", 5), $ret_val);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        $testMap = array("k30"=>5, "k20"=>5, "k10"=>5);
        $result = array_diff_assoc_recursive($testMap, $ret_val);
        if(!empty($result)) {
            return Aerospike::ERR_CLIENT;
        }
        return Aerospike::OK;
    }

    /**
     * @test
     * Apply UDF on a record which returns record.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyReturnsRecord)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyReturnsRecord() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test", "demo", "udf_returns_record");
        $map = array("k30"=>6, "k20"=>5, "k10"=>1);
        $list = array(1, 2, 89);
        $this->db->put($key, array("bin1"=>$map, "bin2"=>$list));
        $this->keys[] = $key; // mark record for removal during teardown
        $status = $this->db->apply($key, "module", "udf_returns_record", array(), $returned);
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        if (isset($returned['bin1']) && isset($returned['bin2'])) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_UDF;
    }

    /**
     * @test
     * Apply UDF on a record which accepts nothing and returns nothing.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveApplyAcceptsNothingAndReturnsNothing)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveApplyAcceptsNothingAndReturnsNothing() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test", "demo", "udf_put");

        if ($this->db->isConnected()) {
            $status = $this->db->put($key, array("bin1"=>"aa"));
        } else{
            $this->db->close();
            return($this->db->errorno());
        }
        $return_val = NULL;
        $status = $this->db->apply($key, "module", "udf_without_arg_and_return",
            NULL, $return_val, array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        return ($status);
    }

    /**
     * @test
     * Apply UDF on record, Where Module name is empty string.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfApplyNegativeModuleIsEmptyString)
     *
     * @test_plans{1.1}
     */
    function testUdfApplyNegativeModuleIsEmptyString() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        $key = $this->db->initKey("test", "demo", "udf_apply_modules_empty_string");
        $status = $this->db->apply($key, "", "udf_put_bytes", array("bin1"));
        if ($status != Aerospike::OK) {
            return ($this->db->errorno());
        }
        $status = $this->db->remove($key,
            array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
        return ($status);
    }
    /**
     * @test
     * List registered LUA(UDF_TYPE_LUA) modules.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveListRegisteredLUAModules)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveListRegisteredLUAModules() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $modules = array();
            $status = $this->db->listRegistered($modules, Aerospike::UDF_TYPE_LUA,
                array(Aerospike::OPT_READ_TIMEOUT => 5000));
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
            foreach ($modules as $moduleeach) {
                if ($moduleeach['name'] == 'module.lua') {
                    return Aerospike::OK;
                }
            }
            return Aerospike::ERR_CLIENT;
        }
        return($status);
    }

    /**
     * @test
     * List registered all type(UDF_TYPE_*) modules.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveListRegisteredModules)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveListRegisteredModules() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $modules = array();
            $status = $this->db->listRegistered($modules);
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
            foreach ($modules as $moduleeach) {
                if ($moduleeach['name']=='module.lua') {
                    return Aerospike::OK;
                }
            }
            return Aerospike::ERR_CLIENT;
        }
        return($status);
    }

    /**
     * @test
     * List registered UDF modules and PHP script passing variable.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveListRegisteredModulesPassingVariable)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveListRegisteredModulesPassingVariable() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $modules = 5;
            $status = $this->db->listRegistered($modules);
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
            foreach ($modules as $moduleeach) {
                if ($moduleeach['name'] == 'module.lua') {
                    return Aerospike::OK;
                }
            }
            return Aerospike::ERR_CLIENT;
        }
        return($status);
    }

    /**
     * @test
     * Get registered UDF's code.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveGetRegisteredModule)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveGetRegisteredModule() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $code = array();
            $status = $this->db->getRegistered("module.lua", $code,
                Aerospike::UDF_TYPE_LUA);
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
        }
        $handle = fopen("tests/lua/test_record_udf.lua","r");
        $mycode = fread($handle,filesize("tests/lua/test_record_udf.lua"));
        $result = strcmp($code, $mycode);
        if(!$result) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Get registered UDF's code with policy
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveGetRegisteredModule)
     *
     * @test_plans{1.1}
     */
    function testUdfPositiveGetRegisteredModuleWithPolicy() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $code = array();
            $status = $this->db->getRegistered("module.lua", $code,
                Aerospike::UDF_TYPE_LUA,
                array(Aerospike::OPT_READ_TIMEOUT => 2000));
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
        }
        $handle = fopen("tests/lua/test_record_udf.lua","r");
        $mycode = fread($handle,filesize("tests/lua/test_record_udf.lua"));
        $result = strcmp($code, $mycode);
        if(!$result) {
            return Aerospike::OK;
        }
        return Aerospike::ERR_CLIENT;
    }

    /**
     * @test
     * Get contents of a module which is not registered.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfNegativeGetNotRegisteredModule)
     *
     * @test_plans{1.1}
     */
    function testUdfNegativeGetNotRegisteredModule() {
        if (!$this->db->isConnected()) {
            return ($this->db->errorno());
        }
        else {
            $code = array();
            $status = $this->db->getRegistered("unknown", $code,
                Aerospike::UDF_TYPE_LUA);
            if ($status != Aerospike::OK) {
                return($this->db->errorno());
            }
        }
        return($status);
    }
    /**
     * @test
     * apply Udf on a record and returned parameter of udf is initialized to
     * NULL.
     *
     * @pre
     * Udf using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testUdfPositiveReturnvalueInitializedNULL)
     *
     * @test_plans{1.1}
     */
     function testUdfPositiveReturnvalueInitializedNULL() {
         if (!$this->db->isConnected()) {
             return ($this->db->errorno());
         }
         $key = $this->db->initKey("test","demo", "udf_list_no_return_value");
         $list = array(94, 5, 25);
         if ($this->db->isConnected()) {
             $status = $this->db->put($key, array("bin1"=>$list), NULL);
         } else {
             $this->db->close();
             return($this->db->errorno());
         }
         $returned = NULL;
         $udf_apply_status = $this->db->apply($key, "module",
             "list_iterate", array("bin1", 2, 20), $returned,
             array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
         if ($udf_apply_status != Aerospike::OK) {
             return ($this->db->errorno());
         }
         if (!empty($returned)) {
             return Aerospike::ERR_CLIENT;
         }
         $status = $this->db->remove($key,
             array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE));
         return ($status);
     }
    /**
     * @test
     * apply Udf on a record and append float value to existing bin
     * NULL.
     *
     * @pre
     * Udf using aerospike object to the specified node
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
         if (!$this->db->isConnected()) {
             return ($this->db->errorno());
         }
         $key = $this->db->initKey("test","demo", "pi");
         $list = array(94, 5, 25);
         if ($this->db->isConnected()) {
             $status = $this->db->put($key, array("bin1"=>$list), NULL);
         } else {
             $this->db->close();
             return($this->db->errorno());
         }
         $returned = NULL;
         $udf_apply_status = $this->db->apply($key, "module",
             "list_append", array("bin1", 3.141), $returned,
             array(Aerospike::OPT_WRITE_TIMEOUT => 2000));
         $this->keys[] = $key;
         if ($udf_apply_status != Aerospike::OK) {
             return ($this->db->errorno());
         }

         if (abs($returned[3] - 3.141) > 0.00001) {
             return Aerospike::ERR_CLIENT;
         }
         return Aerospike::OK;
     }
}
?>
