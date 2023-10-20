#include <resultpp.hxx>
#include <string>

int main(int argc, const char **argv) {
    resultpp::Result<int> result = {true, "false"};
    resultpp::Result<std::string> result01 = {"hello", ""};

    return 0;
}