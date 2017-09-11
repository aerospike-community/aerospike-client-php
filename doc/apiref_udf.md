
# UDF Methods

### [Aerospike::register](aerospike_register.md)
```
public int Aerospike::register ( string $path, string $module [, int $language = Aerospike::UDF_TYPE_LUA  [, array $options ]] )
```

### [Aerospike::deregister](aerospike_deregister.md)
```
public int Aerospike::deregister ( string $module [, array $options ])
```

### [Aerospike::listRegistered](aerospike_listregistered.md)
```
public int Aerospike::listRegistered ( array &$modules [, int $language  [, array $options ]] )
```

### [Aerospike::getRegistered](aerospike_getregistered.md)
```
public int Aerospike::getRegistered ( string $module, string &$code [, int $language = Aerospike::UDF_TYPE_LUA [, array $options ]] )
```

### [Aerospike::apply](aerospike_apply.md)
```
public int Aerospike::apply ( array $key, string $module, string $function[, array $args [, mixed &$returned [, array $options]]] )
```

### [Aerospike::scanApply](aerospike_scanapply.md)
```
public int Aerospike::scanApply ( string $ns, string $set, string $module, string $function, array $args, int &$scan_id [, array $options ] )
```

### [Aerospike::queryApply](aerospike_queryapply.md)
```
public int Aerospike::queryApply ( string $ns, string $set, array $where, string $module, string $function, array $args, int &$job_id [, array $options ] )
```

### [Aerospike::jobInfo](aerospike_jobinfo.md)
```
public int Aerospike::jobInfo ( integer $job_id, array &$info [, array $options ] )
```

### [Aerospike::scanInfo](aerospike_scaninfo.md) (deprecated)
```
public int Aerospike::scanInfo ( integer $scan_id, array &$info [, array $options ] )
```

### [Aerospike::aggregate](aerospike_aggregate.md)
```
public int Aerospike::aggregate ( string $ns, string $set, array $where, string $module, string $function, array $args, mixed &$returned [, array $options ] )
```

## Example

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]], "shm"=>[]];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$key = ["ns" => "test", "set" => "users", "key" => 1234];
$bins = ["email" => "hey@example.com", "name" => "Hey There"];
// attempt to 'CREATE' a new record at the specified key
$status = $client->put($key, $bins, 0, [Aerospike::OPT_POLICY_EXISTS => Aerospike::POLICY_EXISTS_CREATE]);
if ($status == Aerospike::OK) {
    echo "Record written.\n";
} elseif ($status == Aerospike::ERR_RECORD_EXISTS) {
    echo "The Aerospike server already has a record with the given key.\n";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

// apply a UDF to a record
$status = $client->apply($key, 'my_udf', 'startswith', ['email', 'hey@'], $returned);
if ($status == Aerospike::OK) {
    if ($returned) {
        echo "The email of the user with key {$key['key']} starts with 'hey@'.\n";
    } else {
        echo "The email of the user with key {$key['key']} does not start with 'hey@'.\n";
    }
} elseif ($status == Aerospike::ERR_UDF_NOT_FOUND) {
    echo "The UDF module my_udf.lua was not registered with the Aerospike DB.\n";
}

// filtering for specific keys
$status = $client->get($key, $record, ["email"], Aerospike::POLICY_RETRY_ONCE);
if ($status == Aerospike::OK) {
    echo "The email for this user is ". $record['bins']['email']. "\n";
    echo "The name bin should be filtered out: ".var_export(is_null($record['bins']['name']), true). "\n";
}
?>
```
