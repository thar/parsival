#ifndef TimedPose_h
#define TimedPose_h

#include "Pose.h"
#include "JointControllerInterface.h"


class TimedPose : public Pose {
public:
    TimedPose(JointControllerInterface& jc, const int *jointsAngles, int time) : jc_(jc), jointsAngles_(jointsAngles),
                                                                        time_(time) {}
    
    void update() {
        jc_.refresh();
    }
    
    Pose * getNextPose() {
        if (!jc_.isRunning()) {
            return getInitilizedNextPose();
        } else {
            return this;
        }
    }
    
    void init() {
        jc_.setJointAngles(jointsAngles_, time_);
    }
    
protected:
private:
    JointControllerInterface& jc_;
    const int *jointsAngles_;
    int time_;

};

#endif /* TimedPose_h */
