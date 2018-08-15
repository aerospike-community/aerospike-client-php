<?php
class GetNodes extends AerospikeTestCommon
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
     * Basic GetNodes positive
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Get information of nodes
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testGetNodesPositive()
    {
        try {
            $info = $this->db->getNodes();
            if (is_array($info) && count($info) > 0) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
    /**
     * @test
     * Basic GetNodes with parameters
     *
     * @pre
     * Connect using aerospike object to the specified node
     *
     * @post
     * Test should fail
     *
     * @remark
     *
     *
     * @test_plans{1.1}
     */
    function testGetNodesWithParameter()
    {
        try {
            if ($this->db->getNodes($request)) {
                return Aerospike::OK;
            } else {
                return Aerospike::ERR_CLIENT;
            }
        } catch (ErrorException $e) {
            return $this->db->errorno();
        }
    }
}
?>
