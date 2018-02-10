#include "Babel.h"

Babel::Babel(): hash(hashedPoints) // <= initialization list: http://stackoverflow.com/questions/2785612/c-what-does-the-colon-after-a-constructor-mean
{
    // constructor

    // setup shader
	shader.load("shader.vert", "shader.frag");

    // some default values
	renderCheckWaitCount = 0;
}

// ----

void Babel::init(string c)
{
    /*
    // init
    dataFeeder = DataFeeder("test10000.xml");
    dataFeeder.setTimeScale(3600*1); // 3600=hour
    //dataFeeder.setTimeRange(1437465910, 1438100056);
    */

    // set city
    city = c;

    loadSvgs({ "twitter.svg", "weibo.svg", "instagram.svg" }); // cache svgs

    //// TIMELINE ////

    float TIMELINE_FPS = 60.0;
    float TIMELINE_SECTION_SECONDS = 5.0; //
    float TIMELINE_SECTION_FRAMES = TIMELINE_FPS * TIMELINE_SECTION_SECONDS;

    if (city == "hk")
    {
        timeline.push_back( ViewState( 0,                           "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(10,10, 400), ofVec3f(0,0,0), ofVec3f(0,1.0,0.0) }, {{}} )); // map
        timeline.push_back( ViewState( 1 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(400,-500, 800), ofVec3f(0,0,0), ofVec3f(0,0,1.0) }, {{}} )); // zoomed out
        timeline.push_back( ViewState( 2 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-1000,-700,400), ofVec3f(0,0,0), ofVec3f(0,0.2,1.0) }, {{}} ));
        timeline.push_back( ViewState( 3 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-700, 200,1000), ofVec3f(0,0,-40), ofVec3f(0,-0.4,1.0) }, {{}} )); // sideways
        timeline.push_back( ViewState( 4 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-500, 1200,30), ofVec3f(0,500,0), ofVec3f(0,0,1.0) }, {{}} )); // skyline
        timeline.push_back( ViewState( 5 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(600,-400,60), ofVec3f(0,0,1.0) }, {{}} )); // zoomin first
        timeline.push_back( ViewState( 6 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(400,400,60), ofVec3f(0,0,1.0) }, {{}} )); // look around
        timeline.push_back( ViewState( 7 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 300), ofVec3f(44, -22, 127), ofVec3f(0,0.5,0.5) }, {{}} )); // top down
        timeline.push_back( ViewState( 8 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-50, 500, 900), ofVec3f(0, -500, 0), ofVec3f(0, 1.0, 0.3) }, {{}} )); // move down back
        //timeline.push_back( ViewState( TIMELINE_SECTION_FRAME + 7 * TIMELINE_SECTION_FRAME, "animateTo", TIMELINE_SECTION_FRAME, { ofVec3f(-1600, 860, 660), ofVec3f(0,0,0), ofVec3f(0,0, 0.0) }, {{}} ));
    }


    //////////////////// SHENZHEN TIMELINE //////////////////
    else if (city == "shz")
    {
        timeline.push_back( ViewState( 0,                           "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(10,10, 800), ofVec3f(0,0,0), ofVec3f(0,1.0,0.0) }, {{}} )); // map
        timeline.push_back( ViewState( 1 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(400,-500, 800), ofVec3f(0,0,0), ofVec3f(0,0,1.0) }, {{}} )); // zoomed out
        timeline.push_back( ViewState( 2 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-1000,-700,400), ofVec3f(0,0,0), ofVec3f(0,0.2,1.0) }, {{}} ));
        timeline.push_back( ViewState( 3 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-700, 200,1000), ofVec3f(0,0,-40), ofVec3f(0,-0.4,1.0) }, {{}} )); // sideways
        timeline.push_back( ViewState( 4 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-500, 1200,30), ofVec3f(0,500,0), ofVec3f(0,0,1.0) }, {{}} )); // skyline
        timeline.push_back( ViewState( 5 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(600,-400,60), ofVec3f(0,0,1.0) }, {{}} )); // zoomin first
        timeline.push_back( ViewState( 6 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(400,400,60), ofVec3f(0,0,1.0) }, {{}} )); // look around
        timeline.push_back( ViewState( 7 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 300), ofVec3f(44, -22, 127), ofVec3f(0,0.5,0.5) }, {{}} )); // top down
        timeline.push_back( ViewState( 8 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-50, 500, 900), ofVec3f(0, -500, 0), ofVec3f(0, 1.0, 0.3) }, {{}} )); // move down back
        //timeline.push_back( ViewState( TIMELINE_SECTION_FRAME + 7 * TIMELINE_SECTION_FRAME, "animateTo", TIMELINE_SECTION_FRAME, { ofVec3f(-1600, 860, 660), ofVec3f(0,0,0), ofVec3f(0,0, 0.0) }, {{}} ));
    }
    else if (city == "sp") {
        timeline.push_back( ViewState( 0,                           "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(10,10, 800), ofVec3f(0,0,0), ofVec3f(0,1.0,0.0) }, {{}} )); // map
        timeline.push_back( ViewState( 1 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(400,-500, 800), ofVec3f(0,0,0), ofVec3f(0,0,1.0) }, {{}} )); // zoomed out
        timeline.push_back( ViewState( 2 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-1000,-700,400), ofVec3f(0,0,0), ofVec3f(0,0.2,1.0) }, {{}} ));
        timeline.push_back( ViewState( 3 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-700, 200,1000), ofVec3f(0,0,-40), ofVec3f(0,-0.4,1.0) }, {{}} )); // sideways
        timeline.push_back( ViewState( 4 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-500, 1200,30), ofVec3f(0,500,0), ofVec3f(0,0,1.0) }, {{}} )); // skyline
        timeline.push_back( ViewState( 5 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(600,-400,60), ofVec3f(0,0,1.0) }, {{}} )); // zoomin first
        timeline.push_back( ViewState( 6 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 127), ofVec3f(400,400,60), ofVec3f(0,0,1.0) }, {{}} )); // look around
        timeline.push_back( ViewState( 7 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(44, -22, 300), ofVec3f(44, -22, 127), ofVec3f(0,0.5,0.5) }, {{}} )); // top down
        timeline.push_back( ViewState( 8 * TIMELINE_SECTION_FRAMES, "animateTo", TIMELINE_SECTION_FRAMES, { ofVec3f(-50, 500, 900), ofVec3f(0, -500, 0), ofVec3f(0, 1.0, 0.3) }, {{}} )); // move down back
        //timeline.push_back( ViewState( TIMELINE_SECTION_FRAME + 7 * TIMELINE_SECTION_FRAME, "animateTo", TIMELINE_SECTION_FRAME, { ofVec3f(-1600, 860, 660), ofVec3f(0,0,0), ofVec3f(0,0, 0.0) }, {{}} ));

    }

    //// NAMES OF SETS ////
    DATASET_WEIBO = "weibo_" + city;
    DATASET_WEIBO_PATHS = "weibo_" + city + "_paths";
    DATASET_WEIBO_TOPICS = "weibo_" + city + "_topics";
    DATASET_TWITTER = "twitter_" + city;
    DATASET_INSTAGRAM = "instagram_" + city;
    DATASET_INSTAGRAM_PATHS = "instagram_" + city + "_paths";


    //// VIEWCONTROL ////
    if (!SETTINGS_DISABLE_SHOW )
    {
        viewcontrol = ViewControl(&cam, timeline);
        viewcontrol.startShow();
    }

    //// SOUNDTRACK ////
    playSoundtrack();
}

