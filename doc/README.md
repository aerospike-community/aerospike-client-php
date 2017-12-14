
# Overview

The Aerospike <a href="http://www.aerospike.com/docs/architecture/clients.html"
target="_doc">client</a> for PHP enables your application to work with an
Aerospike <a href="http://www.aerospike.com/docs/architecture/distribution.html"
target="_doc">cluster</a> as its
<a href="http://www.aerospike.com/docs/guide/kvs.html" target="_doc">key-value store</a>.

The <a href="http://www.aerospike.com/docs/architecture/data-model.html" target="_doc">Data Model</a>
document gives further details on how data is organized in the cluster.

## Generating documentation with phpDocumentor

The Aerospike client for PHP is written as a C module, but we have added a PHP
stub under `docs/phpdoc`. You can import the stub into your IDE for
auto-completion, or build the API documentation using
[phpDocumentor](https://phpdoc.org/)

```
phpdoc run -v -d phpdoc/ -t html/
```

## Aerospike Session Handler

The Aerospike PHP client provides a custom session handler that can be used to
store user sessions in Aerospike. A set and namespace must be given to act as
the container for the sessions.

The session ID is used as the primary key of the record containing the
session. The key-value pairs in the `$_SESSION` object are stored in the matching
record. The value of `session.gc_maxlifetime` is used as the record ttl.


`session.save_handler string`
    Set to _aerospike_ to enable sessions support.

`session.save_path string`
    A string formatted as **ns|set|addr:port\[,addr:port\[,...\]\]**. for example "test|sess|127.0.0.1:3000"
    As with the *$config* of the constructor, the host info of just one cluster node is necessary.

## Configuration in a Web Server Context

Initializing the client to a cluster is a costly
operation, so ideally it should be reused for the multiple requests
that the same PHP process will handle (as is the case for mod\_php and fastCGI).

The developer can determine whether the constructor will
use persistent connections by way of an optional boolean argument.
After the first time `Aerospike::__construct__()` is called within the process, the
client will be stored for reuse by subsequent requests, if persistent connections
were indicated. With persistent connections, the methods _reconnect()_ and
_close()_ do not actually close the connection.

Each Aerospike client opens 2*N connections to the cluster, where N is the number
of nodes in that cluster, and a connection simply costs a file descriptor
on the server-side. Depending on the number of web servers and PHP processes on
each server, you may need to adjust the
[proto-fd-max](http://www.aerospike.com/docs/reference/configuration/#proto-fd-max)
server config parameter and the OS limits to account for the necessary number of
[file descriptors](http://www.aerospike.com/docs/operations/troubleshoot/startup/#not-enough-file-descriptors-error-in-log).
On the client side, the web server should be configured to reduce the frequency
in which new clients are created. Historically, the `max_requests` for mod\_php
and FPM was set low to combat memory leaks. PHP has been stable on memory for a
long while, so the correct configuration would be to have fewer processes, and
let each of them handle a high number of requests. This reduces the process
initialization overhead, and the overall number of connections on the web
server. Monitoring the memory consumption of the PHP processes allows the
`max_requests` to be raised safely to an efficient, stable value.

The client keeps track of changes at the server-side through a
[cluster tending](http://www.aerospike.com/docs/architecture/clustering.html)
thread. In a web server context, a single client can handle cluster tending and
share its state through a shared-memory segment. To enable shm cluster tending,
the developer can set the `aerospike.shm.use` ini config
to `true`, or at the constructor through its config.

## Halting a Stream

Halting a _query()_ or _scan()_ result stream can be done by returning (an
explicit) boolean **false** from the callback.
The client will then close the sockets to the nodes of the cluster, halting the
stream of records.

## How PHP Data is stored in Aerospike
The PHP Data types of Integer, String, Float, Array map directly to Aerospike data types.
All other data types will be handled through serialization. See the "Handling Unsupported Types"
sections for details.
The following table shows how data is stored

|PHP Data Type    | Aerospike Data type|
|-----------------|--------------------|
|Integer          | integer            |
|Float            | float              |
|String           | string             |
|Array            | (list or map)*     |
|\Aerospike\Bytes | blob               |

Depending on the structure of the array in PHP, the data will be either stored as an as_list, or an as_map.
If the array has only integer keys, which are sequential and begin with 0, it will be stored as `as_list`. Otherwise
it will be stored as `as_map`. For example ["a", "b", "c"] would be stored as a list, whereas ["a" => "1", "b", "c"=>"d"] would
be stored as as\_map.

Both list and map datatypes from the server will be converted into PHP arrays
## Handling Unsupported Types

See: [Data Types](http://www.aerospike.com/docs/guide/data-types.html)
See: [as_bytes.h](https://github.com/aerospike/aerospike-common/blob/master/src/include/aerospike/as_bytes.h)
* Allow the user to register their own serializer/deserializer method
 - OPT\_SERIALIZER : SERIALIZER\_PHP (default), SERIALIZER\_NONE, SERIALIZER\_USER
* when a write operation runs into types that do not map directly to Aerospike DB types it checks the OPT\_SERIALIZER setting:
 - if SERIALIZER\_NONE it returns an Aerospike::ERR\_PARAM error
 - if SERIALIZER\_PHP it calls the PHP serializer, sets the object's as\_bytes\_type to AS\_BYTES_PHP. This is the default behavior.
 - if SERIALIZER\_USER it calls the PHP function the user registered a callback with Aerospike::setSerializer(), and sets as\_bytes\_type to AS\_BYTES\_BLOB
* when a read operation extracts a value from an AS\_BYTES type bin:
 - if it’s a AS\_BYTES\_PHP use the PHP unserialize function
 - if it’s a AS\_BYTES\_BLOB and the user registered a callback with Aerospike::setDeserializer() call that function, otherwise convert it to \Aerospike\Bytes

**Warning**
Storing \Aerospike\Bytes results in information being stored as: AS\_BYTES\_BLOB . Subsequent retrieval of this data will be passed to a user
specified deserializer if it has been registered. Storing \Aerospike\Bytes and using a user specified deserializer will likely
cause errors, and is strongly discouraged.

**Warning:** Strings in PHP are a binary-safe structure that allows for the
null-byte (**\0**) to be stored inside the string, not just at its end.
Binary-strings with this characteristic are created by calling functions such
as serialize() and gzdeflate(). As a result, the Aerospike client may truncate
the resulting strings. On the Aerospike server, strings are a data type that can
be queried using a secondary index, while bytes are a data type that is only
used for storage. The developer should wrap binary-string with an object to
distinguish them. This allows the serializer to behave in the correct manner.

### Example:
```php
require('autoload.php');
$client = new Aerospike(['hosts'=>[['addr'=>'127.0.0.1', 'port'=>3000]]]);

$str = 'Glagnar\'s Human Rinds, "It\'s a bunch\'a munch\'a crunch\'a human!';
$deflated = new \Aerospike\Bytes(gzdeflate($str));
$wrapped = new \Aerospike\Bytes("trunc\0ated");

$key = $client->initKey('test', 'demo', 'wrapped-bytes');
$status = $client->put($key, ['unwrapped'=>"trunc\0ated", 'wrapped'=> $wrapped, 'deflated' => $deflated]);
if ($status !== Aerospike::OK) {
    die($client->error());
}
$client->get($key, $record);
$wrapped = \Aerospike\Bytes::unwrap($record['bins']['wrapped']);
$deflated = $record['bins']['deflated'];
$inflated = gzinflate($deflated->s);
echo "$inflated\n";
echo "wrapped binary-string: ";
var_dump($wrapped);
$unwrapped = $record['bins']['unwrapped'];
echo "The binary-string that was given to put() without a wrapper: $unwrapped\n";

$client->close();
```
Outputs:
```
Glagnar's Human Rinds, "It's a bunch'a munch'a crunch'a human!
wrapped binary-string: string(10) "truncated"
The binary-string that was given to put() without a wrapper: trunc
```

## Differences from the previous Aerospike PHP 5 Client:
- LDT Support has been removed.
- Type checking in general is stricter for method parameters. If you are not sure whether an argument to a function is an integer or a string, we recommend casting it to the type specified by the method. Running with strict_types enabled may help to catch some issues.
- An exception will be raised if the constructor fails to connect to the cluster.
- \Aerospike\Bytes will be stored to the server as type AS\_BYTES\_BLOB instead of AS\_BYTES\_PHP. This change allows better compatability with other clients.
- Correspondingly, data stored in the server as AS\_BYTES\_BLOB will be returned as Aerospike\Bytes,  if no deserializer has been registered. The Previous version of the Aerospike PHP Client returned a string if AS_BYTES_BLOB was encountered with no registered deserializer.
- Support for PHP versions < 7 has been removed.
- The INI entry `aerospike.serializer` now takes an integer value. 0 for No Serializer, 1 for default PHP serialization, and 2 for user specified serializer. See [Configuration](phpdoc/aerospike.php) for additional information on the code values.
- The constructor will no longer attempt to create a unique SHM key for the user. If a key is not specified in the shm configuration array, the default value will be used. A key provided in the constructor takes precedence over a value specified by INI.
- The layout of the shared memory used by the client when using an SHM key has changed. The default key has changed as well in order to prevent accidental sharing between new and old clients.
- The formatting of the response from an info call may have changed. It now includes the request at the beginning of the response.
- When using initKey with a digest, the digest must now be exactly 20 bytes.
- The integer values of the `Aerospike::LOG_LEVEL_*` constants have changed. This should not effect the user unless they were providing log levels as integers rather than using the constants.
- `Aerospike::LOG_LEVEL_OFF` has been removed. It no longer had any effect.

## PHP Version Support
PHP >= 7 is supported by this client.

## Further Reading

- [How does the Aerospike client find a node](https://discuss.aerospike.com/t/how-does-aerospike-client-find-a-node/706)
