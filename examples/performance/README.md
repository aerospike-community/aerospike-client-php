# Performance Scripts

## Single Process
The read, write, and read-write-mix scripts do basic clocking of single process
performance, one client talking to one cluster.

### Write Performance
`write.php` writes n times to a record with key ("test", "performance", "write")

```bash
php write.php --host=192.168.119.3 --num-ops=100000
```

### Read Performance
`read.php` reads n times from a record with key ("test", "performance", "Write")

```bash
php read.php --host=192.168.119.3 --num-ops=100000
```

### Read-Write Performance
`read-write-mix.php` measures combined writes and reads with a given ratio.

For example a test of 250k read-write operations with a 9:1 ratio:

```bash
php read-write-mix.php --host=192.168.119.3 --num-ops=250000 --write-every=10
```

## Multi-Process
A more realistic performance test is given by the `rw-concurrent.sh` shell script
which launches n concurrent `rw-worker.php` scripts, waits on them to finish and
calls the `rw-summary.php` script to combine and display the results those
workers logged to a given log file.

For example 8 concurrent workers each doing 50000 transactions with a 9:1
read-write ratio:

```bash
chmod +x rw-concurrent.sh
./rw-concurrent.sh -h 192.168.119.3 -c 4 -n 50000 -w 10 run.log
```
