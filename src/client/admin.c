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

#include "aerospike_class.h"
#include "conversions.h"
#include "php_aerospike_types.h"
#include "policy_conversions.h"

/* proto  int Aerospike::createRole ( string $role, array $privileges [, array $options ] ) {{{ */
PHP_METHOD(Aerospike, createRole) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;

	char* rolename = NULL;
	size_t role_len;

	HashTable* z_priv_hash = NULL;
	zval* z_policy = NULL;

	as_privilege** privileges = NULL;
	int privilege_count = 0;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sh|z",
			&rolename, &role_len, &z_priv_hash, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid Parameters for createUser");
		goto CLEANUP;
	}

	if (!role_len) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Role must not be empty");
		goto CLEANUP;
	}

	if (role_len >= AS_ROLE_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Role name is too long");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK){
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid_policy");
			goto CLEANUP;
		}
	}

	privilege_count = zend_hash_num_elements(z_priv_hash);
	if (privilege_count) {
		privileges = (as_privilege**)calloc(privilege_count, sizeof(as_privilege*));
		if (!privileges) {
			as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to allocate memory for privileges");
			goto CLEANUP;
		}
		if (z_hashtable_to_as_privileges(z_priv_hash, privileges, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}

	aerospike_create_role(as_client, &err, admin_policy_p, rolename, privileges, privilege_count);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (privileges) {
		for (int i = 0; i < privilege_count; i++) {
			if (privileges[i]) {
				free(privileges[i]);
			}
		}
		free(privileges);
	}
	RETURN_LONG(err.code);
}
/* }}} */

/* proto int queryRole ( string $role, array &$privileges [, array $options ] ) {{{*/
PHP_METHOD(Aerospike, queryRole) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;

	char* rolename = NULL;
	size_t role_len;
	zval* z_policy = NULL;
	zval* z_privileges = NULL;

	as_role* role = NULL;

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	reset_client_error(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/|z",
			&rolename, &role_len, &z_privileges, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid arguments to queryRole");
		goto CLEANUP;
	}
	zval_dtor(z_privileges);
	ZVAL_NULL(z_privileges);

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy to queryRole");
			goto CLEANUP;
		}
	}


	if (aerospike_query_role(as_client, &err, admin_policy_p, rolename, &role) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	as_role_to_zval(role, z_privileges, &err);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		if (z_privileges) {
			ZVAL_NULL(z_privileges);
		}
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (role) {
		as_role_destroy(role);
	}

	RETURN_LONG(err.code);
}
/* }}} */

/* proto int queryRoles (array &$privileges [, array $options ] ) {{{*/
PHP_METHOD(Aerospike, queryRoles) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;

	zval* z_policy = NULL;
	zval* z_roles = NULL;

	as_role** roles = NULL;
	int roles_size;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z/|z", &z_roles, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters for queryRoles");
		goto CLEANUP;
	}
	zval_dtor(z_roles);
	ZVAL_NULL(z_roles);

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for queryRoles");
			goto CLEANUP;
		}
	}

	if (aerospike_query_roles(as_client, &err, admin_policy_p, &roles, &roles_size) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	as_roles_to_zval(roles, roles_size, z_roles, &err);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
		if (z_roles) {
			zval_dtor(z_roles);
			ZVAL_NULL(z_roles);
		}
	}
	if (roles) {
		as_roles_destroy(roles, roles_size);
	}
	RETURN_LONG(err.code);
}
/* }}} */

/* proto int int dropRole ( string $role [, array $options ] ) {{{ */
PHP_METHOD(Aerospike, dropRole) {

	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;

	char* role = NULL;
	size_t role_len;
	zval* z_policy = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|z", &role, &role_len, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters for dropRole");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for dropRole");
			goto CLEANUP;
		}
	}

	aerospike_drop_role(as_client, &err, admin_policy_p, role);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}
/* }}} */

