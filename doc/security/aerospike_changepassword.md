
# Aerospike::changePassword

Aerospike::changePassword - Changes the password of an existing user in a security-enabled Aerospike database

## Description

```
public int changePassword ( string $user, string $password [, array $options ] )
```

**Aerospike::changePassword()** will change the password of an existing user.
Clear-text password will be hashed using bcrypt before sending to server.

## Parameters

**user** string username.

**password** string new password.

**[options](aerospike.md)** including
- **Aerospike::OPT_WRITE_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## Example

```php
<?php

$config = ["hosts" => [["addr"=>"localhost", "port"=>3000]],
           "shm"=>[],
           "user"=>"admin", "pass"=>"admin"];
$client = new Aerospike($config, true);
if (!$client->isConnected()) {
   echo "Aerospike failed to connect[{$client->errorno()}]: {$client->error()}\n";
   exit(1);
}

$res = $client->changePassword("john", "john@123");
if ($res == Aerospike::OK) {
    echo "Password successfully changed";
} elseif ($res == Aerospike::ERR_ROLE_VIOLATION) {
    echo "User does not possess the required role to change password";
} elseif ($res == Aerospike::ERR_INVALID_USER) {
    echo "User john does not exist";
} else {
    echo "[{$client->errorno()}] ".$client->error();
}

?>
```

We expect to see:

```
User john successfully created
```

