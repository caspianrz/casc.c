#include <string.h>

#define TOTAL_KEYWORDS 34
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 54

#ifdef __GNUC__
__inline
#else
#endif
    static unsigned int
    hash(register const char *str, register size_t len)
{
    static unsigned char asso_values[] =
        {
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 20, 0, 30,
            20, 20, 0, 10, 55, 5, 55, 55, 40, 55,
            0, 0, 5, 55, 0, 5, 0, 10, 0, 5,
            55, 55, 30, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
            55, 55, 55, 55, 55, 55};
    register unsigned int hval = len;

    switch (hval)
    {
    default:
        hval += asso_values[(unsigned char)str[2]];
    /*FALLTHROUGH*/
    case 2:
    case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
    return hval;
}

const char *
in_word_set(register const char *str, register size_t len)
{
    static const char *wordlist[] =
        {
            "", "", "",
            "for",
            "",
            "float",
            "return",
            "if",
            "int",
            "void",
            "short",
            "struct",
            "typedef",
            "restrict",
            "goto",
            "while",
            "switch",
            "",
            "register",
            "",
            "union",
            "signed",
            "do",
            "unsigned",
            "auto",
            "break",
            "extern",
            "default",
            "",
            "else",
            "",
            "static",
            "", "",
            "enum",
            "const",
            "double",
            "",
            "continue",
            "case",
            "",
            "sizeof",
            "", "",
            "long",
            "", "", "",
            "volatile",
            "", "",
            "inline",
            "", "",
            "char"};

    if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
        register unsigned int key = hash(str, len);

        if (key <= MAX_HASH_VALUE)
        {
            register const char *s = wordlist[key];

            if (*str == *s && !strcmp(str + 1, s + 1))
                return s;
        }
    }
    return (const char *)0;
}