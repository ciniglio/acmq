#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

#define MAX_STRING_LENGTH 1024

int add(struct client *c, char *buf)
{
	int len = strnlen(buf, MAX_STRING_LENGTH);
	char * str = malloc(sizeof(char) * (len + 10 + 1));
	if(str == NULL){
		perror("alloc");
		return 1;
	}

	strlcpy(str, "BLOOM_ADD ", 11);
	strlcat(str, buf, len + 10 + 1);

	if(client_transaction(c, str, NULL) != 0){
		perror("sending");
		return 1;
	}

	free(str);
	return 0;

}

int check(struct client *c, char *buf)
{
	int len = strnlen(buf, MAX_STRING_LENGTH);
	char * str = malloc(sizeof(char) * (len + 12 + 1));
	if(str == NULL){
		perror("alloc");
		return 1;
	}

	strlcpy(str, "BLOOM_CHECK ", 13);
	strlcat(str, buf, len + 12 + 1);

	if(client_transaction(c, str, NULL) != 0){
		perror("sending");
		return 1;
	}

	free(str);
	return 0;

}

int delete(struct client *c, char * buf)
{

	int len = strnlen(buf, MAX_STRING_LENGTH);
	char * str = malloc(sizeof(char) * (len + 13 + 1));
	if(str == NULL){
		perror("alloc");
		return 1;
	}

	strlcpy(str, "BLOOM_DELETE ", 14);
	strlcat(str, buf, len + 13 + 1);

	if(client_transaction(c, str, NULL) != 0){
		perror("sending");
		return 1;
	}

	free(str);
	return 0;
}

int save(struct client *c, char * buf)
{

	int len = strnlen(buf, MAX_STRING_LENGTH);
	char * str = malloc(sizeof(char) * (len + 13 + 1));
	if(str == NULL){
		perror("alloc");
		return 1;
	}

	strlcpy(str, "BLOOM_SAVE ", 14);
	strlcat(str, buf, len + 13 + 1);

	if(client_transaction(c, str, NULL) != 0){
		perror("sending");
		return 1;
	}

	free(str);
	return 0;
}