// ----

void Babel::update()
{
    // TODO: check renderers  etc

    if (!SETTINGS_DISABLE_SHOW ){
        viewcontrol.update();}


    checkRenderers();

    reactivateNearPoints(cam.getPosition(), REACTIVATE_POINTS_NUM);
}

// ----

void Babel::draw()
{
    // main draw routine

    // rendering dynamic geometry
    // TODO: make this quicker by creating a geometries subset of only dynamic ones


    cam.begin();


    ofVec3f camPosition = cam.getPosition();

    // render fonts in batches
    if (fontRenderer->isLoaded())
        fontRenderer->beginBatch();

    // separate geometries from feeder
    for (int g = 0; g < geometries.size(); g++)
    {
        //ofLogNotice() << "===> DRAW" <<  geometries[g]->position << " type=" << geometries[g]->type << " table=" << geometries[g]->table;
        //if (geometries[g]->type == "linestring")
        //    ofLogNotice() << " LINESTRING";

        geometries[g]->update(camPosition);
        geometries[g]->draw();
    }

    // generated geometries within babel
    for (int g = 0; g < newGeometries.size(); g++)
    {
        newGeometries[g].update(camPosition);
        newGeometries[g].draw();
    }


    if (fontRenderer->isLoaded())
        fontRenderer->endBatch();


    // render on static geometry on vbo

    // draw vbo (if any)
    shader.begin();
        vboPoints.draw(GL_POINTS, 0, (int)vboPointPositions.size());
        // draw vbo lines
        vboLines.draw(GL_LINES, 0, (int)vboLinePositions.size());

         // http://stackoverflow.com/questions/26944959/opengl-separating-polygons-inside-vbo
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(0xffff);
        //vboPolys.draw(GL_LINE_STRIP, 0, (int)vboPolyPositions.size()); // TRIANGLE_FAN
        vboPolys.drawElements(GL_POLYGON,(int)vboPolyPositions.size()); // LINE_STRIP TRIANGLE_FAN

    shader.end();

    cam.end();

}

// ----

