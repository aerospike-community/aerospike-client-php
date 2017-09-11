
# Aerospike::dropIndex

Aerospike::dropIndex - drops a secondary index

## Description

```
public int Aerospike::dropIndex ( string $ns, string $name [, array $options ] )
```

**Aerospike::dropIndex()** will drop a secondary index from
a namespace with a specified index *name*.

## Parameters

**ns** the namespace

**name** the name of the index

**options** including
- **Aerospike::OPT_WRITE_TIMEOUT**

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

$status = $client->dropIndex("test", "user_email_idx");
if ($status == Aerospike::OK) {
    echo "Index user_email_idx was dropped from namespace 'test'\n";
} else if ($status == Aerospike::ERR_INDEX_NOT_FOUND) {
    echo "No such index exists.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
Index user_email_idx was dropped from namespace 'test'
```

