#include <vector>
#include <unordered_map>
#include <string>

class ArgParser {
public:
    ArgParser() = default;
    void addOption(const std::string& name, void* ptr, void(*valueParser)(const std::string&, void*)) {
        options[name] = { ptr, valueParser };
    }
    void addOptionAlias(const std::string& name, const std::string& alias) {
        auto option = options.find(name);
        if (option == options.end()) {
            return;
        }
        options[alias] = option->second;
    }
    void parse(size_t argCount, char** args) {
        std::string name = "", value = "";
        for (size_t i = 1; i < argCount; i++) {
            std::string arg(args[i]);
            if (arg[0] == '-') {
                name = arg.substr(1);
                value = "";
            } else {
                value = arg;
            }
            auto option = options.find(name);
            if (option != options.end()) {
                option->second.valueParser(value, option->second.ptr);
            }
        }
    }

private:
    struct Option { void* ptr; void(*valueParser)(const std::string&, void*); };
    std::unordered_map<std::string, Option> options;
};

namespace default_parser {
    inline void parseBool(const std::string& val, void* ptr) {
        if (val == "false") {
            *((bool*) ptr) = false;
            return;
        }
        *((bool*) ptr) = true;
    }

    inline void parseInt(const std::string& val, void* ptr) {
        *((int*) ptr) = atoi(val.c_str());
    }

    inline void parseDouble(const std::string& val, void* ptr) {
        *((double*) ptr) = atof(val.c_str());
    }

    inline void parseString(const std::string& val, void* ptr) {
       *((std::string*) ptr) =  val;
    }

    inline void parseStringList(const std::string& val, void* ptr) {
        std::string str = val;
        std::vector<std::string> v;
        size_t i = str.find_first_of(':');
        while (i < str.size()) {
            v.push_back(str.substr(0, i));
            str = str.substr(i + 1);
            i = str.find_first_of(':');
        }
        if (!str.empty()) {
            v.push_back(str);
        }
        *((std::vector<std::string>*) ptr) =  v;
    }
}