void Babel::reactivateNearPoints(ofVec3f fromPoint, int numPoints)
{
    if (hashResults.size() != numPoints)
        hashResults.resize(numPoints);

    hash.findNClosestPoints(fromPoint, numPoints, hashResults);

    // cycle through hash results and reactivate point if nessacary

    for (int i = 0; i < hashResults.size(); i++)
    {

        int pointIndex = hashResults.at(i).first;
        float distance = hashResults.at(i).second;

        // protect against an index that is not there yet
        if (pointIndex < hashedPoints.size())
        {
            int pointId = int( hashedPoints.at(pointIndex)[3]);

            if (pointId > 0 )
            {
                Geometry* geom = pointsById.at(pointId);

                // only reactive if render is static
                if (geom->renderer == "static" )
                {
                    geom->reactivated = true;
                    geom->camDistance = sqrt(distance);
                }
            }

        }
    }
}


// ----

void Babel::reset()
{
    // reset babel
    geometries.clear(); // clear geometries vector
    resetVbo();
    resetGrid();
}

// ----

void Babel::addGeometry(Geometry* g)
{
    // SETTINGS STYLING
    //ofLogNotice("Babel::addGeometry") << "=> Add geometry " << g->type;

    float HEIGHT_GROWTH_FACTOR = 0.25; // 0.25 for SHZ

    // weibo paths
    ofColor SETTINGS_PATH_COLOR = ofColor(255,255,255);
    int SETTINGS_PATH_ALPHA = 25;
    int SETTINGS_PATH_DURATION = 1000;


    //// weibo / instagram photos ////
    float SETTINGS_PHOTOS_MAXSIZE = 9.0;
    float SETTINGS_PHOTOS_MINSIZE = 3.0;
    float SETTINGS_PHOTOS_MAXLIKES = 3000;
    float SETTINGS_PHOTOS_MINLIKES = 50;
    int SETTINGS_PHOTOS_ALPHA = 50;
    int SETTINGS_PHOTOS_IMAGE_ALPHA = 150;

    // weibo topics
    ofColor SETTINGS_WEIBO_TOPICS_LABEL_COLOR = ofColor(255,255,255,80);

    //// twitter ////
    int SETTINGS_TWITTER_CUTOFF_FOLLOWERS = 100;

    int SETTINGS_TWITTER_ALPHA = 50;
    int SETTINGS_TWITTER_MIN_ALPHA = 30;
    int SETTINGS_TWITTER_MAX_ALPHA = 120;

    int SETTINGS_TWITTER_MIN_FOLLOWERS = 500;
    int SETTINGS_TWITTER_MAX_FOLLOWERS = 3000;
    float SETTINGS_TWITTER_MIN_SIZE = 4.0;
    float SETTINGS_TWITTER_MAX_SIZE = 9.0;

    int SETTINGS_TWITTER_FOLLOWERS_SHOW_SVG = 3000;

    int SETTINGS_TWITTER_SVG_SIZE = 5.0;
    float SETTINGS_TWITTER_HEIGHT_FACTOR = 1.0; // grow slower than weibo
    ofColor SETTINGS_TWITTER_COLOR = ofColor(94,169,221);

    //// BASE MAP ////
    int SETTINGS_BASE_ALPHA = 100;
    ofColor SETTINGS_BASE_COLOR = ofColor(0,0,0);

    //// CLUSTERS ////
    int SETTINGS_CLUSTERS_ALPHA = 20;


    //ofLogNotice() << "Babel::addGeometry: added to gridcell: [" << gridcell->x << "," << gridcell->y << "]";

    // MAIN DATAFEED TEMPLATES
    // depending on table, type and datapoints handle data in some way

    // adds geometry (reference) to babel system

    GridCell* gridcell = addToGrid(g); // add to grid

    float height = gridcell->getNumGeometries() * HEIGHT_GROWTH_FACTOR;
    g->positionOrig.z = height; // set original height

    //////// WEIBO /////////

    if (g->table == DATASET_WEIBO || g->table == DATASET_INSTAGRAM)
    {

        // Z ~ GRID
        g->setPositionOrigZ(height);

        // FLOAT
        g->setFloatRotation(ofVec3f(ofRandom(1,3.0), ofRandom(1.0,3.0), ofRandom(1.0,3.0)));
        g->position = ofVec3f( ofRandom(-1500,1500), ofRandom(500,2000), ofRandom(-500, 500) );

        // COLOR ~ TIME OF DAY
        Poco::DateTime date = epochToDateTime(g->time);
        int hour = date.hour();
        g->setColor(hourToColor(hour));


        // SIZE ~ LIKES
        int likes = ofToInt(g->data["likes"]);
        g->radius = ofClamp( ofMap(likes, SETTINGS_PHOTOS_MINLIKES, SETTINGS_PHOTOS_MAXLIKES, SETTINGS_PHOTOS_MINSIZE, SETTINGS_PHOTOS_MAXSIZE), SETTINGS_PHOTOS_MINSIZE, SETTINGS_PHOTOS_MAXSIZE);
        g->imageSize = ofClamp( ofMap(likes, SETTINGS_PHOTOS_MINLIKES, SETTINGS_PHOTOS_MAXLIKES, SETTINGS_PHOTOS_MINSIZE, SETTINGS_PHOTOS_MAXSIZE), SETTINGS_PHOTOS_MINSIZE, SETTINGS_PHOTOS_MAXSIZE);


        // POPULAR ONES: BRIGHTER
        if(likes > 5000)
            g->brightenColor();


        g->facing = true;

        g->alpha = 0;
        g->animateToOriginalPosition(500.0, {{ "Z" , height }, { "A",  SETTINGS_PHOTOS_ALPHA }}); // animate to original position

        if (likes >= 5000)
        {
            // photos: automatically set
        }
        else {
            // svgs
            //g->setSvg(&svgs["instagram"]);
            //g->setFacing(); // seams not really usefull
            //g->setLabel(g->data["text"], fontRenderer);
        }

        g->imageAlpha = SETTINGS_PHOTOS_IMAGE_ALPHA;
        //g->setLabel((string) "BLA", fontRenderer);
        //g->labelColor = ofColor(255,255,255,100);

        //////// ADD ///////////
        geometries.push_back(g);
    }


        // adds line from this geometry to previous one in gridcell


        /*

        if ( gridcell->geometries.size() > 1 )
        {
                int i = gridcell->geometries.size() - 2;
                Geometry* prevGeom = gridcell->geometries[ i ];
                newGeometries.push_back( Geometry(  ofVec3f(prevGeom->positionOrig.x, prevGeom->positionOrig.y, prevGeom->positionOrig.z) , g->positionOrig , ofColor(255,255,255), ofColor(255,255,255) ) );
                //newGeometries.push_back( Geometry( ofVec3f(0,0,0) , g->positionOrig, ofColor(255,255,255), ofColor(255,255,255) ) );

                Geometry* lastGeom = &(newGeometries[newGeometries.size() -1]);

                //lastGeom->length = 0.5;
                lastGeom->alpha = 50;
                // THE ANIMATION SEEMS TO CRASH EVERYHING; TWEEN.UPDATE()
                //lastGeom->animateTo({{ "L", 1.0 },{"A", SETTINGS_PATH_ALPHA }}, SETTINGS_PATH_DURATION);
                //ofExit();
        }
        */



        /* generate test geoms extra */
        /*
        for (int c = 0; c < 1; c++)
        {
            //newGeometries.push_back( Geometry( ofVec3f(ofRandom(-400,500), ofRandom(400,-500), ofRandom(400,-500)))  );
            newGeometries.push_back( Geometry( ofVec3f( ofRandom(-400,400) , ofRandom(-400,400) , ofRandom(-400,400)) , ofVec3f( ofRandom(-400,400) , ofRandom(-400,400), ofRandom(-400,400)), ofColor(255,255,255), ofColor(255,255,255) ) );
        }

        for (int c = 0; c < newGeometries.size(); c++)
        {
            //newGeometries[c].animateTo( {{ "Z", ofRandom(-400,400) }}, 1000);
            newGeometries[c].length = 0.0;
            newGeometries[c].animateTo({{ "L", 1.0 },{"A", SETTINGS_PATH_ALPHA }}, SETTINGS_PATH_DURATION);
        }
        */

    //////// WEIBO_PATHS /////////
    else if(g->table == DATASET_WEIBO_PATHS  || g->table == DATASET_INSTAGRAM_PATHS ) //
    {
        // linestrings
        g->strokeColor = ofColor(255,255,255);
        g->alpha = SETTINGS_PATH_ALPHA;
        g->length = 0.0;
        //g->animateToOriginalPosition(1000.0, {{ "A",  128 }});
        g->animateTo({{ "L", 1.0 },{"A", SETTINGS_PATH_ALPHA }}, SETTINGS_PATH_DURATION);
        //g->setFacing();
        //g->setFloatRotation(ofVec3f(ofRandom(1,3.0), ofRandom(1.0,3.0), ofRandom(1.0,3.0)));

        //////// ADD ///////////
        geometries.push_back(g);

    }
    else if(g->table == DATASET_WEIBO_TOPICS)
    {
        // Z ~ GRID
        g->setPositionOrigZ(height);
        g->radius = 3.0;
        g->fillColor = SETTINGS_WEIBO_TOPICS_LABEL_COLOR;
        g->setLabel(g->data["topic"], 9.0, SETTINGS_WEIBO_TOPICS_LABEL_COLOR, fontRenderer);
        g->labelFacing = false;

        // FLOAT
        g->setFloatRotation(ofVec3f(ofRandom(0.5,1.0), ofRandom(0.5,1.0), ofRandom(0.5,1.0)));
        g->position = ofVec3f( ofRandom(-1500,1500), ofRandom(-500,1000), ofRandom(-500, 500) );
        g->alpha = 0;
        g->animateToOriginalPosition(500.0, {{ "Z" , height }, { "A",  100 }}); // animate to original position

        //////// ADD ///////////
        geometries.push_back(g);

    }
    else if(g->table == DATASET_TWITTER)
    {
        int followers = ofToInt(g->data["followers"]);

        // CUT SOME LOW FOLLOWER TWEETS OUT FOR SPEED
        if (followers > SETTINGS_TWITTER_CUTOFF_FOLLOWERS)
        {
            // Z ~ GRID
            g->setPositionOrigZ(height*SETTINGS_TWITTER_HEIGHT_FACTOR);
            g->setFloatRotation(ofVec3f(ofRandom(0.7,1.3), ofRandom(0.7,1.3), ofRandom(0.7,1.3)));

             Poco::DateTime date = epochToDateTime(g->time);
            int hour = date.hour();
            g->setColor(hourToColor(hour));
            //g->setFacing();
            //g->fillColor = SETTINGS_TWITTER_COLOR; // ofColor(94,169,221);

            // RADIUS
            g->radius = ofClamp( ofMap(followers, SETTINGS_TWITTER_MIN_FOLLOWERS, SETTINGS_TWITTER_MAX_FOLLOWERS, SETTINGS_TWITTER_MIN_SIZE, SETTINGS_TWITTER_MAX_SIZE), SETTINGS_TWITTER_MIN_SIZE, SETTINGS_TWITTER_MAX_SIZE);

            // SVG OF MOST POPULAR ONES
            if (followers > SETTINGS_TWITTER_FOLLOWERS_SHOW_SVG){
                g->setSvg(&svgs["twitter"], SETTINGS_TWITTER_SVG_SIZE);}

            // FLOAT
            g->position = ofVec3f( ofRandom(-1500,1500), ofRandom(500,1500), ofRandom(-500, 500) );
            g->alpha = 0;
            g->animateToOriginalPosition(500.0, {{ "Z" , height*SETTINGS_TWITTER_HEIGHT_FACTOR }, { "A",  SETTINGS_TWITTER_ALPHA }}); // animate to original position

            //////// ADD ///////////
            geometries.push_back(g);
        }
    }

    else if (g->table == "base_shz" || g->table == "base_sp")
    {

        g->strokeColor = SETTINGS_BASE_COLOR;
        g->fillColor = SETTINGS_BASE_COLOR;
        g->alpha = SETTINGS_BASE_ALPHA;
        g->requestStaticRenderer();
        //g->animateTo({{"A", 255 }}, int(ofRandom(50,100)) );


        //////// ADD ///////////
        geometries.push_back(g);
    }

    //// CLUSTERS ////

    //// CLUSTER STRUCTURES ////

    if (g->type == "point")
    {
        // if we add a point: create lines from that point to all nearest point within a radius

        hashClusterResults.resize(CLUSTERS_SCAN_RADIUS_ESTIMATED_POINTS);
        hash.findPointsWithinRadius( g->positionOrig, CLUSTERS_SCAN_RADIUS, hashClusterResults); // find points near the point

        // now create lines (starting from old point)

        for (int i = 0; i < hashClusterResults.size(); i++)
        {
            int pointIndex = hashClusterResults[i].first;

            if (pointIndex < hashedPoints.size())
            {
                Geometry* prevGeom = pointsById[ hashedPoints[pointIndex][3] ];

                // only between points with more or less the same hour
                Poco::DateTime date = epochToDateTime(g->time);
                Poco::DateTime prevDate = epochToDateTime(prevGeom->time);

                int hourDiff = abs( prevDate.hour() - date.hour());
                //int dayDiff = abs( prevDate.day() - date.day());
                float heightDiff = g->positionOrig.z - prevGeom->positionOrig.z;

                if (hourDiff < CLUSTERS_MAX_TIME_DIFFERENCE &&  heightDiff > 2.0) // dayDiff > CLUSTERS_MIN_DAY_DIFFERENCE && dayDiff < CLUSTERS_MAX_DAY_DIFFERENCE
                {
                    newGeometries.push_back( Geometry(  ofVec3f(prevGeom->positionOrig.x, prevGeom->positionOrig.y, prevGeom->positionOrig.z) , ofVec3f(g->positionOrig.x, g->positionOrig.y, g->positionOrig.z), ofColor(255,255,255), ofColor(255,255,255) ) );
                    //newGeometries.push_back( Geometry( ofVec3f(0,0,0) , g->positionOrig, ofColor(255,255,255), ofColor(255,255,255) ) );

                    Geometry* lastGeom = &(newGeometries[newGeometries.size() -1]);
                    //lastGeom->strokeColor = ofColor(255,255,0);
                    lastGeom->alpha = SETTINGS_CLUSTERS_ALPHA;
                    lastGeom->setColor(hourToColor(date.hour()));
                    lastGeom->requestStaticRenderer();

                    //geometries.push_back(&(newGeometries[newGeometries.size() -1]));

                    // lastGeom->length = 0.0;
                    // lastGeom->animateTo({{"A", 255 }}, 1000 );

                }
            }
        }


    }

    //// HANDLE HASH ////

    if (g->type == "point")
    {

        pointGeometriesCount++; // new id for added point geometry
        hashedPoints.push_back( ofVec4f(g->positionOrig.x, g->positionOrig.y, g->positionOrig.z, float(pointGeometriesCount)) ); // add to base points for hash
        pointsById[pointGeometriesCount] = g; // to refind original geometry from id
        addedPointsCountForHash++;

        if (addedPointsCountForHash >= MAX_HASH_QUEUE)
        {
            // rebuild hash
            hash.buildIndex();

            addedPointsCountForHash = 0;

            ofLogNotice("Babel") << "Rebuilded hash with " << ofToString(hashedPoints.size()) << " points";
        }

    }

}

