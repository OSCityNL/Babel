#include "ofxTween.h"

class tweenTarget
{
    public:
    // CONSTRUCTOR
    tweenTarget(string tt, float v1, float v2);

    // METHODS

    // GETTERS
    string getTarget();
    float getStartValue();
    float getEndValue();
    float getDeltaValue();

    private:
        string target; // target of tween animation: X/Y/Z/A/H/S/RX/RY/RZ/B/ETC
        float startValue;
        float endValue;
        float deltaValue;
};

class Tweener
{
    // NOTES:
    // we use Tweener class only as abstract position from 0.0 - 1.0
    public:
        // CONSTRUCTOR
        Tweener();

        // METHODS
        void start(float duration, float delay);
        void stop();
        float getPosition(); // returns 0.0 - 1.0

        // SET / GET
        bool isRunning();
        bool isCompleted();

    private:
        // BASE TWEEN
        ofxTween tween;

        // PROPERTIES
        bool completed;

        // EASINGS
        ofxEasingBack       easingback;
        ofxEasingBounce     easingbounce;
        ofxEasingCirc       easingcirc;
        ofxEasingCubic      easingcubic;
        ofxEasingElastic    easingelastic;
        ofxEasingExpo       easingexpo;
        ofxEasingLinear     easinglinear;
        ofxEasingQuad       easingquad;
        ofxEasingQuart      easingquart;
        ofxEasingQuint      easingquint;
        ofxEasingSine       easingsine;
};
