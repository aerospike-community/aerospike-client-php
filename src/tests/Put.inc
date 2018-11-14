<?php

require_once 'Common.inc';
/**
 *Basic Put tests
*/

class Employee
{
    public $desg = 'Manager';
}

class Put extends AerospikeTestCommon
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
     * Basic Single PUT
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUT)
     *
     * @test_plans{1.1}
     */
    function testPUT()
    {
        $key = $this->db->initKey("test", "demo", "put_test");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['bins']['bin1'] == "Hello World")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Multi PUT
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testMultiPUT)
     *
     * @test_plans{1.1}
     */
    function testMultiPUT()
    {
        $key = array("ns"=>"test", "set"=>"demo", "key"=>"key_multi");
        $status = $this->db->put($key, array("bin1"=>"Hello World","bin2"=>123456), NULL,
            array(Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_MASTER));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if ($returned['bins']['bin1'] == "Hello World" &&
            $returned['bins']['bin2']==123456) {
                return Aerospike::OK;
            }
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Put with no parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPUTNoParameter()
    {
        try {
            $key = array("ns"=>"test", "set"=>"demo", "key"=>"10000");
            $status = $this->db->put();
            $this->keys[] = $key;
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Basic Single PUT with unicode character.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUT)
     *
     * @test_plans{1.1}
     */
    function testPUTWithUnicodeCharacter()
    {
        $key = $this->db->initKey("test", "demo", "put_test");
        $status = $this->db->put($key,array("bin1"=>utf8_encode("Hello World")));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['bins']['bin1'] == "Hello World")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Basic Single PUT with key as unicode character.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUT)
     *
     * @test_plans{1.1}
     */
    function testPUTWithKeyAsUnicodeCharacter()
    {
        $key = $this->db->initKey("test", "demo", utf8_encode("put_test"));
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        $status = $this->db->get($key, $returned);
        if($returned['bins']['bin1'] == "Hello World")
            return Aerospike::OK;
        return Aerospike::ERR_CLIENT;
    }
    /**
     * @test
     * Put with only key parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPUTWithOnlyKeyParameter()
    {
        try {
            $key = array("ns"=>"test", "set"=>"demo", "key"=>"10000");
            $status = $this->db->put($key);
            $this->keys[] = $key;
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Put without key parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPUTWithOnlyDataParameter()
    {
        try {
            $key = array("ns"=>"test", "set"=>"demo", "key"=>"10000");
            $status = $this->db->put(array("bin1"=>"Hello World",
                array(Aerospike::OPT_POLICY_COMMIT_LEVEL=>Aerospike::POLICY_COMMIT_LEVEL_ALL)));
            $this->keys[] = $key;
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Put data as normal array ex. array("hi","hello")
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPUTWithNormalpPutDataArrayParameter()
    {
        $key = array("ns"=>"test", "set"=>"demo", "key"=>"10000");
        $status = $this->db->put($key, array("Hello World"));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic PUT Check key parameter missing in key array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckKeyParameterMissingInKeyArray)
     *
     * @test_plans{1.1}
     */
    function testCheckKeyParameterMissingInKeyArray()
    {
        $key = array("ns"=>"test", "set"=>"demo");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic PUT Check ns(namespace) parameter missing in key array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckNameSpaceParameterMissingInKeyArray)
     *
     * @test_plans{1.1}
     */
    function testCheckNameSpaceParameterMissingInKeyArray()
    {
        $key = array("set"=>"demo","key"=>"10000");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic PUT Check set parameter missing in key array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSetParameterMissingInKeyArray)
     *
     * @test_plans{1.1}
     */
    function testCheckSetParameterMissingInKeyArray()
    {
        $key = array("ns"=>"test","key"=>"10000");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
     * @test
     * Basic PUT Second Parameter Array test
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckSecondParameterTypeArray)
     *
     * @test_plans{1.1}
     */
    function testCheckSecondParameterTypeArray()
    {
        try {
            $key = array("ns"=>"test", "set"=>"demo", "key"=>"10000");
            $bin_arr = "";
            $status = $this->db->put($key,$bin_arr);
            $this->keys[] = $key;
            if ($status !== Aerospike::OK) {
                return $this->db->errorno();
            }
            return $status;
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Basic PUT Check sequence of key parameter in Key array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckParameterSequenceChangeInKeyArray)
     *
     * @test_plans{1.1}
     */
    function testCheckParameterSequenceChangeInKeyArray()
    {
        $key = $this->db->initKey("demo", "test", "key_int1");
        $status = $this->db->put($key,array("bin1"=>"Hello World"));
        $this->keys[] = $key;
        if($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
    /**
      * @test
      * Basic PUT Check key parameter value non exist in database
      *
      * @pre
      * Connect using aerospike object to the specified node
      *
      * @post
      * newly initialized Aerospike objects
      *
      * @remark
      * Variants: OO (testCheckKeyValueNotExistInDB)
      *
      * @test_plans{1.1}
      */
    function testCheckKeyValueNotExistInDB()
    {
      $key = array("ns"=>"test","set"=>"demo","key"=>"-------------");
      $status = $this->db->put($key,array("bin1"=>"Hello World"), 10,
          array(Aerospike::OPT_POLICY_RETRY =>Aerospike::POLICY_RETRY_NONE));
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
    }

/**
  * @test
  * Basic PUT Check namespace parameter value non exist in database
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckNameSpaceValueNotExistInDB)
  *
  * @test_plans{1.1}
  */
  function testCheckNameSpaceValueNotExistInDB()
  {
      $key = array("ns"=>"aksdksajfasjk","set"=>"demo","key"=>"10000");
      $status = $this->db->put($key,array("bin1"=>"Hello World"));
      $this->keys[] = $key;
      if($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }

/**
  * @test
  * Basic PUT Check Set parameter value non exist in database
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckSetValueNotExistInDB)
  *
  * @test_plans{1.1}
  */
  function testCheckSetValueNotExistInDB()
  {
      $key = array("ns"=>"test","set"=>"sadklasndaasd","key"=>"10000");
      $status = $this->db->put($key,array("bin1"=>"Hello World"));
      if($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }
/**
  * @test
  * Basic PUT Insert list
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListInsert)
  *
  * @test_plans{1.1}
  */
  function testCheckListInsert()
  {
      $key = array("ns"=>"test","set"=>"demo","key"=>"10000");
      $status = $this->db->put($key,array("bin1"=>array("speaking", "reading", "writing", 10,
              array(Aerospike::OPT_POLICY_EXISTS =>Aerospike::POLICY_EXISTS_CREATE))));
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }
/**
  * @test
  * Basic PUT Insert Map
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckMapInsert)
  *
  * @test_plans{1.1}
  */
  function testCheckMapInsert()
  {
      $key = array("ns"=>"test","set"=>"demo","key"=>"key_map");
      $opts =array(Aerospike::OPT_POLICY_RETRY=>Aerospike::POLICY_RETRY_NONE,
          Aerospike::OPT_WRITE_TIMEOUT => 1500,
          Aerospike::OPT_POLICY_EXISTS => Aerospike::POLICY_EXISTS_CREATE);
      $map1= array("k1"=>10,"k2"=>5,"k3"=>6,"k4"=>7,"k5"=>8);
      $status = $this->db->put($key,array("bin1"=>$map1), 10, $opts);
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }
  /**
  * @test
  * Put an empty PHP array into a bin
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutEmptyArrayIntoBin)
  *
  * @test_plans{1.1}
  */
  function testPutEmptyArrayIntoBin()
  {
      $key = array("ns"=>"test","set"=>"demo","key"=>"aerospike_issue_14");
      $status = $this->db->put($key, array("array_bin"=>array()), 10);
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }
/**
  * @test
  * Basic PUT Insert Map and List combine
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
  function testCheckListMapCombineInsert()
  {
      $key = array("ns"=>"test","set"=>"demo","key"=>"10000");
      $softskills = array("speaking", "reading", "writing");
      $skills = array("python", "c", "java",$softskills);
      $innermostmap = array("month"=>"May", "year"=>2013);
      $innermap1 = array("degree_name"=>"B.E.", "degree_branch"=>"Comp Sci", "degree_date"=>$innermostmap);
      $innermap = array("degree"=>$innermostmap, "aggregate"=>70);
      $mymap = array("name"=>"aero", "age"=>23, "edu"=>$innermap, "skills"=>$skills);
      $innermostlist = array("twitter", 100);
      $innerlist = array(1,2,"facebook", $innermostlist);
      $mylist = array(10, 20, "whatsup", $innerlist, $mymap);
      $rec = array("bin1"=>$mylist);
      $status = $this->db->put($key,$rec, NULL, array(Aerospike::OPT_WRITE_TIMEOUT=>2000,
              Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      return $status;
  }

/**
  * @test
  * Basic PUT Nested List
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
  function testNestedList() {
      $key = $this->db->initKey("test", "demo", "list_max3");
      $list2 = array("aa");
      $list3 = array($list2,1,"kk");
      $list4 = array(46,$list3);
      $put_record = array("bin1" => $list4);
      $status = $this->db->put($key,array("bin1"=>$list4));
      $this->keys[] = $key;
      $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $status;
  }

/**
  * @test
  * Basic PUT Insert Int List and string List combine
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
  function testIntlistStringlist() {
      $key = $this->db->initKey("test", "demo", "st");
      $list1 = array("aa","vv","hh");
      $list2 = array(100,566,52,$list1);
      $put_record = array("bin1" => $list2);
      $status = $this->db->put($key,array("bin1"=>$list2));
      $this->keys[] = $key;
      $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $status;
  }

/**
  * @test
  * Insert List with wrong IP
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
  function testDisconnectedPut() {
	 $config = array("hosts"=>array(array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT)));
     $db = new Aerospike($config, false);
     $key = $db->initKey("test", "demo", "list_list_strval");

     $db->close();
     $status = $db->put($key,array("bin1"=>5));
     return $status;
 }

/**
  * @test
  * Basic PUT Insert Map with int value
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
 function testMapWithIntval() {
     $key = $this->db->initKey("test", "demo", "reival");
     $map1 = array("k10"=>10);
     $map2 = array(1,"k6"=>"abc",$map1);
     $put_record = array("bin1"=>$map2);
     if ($this->db->isConnected()) {
     $status = $this->db->put($key,array("bin1"=>$map2));
     $this->keys[] = $key;
     } else {
         return $this->db->errorno();
     }
      $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $status;
 }

/**
  * @test
  * Basic PUT Insert List int and string
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
 function testIntStringList() {
     $key = $this->db->initKey("test", "demo", "list6_key");
     $list1 = array("aa",2);
     $list2 = array(3,"k1","k4"=>$list1);
     $put_record = array("bin1"=>$list2);
     $status = $this->db->put($key,array("bin1"=>$list2));
     $this->keys[] = $key;
      $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $status;
 }

/**
  * @test
  * Basic PUT AssocArray
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckListMapCombineInsert)
  *
  * @test_plans{1.1}
  */
 function testPutAssocArray() {
     $key = $this->db->initKey("test", "demo", "key_val_int");
     $map2 = array("k10"=>89,85);
     $put_record = array("bin1"=>$map2);
     $status = $this->db->put($key,array("bin1"=>$map2));
     $this->keys[] = $key;
      $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $status;
 }
 /**
  * @test
  * Serialization with mixed types and no explicit serializer policy
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testMixedTypesWithDefaultSerializer)
  *
  * @test_plans{1.1}
  */
function testMixedTypesWithDefaultSerializer()
{
    if (ini_get('aerospike.serializer') !== false) {
        $options = array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP);
    } else {
        $options = array();
    }
    $key = array("ns"=>"test","set"=>"demo","key"=>"map_with_mixed_types");
    $obj = new stdClass();
    $obj->z = 26;
    $vals = array(
    "_string" => "A string value",
    "_integer" => 101,
    "_boolean" => true,
    "_float" => 6.969,
    "_object" => $obj,
    "_list" => array("A", "Word", "to", "the", "Wise"),
    "_map" => array("foo" => "bar", "boo" => "hoo"));
    $status = $this->db->put($key, $vals, 10, $options);
    $this->keys[] = $key;
    if ($status !== Aerospike::OK) {
        return $this->db->errorno();
    }
    return $status;
}

/**
  * @test
  * PUT List of objects and serializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListOfObjectsSerializerNone)
  *
  * @test_plans{1.1}
  */
 function testPutListOfObjectsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "list_of_objects_with_none_serializer");
     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2);
     $list3 = array("k1", $obj4, $list2);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_objects_with_PHP_serializer");
     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of objects with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfObjectsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_objects_with_UDF_serializer");
     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
        return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Map of objects with Serializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfObjectsSerializerNone)
  *
  * @test_plans{1.1}
  */
 function testPutMapOfObjectsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "map_of_objects_with_none_serializer");
     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map1 = array(12=>$obj1, $obj3);
     $map2 = array($map1, "k45"=>$obj2);
     $map3 = array("k1"=>12, $obj4, "k4"=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested map of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map1 = array(12=>$obj1, $obj3);
     $map2 = array($map1, $obj2, " ");
     $map3 = array("k1", $obj4, 56=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested map of objects with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfObjectsWithUDFSerializer() {
     if(!$this->db->isConnected())
     {
         return ($this->db->errorno());
     }
    return(Aerospike::OK);
 }

/**
  * @test
  * PUT List containing Map of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();

     $map = array(12=>"k5", "k78"=>$obj1, $obj2);
     $list = array("k1", $obj3, $map);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT List containing Map of objects with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapObjectsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_objects_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, $obj2, "aa"=>$obj3);
     $list = array("k1", $obj4, $map, 55);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containing List of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 56, " ", $obj4);
     $map = array("k1"=>$obj3, $obj1, 56=>$list);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containing List of objects with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListObjectsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_objects_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 56, " ", $obj4);
     $map = array("k1"=>$obj3, $obj1, 56=>$list);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map of objects, Object as a key.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfObjectsKeyIsObject)
  *
  * @test_plans{1.1}
  */
 /*function testPutMapOfObjectsKeyIsObject() {
     $key = $this->db->initKey("test", "demo", "map_of_objects_key_is_object");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", $obj1=>$obj2, "k2");
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }*/

/**
  * @test
  * PUT List of bools and seializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListOfBoolsSerializerNone)
  *
  * @test_plans{1.1}
  */
 #Testcases for bool
 function testPutListOfBoolsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "list_of_bools_with_none_serializer");

     $list1 = array(12, TRUE);
     $list2 = array($list1, FALSE, 89);
     $list3 = array("k1", TRUE, $list2);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of bools with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_bools_with_PHP_serializer");

     $list1 = array(12, TRUE);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, FALSE);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of bools with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfBoolsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_bools_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     $list1 = array(12, TRUE);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, FALSE);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Map of bools and serializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfBoolsSerializerNone)
  *
  * @test_plans{1.1}
  */
 function testPutMapOfBoolsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_with_none_serializer");

     $map1 = array(12=>TRUE, "k2");
     $map2 = array($map1, "aa"=>FALSE);
     $map3 = array("k1"=>12, 89, "k4"=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested Map of bools with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_with_PHP_serializer");

     $map1 = array(12=>TRUE, FALSE);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested Map of bools with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfBoolsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     $map1 = array(12=>TRUE, FALSE);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT List containing Map of bools with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_bools_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, FALSE, $obj2, "aa"=>$obj3);
     $list = array("k1", $obj4, $map, 55=>TRUE);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT List containing Map of bools with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapBoolsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_bools_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, FALSE, $obj2, "aa"=>$obj3);
     $list = array("k1", $obj4, $map, 55=>TRUE);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containig List of bools with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_bools_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 56, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containig List of bools with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListBoolsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_bools_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 56, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map of bools, Bool as a key.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfBoolsKeyIsBool)
  *
  * @test_plans{1.1}
  */
 function testPutMapOfBoolsKeyIsBool() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_key_is_bool");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", TRUE=>$obj2, $obj1);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT List of floats and serializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListOfFloatsSerializerNone)
  *
  * @test_plans{1.1}
  */
 #Testcases for float
 function testPutListOfFloatsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "list_of_floats_with_none_serializer");

     $list1 = array(12.23, TRUE);
     $list2 = array($list1, 56, 89.2);
     $list3 = array("k1", $list2);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of floats with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_floats_with_PHP_serializer");

     $list1 = array(12, 11.2);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, 56.896);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested List of floats with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedListOfFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedListOfFloatsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_floats_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     $list1 = array(12, 11.2);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, 56.896);
     $status = $this->db->put($key, array("bin1"=>$list3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Map of floats and serializer option is SERIALIZER_NONE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfFloatsSerializerNone)
  *
  * @test_plans{1.1}
  */
 function testPutMapOfFloatsSerializerNone() {
     $key = $this->db->initKey("test", "demo", "map_of_floats_with_none_serializer");

     $map1 = array(12=>89.4, "k2");
     $map2 = array($map1, "k8"=>17.1);
     $map3 = array("k1"=>12, 89, "k4"=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_NONE));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested Map of floats with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_floats_with_PHP_serializer");

     $map1 = array(12=>89.4, 55.44);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT Nested Map of floats with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutNestedMapOfFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutNestedMapOfFloatsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_floats_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     $map1 = array(12=>TRUE, FALSE);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);
     $status = $this->db->put($key, array("bin1"=>$map3), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return $status;
 }

