#!/bin/bash
################################################################################
# Copyright 2013-2017 Aerospike, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

CWD=`dirname $0`
AEROSPIKE_EXT_PATH=${CWD}/..
if [ ! -d $AEROSPIKE_EXT_PATH ]; then
    echo "Cannot find the extension source directory at ${AEROSPIKE_EXT_PATH}"
    exit 1
fi
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.out" -exec rm {} \;
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.diff" -exec rm {} \;
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.exp" -exec rm {} \;
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.log" -exec rm {} \;
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.php" -exec rm {} \;
find ${AEROSPIKE_EXT_PATH}/tests/ -name "*\.sh" -exec rm {} \;
