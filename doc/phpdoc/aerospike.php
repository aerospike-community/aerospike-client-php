<?php
/**
 * Copyright 2013-2017 Aerospike, Inc.
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
 * @author     Robert Marks <robert@aerospike.com>
 * @copyright  Copyright 2013-2017 Aerospike, Inc.
 * @license    http://www.apache.org/licenses/LICENSE-2.0 Apache License, Version 2
 * @link       http://www.aerospike.com/docs/client/php/
 * @filesource
 */

/**
 * The Aerospike client class.
 * @author Robert Marks <robert@aerospike.com>
 */
class Aerospike {

    // Lifecycle and Connection Methods

    /**
     * Construct an Aerospike client object, and connect to the cluster defined
     * in $config.
     *
     * Aerospike::isConnected() can be used to test whether the connection has
     * succeeded. If a config or connection error has occured, Aerospike::error()
     * and Aerospike::errorno() can be used to inspect it.
     *
     * ```php
     * $config = [
     *   "hosts" => [
     *     ["addr" => "localhost", "port" => 3000]
     *   ],
     *   "shm" => []
     * ];
     * $opts = [Aerospike::OPT_POLICY_KEY => Aerospike::POLICY_KEY_SEND];
     * $client = new Aerospike($config, true, $opts);
     * if (!$client->isConnected()) {
     *   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *   exit(1);
     * }
     * ```
     *
     * @link https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#configuration-in-a-web-server-context Configuration in a Web Server Context
     * @param array $config holds cluster connection and client config information
     * * _hosts_ a **required** array of host pairs. One node or more (for failover)
     *        may be defined. Once a connection is established to the
     *        "seed" node, the client will retrieve the full list of nodes in
     *        the cluster, and manage its connections to them.
     * * _addr_ hostname or IP of the node
     * * _port_ the port of the node
     * * _user_ **required** for the Enterprise Edition
     * * _pass_ **required** for the Enterprise Edition
     * * _shm_ optional. Shared-memory cluster tending is enabled if an array
     *     (even an empty one) is provided. Disabled by default.
     * * _shm\_key_ explicitly sets the shm key for the cluster. It is
     *       otherwise implicitly evaluated per unique hostname, and can be
     *       inspected with shmKey(). (default: 0xA5000000)
     * * _shm\_max\_nodes_ maximum number of nodes allowed. Pad so new nodes
     *       can be added without configuration changes (default: 16)
     * * _shm\_max\_namespaces_ maximum number of namespaces allowed (default: 8)
     * * _shm\_takeover\_threshold\_sec_ take over tending if the cluster
     *       hasn't been checked for this many seconds (default: 30)
     * * _max\_threads_ (default: 300)
     * * _thread\_pool\_size_ should be at least the number of nodes in the cluster (default: 16) In ZTS builds this is set to 0
     * * _compression\_threshold_ client will compress records larger than this value for transport (default: 0)
     * @param bool $persistent_connection In a multiprocess context, such as a
     *        web server, the client should be configured to use
     *        persistent connections. This allows for reduced overhead,
     *        saving on discovery of the cluster topology, fetching its partition
     *        map, and on opening connections to the nodes.
     * @param array $options An optional client config array whose keys include
     * * Aerospike::OPT_CONNECT_TIMEOUT
     * * Aerospike::OPT_READ_TIMEOUT
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_EXISTS
     * * Aerospike::OPT_SERIALIZER
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_REPLICA
     * * Aerospike::OPT_POLICY_CONSISTENCY
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_CONNECT_TIMEOUT Aerospike::OPT_CONNECT_TIMEOUT options
     * @see Aerospike::OPT_READ_TIMEOUT Aerospike::OPT_READ_TIMEOUT options
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_EXISTS Aerospike::OPT_POLICY_EXISTS options
     * @see Aerospike::OPT_SERIALIZER Aerospike::OPT_SERIALIZER options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_REPLICA Aerospike::OPT_POLICY_REPLICA options
     * @see Aerospike::OPT_POLICY_CONSISTENCY Aerospike::OPT_POLICY_CONSISTENCY options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::isConnected() isConnected()
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return void
     */
    public function __construct($config, $persistent_connection = true, array $options = []) {}

    /**
     * Disconnect from the Aerospike cluster and clean up resources.
     *
     * No need to ever call this method explicilty.
     * @return void
     */
    public function __destruct() {}

    /**
     * Test whether the client is connected to the cluster.
     *
     * If a connection error has occured, Aerospike::error() and Aerospike::errorno()
     * can be used to inspect it.
     * ```php
     * if (!$client->isConnected()) {
     *   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *   exit(1);
     * }
     * ```
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return bool
     */
    public function isConnected() {}

    /**
     * Disconnect the client from all the cluster nodes.
     *
     * This method should be explicitly called when using non-persistent connections.
     * @return void
     */
    public function close() {}

    /**
     * Reconnect the client to the cluster nodes.
     *
     * Aerospike::isConnected() can be used to test whether the re-connection
     * succeded. If a connection error occured Aerospike::error() and
     * Aerospike::errorno() can be used to inspect it.
     * ```php
     * $client = new Aerospike($config, false);
     * $client->close();
     * $client->reconnect();
     * if (!$client->isConnected()) {
     *   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *   exit(1);
     * }
     * ```
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return void
     */
    public function reconnect() {}

    /**
     * Return the error message associated with the last operation.
     *
     * If the operation was successful the return value should be an empty string.
     * ```php
     * $client = new Aerospike($config, false);
     * if (!$client->isConnected()) {
     *   echo "{$client->error()} [{$client->errorno()}]";
     *   exit(1);
     * }
     * ```
     * On connection error would show:
     * ```
     * Unable to connect to server [-1]
     * ```
     * @link http://www.aerospike.com/docs/dev_reference/error_codes.html Error Codes
     * @return string
     */
    public function error() {}

    /**
     * Return the error code associated with the last operation.
     * If the operation was successful the return value should be 0 (Aerospike::OK)
     * @link http://www.aerospike.com/docs/dev_reference/error_codes.html Error Codes
     * @see Aerospike::OK Aerospike::OK
     * @return int
     */
    public function errorno() {}

    // Key-Value Methods.

    /**
     * Return an array that represents the record's key.
     *
     * This value can be passed as the $key arguement required by other
     * key-value methods.
     *
     * In Aerospike, a record is identified by the tuple (namespace, set,
     * primary key), or by the digest which results from hashing this tuple
     * through RIPEMD-160.
     *
     * ** Initializing a key **
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * var_dump($key);
     * ```
     *
     * ```bash
     *array(3) {
     *  ["ns"]=>
     *  string(4) "test"
     *  ["set"]=>
     *  string(5) "users"
     *  ["key"]=>
     *  int(1234)
     *}
     * ```
     *
     * ** Setting a digest **
     *
     * ```php
     * $base64_encoded_digest = '7EV9CpdMSNVoWn76A9E33Iu95+M=';
     * $digest = base64_decode($base64_encoded_digest);
     * $key = $client->initKey("test", "users", $digest, true);
     * var_dump($key);
     * ```
     *
     * ```bash
     *array(3) {
     *  ["ns"]=>
     *  string(4) "test"
     *  ["set"]=>
     *  string(5) "users"
     *  ["digest"]=>
     *  string(20) "?E}
     *?LH?hZ~??7Ü‹???"
     *}
     * ```
     *
     * @link https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#configuration-in-a-web-server-context Configuration in a Web Server Context
     * @param string $ns The namespace
     * @param string $set The set within the given *$namespace*
     * @param int|string $pk The primary key in the application, or the RIPEMD-160 digest of the (namespce, set, primary-key) tuple
     * @param bool $is_digest True if the *$pk* argument is a digest
     * @return array
     * @see Aerospike::getKeyDigest() getKeyDigest()
     */
    public function initKey ($ns, $set, $pk, $is_digest = false) {}

