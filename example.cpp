#include "argparse.hpp"

int main(int argc, char** argv) {
    argparse::ArgumentParser parser("test", "argparse test program", "Apache License 2.0");

    parser.addArgument({"--value", "--set-value"}, "set value");
    parser.addArgument({"--flag"}, "flag", argparse::ArgumentType::StoreTrue);
    parser.addArgument({"--value-default", "--value-with-default-value"}, "set value or use default value");
    parser.addArgument({"value1"}, "value1");
    parser.addArgument({"value2"}, "value2");

    auto args = parser.parseArgs(argc, argv);

    std::cout << std::boolalpha;
    std::cout << "value: " << args.get<std::string>("value") << std::endl;
    std::cout << "flag:  " << args.has("flag") << std::endl;
    std::cout << "value(default='abc'): " << args.safeGet<std::string>("value-default", "abc") << std::endl;
    std::cout << "value1: " << args.get<std::string>("value1")<<std::endl;
    std::cout << "value2: " << args.get<std::string>("value2")<<std::endl;
}
