//
// Created by chengxiao on 2/10/22.
//

#ifndef LOOPANALYSIS_INTERLOOPFLATTENHANDLER_H
#define LOOPANALYSIS_INTERLOOPFLATTENHANDLER_H

#include "LoopFlattenHandler.h"

namespace SVF {
class InterLoopFlattenHandler : public LoopFlattenHandler {
public:
    InterLoopFlattenHandler() = default;

    ~InterLoopFlattenHandler() = default;

    void runOnModule(SVFModule *svfModule) override;
}; // end class InterLoopFlattenHandler
} // end namespace SVF


#endif //LOOPANALYSIS_INTERLOOPFLATTENHANDLER_H