    /**
     * Return the digest of hashing the (namespace, set, primary-key) tuple
     * with RIPEMD-160.
     *
     * The digest uniquely identifies the record in the cluster, and is used to
     * calculate a partition ID. Using the partition ID, the client can identify
     * the node holding the record's master partition or replica partition(s) by
     * looking it up against the cluster's partition map.
     *
     * ```php
     * $digest = $client->getKeyDigest("test", "users", 1);
     * $key = $client->initKey("test", "users", $digest, true);
     * var_dump($digest, $key);
     * ```
     *
     * ```bash
     * string(20) "9!?@%??;???Wp?'??Ag"
     * array(3) {
     *   ["ns"]=>
     *   string(4) "test"
     *   ["set"]=>
     *   string(5) "users"
     *   ["digest"]=>
     *   string(20) "9!?@%??;???Wp?'??Ag"
     * }
     * ```
     *
     * @link https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#configuration-in-a-web-server-context Configuration in a Web Server Context
     * @param string $ns The namespace
     * @param string $set The set within the given *$namespace*
     * @param int|string $pk The primary key in the application
     * @return string
     * @see Aerospike::initKey() initKey()
     */
    public function getKeyDigest ($ns, $set, $pk ) {}

    /**
     * Write a record identified by the $key with $bins, an array of bin-name => bin-value pairs.
     *
     * By default Aerospike::put() behaves in a set-and-replace mode, similar to
     * how new keys are added to an array, or the value of existing ones is overwritten.
     * This behavior can be modified using the *$options* parameter.
     *
     * **Note:** a binary-string which includes a null-byte will get truncated
     * at the position of the **\0** character if it is not wrapped. For more
     * information and the workaround see 'Handling Unsupported Types'.
     *
     * **Example #1 Aerospike::put() default behavior example**
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $bins = ["email" => "hey@example.com", "name" => "Hey There"];
     * // will ensure a record exists at the given key with the specified bins
     * $status = $client->put($key, $bins);
     * if ($status == Aerospike::OK) {
     *     echo "Record written.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * 
     * // Updating the record
     * $bins = ["name" => "You There", "age" => 33];
     * // will update the name bin, and create a new 'age' bin
     * $status = $client->put($key, $bins);
     * if ($status == Aerospike::OK) {
     *     echo "Record updated.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * Record written.
     * Record updated.
     * ```
     *
     * **Example #2 Fail unless the put explicitly creates a new record**
     * ```php
     *
     * // This time we expect an error, due to the record already existing (assuming we
     * // ran Example #1)
     * $status = $client->put($key, $bins, 0, [Aerospike::OPT_POLICY_EXISTS => Aerospike::POLICY_EXISTS_CREATE]);
     * 
     * if ($status == Aerospike::OK) {
     *     echo "Record written.\n";
     * } elseif ($status == Aerospike::ERR_RECORD_EXISTS) {
     *     echo "The Aerospike server already has a record with the given key.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * The Aerospike cluster already has a record with the given key.
     * ```
     *
     * **Example #3 Fail if the record has been written since it was last read
     * (CAS)**
     * ```php
     * // Get the record metadata and note its generation
     * $client->exists($key, $metadata);
     * $gen = $metadata['generation'];
     * $gen_policy = [Aerospike::POLICY_GEN_EQ, $gen];
     * $res = $client->put($key, $bins, 0, [Aerospike::OPT_POLICY_GEN => $gen_policy]);
     * 
     * if ($res == Aerospike::OK) {
     *     echo "Record written.\n";
     * } elseif ($res == Aerospike::ERR_RECORD_GENERATION) {
     *     echo "The record has been written since we last read it.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ?>
     * ```
     * ```
     * The record has been written since we last read it.
     * ```
     *
     * **Example #4 Handling binary strings**
     * ```php
     * $str = 'Glagnar\'s Human Rinds, "It\'s a bunch\'a munch\'a crunch\'a human!';
     * $deflated = new \Aerospike\Bytes(gzdeflate($str));
     * $wrapped = new \Aerospike\Bytes("trunc\0ated");
     *
     * $key = $client->initKey('test', 'demo', 'wrapped-bytes');
     * $status = $client->put($key, ['unwrapped'=>"trunc\0ated", 'wrapped'=> $wrapped, 'deflated' => $deflated]);
     * if ($status !== Aerospike::OK) {
     *     die($client->error());
     * }
     * $client->get($key, $record);
     * $wrapped = \Aerospike\Bytes::unwrap($record['bins']['wrapped']);
     * $deflated = $record['bins']['deflated'];
     * $inflated = gzinflate($deflated->s);
     * echo "$inflated\n";
     * echo "wrapped binary-string: ";
     * var_dump($wrapped);
     * $unwrapped = $record['bins']['unwrapped'];
     * echo "The binary-string that was given to put() without a wrapper: $unwrapped\n";
     * ```
     * ```
     * Glagnar's Human Rinds, "It's a bunch'a munch'a crunch'a human!
     * wrapped binary-string: string(10) "truncated"
     * The binary-string that was given to put() without a wrapper: trunc
     * ```
     * @link http://www.aerospike.com/docs/architecture/data-model.html Aerospike Data Model
     * @link http://www.aerospike.com/docs/guide/kvs.html Key-Value Store
     * @link https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#handling-unsupported-types Handling Unsupported Types
     * @link http://www.aerospike.com/docs/client/c/usage/kvs/write.html#change-record-time-to-live-ttl Time-to-live
     * @link http://www.aerospike.com/docs/guide/glossary.html Glossary
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $bins The array of bin names and values to write. **Bin names cannot be longer than 14 characters.** Binary data containing the null byte (**\0**) may get truncated. See 'Handling Unsupported Types' for more details and a workaround
     * @param int $ttl The record's time-to-live in seconds
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_SERIALIZER
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_EXISTS
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_SERIALIZER Aerospike::OPT_SERIALIZER options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_EXISTS Aerospike::OPT_POLICY_EXISTS options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function put ( array $key, array $bins, $ttl = 0, array $options = []) {}

    /**
     * Read a record with a given key, and store it in $record
     *
     * The bins returned in *$record* can be filtered by passing a *$select*
     * array of bin names. Non-existent bins will appear in the *$record* with a `NULL` value.
     *
     * **Example #1 Aerospike::get() default behavior example**
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $status = $client->get($key, $record);
     * if ($status == Aerospike::OK) {
     *     var_dump($record);
     * } elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
     *     echo "A user with key ". $key['key']. " does not exist in the database\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(3) {
     *   ["key"]=>
     *   array(4) {
     *     ["digest"]=>
     *     string(40) "436a3b9fcafb96d12844ab1377c0ff0d7a0b70cc"
     *     ["namespace"]=>
     *     NULL
     *     ["set"]=>
     *     NULL
     *     ["key"]=>
     *     NULL
     *   }
     *   ["metadata"]=>
     *   array(2) {
     *     ["generation"]=>
     *     int(3)
     *     ["ttl"]=>
     *     int(12345)
     *   }
     *   ["bins"]=>
     *   array(3) {
     *     ["email"]=>
     *     string(9) "hey@example.com"
     *     ["name"]=>
     *     string(9) "You There"
     *     ["age"]=>
     *     int(33)
     *   }
     * }
     * ```
     * **Example #2 get the record with filtered bins**
     * ```php
     * // assuming this follows Example #1, getting a filtered record
     * $filter = ["email", "manager"];
     * unset($record);
     * $status = $client->get($key, $record, $filter);
     * if ($status == Aerospike::OK) {
     *     var_dump($record);
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(3) {
     *   ["key"]=>
     *   array(4) {
     *     ["digest"]=>
     *     string(40) "436a3b9fcafb96d12844ab1377c0ff0d7a0b70cc"
     *     ["namespace"]=>
     *     NULL
     *     ["set"]=>
     *     NULL
     *     ["key"]=>
     *     NULL
     *   }
     *   ["metadata"]=>
     *   array(2) {
     *     ["generation"]=>
     *     int(3)
     *     ["ttl"]=>
     *     int(12344)
     *   }
     *   ["bins"]=>
     *   array(2) {
     *     ["email"]=>
     *     string(15) "hey@example.com"
     *     ["manager"]=>
     *     NULL
     *   }
     * }
     * ```
     * @link http://www.aerospike.com/docs/architecture/data-model.html Aerospike Data Model
     * @link http://www.aerospike.com/docs/guide/kvs.html Key-Value Store
     * @link http://www.aerospike.com/docs/guide/glossary.html Glossary
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $record an array of `['key', metadata', 'bins]` with the structure:
     * ```
     * Array:
     *   key => Array
     *     ns => namespace
     *     set => set name
     *     key => primary-key, present if written with POLICY_KEY_SEND
     *     digest => the record's RIPEMD-160 digest, always present
     *   metadata => Array
     *     ttl => time in seconds until the record expires
     *     generation => the number of times the record has been written
     *   bins => Array of bin-name => bin-value pairs
     * ```
     * @param array $select only these bins out of the record (optional)
     * @param array $options An optional array of read policy options, whose keys include
     * * Aerospike::OPT_READ_TIMEOUT
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_REPLICA
     * * Aerospike::OPT_POLICY_CONSISTENCY
     * @see Aerospike::OPT_READ_TIMEOUT Aerospike::OPT_READ_TIMEOUT options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_REPLICA Aerospike::OPT_POLICY_REPLICA options
     * @see Aerospike::OPT_POLICY_CONSISTENCY Aerospike::OPT_POLICY_CONSISTENCY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function get ( array $key, array &$record, array $select = [], array $options = []) {}

    /**
     * Get the metadata of a record with a given key, and store it in $metadata
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $status = $client->exists($key, $metadata);
     * if ($status == Aerospike::OK) {
     *     var_dump($metadata);
     * } elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
     *     echo "A user with key ". $key['key']. " does not exist in the database\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(2) {
     *   ["generation"]=>
     *   int(4)
     *   ["ttl"]=>
     *   int(1337)
     * }
     * ```
     * **or**
     * ```
     * A user with key 1234 does not exist in the database.
     * ```
     * @link http://www.aerospike.com/docs/guide/glossary.html Glossary
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $metadata an array of `['ttl', 'generation']` values
     * @param array $options An optional array of read policy options, whose keys include
     * * Aerospike::OPT_READ_TIMEOUT
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_REPLICA
     * * Aerospike::OPT_POLICY_CONSISTENCY
     * @see Aerospike::OPT_READ_TIMEOUT Aerospike::OPT_READ_TIMEOUT options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_REPLICA Aerospike::OPT_POLICY_REPLICA options
     * @see Aerospike::OPT_POLICY_CONSISTENCY Aerospike::OPT_POLICY_CONSISTENCY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function exists ( array $key, array &$metadata, array $options = []) {}

    /**
     * Touch the record identified by the $key, resetting its time-to-live.
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $status = $client->touch($key, 120);
     * if ($status == Aerospike::OK) {
     *     echo "Added 120 seconds to the record's expiration.\n"
     * } elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
     *     echo "A user with key ". $key['key']. " does not exist in the database\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * Added 120 seconds to the record's expiration.
     * ```
     * **or**
     * ```
     * A user with key 1234 does not exist in the database.
     * ```
     * @link http://www.aerospike.com/docs/client/c/usage/kvs/write.html#change-record-time-to-live-ttl Time-to-live
     * @link https://www.aerospike.com/docs/guide/FAQ.html FAQ
     * @link https://discuss.aerospike.com/t/records-ttl-and-evictions/737 Record TTL and Evictions
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param int $ttl The record's time-to-live in seconds
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function touch ( array $key, int $ttl = 0, array $options =[]) {}

    /**
     * Remove the record identified by the $key.
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $status = $client->remove($key, array(Aerospike::OPT_POLICY_RETRY => Aerospike::POLICY_RETRY_NONE));
     * if ($status == Aerospike::OK) {
     *     echo "Record removed.\n";
     * } elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
     *     echo "A user with key ". $key['key']. " does not exist in the database\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * Record removed.
     * ```
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function remove ( array $key, array $options = []) {}

    /**
     * Remove $bins from the record identified by the $key.
     *
     * ```php
     * $key = ["ns" => "test", "set" => "users", "key" => 1234];
     * $options = array(Aerospike::OPT_TTL => 3600);
     * $status = $client->removeBin($key, ["age"], $options);
     * if ($status == Aerospike::OK) {
     *     echo "Removed bin 'age' from the record.\n";
     * } elseif ($status == Aerospike::ERR_RECORD_NOT_FOUND) {
     *     echo "The database has no record with the given key.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $bins A list of bin names to remove
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function removeBin ( array $key, array $bins , array $options = []) {}

    /**
     * Increment the value of $bin in the record identified by the $key by an
     * $offset.
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $options = [Aerospike::OPT_TTL => 7200];
     * $status = $client->increment($key, 'pto', -4, $options);
     * if ($status == Aerospike::OK) {
     *     echo "Decremented four vacation days from the user's PTO balance.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param string $bin The name of the bin to increment
     * @param int|float $offset The value by which to increment the bin
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_TTL
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_TTL Aerospike::OPT_TTL options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function increment ( array $key, $bin, $offset, array $options = []) {}

    /**
     * Append a string $value to the one already in $bin, in the record identified by the $key.
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $options = [Aerospike::OPT_TTL => 3600];
     * $status = $client->append($key, 'name', ' Ph.D.', $options);
     * if ($status == Aerospike::OK) {
     *     echo "Added the Ph.D. suffix to the user.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param string $bin The name of the bin
     * @param string $value The string value to append to the bin
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_TTL
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_TTL Aerospike::OPT_TTL options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function append ( array $key, $bin, $value, array $options = []) {}

    /**
     * Prepend a string $value to the one already in $bin, in the record identified by the $key.
     *
     * ```php
     * $key = $client->initKey("test", "users", 1234);
     * $options = [Aerospike::OPT_TTL => 3600];
     * $status = $client->prepend($key, 'name', '*', $options);
     * if ($status == Aerospike::OK) {
     *     echo "Starred the user.\n";
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param string $bin The name of the bin
     * @param string $value The string value to prepend to the bin
     * @param array $options An optional array of write policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_TTL
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_RETRY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_TTL Aerospike::OPT_TTL options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function prepend ( array $key, $bin, $value, array $options = []) {}

    /**
     *  Perform multiple bin operations on a record with a given key, with write operations happening before read ones.
     *
     *  Non-existent bins being read will have a `NULL` value.
     *
     * Currently a call to operate() can include only one write operation per-bin.
     * For example, you cannot both append and prepend to the same bin, in the same call.
     *
     * Like other bin operations, operate() only works on existing records (i.e. ones that were previously created with a put()).
     *
     * **Example #1 Combining several write operations into one multi-op call**
     *
     * ```
     * [
     *   ["op" => Aerospike::OPERATOR_APPEND, "bin" => "name", "val" => " Ph.D."],
     *   ["op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1],
     *   ["op" => Aerospike::OPERATOR_READ, "bin" => "age"]
     * ]
     * ```
     *
     * ```php
     * $config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
     * $client = new Aerospike($config, true);
     * if (!$client->isConnected()) {
     *    echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *    exit(1);
     * }
     *
     * $key = $client->initKey("test", "users", 1234);
     * $operations = [
     *   ["op" => Aerospike::OPERATOR_APPEND, "bin" => "name", "val" => " Ph.D."],
     *   ["op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1],
     *   ["op" => Aerospike::OPERATOR_READ, "bin" => "age"],
     * ];
     * $options = [Aerospike::OPT_TTL => 600];
     * $status = $client->operate($key, $operations, $returned, $options);
     * if ($status == Aerospike::OK) {
     *     var_dump($returned);
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(1) {
     *   ["age"]=>
     *   int(34)
     * }
     * ```
     *
     * **Example #2 Implementing an LRU by reading a bin and touching a record in the same operation**
     *
     * ```
     * [
     *   ["op" => Aerospike::OPERATOR_READ, "bin" => "age"],
     *   ["op" => Aerospike::OPERATOR_TOUCH, "ttl" => 20]
     * ]
     * ```
     * @link http://www.aerospike.com/docs/guide/kvs.html Key-Value Store
     * @link https://github.com/aerospike/aerospike-client-php/blob/master/doc/README.md#handling-unsupported-types Handling Unsupported Types
     * @link http://www.aerospike.com/docs/client/c/usage/kvs/write.html#change-record-time-to-live-ttl Time-to-live
     * @link http://www.aerospike.com/docs/guide/glossary.html Glossary
     * @param array $key The key identifying the record. An array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $operations The array of of one or more per-bin operations conforming to the following structure:
     * ```
     * Write Operation:
     *   op => Aerospike::OPERATOR_WRITE
     *   bin => bin name (cannot be longer than 14 characters)
     *   val => the value to store in the bin
     *
     * Increment Operation:
     *   op => Aerospike::OPERATOR_INCR
     *   bin => bin name
     *   val => the integer by which to increment the value in the bin
     *
     * Prepend Operation:
     *   op => Aerospike::OPERATOR_PREPEND
     *   bin => bin name
     *   val => the string to prepend the string value in the bin
     *
     * Append Operation:
     *   op => Aerospike::OPERATOR_APPEND
     *   bin => bin name
     *   val => the string to append the string value in the bin
     *
     * Read Operation:
     *   op => Aerospike::OPERATOR_READ
     *   bin => name of the bin we want to read after any write operations
     *
     * Touch Operation: reset the time-to-live of the record and increment its generation
     *                  (only combines with read operations)
     *   op => Aerospike::OPERATOR_TOUCH
     *   ttl => a positive integer value to set as time-to-live for the record
     *
     * List Append Operation:
     *   op => Aerospike::OP_LIST_APPEND,
     *   bin =>  "events",
     *   val =>  1234
     *
     * List Merge Operation:
     *   op => Aerospike::OP_LIST_MERGE,
     *   bin =>  "events",
     *   val =>  [ 123, 456 ]
     *
     * List Insert Operation:
     *   op => Aerospike::OP_LIST_INSERT,
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  1234
     *
     * List Insert Items Operation:
     *   op => Aerospike::OP_LIST_INSERT_ITEMS,
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  [ 123, 456 ]
     *
     * List Pop Operation:
     *   op => Aerospike::OP_LIST_POP, # returns a value
     *   bin =>  "events",
     *   index =>  2
     *
     * List Pop Range Operation:
     *   op => Aerospike::OP_LIST_POP_RANGE, # returns a value
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  3 # remove 3 elements starting at index 2
     *
     * List Remove Operation:
     *   op => Aerospike::OP_LIST_REMOVE,
     *   bin =>  "events",
     *   index =>  2
     *
     * List Remove Range Operation:
     *   op => Aerospike::OP_LIST_REMOVE_RANGE,
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  3 # remove 3 elements starting at index 2
     *
     * List Clear Operation:
     *   op => Aerospike::OP_LIST_CLEAR,
     *   bin =>  "events"
     *
     * List Set Operation:
     *   op => Aerospike::OP_LIST_SET,
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  "latest event at index 2" # set this value at index 2
     *
     * List Get Operation:
     *   op => Aerospike::OP_LIST_GET, # returns a value
     *   bin =>  "events",
     *   index =>  2 # similar to Aerospike::OPERATOR_READ but only returns the value
     *                 at index 2 of the list, not the whole bin
     *
     * List Get Range Operation:
     *   op => Aerospike::OP_LIST_GET_RANGE, # returns a value
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  3 # get 3 elements starting at index 2
     *
     * List Trim Operation:
     *   op => Aerospike::OP_LIST_TRIM,
     *   bin =>  "events",
     *   index =>  2,
     *   val =>  3 # remove all elements not in the range between index 2 and index 2 + 3
     *
     * List Size Operation:
     *   op => Aerospike::OP_LIST_SIZE, # returns a value
     *   bin =>  "events" # gets the size of a list contained in the bin
     *
     * ```
     *
     * @param array $returned an array of bins retrieved by read operations. If multiple operations exist for a specific bin name, the last operation will be the one placed as the value
     * @param array $options An optional array of policy options, whose keys include
     * * Aerospike::OPT_WRITE_TIMEOUT
     * * Aerospike::OPT_TTL
     * * Aerospike::OPT_POLICY_RETRY
     * * Aerospike::OPT_POLICY_KEY
     * * Aerospike::OPT_POLICY_GEN
     * * Aerospike::OPT_POLICY_COMMIT_LEVEL
     * * Aerospike::OPT_POLICY_REPLICA
     * * Aerospike::OPT_POLICY_CONSISTENCY
     * @see Aerospike::OPT_WRITE_TIMEOUT Aerospike::OPT_WRITE_TIMEOUT options
     * @see Aerospike::OPT_TTL Aerospike::OPT_TTL options
     * @see Aerospike::OPT_POLICY_RETRY Aerospike::OPT_POLICY_RETRY options
     * @see Aerospike::OPT_POLICY_KEY Aerospike::OPT_POLICY_KEY options
     * @see Aerospike::OPT_POLICY_GEN Aerospike::OPT_POLICY_GEN options
     * @see Aerospike::OPT_POLICY_COMMIT_LEVEL Aerospike::OPT_POLICY_COMMIT_LEVEL options
     * @see Aerospike::OPT_POLICY_REPLICA Aerospike::OPT_POLICY_REPLICA options
     * @see Aerospike::OPT_POLICY_CONSISTENCY Aerospike::OPT_POLICY_CONSISTENCY options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function operate ( array $key, array $operations, array &$returned = [], array $options = []) {}

    // Batch Operation Methods

    /**
     * Read a batch of records from a list of given keys, and fill $records with the resulting indexed array
     *
     * Each record is an array consisting of *key*, *metadata* and *bins* (see: {@see Aerospike::get() get()}).
     * Non-existent records will have `NULL` for their *metadata* and *bins* fields.
     * The bins returned can be filtered by passing an array of bin names.
     *
     * **Note** that the protocol getMany() will use (batch-direct or batch-index)
     * is configurable through the config parameter `Aerospike::USE_BATCH_DIRECT`
     * or `php.ini` config parameter `aerospike.use_batch_direct`.
     * By default batch-index is used with servers that support it (version >= 3.6.0).
     *
     * **Example #1 Aerospike::getMany() default behavior example**
     * ```php
     * $config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
     * $client = new Aerospike($config, true);
     * if (!$client->isConnected()) {
     *    echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *    exit(1);
     * }
     *
     * $key1 = $client->initKey("test", "users", 1234);
     * $key2 = $client->initKey("test", "users", 1235); // this key does not exist
     * $key3 = $client->initKey("test", "users", 1236);
     * $keys = array($key1, $key2, $key3);
     * $status = $client->getMany($keys, $records);
     * if ($status == Aerospike::OK) {
     *     var_dump($records);
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(3) {
     *   [0]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1234)
     *       ["digest"]=>
     *       string(20) "M?v2Kp???
     *
     * ?[??4?v
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(1)
     *     }
     *     ["bins"]=>
     *     array(3) {
     *       ["email"]=>
     *       string(15) "hey@example.com"
     *       ["name"]=>
     *       string(9) "You There"
     *       ["age"]=>
     *       int(33)
     *     }
     *   }
     *   [1]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1235)
     *       ["digest"]=>
     *       string(20) "?C??[?vwS??ƨ?????"
     *     }
     *     ["metadata"]=>
     *     NULL
     *     ["bins"]=>
     *     NULL
     *   }
     *   [2]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1236)
     *       ["digest"]=>
     *       string(20) "'?9?
     *                       ??????
     * ?	?"
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(1)
     *     }
     *     ["bins"]=>
     *     array(3) {
     *       ["email"]=>
     *       string(19) "thisguy@example.com"
     *       ["name"]=>
     *       string(8) "This Guy"
     *       ["age"]=>
     *       int(42)
     *     }
     *   }
     * }
     * ```
     * **Example #2 getMany records with filtered bins**
     * ```php
     * // assuming this follows Example #1
     *
     * $filter = ["email"];
     * $keys = [$key1, $key3];
     * $status = $client->getMany($keys, $records, $filter);
     * if ($status == Aerospike::OK) {
     *     var_dump($records);
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(2) {
     *   [0]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1234)
     *       ["digest"]=>
     *       string(20) "M?v2Kp???
     *
     * ?[??4?v
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(4)
     *     }
     *     ["bins"]=>
     *     array(1) {
     *       ["email"]=>
     *       string(15) "hey@example.com"
     *     }
     *   }
     *   [1]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1236)
     *       ["digest"]=>
     *       string(20) "'?9?
     *                       ??????
     * ?	?"
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(4)
     *     }
     *     ["bins"]=>
     *     array(1) {
     *       ["email"]=>
     *       string(19) "thisguy@example.com"
     *     }
     *   }
     * }
     * ```
     * @param array $keys an array of initialized keys, each key an array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $records filled by an array of record values, each record an array of `['key', 'metadata', 'bins']`
     * @param array $select only these bins out of the record (optional)
     * @param array $options An optional array of read policy options, whose keys include
     * * Aerospike::OPT_READ_TIMEOUT
     * * Aerospike::USE_BATCH_DIRECT
     * @see Aerospike::USE_BATCH_DIRECT Aerospike::USE_BATCH_DIRECT options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @see Aerospike::get() get()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function getMany ( array $keys, array &$records, array $select = [], array $options = []) {}


    /**
     * Check if a batch of records exists in the database and fill $metdata with the results
     *
     * Checks for the existence a batch of given *keys* (see: {@see Aerospike::exists() exists()}),
     * and return an indexed array matching the order of the *keys*.
     * Non-existent records will have `NULL` for their *metadata*.
     *
     * **Note** that the protocol existsMany() will use (batch-direct or batch-index)
     * is configurable through the config parameter `Aerospike::USE_BATCH_DIRECT`
     * or `php.ini` config parameter `aerospike.use_batch_direct`.
     * By default batch-index is used with servers that support it (version >= 3.6.0).
     *
     * **Example #1 Aerospike::existsMany() default behavior example**
     * ```php
     * $config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
     * $client = new Aerospike($config, true);
     * if (!$client->isConnected()) {
     *    echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *    exit(1);
     * }
     *
     * $key1 = $client->initKey("test", "users", 1234);
     * $key2 = $client->initKey("test", "users", 1235); // this key does not exist
     * $key3 = $client->initKey("test", "users", 1236);
     * $keys = array($key1, $key2, $key3);
     * $status = $client->existsMany($keys, $metadata);
     * if ($status == Aerospike::OK) {
     *     var_dump($records);
     * } else {
     *     echo "[{$client->errorno()}] ".$client->error();
     * }
     * ```
     * ```
     * array(3) {
     *   [0]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1234)
     *       ["digest"]=>
     *       string(20) "M?v2Kp???
     *
     * ?[??4?v
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(1)
     *     }
     *   }
     *   [1]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1235)
     *       ["digest"]=>
     *       string(20) "?C??[?vwS??ƨ?????"
     *     }
     *     ["metadata"]=>
     *     NULL
     *   }
     *   [2]=>
     *   array(3) {
     *     ["key"]=>
     *     array(4) {
     *       ["ns"]=>
     *       string(4) "test"
     *       ["set"]=>
     *       string(5) "users"
     *       ["key"]=>
     *       int(1236)
     *       ["digest"]=>
     *       string(20) "'?9?
     *                       ??????
     * ?	?"
     *     }
     *     ["metadata"]=>
     *     array(2) {
     *       ["ttl"]=>
     *       int(4294967295)
     *       ["generation"]=>
     *       int(1)
     *     }
     *   }
     * }
     * ```
     * @param array $keys an array of initialized keys, each key an array with keys `['ns','set','key']` or `['ns','set','digest']`
     * @param array $metadata filled by an array of metadata values, each an array of `['key', 'metadata']`
     * @param array $options An optional array of read policy options, whose keys include
     * * Aerospike::OPT_READ_TIMEOUT
     * * Aerospike::USE_BATCH_DIRECT
     * @see Aerospike::USE_BATCH_DIRECT Aerospike::USE_BATCH_DIRECT options
     * @see Aerospike::OK Aerospike::OK and error status codes
     * @see Aerospike::error() error()
     * @see Aerospike::errorno() errorno()
     * @see Aerospike::exists() exists()
     * @return int The status code of the operation. Compare to the Aerospike class status constants.
     */
    public function existsMany ( array $keys, array &$metadata, array $options = []) {}

