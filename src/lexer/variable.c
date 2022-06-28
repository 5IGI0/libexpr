#include <stdlib.h>
#include <string.h>

const char *expr_parse_variable(const char *str, char **varname) {
	size_t len;
	char *tmp;

	/* baguette hon hon hon */
	len = strspn(str,
		"azertyuiopqsdfghjklmwxcvbn"
		"AZERTYUIOPQSDFGHJKLMWXCVBN"
		"0123456789"
	);

	if (len == 0)
		return NULL;
	
	if (varname) {
		tmp = malloc(len + 1);
		if (!tmp)
			return NULL;
		
		memcpy(tmp, str, len);
		tmp[len] = 0;
		*varname = tmp;
	}
	return str + len;
}