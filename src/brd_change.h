#ifndef BRD_CHANGE_H
#define BRD_CHANGE_H

#include <vector>

#include "types_and_constants.h"
#include "position.h"
#include "board.h"



class BrdChange
{
public:
    template <typename T>
    struct Change {
        T origin_, now_;
    };

    struct Pair {
        PointIndex indx_;
        Change<Point> pnt_;

        inline void Set(PointIndex indx, Point orgn, Point nw) {
            indx_ = indx;
            pnt_.origin_ = orgn;
            pnt_.now_ = nw;
        }
    };

    BrdChange() = default;
    ~BrdChange() = default;
    DISALLOW_COPY_AND_ASSIGN(BrdChange);
    void Init(PlayerColor color, PointIndex ko);
    void SetNow(PointIndex ko, PointIndex move_i, bool now_empty,
                const std::vector<PointIndex> &removed);

    const Change<PointIndex> &KoChng() const {return ko_indx_;}
    const Change<PlayerColor> &LastPlayerChng() const {return last_player_;}
    const std::vector<Pair> &PointsChng() const {return indxs_;}

private:
    Change<PointIndex> ko_indx_;
    Change<PlayerColor> last_player_;
    std::vector<Pair> indxs_;
};



#include "brd_change-TLT.h"

#endif
