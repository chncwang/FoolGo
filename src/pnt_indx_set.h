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
    void Init() {indxs_.flip();}
    void Remove(PointIndex indx) {indxs_.reset(indx);}

private:
    std::bitset<FOO_SQUARE(BOARD_LEN)> indxs_;

    DISALLOW_COPY_AND_ASSIGN(PntIndxSet);
};



#include "pnt_indx_set-TLT.h"

#endif
