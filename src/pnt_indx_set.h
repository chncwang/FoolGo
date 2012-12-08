#ifndef RNDRD_INDX_SET_H
#define RNDRD_INDX_SET_H

#include <bitset>

#include "def.h"
#include "position.h"



template <BoardLen BOARD_LEN>
class PntIndxSet
{
public:
    explicit PntIndxSet();
    void Init();
    void Copy(const PntIndxSet &s);

    void Remove(PointIndex indx);
    void Add(PointIndex indx);
    void Set(PointIndex indx, bool v);

private:
    std::bitset<BoardLenSquare<BOARD_LEN>()> indxs_;

    DISALLOW_COPY_AND_ASSIGN(PntIndxSet);
};



#include "pnt_indx_set-TLT.h"

#endif
