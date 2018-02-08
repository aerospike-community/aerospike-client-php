<?php
require_once 'Common.inc';

/**
 *Basic Append  opeartion tests
*/

class Operate  extends AerospikeTestCommon
{
    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "operate_key");
        $this->db->put($key,array("first_name"=>"John","last_name"=>"Smith","age"=>25));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "operate_bin");
        $this->db->put($key,array("Athlete" => "Dan"));
        $this->keys[] = $key;
    }

    /**
     * @test
     * Basic Operate on a single record.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperatePositive)
     *
     * @test_plans{1.1}
     */
    function testOperatePositive() {
        $check = array("first_name" => "Mr John");
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );
        $returned = array();
        $status = $this->db->operate($this->keys[0], $operations, $returned);
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($returned, $check);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        else {
            return AEROSPIKE::OK;
        }
        return $status;
    }
    /**
    * @test
    * Basic operate with OPERATOR_INCR and offset is an integer value
    *
    * @pre
    * Connect using aerospike object to the specified node
    *
    * @post
    * newly initialized ttl value for record
    *
    */
   function testOperatePositiveIncrement() {
       $operations = array(array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 2));
       $status = $this->db->operate($this->keys[0], $operations, $returned);
       if ($status !== Aerospike::OK) {
           return $this->db->errorno();
       }
       $status = $this->db->get($this->keys[0], $record, array('age'));
       if ($status == Aerospike::OK) {
           if ($record['bins']['age'] == 27) {
               return Aerospike::OK;
           } else {
               return Aerospike::ERR_SERVER;
           }
       } else {
           return $this->db->errorno();
       }
   }
    /**
     * @test
     * Basic Operate with options. 
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateWithOptionsPositive)
     *
     * @test_plans{1.1}
     */
    function testOperateWithOptionsPositive() {
        $check = array("first_name" => "Mr John");
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );
        $returned = array();
        $status = $this->db->operate($this->keys[0], $operations, $returned,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000,
            Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($returned, $check);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return AEROSPIKE::OK;
        }
        return $status;
    }
    /**
     * @test
     * Basic Operate with multiple operations on same bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperatePositive)
     *
     * @test_plans{1.1}
     */
    function testOperateSameBinNegative() {
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "first_name", "val" => "."),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );
        $returned = array();
        $status = $this->db->operate($this->keys[0], $operations, $returned);

        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        if ($returned['first_name'] == "Mr John.") {
            return AEROSPIKE::OK;
        }
        else {
            return AEROSPIKE::ERR_SERVER;
        }
    }
    /**
     * @test
     * Basic Operate with empty arguments.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateEmptyArgumentsNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateEmptyArgumentsNegative() {
        try {
            $status = $this->db->operate();
        } catch (ErrorException $e) {
            return Aerospike::ERR_CLIENT;
        }
    }
    /**
     * @test
     * Basic bin value append when append value not string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateKeyIsEmptyStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateKeyIsEmptyStringNegative() {
        $check = array("first_name" => "Mr John");
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );
        $returned = array();
        try {
			$status = $this->db->operate("", $operations, $returned,
				array(Aerospike::OPT_WRITE_TIMEOUT=>2000,
				Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_ALL));
			if ($status != AEROSPIKE::OK) {
				return $this->db->errorno();
			}
			$comp_res = array_diff_assoc_recursive($returned, $check);
			if (!empty($comp_res)) {
				return Aerospike::ERR_CLIENT;
			}
			else {
				return AEROSPIKE::OK;
			}
        } catch (ErrorException $e) {
        	return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic return value is string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateReturnIsStringPositive() {
        $check = array("first_name" => "Mr John");
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned,
            array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($returned, $check);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        else {
            return AEROSPIKE::OK;
        }
    }
    /**
     * @test
     * Basic bin value append when append value not string
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateReturnNotPassedPositive() {
        $check = array("first_name" => "Mr John");
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = NULL;
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_WRITE_TIMEOUT=>2000));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($returned, $check);
        if (!empty($comp_res)) {
            return Aerospike::ERR_CLIENT;
        }
        else {
            return AEROSPIKE::OK;
        }
        return $status;
    }
    /**
     * @test
     * Basic operate with policy gen GT lesser
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithGenGTLesser() {
        $check = array("first_name" => "John");
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy gen GT positive
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithGenGTPositive() {
        $check = array("first_name" => "Mr John");
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT, $gen_value)));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy gen EQ positive
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithGenEQPositive() {
        $check = array("first_name" => "Mr John");
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"];
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy gen EQ not equal
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithGenEQNotEqual() {
        $check = array("first_name" => "John");
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy gen ignore
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithGenEQIgnore() {
        $check = array("first_name" => "Mr John");
        $exists_status = $this->db->exists($this->keys[0], $metadata);
        $gen_value = $metadata["generation"] + 5;
        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, $gen_value)));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy key digest
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithPolicyKeyDigest() {
        $check = array("first_name" => "Mr John");
        $key = $this->db->initKey("test", "demo",
        		str_repeat("a", 20), true);
        $put_status = $this->db->put($key, array("first_name"=>"John", "last_name"=>"Smith", "age"=>25), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));

        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($key, $operations, $returned, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }
    /**
     * @test
     * Basic operate with policy key send
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testOperateReturnIsStringNegative)
     *
     * @test_plans{1.1}
     */
    function testOperateWithPolicyKeySend() {
        $check = array("first_name" => "Mr John");

        $operations = array(
            array("op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr "),
            array("op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name",
            "val" => "."),
            array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1),
            array("op" => Aerospike::OPERATOR_READ, "bin" => "first_name")
        );

        $returned = "abc";
        $status = $this->db->operate($this->keys[0], $operations, $returned, array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
        if ($status != AEROSPIKE::OK) {
            return $this->db->errorno();
        }
        else {
            $comp_res = array_diff_assoc_recursive($returned, $check);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
            else {
                return AEROSPIKE::OK;
            }
        }
    }

    /**
     * @test
     * Basic operate with ttl value set
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized ttl value for record
     *
     */
    function testOperateWithTTLPositive() {
        $operations = array(array("op" => Aerospike::OPERATOR_TOUCH, "ttl" => 400));
        $status = $this->db->operate($this->keys[0], $operations, $returned);
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        }

        $status = $this->db->exists($this->keys[0], $metadata);
        if ($status == Aerospike::OK && !empty($metadata)) {
            return Aerospike::OK;
        }
        return $this->db->errorno();
    }


    /**
     * @test
     * Basic operate with ttl value set
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized ttl value for record
     *
     */
    function testOperateWithTTLString() {
        $operations = array(array("op" => Aerospike::OPERATOR_TOUCH, "ttl" => "fail"));
        $status = $this->db->operate($this->keys[0], $operations, $returned);
        //var_dump($this->db->errorno());
        if ($status != Aerospike::OK) {
            return $this->db->errorno();
        } else {
            return Aerospike::OK;
        }
    }
    /**
    * @test
    * Basic operate with OPERATOR_INCR and offset is a numeric string value
    *
    * @pre
    * Connect using aerospike object to the specified node
    *
    * @post
    * newly initialized ttl value for record
    *
    */
   function TestOperateWithNumericStringOffset() {
       $operations = array(array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => "100"));
       $status = $this->db->operate($this->keys[0], $operations, $returned);
       if ($status != Aerospike::OK) {
           return $this->db->errorno();
       } else {
           return Aerospike::OK;
       }
   }
   /**
    * @test
    * Basic operate with OPERATOR_INCR and offset is a negative numeric string value
    *
    * @pre
    * Connect using aerospike object to the specified node
    *
    * @post
    * newly initialized ttl value for record
    *
    */
   function TestOperateWithNegativeNumericStringOffset() {
       $operations = array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => "-100");
       $status = $this->db->operate($this->keys[0], $operations, $returned);
       if ($status != Aerospike::OK) {
           return $this->db->errorno();
       } else {
           return Aerospike::OK;
       }
   }
   /**
    * @test
    * Basic operate with OPERATOR_INCR and offset is an invalid numeric string value
    *
    * @pre
    * Connect using aerospike object to the specified node
    *
    * @post
    * newly initialized ttl value for record
    *
    */
   function TestOperateWithInvalidNumericStringOffset() {
       $operations = array(array("op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => "100ia"));
       $status = $this->db->operate($this->keys[0], $operations, $returned);
       if ($status != Aerospike::OK) {
           return $this->db->errorno();
       } else {
           return Aerospike::OK;
       }
   }

   /**
     * @test
     * operate() with OPT_TTL option
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (bug_CLIENT_510)
     *
     * @test_plans{1.1}
     */
    function bug_CLIENT_510() {
        $key = $this->keys[1];
        $ops = [["op"=>Aerospike::OPERATOR_APPEND, "bin"=>"Athlete", "val"=>" Millman"],
            ["op"=>Aerospike::OPERATOR_READ, "bin"=>"Athlete"]];
        $status = $this->db->operate($key, $ops, $returned, array(Aerospike::OPT_TTL=>2));
        if ($status == Aerospike::OK && strcmp($returned["Athlete"], "Dan Millman") == 0) {
            sleep(3);
            $status = $this->db->get($key, $get_record);
            if ($status === Aerospike::ERR_RECORD_NOT_FOUND) {
                return Aerospike::OK;
            }
        }
        return $status;
    }
}
?>
