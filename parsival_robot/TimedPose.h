#ifndef TimedPose_h
#define TimedPose_h

#include "Pose.h"
#include "JointController.h"

template <typename AngleType>
class TimedPose : public Pose {
public:
    TimedPose(JointController& jc, const int *jointsAngles, int time) : jc_(jc), jointsAngles_(jointsAngles),
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
        jc_.setPose<AngleType>(jointsAngles_, time_);
    }
    
protected:
private:
    JointController& jc_;
    const int *jointsAngles_;
    int time_;

};

#endif /* TimedPose_h */
