#ifndef RelaxPoseMovement_h
#define RelaxPoseMovement_h

#include "Movement.h"
#include "DirectPose.h"
#include "JointControllerInterface.h"

class RelaxPoseMovement : public Movement {
public:
    RelaxPoseMovement(JointControllerInterface& jc) : relaxPose(jc, pose) {}

    Pose * getInitialPose() {
        return &relaxPose;
    }

protected:
private:
    static const int pose[];
    DirectPose relaxPose;
    
};

const int RelaxPoseMovement::pose[] = {100, 76, 145, 93, 100, 60, 100, 30, 80, 0, 0, 0, 100, 30, 80, 0, 0, 0, 100, 76, 145, 93, 100, 60};

#endif /* RelaxPoseMovement_h */