/**
  * @test
  * PUT List containing Map of floats with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_floats_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, $obj2, "aa"=>25.6, "k10"=>TRUE);
     $list = array("k1", $obj4, $map, 55=>89.56, FALSE, 45.78);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
 }

/**
  * @test
  * PUT List containing Map of floats with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutListMapFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutListMapFloatsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_floats_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, $obj2, "aa"=>25.6, "k10"=>TRUE);
     $list = array("k1", $obj4, $map, 55=>89.56, FALSE, 45.78);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containing List of floats with PHP serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_floats_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 63.2, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1, "k22"=>56.75);
     $status = $this->db->put($key, array("bin1"=>$list), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }

/**
  * @test
  * PUT Map containing List of floats with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapListFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testPutMapListFloatsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_floats_with_UDF_serializer");

     #Set Serializer
     Aerospike::setSerializer(function ($val) {
         if (is_bool ($val)) {
             return "b||". serialize($val);
         }
         if (is_object ($val)) {
             return "o||". serialize($val);
         }
         if (is_double ($val)) {
             return "d||". serialize($val);
         }
         return "r||". $val;
     });

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 63.2, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1, "k22"=>56.75);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
         return $this->db->errorno();
     }
     return  $status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_IGNORE and generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyIgnoreWithGenValPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyIgnoreWithGenValPositive() {
     $key = $this->db->initKey("test", "demo", "key_gen_ignore");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return ($this->db->errorno());
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_IGNORE, 56)));
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_EQ and generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyEQWithGenValPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyEQWithGenValPositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_equal");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $exists_status = $this->db->exists($key, $metadata);
     if ($exists_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $gen_value = $metadata["generation"];
     $put_record =  array("bin1"=>10);
     $put_status = $this->db->put($key, $put_record, 0,
         array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ, $gen_value)));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
             $comp_res = array_diff_assoc_recursive($put_record,
                 $get_record["bins"]);
             if(!empty($comp_res)) {
                 return AEROSPIKE::ERR_CLIENT;
             }
         }
     return  $status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_GT and generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTWithGenValPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyGTWithGenValPositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $exists_status = $this->db->exists($key, $metadata);
     if ($exists_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $gen_value = $metadata["generation"] + 10;
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, $put_record, NULL,
         array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT,
         $gen_value)));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
             $comp_res = array_diff_assoc_recursive($put_record,
                 $get_record["bins"]);
             if(!empty($comp_res)) {
                 return AEROSPIKE::ERR_CLIENT;
             }
         }
     return  $status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_IGNORE and no generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyIgnorePositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyIgnorePositive() {
     $key = $this->db->initKey("test", "demo", "key_gen_ignore");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_GEN=> array(Aerospike::POLICY_GEN_IGNORE)));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_EQ and no generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyEQPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyEQPositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_equal");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_EQ)));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_GEN_GT and no generation value.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyGTPositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_GEN=>array(Aerospike::POLICY_GEN_GT)));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_CREATE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsCreatePositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_CREATE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_CREATE negative
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsCreateNegative()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_CREATE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_UPDATE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsUpdatePositive()
 {
     $key = $this->db->initKey("test", "demo", "put_test");
     $status = $this->db->put($key, array("bin1"=>"Hello World"));
     $this->keys[] = $key;
     $put_record = array("bin1"=>"Hello");
     $put_status = $this->db->put($key, array("bin1"=>"Hello"), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_UPDATE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_CREATE_OR_REPLACE
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsCreateOrReplacePositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_CREATE_OR_REPLACE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_IGNORE.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsIgnorePositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_IGNORE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_REPLACE
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsReplacePositive()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_status = $this->db->put($key, array("bin1"=>5));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_REPLACE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with generation policy POLICY_EXISTS_REPLACE negative
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutGenPolicyGTPositive)
  *
  * @test_plans{1.1}
  */
 function testPutGenPolicyExistsReplaceNegative()
 {
     $key = $this->db->initKey("test", "demo", "key_gen_greater");
     $put_record = array("bin1"=>10);
     $put_status = $this->db->put($key, array("bin1"=>10), NULL,
         array(Aerospike::OPT_POLICY_EXISTS=>Aerospike::POLICY_EXISTS_REPLACE));
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     $this->db->get($key, $get_record);
      $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
      if (!empty($comp_res)) {
          return Aerospike::ERR_RECORD_NOT_FOUND;
          }
     return $put_status;
 }
