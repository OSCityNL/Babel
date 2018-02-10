#include "Bone.h"

Bone::Bone()
{
    radius = 0;
}

// ----

Bone::Bone(ofVec3f pnt1, ofVec3f pnt2)
{
    p1 = pnt1;
    p2 = pnt2;

    radius = 0;
}

// ----

void Bone::draw()
{
    ofPushMatrix();
    ofPushStyle();

    ofSetColor(255,0,0);
    ofLine(p1, p2);

    ofPopStyle();
    ofPopMatrix();
}

// ----

void Bone::setCoords(vector<ofVec3f> coords)
{
    p1 = coords[0];
    p2 = coords[1];
}
