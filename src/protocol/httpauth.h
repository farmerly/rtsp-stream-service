#pragma once

#include <memory>

/**********************************************************
 * 身份验证类型，由弱到强。
 **********************************************************/
enum HTTPAuthType {
    HTTP_AUTH_NONE = 0, // 无身份校验
    HTTP_AUTH_BASIC,    // HTTP 1.0 基本认证 RFC 1945 (及 RFC 2617)
    HTTP_AUTH_DIGEST,   // HTTP 1.1 摘要认证 RFC 2617
};

/**********************************************************
 * 摘要认证参数结构体。
 **********************************************************/
struct DigestParams {
    char nonce[300];
};

/**********************************************************
 * HTTP 身份验证结构体。
 **********************************************************/
struct HTTPAuthState {
    HTTPAuthType auth_type;
    char         realm[200];
    DigestParams digest_params;
};

// 生成 digest 加密串
std::shared_ptr<char> make_digest_auth(
    HTTPAuthState *state, const char *username, const char *password, const char *uri, const char *method);

// 返回生成的 Authorization 数据
std::shared_ptr<char> http_auth_create_response(HTTPAuthState *state, const char *auth, const char *uri, const char *method);