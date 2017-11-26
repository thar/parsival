#ifndef ${movement["name"]}_h
#define ${movement["name"]}_h

#include "Movement.h"
#include "TimedPose.h"
#include "DirectPose.h"
#include "StaticTimePose.h"
#include "JointControllerInterface.h"

<%!
    def create_pose_initialization(pose, position):
        if pose["type"] == 'TimedPose':
            return 'pose%d(jc, poseAngles%d, %d)' % (position, position, pose["time"])
        elif pose["type"] == 'DirectPose':
            return 'pose%d(jc, poseAngles%d)' % (position, position)
        elif pose["type"] == 'StaticTimePose':
            return 'pose%d(%d)' % (position, pose["time"])
%>

class ${movement["name"]} : public Movement {
public:
    ${movement["name"]}(JointControllerInterface& jc) :
        % for i in range(len(movement["poses"])):
            ${create_pose_initialization(movement["poses"][i], i)}
            % if i != (len(movement["poses"])-1):
            ,
            %endif
        % endfor
    {
% for i in range(len(movement["poses"]) - 1):
        pose${i}.setNextPose(&pose${i+1});
% endfor
    }

    Pose * getInitialPose() {
        return &pose1;
    }
    
protected:
private:
% for i in range(len(movement["poses"])):
    % if movement["poses"][i]["type"] != 'StaticTimePose':
    ${movement["poses"][i]["type"]} pose${i};
    % else:
    ${movement["poses"][i]["type"]} pose${i};
    % endif
% endfor

% for i in range(len(movement["poses"])):
    % if movement["poses"][i]["type"] != 'StaticTimePose':
    static const int poseAngles${i}[];
    % endif
% endfor
    
};
% for i in range(len(movement["poses"])):
    % if movement["poses"][i]["type"] != 'StaticTimePose':
const int ${movement["name"]}::poseAngles${i}[] = ${movement["poses"][i]["positions"]};
    % endif
% endfor

#endif /* ${movement["name"]}_h */
