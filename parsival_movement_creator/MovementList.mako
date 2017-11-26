#ifndef MovementList_h
#define MovementList_h

#include "JointControllerInterface.h"
#include "RelaxPoseMovement.h"

<%!
    lower_first_char = lambda s: s[:1].lower() + s[1:] if s else ''
%>

% for movement in movements:
#include "movements/${movement}.h"
% endfor

class MovementList {
public:
    MovementList(JointControllerInterface& jc)
    : relaxPoseMovement(jc)
% for movement in movements:
      , ${lower_first_char(movement)}(jc)
% endfor
    {
% for movement in movements:
        movementsList[${loop.index}] = &${lower_first_char(movement)};
% endfor
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
    static const unsigned int numberOfMovements = ${len(movements)};
    RelaxPoseMovement relaxPoseMovement;
% for movement in movements:
    ${movement} ${lower_first_char(movement)};
% endfor
    Movement* movementsList[numberOfMovements];
    
};

#endif /* MovementList_h */
