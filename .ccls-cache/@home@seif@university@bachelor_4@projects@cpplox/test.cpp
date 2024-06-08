#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<int, std::string> testMap;
    testMap[1] = "one";
    testMap[2] = "two";
    for (const auto& pair : testMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    return 0;
}
