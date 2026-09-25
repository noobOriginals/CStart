#include <vector>
#include <unordered_map>
#include <string>

class BaseArg {
public:
    virtual ~BaseArg() {}
    virtual void parseValue(const std::string& val) = 0;
    virtual void* valuePtr() = 0;
};

template<typename T>
class Arg : public BaseArg {
public:
    Arg() = default;
    Arg(const T& defaultValue, T(*parser)(const std::string&)) : value(defaultValue), parser(parser) {}
    void parseValue(const std::string& val) override { value = parser(val); }
    void* valuePtr() override { return (void*) &value; }

private:
    T value;
    T(*parser)(const std::string&) = nullptr;
};

class ArgParser {
public:
    ArgParser() = default;
    ~ArgParser() {
        if (args.empty()) {
            return;
        }
        for (auto it = args.begin(); it != args.end(); it++) {
            if (it->second) {
                delete it->second;
            }
        }
    }
    template<typename T>
    void put(const std::string& name, const T& defaultValue, T(*inputParser)(const std::string&)) {
        BaseArg*& x = args[name];
        if (x) {
            delete x;
        }
        x = new Arg<T>(defaultValue, inputParser);
    }
    void remove(const std::string& name) {
        BaseArg*& x = args[name];
        delete x;
        x = nullptr;
    }
    template<typename T>
    T get(const std::string& name) {
        BaseArg*& x = args[name];
        if (!x) {
            return T();
        }
        return *((T*) x->valuePtr());
    }
    void parse(size_t argc, char** args) {
        for (size_t i = 1; i < argc; i++) {
            std::string arg(args[i]);
            size_t delimiter = arg.find_first_of('=');
            std::string value = "";
            if (delimiter < arg.size()) {
                value = arg.substr(delimiter + 1, arg.size() - delimiter - 1);
                arg = arg.substr(0, delimiter);
            }
            BaseArg*& x = this->args[arg];
            if (x) {
                x->parseValue(value);
            }
        }
    }

private:
    std::unordered_map<std::string, BaseArg*> args;
};

namespace default_parser {
    inline bool parseBool(const std::string& val) {
        if (val == "false") {
            return false;
        }
        return true;
    }

    inline int parseInt(const std::string& val) {
        return atoi(val.c_str());
    }

    inline double parseDouble(const std::string& val) {
        return atof(val.c_str());
    }

    inline std::string parseString(const std::string& val) {
        return val;
    }

    inline std::vector<std::string> parseStringList(const std::string& val) {
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
        return v;
    }
}
