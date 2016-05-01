//
// Created by indrek on 1.05.2016.
//

#ifndef LIVEOV7670_SETUP_H
#define LIVEOV7670_SETUP_H



/*
 *  BUFFERED_CAMERA = 1
 *  Use camera class that reads line into buffer and then processes the data during blank lines
 *
 *  BUFFERED_CAMERA = 0
 *  Use camera class that processes data pixel by pixel
 *  VGA can not be used with line buffer since there is no
 *  time (no blank lines) to do something useful with a buffered line
 *
 */

#define BUFFERED_CAMERA 0



void initializeScreenAndCamera();
void processFrame();



#endif //LIVEOV7670_SETUP_H