// ----

ofColor Babel::hourToColor(int hour)
{
    ofColor newColor;

    if (hour <= 3 )
    {    // night
        //newColor = interpolateColor(hour, 0.0, 3.0, ofColor(200,0,0), ofColor(20,50,200));
        //newColor = ofColor(60,20,66);
        newColor = ofColor(0,20,100);
    }
    else if (hour <= 6 )
    {
        //newColor = interpolateColor(hour, 3.0, 6.0, ofColor(20,50,200), ofColor(0,50,100));
        //g->fillColor = ofColor(0,0,0);
        //newColor = ofColor(8,99,117);
        newColor = ofColor(56,97,140);
    }
    else if( hour <= 11)
    {
        //newColor = interpolateColor(hour, 6.0, 10.0, ofColor(8,99,117), ofColor(255,255,255));
        //newColor = ofColor(8,99,117);
        newColor = ofColor(255,255,255);
    }
    else if ( hour <= 16)
    {
        //newColor = interpolateColor(hour, 10.0, 16.0, ofColor(137,200,240), ofColor(255,255,255));
        //g->fillColor = ofColor(255,255,0);
        //newColor = ofColor(29,211,176);
        newColor = ofColor(255,231,76);
    }
    else if ( hour <= 21)
    {
        //newColor = interpolateColor(hour, 12.0, 16.0, ofColor(255,255,255), ofColor(255,255,0));
        //g->fillColor = ofColor(255,255,0);
        //newColor = ofColor(255,89,100);
        newColor = interpolateColor(hour, 16.0, 21.0, ofColor(255,231,76), ofColor(255,89,100));
    }
    else if ( hour < 24){ // nightlife
            //newColor = interpolateColor(hour, 21.0, 24.0, ofColor(255,0,100), ofColor(255,0,0));
            newColor = ofColor(53,167,255);
            }

    return newColor;
}

