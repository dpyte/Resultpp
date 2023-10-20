#include <resultpp.hxx>
#include <string>

int main(int argc, const char **argv) {
    resultpp::Result<int> result = {true, "false"};
    resultpp::Result<std::string> result01 = {"hello", ""};

    std::printf("Original value(s) for\n`result`: %d // %s\n\n", result.Data(), result.Message().c_str());

    result << false;
    result = "True";
    std::printf("Post modification:\nresult: %d // %s\n\n", result.Data(), result.Message().c_str());

    std::printf("Is OK; Contains error => %d // %d\n", result.IsOk(), result.IsErr());
    result = "";
    std::printf("Is Ok; Contains error => %d // %d\n", result.IsOk(), result.IsErr());

    return 0;
}