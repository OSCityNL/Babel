#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1800, 1024, OF_WINDOW); // 780 // 3840, 2160
	ofRunApp(new testApp());
}
