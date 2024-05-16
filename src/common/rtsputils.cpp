#include "rtsputils.h"
#include "strutils.h"
#include <cstring>
#include <memory>

using namespace std;

void split_video_url(RTSPUrlInfo *info, const char *url)
{
    const char *pos = nullptr;
    if ((pos = strchr(url, ':'))) {
        string_copy(info->proto, url, (pos + 1 - url));

        pos++; // skip ':'
        if (*pos == '/') {
            pos++;
        }
        if (*pos == '/') {
            pos++;
        }
    } else {
        string_copy(info->path, url, sizeof(info->path));
        return;
    }

    const char *lstr = pos + strcspn(pos, "/?#");
    string_copy(info->path, lstr, sizeof(info->path));

    if (lstr != pos) {
        const char *begin = pos, *end = nullptr;
        while ((end = strchr(pos, '@')) && end < lstr) {
            string_copy(info->authorization, begin, end + 1 - begin);
            pos = end + 1;
        }

        const char *brk = nullptr, *col = nullptr;
        if (*pos == '[' && (brk = strchr(pos, ']')) && brk < lstr) {
            string_copy(info->hostname, pos + 1, brk - pos);
            if (brk[1] == ':') {
                info->port = atoi(brk + 2);
            }
        } else if ((col = strchr(pos, ':')) && col < lstr) {
            string_copy(info->hostname, pos, col + 1 - pos);
            info->port = atoi(col + 1);
        } else {
            string_copy(info->hostname, pos, lstr + 1 - pos);
        }
    }
}

void get_str_until_chars(char *buf, int buf_size, const char *sep, const char **pp)
{
    const char *p = *pp;
    char       *q = buf;

    p += strspn(p, SPACE_CHARS);
    while (!strchr(sep, *p) && (*p != '\0')) {
        if ((q - buf) < buf_size - 1) {
            *q++ = *p;
        }
        p++;
    }
    if (buf_size > 0) {
        *q = '\0';
    }
    *pp = p;
}

void get_str_skip_slash(char *buf, int buf_size, const char *sep, const char **pp)
{
    if (**pp == '/') {
        (*pp)++;
    }
    get_str_until_chars(buf, buf_size, sep, pp);
}