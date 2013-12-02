#ifndef CORNERTABLE_H
#define CORNERTABLE_H

#include <vector>


class CornerTable
{
public:

    CornerTable();
    ~CornerTable();

    void buildTable(const std::vector<int>& triangles);
    bool checkTable() const;

    int triangle(int corner) const;
    int vertex  (int corner) const;
    int opposite(int corner) const;

    int next(int corner) const;
    int prev(int corner) const;

    int  clockwise(int corner) const;
    int  counterclockwise(int corner) const;

    void cornersLoop (int corner, std::vector<int>& cloop) const;
    void corners1ring(int corner, std::vector<int>& cring) const;

    int valence(int corner) const;

    std::vector<int>& getVTable();
    std::vector<int>& getOTable();

private:

    std::vector<int> vTable;    //vertex table
    std::vector<int> oTable;    //opposites table

};


inline int CornerTable::triangle(int c) const {
    if (c < 0 || c >= int(vTable.size())) return -1;
    return c/3;
}

inline int CornerTable::vertex(int c) const {
    if (c < 0 || c >= int(vTable.size())) return -1;
    return vTable[c];
}

inline int CornerTable::opposite(int c) const {
    if (c < 0 || c >= int(oTable.size())) return -1;
    return oTable[c];
}

inline int CornerTable::next(int c) const {
    return (c + 1)%3 + 3*triangle(c);
}

inline int CornerTable::prev(int c) const {
    return (c + 2)%3 + 3*triangle(c);
}

inline int CornerTable::clockwise(int c) const {
    return next(opposite(next(c)));
}

inline int CornerTable::counterclockwise(int c) const {
    return prev(opposite(prev(c)));
}

inline std::vector<int>& CornerTable::getVTable() {
    return vTable;
}

inline std::vector<int>& CornerTable::getOTable() {
    return oTable;
}


#endif // CORNERTABLE_H
