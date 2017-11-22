#ifndef Movement_h
#define Movement_h

#include "Pose.h"

class Movement {
public:
    virtual ~Movement() {}
    virtual Pose * getInitialPose() = 0;
    
protected:
    
private:
};

#endif /* Movement_h */
