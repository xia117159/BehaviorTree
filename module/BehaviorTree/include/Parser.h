#pragma once
#include "BehaviorTree/include/BehaviorTree.h"

namespace BT {
class XmlParser {

public:
    XmlParser();
    ~XmlParser();

    XmlParser(const XmlParser& other)            = delete;
    XmlParser& operator=(const XmlParser& other) = delete;

    BehaviorTree createBehaviorTreeFromXml(const std::string& filePath, BlackBoard::Ptr blackBoard);

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> _pimpl;
};

}  // namespace BT
