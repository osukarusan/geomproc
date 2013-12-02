#include "cornertable.h"
#include <map>
#include <utility>
#include <iostream>
#include <set>


CornerTable::CornerTable()
{
}

CornerTable::~CornerTable()
{
}

void CornerTable::buildTable(const std::vector<int>& tris)
{
    int n = tris.size();
    vTable = std::vector<int>(n, -1);
    oTable = std::vector<int>(n, -1);

    std::map<std::pair<int, int>, int> map;

    for (int i = 0; i < n; i += 3) {

        for (int j = 0; j < 3; j++) {

            int v1 = tris[i + j];
            int v2 = tris[i + (j + 1)%3];
            int c  = i + (j + 2)%3;
            std::pair<int, int> p(std::min(v1, v2), std::max(v1, v2));

            vTable[i + j] = tris[i + j];

            if (map.find(p) == map.end()) {
                map[p] = c;
            }
            else {
                int o = map[p];
                oTable[c] = o;
                oTable[o] = c;
                map.erase(map.find(p));
            }
        }
    }
}

int CornerTable::valence(int corner) const {
    std::set<int> vset;
    int c = corner;
    do {
        vset.insert(c);
        c = clockwise(c);
    } while (c >= 0 && c != corner);
    if (c < 0) {
        c = counterclockwise(corner);
        do {
            vset.insert(c);
            c = counterclockwise(c);
        } while (c >= 0 && c != corner);
    }
    return vset.size();
}

bool CornerTable::checkTable() const {
    for (int c = 0; c < int(vTable.size()); c++) {
        if (opposite(c) != -1 && opposite(opposite(c)) != c)
            return false;
    }
    return true;
}

void CornerTable::cornersLoop(int ci, std::vector<int> &cloop) const {
    int c = ci;
    do {
        cloop.push_back(c);
        c = clockwise(c);
    } while (c >= 0 && c != ci);
    if (c < 0) {
        c = counterclockwise(ci);
        while (c >= 0) {
            cloop.push_back(c);
            c = counterclockwise(c);
        }
    }
}

void CornerTable::corners1ring(int ci, std::vector<int> &cring) const {
    int c = ci;
    do {
        cring.push_back(next(c));
        c = clockwise(c);
    } while (c >= 0 && c != ci);
    if (c < 0) {
        c = ci;
        while (counterclockwise(c) >= 0) {
            c = counterclockwise(c);
            cring.push_back(next(c));
        }
        cring.push_back(prev(c));
    }
}
