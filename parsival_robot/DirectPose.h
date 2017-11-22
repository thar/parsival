#ifndef DirectPose_h
#define DirectPose_h

#include "Pose.h"
#include "JointController.h"

template <typename AngleType>
class DirectPose : public Pose {
public:
    DirectPose(JointController& jc, const int *jointsAngles) : jc_(jc), jointsAngles_(jointsAngles) {}
    
    void update() {
    }
    
    Pose * getNextPose() {
        return getInitilizedNextPose();
    }
    
    void init() {
        jc_.setPose<AngleType>(jointsAngles_, -1);
    }
    
protected:
private:
    JointController& jc_;
    const int *jointsAngles_;
};

#endif /* DirectPose_h */
