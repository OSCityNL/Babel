#pragma once
#include "ofMain.h"
#include "Geometry.h"
#include "Babel.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
#include "ofxSvg.h"

#define CHECK_ADDGEOMETRY_PERIOD 1 // in seconds
#define DATE_ON_SCREEN_Y 750

class DataFeeder
{
    public:
        // CONSTRUCTOR
        DataFeeder();
        DataFeeder(string f); // directly load file

        // PUBLIC PROPERTIES
        vector <float> bbox;
        int timeStart; // in unix epoch
        int timeEnd;
        int feedTime; // current epoch time cursor
        int feedIndex; // index of last geometry added
        Poco::DateTime feedDateTime; // poco datetime object for easy access to time specifics
        unsigned int lastFrameTime;
        int feeding;
        int timeScale; // seconds per 1 real second

        vector <Geometry> geometries; // vector geometries
        map <string, ofxSVG> svgs; // filename (without ext) => ofxSVG

        ofTrueTypeFont font;

        // METHODS
        string loadData(string f); // returns table name of new geometry set
        void loadDataDirect(string f, Babel* b, int gridSize); // load geometry direct into babel as statical
        void connectBabel(Babel* b);
        void feedBabel(Geometry* g);
        void update();
        void draw(); // draw date in left corner
        void makeStats(); // get stats of data
        void getTimeRange();
        void nextSecondOfGeometries(); // after 1s new geometries need to be added
        vector<Geometry*> getGeometriesByTable(string t); // returns references to geometries
        void checkTimeTrigger();
        void start();
        void stop();
        void restart();
        Poco::DateTime epochToDateTime(unsigned int t);
        string getFeedTimeFormatted();
        void loadSvgs(vector<string> filenames);

        // SETTERS
        void setTimeScale(int s);
        void setTimeRange(unsigned int s, unsigned int e);

        // GETTERS

     // PRIVATE PROPERTIES
     private:
        // TODO
        Babel* babel; // reference to babel
};
