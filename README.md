# Aerospike PHP Client
[![Build Status](https://travis-ci.org/aerospike/aerospike-client-php.svg?branch=master)](https://travis-ci.org/aerospike/aerospike-client-php)
[![License](https://img.shields.io/packagist/l/aerospike/aerospike-client-php.svg)](https://img.shields.io/packagist/l/aerospike/aerospike-client-php.svg)

Note: This client supports PHP versions >= 7 . If you are looking for the Legacy client which supports PHP versions up through 5, it can be found at the [aerospike-client-php5 repo](https://github.com/aerospike/aerospike-client-php5)

## Differences from the previous Aerospike PHP Client:

- LDT Support has been removed.
- Type checking in general is stricter for method parameters. If you are not sure whether an argument to a function is an integer or a string, we recommend casting it to the type specified by the method. Running with strict_types enabled may help to catch some issues.
- An exception will be raised if the constructor fails to connect to the cluster.
- \Aerospike\Bytes will be stored to the server as type AS\_BYTES\_BLOB instead of AS\_BYTES\_PHP. This change allows better compatability with other clients.
- Correspondingly, data stored in the server as AS\_BYTES\_BLOB will be returned as Aerospike\Bytes,  if no deserializer has been registered. The Previous version of the Aerospike PHP Client returned a string if AS_BYTES_BLOB was encountered with no registered deserializer. **Note** It is not recommended to combine the use of \Aerospike\Bytes and user specified deserializers, as this may cause errors.
- Support for PHP versions < 7 has been removed.
- The INI entry `aerospike.serializer` now takes an integer value. 0 for No Serializer, 1 for default PHP serialization, and 2 for user specified serializer. See [Configuration](doc/aerospike_config.md) for additional information on the code values.
- The constructor will no longer attempt to create a unique SHM key for the user. If a key is not specified in the shm configuration array, the default value will be used. A key provided in the constructor takes precedence over a value specified by INI.
- The layout of the shared memory used by the client when using an SHM key has changed. The default key has changed as well in order to prevent accidental sharing between new and old clients.
- The formatting of the response from an info call may have changed. It now includes the request at the beginning of the response.
- When using initKey with a digest, the digest must now be exactly 20 bytes.
- The integer values of the `Aerospike::LOG_LEVEL_*` constants have changed. This should not effect the user unless they were providing log levels as integers rather than using the constants.
- `Aerospike::LOG_LEVEL_OFF` has been removed. It no longer had any effect.


## Documentation

Documentation of the Aerospike PHP Client may be found in the [doc directory](doc/README.md).
The API described there is the [specification](doc/aerospike.md) for the PHP Client.
Notes on the internals of the implementation are in [doc/internals.md](doc/internals.md).

[Example PHP code](examples/) can be found in the `examples/` directory.

Full documentation of the Aerospike database is available at http://www.aerospike.com/docs/

## Dependencies

### CentOS and RedHat (yum)

    sudo yum groupinstall "Development Tools"
    sudo yum install openssl-devel
    # You will need PHP7 development headers. If PHP7 was manually installed, these should be available
    # by default; Otherwise, you will need to fetch them from a repository, the package name may vary.
    sudo yum install php-pear # unless PHP was manually installed

### Ubuntu and Debian (apt)

    sudo apt-get install build-essential autoconf libssl-dev
    sudo apt-get install php7.0-dev php-pear # unless PHP was manually installed

### OS X

By default OS X will be missing command line tools. On Mavericks (OS X 10.9)
and higher those [can be installed without Xcode](http://osxdaily.com/2014/02/12/install-command-line-tools-mac-os-x/).

    xcode-select --install # install the command line tools, if missing

The dependencies can be installed through the OS X package manager [Homebrew](http://brew.sh/).

    brew update && brew doctor
    brew install automake
    brew install openssl

To switch PHP versions [see this gist](https://gist.github.com/rbotzer/198a04f2315e88c75322).

If you run into issues with linking to openssl during compile time, it may be necessary to edit
the ./build.sh file and add the path to your openssl files to the compile and link options.
e.g  `-I/usr/local/opt/openssl/include` `-L/usr/local/opt/openssl/lib`

Windows is currently not supported.

## Installation

### Building with Composer

Using [Composer](https://getcomposer.org/) you can download and build the PHP
extension:

    composer require aerospike/aerospike-client-php ~7.0
    find vendor/aerospike/aerospike-client-php/ -name "*.sh" -exec chmod +x {} \;
    cd vendor/aerospike/aerospike-client-php/ && sudo composer run-script post-install-cmd

### Building Manually

To build the PHP extension manually you will need to fetch the
[latest release](https://github.com/aerospike/aerospike-client-php/releases/latest)
from Github, then run the `build.sh` script in the `src/` directory.

    cd src
    ./build.sh

This will download the Aerospike C client SDK if necessary into
`src/../aerospike-client-c/`, and initiate `make`.

## Installing the PHP Extension

To install the PHP extension do:

    make install
    php -i | grep ".ini "

Now edit the php.ini file.  If PHP is configured --with-config-file-scan-dir
(usually set to `/etc/php.d/`) you can create an `aerospike.ini` file in the
directory, otherwise edit `php.ini` directly. Add the following directive:

    extension=aerospike.so
    aerospike.udf.lua_system_path=/path/to/aerospike/lua
    aerospike.udf.lua_user_path=/path/to/aerospike/usr-lua

The *aerospike* module should now be available to the PHP CLI:

    php -m | grep aerospike
    aerospike

Remember that if you are using PHP with Nginx or Apache there is likely a
separate `php.ini` config file for the web server  Copy the `aerospike.ini`
you have just created into `/etc/php7/apache2/conf.d/`, `/etc/php7/fpm/conf.d/`
or wherever the configuration include directory of the web server is, then issue
a graceful restart.

## License

The Aerospike PHP Client is made availabled under the terms of
the Apache License, Version 2, as stated in the file [LICENSE](./LICENSE).

Individual files may be made available under their own specific license,
all compatible with Apache License, Version 2. Please see individual files for
details.


