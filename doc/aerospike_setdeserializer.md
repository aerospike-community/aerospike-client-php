
# Aerospike::setDeserializer

Aerospike::setDeserializer - sets a deserialization handler for unsupported types

## Description

```
public static Aerospike::setDeserializer ( callback $unserialize_cb )
```

**Aerospike::setDeserializer()** registers a callback method that will be triggered
whenever a read method handles a value whose type is unsupported.
This is a static method and the *unserialize_cb* handler is global across all
instances of the Aerospike class.

The callback method must follow the signature
```
public function string aerospike_deserialize ( mixed $value )
```

The value passed to it is an as\_bytes (AS\_BYTES\_BLOB).

## Parameters

**unserialize_cb** is a callback function invoked for each value of an unsupported type.

## Examples

```php
<?php

Aerospike::setDeserializer(function ($val) {
    return json_decode(gzuncompress($val));
});

?>
```

## See Also
 - [Aerospike::setSerializer()](aerospike_setserializer.md)
