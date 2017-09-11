
# Aerospike::listRegistered

Aerospike::listRegistered - lists the UDF modules registered with the server

## Description

```
public int Aerospike::listRegistered ( array &$modules [, int $language [, array $options ]] )
```

**Aerospike::listRegistered()** populates *modules* with the list of UDF modules
registered with the server.

## Parameters

**modules** filled by an array of module info.

**language** one of *Aerospike::UDF\_TYPE\_\**.  Optionally filters a subset of
modules matching the given type.

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

The modules array has multiples of the following structure:
```
Array of:
  name => module name
  type => Aerospike::UDF_TYPE_*
```

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$status = $client->listRegistered($modules);
if ($status == Aerospike::OK) {
    var_dump($modules);
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
array(3) {
  [0]=>
  array(2) {
    ["name"]=>
    string(13) "my_record_udf"
    ["type"]=>
    int(0)
  }
  [1]=>
  array(2) {
    ["name"]=>
    string(13) "my_stream_udf"
    ["type"]=>
    int(0)
  }
  [2]=>
  array(2) {
    ["name"]=>
    string(5) "utils"
    ["type"]=>
    int(0)
  }
}
```


