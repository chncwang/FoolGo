#include "brd_change.h"


void BrdChange::SetNow(PointIndex ko, PointIndex move_i, bool now_empty,
                       const std::vector<PointIndex> &removed)
{
    last_player_.now_ = OppstColor(last_player_.origin_);
    ko_indx_.now_ = ko;
    int rsize = removed.size();
    indxs_.resize(rsize + 1);

    Point rc = (now_empty ? last_player_.now_ : last_player_.origin_);

    for (int i=0; i<rsize; ++i) {
        PointIndex ri = removed[i];
        indxs_[i].Set(ri, rc, EMPTY_POINT);
    }

    indxs_[rsize].Set(move_i, EMPTY_POINT, last_player_.now_);
}