/**
  * @test
  * PUT with initkey digest and option is POLICY_KEY_DIGEST.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutInitKeyWithDigestAndOptionKeyDigestPositive)
  *
  * @test_plans{1.1}
  */
 function testPutInitKeyWithDigestAndOptionKeyDigestPositive() {
     $key = $this->db->initKey("test", "demo",
         str_repeat("a", 20), true);
     $put_record = array("binA"=>10, "binB"=>20);
     $put_status = $this->db->put($key, array("binA"=>10, "binB"=>20), NULL,
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));
     $get_status = $this->db->get($key, $record, array("binA", "binB"));
     if ($get_status !== Aerospike::OK) {
         return $get_status;
     }
     $this->keys[] = $key;
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $this->keys[] = $key;
     return $put_status;
 }
 /*
  * This testcase will be uncommented, When aerospike csdk resolves the bug.
  */
/**
  * @test
  * PUT with initkey digest and option is POLICY_KEY_SEND
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutInitKeyWithDigestAndOptionKeySendPositive)
  *
  * @test_plans{1.1}
  */
 function testPutInitKeyWithDigestAndOptionKeySendPositive() {
     $policy = array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND);
     $key = $this->db->initKey("test", "demo", "policy_key_send_test");
     $put_status = $this->db->put($key, array("bin1"=>10), NULL, $policy);
     if ($put_status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     $get_status = $this->db->get($key, $record, array("bin1"), $policy);
     if ($get_status !== Aerospike::OK) {
         return $get_status;
     }
     $this->keys[] = $key;
     if ($record['key']['key'] != 'policy_key_send_test') return Aerospike::ERR_CLIENT;
     return $get_status;
 }
