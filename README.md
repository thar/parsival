Parsival robot code
===================


This repository contains the code for Parsival robot.
Parsival is a humanoid robot built using the Robonova platform.
The servos controller has been changed by an Arduino Mega and a Pololu Maestro.
The different movements are received through an X-Bee connected in the Arduino board.
A X-Bee remote sends the different movements to Parsival.
The remote is based on an Arduino Leonardo board.

----------


Repository Content
-------------

The content of the different folders is the following

#### parsival_controller
This is the code for the remote controlled (Arduino Leonardo)

#### parsival_robot

This is the code for the Arduino Mega that controls Parsival servos

#### parsival_robot_pc_debug

This folder contains the code of a program that simulates the instructions executed in the Parsival code. It can be a start point for a Parsival simulator

#### parsival_movement_creator

This folder contains a python script that can be used to create the code for the different Parsival movements


----------


Create a new movement
-------------------

Lots of movements are already available for the Robonova robot, so we'll try to use them as a base for the Parsival movements.

Parsival movements:
Parsival has 3 types of movements:


TimedPose
:   This movement makes the robot to reach the wanted joints position all at the same time. The movement describes a sinusiod form (it starts slowly, reaches the max speed at the half and slows down until the final position is reached.

DirectPose
:   This movement sends the final position to the servos inmediatly and does not control their position, so the servos reach the final position the fastest they can

StaticTimePose

:   The robot does not move for a specific time.



Lets take as an example the Turn Right movement and transform it to a Parsival movement:

The movent as written in the roboBASIC lenguage is as following:
```
right_turn:
	SPEED 6
	
	MOVE G6A,  85,  71, 152,  91, 112,  60  
	MOVE G6D, 112,  76, 145,  93,  92,  60 
	MOVE G6B, 100,  40,  80,    ,    ,    ,
	MOVE G6C, 100,  40,  80,    ,    ,    ,
	WAIT

	SPEED 9
	
	MOVE G6D, 113,  75, 145,  97,  93,  60
	MOVE G6A,  90,  50, 157, 115, 112,  60 
	MOVE G6C, 105,  40,  70,    ,    ,    , 
	MOVE G6B,  90,  40,  70,    ,    ,    , 
	WAIT   

	MOVE G6D, 108,  78, 145,  98,  93,  60
	MOVE G6A,  95,  43, 169, 110, 110,  60 
	MOVE G6C, 105,  40,  70,    ,    ,    ,
	MOVE G6B,  80,  40,  70,    ,    ,    , 
	WAIT
	
RETURN
```
First we need to compose the poses strings. The poses must be the secuence of positions the joints must reach in the order from the A port to the D port. All the values must be filled. The void values can be filled with 0.
Then, the above movement is translated into the following poses:
```
Pose 1: {85,  71, 152, 91, 112, 60, 100, 40, 80, 0, 0, 0, 100,  40,  80, 0, 0, 0, 112, 76, 145, 93, 92, 60}

Pose 2: {90,  50, 157, 115, 112, 60, 90, 40, 70, 0, 0, 0, 105,  40,  70, 0, 0, 0, 113, 75, 145, 97, 93, 60}

Pose 3: {95,  43, 169, 110, 110, 60, 80, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 108, 78, 145, 98, 93, 60}
```
Now we just need to create a JSON dictionary with the movement sequence.
We must choose for each pose the type of movement we want.
**The movement name can't contain white spaces and must start with a capital letter**.

The example below contains poses of all types as a reference.
The time is expressed in milliseconds.
```
movementTurnRight = {
    "name": "MovementTurnRight",
    "poses": [
        {
            "type": "TimedPose",
            "time": 200,
            "positions": "{85,  71, 152, 91, 112, 60, 100, 40, 80, 0, 0, 0, 100,  40,  80, 0, 0, 0, 112, 76, 145, 93, 92, 60}"
        },
        {
            "type": "TimedPose",
            "time": 100,
            "positions": "{90, 50, 157, 115, 112, 60, 90, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 113, 75, 145, 97, 93, 60}"
        },
        {
            "type": "DirectPose",
            "time": 0,
            "positions": "{95,  43, 169, 110, 110, 60, 80, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 108, 78, 145, 98, 93, 60}"
        },
        {
            "type": "StaticTimePose",
            "time": 100,
            "positions": ""
        }
    ]
}
```
> **Note:**

> - It is not written in the example but the  final **standard_pose** must also be added to the movement as if a new pose is, following the same principles described above.

Now that the movement is prepared, it can be added in the file
parsival_movement_creator/movement_code_creator.py

Simply add the above code and add the dictionary to the movements_list object.
Call the generation of the movements:
python parsival_movement_creator/movement_code_creator.py
And the new movements code should be created in their correct places. Upload to Parsival and test them.

> **Note:**

> - Many movements can be obtained from the repo: https://github.com/ozfiddler/sc-robonova-archive


Launch a movement in Parsival
-------------------
Parsival accepts a byte indicating the movement number. The movement number must be lower than the number of movements available in parsival. This way, if we only have 2 movements, the accepted movement numbers are 0 and 1:

To test the movements:

* Upload the code to parsival
* Open a serial terminal that accepts sending raw data
* Send the byte 0 or the byte 1



