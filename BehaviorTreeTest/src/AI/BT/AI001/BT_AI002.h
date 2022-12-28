#pragma once

#include <iostream>
#include "BehaviorTree/include/BehaviorTreeFactory.h"

namespace BT_AI_002 {

class BT_CUSTOM_ACTION_NODE_PROPERTY(InputNumber) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR_AND_INOUT(InputNumber) = default;
    virtual BT::NodeStatus onRunning(float delta) override {
        m_cin_data = std::numeric_limits<int>::max();
        while (m_cin_data < m_input_min || m_cin_data > m_input_max) {
            std::cout << "input range[" << m_input_min << "," << m_input_max << "]:";
            std::cin >> m_cin_data;
        }
        return BT::NodeStatus::SUCCESS;
    }

private:
    BT_PROPERTY_SETTER(int, m_input_min) = 0;
    BT_PROPERTY_SETTER(int, m_input_max) = 10;
    BT_PROPERTY_OUTPUT(int, m_cin_data)  = 0;
};

class BT_CUSTOM_ACTION_NODE_PROPERTY(InputNumberValid) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR(InputNumberValid) = default;

    virtual BT::NodeStatus onRunning(float delta) override {
        m_cin_data = std::numeric_limits<int>::max();
        std::cout << "input id[" << m_test_id << "]:";
        std::cin >> m_cin_data;
        if (m_cin_data == 0) {
            std::cout << "输入无效 id[" << m_test_id << "]" << std::endl;
            return BT::NodeStatus::FAILURE;
        }

        if (m_cin_data == 2) {
            std::cout << "继续输入 id[" << m_test_id << "]" << std::endl;
            return BT::NodeStatus::RUNNING;
        }

        std::cout << "输入完成 id[" << m_test_id << "]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    virtual void onInterrupted() override {
        std::cout << "输入被打断 id[" << m_test_id << "]" << std::endl;
    }

private:
    BT_PROPERTY_SETTER(int, m_test_id) = 0;
    int m_cin_data                     = 0;
};

class BT_CUSTOM_ACTION_NODE_PROPERTY(PrintAnything) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR_AND_INOUT(PrintAnything) = default;
    virtual BT::NodeStatus onRunning(float delta) override {
        auto pint = std::any_cast<int>(&m_print_data);
        if (pint != nullptr) {
            std::cout << m_print_prefix << *pint << std::endl;
            return BT::NodeStatus::SUCCESS;
        }

        std::cout << m_print_prefix << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

private:
    BT_PROPERTY_SETTER(std::string, m_print_prefix);
    BT_PROPERTY_INPUT(std::any, m_print_data);
};

class BT_CUSTOM_ACTION_NODE_PROPERTY(SumInput) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR_AND_INOUT(SumInput) = default;
    virtual BT::NodeStatus onRunning(float delta) override {
        m_sum = m_input_one + m_input_two + m_sum_other;
        if (m_sum > m_sum_threshold)
            return BT::NodeStatus::FAILURE;
        return BT::NodeStatus::SUCCESS;
    }

private:
    BT_PROPERTY_SETTER(int, m_sum_other)     = 0;
    BT_PROPERTY_SETTER(int, m_sum_threshold) = 1000;
    BT_PROPERTY_INPUT(int, m_input_one)      = 0;
    BT_PROPERTY_INPUT(int, m_input_two)      = 0;
    BT_PROPERTY_OUTPUT(int, m_sum)           = 0;
};

}  // namespace BT_AI_002