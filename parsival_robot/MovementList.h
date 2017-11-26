#ifndef MovementList_h
#define MovementList_h

#include "JointControllerInterface.h"
#include "RelaxPoseMovement.h"
#include "movements/MovementTurnRight.h"
#include "movements/MovementTurnLeft.h"

class MovementList {
public:
    MovementList(JointControllerInterface& jc)
    : relaxPoseMovement(jc), movementTurnRight(jc), movementTurnLeft(jc) {
        movementsList[0] = &movementTurnRight;
        movementsList[1] = &movementTurnLeft;
    }
    ~MovementList() {
    }
    Movement* operator[] (int i) {
        return movementsList[i];
    }
    constexpr static int size() {
        return numberOfMovements;
    }
    Movement * getRelaxPoseMovement() {
        return &relaxPoseMovement;
    }
    
protected:
private:
    static const unsigned int numberOfMovements = 2;
    RelaxPoseMovement relaxPoseMovement;
    MovementTurnRight movementTurnRight;
    MovementTurnLeft movementTurnLeft;
    Movement* movementsList[numberOfMovements];
    
};

#endif /* MovementList_h */
