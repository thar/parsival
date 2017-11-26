//
// Created by Miguel Angel Julian Aguilar on 26/11/17.
//

#ifndef PARSIVAL_JOINTINTERFACE_H
#define PARSIVAL_JOINTINTERFACE_H


class JointInterface {
public:
    virtual void setObjectivePosition(int angle, float time) = 0;
    virtual int getActualPosition() const = 0;
    virtual void refresh() = 0;
    virtual bool isRunning() const = 0;
    virtual void setActualPosition(int actualPosition) = 0;
    virtual ~JointInterface() {}
};


#endif //PARSIVAL_JOINTINTERFACE_H