    // Logging Methods

    /**
     * Set the logging threshold of the Aerospike object
     *
     * @param $log_level one of `Aerospike::LOG_LEVEL_*` values
     * * Aerospike::LOG_LEVEL_OFF
     * * Aerospike::LOG_LEVEL_ERROR
     * * Aerospike::LOG_LEVEL_WARN
     * * Aerospike::LOG_LEVEL_INFO
     * * Aerospike::LOG_LEVEL_DEBUG
     * * Aerospike::LOG_LEVEL_TRACE
     * @see Aerospike::LOG_LEVEL_OFF Aerospike::LOG_LEVEL_* constants
     */
    public function setLogLevel ( int $log_level ) {}

    /**
     * Set a handler for log events
     *
     * Registers a callback method that will be triggered whenever a logging event above the declared log threshold occurs.
     *
     * ```php
     * $config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
     * $client = new Aerospike($config, true);
     * if (!$client->isConnected()) {
     *   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
     *   exit(1);
     * }
     * $client->setLogLevel(Aerospike::LOG_LEVEL_DEBUG);
     * $client->setLogHandler(function ($level, $file, $function, $line) {
     *   switch ($level) {
     *     case Aerospike::LOG_LEVEL_ERROR:
     *       $lvl_str = 'ERROR';
     *       break;
     *     case Aerospike::LOG_LEVEL_WARN:
     *       $lvl_str = 'WARN';
     *       break;
     *     case Aerospike::LOG_LEVEL_INFO:
     *       $lvl_str = 'INFO';
     *       break;
     *     case Aerospike::LOG_LEVEL_DEBUG:
     *       $lvl_str = 'DEBUG';
     *       break;
     *     case Aerospike::LOG_LEVEL_TRACE:
     *       $lvl_str = 'TRACE';
     *       break;
     *     default:
     *       $lvl_str = '???';
     *   }
     *   error_log("[$lvl_str] in $function at $file:$line");
     * });
     * ```
     *
     * @see Aerospike::LOG_LEVEL_OFF Aerospike::LOG_LEVEL_* constants
     * @param callback $log_handler a callback function with the signature
     * ```php
     * function log_handler ( int $level, string $file, string $function, int $line ) : void
     * ```
     */
    public function setLogHandler ( callback $log_handler ) {}

