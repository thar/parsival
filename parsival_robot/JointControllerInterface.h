//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#ifndef PARSIVAL_JOINTCONTROLLERINTERFACE_H
#define PARSIVAL_JOINTCONTROLLERINTERFACE_H


class JointControllerInterface {
public:
    virtual void setJointAngles(const int * jointsAngles, int time = -1) = 0;
    virtual bool isRunning() const = 0;
    virtual void refresh() = 0;
    virtual ~JointControllerInterface() {}
};


#endif //PARSIVAL_JOINTCONTROLLERINTERFACE_H
