
# Aerospike::setSerializer

Aerospike::setSerializer - sets a serialization handler for unsupported types

## Description

```
public static Aerospike::setSerializer ( callback $serialize_cb )
```

**Aerospike::setSerializer()** registers a callback method that will be triggered
whenever a write method handles a value whose type is unsupported.
This is a static method and the *serialize_cb* handler is global across all
instances of the Aerospike class.

The callback method must follow the signature
```
public function string aerospike_serialize ( mixed $value )
```

The value returned from the callback must be a string, and will be cast to an
as\_bytes (AS\_BYTES\_BLOB) bin type.

## Parameters

**serialize_cb** is a callback function invoked for each value of an unsupported type.

## Examples

```php
<?php

Aerospike::setSerializer(function ($val) {
    return gzcompress(json_encode($val));
});

?>
```

## See Also
 - [Aerospike::setDeserializer()](aerospike_setdeserializer.md)
