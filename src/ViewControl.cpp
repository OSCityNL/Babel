#include "ViewControl.h"
#include <iostream>

////// ViewState ////////

ViewState::ViewState(int f, string a, int d, vector <ofVec3f> ps, map <string, float> p)
{
    frame = f;
    action = a;
    duration = d;
    positions = ps;
    params = p;
}


/////////////// ViewControl /////////////

ViewControl::ViewControl()
{

}

// ----

ViewControl::ViewControl(ofEasyCam* cam, vector <ViewState> t)
{
    // CONSTRUCTOR
    camRotationSpeed = 2.0; // in deg angle
    camTween.setFrameBasedAnimation(true);

    timeline = t;
    camera = cam;

    showFrame = 0;
    camStatus = "static";
    showStatus = "stopped";

    camUp = ofVec3f(0,1,0); // y-axis is up as a start
}

// ----

void ViewControl::update()
{
    updateShow();
    updateCam();
}

// ----

void ViewControl::camBegin()
{
    // start draw
    camera->begin();
}

// ----

void ViewControl::camEnd()
{
    // end draw
    camera->end();
}

// ----

void ViewControl::startShow()
{
    ofLogNotice("ViewControl::startShow");

    showStatus = "running";
}

// ----

void ViewControl::updateShow()
{
    if (showStatus == "running")
    {
        //ofLogNotice("ViewControl::updateShow") << "Show is running: check timeline";

        checkTimeline();

        showFrame++;

    }
}

// ----

void ViewControl::checkTimeline()
{
    // cycle through timeline to check if the current frame is a keyframe to do something

    //ofLogNotice() << " show frame " << showFrame;

    for (int c = 0; c < timeline.size(); c++)
    {
        if ( timeline[c].frame == showFrame )
        {
            ofLogNotice("ViewControl::checkTimeline") << " Keyframe " <<  timeline[c].frame << " action: " << timeline[c].action;

            // do action
            if (timeline[c].action == "goto")
            {
                // just set position of camera
                camera->setPosition( timeline[c].positions[0] );
                camera->setTarget( timeline[c].positions[1] );
            }
            else if (timeline[c].action == "animateTo")
            {
                // animate to position
                animateTo(timeline[c].positions[0], timeline[c].positions[1], timeline[c].positions[2], timeline[c].duration);
            }

            activeKeyFrame = c;
            break;
        }
    }

    // check last keyframe:
    // reset show
    if (showFrame == timeline[timeline.size()-1].frame + timeline[timeline.size()-1].duration)
    {
        // loop
        showFrame = -1;
    }

}

// ----

void ViewControl::animateTo(ofVec3f p, ofVec3f t, ofVec3f u, float duration)
{

    camTween.setParameters(easingExpo,ofxTween::easeOut, 0.0, 1.0 , duration, 0.0);
    camTargetPosition = p;
    camTargetLookAt = t;
    camTargetUp = u;

    camStartPosition = camera->getPosition();
    camStartLookAt = camera->getTarget().getPosition();
    camDeltaPosition = camTargetPosition - camStartPosition;
    camDeltaLookAt = camTargetLookAt - camStartLookAt;

    camStartUp = camUp;
    camDeltaUp = camTargetUp - camStartUp;

    //ofLogNotice() << "ViewControl::animateTo()" << "camDeltaPosition: " << camDeltaPosition << " camDeltaLookAt " << camDeltaLookAt << " camDeltaUp " << camDeltaUp;

    camStatus = "animating";

}