    // Unsupported Type Handler Methods

    /**
     * Set a serialization handler for unsupported types
     *
     * Registers a callback method that will be triggered whenever a write method handles a value whose type is unsupported.
     * This is a static method and the *serialize_cb* handler is global across all instances of the Aerospike class.
     *
     * ```php
     * Aerospike::setSerializer(function ($val) {
     *   return gzcompress(json_encode($val));
     * });
     * ```
     *
     * @link https://github.com/citrusleaf/aerospike-client-php7/tree/master/doc#handling-unsupported-types Handling Unsupported Types
     * @param callback $serialize_cb a callback invoked for each value of an unsupported type, when writing to the cluster. The function must follow the signature
     * ```php
     * function aerospike_serialize ( mixed $value ) : string
     * ```
     * @see Aerospike::OPT_SERIALIZER Aerospike::OPT_SERIALIZER options
     */
    public function setSerializer ( callback $serialize_cb ) {}

    /**
     * Set a deserialization handler for unsupported types
     *
     * Registers a callback method that will be triggered whenever a read method handles a value whose type is unsupported.
     * This is a static method and the *unserialize_cb* handler is global across all instances of the Aerospike class.
     *
     * ```php
     * Aerospike::setDeserializer(function ($val) {
     *   return json_decode(gzuncompress($val));
     * });
     * ```
     *
     * @link https://github.com/citrusleaf/aerospike-client-php7/tree/master/doc#handling-unsupported-types Handling Unsupported Types
     * @param callback $unserialize_cb a callback invoked for each value of an unsupported type, when reading from the cluster. The function must follow the signature
     * ```php
     * // $value is binary data of type AS_BYTES_BLOB
     * function aerospike_deserialize ( string $value )
     * ```
     * @see Aerospike::OPT_SERIALIZER Aerospike::OPT_SERIALIZER options
     */
    public function setDeserializer ( callback $unserialize_cb ) {}


