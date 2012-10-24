#include "bloom_router.h"
#include "bloom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloom_io.h"

#define BLOOM_ADD "BLOOM_ADD"
#define ADD_LEN 9

#define BLOOM_CHECK "BLOOM_CHECK"
#define CHECK_LEN 11

#define BLOOM_DELETE "BLOOM_DELETE"
#define DELETE_LEN 12

#define BLOOM_SAVE "BLOOM_SAVE"
#define SAVE_LEN 10

void handle_string(char *str, char **result, void * state)
{
	BLOOM * bloom = (BLOOM *) state;
	*result = malloc(sizeof(char) * 1);
	int ret_val;
	char *command, *body;
	command = strtok_r(str, " ", &body);
	if (command != NULL && body != NULL &&
		!strncmp(command, BLOOM_ADD, ADD_LEN)){
		ret_val = bloom_insert(bloom, body, strlen(body));
		sprintf(*result, "%d", ret_val);
	} else if (command != NULL && body != NULL &&
		!strncmp(command, BLOOM_CHECK, CHECK_LEN)){
		ret_val = bloom_check(bloom, body, strlen(body));
		sprintf(*result, "%d", ret_val);
	} else if (command != NULL && body != NULL &&
		!strncmp(command, BLOOM_DELETE, DELETE_LEN)){
		ret_val = bloom_delete(bloom, body, strlen(body));
		sprintf(*result, "%d", ret_val);
	} else if (command != NULL &&
		!strncmp(command, BLOOM_SAVE, SAVE_LEN)){
		ret_val = bloom_write(bloom);
		sprintf("bloom filter saved to %s\n", FILENAME);
	} else {
		printf("ERROR! Enter a valid bloom command\n");
	}
}














