
# Aerospike::error

Aerospike::error - display an error message associated with the last operation

## Description

```
public string Aerospike::error ( void )
```

**Aerospike::error()** will return an error message associated with the last
operation. If the operation was successful the return value should be an empty
string ''.

Use any explicit error message returned from the server or C client, otherwise
attempt to map the error code to a static error string.

## Return Values

Returns an error string, which may be empty on success.

## Examples

### Example #1 Aerospike::error()

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

?>
```

On error we expect to see:

```
Aerospike failed to connect[300]: failed to initialize cluster
```

