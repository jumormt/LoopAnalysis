//
// Created by chengxiao on 2/10/22.
//

#ifndef LOOPANALYSIS_INTRALOOPFLATTENHANDLER_H
#define LOOPANALYSIS_INTRALOOPFLATTENHANDLER_H

#include "LoopFlattenHandler.h"

namespace SVF {
class IntraLoopFlattenHandler : public LoopFlattenHandler {
public:
    IntraLoopFlattenHandler() = default;

    ~IntraLoopFlattenHandler() = default;

    void runOnModule(SVFModule *svfModule) override;
}; // end class IntraLoopFlattenHandler
} // end namespace SVF


#endif //LOOPANALYSIS_INTRALOOPFLATTENHANDLER_H
