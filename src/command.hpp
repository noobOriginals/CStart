#include <iostream>
#include <cstdarg>
#include <string>
#include <vector>

class Command {
public:
    Command() = delete;
    Command(const std::string& name) : name(name) {}
    std::string build(const std::string& arguments) { return name + "(" + arguments + ")"; }
    std::string build(const char* format, ...) {
        va_list args;
        va_start(args, format);
        std::string f = format, str = "";
        size_t param = f.find_first_of('%');
        while (param < f.size()) {
            str += f.substr(0, param);
            switch (f[param + 1]) {
                case 's': str += va_arg(args, const char*); break;
                case 'd': case 'i': str += std::to_string(va_arg(args, int));
                default: std::cerr << "ERROR at Command::build(const char*, ...): invalid format %" << f[param + 1] << "\n"; return "";
            }
            f = f.substr(param + 2);
            param = f.find_first_of('%');
        }
        va_end(args);
        return build(str);
    }

private:
    std::string name;
};

class CommandGroup {
public:
    CommandGroup() = default;
    CommandGroup(const std::vector<std::string>& commands) : commands(commands) {}
    void addCommand(const std::string& command) { commands.push_back(command); }
    size_t size() const { return commands.size(); }
    std::string& operator[](size_t i) { return commands[i]; }
    const std::string& operator[](size_t i) const { return commands[i]; }

private:
    std::vector<std::string> commands;
};

inline std::ostream& operator<<(std::ostream& out, const CommandGroup& commands) {
    for (size_t i = 0; i < commands.size() - 1; i++) { out << commands[i] << "\n"; }
    out << commands[commands.size() - 1];
    return out;
}
