#include "Geometry.h"

// base geometry type: point
Geometry::Geometry( ofVec3f point)
{
        setDefaultProperties();

        type = "point";
        positionOrig = point;
		position = positionOrig; // first coordinate

        // add point to mesh
        mesh.setMode(OF_PRIMITIVE_POINTS);
        mesh.addVertex(ofVec3f(0,0,0)); // this is 0,0,0 since we transform the coordinate system on the position (to allow rotations)
        mesh.addColor(fillColor); // vertex color: is this useful?
}

// ----

// make specific line
Geometry::Geometry( ofVec3f p1, ofVec3f p2, ofColor c1, ofColor c2)
{

    setDefaultProperties();

    type = "linestring";
    lineColorStart = c1;
    lineColorEnd = c2;
    points.push_back(p1);
    points.push_back(p2);

    strokeColor = c1;

    // get center pivot
    ofVec3f lv = p2 - p1;
    positionOrig = p1 + lv*0.5;
    position = positionOrig;

    // center all points in poly
    vector <ofVec3f> relativePoints;

    for( int p = 0; p < points.size(); p++)
    {
        // subtract center from point
        relativePoints.push_back( points[p] - position);
    }

    // realign point in poly
    polyline.addVertices(relativePoints);
}

// ---- generic geometry : TODO: base on primitives above

Geometry::Geometry(string tbl, string gt, string coords, int t, ofColor fc, ofColor sc)
{
    //cout << "=> New Geometry: type=" << type << " coords=" << coords << endl;

	// SETTINGS
	setDefaultProperties();

	// CONSTRUCTOR
	type = gt;
	table = tbl;
	time = t;
	strokeColor = sc;
	fillColor = fc;

	// get points
	points = getCoordsFromString(coords);

	// HANDLE GEOMETRY BY TYPE
	if ( type == "point" )
	{
	    positionOrig = points[0];
		position = positionOrig; // first coordinate

        mesh.setMode(OF_PRIMITIVE_POINTS);
        mesh.addVertex(ofVec3f(0,0,0)); // this is 0,0,0 since we transform the coordinate system on the position (to allow rotations)
        mesh.addColor(fillColor); // vertex color
	}
	else if ( type == "linestring" )
    {
        // get center pivot
        ofVec3f lv = points[1] - points[0];
        positionOrig = points[0] + lv*0.5;
        position = positionOrig;

        // center all points in poly
        vector <ofVec3f> relativePoints;

        for( int p = 0; p < points.size(); p++)
        {
            // subtract center from point
            relativePoints.push_back( points[p] - position);
        }

        //ofLogNotice() << " LINE NEW POSITIONS " << points[0];

        // realign point in poly
        polyline.addVertices(relativePoints);

	}
	else if ( type == "polygon")
    {
        ofPolyline tmpPoly;
        tmpPoly.addVertices(points);

        // get center pivot
        position = tmpPoly.getCentroid2D();
        positionOrig = position;

        // protect against Nan values
        if (isnan(position.x) || isnan(position.y) || isinf(position.x) || isinf(position.y) )
        {
            ofLogWarning("Geometry") << "Skipped geometry with NaN/Inf position";
            invalid = true;
        }

        // center all points in poly
        vector <ofVec3f> relativePoints;

        for( int p = 0; p < points.size(); p++)
        {
            // subtract center from point
            relativePoints.push_back( points[p] - position );
        }

        polyline.addVertices(relativePoints);
        mesh = ofMesh(OF_PRIMITIVE_TRIANGLE_FAN, relativePoints);

    }
	else {
        ofLogNotice("Geometry::Geometry") << "unknown geometry type ";
        disabled = true;
        invalid = true;
	}

}

// ----

void Geometry::setDefaultProperties()
{
    // INITIAL PROPERTY VALUES
	requestedRenderer = "";
    renderer = "dynamic";
	alpha = 255;
	radius = 10;
    rotation = ofVec3f(0.0,0.0,0.0);
	disabled = false;
    strokeColor = ofColor(255,255,255);
	fillColor = ofColor(255,255,255);
	framesStatic = 0;
	svg = nullptr;
	time = 0;
	length = 1.0;
	facing = false;
	table = "none";
	svgSize = 0.0;
	boneSpiralAnimation = false;
	labelSize = 5.0;
	imageRef = nullptr;
	imageSize = 0.0;
	imageAlpha = -1;
	length = 1.0;
	lineColorEnd = 0;
	lineColorStart = 0;
	invalid = false;
	labelFacing = true;
	reactivated = false;
	camDistance = -1;
	camDistanceAlphaFactor = 1.0;

	// tweener = Tweener(); // BUG not initiating?
}


