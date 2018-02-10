#include "DataFeeder.h"

DataFeeder::DataFeeder()
{
    // constructor
    // TODO
    feeding = false;
}

// ----

DataFeeder::DataFeeder(string fileName)
{
    // directly load data
    loadData(fileName);

    makeStats();

}

// ----

void DataFeeder::connectBabel(Babel* b)
{
    babel = b;
}


// ----

void DataFeeder::start()
{
    // setup font
    font.loadFont("Arial Unicode.ttf", 14);

    feedTime = timeStart; // set cursor
    feedDateTime = epochToDateTime(feedTime);

    feedIndex = 0;
    lastFrameTime = time(0);

    feeding = true;

    ofLogNotice() << "DataFeeder::start" << " Started feed from " << getFeedTimeFormatted();
}

// ----

void DataFeeder::stop()
{
    ofLogNotice() << "DataFeeder::stop: data stopped";
    feeding = false;

}

// ----

void DataFeeder::restart()
{
    ofLogNotice() << "DataFeeder::restart";

    feedTime = timeStart;
    feedDateTime = epochToDateTime(feedTime);

    lastFrameTime = time(0);
    feedIndex = 0;

    feeding = true;

}

// ----

string DataFeeder::loadData(string fileName)
{
    ofFile file;
    file.open(fileName);

    if( !file.exists() || !file.canRead() )
    {
        ofLogError("DataFeeder: Can't open xml file");
        ofExit();
    }

    // continue with open file
    string line;
    ofXml lineXml;
    string tableName;
    string table;

    while (getline(file, line))
    {
        // process XML within string
        lineXml.loadFromBuffer(line); // load into XML object

        int numElems = lineXml.getNumChildren();

        string elemName;
        string elemValue;
        string geomType;
        string geomCoords;
        int time;
        map<string,string> geomData;

        // find dataproperties
        for (int c = 0; c < numElems; c++)
        {
            if (!c)
                lineXml.setToChild(c);
            else
                lineXml.setToSibling();

            // do some ordering of data

            elemName = lineXml.getName();

            // NOTE: zero length values crash!
            try {
                elemValue = lineXml.getValue();
            }
            catch (const out_of_range &e)
            {
                    ofLogNotice("DataFeeder::loadData") << "Empty xml tag";
                    elemValue = "";
            }
            // TODO: extra protect

            if (elemName == "" && elemValue == "")
                ofLogNotice() << " empty geom";

            if ( elemName == "geometry")
            {
                geomCoords = elemValue;
                geomType = lineXml.getAttribute("type");
            }
            else if( elemName == "time")
            {
                time = ofToInt(elemValue);
            }
            else if( elemName == "table")
            {
                table = elemValue;
            }
            else {
                // easy: just save in map
                if (elemName != "")
                    geomData[elemName] = elemValue;

            }
        }

        // now we have all the data: geomType, geomCoords and the rest of data (like time)

        //vector<Geometry> geometries;

        Geometry newGeom = Geometry(table, geomType, geomCoords, time, ofColor(255,0,0), ofColor(255,0,0) );
        newGeom.setData(geomData);

        geometries.push_back( newGeom ); // add geometry to main vector

    }

    ofLogNotice() << "NUM GEOMETRIES LOADED: " << geometries.size();

    return table;
}

// ----

void DataFeeder::loadDataDirect(string fileName, Babel* b, int gs)
{
    ofLogNotice("DataFeeder::loadDataDirect") << fileName;

    connectBabel(b);

    string tableName = loadData(fileName); // load into geometries vector

    // we need to set up a grid, because directy adding geomety needs that
    makeStats();
    b->setupGrid( bbox , gs);

    ofLogNotice("DataFeeder::loadDataDirect") << "Start direct feed";

    vector <Geometry*> geoms = getGeometriesByTable(tableName); // get geometries

    for (int c = 0; c < geoms.size(); c++)
    {
        ofLogNotice("FEED BABEL") << geoms[c]->table << " index: " << ofToString(c);
        feedBabel(geoms[c]);
    }


}

// ----

void DataFeeder::makeStats()
{
    // cycle through geometries to detect bounding box, timerange and other stats

    float minX = 99999;
    float maxX = -99999;
    float minY = 99999;
    float maxY = -99999;

    unsigned int minTime = 4000000000; // 4,294,967,295 -- 1430111869
    unsigned int maxTime = 0;
    unsigned int geomTime;

    for (int g = 0; g < geometries.size(); g++)
    {
        Geometry curGeom = geometries[g];

        vector<ofPoint> points = curGeom.points;

        for (int p = 0; p < points.size(); p++)
        {
            // check coordinates of points
            ofPoint curPoint = points[p];

            if (curPoint.x > maxX)
                maxX = curPoint.x;
            if (curPoint.x < minX)
                minX = curPoint.x;
            if (curPoint.y > maxY)
                maxY = curPoint.y;
            if (curPoint.y < minY)
                minY = curPoint.y;

        }

        geomTime = curGeom.time;
        // time stats
        if (geomTime < minTime)
            minTime = geomTime;
        if (geomTime > maxTime)
            maxTime = geomTime;

    }

    // finish bbox
    // overwrite
    // bbox = { minX, minY, maxX, maxY };
    bbox = { -1000, -1000, 1000, 1000 };

    ofLogNotice() << "DataFeeder:makeStats: Bbox: [" << bbox[0] << " ," << bbox[1] << " ," << bbox[2] << " ," << bbox[3] << "]";

    // check time
    timeStart = minTime;
    timeEnd = maxTime;

    ofLogNotice() << "DataFeeder:makeStats: Time" << timeStart << " - " << timeEnd << " Total duration:" << (timeEnd - timeStart)/(3600*24) << " days";

    // we want to check if the geometries are in time order
    if ( timeStart != geometries[0].time || timeEnd != geometries[geometries.size() - 1].time)
    {
        ofLogWarning("Datafeeder:makeStats: geometries seem to be in non-time order");
        ofLogWarning() <<  "timeStart: " << timeStart << " " << geometries[0].time;
        ofLogWarning() <<  "timeEnd: " << timeEnd << " " << geometries[geometries.size() - 1].time;
    }
}

