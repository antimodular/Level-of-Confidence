#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include <GLFW/glfw3.h>
//#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

//    ofAppGlutWindow window;
//    window.setGlutDisplayString("rgba double samples>=4");
//    ofSetupOpenGL(&window, 1920, 1080, OF_GAME_MODE);

    /*
    ofAppGLFWWindow window;
   // window.setMultiDisplayFullscreen(true);
    window.setDoubleBuffering(true);
    window.setNumSamples(4);
    ofSetupOpenGL(&window,1024,768,OF_FULLSCREEN);
  */

    /*
    ofAppGlutWindow window;
     window.setDoubleBuffering(true);
     ofSetupOpenGL(&window,1024,768,OF_WINDOW);
    */

    // Use GLFW window to have access to multi-monitor positioning
    ofGLFWWindowSettings settings;
    settings.setSize(1080, 1920);  // Portrait HD resolution
    settings.windowMode = OF_FULLSCREEN;

    // Position window on leftmost monitor using GLFW directly
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    if(monitorCount > 0) {
        // Find the leftmost monitor
        int leftmostIndex = 0;
        int minX = 0;
        glfwGetMonitorPos(monitors[0], &minX, nullptr);

        for(int i = 1; i < monitorCount; i++) {
            int x, y;
            glfwGetMonitorPos(monitors[i], &x, &y);
            if(x < minX) {
                minX = x;
                leftmostIndex = i;
            }
        }

        // Set position for the leftmost monitor
        int mx, my;
        glfwGetMonitorPos(monitors[leftmostIndex], &mx, &my);
        settings.setPosition(ofVec2f(mx, my));
        settings.monitor = leftmostIndex;

        cout << "Found " << monitorCount << " monitors" << endl;
        cout << "Positioning on leftmost monitor (#" << leftmostIndex << ") at position: "
             << mx << ", " << my << endl;
    }

    auto window = ofCreateWindow(settings);

	// this kicks off the running of my app
	ofRunApp(new ofApp());
}
