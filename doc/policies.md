# Policies

## Options for every policy type

* `Aerospike::OPT_SOCKET_TIMEOUT`

    Socket idle timeout in milliseconds when processing a database command.

    If `OPT_SOCKET_TIMEOUT` is not zero and the socket has been idle for at least `OPT_SOCKET_TIMEOUT`,
    both `OPT_MAX_RETRIES` and `OPT_TOTAL_TIMEOUT` are checked.  If `OPT_MAX_RETRIES` and `OPT_TOTAL_TIMEOUT` are not
    exceeded, the transaction is retried.

    If both `OPT_SOCKET_TIMEOUT` and `OPT_TOTAL_TIMEOUT` are non-zero and `OPT_SOCKET_TIMEOUT > OPT_TOTAL_TIMEOUT`,
    then `OPT_SOCKET_TIMEOUT` will be set to `OPT_TOTAL_TIMEOUT`.  If `OPT_SOCKET_TIMEOUT` is zero, there will be
    no socket idle limit.

* `Aerospike::OPT_TOTAL_TIMEOUT`
    Total transaction timeout in milliseconds.

    The `OPT_TOTAL_TIMEOUT` is tracked on the client and sent to the server along with
    the transaction in the wire protocol.  The client will most likely timeout
    first, but the server also has the capability to timeout the transaction.

    If `OPT_TOTAL_TIMEOUT` is not zero and `OPT_TOTAL_TIMEOUT` is reached before the transaction
    completes, the transaction will return error `ERR_TIMEOUT`.
    If `OPT_TOTAL_TIMEOUT` is zero, there will be no total time limit.

* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES`

    Milliseconds to sleep between retries.  Enter zero to skip sleep.
    This field is ignored when `OPT_MAX_RETRIES` is zero.  

    Reads do not have to sleep when a node goes down because the cluster
    does not shut out reads during cluster reformation.  The default for
    reads is zero.

    The default for writes is also zero because writes are not retried by default.
    Writes need to wait for the cluster to reform when a node goes down.
    Immediate write retries on node failure have been shown to consistently
    result in errors.  If `OPT_MAX_RETRIES` is greater than zero on a write, then
    `OPT_SLEEP_BETWEEN_RETRIES` should be set high enough to allow the cluster to
    reform (>= 500ms).

* `Aerospike::OPT_MAX_RETRIES`

    Maximum number of retries before aborting the current transaction.
    The initial attempt is not counted as a retry.

    If `OPT_MAX_RETRIES` is exceeded, the transaction will return error `ERR_TIMEOUT`.

    WARNING: Database writes that are not idempotent (such as "add")
    should not be retried because the write operation may be performed
    multiple times if the client timed out previous transaction attempts.
    It's important to use a distinct write policy for non-idempotent
    writes which sets `OPT_MAX_RETRIES` = 0;

## Write Policies

constructor key: `OPT_WRITE_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_POLICY_KEY` default: `Aerospike::POLICY_KEY_DIGEST`
* `Aerospike::OPT_POLICY_EXISTS` default: `POLICY_EXISTS_IGNORE`
* `Aerospike::OPT_POLICY_DURABLE_DELETE` default: `false`
* `Aerospike::OPT_POLICY_COMMIT_LEVEL` default: `Aerospike::POLICY_COMMIT_LEVEL_ALL`
* `Aerospike::OPT_POLICY_REPLICA` default: `Aerospike::POLICY_REPLICA_SEQUENCE`
* `Aerospike::COMPRESSION_THRESHOLD` default: 0

## Read Policies

constructor key: `OPT_READ_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `2`
* `Aerospike::OPT_POLICY_KEY` default: `Aerospike::POLICY_KEY_DIGEST`
* `Aerospike::OPT_POLICY_READ_MODE_AP` default: `Aerospike::POLICY_READ_MODE_AP_ONE`
* `Aerospike::OPT_POLICY_READ_MODE_SC` default: `Aerospike::POLICY_READ_MODE_SC_SESSION`
* `Aerospike::OPT_DESERIALIZE` default: `true`

## Remove Policies

constructor key: `OPT_REMOVE_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_POLICY_KEY` default: `Aerospike::POLICY_KEY_DIGEST`
* `Aerospike::OPT_POLICY_DURABLE_DELETE` default: `false`
* `Aerospike::OPT_POLICY_COMMIT_LEVEL` default: `Aerospike::POLICY_COMMIT_LEVEL_ALL`
* `Aerospike::OPT_POLICY_REPLICA` default: `Aerospike::POLICY_REPLICA_SEQUENCE`

## Batch Policies

constructor key: `OPT_BATCH_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `2`
* `Aerospike::OPT_POLICY_READ_MODE_AP` default: `Aerospike::POLICY_READ_MODE_AP_ONE`
* `Aerospike::OPT_POLICY_READ_MODE_SC` default: `Aerospike::POLICY_READ_MODE_SC_SESSION`
* `Aerospike::OPT_DESERIALIZE` default: `true`
* `Aerospike::OPT_SEND_SET_NAME` default: `false`
* `Aerospike::OPT_BATCH_CONCURRENT` default: `false`
* `Aerospike::OPT_ALLOW_INLINE` default: `true`

## OPERATE Policies

constructor key: `OPT_OPERATE_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_POLICY_KEY` default: `Aerospike::POLICY_KEY_DIGEST`
* `Aerospike::OPT_POLICY_EXISTS` default: `POLICY_EXISTS_IGNORE`
* `Aerospike::OPT_POLICY_DURABLE_DELETE` default: `false`
* `Aerospike::OPT_POLICY_COMMIT_LEVEL` default: `Aerospike::POLICY_COMMIT_LEVEL_ALL`
* `Aerospike::OPT_DESERIALIZE` default: `true`
* `Aerospike::OPT_POLICY_READ_MODE_AP` default: `Aerospike::POLICY_READ_MODE_AP_ONE`
* `Aerospike::OPT_POLICY_READ_MODE_SC` default: `Aerospike::POLICY_READ_MODE_SC_SESSION`
* `Aerospike::OPT_POLICY_REPLICA` default: `Aerospike::POLICY_REPLICA_SEQUENCE`

## Query Policies

constructor key: `OPT_QUERY_DEFAULT_POL`
Keys:

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `0`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_DESERIALIZE` default: `true`

## Scan Policies

constructor key: `OPT_SCAN_DEFAULT_POL`

### Scan Policy Options and keys

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `0`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_POLICY_DURABLE_DELETE` default: `false`
* `Aerospike::OPT_FAIL_ON_CLUSTER_CHANGE` default: `false`

## Apply Policies

constructor key: `OPT_APPLY_DEFAULT_POL`

### Scan policy options and keys

* `Aerospike::OPT_SOCKET_TIMEOUT` default: `30000`
* `Aerospike::OPT_TOTAL_TIMEOUT` default: `1000`
* `Aerospike::OPT_SLEEP_BETWEEN_RETRIES` default: `0`
* `Aerospike::OPT_MAX_RETRIES` default: `0`
* `Aerospike::OPT_POLICY_COMMIT_LEVEL` default: `Aerospike::POLICY_COMMIT_LEVEL_ALL`
* `Aerospike::OPT_POLICY_DURABLE_DELETE` default: `false`
* `Aerospike::OPT_POLICY_KEY` default: `Aerospike::POLICY_KEY_DIGEST`
* `Aerospike::OPT_POLICY_REPLICA` default: `Aerospike::POLICY_REPLICA_SEQUENCE`
