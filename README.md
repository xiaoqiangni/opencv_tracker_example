# Intro
opencv contrib contains some trackers to do object track.
This example is used to test opencv contrib trackers.

# Prerequest
* opencv with contrib installed, instructions can be follow https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
* c++ compiler
* cmake and make tools

# How to run
First checkout then:
```
mkdir build
cd build
cmake ..
make 
./tracker  1
```
use mouse to select ROI to track and press enter. Press 'c' will try another frame without selecting ROI.

press "q" to quit.

# Note for GOTURN
When choose GOTURN as
```
./track 6
```
It will need goturn.prototxt and goturn.caffemodel files to initialize internal NN.

You can train by yourself or download one form https://github.com/Mogball/goturn-files

and put these files with executable.
