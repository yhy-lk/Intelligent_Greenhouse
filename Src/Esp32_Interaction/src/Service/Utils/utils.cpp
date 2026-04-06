#include "utils.h"
#include <string>
#include <algorithm>

namespace Utils {

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    // 优化实现：使用标准库算法，更简洁高效
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(),
                     [](char ca, char cb) { return std::tolower(ca) == std::tolower(cb); });
}

std::string trim(const std::string& str) {
    const auto first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    const auto last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.rfind(prefix, 0) == 0;
}

int indexOf(const std::string& str, const std::string& substr) {
    size_t pos = str.find(substr);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

int lastIndexOf(const std::string& str, char ch) {
    size_t pos = str.rfind(ch);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}


} // namespace Utils