// ----

ofColor Babel::interpolateColor(float val, float valMin, float valMax, ofColor cMin, ofColor cMax)
{
    // whatColor = "FILL" / "STROKE" / "BOTH"

    float dr = (cMax.r - cMin.r);
    float dg = (cMax.g - cMin.g);
    float db = (cMax.b - cMin.b);
    float da = (cMax.a - cMin.a);
    // TODO: ALPHA???

    float f = ofMap(val, valMin, valMax, 0.0, 1.0, true);


    ofColor newColor = ofColor( int ( cMin.r + dr * f ), int ( cMin.g + dg * f) , int ( cMin.b + db * f), int (cMin.a + da * f) );

    return newColor;

}

// ----

GridCell* Babel::addToGrid(Geometry* g)
{
    // add reference to grid vector

    //ofLogNotice() << "Babel::addToGrid: bboxLeft: " << bbox[0] << " bboxBottom: " << bbox[1] << " x:" << g->positionOrig.x << " y:" << g->positionOrig.y;

    int gridX = floor ( (g->positionOrig.x - bbox[0]) / gridCellSize );
    int gridY = floor ( (g->positionOrig.y - bbox[1]) / gridCellSize );

    // protect
    if (gridX < 0){ gridX = 0; }
    if (gridX > gridWidthCells){ gridX = gridWidthCells - 1; }
    if (gridY < 0){ gridY = 0; }
    if (gridY > gridHeightCells){ gridY = gridHeightCells - 1; }


    //ofLogNotice() << "gridX: " << gridX << " gridY: " << gridY << endl;

    int index = gridY * gridWidthCells + gridX;

    grid[index].geometries.push_back(g); // push reference to geometry to geometries vector in cell object

    // return gridcell reference
    return &grid[index];
}

