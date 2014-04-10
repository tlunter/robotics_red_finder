# Robotics Red Finder

## Reasoning for repository

A class project required us to be able to track and follow a small car driven/dragged by our professor.
We experienced in consistent results when attempting to use only Ping/SODAR/SONAR sensors, and decided to get a bit more technical.
Using a webcam, we wanted to take a frame and find the small car, and report to an Arduino the current location.
Given the car's red color, this proved simple to separate from the surrounding colors.
After finding the car, we would report over serial the current direction of the car in a simplified manner.

## Programs

### camera\_capture

This will capture the current picture from the webcam and display it in a window.
It won't due any image manipulation at all.

### red\_capture

This will capture the current picture from the webcam and highlight all the red-ish colors as white, and everything else as black.
This is done by converting the image to HSV and targetting select ranges of each.
The example frame is then sent to the window.

### red\_finder

This will operate similar to red\_capture, but does not display the current frame in a picture.
It will take the mean of all the white pixels x-values and write to an open serial port when the location of the mean changes.
Possible locations are None, Left, Center and Right.
Instead of sending as soon as the frame is seen to change, it takes a majority of N frames to be sent as a change.
To account for slow framerate, we only have a history size of 3.
The values sent over serial were determined because they appear as ASCII values.

## Tools Used

We used a small subset of OpenCV functions.
Otherwise, the code is all simple math-based practical programming.