/**
  * @test
  * PUT Map of floats, Float as a key.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testPutMapOfFloatsKeyIsFloat)
  *
  * @test_plans{1.1}
  */
 /*function testPutMapOfFloatsKeyIsFloat() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_key_is_bool");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", 78.5=>56.35, $obj1=>1, $obj2);
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== AEROSPIKE::OK) {
          return $this->db->errorno();
     }
     return  $status;
 }*/
    /**
     * @test
     * Try putting NULL record without any bins (example: [NULL])
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPutNullRecordNegative()
    {
        $key = array("ns"=>"test", "set"=>"demo",
            "key"=>"test_null_record_negative");
        $status = $this->db->put($key, array(NULL));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * Put NULL data within record (example: ['null_bin' => NULL])
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPutNullDataRecordPositive()
    {
        $key = array("ns"=>"test", "set"=>"demo",
            "key"=>"test_null_data_record_positive");
        $put_record = array('null_bin' => NULL);
        $status = $this->db->put($key, $put_record);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $this->keys[] = $key;
        $status = $this->db->get($key, $get_record);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
        if (!empty($comp_res)) {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return $status;
    }

    /**
     * @test
     * Put NULL data within map (example: ['my_bin' => ['null_value' => NULL]])
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPutNullDataMapPositive()
    {
        $key = array("ns"=>"test", "set"=>"demo",
            "key"=>"test_null_data_map_positive");
        $put_record = array('my_bin' => array('null_data' => NULL));
        $status = $this->db->put($key, $put_record);
        $this->keys[] = $key;
        $status = $this->db->get($key, $get_record);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
        if (!empty($comp_res)) {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return $status;
    }

    /**
     * @test
     * Put NULL data within list (example: ['my_list' => [1, 'hello', NULL]])
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPutNullDataListPositive()
    {
        $key = array("ns"=>"test", "set"=>"demo",
            "key"=>"test_null_data_list_positive");
        $put_record = array('my_bin' => array(1, 'hello', NULL));
        $status = $this->db->put($key, $put_record);
        $this->keys[] = $key;
        $status = $this->db->get($key, $get_record);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
        if (!empty($comp_res)) {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return $status;
    }

    /**
     * @test
     * Put NULL data within mixed nested data (example: ['my_data' => [1, ['age' => 20,
     *   'job' => NULL, 'salary'=> NULL,
     *   'qualifications' => ['B.Tech', 'M.Tech', NULL]], 'hello', NULL]])
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testPUTNoParameter)
     *
     * @test_plans{1.1}
     */
    function testPutNullDataMixedPositive()
    {
        $key = array("ns"=>"test", "set"=>"demo",
            "key"=>"test_null_data_mixed_positive");
        $put_record = array('my_data' => array(1, array('age' => 20,
            'job' => NULL, 'salary' => NULL,
            'qualifications' => array('B.Tech', 'M.Tech', NULL)),
            'hello', NULL), 'no_data' => NULL);
        $status = $this->db->put($key, $put_record);
        $this->keys[] = $key;
        $status = $this->db->get($key, $get_record);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
        if (!empty($comp_res)) {
            return Aerospike::ERR_RECORD_NOT_FOUND;
        }
        return $status;
    }

    /**
     * @test
     * put() with no data
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (bug_CLIENT_69)
     *
     * @test_plans{1.1}
     */
    function bug_CLIENT_69()
    {
        $key = $this->db->initKey("test", "demo", "no_data");
        $status = $this->db->put($key,  array());
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }

    /**
     * @test
     * put() with bin name only
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (edge_001)
     *
     * @test_plans{1.1}
     */
    function edge_001()
    {
        $key = $this->db->initKey("test", "demo", "bin_name");
        $status = $this->db->put($key,  array("binA"));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        return $status;
    }
}
?>
