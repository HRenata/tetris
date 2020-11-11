#include "helper_class.h"

int Map::mMap[mN][mM] ={{0}};

int Map::mCellHeight = 25;
int Map::mCellWidth = 25;

int Map::mOffsetX = 80;
int Map::mOffsetY = 50;

void Map::clearMap()
{
    for(int i = 0; i < Map::mN; ++i)
    {
        for(int j = 0; j < Map::mM; ++j)
        {
            Map::mMap[i][j] = 0;
        }
    }
}