// ----

void Geometry::update(ofVec3f cam)
{

    camPosition = cam; // saves camera position for facing

    Geometry::update();

}

// ----

void Geometry::update()
{
    //ofLogNotice() << " update geom " << " t = " << table << " type=" << type;

    if(renderer == "dynamic")
    {
        checkRenderer();

        // CHECK FLOATING
        if (!facing && floatRotation.length() > 0.0)
        {
            rotation += floatRotation;
        }

        // NOTE: don't tween position when boneSpiralAnimation is on
        if (boneSpiralAnimation)
            return; // don't check tween for bone animations

        // UPDATE ANIMATION: checking the tweenTargets vector and modifing the geometry accordingly
        if(!tweener.isCompleted())
        {
            float tweenPosition = tweener.getPosition(); // return 0.0 - 1.0

            for(int i = 0; i < tweenTargets.size(); i++)
            {
                tweenTarget curTweenTarget = tweenTargets[i];

                if(curTweenTarget.getTarget() == "X")
                {
                    position.x = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "Y")
                {
                    position.y = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "Z")
                {
                    position.z = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "RX")
                {
                    rotation.x = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "RY")
                {
                    rotation.y = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "RZ")
                {
                    rotation.z = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "A")
                {
                    alpha = int ( tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue() );

                    // clamp values to be sure
                    if(alpha > 255)
                        alpha = 255;
                    else if(alpha < 0)
                        alpha = 0;
                }
                else if(curTweenTarget.getTarget() == "R")
                {
                    // POINT / BOX SIZE
                    radius = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
                else if(curTweenTarget.getTarget() == "L")
                {
                    // LENGTH
                    length = tweenPosition * curTweenTarget.getDeltaValue() + curTweenTarget.getStartValue();
                }
            }
        }
        else {
            if(alpha == 0)
                disabled = true;
        }

    }

}

// ----

void Geometry::checkRenderer()
{
    // check if the geometry is static for this frame

    // TODO: lines are not working stable with vbo  || type == "linestring"

    if (type == "point" || type == "linestring" || type == "polygon" )
    {

        if ( position.distance(positionOrig) < STATIC_RADIUS )
        {
            if (type == "linestring" && changed)
            {
                // not static
                //ofLogNotice() << " NOT STATIC LINE!";
            }
            else {
                framesStatic++; // count the number of frames the position is static
            }
        }

        // check if the geometry has been static for a number of frames: switch renderer
        if (framesStatic > STATIC_NUMFRAMES)
        {
            // time to switch to vbo render
            if (requestedRenderer == "" && renderer != "static")
            {
                requestStaticRenderer();

                //ofLogNotice() << "Geometry::checkRenderer :: requested static renderer";
            }
        }
    }
}

// ----

void Geometry::requestStaticRenderer()
{
    requestedRenderer = "static"; // this will be picked up by babel that scans there requests periodically
    framesStatic = 0;

    stopAnimation(); // stop animation
}

// ----

void Geometry::draw()
{
    //ofLogNotice() << " draw geom " << " t = " << table << " type=" << type;

    // don't render static
    if(renderer == "static" && !reactivated){ //
        return;
    }

    // special properties for reactivated points
    if (reactivated){
        if (camDistance != -1) // camDistance was set in nearPoints function
        {
            camDistanceAlphaFactor = ofMap(camDistance,0.0,REACTIVATE_POINTS_MAXRADIUS, 1.0, 0.0, true);
            //ofLogNotice("======>") << camDistance << " " << camDistanceAlphaFactor;
        }
    }

    //ofLogNotice() << " DRAW GEOM TYPE=" << type;

    if(!disabled)
    {
        if(alpha != 0)
        {
            // main draw loop per geometry type

            // general transformation
            ofPushMatrix();
            ofTranslate(position); // translate origin of coordsystem to pivot of object

            if (!facing){
                ofRotateX(rotation.x); // then rotate around that
                ofRotateY(rotation.y);
                ofRotateZ(rotation.z);
            }

            // draw label
            if (label.size() > 0)
            {
                ofPushStyle();
                ofPushMatrix();

                if (labelColor == 0)
                    labelColor = fillColor;

                ofSetColor(labelColor);

                //float textWidth = labelSize * label.size(); //label.getBBox().getWidth();
                ofTranslate( 1.0, 0, 0);

                if (labelFacing){ facingDrawBegin();}
                    ofRotateX(180); // for some reason the labels are upside down otherwise
                    fontRenderer->drawBatch(label, labelSize, 0 , 0);
                if (labelFacing){ facingDrawEnd();}
                //int numLines = 10;
                //fontRenderer->drawMultiLineColumn( label, 10, 0, 0, 100, numLines , false, 0, false, NULL );
                ofPopStyle();
                ofPopMatrix();
            }

            //ofLogNotice() << " GEOM TYPE" << type;

            if ( type == "point" )
            {
                // check image alpha: either use that otherwise default
                if (imageAlpha == -1)
                    imageAlpha = alpha;

                if (imageSize == 0.0)
                    imageSize = radius*2.0;

                // set facing if needed

                ofPushStyle();
                ofPushMatrix();
                ofSetColor(255,255,255, int(float(imageAlpha) * camDistanceAlphaFactor));

                if (facing){ facingDrawBegin();}

                // image or svg icon
                if (image.getWidth() > 1.0)
                {
                    image.draw(-imageSize/2.0 , -imageSize/2.0, 0.0, imageSize, imageSize);
                }

                // img ref
                if (imageRef != nullptr)
                {
                    if (imageRef->getWidth() > 1.0 )
                    {
                        imageRef->draw(-imageSize/2.0 , -imageSize/2.0, 0.0, imageSize, imageSize);
                    }
                }

                // svg
                if (svg != nullptr)
                {
                    if (svg->getWidth() > 0.1)
                    {
                        // normal svg is 10px
                        if (svgSize == 0)
                            svgSize = radius; // default

                        float t = - svgSize /2.0;
                        ofTranslate( t , t, 0.0);
                        float s =  svgSize /10.0; // svgs normalized on 10x10
                        ofScale(s,s,s);

                        svg->draw();

                    }
                    else {
                        //ofLogNotice() << "Geometry::draw: " << " SVG with zero width: not rendered!";
                    }
                }


                if (facing){ facingDrawEnd();}

                // draw point
                mesh.setColor(0, ofColor( fillColor.r, fillColor.g, fillColor.b, int ( fillColor.a * ofMap(alpha,0,255,0.0,1.0))) );
                glPointSize(radius);
                glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
                mesh.draw();

                glPopMatrix();
                ofPopStyle();

            }
            //// draw line /////
            else if ( type == "linestring" )
            {

                ofPushMatrix();
                ofPushStyle();

                // NOTE: 2 colors for like are not working with dynamic render method
                if (lineColorEnd == 0 && lineColorStart == 0){
                    lineColorEnd = strokeColor;
                    lineColorStart = strokeColor;
                }

                ofSetLineWidth(1.0); // TODO?
                ofSetColor(strokeColor.r, strokeColor.g, strokeColor.b, int ( strokeColor.a * ofMap(alpha,0,255,0.0,1.0)) ) ;
                ofNoFill();

                changed = false;

                // IMPLEMENT LINE LENGTH IN PERCENT
                if (length < 1.0)
                {
                    // get current vertices
                    ofVec3f s = points[0];
                    ofVec3f e = points[1];
                    ofVec3f d = e - s;

                    // get point of percent length
                    ofVec3f n = (d * length) + s;
                    // change second point to that one

                    //ofLogNotice() << " RESIZE LINE L=" << length << "S= " << s << " E=" << e << " d=" << d;

                    // reupload
                    polyline.clear();
                    polyline.addVertex(s - position );
                    polyline.addVertex( n - position );

                    changed = true;

                    //ofLogNotice() << " RESIZE LINE " << length << "  " << s << " ---- " << " ---- " << e << " ---- " << n;
                }

                polyline.draw();


                // check mesh to draw
                /*
                if(mesh.getNumVertices() > 0)
                {
                    ofSetColor(fillColor.r, fillColor.g, fillColor.b, int ( fillColor.a * ofMap(alpha,0,255,0.0,1.0)) ) ;
                    ofFill();
                    mesh.draw();
                }
                */

                ofPopMatrix();
                ofPopStyle();

            }
            else if ( type == "polygon")
            {
                ofPushMatrix();
                ofPushStyle();

                // draw outline in polyline
                ofSetLineWidth(1.0); // TODO?
                ofSetColor(strokeColor.r, strokeColor.g, strokeColor.b, int ( strokeColor.a * ofMap(alpha,0,255,0.0,1.0)) ) ;
                ofFill();
                polyline.draw(); // for now only draw simple

                // draw fill
                ofSetColor(fillColor.r, fillColor.g, fillColor.b, int ( fillColor.a * ofMap(alpha,0,255,0.0,1.0)) ) ;
                mesh.draw();


                ofPopMatrix();
                ofPopStyle();
            }
            else {
                //ofLogNotice() << "Error: unknown geometry type: " << type;
            }

            ofPopMatrix();

        } // end draw loop

    }

    // always deactivate
    reactivated = false;
    camDistance = -1;

}

// ---- animation functions

void Geometry::animateTo(map <string, float> targets, float duration)
{
    // INPUT EXAMPLE: {{ "X", 100},{ "A", 255 }};

    // X/Y/Z/A/H/S/RX/RY/RZ/R/ETC

    //cout << "Geometry::animateTo : physical: " << isPhysical << endl;

    disabled = false;

    // create and save the new targets
    vector<tweenTarget> newTweenTargets;

    map<string, float>::iterator curr,end;

    for( curr = targets.begin(), end = targets.end();  curr != end;  curr++ )
    {
       //cout <<  curr->first << " : " << curr->second << endl;

        string target = curr->first; // target property
        float endValue = curr->second;
        float startValue;

       // CREATE TWEENTARGETS
        if(target == "X")
        {
            startValue = position.x;
        }
        else if(target == "Y")
        {
            startValue = position.y;
        }
        else if(target == "Z")
        {
            startValue = position.z;
        }
        else if(target == "RX")
        {
            startValue = rotation.x;
        }
        else if(target == "RY")
        {
            startValue = rotation.y;
        }
        else if(target == "RZ")
        {
            startValue = rotation.z;
        }
        else if(target == "A")
        {
            startValue = alpha;
        }
        else if(target == "R")
        {
            // POINT BOX SIZE
            startValue = radius;
        }
        else if(target == "L")
        {
            // LENGTH OF GEOM (like line length)
            startValue = length;
        }
        else {
            // no known target:
            break;
        }

        if(startValue != endValue)
        {
            // only tween when the start and endvalues are different
            newTweenTargets.push_back(tweenTarget(target, startValue, endValue));
        }
    }

    // check and save tween targets
    if(newTweenTargets.size() > 0)
    {
        tweenTargets = newTweenTargets; // upload new targets
        tweener.start(duration, 0.0); // (re)start tweener
    }

}

// ----

void Geometry::stopAnimation()
{
    tweenTargets.clear();
    tweener.stop();
}

// ----

void Geometry::animateToOriginalPosition(float duration, map<string,float> targets)
{
    // return to original position and rotation: ALSO ALPHA

    map<string,float> returnTargets = {{ "X", positionOrig.x},{ "Y", positionOrig.y},{ "Z", positionOrig.z},{"RX", rotationOrig.x},{"RY", rotationOrig.y}, {"RZ", rotationOrig.z}};

    targets.insert( returnTargets.begin(), returnTargets.end() );

    animateTo( targets, duration);
}

// ----

void Geometry::loadImage(string url)
{
    // loads image with cache
    string fileName = url;

    ofStringReplace(fileName, "/", "||"); // escape url
    ofStringReplace(fileName, ".jpg", ".png");

    if (!ofIsStringInString(".png", fileName));
        fileName += ".png";

    ofFile file(ofToDataPath("img/" + fileName));

    //ofLogNotice() << "Geometry::loadImage: " << fileName << " file " << file;

    if (file.exists())
    {
        //ofLogNotice() << " get image from cache " << fileName;

        image.loadImage(file);
    }
    else {
        // load from url
        image.allocate(90,90, OF_IMAGE_COLOR_ALPHA);

        ofImage tmpImage;
        ofStringReplace(url, "https", "http"); // remove https
        tmpImage.loadImage(url);

        //image.clone(tmpImage);
        //image.setImageType(OF_IMAGE_COLOR_ALPHA);

        // crop and save to cache
        if ( tmpImage.getWidth() > 0)
        {
            //ofLogNotice() << "loaded image";

            //image.crop(0,0, 640, 640);
            tmpImage.resize(90, 90);
            //tmpImage.update();

            // mask
            ofImage mask;
            mask.loadImage("mask.png");
            mask.setImageType(OF_IMAGE_COLOR_ALPHA);

            //unsigned char* imagePixels = image.getPixels();
            //unsigned char* maskPixels= mask.getPixels();

            /*
            for (int p = 0; p < 90*90*4; p++)
            {
                char a = maskPixels[p+3];

                cout << "A " << a << endl;

                //cout << " r " << imagePixels[p] << " g " << imagePixels[p+1] << " b" << imagePixels[p+2] ;

                imagePixels[p] = imagePixels[p];
                imagePixels[p+1] = imagePixels[p+1];
                imagePixels[p+2] = imagePixels[p+2];
                imagePixels[p+3] = 255;

            }
            */

            for (int x = 0; x < 90; x++)
            {
                for (int y = 0; y < 90; y++)
                {
                    ofColor mc = mask.getColor(x,y);
                    ofColor c = tmpImage.getColor(x,y);

                    if(mc.a == 255)
                    {
                        image.setColor(x,y, ofColor(c.r, c.g, c.b, 255));
                    }
                    else {
                        image.setColor(x,y, ofColor(0,0,0,0));
                    }


                }
            }

            ofLogNotice() << " saved image " << fileName;

            image.saveImage("img/" + fileName); // save image "img/" +
        }
    }

}

ofVec3f Geometry::camToRotation()
{
    if (camPosition.length() > 0)
    {
        ofVec3f d = position - camPosition;

        float xRot = ofVec3f( 0, d.y, d.z).angle(ofVec3f(0,1,0));
        float yRot = ofVec3f( d.x, 0, d.z).angle(ofVec3f(0,0,1));
        float zRot = ofVec3f( d.x, d.y, 0).angle(ofVec3f(1,0,0));

        return ofVec3f(xRot,yRot,zRot);
    }

    return ofVec3f(0,0,0);
}

///////// bone spiral methods ////////////

// ----

void Geometry::attachToBones(vector<Bone>* bs)
{
    // save references to all bones
    bones = bs;
    boneIndex = -1;
    boneSpiralAnimation = true;

    //bone = nullptr;

    // constructor

    // first find closest bone
    bool r = attachToNewBone(2000.0);

    if (!r){
        ofLogError() << "Geometry::Geometry: Could not find a bone to connect new geometry to";
        return;
    }

    resetSpiralData();

}

// ----

void Geometry::resetSpiralData()
{
    boneSpiralOrbitFlySpeedFactor = 1.0;
    boneSpiralOrbitRotationSpeedFactor = ofRandom(0.001, 0.1);
    boneSpiralOrbitAlongSpeedFactor = ofRandom(0.01, 0.05);

    boneSpiralOrbitRadiusVariationFactor = 0.0; // set 0 to disable: NOTE: state seems not stable: point slowly removes from line
    boneSpiralOrbitRadiusVariationPeriod = 100.0 ; //200.0; // in frames
    boneSpiralOrbitRadiusVariationInc = M_PI*2.0/ boneSpiralOrbitRadiusVariationPeriod;
    boneSpiralOrbitRadiusVariationPosition = 0.0;
    boneSpiralOnBone = false;
    boneSpiralDirection = 0; // important to set; when int is initiatilzed it can have weird values??

    // reset
    boneSpiralStatus = "fly";

}

// ----

void Geometry::resetSpiralData(Bone* b)
{
    // DEBUG: check radius for existing geometries to new bone
    /*
    if (boneSpiralOnBone)
    {
        ofLogNotice() << " Get new radius " << b->radius;
    }*/
    // END DEBUG

    // get radius from bone
    boneSpiralOrbitRadius = 0;

    if (b != nullptr)
    {
        boneSpiralOrbitRadius = b->radius;
    }

    if (boneSpiralOrbitRadius == 0)
    {
        // default radius
        boneSpiralOrbitRadius = ofRandom(2.0,4.0); //100.0;
    }

    resetSpiralData();
}

// ----

void Geometry::updateBoneSpiral()
{
    // get bone (robust)

    Bone* bone = nullptr;

    try {
        bone = &(bones->at(boneIndex));}
    catch (const out_of_range &e){
        ofLogWarning("Geometry::update") << "Could not get bone of index " << boneIndex << "! Update of geometry cancelled for this frame";
        return;
    }

    // bone found, continue with update

    if ( bone->p1.distance( bone->p2 ) < 0.1 )
    {
        ofLogNotice() << "Geometry::update:" << " Bone is zero length, wait for good bone";
        return;
    }

    //ofLogNotice() << " Geometry::update: bone [" << bone->p1 << " , " << bone->p2 << "]";

    if (boneSpiralStatus == "")
    {
        // initial phase
        boneSpiralStatus = "fly";
    }

    if (bone == nullptr)
    {
        ofLogError() << "Geometry::update " << "No bone attached";
        return;
    }

    // set velocity vector to spiral around bone
    ofVec3f lineDirection = bone->p2 - bone->p1;
    float t = -(bone->p1 - position).dot( lineDirection ) / (lineDirection.length() * lineDirection.length());

    ofVec3f nearestPointOnLine = bone->p1 + lineDirection * t;
    ofVec3f radiusVector = nearestPointOnLine - position;

    float radius = radiusVector.length();
    ofVec3f attractionVector = nearestPointOnLine - position;

    // debug output
    //ofLogNotice() << " lineDirection " << lineDirection << " t " << t << " nearestPointOnLine " << nearestPointOnLine << " radiusVector " << radiusVector << " attractionVector" << attractionVector;


    // direction of boneSpiralDirection; 1 = from p1 to p2, -1 from p2 to p1

    // define direction of spiral for the first time
    if (boneSpiralDirection == 0)
    {
        if ( (bone->p1 - position).length() < (bone->p2 - position).length() ){
            boneSpiralDirection = 1;
        }
        else {
            boneSpiralDirection = -1;
        }

        //ofLogNotice("Geometry::update") << " Set boneSpiralDirection for first time, bone: [" << bone->p1 << "," << bone->p2 << "]" << " position: " << position << " ==> direction" << boneSpiralDirection ;
    }

    // check behaviour of point if nearest point is on bone
    if (t < 0.0 || t > 1.0 )
    {
        // ran out of bone after running along it
        if (boneSpiralOnBone)
        {
            // point is on bounds of bone: let's find a new bone

            bool r = attachToNewBone(10.0);

            if (!r)
            {
                //ofLogNotice() << " Could not find other bone: reverse direction";
                boneSpiralDirection *= -1;
            }
        }
        else {
            // point has not touched bone yet, get some extra speed
            if ( (bone->p1 - position).length() < (bone->p2 - position).length() )
            {
                attractionVector = bone->p1 - position; // some extra speed
            }
            else {
                attractionVector = bone->p2 - position;
            }
        }
    }
    else {
        // point is on bone
        boneSpiralOnBone = true;
    }

    // attractor scenario: either fly or orbit
    if (radius < boneSpiralOrbitRadius)
    {
        boneSpiralStatus = "orbit";
    }
    else {
        boneSpiralStatus = "sky";
    }

    // calculate velocity (out of component rotationVector, attractionVector and alongVector)

    ofVec3f rotationVelocity = radiusVector.crossed(lineDirection).normalized();
    ofVec3f alongVelocity = boneSpiralDirection * lineDirection.normalized();
    ofVec3f attractionVelocity;

    // DEBUG: not moving to bone geometries
    /*
    if (alongVelocity.length() <= 0.1 && !boneSpiralOnBone)
    {
        ofLogWarning("Geometry::update") << " Geometry is not moving towards bone";
        ofLogNotice() << " LineDirection " << lineDirection.normalized() << " boneSpiralDirection " << boneSpiralDirection << " rotationVelocity " << rotationVelocity << " alongVelocity " << alongVelocity;
    }
    */


    // attraction depends on status scenario
    if (boneSpiralStatus == "orbit")
    {
        // TODO: there seems to be a unstable state: radii seem to increase slowly
        // make radius fluctuate around central radius
        boneSpiralOrbitRadiusVariationPosition += boneSpiralOrbitRadiusVariationInc;

        if (boneSpiralOrbitRadiusVariationPosition >= M_PI*2.0)
            boneSpiralOrbitRadiusVariationPosition = 0.0;

        float av = sin(boneSpiralOrbitRadiusVariationPosition);

        // THIS IS THE SIN FLUCTUATING: now disabled
        // attractionVelocity = attractionVector.normalized() * av * boneSpiralOrbitRadiusVariationFactor;
        //ofLogNotice() << "RADIUS VARIATION: " << av << "attractionVector: " << attractionVelocity;

        // NEW: go towards radius ( for entries that came in low in orbit)
        float towardsRadius = boneSpiralOrbitRadius - radius;

        attractionVelocity = -attractionVector.normalized() * towardsRadius * 0.05;
    }
    else {
        // fly
         attractionVelocity = attractionVector.normalized() * boneSpiralOrbitFlySpeedFactor;
    }

    //ofExit();

    // direction of along velocity

    /*
    ofLogNotice() << "T: " << t;
    ofLogNotice() << "NEAREST POINT: " << nearestPointOnLine;
    ofLogNotice() << "RADIUS VECTOR " << radiusVector << " DISTANCE: " << radiusVector.length();
    ofLogNotice() << "ROTATION VELOCITY VECTOR " << rotationVelocity;
    ofLogNotice() << "ANGLE ALONGVELOCITY - LINEDIRECTION" << alongVelocity.angle(lineDirection);
    */

    // new position of point
    velocity = rotationVelocity*boneSpiralOrbitRotationSpeedFactor + attractionVelocity + alongVelocity*boneSpiralOrbitAlongSpeedFactor;
    position += velocity;

}

// ----

void Geometry::attachToBone(int index)
{
    boneIndex = index; // set bone index

    // get radius from bone if specified
    Bone* bone = nullptr;
    try {
        bone = &(bones->at(index)); // temp reference to do some chores
    }
     catch (const out_of_range &e){
        // TODO
    }

    resetSpiralData(bone);
}

// ----

bool Geometry::attachToNewBone(float scanRadius)
{
    // scan vector of bones to find a bone within a distance that is not the current bone

    int closestBoneIndex = -1;
    float smallestDistance = 999999;

    //ofLogNotice() << " scan for bones for new geometry: num bones: " << bones->size();

    for (int c = 0; c < bones->size(); c++)
    {
        Bone* b = &(bones->at(c));

        //ofLogNotice() << "CUR BONE " << boneIndex;

        if (c != boneIndex)
        {
            float d = distanceToBone(b);

            //ofLogNotice() << " distance to new bone " << d;

            if ( d < scanRadius )
            {
                if (d < smallestDistance)
                {
                    closestBoneIndex = c;
                    smallestDistance = d;
                }
            }
        }
    }

    // check result

    if (closestBoneIndex == -1)
    {
        // no bone found
        return false;
    }
    else {
        // found a bone to connect to
        //ofLogNotice() << "connect to new bone on index: " << closestBoneIndex;

        attachToBone(closestBoneIndex);

        return true;
    }

}

// ----

float Geometry::distanceToBone(Bone* b)
{
    ofVec3f lineDirection = b->p2 - b->p1;
    float t = -(b->p1 - position).dot( lineDirection ) / (lineDirection.length() * lineDirection.length());

    ofVec3f r; // distance vector

    if (t > 0.0 && t < 1.0)
    {
        // nearest point is on line
        ofVec3f nearestPointOnLine = b->p1 + lineDirection * t;

        r = nearestPointOnLine - position;
    }
    else {
        // nearest point is not on bone, so distance is that of one of the 2 outer bone coordinates
        if ( (b->p1 - position).length() < (b->p2 - position).length() )
        {
            r = b->p1 -position;
        }
        else {
            r = b->p2 -position;
        }
    }

    // return distance
    return r.length();
}

// ----

/*
void Geometry::draw()
{
    ofPushMatrix();
    ofPushStyle();

    ofTranslate(position.x, position.y, position.z);
    ofFill();
    ofSetColor(0,0,255, 60);
    ofBox(ofVec3f(0,0,0), 0.5);
    ofLine(ofVec3f(0,0,0), velocity*2.0); // velocity

    ofPopStyle();
    ofPopMatrix();
}
*/

///////////////////// GET /////////////////////////////////

// ----

vector<ofPoint> Geometry::getCoordsFromString(string s)
{

    vector<ofPoint> pnts;

    // protect
    if (s == "0" || s == "")
    {
        ofLogNotice() << "Geometry::getCoordsFromString: Empty geometry!";
        pnts.push_back(ofVec3f(0,0,0));
        return pnts; // no length: do nothing
    }


    vector<string> coordStrings = ofSplitString(s, ",");

    //cout << coordStrings.size() << endl;

    for( int p = 0; p < coordStrings.size(); p++)
    {
        vector<string> coords = ofSplitString(coordStrings[p], " "); // now split again with ' ' between x/y coords

        ofPoint pnt = ofPoint(ofToInt(coords[0]),ofToInt(coords[1]));

        //cout << "point added: (" << pnt << ")" << endl;

        pnts.push_back(pnt);
    }

    //cout << "points loaded: " << pnts.size() << endl;

    return pnts;
}

// ----

ofVec3f Geometry::getPosition(){
    return position;
}

// ----

string Geometry::getRequestedRenderer()
{
    return requestedRenderer;
}

// ----

string Geometry::getRenderer()
{
    return renderer;
}

// ----

ofColor Geometry::getFillColor()
{
    return fillColor;
}

// ----

int Geometry::getRadius()
{
    return radius;
}

// ----

ofFloatColor Geometry::getFloatFillColor()
{
    return ofFloatColor( float(fillColor.r) / 255.0, float(fillColor.g) / 255.0, float(fillColor.b) / 255.0, float(alpha) / 255.0);
}

// ----

ofFloatColor Geometry::getFloatStrokeColor()
{
    return ofFloatColor( float(strokeColor.r) / 255.0, float(strokeColor.g) / 255.0, float(strokeColor.b) / 255.0, float(alpha) / 255.0);
}

//////////////////// SET ///////////////////////

void Geometry::requestRenderer(string type)
{
    // rendering is done either from the Geometry itself (dynamic) or central from a vbo for static coordinates
    // managing these operations is done on a level on up (layer)
    // Geometry puts in a request to change renderer with the property "requestedRenderer" that is picked up centrally
    requestedRenderer = type;
}

// ---

void Geometry::resetRequestedRenderer()
{
    requestedRenderer = "";
}

// ---

void Geometry::setRenderer(string r)
{
    renderer = r;
}

// ----

void Geometry::setPositionOrigZ(float z)
{
    positionOrig.z = z;
}

// ----

void Geometry::setRadius(int r)
{
    radius = r;
}

// ----

void Geometry::setData(map<string,string> d)
{
    data = d;
}


// ----

void Geometry::setFillColor(int r, int g, int b, int a)
{
    fillColor = ofColor(r, g, b, a);
}

// ----

void Geometry::setFloatRotation(ofVec3f v )
{
    floatRotation = v;
}

// ----

void Geometry::setSvg(ofxSVG* s)
{
    svg = s;
}

// ----

void Geometry::setSvg(ofxSVG* s, float ss)
{
    svg = s;
    svgSize = ss;
}

// ----

void Geometry::setFacing()
{
    facing = true;
}

// ----

void Geometry::setLabel(string t, ofxFontStash* renderer)
{
    // with reference to font renderer
    if (renderer != nullptr)
    {
        label = t;
        fontRenderer = renderer;
    }
}

// ---

void Geometry::setLabel(string t, float s, ofxFontStash* renderer)
{
    // with reference to font renderer
    if (renderer != nullptr)
    {
        label = t;
        labelSize = s;
        fontRenderer = renderer;
    }
}

// ----

void Geometry::setLabel(string t, float s, ofColor c, ofxFontStash* renderer)
{
    // with reference to font renderer
    if (renderer != nullptr)
    {
        label = t;
        labelColor = c;
        labelSize = s;
        fontRenderer = renderer;
    }
}

// ----

void Geometry::setImageRef(ofImage* img)
{
    imageRef = img;
}

// ----

void Geometry::brightenColor()
{
    int f = 1.3;

    fillColor.r *=  f;
    fillColor.g *=  f;
    fillColor.b *=  f;
}

// ----

void Geometry::setColor(ofColor newColor)
{
    strokeColor = newColor;
    fillColor = newColor;
}

// ----

void Geometry::setColorOnValue(float val, float valMin, float valMax, ofColor cMin, ofColor cMax)
{
    // whatColor = "FILL" / "STROKE" / "BOTH"

    float dr = (cMax.r - cMin.r);
    float dg = (cMax.g - cMin.g);
    float db = (cMax.b - cMin.b);
    float da = (cMax.a - cMin.a);
    // TODO: ALPHA???

    float f = ofMap(val, valMin, valMax, 0.0, 1.0, true);


    ofColor newColor = ofColor( int ( cMin.r + dr * f ), int ( cMin.g + dg * f) , int ( cMin.b + db * f), int (cMin.a + da * f) );
    alpha = int (cMin.a + da * f);

    strokeColor = newColor;
    fillColor = newColor;

}

// ----

void Geometry::facingDrawBegin()
{
    float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well
	for( i=0; i<3; i++ )
	    for( j=0; j<3; j++ ) {
            if ( i==j )
                modelview[i*4+j] = 1.0;
            else
                modelview[i*4+j] = 0.0;
	    }

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

// ----

void Geometry::facingDrawEnd()
{

	// restore the previously
	// stored modelview matrix
	glPopMatrix();
}