void ViewControl::updateCam()
{
    if(camStatus == "animating")
    {
            float p = camTween.update();

            // update cam position according to tween

            camera->setPosition( camDeltaPosition * p + camStartPosition );
            camera->setTarget( camDeltaLookAt * p + camStartLookAt);
            camera->lookAt( camDeltaLookAt * p + camStartLookAt, camDeltaUp * p + camStartUp ); // DOES WEIRD THINGS

            //ofLogNotice() << "ViewControl::updateCam(): " << p << " new position " << camera->getPosition() << " target " << camera->getTarget().getPosition();

            //camera->setPosition( ofVec3f( camTween.update(), camTween.getTarget(1), camTween.getTarget(2)) );
            //camera->lookAt(ofVec3f( camTween.getTarget(3), camTween.getTarget(4), camTween.getTarget(5) ), ofVec3f(0,0,1));

            // check if completed
            if( int (camTargetPosition.x) == int (camera->getPosition().x) && int (camTargetPosition.y) == int (camera->getPosition().y) && int (camTargetPosition.z) == int (camera->getPosition().z) &&  int(camTargetLookAt.z) == int (camera->getTarget().getZ()) && int(camTargetLookAt.y) == int (camera->getTarget().getY()) && int(camTargetLookAt.z) == int (camera->getTarget().getZ()) )
            {
                cout << "camTween completed" << endl;
                camStatus = "static";

                camUp = camTargetUp;
            }
    }
    else
    {
        // update from mouse input?
        //cout << "cam target [" << camera->getLookAtDir() << " ]" << endl;
        //cout << "cam postion [" << camera->getPosition() << "]" << endl;
        //setCamAngle(getCamAngle());
    }
}

// ----

float ViewControl::getCamAngle()
{
    float a = ofRadToDeg( atan(  abs( camera->getPosition().y - camera->getTarget().getY()) / abs(camera->getPosition().x - camera->getTarget().getX())));

    // Q2
    if ( (camera->getPosition().x - camera->getTarget().getX()) < 0 && (camera->getPosition().y - camera->getTarget().getY()) > 0)
    {
        a = 180 - a;
    }
    // Q3
    if ( (camera->getPosition().x - camera->getTarget().getX()) < 0 && (camera->getPosition().y - camera->getTarget().getY()) < 0)
    {
        a = 180 + a;
    }
    // Q4
    if ( (camera->getPosition().x - camera->getTarget().getX()) > 0 && (camera->getPosition().y - camera->getTarget().getY()) < 0)
    {
        a = 360 - a;
    }

    return a;
}

// ----

void ViewControl::printCamStats()
{
    cout << "CAM: position: " << camera->getPosition() << " target" << ofVec3f(camera->getTarget().getX(), camera->getTarget().getY(), camera->getTarget().getZ()) << endl;
}



// ----

void ViewControl::setView(int id)
{

}

// ----

void ViewControl::setCamAngle(float ang)
{
    if(camStatus != "animating")
    {
        ofVec3f p2d = ofVec3f( camera->getPosition().x, camera->getPosition().y , 0.0 );
        ofVec3f t2d = ofVec3f ( camera->getTarget().getX(), camera->getTarget().getY(), 0.0);

        float d = p2d.distance(t2d);

        float nx = d * cos(ofDegToRad(ang)) + t2d.x;
        float ny = d * sin(ofDegToRad(ang)) + t2d.y;

        camera->setPosition(ofVec3f(nx, ny, camera->getPosition().z));
    }
    else {
        cout << "ViewControl::setCamAngle: can't set cam angle: Animating" << endl;
    }
}

void ViewControl::rotateCamLeft(float deg)
{
    cout << "ViewControl::rotateCamLeft" << endl;
    setCamAngle(getCamAngle() - deg);
    camera->lookAt( ofVec3f( camera->getTarget().getX(), camera->getTarget().getY(), camera->getTarget().getZ() ) , ofVec3f(0,0,1));
}

void ViewControl::rotateCamRight(float deg)
{
    cout << "ViewControl::rotateCamRight" << endl;
    setCamAngle(getCamAngle() + deg);
    camera->lookAt( ofVec3f( camera->getTarget().getX(), camera->getTarget().getY(), camera->getTarget().getZ() ) , ofVec3f(0,0,1));
}





