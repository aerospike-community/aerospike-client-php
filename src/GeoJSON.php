<?php
/**
 * Copyright 2014-2017 Aerospike, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @category   Database
 * @package    Aerospike
 * @subpackage GeoJSON
 * @author     Ronen Botzer <rbotzer@aerospike.com>
 * @copyright  Copyright 2014-2017 Aerospike, Inc.
 * @license    http://www.apache.org/licenses/LICENSE-2.0 Apache License, Version 2
 * @filesource
 */

namespace Aerospike;

class GeoJSON implements \Aerospike\GeoJSON\Serializable
{
    protected $type;
    protected $coordinates;

    public function __construct($geo) {
        $this->type = $geo->type;
        $this->coordinates = $geo->coordinates;
    }

    public function getType() {
        return $this->type;
    }

    public function __toString() {
        $arr = array('type'=> $this->type, 'coordinates' => $this->coordinates);
        return json_encode($arr);
    }

    public function jsonSerialize() {
        $geo_obj = new \stdClass();
        $geo_obj->type = $this->type;
        $geo_obj->coordinates = $this->coordinates;
        return $geo_obj;
    }

    public function toObject() {
        $geo_obj = new \stdClass();
        $geo_obj->type = $this->type;
        $geo_obj->coordinates = $this->coordinates;
        return $geo_obj;
    }

    public function toArray() {
        return array('type'=> $this->type, 'coordinates' => $this->coordinates);
    }

    public static function fromArray($geo_array) {
        $geo_obj = new \stdClass();
        $geo_obj->type = $geo_array["type"];
        $geo_obj->coordinates = $geo_array["coordinates"];
        return new \Aerospike\GeoJSON($geo_obj);
    }

    public static function fromJson($geo_json) {
        $geo = json_decode($geo_json);
        $geo_obj = new \stdClass();
        $geo_obj->type = $geo->type;;
        $geo_obj->coordinates = $geo->coordinates;
        return new \Aerospike\GeoJSON($geo_obj);
    }
}

