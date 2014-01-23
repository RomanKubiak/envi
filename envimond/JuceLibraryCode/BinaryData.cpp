/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== LICENSE ==================
static const unsigned char temp_binary_data_0[] =
"Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>\n"
"Copyright (c) 2013 Cesanta Software Limited\n"
"All rights reserved\n"
"\n"
"This code is dual-licensed: you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License version 2 as\n"
"published by the Free Software Foundation. For the terms of this\n"
"license, see <http://www.gnu.org/licenses/>.\n"
"\n"
"You are free to use this code under the terms of the GNU General\n"
"Public License, but WITHOUT ANY WARRANTY; without even the implied\n"
"warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"See the GNU General Public License for more details.\n"
"\n"
"Alternatively, you can license this code under a commercial\n"
"license, as set out in <http://cesanta.com/products.html>.\n";

const char* LICENSE = (const char*) temp_binary_data_0;

//================== README.md ==================
static const unsigned char temp_binary_data_1[] =
"SLRE: Super Light Regular Expression library\n"
"============================================\n"
"\n"
"SLRE is an ISO C library that implements a subset of Perl regular\n"
"expression syntax. Main features of SLRE are:\n"
"\n"
"   * Written in strict ANSI C'89\n"
"   * Small size (compiled x86 code is about 5kB)\n"
"   * Uses little stack and does no dynamic memory allocation\n"
"   * Provides simple intuitive API\n"
"   * Implements most useful subset of Perl regex syntax (see below)\n"
"   * Easily extensible. E.g. if one wants to introduce a new\n"
"metacharacter `\\i`, meaning \"IPv4 address\", it is easy to do so with SLRE.\n"
"\n"
"SLRE is perfect for tasks like parsing network requests, configuration\n"
"files, user input, etc, when libraries like [PCRE](http://pcre.org) are too\n"
"heavyweight for the given task. Developers of embedded systems would benefit\n"
"most.\n"
"\n"
"## Supported Syntax\n"
"\n"
"    (?i)    Must be at the beginning of the regex. Makes match case-insensitive\n"
"    ^       Match beginning of a buffer\n"
"    $       Match end of a buffer\n"
"    ()      Grouping and substring capturing\n"
"    \\s      Match whitespace\n"
"    \\S      Match non-whitespace\n"
"    \\d      Match decimal digit\n"
"    +       Match one or more times (greedy)\n"
"    +?      Match one or more times (non-greedy)\n"
"    *       Match zero or more times (greedy)\n"
"    *?      Match zero or more times (non-greedy)\n"
"    ?       Match zero or once (non-greedy)\n"
"    x|y     Match x or y (alternation operator)\n"
"    \\meta   Match one of the meta character: ^$().[]*+?|\\\n"
"    \\xHH    Match byte with hex value 0xHH, e.g. \\x4a\n"
"    [...]   Match any character from set. Ranges like [a-z] are supported\n"
"    [^...]  Match any character but ones from set\n"
"\n"
"Under development: Unicode support.\n"
"\n"
"## API\n"
"\n"
"    int slre_match(const char *regexp, const char *buf, int buf_len,\n"
"                   struct slre_cap *caps, int num_caps);\n"
"\n"
"`slre_match()` matches string buffer `buf` of length `buf_len` against\n"
"regular expression `regexp`, which should conform the syntax outlined\n"
"above. If regular expression `regexp` contains brackets, `slre_match()`\n"
"can capture the respective substrings into the array of `struct slre_cap`\n"
"structures:\n"
"\n"
"    /* Stores matched fragment for the expression inside brackets */\n"
"    struct slre_cap {\n"
"      const char *ptr;  /* Points to the matched fragment */\n"
"      int len;          /* Length of the matched fragment */\n"
"    };\n"
"\n"
"N-th member of the `caps` array will contain fragment that corresponds to the\n"
"N-th opening bracket in the `regex`, N is zero-based. `slre_match()` returns\n"
"number of bytes scanned from the beginning of the string. If return value is\n"
"greater or equal to 0, there is a match. If return value is less then 0, there\n"
"is no match. Negative return codes are as follows:\n"
"\n"
"    #define SLRE_NO_MATCH               -1\n"
"    #define SLRE_UNEXPECTED_QUANTIFIER  -2\n"
"    #define SLRE_UNBALANCED_BRACKETS    -3\n"
"    #define SLRE_INTERNAL_ERROR         -4\n"
"    #define SLRE_INVALID_CHARACTER_SET  -5\n"
"    #define SLRE_INVALID_METACHARACTER  -6\n"
"    #define SLRE_CAPS_ARRAY_TOO_SMALL   -7\n"
"    #define SLRE_TOO_MANY_BRANCHES      -8\n"
"    #define SLRE_TOO_MANY_BRACKETS      -9\n"
"\n"
"\n"
"## Example: parsing HTTP request line\n"
"\n"
"    const char *request = \" GET /index.html HTTP/1.0\\r\\n\\r\\n\";\n"
"    struct slre_cap caps[4];\n"
"\n"
"    if (slre_match(\"^\\\\s*(\\\\S+)\\\\s+(\\\\S+)\\\\s+HTTP/(\\\\d)\\\\.(\\\\d)\",\n"
"                   request, strlen(request), caps, 4) > 0) {\n"
"      printf(\"Method: [%.*s], URI: [%.*s]\\n\",\n"
"             caps[0].len, caps[0].ptr,\n"
"             caps[1].len, caps[1].ptr);\n"
"    } else {\n"
"      printf(\"Error parsing [%s]\\n\", request);\n"
"    }\n"
"\n"
"## Example: find all URLs in a string\n"
"\n"
"    static const char *str =\n"
"      \"<img src=\\\"HTTPS://FOO.COM/x?b#c=tab1\\\"/> \"\n"
"      \"  <a href=\\\"http://cesanta.com\\\">some link</a>\";\n"
"\n"
"    static const char *regex = \"(?i)((https?://)[^\\\\s/'\\\"<>]+/?[^\\\\s'\\\"<>]*)\";\n"
"    struct slre_cap caps[2];\n"
"    int i, j = 0, str_len = strlen(str);\n"
"\n"
"    while (j < str_len &&\n"
"           (i = slre_match(regex, str + j, str_len - j, caps, 2, NULL)) > 0) {\n"
"      printf(\"Found URL: [%.*s]\\n\", caps[0].len, caps[0].ptr);\n"
"      j += i;\n"
"    }\n"
"\n"
"Output:\n"
"\n"
"    Found URL: [HTTPS://FOO.COM/x?b#c=tab1]\n"
"    Found URL: [http://cesanta.com]\n"
"\n"
"# License\n"
"\n"
"SLRE is released under\n"
"[GNU GPL v.2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html).\n"
"Businesses have an option to get non-restrictive, royalty-free commercial\n"
"license and professional support from\n"
"[Cesanta Software](http://cesanta.com).\n"
"\n"
"[Super Light DNS Resolver](https://github.com/cesanta/sldr),\n"
"[Mongoose web server](https://github.com/cesanta/mongoose)\n"
"are other projects by Cesanta Software, developed with the same philosophy\n"
"of functionality and simplicity.\n";

const char* README_md = (const char*) temp_binary_data_1;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x34bc1021:  numBytes = 736; return LICENSE;
        case 0x64791dc8:  numBytes = 4637; return README_md;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "LICENSE",
    "README_md"
};

}
