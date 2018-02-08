<?php
class InfoMany extends AerospikeTestCommon
{
    private $config;

    protected function setUp() {
        $this->config = get_as_config();
        $this->db = new Aerospike($this->config);
        if (!$this->db->isConnected()) {
            return $this->db->errorno();
        }
    }

    /**
     * @test
     * Basic INFOMANY without parameter
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Error
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNoParameter()
    {
        try {
            return($this->db->infoMany());
        } catch(ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany positive for bins
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyPositiveForBins()
    {
        try {
            $response = $this->db->infoMany('bins/test');
            if ($response == NULL)
                return $this->db->errorno();
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany positive for bins with config array
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyPositiveForBinsWithConfig()
    {
        try {
            $response = $this->db->infoMany('bins/test', $this->config);
            if ($response == NULL)
                return $this->db->errorno();
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany positive for bins with multiple hosts
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyPositiveForBinsWithMultipleHosts()
    {
        $host = array("addr"=>AEROSPIKE_CONFIG_NAME, "port"=>AEROSPIKE_CONFIG_PORT);
        $hostnew = array("addr"=>AEROSPIKE_CONFIG_NAME2, "port"=>AEROSPIKE_CONFIG_PORT2);
        $config = array("hosts"=>array($host, $hostnew));
        try {
            $response = $this->db->infoMany('bins/test', $config);
            if ($response == NULL)
                return $this->db->errorno();
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany positive for bins with config array and policy argument
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyPositiveForBinsWithPolicy()
    {
        try {
            $response = $this->db->infoMany('bins/test', NULL, array(Aerospike::OPT_READ_TIMEOUT => 20000));
            if ($response == NULL)
                return $this->db->errorno();
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany negative incorrect command
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNegativeIncorrectCommand()
    {
        try {
            $response = $this->db->infoMany('sadfsdfasdfasfdbcd', NULL, array(Aerospike::OPT_READ_TIMEOUT => 20000));
            if ($response == NULL) {
                return $this->db->errorno();
            } else {
                return (Aerospike::OK);
            }
        } catch (Exception $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany negative empty command
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark Adjust after client bug is fixed
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNegativeEmptyCommand()
    {
        $response = $this->db->infoMany('', NULL, array(Aerospike::OPT_READ_TIMEOUT => 20000));
        if ($response == NULL) {
            return $this->db->errorno();
        } else {
            return Aerospike::OK;
        }
    }
    /**
     * @test
     * Basic InfoMany negative incorrect config structure
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNegativeIncorrectConfigStructure()
    {
        try {
            $response = $this->db->infoMany('bins/test',
                array("addr"=>"localhost", "port"=>AEROSPIKE_CONFIG_PORT),
                array(Aerospike::OPT_READ_TIMEOUT => 20000));
            if ($response == NULL)
                return $this->db->errorno() ;
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany negative incorrect hostname
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNegativeIncorrectHostname()
    {
        $host = array("addr"=>"localhost2", "port"=>AEROSPIKE_CONFIG_PORT);
        $myconfig = array("hosts"=>array($host));
        try {
            $response = $this->db->infoMany('bins/test', $myconfig,
                array(Aerospike::OPT_READ_TIMEOUT => 20000));
            if ($response == NULL) {
                return $this->db->errorno();
            }
            else {
                return (Aerospike::OK);
            }
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany negative incorrect port
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyNegativeIncorrectPort()
    {
        $host = array("addr"=>"localhost", "port"=>3002);
        $config = array("hosts"=>array($host));
        try {
            $response = $this->db->infoMany('bins/test', $config,
                array(Aerospike::OPT_READ_TIMEOUT => 20000));
            if ($response == NULL) {
                return $this->db->errorno();
            }
            else {
                return (Aerospike::OK);
            }
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic InfoMany positive for build
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Info for bins of namespace
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testInfoManyPositiveForBuild()
    {
        try {
            $response = $this->db->infoMany('build');
            if ($response == NULL)
                return $this->db->errorno();
            else
                return (Aerospike::OK);
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
}
?>
