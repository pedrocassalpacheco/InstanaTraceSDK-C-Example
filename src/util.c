#include "util.h"

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char *toString(uint64_t ulong_value)
{
  const int n = snprintf(NULL, 0, "%lu", ulong_value);

  assert(n > 0);

  char* str = (char*)calloc(n+1, sizeof(char));
  int c = snprintf(str, n+1, "%lu", ulong_value);

  assert(str[n] == '\0');
  assert(c == n);

  return str;
}

char *string_trim_inplace(char *s)
{
  while (isspace((unsigned char) *s)) s++;

    if (*s)
    {
      char *p = s;
      while (*p) p++;
      while (isspace((unsigned char) *(--p)));
      p[1] = '\0';
    }

  return s;

}
