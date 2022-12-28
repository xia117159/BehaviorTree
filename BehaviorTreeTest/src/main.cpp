#include <iostream>
#include <utility>
#include <sys/time.h>
#include <thread>
using namespace std::literals;

#include "BehaviorTree/include/BehaviorTreeFactory.h"
#include "BehaviorTree/include/BlackBoard.h"
#include "BehaviorTree/include/NodeSetting.h"
#include "BehaviorTree/include/utils/BehaviorTreeUtils.h"

using namespace std;
int main(int argc, char* argv[]) {

    BT::BlackBoard::Ptr blackBoard(new BT::BlackBoard());
    try {
        auto tree = BT::BehaviorTreeFactory::createBehaviorTreeFromXml("/mnt/D/mycode/BehaviorTree/asset/AI004.xml", blackBoard);

        while (true) {
            std::this_thread::sleep_for(50ms);
            auto status = tree.update(0.05);
            if (BT::NodeStatus::SUCCESS == status) {
                std::cout << "------------------------ 运行成功 ------------------------" << std::endl;
                break;
            }
            else if (BT::NodeStatus::FAILURE == status) {
                std::cout << "------------------------ 运行失败 ------------------------" << std::endl;
                break;
            }
        }
    }
    catch (const std::exception& ex) {
        cout << ex.what() << endl;
    }

    return 0;
}