    /**
     * Options can be assigned values that modify default behavior
     * Used by the constructor, read, write, scan, query, apply, and info
     * operations.
     */

    /**
     * Defines the length of time (in milliseconds) the client waits on establishing a connection.
     * @const OPT_CONNECT_TIMEOUT value in milliseconds (default: 1000)
     */
    const OPT_CONNECT_TIMEOUT = 0;
    /**
     * Defines the length of time (in milliseconds) the client waits on a read
     * operation.
     * @const OPT_READ_TIMEOUT value in milliseconds (default: 1000)
     */
    const OPT_READ_TIMEOUT = 0;
    /**
     * Defines the length of time (in milliseconds) the client waits on a write
     * operation.
     * @const OPT_WRITE_TIMEOUT value in milliseconds (default: 1000)
     */
    const OPT_WRITE_TIMEOUT = 0;

    /**
     * Sets the TTL of the record along with a write operation.
     *
     * * TTL > 0 sets the number of seconds into the future in which to expire the record.
     * * TTL = 0 uses the default TTL defined for the namespace.
     * * TTL = -1 means the record should never expire.
     * * TTL = -2 means the record's TTL should not be modified.
     * @const OPT_TTL value in seconds, or the special values 0, -1 or -2 (default: 0)
     */
    const OPT_TTL = "OPT_TTL";

