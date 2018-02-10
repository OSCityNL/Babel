#include "testApp.h"
//#include "User.h"

void testApp::setup() {


    /* ------- NOTES --------

        => dataFeeder of babel is seperate due to in between referencing (compiling fails with birefs)

     -----------------------*/

     /*//////////// SETTINGS ///////////*/

    SETTINGS_CITY = "sp";

    SETTINGS_DISABLE_MANAGER = true; // MN: totally disabled from now
    SETTINGS_DISABLE_SHOW = false;

    SETTINGS_TIMESCALE = 1.0; // 0.5; // X hours per second of app run
    SETTINGS_GRIDSIZE = 50.0;

    SETTINGS_BORDER_BOTTOM = 0;


     /*/////////////////////////////////*/

	ofSetFrameRate(60);

    // setup font renderer
    font.setup("Arial Unicode.ttf", 1.0, 1024, true, 8.0, 10.0f); // last: dpi: higher is more detail

    //// user manager ////
    /*    
    if (!SETTINGS_DISABLE_MANAGER)
    {
        manager.setFontRenderer(&font);
        manager.start();
    }
    */


    //// babel ////
    babel.init(SETTINGS_CITY);
    babel.setFontRenderer(&font);
    babel.SETTINGS_DISABLE_SHOW = SETTINGS_DISABLE_SHOW;

    babelFeederBase.loadDataDirect("sp_base.xml", &babel, SETTINGS_GRIDSIZE);

    //// babel datafeeder /////

    babelFeeder = DataFeeder(SETTINGS_CITY + "_all.xml");
    babelFeeder.setTimeScale(int(900.0 * SETTINGS_TIMESCALE)); // 3600=hour
    babelFeeder.setTimeRange(1462060800,1462406400+10000); // 1438387200, 1443657600

    babel.setupGrid( babelFeeder.bbox , SETTINGS_GRIDSIZE);


    babelFeeder.connectBabel(&babel);
    babel.cacheImages(&(babelFeeder.geometries));

    babelFeeder.start();

    // VIDEO CAPTURE
    recording = false;
    framesRecorded = 0;

    ofLogNotice("=> Setup succesful: now run!");

}

void testApp::update() {
    
    /*
    if (!SETTINGS_DISABLE_MANAGER)
        manager.update();
    */


    babelFeeder.update();
    babel.update();

}

void testApp::draw() {

    // basic graphic settings
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    glEnable( GL_POINT_SMOOTH );
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    //ofBackgroundGradient(ofColor(80,191,190), ofColor(7,104,136), OF_GRADIENT_LINEAR);
    //ofBackgroundGradient( ofColor(5,30,80), ofColor(4,15,40), OF_GRADIENT_CIRCULAR);
    // ofBackgroundGradient( ofColor(10,35,80), ofColor(0,5,20), OF_GRADIENT_CIRCULAR); // donkerblauw-zwart
    ofBackgroundGradient( ofColor(37,57,95), ofColor(25,27,35), OF_GRADIENT_CIRCULAR); // donkerblauw-zwart
    //ofBackgroundGradient( ofColor(15,45,60), ofColor(0,5,15), OF_GRADIENT_CIRCULAR); // donkerblauw-zwart
    //ofBackgroundGradient( ofColor(220,245,245), ofColor(230,230,230), OF_GRADIENT_CIRCULAR); // WIT



    babel.draw();
    babelFeeder.draw();

    /*
    if (!SETTINGS_DISABLE_MANAGER)
        manager.draw();
    */



    /* rect to cut screen */
    /*
    ofPushStyle();
    ofFill();
    ofSetColor(0,0,0,255);
    ofRect(0, ofGetHeight() - SETTINGS_BORDER_BOTTOM, ofGetWidth(), SETTINGS_BORDER_BOTTOM);
    ofPopStyle();
    */

    ofSetWindowTitle(ofToString(ofGetFrameRate())+"fps");

    // SCREEN CAPTURE
    if(recording)
    {
        ofImage outpImage;
        outpImage.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        outpImage.saveImage("sp_run_n/frame" + ofToString(framesRecorded) + ".jpg");
        framesRecorded++;
    }
}

// ----

void testApp::reset()
{
    ofLogNotice("app") << "=> App restart";

    babelFeeder.restart();
    babel.reset();
}

void testApp::keyPressed(int key) {
    // DEBUG: crash
    //Geometry u = manager.geometries["0"].at(-1);

    //ofLogNotice() << " cam: " << babel.cam.getPosition();
    //reset();

    // make screenshot

    /*
    ofLogNotice("=> Make screenshot");

    ofImage outpImage;
    outpImage.grabScreen(0,0,ofGetWidth(), ofGetHeight());
    outpImage.saveImage("screenshot_" + ofToString(ofGetUnixTime()) + ".jpg");
    */

    // SCREEN
    if(key == 'r'){
        recording = true;
        ofLogNotice("=========== START RECORDING ==========");
    }

    if(key == 'q')
    {
        recording = false;
        ofLogNotice("=========== STOP RECORDING ==========");
    }


 }

// ----

