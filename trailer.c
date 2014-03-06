#include "cache.h"
/*
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 */

enum action_where { WHERE_AFTER, WHERE_BEFORE };
enum action_if_exists { EXISTS_ADD_IF_DIFFERENT, EXISTS_ADD_IF_DIFFERENT_NEIGHBOR,
			EXISTS_ADD, EXISTS_OVERWRITE, EXISTS_DO_NOTHING };
enum action_if_missing { MISSING_ADD, MISSING_DO_NOTHING };

struct conf_info {
	char *name;
	char *key;
	char *command;
	enum action_where where;
	enum action_if_exists if_exists;
	enum action_if_missing if_missing;
};

struct trailer_item {
	struct trailer_item *previous;
	struct trailer_item *next;
	const char *token;
	const char *value;
	struct conf_info conf;
};

static int same_token(struct trailer_item *a, struct trailer_item *b, int alnum_len)
{
	return !strncasecmp(a->token, b->token, alnum_len);
}

static int same_value(struct trailer_item *a, struct trailer_item *b)
{
	return !strcasecmp(a->value, b->value);
}

static int same_trailer(struct trailer_item *a, struct trailer_item *b, int alnum_len)
{
	return same_token(a, b, alnum_len) && same_value(a, b);
}

/* Get the length of buf from its beginning until its last alphanumeric character */
static size_t alnum_len(const char *buf, size_t len)
{
	while (len > 0 && !isalnum(buf[len - 1]))
		len--;
	return len;
}
