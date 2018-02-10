#pragma once
#include "ofMain.h"
#include "ofxTween.h"

class ViewState
{
    public:
        ViewState(int frame, string action, int duration, vector <ofVec3f> positions, map <string,float> params);

        int frame;
        string action;
        int duration;
        vector <ofVec3f> positions;
        map <string, float> params;

};

// ----

class ViewControl
{
    public:
        // CONSTRUCTORS
        ViewControl();
        ViewControl(ofEasyCam* cam, vector <ViewState> t);

        // PROPERTIES
        ofEasyCam* camera;
        ofxTween camTween; // 0 = CX, 1 = CY, 2 = CZ
        ofxEasingExpo easingExpo;

        vector <ViewState> timeline;


        float camRotationSpeed;

        ofVec3f camTargetPosition;
        ofVec3f camTargetLookAt;
        ofVec3f camStartPosition;
        ofVec3f camStartLookAt;
        ofVec3f camDeltaPosition;
        ofVec3f camDeltaLookAt;

        ofVec3f camStartUp;
        ofVec3f camTargetUp;
        ofVec3f camDeltaUp;
        ofVec3f camUp;


        string camStatus; // static, animating
        string showStatus; // stopped, running
        int showFrame; // current frame of show
        int activeKeyFrame; // index

        float camDistance;
        float camRotation;


        // METHODS
        void update();
        void camBegin();
        void camEnd();
        void updateCam();
        void animateTo(ofVec3f p, ofVec3f t, ofVec3f u, float duration);
        void setView(int id);
        float getCamAngle();
        void setCamAngle(float ang);
        void printCamStats();
        void rotateCamRight(float deg);
        void rotateCamLeft(float deg);

        void startShow();
        void updateShow();
        void checkTimeline();

        // SET

        // GET

    private:
};
