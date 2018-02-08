<?php
require_once 'Common.inc';

/**
 *Basic Index creation and drop tests
*/
function ensure_dropped_index($client, $ns, $index_name) {
	$retries = 10;
	$code = $client->dropIndex($ns, $index_name);
	while ($retries > 0) {
		$found = false;
		$responses =  $client->infoMany("sindex");
		foreach($responses as $node => $response) {
			if (strpos($index_name, $response) !== false) {
				$found = true;
			}
		}
		if ($found) {
			$retries--;
			sleep(1);
		} else {
			return;
		}
	}
	return;
}

class Index extends AerospikeTestCommon
{
    function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $key = $this->db->initKey("test", "demo", "key1");
        $this->db->put($key, array("bin1"=>"1", "bin2"=>2, "bin3"=>3, "numeric_list"=>array(1,2,3)));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "key2");
        $this->db->put($key, array("string_list"=>array('a', 'b', 'c', 'd', 'e')));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "key3");
		$this->db->put($key, array("str_map_keys"=>array('a' =>1, 'b' => 2, 'c' => 3),
            "str_map_vals"=>array('a' => 'x', 'b' => 'y', 'c' => 'z')));
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "key4");
		$this->db->put($key, array("num_mapkeys"=>array(1 => 'aa', 2 => 'bb', 3 => 'cc'),
			"num_map_vals"=>array('a' => 11, 'b' => 12, 'c' => 13)));
        $this->keys[] = $key;
   }


    /**
     * @test
     * Basic index create and drop operation on integer bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testCreateAndDropIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinPositive() {
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC)) {
			$code = $this->db->dropIndex("test", "bin2_integer_index");
			ensure_dropped_index($this->db, "test", "bin2_integer_index");
			return $code;
        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Basic index create and drop operation on integer bin with options
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexStringBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinWithOptionsPositive() {
		ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC,
            array(Aerospike::OPT_WRITE_TIMEOUT => 12000))) {
			$code = $this->db->dropIndex("test", "bin2_integer_index",
										 [Aerospike::OPT_WRITE_TIMEOUT => 10000]);
			ensure_dropped_index($this->db, "test", "bin2_integer_index");
        	return $code;
        } else {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Index recreate and drop operation on integer bin with same name on which index is already created
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexSameIntegerBinWithSameNamePositive() {
		ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_NUMERIC)) {
			$code = $this->db->addIndex("test", "demo", "bin2",
					"bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT,
					Aerospike::INDEX_NUMERIC);
			ensure_dropped_index($this->db, "test", "bin2_integer_index");
			return $code;
        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Index create and drop operation with same name as an existing index on a different integer bin. Should raise an error
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexDifferentIntegerBinWithSameNamePositive() {
    	ensure_dropped_index($this->db, "test", "test_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "test_index", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC)) {
			$code = $this->db->addIndex("test", "demo", "bin3",
					"test_index", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);
			ensure_dropped_index($this->db, "test", "test_index");
			return $code;

        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Index create and drop operation with new index with different name on an already
     * indexed integer bin. This should raise an index exists error
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexSameIntegerBinWithDifferentNamePositive() {
    	ensure_dropped_index($this->db, "test", "test_index");
    	ensure_dropped_index($this->db, "test", "test_index_new");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "test_index", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC)) {
            $code = $this->db->addIndex("test", "demo", "bin2",
            		"test_index_new", Aerospike::INDEX_TYPE_DEFAULT, Aerospike::INDEX_NUMERIC);
        	ensure_dropped_index($this->db, "test", "test_index");
        	ensure_dropped_index($this->db, "test", "test_index_new");
        	return $code;
        } else {
            return $this->db->errorno();
        }
     }

    /**
     * @test
     * Basic index create and drop operation on string bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexStringBinPositive() {
    	ensure_dropped_index($this->db, "test", "bin1_string_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin1",
            "bin1_string_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING)) {
            	$code = $this->db->dropIndex("test", "bin1_string_index");
            	ensure_dropped_index($this->db, "test", "bin1_string_index");
            	return $code;
            } else {
                return $this->db->errorno();
            }
    }
    /**
     * @test
     * Basic index create and drop operation on string bin with set as NULL
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexStringSetNull() {
        if (Aerospike::OK == $this->db->addIndex("test", NULL, "bin1",
            "bin1_string_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING)) {
                return($this->db->dropIndex("test", "bin1_string_index"));
            } else {
                return $this->db->errorno();
            }
    }

    /**
     * @test
     * Basic index create and drop operation on string bin with options
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexStringBinWithOptionsPositive() {
    	ensure_dropped_index($this->db, "test", "bin1_string_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin1",
            "bin1_string_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING, array(Aerospike::OPT_WRITE_TIMEOUT => 10000))) {
                $code = $this->db->dropIndex("test", "bin1_string_index",
                		array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
                ensure_dropped_index($this->db, "test", "bin1_string_index");
                return $code;
            } else {
                return $this->db->errorno();
            }
    }
    /**
     * @test
     * Index recreate and drop operation on string bin with same name on which index is already created
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexSameStringBinWithSameNamePositive() {
    	ensure_dropped_index($this->db, "test", "bin2_string_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "bin2_string_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING)) {
        		$code = $this->db->addIndex("test", "demo", "bin2",
        				"bin2_string_index", Aerospike::INDEX_TYPE_DEFAULT,
						Aerospike::INDEX_STRING);
            	ensure_dropped_index($this->db, "test", "bin2_string_index");
                return $code;
        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Index create and drop operation with same name as an existing index on a different string bin.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexDifferentStringBinWithSameNamePositive() {
    	ensure_dropped_index($this->db, "test", "test_index");

        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "test_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING)) {
            $code = $this->db->addIndex("test", "demo", "bin3",
            		"test_index", Aerospike::INDEX_TYPE_DEFAULT,
					Aerospike::INDEX_STRING);
        	ensure_dropped_index($this->db, "test", "test_index");
            return $code;

        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Index create and drop operation with new index with different name on an already
     * indexed integer bin.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexSameStringBinWithDifferentNamePositive() {
    	ensure_dropped_index($this->db, "test", "test_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo", "bin2",
            "test_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_STRING)) {
            $code = $this->db->addIndex("test", "demo", "bin2",
				"test_index_new", Aerospike::INDEX_TYPE_DEFAULT,
				Aerospike::INDEX_STRING);
        	ensure_dropped_index($this->db, "test", "test_index");
            return $code;
        } else {
            return $this->db->errorno();
        }
     }
    /**
     * @test
     * Positive index create and drop operation on integer bin: non existing bin
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinInvalidBinPositive() {
    	ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo",
            "invalid_bin", "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_NUMERIC)) {
                $code = $this->db->dropIndex("test", "bin2_integer_index");
                ensure_dropped_index($this->db, "test", "bin2_integer_index");
                return $code;
            } else {
                return $this->db->errorno();
            }
    }

    /**
     * @test
     * Positive index create and drop operation on integer bin: non existing set
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinInvalidSetPositive() {
		ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("test", "invalid_set",
            "bin2", "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_NUMERIC)) {
            $code = $this->db->dropIndex("test", "bin2_integer_index");
            ensure_dropped_index($this->db, "test", "bin2_integer_index");
            return $code;
        } else {
            return $this->db->errorno();
        }
    }


    /**
     * @test
     * Negative index create and drop operation on integer bin: non existing namespace
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinInvalidNamespaceNegative() {
		ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("invalid_namespace", "demo",
            "bin2", "bin2_integer_index", Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_NUMERIC)) {
                return($this->db->dropIndex("test", "bin2_integer_index"));
            } else {
                return $this->db->errorno();
            }
    }

    /**
     * @test
     * Positive index create and drop operation on integer bin: invalid index
     * type, will use the default string index type.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinInvalidIndexTypePositive() {
		ensure_dropped_index($this->db, "test", "bin2_integer_index");
        if (Aerospike::OK == $this->db->addIndex("test", "demo",
            "bin2", "bin2_integer_index", 10,
            Aerospike::INDEX_NUMERIC)) {
            $code = $this->db->dropIndex("test", "bin2_integer_index");
			ensure_dropped_index($this->db, "test", "bin2_integer_index");
			return $code;
        } else {
            return($this->db->errorno());
        }
    }

    /**
     * @test
     * Negative index create and drop operation on integer bin: very long index name
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testaddIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testCreateAndDropIndexIntegerBinVeryLongIndexNameNegativeWithDBCrash() {
    	$long_name = "bin2_integer_indexsdadadfasdfasdfeartfqrgahfas;dfheudsdfasdfawf312342q3453rf9qwfasdcfasdcalskdcbacfq34915rwcfasdcas.cnabscbaskjdbcalsjkbcdasc";
        if (Aerospike::OK == $this->db->addIndex("test", "demo",
            "bin2", $long_name, Aerospike::INDEX_TYPE_DEFAULT,
            Aerospike::INDEX_NUMERIC)) {
				$code = $this->db->dropIndex("test", $long_name);
				ensure_dropped_index($this->db, "test", $long_name);
				return $code;

        } else {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Negative index drop operation on integer bin: drop a non existing index
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testdropIndexIntegerBinPositive)
     *
     * @test_plans{1.1}
     */
    function testDropIndexIntegerBinInvalidIndexNegative() {
        return($this->db->dropIndex("test", "invalid_index"));
    }
    /**
     * @test
     * Basic index create on list and datatype is string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexListStringPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexListStringPositive() {
        $status = $this->db->addIndex("test", "demo", "string_list",
			"index_list_string", Aerospike::INDEX_TYPE_LIST,
			Aerospike::INDEX_STRING, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_list_string");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
		return $status;
    }
    /**
     * @test
     * Basic index create on list and datatype is integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexListNumericPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexListNumericPositive() {
        $status = $this->db->dropIndex("test", "index_list_numeric");
        ensure_dropped_index($this->db, "test", "index_list_numeric");
        $status = $this->db->addIndex("test", "demo", "numeric_list",
			"index_list_numeric", Aerospike::INDEX_TYPE_LIST,
			Aerospike::INDEX_NUMERIC, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_list_numeric");
        ensure_dropped_index($this->db, "test", "index_list_numeric");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
		return $status;
    }
    /**
     * @test
     * Basic index create on map keys and datatype is string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexMapKeysStringPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexMapKeysStringPositive() {
        $status = $this->db->dropIndex("test", "index_mapkeys_string");
        ensure_dropped_index($this->db, "test", "index_mapkeys_string");
        $status = $this->db->addIndex("test", "demo", "str_map_keys",
			"index_mapkeys_string", Aerospike::INDEX_TYPE_MAPKEYS,
            Aerospike::INDEX_STRING, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_mapkeys_string");
        ensure_dropped_index($this->db, "test", "index_mapkeys_string");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
        }
		return $status;
    }
    /**
     * @test
     * Basic index create on map keys and datatype is numeric.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexMapKeysNumericPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexMapKeysNumericPositive() {
        ensure_dropped_index($this->db, "test", "index_mapkeys_numeric");
        $status = $this->db->addIndex("test", "demo", "nume_mapkeys",
			"index_mapkeys_numeric", Aerospike::INDEX_TYPE_MAPKEYS,
			Aerospike::INDEX_NUMERIC, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_mapkeys_numeric");
        ensure_dropped_index($this->db, "test", "index_mapkeys_numeric");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
		return $status;
    }
    /**
     * @test
     * Basic index create on map values and datatype is string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexMapValuesStringPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexMapValuesStringPositive() {
        ensure_dropped_index($this->db, "test", "index_mapvalues_string");
        $status = $this->db->addIndex("test", "demo", "str_map_vals",
			"index_mapvalues_string", Aerospike::INDEX_TYPE_MAPVALUES,
			Aerospike::INDEX_STRING, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_mapvalues_string");
        ensure_dropped_index($this->db, "test", "index_mapvalues_string");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
		return $status;
    }
    /**
     * @test
     * Basic index create on map values and datatype is numeric.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testAddIndexMapValuesNumericPositive)
     *
     * @test_plans{1.1}
     */
    function testAddIndexMapValuesNumericPositive() {
        ensure_dropped_index($this->db, "test", "index_mapvalues_numeric");
        $status = $this->db->addIndex("test", "demo", "num_map_vals",
			"index_mapvalues_numeric", Aerospike::INDEX_TYPE_MAPKVALUES,
			Aerospike::INDEX_NUMERIC, array(Aerospike::OPT_WRITE_TIMEOUT => 12000));
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
        $status = $this->db->dropIndex("test", "index_mapvalues_numeric");
        ensure_dropped_index($this->db, "test", "index_mapvalues_numeric");
		if ($status != AEROSPIKE::OK) {
			return $this->db->errorno();
		}
		return $status;
    }
}
?>