    /**
     * Accepts one of the POLICY_KEY_* values.
     *
     * {@link http://www.aerospike.com/docs/client/php/usage/kvs/record-structure.html Records}
     * are uniquely identified by their digest, and can optionally store the value of their primary key
     * (their unique ID in the application).
     * @const OPT_POLICY_KEY Key policy option (digest-only or send key)
     */
    const OPT_POLICY_KEY = "OPT_POLICY_KEY";
    /**
     * Do not store the primary key with the record (default)
     * @const POLICY_KEY_DIGEST digest only
     */
    const POLICY_KEY_DIGEST = 0;
    /**
     * Store the primary key with the record
     * @const POLICY_KEY_SEND store the primary key with the record
     */
    const POLICY_KEY_SEND = 1;

    /**
     * Accepts one of the POLICY_EXISTS_* values.
     *
     * By default writes will try to create a record or update its bins, which
     * is a behavior similar to how arrays work in PHP. Setting a write with a
     * different POLICY\_EXISTS\_* value can simulate a more DML-like behavior,
     * similar to an RDBMS.
     * @const OPT_POLICY_EXISTS existence policy option
     */
    const OPT_POLICY_EXISTS = "OPT_POLICY_EXISTS";
    /**
     * "CREATE_OR_UPDATE" behavior. Create the record if it does not exist,
     * or update its bins if it does. (default)
     * @const POLICY_EXISTS_IGNORE create or update behavior
     */
    const POLICY_EXISTS_IGNORE = 0;
    /**
     * Create a record ONLY if it DOES NOT exist.
     * @const POLICY_EXISTS_CREATE create only behavior (fail otherwise)
     */
    const POLICY_EXISTS_CREATE = 1;
    /**
     * Update a record ONLY if it exists.
     * @const POLICY_EXISTS_UPDATE update only behavior (fail otherwise)
     */
    const POLICY_EXISTS_UPDATE = 2;
    /**
     * Replace a record ONLY if it exists.
     * @const POLICY_EXISTS_REPLACE replace only behavior (fail otherwise)
     */
    const POLICY_EXISTS_REPLACE = 3;
    /**
     * Create the record if it does not exist, or replace its bins if it does.
     * @const POLICY_EXISTS_CREATE_OR_REPLACE create or replace behavior
     */
    const POLICY_EXISTS_CREATE_OR_REPLACE = 4;

    /**
     * Set to an array( Aerospike::POLICY_GEN_* [, (int) $gen_value ] )
     *
     * Specifies the behavior of write opertions with regards to the record's 
     * generation. Used to implement a check-and-set (CAS) pattern.
     * @const OPT_POLICY_GEN generation policy option
     */
    const OPT_POLICY_GEN = "OPT_POLICY_GEN";
    /**
     * Do not consider generation for the write operation.
     * @const POLICY_GEN_IGNORE write a record, regardless of generation (default)
     */
    const POLICY_GEN_IGNORE = 0;
    /**
     * Only write if the record was not modified since a given generation value.
     * @const POLICY_GEN_EQ write a record, ONLY if generations are equal
     */
    const POLICY_GEN_EQ = 1;
    /**
     *
     * @const POLICY_GEN_GT write a record, ONLY if local generation is greater-than remote generation
     */
    const POLICY_GEN_GT = 2;

    /**
     * Set to one of the SERIALIZER_* values.
     *
     * Supported types, such as string, integer, and array get directly cast to
     * the matching Aerospike types, such as as_string, as_integer, and as_map.
     * Unsupported types, such as boolean, need a serializer to handle them.
     * @const OPT_SERIALIZER determines a handler for unsupported data types
     */
    const OPT_SERIALIZER = "OPT_SERIALIZER";
    /**
     * Throw an exception instead of serializing unsupported types.
     * @const SERIALIZER_NONE throw an error when serialization is required
     */
    const SERIALIZER_NONE = 0;
    /**
     * Use the built-in PHP serializer for any unsupported types.
     * @const SERIALIZER_PHP use the PHP serialize/unserialize functions (default)
     */
    const SERIALIZER_PHP = 1;
    /**
     * Use a user-defined serializer for any unsupported types.
     * @const SERIALIZER_USER use a pair of functions written in PHP for serialization
     */
    const SERIALIZER_USER = 2;

