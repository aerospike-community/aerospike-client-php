
# Aerospike::setLogHandler

Aerospike::setLogHandler - sets a handler for log events

## Description

```
public Aerospike::setLogHandler ( callback $log_handler )
```

**Aerospike::setLogHandler()** registers a callback method that will be triggered
whenever a logging event above the declared [log threshold](aerospike_setloglevel.md) occurs.

The callback method must follow the signature
```
public function log_handler ( int $level, string $file, string $function, int $line )
```
with **level** matching one of the *Aerospike::LOG\_LEVEL\_\** values

## Parameters

**log_handler** a callback function invoked for each logging event above the threshold.

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
$client->setLogHandler(function ($level, $file, $function, $line) {
    switch ($level) {
        case Aerospike::LOG_LEVEL_ERROR:
            $lvl_str = 'ERROR';
            break;
        case Aerospike::LOG_LEVEL_WARN:
            $lvl_str = 'WARN';
            break;
        case Aerospike::LOG_LEVEL_INFO:
            $lvl_str = 'INFO';
            break;
        case Aerospike::LOG_LEVEL_DEBUG:
            $lvl_str = 'DEBUG';
            break;
        case Aerospike::LOG_LEVEL_TRACE:
            $lvl_str = 'TRACE';
            break;
        default:
            $lvl_str = '???';
    }
    error_log("[$lvl_str] in $function at $file:$line");
});

?>
```
