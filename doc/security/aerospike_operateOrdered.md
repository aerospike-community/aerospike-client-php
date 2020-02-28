
# Aerospike::operateOrdered

Aerospike::operateOrdered - multiple operations on a single record

## Description

```
public int Aerospike::operateOrdered ( array $key, array $operations [, array &$returned [, array $options]] )
```

**Aerospike::operateOrdered()** allows for multiple per-bin operations on a *record*
with a given *key*, with write operations happening before read ones.
Non-existent bins being read will have a NULL value.

Currently only a call to operateOrdered() can include only one write operation per-bin.
For example, you cannot both append and prepend to the same bin, in the same
call.

Like other bin operations, operateOrdered() only works on existing records
(i.e. ones that were previously created with a put()).

This api returns an indexed array of results where the aerospike::operate api returns a key-value associative array.

## Parameters

**key** the key identifying the record. An array with keys ['ns','set','key'] or ['ns','set','digest'].

**operations** an array of one or more per-bin operations conforming
to the following structure:
```
Write Operation:
  op => Aerospike::OPERATOR_WRITE
  bin => bin name (cannot be longer than 14 characters)
  val => the value to store in the bin

Increment Operation:
  op => Aerospike::OPERATOR_INCR
  bin => bin name
  val => the integer by which to increment the value in the bin

Prepend Operation:
  op => Aerospike::OPERATOR_PREPEND
  bin => bin name
  val => the string to prepend the string value in the bin

Append Operation:
  op => Aerospike::OPERATOR_APPEND
  bin => bin name
  val => the string to append the string value in the bin

Read Operation:
  op => Aerospike::OPERATOR_READ
  bin => name of the bin we want to read after any write operations

Touch Operation: reset the time-to-live of the record and increment its generation
                 (only combines with read operations)
  op => Aerospike::OPERATOR_TOUCH
  ttl => a positive integer value to set as time-to-live for the record

Delete Operation:
  op => Aerospike::OPERATOR_DELETE

List Append Operation:
  op => Aerospike::OP_LIST_APPEND,
  bin =>  "events",
  val =>  1234

List Merge Operation:
  op => Aerospike::OP_LIST_MERGE,
  bin =>  "events",
  val =>  [ 123, 456 ]

List Insert Operation:
  op => Aerospike::OP_LIST_INSERT,
  bin =>  "events",
  index =>  2,
  val =>  1234

List Insert Items Operation:
  op => Aerospike::OP_LIST_INSERT_ITEMS,
  bin =>  "events",
  index =>  2,
  val =>  [ 123, 456 ]

List Pop Operation:
  op => Aerospike::OP_LIST_POP, # returns a value
  bin =>  "events",
  index =>  2

List Pop Range Operation:
  op => Aerospike::OP_LIST_POP_RANGE, # returns a value
  bin =>  "events",
  index =>  2,
  val =>  3 # remove 3 elements starting at index 2

List Remove Operation:
  op => Aerospike::OP_LIST_REMOVE,
  bin =>  "events",
  index =>  2

List Remove Range Operation:
  op => Aerospike::OP_LIST_REMOVE_RANGE,
  bin =>  "events",
  index =>  2,
  val =>  3 # remove 3 elements starting at index 2

List Clear Operation:
  op => Aerospike::OP_LIST_CLEAR,
  bin =>  "events"

List Set Operation:
  op => Aerospike::OP_LIST_SET,
  bin =>  "events",
  index =>  2,
  val =>  "latest event at index 2" # set this value at index 2

List Get Operation:
  op => Aerospike::OP_LIST_GET, # returns a value
  bin =>  "events",
  index =>  2 # similar to Aerospike::OPERATOR_READ but only returns the value
                at index 2 of the list, not the whole bin

List Get Range Operation:
  op => Aerospike::OP_LIST_GET_RANGE, # returns a value
  bin =>  "events",
  index =>  2,
  val =>  3 # get 3 elements starting at index 2

List Trim Operation:
  op => Aerospike::OP_LIST_TRIM,
  bin =>  "events",
  index =>  2,
  val =>  3 # remove all elements not in the range between index 2 and index 2 + 3

List Size Operation:
  op => Aerospike::OP_LIST_SIZE, # returns a value
  bin =>  "events" # gets the size of a list contained in the bin

```
*examples:*
Combining several write operations into one multi-op call:
```
[
  ["op" => Aerospike::OPERATOR_APPEND, "bin" => "name", "val" => " Ph.D."],
  ["op" => Aerospike::OPERATOR_INCR, "bin" => "age", "val" => 1],
  ["op" => Aerospike::OPERATOR_READ, "bin" => "age"]
]
```
To implement an LRU you can read a bin and touch a record in the same
operation:
```
[
  ["op" => Aerospike::OPERATOR_READ, "bin" => "age"],
  ["op" => Aerospike::OPERATOR_TOUCH, "ttl" => 20]
]
```

**returned** an indexed array of bins retrieved by read operations. If multiple operations exist for a specific bin name, each operation will have an entry in the array.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**
- **Aerospike::OPT_TTL**
- **[Aerospike::OPT_POLICY_RETRY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9730980a8b0eda8ab936a48009a6718)**
- **[Aerospike::OPT_POLICY_KEY](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gaa9c8a79b2ab9d3812876c3ec5d1d50ec)**
- **[Aerospike::OPT_POLICY_GEN](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga38c1a40903e463e5d0af0141e8c64061)**
- **[Aerospike::OPT_POLICY_REPLICA](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gabce1fb468ee9cbfe54b7ab834cec79ab)**
- **[Aerospike::OPT_POLICY_READ_MODE_AP](https://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#gadeed264a3e2331fefff177eeb72b8a27)**
- **[Aerospike::OPT_POLICY_READ_MODE_SC](https://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga48f29009f4394c481306f788e7802624)**
- **[Aerospike::OPT_POLICY_COMMIT_LEVEL](http://www.aerospike.com/apidocs/c/db/d65/group__client__policies.html#ga17faf52aeb845998e14ba0f3745e8f23)**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = $client->initKey("test", "users", 1234);
$operations = [
  ["op" => Aerospike::OPERATOR_PREPEND, "bin" => "first_name", "val" => "Mr. "],
  ["op" => Aerospike::OPERATOR_READ, "bin" => "first_name"],
  ["op" => Aerospike::OPERATOR_APPEND, "bin" => "last_name", "val" => " Ph.D."],
  ["op" => Aerospike::OPERATOR_READ, "bin" => "last_name"],
  ["op" => Aerospike::OPERATOR_READ, "bin" => "age"],
];
$options = [Aerospike::OPT_TTL => 600];
$status = $client->operateOrdered($key, $operations, $returned, $options);
if ($status == Aerospike::OK) {
    var_dump($returned);
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(3) {
  [0]=> array(2) { [0]=> string(10) "first_name" [1]=> string(7) "Mr John" }
  [1]=> array(2) { [0]=> string(9) "last_name" [1]=> string(11) "Smith Ph.D." }
  [2]=> array(2) { [0]=> string(3) "age" [1]=> int(25) }
}
```

