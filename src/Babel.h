#pragma once
#include "ofMain.h"
#include "Geometry.h"
#include "GridCell.h"
#include "ofxSvg.h"
#include "ofxFontStash.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTime.h"
#include "ViewControl.h"
#include "ofxSpatialHash.h" // modified hash that handles 4f vectors with the [3] component as id

/////////// SETTINGS ////////////

#define RENDERCHECK_NUMFRAMES 20 // 100 best?
#define SOUNDTRACK_VOLUME 0.0

#define MAX_HASH_QUEUE 1000 // rebuild after X geometries are added to Babel
#define REACTIVATE_POINTS_NUM 2000

#define CLUSTERS_SCAN_RADIUS 25
#define CLUSTERS_SCAN_RADIUS_ESTIMATED_POINTS 10
#define CLUSTERS_MAX_TIME_DIFFERENCE 1
#define CLUSTERS_MIN_DAY_DIFFERENCE 3
#define CLUSTERS_MAX_DAY_DIFFERENCE 4

class Babel
{
    public:
        // CONSTRUCTOR
        Babel();

        // PUBLIC PROPERTIES
        vector<float> bbox;
        string city; // city: shz or hk
        float gridCellSize;
        int gridWidthCells;
        int gridHeightCells;
        int renderCheckWaitCount;

        vector <Geometry*> geometries; // reference to geometries
        vector <Geometry> newGeometries;
        vector <GridCell> grid; // 2d vector of gridcell objects [i = y * cellsW + x ]

          // 3d spatial hash index
        int pointGeometriesCount; // all geometries in Babel at current moment
        vector <ofVec4f> hashedPoints;
        int addedPointsCountForHash;
        map <int,Geometry*> pointsById; // map of id => pnt to geometry instance
        ofx::KDTree<ofVec4f> hash;
        ofx::KDTree<ofVec4f>::SearchResults hashResults;
        ofx::KDTree<ofVec4f>::SearchResults hashClusterResults;

        ofShader shader;

        vector <ofVec3f> vboPointPositions;
		vector <ofVec3f> vboPointSizes;
		vector <ofFloatColor> vboPointColors;
        ofVbo vboPoints;

        vector <ofVec3f> vboLinePositions;
		vector <ofFloatColor> vboLineColors;
        ofVbo vboLines;

        vector <ofVec3f> vboPolyPositions;
		vector <ofFloatColor> vboPolyColors;
		vector <ofIndexType> vboPolyIndices; // used to close polygons
        ofVbo vboPolys;

        ofxFontStash* fontRenderer;
        ofSoundPlayer soundtrack;

        map <string, ofxSVG> svgs; // svg cache

        ofEasyCam cam;
        ViewControl viewcontrol;
        vector <ViewState> timeline;

        // METHODS
        void init(string city);
        void draw();
        void update();
        void reset();
        void resetVbo();
        void resetGrid();
        void setupGrid(vector<float> b, float cellSize);
        void addGeometry(Geometry* g);
        GridCell* addToGrid(Geometry* g);
        void checkRenderers();
        void animateToOriginal(float duration, map<string,float> targets);
        void loadSvgs(vector <string> filenames); // load svgs
        //void setupFontRenderer();
        void setFontRenderer(ofxFontStash* f);
        void cacheImages(vector<Geometry>* vectorGeoms);
        void playSoundtrack();
        Poco::DateTime epochToDateTime(unsigned int t);
        void reactivateNearPoints(ofVec3f fromPoint, int numPoints);
        ofColor hourToColor(int hour);
        ofColor interpolateColor(float val, float valMin, float valMax, ofColor cMin, ofColor cMax);

        // SETTERS

        // GETTERS

        // SETTINGS: some flags
        bool SETTINGS_DISABLE_SHOW;
        // names of datasets ( with city )
        string DATASET_WEIBO;
        string DATASET_WEIBO_PATHS;
        string DATASET_WEIBO_TOPICS;
        string DATASET_TWITTER;
        string DATASET_INSTAGRAM;
        string DATASET_INSTAGRAM_PATHS;


     // PRIVATE PROPERTIES
     private:

};
