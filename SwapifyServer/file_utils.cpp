#include "file_utils.h"

std::vector<unsigned char> base64Decode(const std::string& input) {
    std::vector<unsigned char> output;

    // Base64 decoding table
    const std::string base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string::size_type inLen = input.size();
    int i = 0;

    while (i < inLen) {
        char c1 = input[i++];
        if (c1 == '=') break; // padding character, end of input
        if (c1 > 127 || base64Chars.find(c1) == std::string::npos) continue; // ignore non-base64 characters

        char c2 = input[i++];
        if (c2 == '=') break; // padding character, end of input
        if (c2 > 127 || base64Chars.find(c2) == std::string::npos) continue; // ignore non-base64 characters

        char c3 = input[i++];
        if (c3 == '=') break; // padding character, end of input
        if (c3 > 127 || base64Chars.find(c3) == std::string::npos) continue; // ignore non-base64 characters

        char c4 = input[i++];
        if (c4 == '=') break; // padding character, end of input
        if (c4 > 127 || base64Chars.find(c4) == std::string::npos) continue; // ignore non-base64 characters

        unsigned char b1 = base64Chars.find(c1);
        unsigned char b2 = base64Chars.find(c2);
        unsigned char b3 = base64Chars.find(c3);
        unsigned char b4 = base64Chars.find(c4);

        output.push_back((b1 << 2) | (b2 >> 4));
        if (c3 != '=') output.push_back(((b2 & 0x0F) << 4) | (b3 >> 2));
        if (c4 != '=') output.push_back(((b3 & 0x03) << 6) | b4);
    }

    return output;
}
std::string base64Encode(const std::vector<unsigned char>& input) {
    std::string output;
    const std::string base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::vector<unsigned char>::size_type i = 0;
    while (i < input.size()) {
        unsigned char b1 = i < input.size() ? input[i++] : 0;
        unsigned char b2 = i < input.size() ? input[i++] : 0;
        unsigned char b3 = i < input.size() ? input[i++] : 0;

        unsigned char c1 = b1 >> 2;
        unsigned char c2 = ((b1 & 0x03) << 4) | (b2 >> 4);
        unsigned char c3 = ((b2 & 0x0F) << 2) | (b3 >> 6);
        unsigned char c4 = b3 & 0x3F;

        output += base64Chars[c1];
        output += base64Chars[c2];
        output += (i <= input.size() + 1) ? base64Chars[c3] : '=';
        output += (i <= input.size()) ? base64Chars[c4] : '=';
    }

    return output;
}