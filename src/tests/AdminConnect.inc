<?php
class AdminConnect extends AerospikeTestCommon
{
    protected function setUp() {
    }

    function testConnectPositive() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin");
        $db = new Aerospike($config);
        if ($db->isConnected()) {
            $db->close();
            return Aerospike::OK;
        } else {
            return Aerospike::ERR_CLIENT;
        }
    }

    function testConnectInvalidPassword() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host), "user"=>"admin", "pass"=>"admin1");
        try {
			$db = new Aerospike($config);
			if ($db->isConnected()) {
				return Aerospike::OK;
			} else {
				return $db->errorno();
			}
        }catch(Exception $e) {
        	if (strcmp($e->getMessage(), "Failed to connect to Aerospike server") == 0) {
        		return Aerospike::ERR_CLIENT;
        	}
        	return Aerospike::OK;
        }
    }

    function testConnectNoUsernamePassword() {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $config = array("hosts"=>array($host));
        try{
			$db = new Aerospike($config);
			if ($db->isConnected()) {
				return Aerospike::OK;
			} else {
				return $db->errorno();
			}
		}catch(Exception $e) {
			if (strcmp($e->getMessage(), "Failed to connect to Aerospike server") == 0) {
				return Aerospike::ERR_CLIENT;
			}
			return Aerospike::OK;
		}
    }
}
?>
