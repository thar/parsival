from mako.template import Template
from mako.lookup import TemplateLookup
import os


class MovementList(object):
    def __init__(self):
        self.movements = []
        script_path = os.path.dirname(os.path.realpath(__file__))
        mylookup = TemplateLookup(directories=[script_path])
        self.movement_list_template = mylookup.get_template('MovementList.mako')
        self.movement_template = mylookup.get_template('Movement.mako')
        script_path = os.path.dirname(os.path.realpath(__file__))
        self.parsival_code_path = os.path.join(script_path, '../parsival_robot')


    def add(self, movement):
        if movement in self.movements:
            print 'The movement %s already exists. Correct it' % movement
            exit(1)
        self.movements.append(movement)

    def get_movements_names(self):
        return [movement['name'] for movement in self.movements]

    def save(self):
        with open(os.path.join(self.parsival_code_path, 'MovementList.h'), 'w') as movement_list_file:
            print os.path.join(self.parsival_code_path, 'MovementList.h')
            movement_list_file.write(self.movement_list_template.render(movements=self.get_movements_names()))
        for m in self.movements:
            with open(os.path.join(self.parsival_code_path, 'movements/%s.h') % m['name'], 'w') as movement_file:
                print os.path.join(self.parsival_code_path, 'movements/%s.h') % m['name']
                movement_file.write(self.movement_template.render(movement=m))

m1 = {
    "name": "MovementTest1",
    "poses": [
        {
            "type": "TimedPose",
            "time": 100,
            "positions": "{85, 71, 152, 91, 112, 60, 100, 40, 80, 0, 0, 0, 00, 40, 80, 0, 0, 0, 112, 76, 145, 93, 92, 60}"
        },
        {
            "type": "TimedPose",
            "time": 100,
            "positions": "{90, 50, 157, 115, 112, 60, 90, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 113, 75, 145, 97, 93, 60}"
        },
        {
            "type": "TimedPose",
            "time": 100,
            "positions": "{95,  43, 169, 110, 110, 60, 80, 40, 70, 0, 0, 0, 105, 40, 70, 0, 0, 0, 113, 75, 145, 97, 93, 60}"
        },
        {
            "type": "DirectPose",
            "time": 0,
            "positions": "{100, 76, 145, 93, 100, 60, 100, 30, 80, 0, 0, 0, 100, 30, 80, 0, 0, 0, 100, 76, 145, 93, 100, 60}"
        },
        {
            "type": "StaticTimePose",
            "time": 100,
            "positions": ""
        }
    ]
}

movements_list = MovementList()
movements_list.add(m1)
movements_list.save()


