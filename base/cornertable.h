#ifndef CORNERTABLE_H
#define CORNERTABLE_H

#include <vector>


class CornerTable
{
public:

    CornerTable();
    ~CornerTable();

    void buildTable(const std::vector<int>& triangles);

    int triangle(int corner) const;
    int vertex  (int corner) const;
    int opposite(int corner) const;

    int next(int corner) const;
    int prev(int corner) const;

private:

    std::vector<int> vTable;    //vertex table
    std::vector<int> oTable;    //opposites table

};


inline int CornerTable::triangle(int c) const {
    return c/3;
}

inline int CornerTable::vertex(int c) const {
    return vTable[c];
}

inline int CornerTable::opposite(int c) const {
    return oTable[c];
}

inline int CornerTable::next(int c) const {
    return (c + 1)%3 + 3*triangle(c);
}

inline int CornerTable::prev(int c) const {
    return (c - 1)%3 + 3*triangle(c);
}


#endif // CORNERTABLE_H
