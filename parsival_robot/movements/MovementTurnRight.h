#ifndef MovementTurnRight_h
#define MovementTurnRight_h

#include "../Movement.h"
#include "../TimedPose.h"
#include "../DirectPose.h"
#include "../StaticTimePose.h"
#include "../JointControllerInterface.h"

class MovementTurnRight : public Movement {
public:
    MovementTurnRight(JointControllerInterface& jc) :
            pose1(jc, poseAngles1, 100),
            pose2(jc, poseAngles2, 100),
            pose3(jc, poseAngles3, 100),
            pose4(jc, poseAngles4),
            pose5(100) {
        pose1.setNextPose(&pose2);
        pose2.setNextPose(&pose3);
        pose3.setNextPose(&pose4);
        pose4.setNextPose(&pose5);
    }

    Pose * getInitialPose() {
        return &pose1;
    }

protected:
private:
    TimedPose pose1;
    TimedPose pose2;
    TimedPose pose3;
    DirectPose pose4;
    StaticTimePose pose5;

    static const int poseAngles1[];
    static const int poseAngles2[];
    static const int poseAngles3[];
    static const int poseAngles4[];
};

const int MovementTurnRight::poseAngles1[] = {85, 71, 152, 91, 112, 60, 100, 40, 80, 0, 0, 0, 00, 40, 80, 0, 0, 0, 112, 76, 145, 93, 92, 60};
const int MovementTurnRight::poseAngles2[] = {90, 50, 157, 115, 112, 60, 90, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 113, 75, 145, 97, 93, 60};
const int MovementTurnRight::poseAngles3[] = {95,  43, 169, 110, 110, 60, 80, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 113, 75, 145, 97, 93, 60};
const int MovementTurnRight::poseAngles4[] = {100, 76, 145, 93, 100, 60, 100, 30, 80, 0, 0, 0, 100, 30, 80, 0, 0, 0, 100, 76, 145, 93, 100, 60};


#endif /* MovementTurnRight_h */
