#pragma once

#include <string>
#include <cmath>

// 큰 숫자를 K, M, B, T로 표기
inline std::string formatNumber(long long num) {
    if (num < 1000) {
        return std::to_string(num);
    } else if (num < 1000000) {
        double val = num / 1000.0;
        return std::to_string(static_cast<int>(val)) + "K";
    } else if (num < 1000000000) {
        double val = num / 1000000.0;
        return std::to_string(static_cast<int>(val)) + "M";
    } else if (num < 1000000000000LL) {
        double val = num / 1000000000.0;
        return std::to_string(static_cast<int>(val)) + "B";
    } else {
        double val = num / 1000000000000.0;
        return std::to_string(static_cast<int>(val)) + "T";
    }
}
