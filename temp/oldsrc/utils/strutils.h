#ifndef STRUTILS_H_H_H
#define STRUTILS_H_H_H

#include <cstring>

/**********************************************************
 * 字母转大写
 **********************************************************/
static inline char ch_toupper(char c)
{
    if (c >= 'a' && c <= 'z') {
        c ^= 0x20;
    }
    return c;
}

/**********************************************************
 * 字母转小写
 **********************************************************/
static inline char ch_tolower(char c)
{
    if (c >= 'A' && c <= 'Z') {
        c ^= 0x20;
    }
    return c;
}

/**********************************************************
 * 字符串安全拷贝，返回字符串拷贝长度
 **********************************************************/
size_t string_copy(char *dst, const char *src, size_t maxlen);

/**********************************************************
 * 字符串追加，返回字符串追加后的总长度
 **********************************************************/
size_t string_lcat(char *dst, const char *src, size_t maxlen);

/**********************************************************
 * 字符串格式化追加拷贝，返回字符串追加后的总长度
 **********************************************************/
size_t snprint_lcatf(char *dst, size_t maxlen, const char *fmt, ...);

/**********************************************************
 * 字符串前缀是否匹配
 **********************************************************/
bool string_istart(const char *str, const char *prefix, const char **ptr);

/**********************************************************
 * 字符串比较（忽略大小写）
 **********************************************************/
int string_casecmp(const char *a, const char *b);

/**********************************************************
 * 字符串指定长度比较（忽略大小写）
 **********************************************************/
int string_casencmp(const char *a, const char *b, size_t n);

#endif