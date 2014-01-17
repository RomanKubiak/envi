/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== LICENSE ==================
static const unsigned char temp_binary_data_0[] =
"Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>\r\n"
"Copyright (c) 2013 Cesanta Software Limited\r\n"
"All rights reserved\r\n"
"\r\n"
"This code is dual-licensed: you can redistribute it and/or modify\r\n"
"it under the terms of the GNU General Public License version 2 as\r\n"
"published by the Free Software Foundation. For the terms of this\r\n"
"license, see <http://www.gnu.org/licenses/>.\r\n"
"\r\n"
"You are free to use this code under the terms of the GNU General\r\n"
"Public License, but WITHOUT ANY WARRANTY; without even the implied\r\n"
"warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\r\n"
"See the GNU General Public License for more details.\r\n"
"\r\n"
"Alternatively, you can license this code under a commercial\r\n"
"license, as set out in <http://cesanta.com/products.html>.\r\n";

const char* LICENSE = (const char*) temp_binary_data_0;

//================== README.md ==================
static const unsigned char temp_binary_data_1[] =
"SLRE: Super Light Regular Expression library\r\n"
"============================================\r\n"
"\r\n"
"SLRE is an ISO C library that implements a subset of Perl regular\r\n"
"expression syntax. Main features of SLRE are:\r\n"
"\r\n"
"   * Written in strict ANSI C'89\r\n"
"   * Small size (compiled x86 code is about 5kB)\r\n"
"   * Uses little stack and does no dynamic memory allocation\r\n"
"   * Provides simple intuitive API\r\n"
"   * Implements most useful subset of Perl regex syntax (see below)\r\n"
"   * Easily extensible. E.g. if one wants to introduce a new\r\n"
"metacharacter `\\i`, meaning \"IPv4 address\", it is easy to do so with SLRE.\r\n"
"\r\n"
"SLRE is perfect for tasks like parsing network requests, configuration\r\n"
"files, user input, etc, when libraries like [PCRE](http://pcre.org) are too\r\n"
"heavyweight for the given task. Developers of embedded systems would benefit\r\n"
"most.\r\n"
"\r\n"
"## Supported Syntax\r\n"
"\r\n"
"    (?i)    Must be at the beginning of the regex. Makes match case-insensitive\r\n"
"    ^       Match beginning of a buffer\r\n"
"    $       Match end of a buffer\r\n"
"    ()      Grouping and substring capturing\r\n"
"    \\s      Match whitespace\r\n"
"    \\S      Match non-whitespace\r\n"
"    \\d      Match decimal digit\r\n"
"    +       Match one or more times (greedy)\r\n"
"    +?      Match one or more times (non-greedy)\r\n"
"    *       Match zero or more times (greedy)\r\n"
"    *?      Match zero or more times (non-greedy)\r\n"
"    ?       Match zero or once (non-greedy)\r\n"
"    x|y     Match x or y (alternation operator)\r\n"
"    \\meta   Match one of the meta character: ^$().[]*+?|\\\r\n"
"    \\xHH    Match byte with hex value 0xHH, e.g. \\x4a\r\n"
"    [...]   Match any character from set. Ranges like [a-z] are supported\r\n"
"    [^...]  Match any character but ones from set\r\n"
"\r\n"
"Under development: Unicode support.\r\n"
"\r\n"
"## API\r\n"
"\r\n"
"    int slre_match(const char *regexp, const char *buf, int buf_len,\r\n"
"                   struct slre_cap *caps, int num_caps);\r\n"
"\r\n"
"`slre_match()` matches string buffer `buf` of length `buf_len` against\r\n"
"regular expression `regexp`, which should conform the syntax outlined\r\n"
"above. If regular expression `regexp` contains brackets, `slre_match()`\r\n"
"can capture the respective substrings into the array of `struct slre_cap`\r\n"
"structures:\r\n"
"\r\n"
"    /* Stores matched fragment for the expression inside brackets */\r\n"
"    struct slre_cap {\r\n"
"      const char *ptr;  /* Points to the matched fragment */\r\n"
"      int len;          /* Length of the matched fragment */\r\n"
"    };\r\n"
"\r\n"
"N-th member of the `caps` array will contain fragment that corresponds to the\r\n"
"N-th opening bracket in the `regex`, N is zero-based. `slre_match()` returns\r\n"
"number of bytes scanned from the beginning of the string. If return value is\r\n"
"greater or equal to 0, there is a match. If return value is less then 0, there\r\n"
"is no match. Negative return codes are as follows:\r\n"
"\r\n"
"    #define SLRE_NO_MATCH               -1\r\n"
"    #define SLRE_UNEXPECTED_QUANTIFIER  -2\r\n"
"    #define SLRE_UNBALANCED_BRACKETS    -3\r\n"
"    #define SLRE_INTERNAL_ERROR         -4\r\n"
"    #define SLRE_INVALID_CHARACTER_SET  -5\r\n"
"    #define SLRE_INVALID_METACHARACTER  -6\r\n"
"    #define SLRE_CAPS_ARRAY_TOO_SMALL   -7\r\n"
"    #define SLRE_TOO_MANY_BRANCHES      -8\r\n"
"    #define SLRE_TOO_MANY_BRACKETS      -9\r\n"
"\r\n"
"\r\n"
"## Example: parsing HTTP request line\r\n"
"\r\n"
"    const char *request = \" GET /index.html HTTP/1.0\\r\\n\\r\\n\";\r\n"
"    struct slre_cap caps[4];\r\n"
"\r\n"
"    if (slre_match(\"^\\\\s*(\\\\S+)\\\\s+(\\\\S+)\\\\s+HTTP/(\\\\d)\\\\.(\\\\d)\",\r\n"
"                   request, strlen(request), caps, 4) > 0) {\r\n"
"      printf(\"Method: [%.*s], URI: [%.*s]\\n\",\r\n"
"             caps[0].len, caps[0].ptr,\r\n"
"             caps[1].len, caps[1].ptr);\r\n"
"    } else {\r\n"
"      printf(\"Error parsing [%s]\\n\", request);\r\n"
"    }\r\n"
"\r\n"
"## Example: find all URLs in a string\r\n"
"\r\n"
"    static const char *str =\r\n"
"      \"<img src=\\\"HTTPS://FOO.COM/x?b#c=tab1\\\"/> \"\r\n"
"      \"  <a href=\\\"http://cesanta.com\\\">some link</a>\";\r\n"
"\r\n"
"    static const char *regex = \"(?i)((https?://)[^\\\\s/'\\\"<>]+/?[^\\\\s'\\\"<>]*)\";\r\n"
"    struct slre_cap caps[2];\r\n"
"    int i, j = 0, str_len = strlen(str);\r\n"
"\r\n"
"    while (j < str_len &&\r\n"
"           (i = slre_match(regex, str + j, str_len - j, caps, 2, NULL)) > 0) {\r\n"
"      printf(\"Found URL: [%.*s]\\n\", caps[0].len, caps[0].ptr);\r\n"
"      j += i;\r\n"
"    }\r\n"
"\r\n"
"Output:\r\n"
"\r\n"
"    Found URL: [HTTPS://FOO.COM/x?b#c=tab1]\r\n"
"    Found URL: [http://cesanta.com]\r\n"
"\r\n"
"# License\r\n"
"\r\n"
"SLRE is released under\r\n"
"[GNU GPL v.2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html).\r\n"
"Businesses have an option to get non-restrictive, royalty-free commercial\r\n"
"license and professional support from\r\n"
"[Cesanta Software](http://cesanta.com).\r\n"
"\r\n"
"[Super Light DNS Resolver](https://github.com/cesanta/sldr),\r\n"
"[Mongoose web server](https://github.com/cesanta/mongoose)\r\n"
"are other projects by Cesanta Software, developed with the same philosophy\r\n"
"of functionality and simplicity.\r\n";

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
        case 0x34bc1021:  numBytes = 752; return LICENSE;
        case 0x64791dc8:  numBytes = 4759; return README_md;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const int namedResourceListSize = 2;

const char* namedResourceList[] =
{
    "LICENSE",
    "README_md"
};

}
