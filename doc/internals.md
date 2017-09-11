
# Aerospike PHP Client Internal Architecture

The Aerospike PHP client provides access to the Aerospike server via a
set of PHP object classes.  The objects are implanted as a PHP extension
implemented in the C programming language using the Zend Engine
extension API.  The Aerospike PHP client extension is layered on top of
the Aerospike C client.  [Note:  The Aerospike C client version 3 is
required by the PHP client, as is Lua 5.1.]

The Aerospike PHP client API is structurally quit similar to the other
Aerospike client APIs, especially the Java client API.

## Arrays as Containers

PHP array objects are used to describe data structures, such as for
specifying a particular object in the database or for the results
returned by a request to retrieve that object from the database.

Internally, the PHP API method implementation C function receives an
array object as an input `zval` argument.  The C function first
validates each argument type (to the extent that it can) and the
contents of structured objects.  A number of the API methods are
polymorphic, meaning they have different behaviors depending upon
received argument types.

## Key Structure

Objects in the Aerospike database are uniquely referenced by a key
structure.  In the PHP client, keys are specified by an array with three
fields: the namespace ("ns"), the set ("set") and the object being used
as the name of the object ("key").

For example, to refer to the key `key1` in namespace `test` and set
`demo`, an array of the following form must be created:

```
$my_key = array("ns"=>"test", "set"=>"demo", "key"=>"key1");
```

## Record Structure

Each object in the database has metadata described by two integer
properties, a generation (or version) and a lifetime (Time To Live, or
TTL.)

For example, to define record metadata with a generation of 5 and a TTL
of 3600, an array of the following form must be created:

```
$my_record = array("generation"=>5, "ttl"=>3600);
```

The `Aerospike::getHeader()` API return the object's record structure.

## Bin Structure

The value of an object in the database is comprised of one or more
fields, known as "bins", each with a value.  Two types of arrays are
used to access bins:  an array of one or more bin name strings, or an
array with elements that are the bin values indexed by bin name.

For example, to refer to the 2 bins "bin1" and "bin3", for example to
get their values, an array of the following form must be created:

```
$my_bins_names = array("bin1", "bin3");
```

After executing the following API call to get the values,

```
$rv = $as->get($my_key, $my_bin_names, &$my_bin_values);
```

an array of the following form would be returned (i.e., as if it were
created by the following):

```
$my_bin_values = array("bin1"=>"value1", "bin3"=>"value3");
```

## Bin Values

### Supported Types
PHP strings, integers and associative arrays should map directly to the
equivalent types on the database - string, integer, list, and map.

### Handling Unsupported Types
PHP values which do not have a directly mappable bin type need to be handled
in configurable way, whether by calling a specified serializer or by throwing an
explicit error.
