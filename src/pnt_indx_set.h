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
    void Remove(PointIndex indx);
    void Add(PointIndex indx);

private:
    std::bitset<BoardLenSquare<BOARD_LEN>()> indxs_;

    DISALLOW_COPY_AND_ASSIGN(PntIndxSet);
};



#include "pnt_indx_set-TLT.h"

#endif
