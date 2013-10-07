#include "cornertable.h"
#include <map>
#include <utility>


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
            }
        }
    }

    // insertar la resta!

}
