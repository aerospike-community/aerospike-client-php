// *****************************************************************************
// Copyright 2017 Aerospike, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// *****************************************************************************

/*constants.h*/
#pragma once
#ifndef AEROSPIKE_PHP_REGISTER_CONSTANTS_H
#define AEROSPIKE_PHP_REGISTER_CONSTANTS_H
#include "php.h"
#include <stdbool.h>
/*
 *
 * Copyright (C) 2014-2017 Aerospike, Inc.
 *
 * Portions may be licensed to Aerospike, Inc. under one or more contributor
 * license agreements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */


#define MAX_STATUS_MSG_SIZE 512
#define MAX_GENERAL_CONSTANT_STR_SIZE 512

bool register_aerospike_class_constants(zend_class_entry* aerospike_ce);
bool register_aerospike_status_codes(zend_class_entry* aerospike_ce);
bool register_aerospike_index_constants(zend_class_entry* aerospike_ce);


#endif /* end of __AEROSPIKE_STATUS_H__*/
