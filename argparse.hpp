//   Copyright 2018 SiLeader.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef PYLIKE_UTILITY_ARGPARSE_HPP
#define PYLIKE_UTILITY_ARGPARSE_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace argparse {
    namespace detail {
        inline std::vector<std::string> split(const std::string& s, char delim) {
            std::vector<std::string> elems;
            std::string item;
            for (char ch : s) {
                if (ch == delim) {
                    if (!item.empty())
                        elems.emplace_back(item);
                    item.clear();
                } else {
                    item += ch;
                }
            }
            if (!item.empty())
                elems.emplace_back(item);
            return elems;
        }
    } // namespace detail

    enum class ArgumentType {
        Store,
        StoreTrue,
    };

    class Arguments {
    private:
        std::unordered_map<std::string, std::vector<std::string>> _arguments;
        std::string _program;
        std::vector<std::string> _command_line;

    public:
        Arguments(const std::unordered_map<std::string, std::vector<std::string>>& arguments, const std::string& program, const std::vector<std::string>& command_line)
                : _arguments(arguments), _program(program), _command_line(command_line) {
        }

    public:
        const std::string& program() const noexcept {
            return _program;
        }

        bool get(const std::string& kw, signed char& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = static_cast<signed char>(std::stoi(_arguments.at(kw)[index]));
            return true;
        }
        bool get(const std::string& kw, unsigned char& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = static_cast<unsigned char>(std::stoi(_arguments.at(kw)[index]));
            return true;
        }
        bool get(const std::string& kw, signed short& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = static_cast<signed short>(std::stoi(_arguments.at(kw)[index]));
            return true;
        }
        bool get(const std::string& kw, unsigned short& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = static_cast<unsigned short>(std::stoi(_arguments.at(kw)[index]));
            return true;
        }
        bool get(const std::string& kw, signed int& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = std::stoi(_arguments.at(kw)[index]);
            return true;
        }
        bool get(const std::string& kw, unsigned int& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = static_cast<unsigned int>(std::stoul(_arguments.at(kw)[index]));
            return true;
        }
        bool get(const std::string& kw, signed long& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = std::stol(_arguments.at(kw)[index]);
            return true;
        }
        bool get(const std::string& kw, unsigned long& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = std::stoul(_arguments.at(kw)[index]);
            return true;
        }
        bool get(const std::string& kw, signed long long& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = std::stoll(_arguments.at(kw)[index]);
            return true;
        }
        bool get(const std::string& kw, unsigned long long& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = std::stoull(_arguments.at(kw)[index]);
            return true;
        }
        bool get(const std::string& kw, std::string& buf, std::size_t index = 0) const {
            if (_arguments.count(kw) == 0) return false;
            if (_arguments.at(kw).size() <= index) return false;
            buf = _arguments.at(kw)[index];
            return true;
        }
        bool has(const std::string& kw) const {
            return _arguments.count(kw) != 0;
        }

        bool get(const std::string& kw, std::vector<std::string>& buf) const {
            if (_arguments.count(kw) == 0) return false;
            buf = _arguments.at(kw);
            return true;
        }

        template <class DataT>
        DataT get(const std::string& kw, std::size_t index = 0) const {
            DataT buf;
            if (get(kw, buf, index)) {
                return buf;
            }
            throw std::runtime_error("cannot find value of '" + kw + "'");
        }

        template <class DataT>
        DataT safeGet(const std::string& kw, const DataT& defaultValue, std::size_t index = 0) const {
            if (has(kw)) {
                return get<DataT>(kw, index);
            }
            return defaultValue;
        }

        const std::vector<std::string>& getCommandLine() const noexcept {
            return _command_line;
        }
    };

    class ArgumentParser {
    private:
        struct _argument_info {
            std::vector<std::string> name;
            std::string help;
            ArgumentType type;
        };

    private:
        std::vector<_argument_info> _positional, _keyword;
        std::string _program;
        std::string _description;
        std::string _epilog;

    public:
        explicit ArgumentParser(std::string prog, std::string description = "", std::string epilog = "")
                : _program(std::move(prog)), _description(std::move(description)), _epilog(std::move(epilog)) {
            addArgument({"--help", "-h"}, "show this help", ArgumentType::StoreTrue);
        }

    public:
        Arguments parseArgs(int argc, char const* const* const argv) const {
            std::string program = argv[0];
            std::vector<std::string> args(argv + 1, argv + argc);

            auto remove_hyphen = [](const std::string& str) -> std::string {
                for (std::size_t i = 0; i < str.size(); ++i) {
                    if (str[i] != '-') {
                        return std::string(std::begin(str) + i, std::end(str));
                    }
                }
                return str;
            };

            {
                std::vector<std::string> a;
                a.reserve(args.size() * 2);
                for (auto& arg : args) {
                    auto ss = detail::split(arg, '=');
                    a.insert(std::end(a), std::begin(ss), std::end(ss));
                }
                args = a;
            }

            std::unordered_map<std::string, std::vector<std::string>> parsed;

            if (args.empty()) {
                return Arguments(parsed, program, args);
            }

            auto itr = std::begin(args);
            if (args[0][0] == '-') {
                auto kw_begin = std::begin(_keyword);
                auto kw_end = std::end(_keyword);

                // check keyword arguments;
                for (; itr != std::end(args); ++itr) {
                    auto kw_itr = std::find_if(kw_begin, kw_end, [&itr](const _argument_info& info) {
                        return std::find(std::begin(info.name), std::end(info.name), *itr) != std::end(info.name);
                    });
                    if (kw_itr == kw_end) {
                        if (itr->front() == '-') {
                            std::cerr << "cannot find keyword argument: " << *itr << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                        break;
                    }

                    auto name = *itr;
                    switch (kw_itr->type) {
                    case ArgumentType::Store: {
                        if (itr + 1 == std::end(args)) {
                            std::cerr << "keyword argument " << name << " must have value." << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                        auto value = *(++itr);
                        parsed[remove_hyphen(kw_itr->name[0])].emplace_back(value);
                        break;
                    }
                    case ArgumentType::StoreTrue: {
                        if (kw_itr->name[0] == "--help") {
                            printHelp();
                            std::exit(EXIT_SUCCESS);
                        }
                        parsed[remove_hyphen(kw_itr->name[0])].emplace_back("");
                        break;
                    }
                    }
                }
            }

            if (_positional.empty() && itr != std::end(args)) {
                std::cerr << "no positional arguments is set. but some positional arguments passed." << std::endl;
                std::exit(EXIT_FAILURE);
            }

            auto passed_positonal_count = static_cast<std::size_t>(std::end(args) - itr);
            if (_positional.size() != passed_positonal_count) {
                std::cerr << "required positional arguments is " << _positional.size() << ". but " << passed_positonal_count << " was passed." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            for (const auto& i : _positional) {
                parsed[remove_hyphen(i.name[0])].emplace_back(*(itr++));
            }

            return Arguments(parsed, program, args);
        }

    public:
        void addArgument(const std::vector<std::string>& name, const std::string& help, ArgumentType type = ArgumentType::Store) {
            if (name.empty()) return;

            _argument_info info;

            info.name = name;
            info.help = help;
            info.type = type;

            if (name[0][0] == '-') {
                _keyword.emplace_back(info);
            } else {
                _positional.emplace_back(info);
            }
        }

    public:
        std::string help() const {
            std::stringstream ss;
            ss << "usage: " << _program;
            if (!_keyword.empty()) {
                ss << " [OPTIONS]";
                if (_keyword.size() > 1) {
                    ss << "...";
                }
            }
            for (const auto& p : _positional) {
                auto n = p.name[0];
                std::transform(std::begin(n), std::end(n), std::begin(n), ::toupper);
                ss << " " << n;
            }

            ss << "\n\n";

            ss << _description << "\n\n";

            if (!_keyword.empty()) {
                ss << "keyword arguments:\n";

                std::size_t most_long_length = 0;
                std::vector<std::tuple<std::string, std::string>> name_helps;
                name_helps.reserve(_keyword.size());
                for (const auto& k : _keyword) {
                    std::string name;
                    for (const auto& n : k.name) {
                        name += n;
                        if (k.type == ArgumentType::Store) {
                            name += " VALUE";
                        }
                        name += ", ";
                    }
                    name.pop_back();
                    name.pop_back();
                    name_helps.emplace_back(name, k.help);

                    if (name.size() > most_long_length) {
                        most_long_length = name.size();
                    }
                }

                for (const auto& nh : name_helps) {
                    std::string name, help;
                    std::tie(name, help) = nh;

                    ss << "  " << std::setw(static_cast<int>(most_long_length)) << std::left << name << " : " << help << "\n";
                }
                ss << "\n";
            }

            if (!_positional.empty()) {
                ss << "positional arguments:\n";

                std::size_t most_long_length = 0;
                std::vector<std::tuple<std::string, std::string>> name_helps;
                name_helps.reserve(_positional.size());
                for (const auto& p : _positional) {
                    std::string name;
                    for (const auto& n : p.name) {
                        name += n;
                        name += ", ";
                    }
                    name.pop_back();
                    name.pop_back();
                    name_helps.emplace_back(name, p.help);

                    if (name.size() > most_long_length) {
                        most_long_length = name.size();
                    }
                }

                for (const auto& nh : name_helps) {
                    std::string name, help;
                    std::tie(name, help) = nh;

                    ss << "  " << std::setw(static_cast<int>(most_long_length)) << std::left << name << " : " << help << "\n";
                }
                ss << "\n";
            }

            if (!_epilog.empty()) {
                ss << _epilog << "\n";
            }
            ss << std::flush;
            return ss.str();
        }
        void printHelp(std::ostream& os = std::cout) const {
            os << help() << std::flush;
        }
    };
} // namespace argparse

#endif //PYLIKE_UTILITY_ARGPARSE_HPP
