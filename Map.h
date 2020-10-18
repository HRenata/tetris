#ifndef MAP_H
#define MAP_H

class Map
{
public:
    static const int mN = 15;
    static const int mM = 20;
    static int mMap[mN][mM];
    static int mCellHeight;
    static int mCellWidth;
    static int mOffsetX;
    static int mOffsetY;

    static void clearMap();

    Map (Map &) = delete;
    void operator= (const Map &) = delete;
    Map (Map &&) = delete;
    Map() = delete;
};

#endif // MAP_H
