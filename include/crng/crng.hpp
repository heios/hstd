#include <array>
#include <fstream>
#include <cstddef>

/// Cryptographic Random Number Generator
namespace crng {

    std::string Generate(const std::size_t size) {
        std::string result;
        result.resize(size);
        #if true or defined __linux__ or defined __APPLE__
            // TODO(heios): optimize it later on
            std::fstream urandom("/dev/urandom", std::ios::binary | std::ios::in);
            urandom.read(&result[0], result.size());
        #else
            #error "Unsupported platform"
        #endif
        return result;
    }

    namespace Impl {
        auto Base64UrlEncode(const std::string& src) {
            static constexpr const auto Base64UrlTable = std::array<char, 64>{
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                    'w', 'x', 'y', 'z', '0', '1', '2', '3',
                    '4', '5', '6', '7', '8', '9', '-', '_',
                };
            std::string result;
            if (src.empty()) {
                return result;
            }
            result.resize((src.size() + 2) / 3 * 4);
            char* iResult = &result[0];
            for (size_t i = 0; i < src.size();) {
                uint32_t value = 0;
                value |= (i < src.size() ? (uint8_t)src[i++] << (2 * 8) : (uint8_t)0);
                value |= (i < src.size() ? (uint8_t)src[i++] << (1 * 8) : (uint8_t)0);
                value |= (i < src.size() ? (uint8_t)src[i++] << (0 * 8) : (uint8_t)0);

                *iResult++ = Base64UrlTable[(value >> (3 * 6)) & 63];
                *iResult++ = Base64UrlTable[(value >> (2 * 6)) & 63];
                *iResult++ = Base64UrlTable[(value >> (1 * 6)) & 63];
                *iResult++ = Base64UrlTable[(value >> (0 * 6)) & 63];
            }
            result.resize((src.size() + 2) * 3 / 4);
            return result;
        }
    }

    std::string GenerateId22Base63() {
        std::string result;
        for (const auto v : Impl::Base64UrlEncode(Generate(32))) {
            if (result.size() == 22) {
                return result;
            }
            if (v != '-') {
                result.push_back(v);
            }
        }
        return result;
    }

}
