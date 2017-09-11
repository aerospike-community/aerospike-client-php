
# Aerospike::setLogLevel

Aerospike::setLogLevel - set the logging threshold of the Aerospike object

## Description

```
public Aerospike::setLogLevel ( int $log_level )
```

**Aerospike::setLogLevel()** declares a logging threshold for the Aerospike C client.

## Parameters

**log_level** one of `Aerospike::LOG_LEVEL_*` values

Possible values are:
- `Aerospike::LOG_LEVEL_ERROR`
- `Aerospike::LOG_LEVEL_WARN`
- `Aerospike::LOG_LEVEL_INFO`
- `Aerospike::LOG_LEVEL_DEBUG`
- `Aerospike::LOG_LEVEL_TRACE`

## Examples

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}
$client->setLogLevel(Aerospike::LOG_LEVEL_DEBUG);

?>
```