    /**
     * Accepts one of the POLICY_COMMIT_LEVEL_* values.
     *
     * One of the {@link http://www.aerospike.com/docs/client/php/usage/kvs/record-structure.html per-transaction consistency guarantees}.
     * Specifies the number of replicas required to be successfully committed
     * before returning success in a write operation to provide the desired
     * consistency guarantee.
     * @const OPT_POLICY_COMMIT_LEVEL commit level policy option
     */
    const OPT_POLICY_COMMIT_LEVEL = "OPT_POLICY_COMMIT_LEVEL";
    /**
     * Return succcess only after successfully committing all replicas.
     * @const POLICY_COMMIT_LEVEL_ALL write to the master and all prole replicas (default)
     */
    const POLICY_COMMIT_LEVEL_ALL = 0;
    /**
     * Return succcess after successfully committing the master replica.
     * @const POLICY_COMMIT_LEVEL_MASTER master will asynchronously write to prole replicas (default)
     */
    const POLICY_COMMIT_LEVEL_MASTER = 1;

    /**
     * Accepts one of the POLICY_REPLICA_* values.
     *
     * One of the {@link http://www.aerospike.com/docs/client/php/usage/kvs/record-structure.html per-transaction consistency guarantees}.
     * Specifies which partition replica to read from.
     * @const OPT_POLICY_REPLICA replica policy option
     */
    const OPT_POLICY_REPLICA = "OPT_POLICY_REPLICA";
    /**
     * Read from the partition master replica node (default).
     * @const POLICY_REPLICA_MASTER read from master (default)
     */
    const POLICY_REPLICA_MASTER = 0;
    /**
     * Read from an unspecified replica node.
     * @const POLICY_REPLICA_ANY read from any replica node  (default)
     */
    const POLICY_REPLICA_ANY = 1;

    /**
     * Accepts one of the POLICY_CONSISTENCY_* values.
     *
     * One of the {@link http://www.aerospike.com/docs/client/php/usage/kvs/record-structure.html per-transaction consistency guarantees}.
     * Specifies the number of replicas to be consulted in a read operation to
     * provide the desired consistency guarantee.
     * @const OPT_POLICY_CONSISTENCY commit level policy option
     */
    const OPT_POLICY_CONSISTENCY = "OPT_POLICY_CONSISTENCY";
    /**
     * Involve a single replica in the operation.
     * @const POLICY_CONSISTENCY_ONE (default)
     */
    const POLICY_CONSISTENCY_ONE = 0;
    /**
     * Involve all replicas in the operation.
     * @const POLICY_CONSISTENCY_ALL (default)
     */
    const POLICY_CONSISTENCY_ALL = 1;

    /**
     * Accepts one of the POLICY_RETRY_* values.
     *
     * Determines if an operation should be retried.
     * @const OPT_POLICY_RETRY retry policy option
     */
    const OPT_POLICY_RETRY = "OPT_POLICY_RETRY";
    /**
     * Only attempt an operation once.
     * @const POLICY_RETRY_NONE do not retry a failed operation (default)
     */
    const POLICY_RETRY_NONE = 0;
    /**
     * If an operation fails, attempt the operation one more time.
     * @const POLICY_RETRY_ONCE allow for a single retry on an operation
     */
    const POLICY_RETRY_ONCE = 1;

    /**
     * Accepts one of the SCAN_PRIORITY_* values.
     *
     * @const OPT_SCAN_PRIORITY The priority of the scan
     */
    const OPT_SCAN_PRIORITY = "OPT_SCAN_PRIORITY";
    /**
     * The cluster will auto-adjust the priority of the scan.
     * @const SCAN_PRIORITY_AUTO auto-adjust the scan priority (default)
     */
    const SCAN_PRIORITY_AUTO = 0;
    /**
     * Set the scan as having low priority.
     * @const SCAN_PRIORITY_LOW low priority scan
     */
    const SCAN_PRIORITY_LOW = 1;
    /**
     * Set the scan as having medium priority.
     * @const SCAN_PRIORITY_MEDIUM medium priority scan
     */
    const SCAN_PRIORITY_MEDIUM = 2;
    /**
     * Set the scan as having high priority.
     * @const SCAN_PRIORITY_HIGH high priority scan
     */
    const SCAN_PRIORITY_HIGH = 3;

    /**
     * Do not return the bins of the records matched by the scan.
     *
     * @const OPT_SCAN_NOBINS boolean value (default: false)
     */
    const OPT_SCAN_NOBINS = "OPT_SCAN_NOBINS";

    /**
     * Set the scan to run over a given percentage of the possible records.
     *
     * @const OPT_SCAN_PERCENTAGE integer value from 1-100 (default: 100)
     */
    const OPT_SCAN_PERCENTAGE = "OPT_SCAN_PERCENTAGE";

    /**
     * Scan all the nodes in the cluster concurrently.
     *
     * @const OPT_SCAN_CONCURRENTLY boolean value (default: false)
     */
    const OPT_SCAN_CONCURRENTLY = "OPT_SCAN_CONCURRENTLY";

    /**
     * Revert to the older batch-direct protocol, instead of batch-index.
     *
     * @const USE_BATCH_DIRECT boolean value (default: false)
     */
    const USE_BATCH_DIRECT = "USE_BATCH_DIRECT";

    /**
     * @const LOG_LEVEL_OFF
     */
    const LOG_LEVEL_OFF = "LOG_LEVEL_OFF";
    /**
     * @const LOG_LEVEL_ERROR
     */
    const LOG_LEVEL_ERROR = "LOG_LEVEL_ERROR";
    /**
     * @const LOG_LEVEL_WARN
     */
    const LOG_LEVEL_WARN = "LOG_LEVEL_WARN";
    /**
     * @const LOG_LEVEL_INFO
     */
    const LOG_LEVEL_INFO = "LOG_LEVEL_INFO";
    /**
     * @const LOG_LEVEL_DEBUG
     */
    const LOG_LEVEL_DEBUG = "LOG_LEVEL_DEBUG";
    /**
     * @const LOG_LEVEL_TRACE
     */
    const LOG_LEVEL_TRACE = "LOG_LEVEL_TRACE";


    // Aerospike Status Codes:
    //
    // Each Aerospike API method invocation returns a status code
    //  depending upon the success or failure condition of the call.
    //
    // The error status codes map to the C client
    //  src/include/aerospike/as_status.h

    // Client status codes:
    //
    const ERR_PARAM              ; // Invalid client parameter
    const ERR_CLIENT             ; // Generic client error

