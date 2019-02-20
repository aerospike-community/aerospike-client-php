<?php 

require_once 'Util.inc';
use PHPUnit\Framework\TestCase;

final class OperateTTLTest extends TestCase {
    protected $db;
    protected $key;

    protected function setUp() : void
    {
        $config = get_as_config();
        $this->db = new Aerospike($config);
        $this->key = $this->db->initKey("test", "demo", 1);
        $this->db->put($this->key, ["Aero" => "spike", "INCR"=>5, "LST"=>[1,2,3]], 15000);
    }

    protected function tearDown() : void 
    {

        $this->db->remove($this->key);

    }

    public function testPutTTL() {
        $ttlVal = 100;
        
        $success = $this->db->put($this->key, ["Aero" => "spike", "INCR"=>5, "LST"=>[1,2,3]], $ttlVal);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }

    public function testAppendTTL() {
        $ttlVal = 200;
        
        $success = $this->db->append($this->key, "Aero" ,"spike", [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }

    public function testPrependTTL() {
        $ttlVal = 300;
        
        $success = $this->db->prepend($this->key, "Aero" ,"spike", [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }


    public function testIncrementTTL() {
        $ttlVal = 400;
        
        $success = $this->db->increment($this->key, "INCR" , 5, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }

    public function testListHelperTTL() {
        $ttlVal = 500;
        
        $success = $this->db->listInsert($this->key, "LST" , 0, 5, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }


    public function testOperateTTL() {
        $ttlVal = 600;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operate($this->key, $ops, $response, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }



    public function testOperateOrderedTTL() {
        $ttlVal = 600;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operateOrdered($this->key, $ops, $response, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, 0);
        
        $success = $this->db->get($this->key, $response);
        $this->assertEquals($success, 0);
        
        $meta = $response["metadata"];
        $ttl = $meta["ttl"];
        $this->assertTrue($ttl <= $ttlVal && $ttl > ($ttlVal - 100));
    }

/*
Test without specifying ttl */


    public function testAppendTTLNoTTL() {
        $ttlVal = "five";
        $success = $this->db->append($this->key, "Aero" ,"spike");
        $this->assertEquals($success, AEROSPIKE::OK);

    }

    public function testPrependTTLNoTTL() {
        $ttlVal = "five";

        $success = $this->db->prepend($this->key, "Aero" ,"spike");
        $this->assertEquals($success, AEROSPIKE::OK);

    }


    public function testIncrementTTLNoTTL() {
        $ttlVal = "five";

        $success = $this->db->increment($this->key, "INCR" , 5);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    public function testListHelperTTLNoTTL() {
        $ttlVal = "five";

        $success = $this->db->listInsert($this->key, "LST" , 0, 5);
        $this->assertEquals($success, AEROSPIKE::OK);
    }


    public function testOperateTTLNoTTL() {
        $ttlVal = "five";
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operate($this->key, $ops, $response);
        $this->assertEquals($success, AEROSPIKE::OK);
    }



    public function testOperateOrderedTTLNoTTL() {
        $ttlVal = "five";
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operateOrdered($this->key, $ops, $response);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    /* NULL TTL TEST */

    public function testAppendNullTTL() {
        $ttlVal = NULL;
        $success = $this->db->append($this->key, "Aero" ,"spike", NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    public function testPrependNullTTL() {
        $ttlVal = NULL;

        $success = $this->db->prepend($this->key, "Aero" ,"spike", NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }


    public function testIncrementNullTTL() {
        $ttlVal = NULL;

        $success = $this->db->increment($this->key, "INCR" , 5, NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    public function testListHelperNullTTL() {
        $ttlVal = NULL;

        $success = $this->db->listInsert($this->key, "LST" , 0, 5, NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }


    public function testOperateNullOpts() {
        $ttlVal = NULL;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operate($this->key, $ops, $response, NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }


    public function testOperateOrderedNullTTL() {
        $ttlVal = NULL;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operateOrdered($this->key, $ops, $response, NULL);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    /* OPTS WITHOUT TTL */

    public function testAppendOptsNoTTL() {
        $ttlVal = NULL;
        $success = $this->db->append($this->key, "Aero" ,"spike", []);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    public function testPrependOptsNoTTL() {
        $ttlVal = NULL;

        $success = $this->db->prepend($this->key, "Aero" ,"spike", []);
        $this->assertEquals($success, AEROSPIKE::OK);
    }


    public function testIncrementOptsNoTTL() {
        $ttlVal = NULL;

        $success = $this->db->increment($this->key, "INCR" , 5, []);
        $this->assertEquals($success, AEROSPIKE::OK);
    }

    public function testListHelperOptsNoTTL() {
        $ttlVal = NULL;

        $success = $this->db->listInsert($this->key, "LST" , 0, 5, []);
        $this->assertEquals($success, AEROSPIKE::OK);

    }


    public function testOperateOptsNoTTL() {
        $ttlVal = NULL;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operate($this->key, $ops, $response, []);
        $this->assertEquals($success, AEROSPIKE::OK);

    }


    public function testOperateOrderedOptsNoTTL() {
        $ttlVal = NULL;
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operateOrdered($this->key, $ops, $response, []);
        $this->assertEquals($success, AEROSPIKE::OK);

    }
    /**/
    /* INVALID TYPE TESTS */
    /**/

    public function testPutTTLInvalid() {
        $ttlVal = "five";

        $success = $this->db->put($this->key, ["Aero" => "spike", "INCR"=>5, "LST"=>[1,2,3]], $ttlVal);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);
    }

    public function testAppendTTLInvalid() {
        $ttlVal = "five";
        $success = $this->db->append($this->key, "Aero" ,"spike", [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);

    }

    public function testPrependTTLInvalid() {
        $ttlVal = "five";

        $success = $this->db->prepend($this->key, "Aero" ,"spike", [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);
    }


    public function testIncrementTTLInvalid() {
        $ttlVal = "five";

        $success = $this->db->increment($this->key, "INCR" , 5, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);
    }

    public function testListHelperTTLInvalid() {
        $ttlVal = "five";

        $success = $this->db->listInsert($this->key, "LST" , 0, 5, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);
    }

    public function testOperateTTLInvalid() {
        $ttlVal = "five";
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operate($this->key, $ops, $response, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);
    }

    public function testOperateOrderedTTLInvalid() {
        $ttlVal = "five";
        $ops = [["op"=>Aerospike::OPERATOR_WRITE, "bin"=>"last_name", "val"=>"Fry"]];

        $success = $this->db->operateOrdered($this->key, $ops, $response, [AEROSPIKE::OPT_TTL => $ttlVal]);
        $this->assertEquals($success, AEROSPIKE::ERR_PARAM);

    }
}

?>