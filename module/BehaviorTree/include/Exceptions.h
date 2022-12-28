#pragma once
#include <stdexcept>

namespace BT {
class BehaviorTreeException : public std::exception {
public:
    BehaviorTreeException(const std::string& message)
        : _message(message) {}

    BehaviorTreeException(std::string&& message)
        : _message(std::move(message)) {}

    const char* what() const noexcept {
        return _message.c_str();
    }

private:
    std::string _message;
};
/// @brief 逻辑错误
class LogicError : public BehaviorTreeException {
public:
    LogicError(const std::string& message)
        : BehaviorTreeException(message) {}

    LogicError(std::string&& message)
        : BehaviorTreeException(std::move(message)) {}
};
/// @brief 运行时错误
class RuntimeError : public BehaviorTreeException {
public:
    RuntimeError(const std::string& message)
        : BehaviorTreeException(message) {}

    RuntimeError(std::string&& message)
        : BehaviorTreeException(std::move(message)) {}
};
};  // namespace BT
