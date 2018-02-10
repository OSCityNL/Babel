#pragma once
#include "ofMain.h"
#include "Tweener.h"
#include "ofxSvg.h"
#include "ofxFontStash.h"
#include "Bone.h"

#define STATIC_RADIUS 2.0
#define STATIC_NUMFRAMES 200
#define REACTIVATE_POINTS_MAXRADIUS 2000

class Geometry {

    public:
        // CONSTRUCTOR
        Geometry(ofVec3f point);
        Geometry( ofVec3f p1, ofVec3f p2, ofColor c1, ofColor c2); // used to make new lines with vertex color
        Geometry(string table, string geomType, string coords, int time, ofColor sc, ofColor fc); // input vector of points

        // METHODS
        void setDefaultProperties();
        void update();
        void update(ofVec3f cam); // gives cam position to enable frontfacing
        void draw();
        vector<ofPoint> getCoordsFromString(string s);
        void checkRenderer();
        void resetRequestedRenderer();
        void requestRenderer(string type);  // change renderer
        void animateTo(map <string,float> targets, float d); // targetproperties/value and duration
        void stopAnimation();
        void animateToOriginalPosition(float duration, map<string,float> targets);
        void loadImage(string img);
        ofVec3f camToRotation();
        void brightenColor();
        void facingDrawBegin();
        void facingDrawEnd();

        void requestStaticRenderer();

        // --- boneSpiral functions
        void attachToBones( vector<Bone>* bones );
        void updateBoneSpiral();
        void attachToBone(int index);
        bool attachToNewBone(float r); // return 0 for not found, 1 for found and connected
        float distanceToBone(Bone* b);
        void resetSpiralData();
        void resetSpiralData(Bone* b);

        // PROPERTIES
        ofVec3f position; // position of pivot of geometry
        ofVec3f positionOrig;
        ofVec3f rotation; // rotation around x,y,z in degs
        ofVec3f rotationOrig;
        ofVec3f velocity;
        ofVec3f floatRotation;

        string type; // type of geometry: point, polygon, linestring
        string table;
        string label;
        int time; // unix epoch
        bool disabled;
        bool invalid;
        bool reactivated;
        float camDistance;
        float camDistanceAlphaFactor;


        ofColor strokeColor;
        ofColor fillColor;
        float strokeSize;
        float length; // length of line example in percent (0-1.0)
        int alpha;
        int radius;

        float svgSize;
        float svgAlpha;
        float imageSize;
        int imageAlpha;
        float labelSize;
        bool labelFacing;
        ofColor labelColor;
        ofColor lineColorStart;
        ofColor lineColorEnd;
        bool changed; // set this flag when the geometry is transformed

        bool facing;
        ofVec3f camPosition;

        vector <ofVec3f> points;
        map <string, string> data;

        int framesStatic; // count number of frames the geom is in the same place

         // renderer
        string requestedRenderer; // to iniatiate a change of renderer
        string renderer; // static (vbo) / dynamic (direct)

        // animation
        vector<tweenTarget> tweenTargets;
        Tweener tweener;

        ofPolyline polyline; // draw linestrings and polygons
        ofMesh mesh; // also use for point

        ofImage image;
        ofImage* imageRef;
        ofxSVG* svg;

        ofxFontStash* fontRenderer; // reference to main fontrenderer in babel

        // --- boneSpiral properties
        bool boneSpiralAnimation;
        int boneSpiralDirection;
        string boneSpiralStatus;
        bool boneSpiralOnBone; // is on bone (nearest point on bone)
        float boneSpiralOrbitRotationSpeedFactor;
        float boneSpiralOrbitAlongSpeedFactor;
        float boneSpiralOrbitFlySpeedFactor;
        float boneSpiralOrbitRadius; // radius to go in orbit
        float boneSpiralOrbitRadiusVariationFactor; // velocity multiplier
        float boneSpiralOrbitRadiusVariationPeriod; // in frames
        float boneSpiralOrbitRadiusVariationInc; // in rad per frame
        float boneSpiralOrbitRadiusVariationPosition; // from 0 to PI

        vector <Bone>* bones; // reference to all bones in world
        int boneIndex; // attached to this bone

        // SETTERS
        void setPosition(ofVec3f p);
        void setPositionOrig(ofVec3f p);
        void setPositionOrigZ(float z);
        void setRotation(ofVec3f r);
        void setRotationRad(ofVec3f r);
        void setFillColor(int r,int g,int b, int a);
        void setData(map<string,string> d);
        void setRenderer(string r);
        void setRadius(int r);
        void setFloatRotation(ofVec3f v);
        void setSvg(ofxSVG* svg);
        void setSvg(ofxSVG* svg, float svgSize);
        void setFacing();
        void setLabel(string t, ofxFontStash* fr);
        void setLabel(string t, float s, ofxFontStash* fr);
        void setLabel(string t, float s, ofColor c, ofxFontStash* fr);
        void setImageRef(ofImage* img);
        void setColor(ofColor c);
        void setColorOnValue(float val, float valMin, float valMax, ofColor cMin, ofColor cMax);

        // GETTERS
        string getRenderer();
        string getRequestedRenderer(); // geometry requests to its parent a other renderer
        ofVec3f getPosition();
        ofColor getFillColor();
        int getRadius();
        int getAlpha();
        ofFloatColor getFloatFillColor();
        ofFloatColor getFloatStrokeColor();

        // --- end boneSpiral properties

    private:
        //

};
