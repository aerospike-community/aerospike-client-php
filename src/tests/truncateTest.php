<?php 

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class TruncateTest extends TestCase {
    protected $db;
    protected $keys1;
    protected $keys2;

    protected function setUp() : void 
    {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $this->keys = [];
        for($i = 0; $i < 20; $i++) {
            $key = $this->db->initKey("test", "truncate1", $i);
            $this->db->put($key, ["val" => $i]);
            $this->keys1[]=$key;
        }
        for($i = 20; $i < 40; $i++) {
            $key = $this->db->initKey("test", "truncate2", $i);
            $this->db->put($key, ["val" => $i]);
            $this->keys2[]=$key;
        }
        sleep(2);
        // Give some time for any clock differences to resolve.
    }

    private function has_been_truncated($keys) {
        /*
            Test that all keys have been removed, since the call may take some time,
            give it multiple attempts to complete;
        */
        $truncated = true;
        $attempts = 25;
        while ($attempts > 0) {
            $truncated = true;
            foreach($keys as $key) {
                $this->db->exists($key, $meta);
                if ($meta) {
                    $truncated = false;
                    break;
                }
            }
            if ($truncated) {
                return $truncated;
            }
            sleep(1);
            $attempts--;
        }
        return false;
    }

    private function has_not_been_truncated($keys) {
        /*
            Make sure that truncation has not happened
        */
        foreach($keys as $key) {
            $this->db->exists($key, $meta);
            if (!$meta) {
                return false;
            }
        }
        return true;
    }

    protected function tearDown() : void
    {
        foreach($this->keys1 as $key) {
            $this->db->remove($key);
        }

        foreach($this->keys2 as $key) {
            $this->db->remove($key);
        }
    }
    /*
    @dataProvider
    */
    public function invalidParamValues() {
        return [
            [["test", "test1"], null, 0],
            ["test", ["truncate1", "truncate2"], 0],
            ["test", null, "five"]
        ];
    }
    public function testTruncateAll()
    {

        $this->db->truncate("test", null, 0);

        $this->assertTrue($this->has_been_truncated($this->keys1));
        $this->assertTrue($this->has_been_truncated($this->keys2));
    }

    public function testTruncateAllWithPolicy()
    {
        $policy = [Aerospike::OPT_WRITE_TIMEOUT => 1500];
        $this->db->truncate("test", null, 0, $policy);

        $this->assertTrue($this->has_been_truncated($this->keys1));
        $this->assertTrue($this->has_been_truncated($this->keys2));
    }

    public function testTruncateSingleSet() {
        $this->db->truncate("test", "truncate2", 0);
        
        $this->assertTrue($this->has_not_been_truncated($this->keys1));
        $this->assertTrue($this->has_been_truncated($this->keys2));
    }

    public function testTruncateWithThresholdTime() {
        $keys3 = [];

        // Epoch time in nanoseconds
        $time = time() * 1000000000;
        sleep(10);
        for($i = 40; $i < 60; $i++) {
            $key = $this->db->initKey("test", "truncate3", $i);
            $this->db->put($key, ["val" => $i]);
            $keys3[]=$key;
        }
        sleep(4);
        $this->db->truncate("test", null, $time);
        
        $this->assertTrue($this->has_been_truncated($this->keys1));
        $this->assertTrue($this->has_been_truncated($this->keys2));

        /* Verify that records created after the update threshold will
        * not have been removed 
        */

        $three_not_trunc =  $this->has_not_been_truncated($keys3);
        
        /* Drop the keys here */
        foreach($keys3 as $key) {
            $this->db->remove($key);
        }
        $this->assertTrue($three_not_trunc);
    }

    /**
     *  @param $ns
     *  @param $set
     *  @param $nanos
     *
     *  @dataProvider invalidParamValues
     */
    public function testTruncateWithNoNamespace($ns, $set, $nanos) {
        $this->expectException(PHPUnit\Framework\Error\Warning::class);
        try {
            $code = $this->db->truncate($ns, $set, $nanos);
        }catch(ErrorException $e) {
            ;
        }
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    }

    public function testTruncateWithNegativeNanos() {
        
        $code = $this->db->truncate("test", null, -1);

        $this->assertEquals($code, Aerospike::ERR_PARAM);
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    }

    public function testTruncateWithNanosInTheFuture() {
        // Nanoseconds after epoch from a week in the future
        $time = (time() + 86400 * 7) * 1000000000;
        $code = $this->db->truncate("test", null, $time);

        $this->assertNotEquals($code, Aerospike::OK);
        $this->assertNotEquals($this->db->errorno(), Aerospike::OK);
    }

    public function testTruncateWithTooFewArguments() {
        $this->expectException(PHPUnit\Framework\Error\Warning::class);
        try {
            $code = $this->db->truncate("test", null);
        } catch(ErrorException $e) {
            ;
        }
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    
    }

    public function testTruncateWithNSTooLong() {
        
        $code = $this->db->truncate(str_repeat("a", 100), null, 0);

        $this->assertEquals($code, Aerospike::ERR_PARAM);
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    }

    public function testTruncateWithSetTooLong() {
        
        $code = $this->db->truncate("test", str_repeat("a", 100), 0);

        $this->assertEquals($code, Aerospike::ERR_PARAM);
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    }

    public function testTruncateWithInvalidPolicyType()
    {
        $policy = 1500;
        $code = $this->db->truncate("test", null, 0, $policy);
        
        $this->assertEquals($code, Aerospike::ERR_PARAM);
        $this->assertEquals($this->db->errorno(), Aerospike::ERR_PARAM);
    }
}

?>