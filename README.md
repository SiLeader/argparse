# argparse: Python like C++ Argument parser

&copy; 2018 SiLeader.

## features
+ only one header file.
+ simular with Python's argparse module

## usage
```c++
#include "argparse.hpp"

int main(int argc, char** argv) {
    argparse::ArgumentParser parser("test", "argparse test program", "Apache License 2.0");

    parser.addArgument({"--value", "--set-value"}, "set value");
    parser.addArgument({"--flag"}, "flag", argparse::ArgumentType::StoreTrue);
    parser.addArgument({"--value-default", "--value-with-default-value"}, "set value or use default value");

    auto args = parser.parseArgs(argc, argv);

    std::cout << std::boolalpha;
    std::cout << "value: " << args.get<std::string>("value") << std::endl;
    std::cout << "flag:  " << args.has("flag") << std::endl;
    std::cout << "value(default='abc'): " << args.safeGet<std::string>("value-default", "abc") << std::endl;
}
```

### run `COMMAND -h` on shell

```plain
usage: test [OPTIONS]...

argparse test program

keyword arguments:
  --help, -h                                              : show this help
  --value VALUE, --set-value VALUE                        : set value
  --flag                                                  : flag
  --value-default VALUE, --value-with-default-value VALUE : set value or use default value

Apache License 2.0
```

### run `COMMAND --value=argparse` on shell
```plain
value: argparse
flag:  false
value(default='abc'): abc
```

### run `COMMAND --value argparse --flag` on shell
```plain
value: argparse
flag:  true
value(default='abc'): abc
```

### run `COMMAND --value=argparse --flag --value-default parser` on shell
```plain
value: argparse
flag:  true
value(default='abc'): parser
```

## license
Apache License 2.0
