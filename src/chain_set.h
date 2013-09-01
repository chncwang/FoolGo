#ifndef PIECES_STR_SET
#define PIECES_STR_SET

#include <bitset>
#include <vector>

#include "def.h"
#include "position.h"
#include "pos_cal.h"

typedef char AirCount;



template <BoardLen BOARD_LEN>
class ChainSet
{
public:
    typedef std::bitset<BoardLenSquare<BOARD_LEN>()> AirSet;
    typedef std::vector<PointIndex> PntIndxVector;

    ChainSet() = default;
    ~ChainSet() = default;
    DISALLOW_COPY_AND_ASSIGN(ChainSet);
    void Copy(const ChainSet &c);

    AirSet GetAirSetByPiece(PointIndex piece_i) const;
    AirCount GetAirCountByPiece(PointIndex piece_i) const;
    PntIndxVector GetPieces(PointIndex piece_i) const;

    void SetAir(PointIndex indx, PointIndex air_i, bool v);
    void AddPiece(PointIndex indx, const AirSet &air_set);
    void RemoveListByPiece(PointIndex piece_i);

#ifdef DTEST
    void PRINT() const;
    static void TEST();
#endif

private:
    static const PointIndex NONE_LIST = -1;

    struct Node {
        PointIndex next_, list_head_;
        Node() : list_head_(NONE_LIST) {}
    } nodes_[BoardLenSquare<BOARD_LEN>()];

    struct List {
        PointIndex tail_, len_;
        AirSet air_set_;
        AirCount air_count_;
    } lists_[BoardLenSquare<BOARD_LEN>()];

    inline PosCalculator<BOARD_LEN> &GetPosClcltr() const {
        return PosCalculator<BOARD_LEN>::Ins();
    }

    inline PointIndex GetListHead(PointIndex node_i) const {
        return nodes_[node_i].list_head_;
    }
    void CreateList(PointIndex node_i, const AirSet &air_set);

    PointIndex MergeLists(PointIndex head_a, PointIndex head_b);
//return the merged list head.

    void RemoveList(PointIndex head);

    AirSet GetAirSetOfChain(PointIndex head) const;
    AirCount GetAirCountOfChain(PointIndex list_i) const;
    PntIndxVector GetPiecesOfChain(PointIndex list_i) const;
};



#include "chain_set-TLT.h"

#ifdef DTEST
#include "chain_set_TEST.h"
#endif

#endif
