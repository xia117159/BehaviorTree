#pragma once

namespace NativeProperty {
const auto All     = "All";
const auto Fields  = "Fields";
const auto Enable  = "Enable";
const auto Disable = "Disable";

// 自定的Action节点，需要被用于生成到节点工厂中
const auto ActionNode = "ActionNode";
// 自定的Action节点的设置器，在节点构造时完成设置赋值，值来自于XML
const auto PropertySetter = "PropertySetter";

const auto PropertyInput           = "PropertyInput";
const auto PropertyInputWithStatus = "PropertyInputWithStatus";
const auto PropertyOutput          = "PropertyOutput";

}  // namespace NativeProperty
