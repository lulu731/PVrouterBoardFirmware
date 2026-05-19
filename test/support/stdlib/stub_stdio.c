#include <stdio.h>
#include <string.h>
#include "unity.h"

static const char* stub_html_line = "<html></html>\n";
static const char* last_test_name = NULL;

FILE* fopen(const char *pathname, const char *mode)
{
    (void)pathname;
    (void)mode;

    if (Unity.CurrentTestName != NULL &&
        strcmp(Unity.CurrentTestName, "test_index_handler_with_unexistant_file_should_fail") == 0)
    {
        return NULL;
    }

    return (FILE*)0x1234;
}

char *fgets(char *s, int size, FILE *stream)
{
    (void)size;

    if (stream != (FILE*)0x1234)
        return NULL;

    if (Unity.CurrentTestName != last_test_name)
    {
        last_test_name = Unity.CurrentTestName;
        strncpy(s, stub_html_line, size - 1);
        s[size - 1] = '\0';
        return s;
    }

    return NULL;
}

int fclose(FILE *stream)
{
    (void)stream;
    return 0;
}
