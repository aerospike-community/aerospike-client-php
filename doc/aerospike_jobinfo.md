
# Aerospike::jobInfo

Aerospike::jobInfo - gets the status of a background job triggered by
scanApply() or queryApply()

## Description

```
public int Aerospike::jobInfo ( integer $job_id, string $module, array &$info [, array $options ] )
```

**Aerospike::jobInfo()** will return *information* on a background job, identified
by *job_id*, which was triggered using **Aerospike::scanApply()** or
**Aerospike::queryApply()**.

## Parameters

**job_id** the job id

**module** one of: ```Aerospike::JOB_QUERY``` or ```Aerospike::JOB_SCAN```

**info** the status of the background job returned as an array conforming to the following:
```
Associative Array:
  progress_pct => progress percentage for the job
  records_read => number of records read by the job
  status => one of Aerospike::JOB_STATUS_*
```

**[options](aerospike.md)** including
- **Aerospike::OPT_READ_TIMEOUT**

## Return Values

Returns an integer status code.  Compare to the Aerospike class status
constants.  When non-zero the **Aerospike::error()** and
**Aerospike::errorno()** methods can be used.

## See Also

- [Aerospike::scanApply()](aerospike_scanapply.md)
- [Aerospike::queryApply()](aerospike_queryapply.md)

## Examples

```php

// after a queryApply() where $job_id was set:
do {
    time_nanosleep(0, 30000000); // pause 30ms
    $status = $client->jobInfo($job_id, Aerospike::JOB_QUERY, $job_info);
    var_dump($job_info);
} while($job_info['status'] != Aerospike::JOB_STATUS_COMPLETED);
```

We expect to see:

```
array(3) {
  ["progress_pct"]=>
  int(70)
  ["records_read"]=>
  int(1000)
  ["status"]=>
  int(1)
}
```