PHP_METHOD(Aerospike, createUser) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	char* pass = NULL;
	size_t user_len;
	size_t pass_len;
	HashTable* z_roles;
	char** roles = NULL;
	int roles_size;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssh|z",
			&user, &user_len, &pass, &pass_len,
			&z_roles, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters for createUser");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for createUser");
			goto CLEANUP;
		}
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name is too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (pass_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "password must not be empty");
		goto CLEANUP;
	}

	roles_size = zend_hash_num_elements(z_roles);

	if (roles_size > 0) {
		roles = (char**)calloc(roles_size, sizeof(char*));
		if (!roles) {
			as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Not enough memory to call createUser");
			goto CLEANUP;
		}

		if (z_hash_to_str_array(z_roles, roles, AS_ROLE_SIZE, roles_size, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	}
	aerospike_create_user(as_client, &err, admin_policy_p, user, pass, (const char**)roles, roles_size);

CLEANUP:
	if (roles) {
		for (int i = 0; i < roles_size; i++) {
			if (roles[i]) {
				free(roles[i]);
			}
		}
	}

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}

/* proto public int grantPrivileges ( string $role, array $privileges [, array $options ] ) {{{ */
PHP_METHOD(Aerospike, grantPrivileges) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* role = NULL;
	size_t role_len;
	HashTable* z_privs = NULL;
	as_privilege** privileges = NULL;
	int priv_size;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		as_error_update(&err, err.code, err.message);
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sh|z",
			&role, &role_len, &z_privs, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters for grantPrivileges");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for grantPrivileges");
			goto CLEANUP;
		}
	}

	priv_size = zend_hash_num_elements(z_privs);
	if (priv_size > 0) {
		privileges = (as_privilege**)calloc(priv_size, sizeof(as_privilege*));
		if (!privileges) {
			as_error_update(&err, AEROSPIKE_ERR_CLIENT, "No memory to create privileges");
			goto CLEANUP;
		}
		if (z_hashtable_to_as_privileges(z_privs, privileges, &err) != AEROSPIKE_OK) {
			goto CLEANUP;
		}
	} else {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Privileges must not be empty");
		goto CLEANUP;
	}

	aerospike_grant_privileges(as_client, &err, admin_policy_p, role, privileges, priv_size);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (privileges) {
		for (int i = 0; i < priv_size; i++) {
			if (privileges[i]) {
				free(privileges[i]);
			}
		}
		free(privileges);
	}
	RETURN_LONG(err.code);
}

/* }}} */

PHP_METHOD(Aerospike, revokePrivileges) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* role = NULL;
	size_t role_len;
	HashTable* z_privs = NULL;
	as_privilege** privileges = NULL;
	int priv_size;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		as_error_update(&err, err.code, err.message);
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sh|z",
			&role, &role_len, &z_privs, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters for revokePrivileges");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for revokePrivileges");
			goto CLEANUP;
		}
	}

	priv_size = zend_hash_num_elements(z_privs);

	if (priv_size == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Privileges must not be empty");
		goto CLEANUP;
	}

	privileges = (as_privilege**)calloc(priv_size, sizeof(as_privilege*));
	if (!privileges) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to allocate memory in grantPrivileges");
		goto CLEANUP;
	}
	if (z_hashtable_to_as_privileges(z_privs, privileges, &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	aerospike_revoke_privileges(as_client, &err, admin_policy_p, role, privileges, priv_size);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	if (privileges) {
		for (int i = 0; i < priv_size; i++) {
			if (privileges[i]) {
				free(privileges[i]);
			}
		}
		free(privileges);
	}

	RETURN_LONG(err.code);
}

/* public int changePassword ( string $user, string $password [, array $options ] ) */
PHP_METHOD(Aerospike, changePassword) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	char* pass = NULL;
	size_t user_len;
	size_t pass_len;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		as_error_update(&err, err.code, err.message);
		RETURN_LONG(err.code);
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|z",
			&user, &user_len, &pass, &pass_len, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to changePassword");
		goto CLEANUP;
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "username is too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (pass_len == 0 ) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for changePassword");
			goto CLEANUP;
		}
	}

	aerospike_change_password(as_client, &err, admin_policy_p, user, pass);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}

/* public int setPassword ( string $user, string $password [, array $options ] ) */
PHP_METHOD(Aerospike, setPassword) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	char* pass = NULL;
	size_t user_len;
	size_t pass_len;


	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	reset_client_error(getThis());
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|z",
			&user, &user_len, &pass, &pass_len, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to changePassword");
		goto CLEANUP;
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "username is too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (pass_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for changePassword");
			goto CLEANUP;
		}
	}

	aerospike_set_password(as_client, &err, admin_policy_p, user, pass);

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}

