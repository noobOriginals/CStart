#include <ostream>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>

class Command {
public:
    Command() = delete;
    Command(const std::string& name) : name(name) {}
    std::string build(const std::string& arguments) { return name + "(" + arguments + ")"; }
    std::string build(const char* format, ...) {
        std::string buffer(512, 0);
        va_list args;
        va_start(args, format);
        int n = va_arg(args, int);
        snprintf(buffer.data(), 512, format, args);
        va_end(args);
        buffer.resize(buffer.find_first_of((char) 0));
        return name + "(" + buffer + ")";
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
    for (size_t i = 0; i < commands.size(); i++) { out << commands[i] << "\n"; }
    return out;
}
