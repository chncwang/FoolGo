#include "pnt_indx_set.h"


template <BoardLen BOARD_LEN>
PntIndxSet<BOARD_LEN>::PntIndxSet() {}


template <BoardLen BOARD_LEN>
void PntIndxSet<BOARD_LEN>::Init()
{
    indxs_.flip();
}


template <BoardLen BOARD_LEN>
void PntIndxSet<BOARD_LEN>::Copy(const PntIndxSet &s)
{
    indxs_ = s.indxs_;
}


template <BoardLen BOARD_LEN>
void PntIndxSet<BOARD_LEN>::Remove(PointIndex indx)
{
    indxs_.reset(indx);
}


template <BoardLen BOARD_LEN>
void PntIndxSet<BOARD_LEN>::Add(PointIndex indx)
{
    indxs_.set(indx);
}


template <BoardLen BOARD_LEN>
void PntIndxSet<BOARD_LEN>::Set(PointIndex indx, bool v)
{
    indxs_[indx] = v;
}
