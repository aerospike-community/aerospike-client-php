
# Aerospike::close

Aerospike::close - Close all connections to the Aerospike DB. If the connection is persistent, this will close the local client's connection, but leave the persistent connection open.

## Description

```
public Aerospike::close ( void )
```

**Aerospike::close()** will disconnect from the Aerospike DB cluster.

## Parameters

This method has no parameters.

## See Also

- [Aerospike::isConnected()](aerospike_isconnected.md)
- [Aerospike::reconnect()](aerospike_reconnect.md)

