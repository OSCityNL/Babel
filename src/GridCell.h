#pragma once
#include "ofMain.h"
#include "Geometry.h"

class GridCell
{
    public:
        // CONSTRUCTOR
        GridCell(int cellx, int celly); // input vector of points

        // PUBLIC PROPERTIES
        int x;
        int y;
        int i;

        vector <Geometry*> geometries; // reference to geometries within this cell

        // METHODS

        // SETTERS

        // GETTERS
        int getNumGeometries();


     // PRIVATE PROPERTIES
     private:

};

