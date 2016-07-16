**Installation Instructions:**

1. Install OpenCV 3.1.0 with WITH_QT=OFF. (http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/).
2. Install QT 5.6.0 (Qt Creator 3.6.1).
3. If you are using virtualbox - disable 3D Acceleration.

**Compile:**
```
qmake Feedback.pro
make all
```

**Using Optional Flags:**
```
qmake Feedback.pro SONAR=SONAR_ACTIVE VIDEO=VIDEO_ACTIVE YAW=YAW_ACTIVE
make all
```

**Run:**
```
sudo ./Feedback
```


**NOTE:** APM should be stable when starting program!

Code Explanation
================

**Main Files:**

Feedback.cpp - this file contains the main function of the program. Starts the QT application (Graphs), then open sensors port (connection to APM), create another thread that runs the sampling from the sensors. Current thread runs the optical flow routine and, opens and updates the window of location graph. When the optical flow thread gets out of its loop, the program ends. 

opticalFlow.cpp - receives frames from camera, and runs our algorithm (As explained in the reports) on each couple of following frames and updates the location of the UAV on the graph. First there is need to change the perspective of the input frame according the pitch and roll of the body. Then the function split the processing effort to 4 thread, every quarter of the input frame is processed by separate thread, and then the function merges the output (displacement of last frame in pixels) of the threads. Then there is a correction caused by changes in body angles. Last frame displacement is accumulated to calculate the global position of the UAV.

Sensors.cpp - handle the communication with the APM. Every time the APM sent new samples packet, the code in function updateSensors reads it and update the global variables of the body angles, distance from ground and GPS (for reference or initial configuration).

**Other Files:**

anglecorrection.cpp - QCustomPlot graph that shows the difference between the error and the correction on both x and y axes.

eulerplot.cpp - QCustomPlot graph that shows the euler angles during the execution.

globals.h - Global/external structs and variables

mainwindow.cpp - Main QCustomPlot graph that shows the location of the UAV.

serial_port.cpp - Interface for serial communication with the APM (using MAVLINK).

