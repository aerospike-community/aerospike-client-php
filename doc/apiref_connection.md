
# Lifecycle and Connection Methods

### [Aerospike::__construct](aerospike_construct.md)
```
public int Aerospike::__construct ( array $config [, boolean $persistent_connection = true [, array $options]] )
```

### [Aerospike::__destruct](aerospike_destruct.md)
```
public Aerospike::__destruct ( void )
```

### [Aerospike::isConnected](aerospike_isconnected.md)
```
public boolean Aerospike::isConnected ( void )
```

### [Aerospike::shmKey](aerospike_shmkey.md)
```
public int|null Aerospike::shmKey ( void )
```

### [Aerospike::close](aerospike_close.md)
```
public Aerospike::close ( void )
```

### [Aerospike::reconnect](aerospike_reconnect.md)
```
public Aerospike::reconnect ( void )
```

## Example

```php
<?php

$config = ["hosts" => [["addr"=>"192.168.1.10", "port"=>3000]]];
$client = new Aerospike($config, false);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$client->close();
$client->reconnect();
$client->__destruct();

?>
```
