# Aerospike PHP Client PHPT Tests

## Build Instructions

Follow the build and installation steps described in this repository's main
[README.md](../../../README.md) file. Certian tests will be skipped if running in
a ZTS PHP environment.

## Configuration:

Use the config file skeleton to create a local configuration file for the tests,
and configure the correct IP address and port for your server.

```
cd src/
cp tests/aerospike.local.skeleton tests/aerospike.local.inc
# edit tests/aerospike.local.inc
```

## Running Tests:
To run the tests with PHPUnit, from the `src/` directory
`phpunit tests`

## Cleanup

To clean up artifacts created by the tests you can run:

```
./scripts/test-cleanup.sh
```

## Source Code

PHPT scripts are grouped by functionality inside `src/tests/`.
Each *.inc* file contains the methods used by individual *.phpt* within
the respective `src/tests/phpt/*/` subdirectory.

For example, the Put test cases are described by the list of
`src/tests/phpt/Put/*.phpt` files, which make use of methods from `src/tests/Put.inc`.

## Expected Values
The structure of PHPT tests is explained at the
[PHP QA site](http://qa.php.net/write-test.php#writing-phpt).

The expected value for a test is the name of one of the
[Aerospike class](../../../doc/aerospike.md) constants, such as **OK**
or **ERR_RECORD_NOT_FOUND**.