// ----

void Babel::setupGrid(vector<float> b, float cellSize)
{

    bbox = b; // save bbox

    //ofLogNotice() << bbox;

    gridCellSize = cellSize; // set cellSize

    // calculate number of cells in width and height
    gridWidthCells = ceil(  (bbox[2] - bbox[0]) / gridCellSize);
    gridHeightCells = ceil(  (bbox[3] - bbox[1]) / gridCellSize);

    // initialize the grid

    // index = y * gridWidthCells + x
    for (int y = 0; y < gridHeightCells; y++)
    {
        for (int x = 0; x < gridWidthCells; x++)
        {
            grid.push_back( GridCell(x,y) );
        }
    }

    // DEBUG
    /*
    for (int i = 0; i < grid.size(); i++)
    {
        ofLogNotice() << "GRIDCELL " << grid[i].x << " " << grid[i].y;
    }
    */

    ofLogNotice() << "Babel::setupGrid:  made grid: " << gridWidthCells << " x " << gridHeightCells;
}

// ----

void Babel::resetGrid()
{
    // keep grid, but set stack count to 0
    for (int c = 0; c < grid.size(); c++)
    {
        grid[c].geometries.clear(); // remove geometry references
    }
}

// ----

void Babel::checkRenderers()
{

    // check if the geometries requested a render change
    renderCheckWaitCount++;

    if ( renderCheckWaitCount > RENDERCHECK_NUMFRAMES)
    {
        ofLogNotice() << "Babel::checkRenderers on " << geometries.size() << " geometries";

        bool pointsAdded = false;
        bool linesAdded = false;
        bool polysAdded = false;

        int numPointsAdded = 0;
        int numLinesAdded = 0;
        int numPolysAdded = 0;

        int polyVerticesAdded = 0;

        for (int g = 0; g < geometries.size(); g++)
        {
            string requestedRenderer = geometries[g]->requestedRenderer;
            string renderer = geometries[g]->renderer;

            if ( requestedRenderer != renderer ) // make sure we don't do things twice
            {
                if(requestedRenderer == "static")
                {
                    // add geometry to vbo depending on geometry type

                    if ( geometries[g]->type == "point")
                    {
                        vboPointPositions.push_back(geometries[g]->positionOrig);
                        vboPointSizes.push_back(ofVec3f(geometries[g]->radius));
                        vboPointColors.push_back(geometries[g]->getFloatFillColor() );
                        // TODO set vbo index?
                        pointsAdded = true;
                        numPointsAdded++;
                    }
                    else if ( geometries[g]->type == "linestring")
                    {
                        vboLinePositions.push_back(geometries[g]->points[0]);
                        vboLinePositions.push_back(geometries[g]->points[1]);
                        vboLineColors.push_back(geometries[g]->getFloatStrokeColor());
                        vboLineColors.push_back(geometries[g]->getFloatStrokeColor());

                        linesAdded = true;
                        numLinesAdded++;
                    }
                    else if (geometries[g]->type == "polygon")
                    {
                        // for now straight copy data from data into buffer
                        for (int p = 0; p < geometries[g]->points.size(); p++)
                        {
                            vboPolyPositions.push_back(geometries[g]->points[p]);
                            vboPolyColors.push_back(geometries[g]->getFloatStrokeColor());

                            // last one: signify new polygon
                            if ( p == geometries[g]->points.size() - 1 )
                            {
                                vboPolyIndices.push_back(0xffff);
                            }
                            else {
                                vboPolyIndices.push_back(polyVerticesAdded);
                            }

                            polyVerticesAdded++;
                        }

                        polysAdded = true;
                        numPolysAdded++;
                    }

                    geometries[g]->renderer = "static";
                }
                else if (requestedRenderer == "dynamic")
                {
                    // change from static to dynamic rendering
                    // TODO
                }

                // reset requested renderer: things are now managed
                geometries[g]->resetRequestedRenderer();
            }
        }

        // HACK: TODO BETTER!!!!!!!!!!!!

        for (int g = 0; g < newGeometries.size(); g++)
        {
            string requestedRenderer = newGeometries[g].requestedRenderer;
            string renderer = newGeometries[g].renderer;

            if ( requestedRenderer != renderer ) // make sure we don't do things twice
            {
                if(requestedRenderer == "static")
                {
                    // add geometry to vbo depending on geometry type

                    if ( newGeometries[g].type == "point")
                    {
                        vboPointPositions.push_back(newGeometries[g].positionOrig);
                        vboPointSizes.push_back(ofVec3f(newGeometries[g].radius));
                        vboPointColors.push_back(newGeometries[g].getFloatFillColor() );
                        // TODO set vbo index?
                        pointsAdded = true;
                        numPointsAdded++;
                    }
                    else if ( newGeometries[g].type == "linestring")
                    {
                        vboLinePositions.push_back(newGeometries[g].points[0]);
                        vboLinePositions.push_back(newGeometries[g].points[1]);
                        vboLineColors.push_back(newGeometries[g].getFloatStrokeColor());
                        vboLineColors.push_back(newGeometries[g].getFloatStrokeColor());

                        linesAdded = true;
                        numLinesAdded++;
                    }
                    else if (newGeometries[g].type == "polygon")
                    {
                        // for now straight copy data from data into buffer
                        for (int p = 0; p < newGeometries[g].points.size(); p++)
                        {
                            vboPolyPositions.push_back(newGeometries[g].points[p]);
                            vboPolyColors.push_back(newGeometries[g].getFloatStrokeColor());

                            // last one: signify new polygon
                            if ( p == newGeometries[g].points.size() - 1 )
                            {
                                vboPolyIndices.push_back(0xffff);
                            }
                            else {
                                vboPolyIndices.push_back(polyVerticesAdded);
                            }

                            polyVerticesAdded++;
                        }

                        polysAdded = true;
                        numPolysAdded++;
                    }

                    newGeometries[g].renderer = "static";
                }
                else if (requestedRenderer == "dynamic")
                {
                    // change from static to dynamic rendering
                    // TODO
                }

                // reset requested renderer: things are now managed
                newGeometries[g].resetRequestedRenderer();
            }
        }

        int totalPoints = (int)vboPointPositions.size();
        int totalLines = (int)vboLinePositions.size();
        int totalPolyPoints = (int)vboPolyPositions.size();

        // upload all geom data to VBO
        if (pointsAdded)
        {
            // upload to vbo
            vboPoints.clear();
            vboPoints.setVertexData(&vboPointPositions[0], totalPoints, GL_STATIC_DRAW);
            vboPoints.setNormalData(&vboPointSizes[0], totalPoints, GL_STATIC_DRAW);
            vboPoints.setColorData(&vboPointColors[0], totalPoints, GL_STATIC_DRAW);
        }

        if ( linesAdded )
        {
            vboLines.clear();
            vboLines.setVertexData(&vboLinePositions[0], totalLines, GL_STATIC_DRAW);
            vboLines.setColorData(&vboLineColors[0], totalLines, GL_STATIC_DRAW);
        }

        if ( polysAdded )
        {
            vboPolys.clear();
            vboPolys.setVertexData(&vboPolyPositions[0], totalPolyPoints, GL_STATIC_DRAW);
            vboPolys.setColorData(&vboPolyColors[0], totalPolyPoints, GL_STATIC_DRAW);
            vboPolys.setIndexData(&vboPolyIndices[0], totalPolyPoints, GL_STATIC_DRAW);
        }

        // stats
        if (pointsAdded || linesAdded || polysAdded)
        {
            //ofLogNotice() << "Babel::checkRenders: ==> uploaded " << numPointsAdded << " points - " <<  numLinesAdded << " lines - " << polysAdded << " polys";
            ofLogNotice() << "Babel::checkRenderers VBO sizes: points " << totalPoints <<  "- lines " << totalLines <<  " - polys points " << totalPolyPoints;
        }


        // reset counter
        renderCheckWaitCount = 0;
    }

}

