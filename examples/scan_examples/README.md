# Scan Method Examples

### Simple Scan
`standard.php` shows how a callback method is invoked for each
record streamed from the server as it is scans the _users_ set of the _test_
namespace.

```bash
php standard.php --host=192.168.119.3 -a -c
```

### Buffering Results With a Limit
`buffered.php` shows how records streaming back from a scan are
buffered into a _$result_ array, with the scan halted by returning `false` once
a predetermined limit is reached.

```bash
php buffered.php --host=192.168.119.3 -a -c
```

### Scan Applying a UDF in the Background
`background.php` shows how a UDF is applied to each record of a background scan
of a set _users_ set in namespace _test_, transforming the value in the _age_ bin
of those records.

After checking the scan info to see whether it has completed, the transformed
records are displayed.

```bash
php background.php --host=192.168.119.3 -a -c
```
