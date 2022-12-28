#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>

#define BT_SET_VARIABLE_SETTING(settings, in) settings.set(#in, in);
#define BT_VARIABLE_INIT_BY_SETTING(settings, out) settings.get(#out, out);

namespace BT {
template <typename T>
struct is_std_string : std::integral_constant<bool, std::is_same_v<std::string, typename std::remove_cv<T>::type>> {};

template <typename T>
struct is_const_char_pointer : std::integral_constant<bool,
                                                      std::is_same_v<const char*, typename std::remove_cv<T>::type>> {};

class NodeSettings {
public:
    NodeSettings() {}
    NodeSettings(NodeSettings&& other) {
        *this = std::move(other);
    }

    NodeSettings& operator=(NodeSettings&& other) {
        m_integer_setting = std::move(other.m_integer_setting);
        m_double_setting  = std::move(other.m_double_setting);
        m_string_setting  = std::move(other.m_string_setting);
        return *this;
    }

    template <typename T>
    void set(const std::string& key, T&& value) {
        if constexpr (std::is_integral_v<typename std::remove_reference_t<T>> || std::is_enum_v<typename std::remove_reference_t<T>>) {
            m_integer_setting[key] = static_cast<int64_t>(value);
        }

        if constexpr (std::is_floating_point_v<typename std::remove_reference_t<T>>) {
            m_double_setting[key] = static_cast<double>(value);
        }

        if constexpr (is_std_string<typename std::remove_reference_t<T>>::value) {
            m_string_setting[key] = std::move(value);
        }

        if constexpr (is_const_char_pointer<typename std::remove_reference_t<T>>::value) {
            m_string_setting[key] = value;
        }
    }

    template <typename T>
    void get(const std::string& key, T& out) {
        if constexpr (std::is_integral_v<T> || std::is_enum_v<T>) {
            auto iter = m_integer_setting.find(key);
            if (iter != m_integer_setting.end()) {
                out = static_cast<T>(iter->second);
            }
        }

        if constexpr (std::is_floating_point_v<T>) {
            auto iter = m_double_setting.find(key);
            if (iter != m_double_setting.end()) {
                out = static_cast<T>(iter->second);
            }
        }

        if constexpr (is_std_string<T>::value) {
            auto iter = m_string_setting.find(key);
            if (iter != m_string_setting.end()) {
                out = std::move(iter->second);
            }
        }
    }

public:
    std::unordered_map<std::string, int64_t>     m_integer_setting;  // 整形，包含：bool、int、enum
    std::unordered_map<std::string, double>      m_double_setting;   // 浮点型
    std::unordered_map<std::string, std::string> m_string_setting;   // 字符串
};
}  // namespace BT