//
// Created by chengxiao on 2/10/22.
//

#ifndef LOOPANALYSIS_LOOPFLATTENHANDLER_H
#define LOOPANALYSIS_LOOPFLATTENHANDLER_H
#include "Util/SVFModule.h"

namespace SVF {
class LoopFlattenHandler {
public:
    LoopFlattenHandler() = default;
    ~LoopFlattenHandler() = default;

    virtual void runOnModule(SVFModule *svfModule) = 0;
}; // end class LoopFlattenHandler
} // end namespace SVF

#endif //LOOPANALYSIS_LOOPFLATTENHANDLER_H