// ----

void DataFeeder::setTimeScale(int s)
{
    // set time scale: number of seconds per real second
    timeScale = s; // like 1 s = 1 hour 3600 or one day: 86400
}

// ----

void DataFeeder::setTimeRange(unsigned int s, unsigned int e)
{
    timeStart = s;
    timeEnd = e;
}

// ---

void DataFeeder::update()
{
    if (feeding)
    {
        // check time passed to trigger check for geometries
        checkTimeTrigger();
    }
}

// ----

void DataFeeder::draw()
{
    // draw date
    ofPushStyle();

    Poco::DateTime date = epochToDateTime(feedTime);
    int hour = date.hour();
    ofColor timeColor = babel->hourToColor(hour);
    ofSetColor(ofColor(timeColor.r, timeColor.g, timeColor.b, 100));

    //ofSetColor(255,255,255,100);
    ofSetDrawBitmapMode(OF_BITMAPMODE_SCREEN);
    //ofDrawBitmapString(getFeedTimeFormatted(), ofGetWidth()-100, DATE_ON_SCREEN_Y);
    font.drawString(getFeedTimeFormatted(), ofGetWidth()-170, ofGetHeight() - 40); // DATE_ON_SCREEN_Y
    ofPopStyle();
}

// ----

void DataFeeder::checkTimeTrigger()
{
    //ofLogNotice() << "DataFeeder::update" << endl;

    unsigned int curFrameTime = time(0);

    // check time passed
    if (curFrameTime - lastFrameTime >= CHECK_ADDGEOMETRY_PERIOD)
    {
        // trigger
        nextSecondOfGeometries();

        // update last frame
        lastFrameTime = curFrameTime;
    }
}

// ---

void DataFeeder::nextSecondOfGeometries()
{
    // place time cursor (1 * timeScale) seconds further and scan for geometries
    int newFeedTime = feedTime + 1 * timeScale;
    int geomCount = 0;

    //ofLogNotice() << "DataFeeder::nextSecondOfGeometries: newFeedTime:" << newFeedTime << " Geometry index: " << feedIndex;

    // check end
    if (feedIndex == geometries.size() - 1)
    {
            ofLogNotice() << "DataFeeder::nextSecondOfGeometries: Feed at end, stopped";
            //stop();
            // RESTART BABEL
            restart();
            babel->reset();
    }


    for (int g = feedIndex; g < geometries.size(); g++)
    {
        Geometry curGeom = geometries[g];
        unsigned int curGeomTime = curGeom.time;

        //ofLogNotice() << "CHECK TIME <==> GEOMTIME " << newFeedTime << "---" << curGeomTime;

        // CHECK END
        if (newFeedTime > timeEnd){
            ofLogNotice() << "Stopped datafeed";
            stop(); }

        // until newFeed time is reached
        if ( curGeomTime <  newFeedTime)
        {
            // add to babel
            // IMPORTANT: can't use curGeom reference, because it will dissappear
            feedBabel(&geometries[g]);

            geomCount++;
        }
        else {
            // stop
            feedTime = newFeedTime;
            feedIndex = g;

            ofLogNotice() << "ADDED " << geomCount << " geometries under time " << feedTime << " " << getFeedTimeFormatted();

            break;
        }
    }
}

// ----

vector<Geometry*> DataFeeder::getGeometriesByTable(string t)
{
    vector <Geometry*> tableGeoms;

    for (int c = 0; c < geometries.size(); c++)
    {
        if ( geometries[c].table == t)
        {
            tableGeoms.push_back(&geometries[c]);
        }
    }

    return tableGeoms;
}

// ----

void DataFeeder::feedBabel(Geometry* g)
{
    if (babel == NULL)
    {
        ofLogError() << "DataFeeder::feedBabel: ERROR: no babel reference given";
        return;
    }

    // if babel specified continue
    babel->addGeometry(g);

}

// ----

void DataFeeder::loadSvgs(vector<string> filenames)
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

string DataFeeder::getFeedTimeFormatted()
{
    feedDateTime = epochToDateTime(feedTime);

    string timeString = ofToString(feedDateTime.day()) + "-" + ofToString(feedDateTime.month()) + "-" + ofToString(feedDateTime.year()) + " @" +  ofToString(feedDateTime.hour()) + "h";

    return timeString;
}

// ----

Poco::DateTime DataFeeder::epochToDateTime(unsigned int t)
{
    /*
    struct tm tm ;
    strptime(ofToString(t).c_str(), "%s", &tm);
    time_t tt = mktime(&tm);
    */
    //cout << tt << endl;

    Poco::Timestamp ts = Poco::Timestamp();
    ts = ts.fromEpochTime(t);

    Poco::DateTime dt = Poco::DateTime(ts);

    return dt;
}
