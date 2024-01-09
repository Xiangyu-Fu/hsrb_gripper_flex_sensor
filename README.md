# HSRB gripper flex sensor repo

## Wire connection
```
Green --> 21
Yellow --> 22
Black --> GND
Brown --> 3V3
```

### Note
Don't put the two sensors too close to each other, otherwise the readings will be affected.


## HSRB

I put the `uart_reader.py` in` /home/hsr-hmi/ros/workspaces/hsrb_ws/src/ros_skin_node/scripts/uart_reader.py`.

How to run
```bash
cd ros/workspaces/hsrb_ws
source devel/setup.bash
rosrun ros_skin_node uart_reader.py
```

In local computer
```
hsrb_mode
rostopic echo /spike_counts
```
