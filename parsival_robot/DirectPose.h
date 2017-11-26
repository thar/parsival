#ifndef DirectPose_h
#define DirectPose_h

#include "Pose.h"
#include "JointControllerInterface.h"


class DirectPose : public Pose {
public:
    DirectPose(JointControllerInterface& jc, const int *jointsAngles) : jc_(jc), jointsAngles_(jointsAngles) {}
    
    void update() {
        jc_.refresh();
    }
    
    Pose * getNextPose() {
        return getInitilizedNextPose();
    }
    
    void init() {
        jc_.setJointAngles(jointsAngles_, -1);
    }
    
protected:
private:
    JointControllerInterface& jc_;
    const int *jointsAngles_;
};

#endif /* DirectPose_h */
