# Query Method Examples

### Query Example
`simple.php` shows how a callback method is invoked for each
record streamed from the server as it queries the _characters_ set in the _test_
namespace. Includes an example of creating of a secondary index on a bin of
this set.

```bash
php simple.php --host=192.168.119.3 -a -c
```

### Applying a Stream UDF to the Results of a Query
`aggregate.php` shows how an aggregation similar to the SQL COUNT(\*) GROUP BY
_field_ is applied to the records streaming out of a secondary index query.

```bash
php aggregate.php --host=192.168.119.3 -a -c
```
