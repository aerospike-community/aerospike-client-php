<?php

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class ErrorInDoubtTest extends TestCase {
    protected $db;

    protected function setUp(): void
    {
        $config = get_as_config();
        $this->db = new Aerospike($config);
    }

    function testReturnIsABool() {
        $key = $this->db->initKey("test", "notarealset", "not_A_real_KEY");
        $this->db->get($key, $returnRec);
        $inDoubt = $this->db->errorInDoubt();

        $this->assertTrue(($inDoubt === false) || ($inDoubt === true));
    }

    function testNewClientHasInDoubtSetToFalse() {
        $config = get_as_config();
        $client = new Aerospike($config, false);
        $this->assertFalse($client->errorInDoubt());
    }
}

?>