    // Server status codes:
    //
    const OK                     ; // Success status
    const ERR_SERVER             ; // Generic server error
    const ERR_SERVER_FULL        ; // Node running out of memory/storage
    const ERR_DEVICE_OVERLOAD    ; // Node storage lagging write load
    const ERR_TIMEOUT            ; // Client or server side timeout error
    const ERR_CLUSTER            ; // Generic cluster discovery and connection error
    const ERR_CLUSTER_CHANGE     ; // Cluster state changed during the request
    const ERR_REQUEST_INVALID    ; // Invalid request protocol or protocol field
    const ERR_UNSUPPORTED_FEATURE;
    const ERR_NO_XDR             ; // XDR not available for the cluster
    // Record specific:
    const ERR_NAMESPACE_NOT_FOUND;
    const ERR_RECORD_NOT_FOUND   ;
    const ERR_RECORD_EXISTS      ; // Record already exists
    const ERR_RECORD_GENERATION  ; // Write policy regarding generation violated
    const ERR_RECORD_TOO_BIG     ; // Record written cannot fit in storage write block
    const ERR_RECORD_BUSY        ; // Hot key: too many concurrent requests for the record
    const ERR_RECORD_KEY_MISMATCH; // Digest incompatibility?
    // Bin specific:
    const ERR_BIN_NAME           ; // Name too long or exceeds the unique name quota for the namespace
    const ERR_BIN_NOT_FOUND      ;
    const ERR_BIN_EXISTS         ; // Bin already exists
    const ERR_BIN_INCOMPATIBLE_TYPE;
    // Query and Scan operations:
    const ERR_SCAN_ABORTED       ; // Scan aborted by the user
    const ERR_QUERY              ; // Generic query error
    const ERR_QUERY_END          ; // Out of records to query
    const ERR_QUERY_ABORTED      ; // Query aborted by the user
    const ERR_QUERY_QUEUE_FULL   ;
    // Index operations:
    const ERR_INDEX              ; // Generic secondary index error
    const ERR_INDEX_OOM          ; // Index out of memory
    const ERR_INDEX_NOT_FOUND    ;
    const ERR_INDEX_FOUND        ;
    const ERR_INDEX_NOT_READABLE ;
    const ERR_INDEX_NAME_MAXLEN  ;
    const ERR_INDEX_MAXCOUNT     ; // Max number of indexes reached
    // UDF operations:
    const ERR_UDF                ; // Generic UDF error
    const ERR_UDF_NOT_FOUND      ; // UDF does not exist
    const ERR_LUA_FILE_NOT_FOUND ; // Source file for the module not found
    // Security operations:
    const ERR_SECURITY_NOT_SUPPORTED;
    const ERR_SECURITY_NOT_ENABLED;
    const ERR_SECURITY_SCHEME_NOT_SUPPORTED;
    const ERR_INVALID_USER;
    const ERR_USER_ALREADY_EXISTS;
    const ERR_INVALID_PASSWORD;
    const ERR_EXPIRED_PASSWORD;
    const ERR_FORBIDDEN_PASSWORD;
    const ERR_INVALID_CREDENTIAL;
    const ERR_INVALID_ROLE;
    const ERR_INVALID_PRIVILEGE;
    const ERR_NOT_AUTHENTICATED;
    const ERR_ROLE_VIOLATION;

    // Status values returned by scanInfo(). Deprecated in favor of jobInfo()
    const SCAN_STATUS_UNDEF;      // scan status is undefined. deprecated.
    const SCAN_STATUS_INPROGRESS; // scan is currently running. deprecated.
    const SCAN_STATUS_ABORTED;    // scan was aborted due to failure or the user. deprecated.
    const SCAN_STATUS_COMPLETED;  // scan completed successfully. deprecated.

    // Status values returned by jobInfo()
    const JOB_STATUS_UNDEF;      // the job's status is undefined.
    const JOB_STATUS_INPROGRESS; // the job is currently running.
    const JOB_STATUS_COMPLETED;  // the job completed successfully.

    // Query Predicate Operators
    const string OP_EQ = '=';
    const string OP_BETWEEN = 'BETWEEN';
    const string OP_CONTAINS = 'CONTAINS';
    const string OP_RANGE = 'RANGE';

    // Multi-operation operators map to the C client
    //  src/include/aerospike/as_operations.h
    const OPERATOR_WRITE;
    const OPERATOR_READ;
    const OPERATOR_INCR;
    const OPERATOR_PREPEND;
    const OPERATOR_APPEND;
    const OPERATOR_TOUCH;

    // UDF types
    const UDF_TYPE_LUA;

    // index types
    const INDEX_TYPE_DEFAULT;   // index records where the bin contains an atomic (string, integer) type
    const INDEX_TYPE_LIST;      // index records where the bin contains a list
    const INDEX_TYPE_MAPKEYS;   // index the keys of records whose specified bin is a map
    const INDEX_TYPE_MAPVALUES; // index the values of records whose specified bin is a map
    // data type
    const INDEX_STRING;  // if the index type is matched, regard values of type string
    const INDEX_NUMERIC; // if the index type is matched, regard values of type integer

    // Security role privileges
    const PRIV_READ; // user can read data only
    const PRIV_READ_WRITE_UDF; // can read and write data through User-Defined Functions
    const PRIV_USER_ADMIN; // user can edit/remove other users
    const PRIV_SYS_ADMIN; // can perform sysadmin functions that do not involve user admin
    const PRIV_DATA_ADMIN; // can perform data admin functions that do not involve user admin
/*
    // key-value methods
    public int listSize ( array $key, string $bin, int &$count [, array $options ] )
    public int listAppend ( array $key, string $bin, mixed $value [, array $options ] )
    public int listMerge ( array $key, string $bin, array $items [, array $options ] )
    public int listInsert ( array $key, string $bin, int $index, mixed $value [, array $options ] )
    public int listInertItems ( array $key, string $bin, int $index, array $items [, array $options ] )
    public int listPop ( array $key, string $bin, int $index, mixed &$element [, array $options ] )
    public int listPopRange ( array $key, string $bin, int $index, int $count, array &$elements [, array $options ] )
    public int listRemove ( array $key, string $bin, int $index [, array $options ] )
    public int listRemoveRange ( array $key, string $bin, int $index, int $count [, array $options ] )
    public int listTrim ( array $key, string $bin, int $index, int $count [, array $options ] )
    public int listClear ( array $key, string $bin [, array $options ] )
    public int listSet ( array $key, string $bin, int $index, mixed $val [, array $options ] )
    public int listGet ( array $key, string $bin, int $index, mixed &$element [, array $options ] )
    public int listGetRange ( array $key, string $bin, int $index, int $count, array &$elements [, array $options ] )

    // UDF methods
    public int register ( string $path, string $module [, int $language = Aerospike::UDF_TYPE_LUA] )
    public int deregister ( string $module )
    public int listRegistered ( array &$modules [, int $language ] )
    public int getRegistered ( string $module, string &$code )
    public int apply ( array $key, string $module, string $function[, array $args [, mixed &$returned [, array $options ]]] )
    public int aggregate ( string $ns, string $set, array $where, string $module, string $function, array $args, mixed &$returned [, array $options ] )
    public int scanApply ( string $ns, string $set, string $module, string $function, array $args, int &$scan_id [, array $options ] )
    public int queryApply ( string $ns, string $set, array $where, string $module, string $function, array $args, int &$job_id [, array $options ] )
    public int jobInfo ( integer $job_id, array &$info [, array $options ] )
    public int scanInfo ( integer $scan_id, array &$info [, array $options ] ) // DEPRECATED. use jobInfo()

    // query and scan methods
    public int query ( string $ns, string $set, array $where, callback $record_cb [, array $select [, array $options ]] )
    public int scan ( string $ns, string $set, callback $record_cb [, array $select [, array $options ]] )
    public array predicateEquals ( string $bin, int|string $val )
    public array predicateBetween ( string $bin, int $min, int $max )
    public array predicateContains ( string $bin, int $index_type, int|string $val )
    public array predicateRange ( string $bin, int $index_type, int $min, int $max )

    // admin methods
    public int addIndex ( string $ns, string $set, string $bin, string $name, int $index_type, int $data_type [, array $options ] )
    public int dropIndex ( string $ns, string $name [, array $options ] )

    // info methods
    public int info ( string $request, string &$response [, array $host [, array $options ] ] )
    public array infoMany ( string $request [, array $config [, array $options ]] )
    public array getNodes ( void )

    // security methods
    public int createRole ( string $role, array $privileges [, array $options ] )
    public int grantPrivileges ( string $role, array $privileges [, array $options ] )
    public int revokePrivileges ( string $role, array $privileges [, array $options ] )
    public int queryRole ( string $role, array &$privileges [, array $options ] )
    public int queryRoles ( array &$roles [, array $options ] )
    public int dropRole ( string $role [, array $options ] )
    public int createUser ( string $user, string $password, array $roles [, array $options ] )
    public int setPassword ( string $user, string $password [, array $options ] )
    public int changePassword ( string $user, string $password [, array $options ] )
    public int grantRoles ( string $user, array $roles [, array $options ] )
    public int revokeRoles ( string $user, array $roles [, array $options ] )
    public int queryUser ( string $user, array &$roles [, array $options ] )
    public int queryUsers ( array &$roles [, array $options ] )
    public int dropUser ( string $user [, array $options ] )

 */
}
