#include "GridCell.h"

GridCell::GridCell(int cellx,int celly)
{
    // constructor
    x = cellx;
    y = celly;
}

// ----

int GridCell::getNumGeometries()
{
    return geometries.size();
}
