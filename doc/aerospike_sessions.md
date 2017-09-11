# Aerospike Session Handler

The Aerospike PHP client provides a custom session handler that can be used to
store user sessions in Aerospike. A set and namespace must be given to act as
the container for the sessions.

The session ID is used as the primary key of the record containing the
session. The key-value pairs in the $_SESSION object are stored in the matching
record. The value of *session.gc_maxlifetime* is used as the record ttl.


**session.save\_handler string**
    Set to _aerospike_ to enable sessions support.

**session.save\_path string**
    A string formatted as **ns|set|addr:port\[,addr:port\[,...\]\]**. for example "test|sess|127.0.0.1:3000"
    As with the *$config* of the constructor, the host info of just one cluster node is necessary.
