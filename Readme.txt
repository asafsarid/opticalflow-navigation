Installation Instructions:
1. Install OpenCV 3.1.0 with WITH_QT=OFF. (http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/)
2. Install QT 5.6.0 (Qt Creator 3.6.1).
3. If you are using virtualbox - disable 3D Acceleration.

Compile:
qmake Feedback.pro
make all

Optional Flags:
qmake Feedback.pro SONAR=SONAR_ACTIVE VIDEO=VIDEO_ACTIVE

Run:
sudo ./Feedback

NOTE: APM should be stable when starting program!
