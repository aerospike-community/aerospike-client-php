
# Aerospike::truncate

Aerospike::truncate - touch a record in the Aerospike DB

## Description

```
public int Aerospike::truncate ( string $ns, string $set, int $nanos [, array $policy ] )
```

**Aerospike::truncate()** Remove records in specified namespace/set efficiently. This method is many orders of magnitude faster than deleting records one at a time. Works with Aerospike Server versions >= 3.12.

This asynchronous server call may return before the truncation is complete. The user can still write new records after the server returns because new records will have last update times greater than the truncate cutoff (set at the time of truncate call)

## Parameters

**ns** The namespace to perform truncation on.

**set** The set to perform truncation on. To perform truncation on the entire namespace: set this to null.

**nanos** A cutoff threshold indicating that records last updated before the threshold will be removed. Units are in nanoseconds since unix epoch (1970-01-01). A value of 0 indicates that all records in the set should be truncated regardless of update time. The value must not be in the future.

**[policy](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**

## Return Value

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$secondsInDay = 24 * 60 * 60;

/* Multiply by 10 ^ 9 to get nanoseconds */
$yesterday = 1000000000 * (time - $secondsInDay);

/* Remove all records in test/truncateSet updated before 24 hours ago */
$status = $client->truncate("test", "truncateSet", $yesterday);

/* Truncate all records in test, regardless of update time */
$status = $client->truncate("test", null, 0);

?>
```
