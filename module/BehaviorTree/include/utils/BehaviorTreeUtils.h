#pragma once

#include <string>
#include <cstring>
#include <vector>
#include "BehaviorTree/include/BasicTypes.h"

namespace BT {
inline bool cstChEqual(const char* a, const char* b) {
    return std::strcmp(a, b) == 0;
}
inline bool isControlNode(const char* xmlNodeName) {
    return cstChEqual(xmlNodeName, kIfThenElseNodeName) || cstChEqual(xmlNodeName, kWhileDoElseNodeName) || cstChEqual(xmlNodeName, kRandomSelectorNodeName) || cstChEqual(xmlNodeName, kReactiveSelectorNodeName) || cstChEqual(xmlNodeName, kProbabilitySelectorNodeName) || cstChEqual(xmlNodeName, kRecordSelectorNodeName) || cstChEqual(xmlNodeName, kSequenceNodeName) || cstChEqual(xmlNodeName, kSelectorNodeName) || cstChEqual(xmlNodeName, kParallelNodeName);
}
inline bool isDecoratorNode(const char* xmlNodeName) {
    return cstChEqual(xmlNodeName, kRetryNodeName) || cstChEqual(xmlNodeName, kRepeatNodeName) || cstChEqual(xmlNodeName, kKeepRunningUntilFailureNodeName) || cstChEqual(xmlNodeName, kInverterNodeName) || cstChEqual(xmlNodeName, kDelayNodeName) || cstChEqual(xmlNodeName, kForceSuccessNodeName) || cstChEqual(xmlNodeName, kForceFailureNodeName);
}

std::string trim(std::string& source_string, const std::string trim_chars);

std::vector<std::string> split(const std::string& input, const std::string& pat);

int random(int min, int max, bool has_max = false);
}  // namespace BT
