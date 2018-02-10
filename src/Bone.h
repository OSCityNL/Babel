#pragma once
#include "ofMain.h"

class Bone
{

    public:
        // CONSTRUCTOR
        Bone();
        Bone(ofVec3f pnt1, ofVec3f pnt2);

        // METHODS
        void draw();
        void setCoords(vector<ofVec3f> coords);

        // PROPERTIES
        ofVec3f p1;
        ofVec3f p2;

        float radius; // radius of bone: taken by geometry once attached

        // GETTERS

    private:
        //

};
