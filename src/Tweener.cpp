#include "Tweener.h"

tweenTarget::tweenTarget(string tt, float v1, float v2)
{
    target = tt;
    startValue = v1;
    endValue = v2;
    deltaValue = v2 - v1;
}

string tweenTarget::getTarget()
{
    return target;
}


float tweenTarget::getStartValue()
{
    return startValue;
}

float tweenTarget::getEndValue()
{
    return endValue;
}

float tweenTarget::getDeltaValue()
{
    return deltaValue;
}

////////////////////////////////////////////////

Tweener::Tweener()
{
    // SETUP TWEEN BASE
    tween.setFrameBasedAnimation(true); // FRAME BASED TWEEN (better for video rendering)
    completed = false;
}

void Tweener::start(float du, float de)
{
    //cout << "Tweener::start : start Tweener [duration: " << du << " | delay : " << de << " ]" << endl ;

    tween.setParameters(easingexpo,ofxTween::easeOut, 0.0, 1.0, du, de);

    completed = false;
}

void Tweener::stop()
{
    completed = true;
}

float Tweener::getPosition()
{
    // RETURN POSITION OF TWEEN 0.0 - 1.0

    if(tween.isCompleted()){
      // check if the tween is finished
        completed = true;
    }

    return tween.update();
}

bool Tweener::isRunning()
{
    return tween.isRunning();
}

bool Tweener::isCompleted()
{
    return completed;
}
