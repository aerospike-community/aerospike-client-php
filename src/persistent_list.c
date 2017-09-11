#include "persistent_list.h"
#include "php_aerospike.h"

#define MAX_PORT_SIZE 6
#define MAX_SHM_SIZE 19

char* get_persistent_search_string(aerospike* as) {
	char* alias_to_search = NULL;
	char port_str[MAX_PORT_SIZE];

	int tot_address_size = 0;
	int tot_port_size = 0;
	int delimiter_size = 0;
	int tot_user_size = 0;
	int tot_shm_size = 0;
	int i = 0;

	//Calculate total size for search string
	for (i = 0; i < (int)as->config.hosts->size; i++)
	{
		as_host *host = (as_host *)as_vector_get(as->config.hosts, i);
		tot_address_size = tot_address_size + strlen(host->name);
		tot_port_size = tot_port_size + MAX_PORT_SIZE;
		delimiter_size = delimiter_size + 3;
		tot_user_size = tot_user_size + strlen(as->config.user);
	}
	if (as->config.use_shm) {
		tot_shm_size = MAX_SHM_SIZE;
	}

	alias_to_search = (char*) malloc(tot_address_size + tot_user_size + tot_port_size + delimiter_size + tot_shm_size);
	if (!alias_to_search) {
		return NULL;
	}
	alias_to_search[0] = '\0';

	for (i=0; i<as->config.hosts->size; i++) {
		as_host *host = (as_host *)as_vector_get(as->config.hosts, i);
		int port = host->port;
		sprintf(port_str, "%d", port);
		strcat(alias_to_search, host->name);
		strcat(alias_to_search, ":");
		strcat(alias_to_search, port_str);
		strcat(alias_to_search, ":");
		strcat(alias_to_search, as->config.user);
		strcat(alias_to_search, ";");
	}

	if (as->config.use_shm) {
		char shm_str[MAX_SHM_SIZE];
		sprintf(shm_str, "%x", as->config.shm_key);
		strcat(alias_to_search, shm_str);
	}
	return alias_to_search;
}


/* This will only be called if the host does not already exist */
persistent_host* add_persistent_host(const char* host_key, aerospike* as_client) {
	size_t len;
	len = strlen(host_key);

	persistent_host* host = (persistent_host*)malloc(sizeof(persistent_host));
	host->ref_cnt = 1;
	host->conn_cnt = 0;
	host->is_connected = false;
	host->as_client = as_client;
	zend_hash_str_add_ptr(AEROSPIKE_G(persistent_list_g), host_key, len, (void*)host);
	return host;
}


persistent_host* get_persistent_host(const char* host_key) {
	persistent_host* host = NULL;
	size_t len;
	len = strlen(host_key);

	host = (persistent_host*)zend_hash_str_find_ptr(AEROSPIKE_G(persistent_list_g), host_key, len);
	return host;
}

void persistent_host_dtor(zval* zval) {
	as_error err;

	persistent_host* host = (persistent_host*)Z_PTR_P(zval);
	if (host && host->as_client) {
		aerospike* as = host->as_client;
		aerospike_close(as, &err);
		aerospike_destroy(as);
	}
	if (host) {
		/* This was malloc'd in add_persistent_host so we need to free it */
		free(host);
	}
}

as_status persistent_connect(AerospikeClient* client) {
	as_error err;
	as_error_init(&err);
	as_status status = AEROSPIKE_OK;
	client->is_persistent = true;
	char* alias_to_search = get_persistent_search_string(client->as_client);

	if (!alias_to_search) {
		status = AEROSPIKE_ERR_CLIENT;
		goto CLEANUP;
	}

	persistent_host* persistent_host = get_persistent_host(alias_to_search);

	if (persistent_host) {
		aerospike_destroy(client->as_client);
		client->as_client = persistent_host->as_client;

		if (!persistent_host->is_connected) {

			status = aerospike_connect(client->as_client, &err);
			if (status == AEROSPIKE_OK) {
				persistent_host->conn_cnt++;
				persistent_host->is_connected = true;
			} else {
				goto CLEANUP;
			}
		} else {
			persistent_host->conn_cnt++;
		}
		persistent_host->ref_cnt++;
	/* We didn't find an existing host, so create a new one, only if the connection succeeded*/
	} else {
		status = aerospike_connect(client->as_client, &err);
		if (status == AEROSPIKE_OK) {
			persistent_host = add_persistent_host(alias_to_search, client->as_client);
			persistent_host->conn_cnt = 1;
			persistent_host->is_connected = true;
		} else {
			goto CLEANUP;
		}
	}

CLEANUP:
	if(alias_to_search) {
		free(alias_to_search);
	}
	return status;
}

as_status non_persistent_connect(AerospikeClient* client) {
	as_error err;
	as_error_init(&err);

	as_status status;
	client->is_persistent = false;
	status = aerospike_connect(client->as_client, &err);
	if (status == AEROSPIKE_OK) {
		client->is_connected = true;
	}

	return status;
}












