<?php

require_once 'Common.inc';
/**
 *Basic Get tests
*/
class Employee
{
    public $desg = 'Manager';
}

class Get extends AerospikeTestCommon
{
    /*
     * TODO: Remove these constants and map with actual Aerospike class
     * constants.
     */
    static $put_rec = array("Greet"=>"World_end");

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "Get_key");
        $this->db->put($key,array("Greet"=>"World_end"));
        $this->keys[] = $key;
    }

    /**
     * @test
     * Basic GET without parameter
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
    function testGETNoParameter() 
    {
        try {
            $this->db->get();
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Basic GET
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETTwoParameter)
     *
     * @test_plans{1.1}
     */
    function testGETTwoParameter() 
    {
        $status = $this->db->get($this->keys[0],$bin_arr);
        if ($status !== Aerospike::OK) {
            return $status;
        }
        if (strcmp($bin_arr["key"]["ns"], $this->keys[0]["ns"]) == 0 &&
            strcmp($bin_arr["key"]["set"], $this->keys[0]["set"]) == 0 &&
            strcmp($bin_arr["key"]["key"], NULL) == 0) {
            $comp_res = array_diff_assoc_recursive(self::$put_rec, $bin_arr["bins"]);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
        } else {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic GET Three Parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETThreeParameter)
     *
     * @test_plans{1.1}
     */
    function testGETThreeParameter()
    {
        $bin_arr = array("Greet");
        $status = $this->db->get($this->keys[0], $rec_arr, $bin_arr, array(Aerospike::OPT_READ_TIMEOUT=>2000));
        if ($status !== Aerospike::OK) {
            return $status;
        }
        if (strcmp($rec_arr["key"]["ns"], $this->keys[0]["ns"]) == 0 &&
            strcmp($rec_arr["key"]["set"], $this->keys[0]["set"]) == 0 &&
            strcmp($rec_arr["key"]["key"], NULL) == 0) {
            $comp_res = array_diff_assoc_recursive(self::$put_rec, $rec_arr["bins"]);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
        } else {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic GET wrong Parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGETThreeParameter)
     *
     * @test_plans{1.1}
     */
    function testGETWrongParameter() 
    {
        try {
            $this->db->get($this->keys[0]);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }
    /**
     * @test
     * Basic GET Second Parameter Array
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
        $rec_array = "";
        $status = $this->db->get($this->keys[0], $get_record);
        if ($status !== Aerospike::OK) {
            return $status;
        }
        if (strcmp($get_record["key"]["ns"], $this->keys[0]["ns"]) == 0 &&
            strcmp($get_record["key"]["set"], $this->keys[0]["set"]) == 0 &&
            strcmp($get_record["key"]["key"], NULL) == 0) {
            $comp_res = array_diff_assoc_recursive(self::$put_rec, $get_record["bins"]);
            if (!empty($comp_res)) {
                return Aerospike::ERR_CLIENT;
            }
        } else {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
    /**
     * @test
     * Basic GET Third Parameter Array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCheckThirdParameterTypeArray)
     *
     * @test_plans{1.1}
     */
    function testCheckThirdParameterTypeArray()
    {
        try {
            $this->db->get($this->keys[0], $rec, "");
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $this->db->errorno();
    }
    /**
     * @test
     * Basic GET Check key parameter missing in key array
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
        try {
            $key1 = $this->db->initKey("test", "demo");
            $rv = $this->db->get($key1, $rec);
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return Undefined_Error;
    }

  /**
     * @test
     * Basic GET Check ns(namespace) parameter missing in key array
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
        $key1 = array("set"=>"demo", "key"=>"10000");
        return($this->db->get($key1, $rec));
    }
    /**
     * @test
     * Basic GET Check set parameter missing in key array
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
        $key1 = array("ns"=>"test","key"=>"10000");
        return $this->db->get($key1,$rec);
    }
        /**
         * @test
         * Basic GET Check parameter sequence change in  key array
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
            $key1 = $this->db->initKey("demo", "test", "key_int1");
            return $this->db->get($key1,$rec);
        }
        /**
         * @test
         * Basic GET Check set parameter value non exist in database
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
            $key1 = $this->db->initKey("test", "ssss---ssss", "key_int1");
            return $this->db->get($key1,$rec);
        }
/**
  * @test
  * Basic GET Check namespace parameter value non exist in database
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
      $key1 = $this->db->initKey("sss--ss", "demo", "key_int1");
      return $this->db->get($key1,$rec);

  }

/**
  * @test
  * Basic GET Check key parameter value non exist in database
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
      $key1 = $this->db->initKey("test", "demo", "ss--sss");
      return $this->db->get($key1,$rec);
  }
/**
  * @test
  * Basic Get list from single bin
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
      $key1 = $this->db->initKey("test", "demo", "10000");
      $status = $this->db->put($key1,array("bin1"=>array("speaking", "reading", "writing")));
      $this->keys[] = $key1;
      if ($status !== Aerospike::OK) {
          return $status;
      }
      $status = $this->db->get($key1,$get_rec);
      if ($status !== Aerospike::OK) {
          return $status;
      }
      if (strcmp($get_rec["key"]["ns"], $key1["ns"]) == 0 &&
          strcmp($get_rec["key"]["set"], $key1["set"]) == 0 &&
          strcmp($get_rec["key"]["key"], NULL) == 0) {
          $comp_res = array_diff_assoc_recursive(array("bin1"=>array("speaking", "reading", "writing")), $get_rec["bins"]);
          if (!empty($comp_res)) {
              return Aerospike::ERR_CLIENT;
          }
      } else {
          return Aerospike::ERR_CLIENT;
      }
      return $status;
  }
/**
  * @test
  * Basic Get Map from single bin
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
      $key1 = $this->db->initkey("test", "demo", "10000");
      $map1= array("k1"=>10, "k2"=>5, "k3"=>6, "k4"=>7, "k5"=>8);
      $status = $this->db->put($key1, array("bin1"=>$map1));
      $this->keys[] = $key1;
      if ($status !== Aerospike::OK) {
          return $status;
      }
      $status = $this->db->get($key1,$get_rec, NULL,
          array(Aerospike::OPT_READ_TIMEOUT=>2000,
          Aerospike::OPT_POLICY_REPLICA=>Aerospike::POLICY_REPLICA_ANY,
          Aerospike::OPT_POLICY_READ_MODE_AP=>Aerospike::POLICY_READ_MODE_AP_ONE));
      if ($status !== Aerospike::OK) {
          return $status;
      }
      if (strcmp($get_rec["key"]["ns"], $key1["ns"]) == 0 &&
          strcmp($get_rec["key"]["set"], $key1["set"]) == 0 &&
          strcmp($get_rec["key"]["key"], NULL) == 0) {
          $comp_res = array_diff_assoc_recursive(array("bin1"=>$map1), $get_rec["bins"]);
          if (!empty($comp_res)) {
              return Aerospike::ERR_CLIENT;
          }
      } else {
          return Aerospike::ERR_CLIENT;
      }
      return $status;
  }
  /**
  * @test
  * Get a bin with an empty map and verify it converts to an empty PHP array
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testEmptyMapToEmptyArray)
  *
  * @test_plans{1.1}
  */
  function testEmptyMapToEmptyArray()
  {
      $key = array("ns"=>"test","set"=>"demo","key"=>"aerospike_issue_14");
      $status = $this->db->put($key, array("array_bin"=>array()), 10);
      $this->keys[] = $key;
      if ($status !== Aerospike::OK) {
          return $this->db->errorno();
      }
      $status = $this->db->get($key, $get_rec);
      if ($status !== Aerospike::OK) {
          return $status;
      }
      if ($get_rec['bins']['array_bin'] !== array()) {
          var_dump($get_rec['bins']['array_bin']);
          return Aerospike::ERR_CLIENT;
      }
      return Aerospike::OK;
  }
/**
  * @test
  * Basic Get Map and List combine from single bin
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
      $key1 = $this->db->initkey("test", "demo", "10000");
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
      $status = $this->db->put($key1,$rec);
      $this->keys[] = $key1;
      if ($status !== Aerospike::OK) {
          return $status;
      }
      $status = $this->db->get($key1,$get_rec);
      if ($status !== Aerospike::OK) {
          return $status;
      }
      if (strcmp($get_rec["key"]["ns"], $key1["ns"]) == 0 &&
          strcmp($get_rec["key"]["set"], $key1["set"]) == 0 &&
          strcmp($get_rec["key"]["key"], NULL) == 0) {
          $comp_res = array_diff_assoc_recursive($rec, $get_rec["bins"]);
          if (!empty($comp_res)) {
              return Aerospike::ERR_CLIENT;
          }
      }
      return $status;
  }
/**
  * @test
  * Basic Get single bin missing
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testCheckBinMissing)
  *
  * @test_plans{1.1}
  */
  function testCheckBinMissing()
  {
      return($this->db->get($this->keys[0],$rec,array("bin1","bin2","00000000000bin1000000")));
  }

/**
  * @test
  * Get Nested List of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 #Testcases for object 
 function testGetNestedListOfObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     $this->keys[] = $key;
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

      if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
          strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
          strcmp($get_record["key"]["key"], NULL) == 0) {
          $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
          if (!empty($comp_res)) {
              return Aerospike::ERR_RECORD_NOT_FOUND;
          }
      }
     return $status;
 }

/**
  * @test
  * GET Nested List of objects with UDF Serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedListOfObjectsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key,$put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     $this->keys[] = $key;
     if ($status !== Aerospike::OK) {
         return $status;
     }

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested map of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map1 = array(12=>$obj1, $obj3);
     $map2 = array($map1, $obj2, " ");
     $map3 = array("k1", $obj4, 56=>$map2);

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested map of objects with UDF Serializer.   
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfObjectsWithUDFSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_objects_with_UDF_serializer");

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

     $map1 = array(12=>$obj1, $obj3);
     $map2 = array($map1, $obj2, " ");
     $map3 = array("k1", $obj4, 56=>$map2);

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of objects with PHP Serializer.    
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();

     $map = array(12=>"k5", "k78"=>$obj1, $obj2);
     $list = array("k1", $obj3, $map);
     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of objects with UDF Serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapObjectsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Map containing List of objects with PHP Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListObjectsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListObjectsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_objects_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();

     $map = array(12=>"k5", "k78"=>$obj1);
     $list = array("k1", $obj3, $obj2, $map);

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
 
/**
  * @test
  * GET Map containing List of objects with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListObjectsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListObjectsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET PUT Nested List of bools with PHP serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 #Testcases for bool
 function testGetNestedListOfBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_bools_with_PHP_serializer");

     $list1 = array(12, TRUE);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, FALSE);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested List of bools with UDF serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedListOfBoolsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested Map of bools with PHP serializer.   
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_with_PHP_serializer");

     $map1 = array(12=>TRUE, FALSE);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested Map of bools with UDF serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfBoolsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of bools with PHP serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_bools_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, FALSE, $obj2, "aa"=>$obj3);
     $list = array("k1", $obj4, $map, 55=>TRUE);

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of bools with UDF serializer. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapBoolsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Map containig List of bools with PHP serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListBoolsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListBoolsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_bools_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 56, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1);

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
 
/**
  * @test
  * GET Map containing List of bools with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListBoolsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListBoolsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested List of floats with PHP serializer.   
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 #Testcases for float
 function testGetNestedListOfFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_of_floats_with_PHP_serializer");

     $list1 = array(12, 11.2);
     $list2 = array($list1, " ");
     $list3 = array("k1", 56, $list2, 56.896);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested List of floats with UDF serializer. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedListOfFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedListOfFloatsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested Map of floats with PHP serializer. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_of_floats_with_PHP_serializer");

     $map1 = array(12=>89.4, 55.44);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Nested Map of floats with UDF serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetNestedMapOfFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetNestedMapOfFloatsWithUDFSerializer() {
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

     $map1 = array(12=>89.4, 55.44);
     $map2 = array($map1, "K5"=>" ");
     $map3 = array("k1", TRUE, 56=>$map2);

     $put_record = array("bin1"=>$map3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of floats with PHP serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "list_map_floats_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $map = array(12=>$obj1, $obj2, "aa"=>25.6, "k10"=>TRUE);
     $list = array("k1", $obj4, $map, 55=>89.56, FALSE, 45.78);

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET List containing Map of floats with UDF serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetListMapFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetListMapFloatsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET Map containing List of floats with PHP serializer.  
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListFloatsWithPHPSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListFloatsWithPHPSerializer() {
     $key = $this->db->initKey("test", "demo", "map_list_floats_with_PHP_serializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list = array(12, $obj2, 63.2, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1, "k22"=>56.75);

     $put_record = array("bin1"=>$list);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
 
/**
  * @test
  * GET Map containing List of floats with UDF Serializer.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetMapListFloatsWithUDFSerializer)
  *
  * @test_plans{1.1}
  */
 function testGetMapListFloatsWithUDFSerializer() {
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

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET object which is serialized using SERIALIZER_PHP and deserialized 
  * with SERIALIZER_USER.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetObjectPHPSerializedAndUDFDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetObjectPHPSerializedAndUDFDeserialized() {
     $key = $this->db->initKey("test", "demo", "list_of_objects_with_PHP_SERIALIZER_USER_deserializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET object which is serialized using SERIALIZER_USER and deserialized 
  * with SERIALIZER_PHP.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetObjectUDFSerializedAndPHPDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetObjectUDFSerializedAndPHPDeserialized() {
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

     $list = array(12, $obj2, 63.2, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1, "k22"=>56.75);

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET bool which is serialized using SERIALIZER_PHP and deserialized 
  * with SERIALIZER_USER.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetBoolPHPSerializedAndUDFDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetBoolPHPSerializedAndUDFDeserialized() {
     $key = $this->db->initKey("test", "demo",
         "list_of_bools_with_PHP_SERIALIZER_USER_deserializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET bool which is serialized using SERIALIZER_USER and deserialized 
  * with SERIALIZER_PHP.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetBoolUDFSerializedAndPHPDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetBoolUDFSerializedAndPHPDeserialized() {
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

     $list = array(12, $obj2, 63.2, " ", $obj4, FALSE);
     $map = array("k1"=>$obj3, "k8"=>TRUE, 56=>$list, $obj1, "k22"=>56.75);

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET float which is serialized using SERIALIZER_PHP and deserialized 
  * with SERIALIZER_USER.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetFloattPHPSerializedAndUDFDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetFloatPHPSerializedAndUDFDeserialized() {
     $key = $this->db->initKey("test", "demo", "list_of_floats_with_PHP_SERIALIZER_USER_deserializer");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();
     $obj3 = new Employee();
     $obj4 = new Employee();

     $list1 = array(12, $obj1, $obj3);
     $list2 = array($list1, $obj2, " ");
     $list3 = array("k1", $obj4, $list2);

     $put_record = array("bin1"=>$list3);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     #Set Deserializer
     Aerospike::setDeserializer(function ($val) {
         $prefix = substr($val, 0, 3);
         if ($prefix !== 'r||') {
             return unserialize(substr ($val, 3));
         }
         return unserialize(substr ($val, 3));
     });

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test
  * GET float which is serialized using SERIALIZER_USER and deserialized 
  * with SERIALIZER_PHP.
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetFloatUDFSerializedAndPHPDeserialized)
  *
  * @test_plans{1.1}
  */
 function testGetFloatUDFSerializedAndPHPDeserialized() {
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

     $put_record = array("bin1"=>$map);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_USER));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
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
  * Variants: OO (testGetMapOfBoolsKeyIsBool)
  *
  * @test_plans{1.1}
  */
 function testGetMapOfBoolsKeyIsBool() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_key_is_bool");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", TRUE=>$obj2, $obj1);
     $status = $this->db->put($key, array("bin1"=>$map), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }

     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
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
  * Variants: OO (testGetMapOfFloatsKeyIsFloat)
  *
  * @test_plans{1.1}
  */
 function testGetMapOfFloatsKeyIsFloat() {
     $key = $this->db->initKey("test", "demo", "map_of_bools_key_is_bool");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", 78.5=>56.35, $obj1=>1, $obj2);
     $status = $this->db->put($key, array("bin1"=>$map), NULL, array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $this->keys[] = $key;
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test  
  * Get a record from DB, POLICY_KEY_SEND is passed as a option. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetWithPolicyKeySend)
  *
  * @test_plans{1.1}
  */
 function testGetWithPolicyKeySend() {
     $key = $this->db->initKey("test", "demo", "policy_key_send");
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record);

     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], $key["key"]) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }

/**
  * @test  
  * Get a record from DB, POLICY_KEY_DIGEST is passed as a option. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetWithDigestKeyOptions)
  *
  * @test_plans{1.1}
  */
 function testGetWithPolicyKeyDigest() {
     $key = $this->db->initKey("test", "demo", "policy_key_digest");
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record);

     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
/**
  * @test  
  * Get a record from DB, POLICY_KEY_SEND and read timeout is passed in options. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetWithPolicyKeySendAndReadTimeout)
  *
  * @test_plans{1.1}
  */
 function testGetWithPolicyKeySendAndReadTimeout() {
     $key = $this->db->initKey("test", "demo", "policy_key_send_and_read_timeout");
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record);

     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_READ_TIMEOUT=>2000,
         Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], $key["key"]) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
/**
  * @test  
  * Get a record from DB, POLICY_KEY_DIGEST and read timeout is passed in
    options. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetWithPolicyKeyDigestAndReadTimeout)
  *
  * @test_plans{1.1}
  */
 function testGetWithPolicyKeyDigestAndReadTimeout() {
     $key = $this->db->initKey("test", "demo", "policy_key_digest_and_read_timeout");
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record);

     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_READ_TIMEOUT=>2000,
         Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], NULL) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
/**
  * @test  
  * Get a record from DB, POLICY_KEY_SEND and read timeout is passed in
    options. Also, While Put POLICY_KEY_SEND option is passed. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetAndPutWithPolicyKeySend)
  *
  * @test_plans{1.1}
  */
 function testGetAndPutWithPolicyKeySend() {
     $key = $this->db->initKey("test", "demo", "policy_key_send_for_get_and_put");
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record, NULL, array(Aerospike::OPT_WRITE_TIMEOUT=>2000, Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }
     $this->keys[] = $key;

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_READ_TIMEOUT=>2000,
         Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], $key["key"]) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }
 /*
  * This testcase will be uncommented, When aerospike csdk fault resolved.
  */
/**
  * @test  
  * Get a record from DB, initkey is done with digest and while put
    POLICY_KEY_DIGEST is passed and for get POLICY_KEY_SEND is passed in options. 
  *
  * @pre
  * Connect using aerospike object to the specified node
  *
  * @post
  * newly initialized Aerospike objects
  *
  * @remark
  * Variants: OO (testGetPolicyKeySendAndPutKeyDigestNegative)
  *
  * @test_plans{1.1}
  */
 /*function testGetPolicyKeySendAndPutKeyDigestNegative() {
     $key = $this->db->initKey("test", "demo", base64_decode( "initkey_digest"), true);
     $put_record = array("bin1"=>45);
     $status = $this->db->put($key, $put_record,
         Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_DIGEST));

     if ($status !== Aerospike::OK) {
         return $this->db->errorno();
     }

     $status = $this->db->get($key, $get_record, array("bin1"),
         array(Aerospike::OPT_READ_TIMEOUT=>2000,
         Aerospike::OPT_POLICY_KEY=>Aerospike::POLICY_KEY_SEND));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     if (strcmp($get_record["key"]["ns"], $key["ns"]) == 0 &&
         strcmp($get_record["key"]["set"], $key["set"]) == 0 &&
         strcmp($get_record["key"]["key"], $key["key"]) == 0) {
         $comp_res = array_diff_assoc_recursive($put_record, $get_record["bins"]);
         if (!empty($comp_res)) {
             return Aerospike::ERR_RECORD_NOT_FOUND;
         }
     }
     return $status;
 }*/
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
  * Variants: OO (testGetMapOfObjectsKeyIsObject)
  *
  * @test_plans{1.1}
  */
 function testGetMapOfObjectsKeyIsObject() {
     /*
     $key = $this->db->initKey("test", "demo", "map_of_objects_key_is_object");

     #Creating objects
     $obj1 = new Employee();
     $obj2 = new Employee();

     $map = array("k1", $obj1=>$obj2, "k2");
     $status = $this->db->put($key, array("bin1"=>$map), NULL,
         array(Aerospike::OPT_SERIALIZER => Aerospike::SERIALIZER_PHP));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $status = $this->db->get($key, $get_record, array("bin1"));
     if ($status !== Aerospike::OK) {
         return $status;
     }
     $comp_res = array_diff_assoc_recursive($put_record, $get_record);
     if (!empty($comp_res)) {
         return Aerospike::ERR_RECORD_NOT_FOUND;
     }*/
    }
    /**
     * @test
     * Basic GET with bin as float.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (normal_001)
     */
    function normal_001()
    {
        $key = $this->db->initKey("test", "demo", "put_float");
        $status = $this->db->put($key,  array("float"=>3.145));
        $this->keys[] = $key;
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        $status = $this->db->get($key, $return);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (abs($return["bins"]["float"] - 3.145) > 0.00001)
        {
            return Aerospike::ERR_CLIENT;
        }
        return $status;
    }
}
?>
