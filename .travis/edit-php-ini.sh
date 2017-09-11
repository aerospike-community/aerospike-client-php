#!/bin/bash

home=`echo ~`
echo "extension = aerospike.so" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
echo "aerospike.udf.lua_system_path=$home/udf/sys" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
echo "aerospike.udf.lua_user_path=$home/udf/usr" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
$(phpenv which php) -m | grep aerospike
if [ $? -gt 0 ] ; then
    echo "The aerospike extension failed to load for $(phpenv version-name)"
    exit 1
fi