// ----

void Babel::resetVbo()
{
    vboLinePositions.clear();
    vboLineColors.clear();
    vboPointPositions.clear();
    vboPointColors.clear();
    vboPointSizes.clear();

    vboLines.clear();
    vboPoints.clear();
}

// ----

void Babel::animateToOriginal(float duration, map<string,float> targets)
{
    // NOTE: animation duration is in frames

    for (int g = 0; g < geometries.size(); g++)
    {
        //ofLogNotice() << "===> DRAW" <<  geometries[g]->position;
        geometries[g]->animateToOriginalPosition(duration, targets);
    }
}

// ----

void Babel::loadSvgs(vector<string> filenames)
{
    // load svgs
    for (int c = 0; c < filenames.size(); c++)
    {
        ofFile file;
        file.open(ofToDataPath(filenames[c]));

        if (file.exists())
        {
            ofxSVG s;
            s.load(filenames[c]);
            string f = filenames[c];
            ofStringReplace(f, ".svg", "");
            svgs[f] = s;

            ofLogNotice() << "Babel::loadSvgs: SVG loaded: " << filenames[c];
        }
        else {
            ofLogWarning() << "Babel::loadSvgs: SVG does not exist: " << filenames[c];
        }
    }
}

// ----

void Babel::cacheImages(vector<Geometry>* vectorGeoms)
{
    // cache some images
    for (int g = 0; g <  vectorGeoms->size(); g++)
    {
        string l = vectorGeoms->at(g).data["likes"];

        if (l != "None")
        {
            int numLikes = ofToInt(l);

            if (numLikes > 100)
            {
                string t = vectorGeoms->at(g).data["img"];

                vectorGeoms->at(g).loadImage(t);
            }
        }
    }

}

// ----

void Babel::playSoundtrack()
{
    ofFile file;
    file.open("soundtrack.wav");

    if(file.exists())
    {
        soundtrack.loadSound("soundtrack.wav");
        soundtrack.setLoop(true);
        soundtrack.play();
        soundtrack.setVolume(SOUNDTRACK_VOLUME);
    }
}

// ----

Poco::DateTime Babel::epochToDateTime(unsigned int t)
{
    Poco::Timestamp ts = Poco::Timestamp();
    ts = ts.fromEpochTime(t);

    Poco::DateTime dt = Poco::DateTime(ts);

    return dt;
}

// ----

/*
void Babel::setupFontRenderer()
{
    ofLogNotice() << "Babel::setupFrontRenderer: ";
    fontRenderer.setup("Arial Unicode.ttf", 1.0, 1024, true, 8, 2.0f);
}
*/
// ----


void Babel::setFontRenderer(ofxFontStash* f)
{
    fontRenderer = f;
}

