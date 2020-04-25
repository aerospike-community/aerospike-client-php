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
 * @author     Ronen Botzer <rbotzer@aerospike.com>
 * @copyright  Copyright 2014-2017 Aerospike, Inc.
 * @license    http://www.apache.org/licenses/LICENSE-2.0 Apache License, Version 2
 * @filesource
 */
namespace Aerospike\GeoJSON;
use Aerospike;

/**
 * GeoJSON allows individual record bins to have geographical locations along 
 * with the record.
 *
 * @package    Aerospike
 * @subpackage GeoJSON
 * @link       http://www.aerospike.com/docs/guide/geojson.html
 * @author     Ronen Botzer <rbotzer@aerospike.com>
 */
interface Serializable extends \JsonSerializable
{
    public function __toString();

    public function getType();

    public function jsonSerialize();
}

