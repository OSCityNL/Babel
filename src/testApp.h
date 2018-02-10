#pragma once

#include "ofMain.h"
//#include "UserManager.h"
#include "DataFeeder.h"
#include "Babel.h"
#include "ofxImageSequenceRecorder.h"


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void reset(); // resets app
	void keyPressed(int key);

	//map<string, User> users;
    	DataFeeder babelFeeder;
    	DataFeeder babelFeederBase;
	//UserManager manager;
	Babel babel;

	ofEasyCam cam;

	// SOME SETTING FLAGS
	string SETTINGS_CITY;
	bool SETTINGS_DISABLE_MANAGER;
	float SETTINGS_TIMESCALE;
	float SETTINGS_GRIDSIZE;
	bool SETTINGS_DISABLE_SHOW;
	int SETTINGS_BORDER_BOTTOM;

	ofxFontStash font;

    // SCREEN CAPTURE
    bool recording;
    int framesRecorded;
};

