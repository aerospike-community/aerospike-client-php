<?php
require_once 'Common.inc';

/**
 *Basic getMany opeartion tests
*/
class GetMany extends AerospikeTestCommon
{
    private $put_records = array();

    protected function setUp() {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
        $key = $this->db->initKey("test", "demo", "getMany1");
        $put_record1 = array("binA"=>10, "binB"=>20, "binC"=>30);
        $this->db->put($key, $put_record1);
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "getMany2");
        $put_record2 = array("binA"=>40, "binB"=>50, "binC"=>60);
        $this->db->put($key, $put_record2);
        $this->keys[] = $key;
        $key = $this->db->initKey("test", "demo", "getMany3");
        $put_record3 = array("binA"=>70, "binB"=>80, "binC"=>90);
        $this->db->put($key, $put_record3);
        $this->keys[] = $key;
        $this->put_records = array($put_record1, $put_record2, $put_record3);
    }

    /**
     * @test
     * Basic getMany operation with all arguments without options and filter bins.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositive() {
        $key4 = $this->db->initKey("test", "demo", "getMany4");
        $my_keys = $this->keys + array($key4);
        $my_put_records = $this->put_records;
        $my_put_records[3] = NULL;
        $status = $this->db->getMany($my_keys, $records);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($my_keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($my_keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($my_keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($my_put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with all arguments without options and filter
     * bins but with one non-existent key.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyWithNonExistentKeyPositive() {
        $my_keys = $this->keys;
        $key4 = array($this->db->initKey("test", "demo", "getMany4"));
        array_splice($my_keys, 1, 0, $key4);
        $my_put_records = $this->put_records;
        $my_put_records[3] = NULL;
        $status = $this->db->getMany($my_keys, $records);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ($i == 3 && $value) {
                return Aerospike::ERR_CLIENT;
            } else {
                continue;
            }
            if ((!strcmp($my_keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($my_keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($my_keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($my_put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with all arguments without options and filter bins with digest in initKey().
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositiveWithDigest() {
        $key4 = $this->db->initKey("test", "demo", "getMany4", true);
        $put_record = array("binD"=>50);
        $this->db->put($key4, $put_record);
        $this->keys[] = $key4;

        $my_keys = $this->keys + array($key4);
        $my_put_records = $this->put_records;
        $my_put_records[3] = NULL;
        $status = $this->db->getMany($my_keys, $records);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ($i == 3) {
                if ($value["key"]["digest"] != "getMany4") {
                    return Aerospike::ERR_CLIENT;
                } else {
                    return Aerospike::OK;
                }
            }
            if ((!strcmp($my_keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($my_keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($my_keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($my_put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with all arguments with options without filter bins.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyWithOptionsPositive() {
        $key4 = $this->db->initKey("test", "demo", "getMany4");
        $my_keys = $this->keys + array($key4);
        $my_put_records = $this->put_records;
        $my_put_records[3] = NULL;
        $status = $this->db->getMany($my_keys, $records, NULL,
            array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== Aerospike::OK) {
        	echo $this->db->error();
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($my_keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($my_keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($my_keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($my_put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with filter bins but without options.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositiveWithFilterBins() {
        $status = $this->db->getMany($this->keys, $records, array("binA", "binC"));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i=0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($this->keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($this->keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($this->keys[$i]["set"], $value["key"]["set"])) &&
                count($value["bins"]) == 2) {
                    $result = array_diff_assoc_recursive($this->put_records[$i],
                        $value["bins"]);
                    if (!array_key_exists("binB", $result) || empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                } else {
                    return Aerospike::ERR_CLIENT;
                }
            $i++;
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with filter bins and options.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositiveWithFilterBinsAndOptions() {
        $status = $this->db->getMany($this->keys, $records, array("binA", "binC"),
            array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i=0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($this->keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($this->keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($this->keys[$i]["set"], $value["key"]["set"])) &&
                count($value["bins"]) == 2) {
                    $result = array_diff_assoc_recursive($this->put_records[$i],
                        $value["bins"]);
                    if (!array_key_exists("binB", $result) || empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                } else {
                    return Aerospike::ERR_CLIENT;
                }
            $i++;
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with empty filter bins.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositiveWithEmptyFilterBins() {
        $status = $this->db->getMany($this->keys, $records, array());
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i=0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($this->keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($this->keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($this->keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($this->put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                } else {
                    return Aerospike::ERR_CLIENT;
                }
            $i++;
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with invalid bin in filter bins.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyPositive)
     *
     * @test_plans{1.1}
     */
    function testGetManyPositiveWithInvalidFilterBins() {
        $status = $this->db->getMany($this->keys, $records, array("INVALID_BIN"));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }

        $i=0;
        foreach ($records as $records_key=>$value) {
            if (strcmp($this->keys[$i]["key"], $value["key"]["key"]) ||
                strcmp($this->keys[$i]["ns"], $value["key"]["ns"]) ||
                strcmp($this->keys[$i]["set"], $value["key"]["set"]) ||
                count($value["bins"]) != 0) {
                    return Aerospike::ERR_CLIENT;
                }
            $i++;
        }
        return $status;
    }
    /**
     * @test
     * Basic getMany operation with empty key array.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyEmptyKeyArrayNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyEmptyKeyArrayNegative() {
        $keys = array();
        $status = $this->db->getMany($keys, $records, NULL,
            array(Aerospike::OPT_READ_TIMEOUT=>3000));
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (!empty($records)) {
            return Aerospike::ERR_CLIENT;
        } else {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic getMany operation with empty string passed 
     in place of key array .
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyKeyEmptyStringNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyKeyEmptyStringNegative() {
        try {
            $status = $this->db->getMany("", $records, NULL, array(Aerospike::OPT_READ_TIMEOUT=>3000));
            if ($status !== Aerospike::OK) {
                var_dump("foo",$this->db->error());
                return $this->db->errorno();
            }
            if (empty($records)) {
                return Aerospike::OK;
            }
            return $status;
        } catch (ErrorException $e) {
            return Aerospike::ERR_PARAM;
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany operation with no arguments.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyNoArgumentsNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyNoArgumentsNegative() {
        try {
            $status = $this->db->getMany();
            if ($status !== AEROSPIKE_OK) {
                return $this->db->errorno();
            }
            if (empty($records)) {
                return Aerospike::ERR_CLIENT;
            }
            return $this->db->status;
        } catch (ErrorException $e) {
            return Aerospike::OK;
        }
    }

    /**
     * @test
     * Basic getMany, record is empty string variable.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyRecordEmptyStringNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyRecordEmptyStringNegative() {
        $records = "";
        $status = $this->db->getMany($this->keys, $records);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }
        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($this->keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($this->keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($this->keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($this->put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany, Record is of type string.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyMetadataStringVariableNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyRecordStringVariableNegative() {
        $records = "abc";
        $status = $this->db->getMany($this->keys, $records);
        if ($status !== Aerospike::OK) {
            return $this->db->errorno();
        }
        if (empty($records)) {
            return Aerospike::ERR_CLIENT;
        }
        $i = 0;
        foreach ($records as $records_key=>$value) {
            if ((!strcmp($this->keys[$i]["key"], $value["key"]["key"])) &&
                (!strcmp($this->keys[$i]["ns"], $value["key"]["ns"])) &&
                (!strcmp($this->keys[$i]["set"], $value["key"]["set"]))) {
                    $result = array_diff_assoc_recursive($this->put_records[$i],
                        $value["bins"]);
                    if (!empty($result)) {
                        return Aerospike::ERR_CLIENT;
                    }
                    $i++;
                } else {
                    return Aerospike::ERR_CLIENT;
                }
        }
        return $status;
    }

    /**
     * @test
     * Basic getMany, Options is of type integer.
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * newly initialized Aerospike objects
     *
     * @remark
     * Variants: OO (testGetManyOptionsIntegerNegative)
     *
     * @test_plans{1.1}
     */
    function testGetManyOptionsIntegerNegative() {
        try {
            $status = $this->db->getMany($this->keys, $records, NULL, 12);
            if ($status !== Aerospike::OK) {
                return $this->db->errorno();
            }
            if (empty($records)) {
                return Aerospike::ERR_CLIENT;
            }
            return $status;
        } catch (ErrorException $e) {
        	return $this->db->errorno();
        }
    }
}
