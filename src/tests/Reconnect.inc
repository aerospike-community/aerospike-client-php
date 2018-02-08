<?php

require_once("Common.inc");

class Reconnect extends AerospikeTestCommon
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
     * Basic Positive Reconnect after close for default persistent connection.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testReconnectPersistentConnectionPositive()
    {
        $key = $this->db->initKey("test", "demo", "testkey");
        $this->db->put($key, array("username"=>"John"));
        $close_status = $this->db->close();
        if (Aerospike::OK == $close_status) {
            $reconnect_status = $this->db->reconnect();
            if (Aerospike::OK == $reconnect_status) {
                if (Aerospike::OK == $this->db->get($key, $rec)) {
                    if (!array_key_exists("username", $rec["bins"])) {
                        return Aerospike::ERR_CLIENT;
                    } else {
                        if ($rec["bins"]["username"] == "John") {
                            $this->db->remove($key);
                            return Aerospike::OK;
                        } else {
                            return Aerospike::ERR_CLIENT;
                        }
                    }
                } else {
                    return Aerospike::ERR_CLIENT;
                }
            } else {
                return $reconnect_status; 
            }
        } else {
            return $close_status;
        }
    }

    /**
     * @test
     * Basic Reconnect after close for non-persistent connection.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testReconnectNonPersistentConnectionPositive()
    {
        $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME2,
            "port"=>AEROSPIKE_CONFIG_PORT2)));
        $db1 = new Aerospike($config, false);
        if ($db1->isConnected()) {
            $key = $db1->initKey("test", "demo", "testkey");
            $db1->put($key, array("username"=>"John"));
            $close_status = $db1->close();
            if (Aerospike::OK == $close_status) {
                $reconnect_status = $db1->reconnect();
                if (Aerospike::OK == $reconnect_status) {
                    if (Aerospike::OK == $db1->get($key, $rec)) {
                        if (!array_key_exists("username", $rec["bins"])) {
                            return Aerospike::ERR_CLIENT;
                        } else {
                            if ($rec["bins"]["username"] == "John") {
                                $db1->remove($key);
                                $db1->close();
                                return Aerospike::OK;
                            } else {
                                return Aerospike::ERR_CLIENT;
                            }
                        }
                    } else {
                        return Aerospike::ERR_CLIENT;
                    }
                } else {
                    return $reconnect_status; 
                }
            } else {
                return $close_status;
            }
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Positive Mutiple Reconnects after close for default persistent connection.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testReconnectMultiplePersistentConnectionPositive()
    {
        $key = $this->db->initKey("test", "demo", "testkey");
        $this->db->put($key, array("username"=>"John"));
        $close_status = $this->db->close();
        if (Aerospike::OK == $close_status) {
            $reconnect_status = $this->db->reconnect();
            $close_status = $this->db->close();
            if (Aerospike::OK == $close_status) {
                $reconnect_status = $this->db->reconnect();                
                if (Aerospike::OK == $reconnect_status) {
                    if (Aerospike::OK == $this->db->get($key, $rec)) {
                        if (!array_key_exists("username", $rec["bins"])) {
                            return Aerospike::ERR_CLIENT;
                        } else {
                            if ($rec["bins"]["username"] == "John") {
                                $this->db->remove($key);
                                return Aerospike::OK;
                            } else {
                                return Aerospike::ERR_CLIENT;
                            }
                        }
                    } else {
                        return Aerospike::ERR_CLIENT;
                    }
                } else {
                    return $reconnect_status; 
                }
            } else {
                return $close_status;
            }
        } else {
            return($this->db->errorno());
        }
    }

    /**
     * @test
     * Negative Reconnect without close.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETNoParameter)
     *
     * @test_plans{1.1}
     */
    function testReconnectWithoutCloseNegative()
    {
        return ($this->db->reconnect());
    }
}
?>
