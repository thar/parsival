#ifndef MovementTurnLeft_h
#define MovementTurnLeft_h

#include "Movement.h"
#include "TimedPose.h"
#include "JointController.h"

class MovementTurnLeft : public Movement {
public:
    MovementTurnLeft(JointController& jc) :
            pose1(jc, poseAngles1, 100),
            pose2(jc, poseAngles2, 100),
            pose3(jc, poseAngles3, 100),
            pose4(jc, poseAngles4, 100) {
        pose1.setNextPose(&pose2);
        pose2.setNextPose(&pose3);
        pose3.setNextPose(&pose4);
    }

    Pose * getInitialPose() {
        return &pose1;
    }
    
protected:
private:
    TimedPose<RobonovaJoints> pose1;
    TimedPose<RobonovaJoints> pose2;
    TimedPose<RobonovaJoints> pose3;
    TimedPose<RobonovaJoints> pose4;

    static const int poseAngles1[];
    static const int poseAngles2[];
    static const int poseAngles3[];
    static const int poseAngles4[];
    
};
const int MovementTurnLeft::poseAngles1[] = {112, 76, 145, 93, 92, 60, 100, 40, 80, 0, 0, 0, 100, 40, 80, 0, 0, 0, 85, 71, 152, 91, 112, 60};
const int MovementTurnLeft::poseAngles2[] = {113, 75, 145, 97, 93, 60, 105, 40, 70, 0, 0, 0, 90, 40, 70, 0, 0, 0, 90, 50, 157, 115, 112, 60};
const int MovementTurnLeft::poseAngles3[] = {108, 78, 145, 98, 93, 60, 105, 40, 70, 0, 0, 0, 80, 40, 70, 0, 0, 0, 95, 43, 169, 110, 110, 60};
const int MovementTurnLeft::poseAngles4[] = {100, 76, 145, 93, 100, 60, 100, 30, 80, 0, 0, 0, 100, 30, 80, 0, 0, 0, 100, 76, 145, 93, 100, 60};

#endif /* MovementTurnLeft_h */
