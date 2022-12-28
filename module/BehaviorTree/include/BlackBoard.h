#pragma once

#include <cstdint>
#include <memory>
#include <any>
#include <string>
#include <unordered_map>

namespace BT {
class BlackBoard {
public:
    using Ptr = std::shared_ptr<BlackBoard>;

    BlackBoard() {}
    ~BlackBoard() {}

    static Ptr create() {
        return std::shared_ptr<BlackBoard>(new BlackBoard());
    }

    template <typename T>
    void set(const std::string& key, const T& in) {
        m_any_collection[key] = in;
    }

    template <typename T>
    void set(const std::string& key, T&& in) {
        m_any_collection[key] = std::move(in);
    }

    template <typename T>
    bool get(const std::string& key, T& out) {
        auto iter = m_any_collection.find(key);
        if (iter != m_any_collection.end()) {
            if constexpr (std::is_same_v<std::any, std::remove_cv_t<T>>) {
                if (iter->second.has_value()) {
                    out = iter->second;
                    return true;
                }
            }
            else {
                auto p = std::any_cast<T>(&(iter->second));
                if (p) {
                    out = *p;
                    return true;
                }
            }
        }
        return false;
    }

    template <typename T>
    bool getWithMove(const std::string& key, T& out) {
        auto iter = m_any_collection.find(key);
        if (iter != m_any_collection.end()) {
            if constexpr (std::is_same_v<std::any, std::remove_cv_t<T>>) {
                if (iter->second.has_value()) {
                    out = std::move(iter->second);
                    return true;
                }
            }
            else {
                auto p = std::any_cast<T>(&(iter->second));
                if (p) {
                    out = std::move(*p);
                    return true;
                }
            }
        }
        return false;
    }

    const std::any* getAny(const std::string& key) const {
        auto iter = m_any_collection.find(key);
        return iter == m_any_collection.end() ? nullptr : &(iter->second);
    }

    std::any* getAny(const std::string& key) {
        auto iter = m_any_collection.find(key);
        return iter == m_any_collection.end() ? nullptr : &(iter->second);
    }

    void remove(const std::string& key) {
        m_any_collection.erase(key);
    }

    void clear() {
        m_any_collection.clear();
    }

private:
    std::unordered_map<std::string, std::any> m_any_collection;
};
};  // namespace BT
