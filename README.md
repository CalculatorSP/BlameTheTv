#BlameTheTv
Analyze lag in video games by processing high framerate video. 

This program can measure the time between an LED turning off as soon as a button is pressed, and the end of the TV frame where a first-frame action is drawn. This is a true measure of "input lag" for a game and TV. Several measurements should be taken since the amount of lag will differ depending on when during the TV frame the button is pressed.

###To build this program:
1. Install Visual Studio 2013 (https://www.microsoft.com/en-us/download/details.aspx?id=44914)
2. Install OpenCV 3.1 (http://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.1.0/opencv-3.1.0.exe/download)
3. Configure OpenCV 3.1 with Visual Studio 2013 (https://www.youtube.com/watch?v=tHX3MLzwF6Q)
4. Tweak to your specifications, build, & enjoy

###To adopt this program to your LED & recording:
1. Open your recording in your favorite video player
2. Take note of the area of the screen that the LED occupies throughout the video
3. Save a screenshot of the screen
  1. In VLC, use Video -> Take Snapshot
  2. In MPC, use File -> Save Image
4. Use your favorite image editor (MS Paint works fine) and determine the top left of the rectangular area you wish to detect the LED in, and the width and height of the rectangle
5. Update the TOP_LEFT_X, TOP_LEFT_Y, WIDTH, and HEIGHT values in the LED_ROI
6. Update the color in the detectLed function (0 = BLUE, 1 = GREEN, 2 = RED)
7. Adjust the threshold values in detectLed as needed (this may require some testing)

###How to use this program:
1. Build and start the program, providing the path to the HFR video you wish to measure as the first argument
2. The program should automatically advance to the first frame before the LED turns off
3. Press forward once to verify that the LED turns off in the next frame. If a false positive was detected, use DELETE to advance to the next frame before the LED turns off.
4. Use the left arrow key to advance the video frame by frame until the last frame where the TV frame is drawn while the first frame action is performed
5. Press Enter to record the number of frames between the frame before the LED turning off and the last frame of the TV frame being drawn. The program will automatically advance to the next frame before the LED turns off.
6. Repeat steps 3-5 until a sufficient amount of measurements have been taken or you reach the end of the video. The measurements will be saved in a CSV file in the same directory as the video provided as input.