PHP_METHOD(Aerospike, grantRoles) {

	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	size_t user_len;
	HashTable* z_roles = NULL;

	int roles_size;
	char** roles = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sh|z",
			&user, &user_len, &z_roles, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to grantRoles");
		goto CLEANUP;
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "username too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for grantRoles");
			goto CLEANUP;
		}
	}

	roles_size = zend_hash_num_elements(z_roles);

	if (roles_size == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for grantRoles");
		goto CLEANUP;
	}

	roles = (char**)calloc(roles_size, sizeof(char*));
	if (!roles) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to allocate memory for roles");
		goto CLEANUP;
	}

	if (z_hash_to_str_array(z_roles, roles, AS_ROLE_SIZE, roles_size, &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	aerospike_grant_roles(as_client, &err, admin_policy_p, user, (const char**)roles, roles_size);

CLEANUP:

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	if (roles) {
		for (int i = 0; i < roles_size; i++) {
			if (roles[i]) {
				free(roles[i]);
			}
		}
		free(roles);
	}

	RETURN_LONG(err.code);
}

PHP_METHOD(Aerospike, revokeRoles) {

	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	size_t user_len;
	HashTable* z_roles = NULL;

	int roles_size;
	char** roles = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sh|z",
			&user, &user_len, &z_roles, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to grantRoles");
		goto CLEANUP;
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "username too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy, &admin_policy_p,
				&as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for grantRoles");
			goto CLEANUP;
		}
	}

	roles_size = zend_hash_num_elements(z_roles);

	if (roles_size == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for grantRoles");
		goto CLEANUP;
	}

	roles = (char**)calloc(roles_size, sizeof(char*));
	if (!roles) {
		as_error_update(&err, AEROSPIKE_ERR_CLIENT, "Unable to allocate memory for roles");
		goto CLEANUP;
	}

	if (z_hash_to_str_array(z_roles, roles, AS_ROLE_SIZE, roles_size, &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	aerospike_revoke_roles(as_client, &err, admin_policy_p, user, (const char**)roles, roles_size);

CLEANUP:

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	if (roles) {
		for (int i = 0; i < roles_size; i++) {
			if (roles[i]) {
				free(roles[i]);
			}
		}
		free(roles);
	}

	RETURN_LONG(err.code);
}

PHP_METHOD(Aerospike, queryUser) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* username = NULL;
	size_t user_len;
	zval* return_roles = NULL;

	as_user* user = NULL;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/|z",
			&username, &user_len, &return_roles, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to queryUser");
		goto CLEANUP;
	}
	zval_dtor(return_roles);
	ZVAL_NULL(return_roles);

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Username too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for query User");
			goto CLEANUP;
		}
	}

	aerospike_query_user(as_client, &err, admin_policy_p, username, &user);

	if (err.code != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	array_init(return_roles);
	/* Not necessarily an error if this happened, but not fixable */
	if (!user) {
		goto CLEANUP;
	}

	for (int i = 0; i < user->roles_size; i++) {
		add_next_index_string(return_roles, user->roles[i]);
	}

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (user) {
		as_user_destroy(user);
	}

	RETURN_LONG(err.code);
}

PHP_METHOD(Aerospike, queryUsers) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	zval user_entry;
	zval* return_users = NULL;

	as_user** users = NULL;
	int users_size;


	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z/|z",
			&return_users, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to queryUsers");
		goto CLEANUP;
	}
	zval_dtor(return_users);
	ZVAL_NULL(return_users);

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for query User");
			goto CLEANUP;
		}
	}

	aerospike_query_users(as_client, &err, admin_policy_p, &users, &users_size);

	if (err.code != AEROSPIKE_OK) {
		goto CLEANUP;
	}

	array_init(return_users);

	if (!users) {
		goto CLEANUP;
	}

	/* create array of form ["name1"=>["role1", "role2"], ...] */
	for (int i = 0; i < users_size; i++) {
		array_init(&user_entry);
		for (int j = 0; j < users[i]->roles_size; j++) {
			add_next_index_string(&user_entry, users[i]->roles[j]);
		}
		add_assoc_zval(return_users, users[i]->name, &user_entry);
	}

CLEANUP:
	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}
	if (users) {
		as_users_destroy(users, users_size);
	}

	RETURN_LONG(err.code);
}

PHP_METHOD(Aerospike, dropUser) {
	as_error err;
	as_error_init(&err);
	AerospikeClient* php_client = NULL;
	aerospike* as_client = NULL;

	as_policy_admin admin_policy;
	as_policy_admin* admin_policy_p = NULL;
	zval* z_policy = NULL;

	char* user = NULL;
	size_t user_len;

	reset_client_error(getThis());

	if (check_object_and_connection(getThis(), &err) != AEROSPIKE_OK) {
		goto CLEANUP;
	}
	php_client = get_aerospike_from_zobj(Z_OBJ_P(getThis()));
	as_client = php_client->as_client;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|z",
			&user, &user_len, &z_policy) == FAILURE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid parameters to dropUser");
		goto CLEANUP;
	}

	if (user_len >= AS_USER_SIZE) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "username too long");
		goto CLEANUP;
	}

	if (user_len == 0) {
		as_error_update(&err, AEROSPIKE_ERR_PARAM, "user name must not be empty");
		goto CLEANUP;
	}

	if (z_policy) {
		if (zval_to_as_policy_admin(z_policy, &admin_policy,
				&admin_policy_p, &as_client->config.policies.admin) != AEROSPIKE_OK) {
			as_error_update(&err, AEROSPIKE_ERR_PARAM, "Invalid policy for query User");
			goto CLEANUP;
		}
	}

	aerospike_drop_user(as_client, &err, admin_policy_p, user);

CLEANUP:

	if (err.code != AEROSPIKE_OK) {
		update_client_error(getThis(), err.code, err.message, err.in_doubt);
	}

	RETURN_LONG(err.code);
}
