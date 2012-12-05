#ifndef PIECES_STR_SET
#define PIECES_STR_SET

#include <bitset>
#include <vector>

#include "def.h"
#include "position.h"
#include "pos_cal.h"

typedef std::vector<PointIndex> PntIndxVector;
typedef char AirCount;



template <BoardLen BOARD_LEN>
class ChainSet
{
public:
    typedef std::bitset<FOO_SQUARE(BOARD_LEN)> AirSet;

    explicit ChainSet();
    ChainSet(const ChainSet &c);
    const ChainSet &operator =(const ChainSet &c);

    AirCount GetAirCountByPiece(PointIndex piece_i) const;
    AirCount GetAirCountByPiece(const Position &pos) const;
    PntIndxVector GetPieces(PointIndex piece_i) const;
    inline PntIndxVector GetPieces(const Position &pos) const;

    void AddPiece(const Position &pos, const AirSet &air_set);
    void LetAdjcntChainsSetAir(const Position &pos, bool v);
    void LetAdjcntChainsSetAir(PointIndex indx, bool v);
    void RemoveListByPiece(PointIndex piece_i);
    void RemoveListByPiece(const Position &pos);

#ifdef FOO_TEST
    void PRINT() const;
    static void TEST();
#endif

private:
    struct Node {
        PointIndex next_, list_head_;
    };
    struct List {
        PointIndex tail_, len_;
        AirSet air_set_;
    };

    void Copy(const ChainSet<BOARD_LEN> &c);

    PosCalculator<BOARD_LEN> &GetPosClcltr() const {
        return PosCalculator<BOARD_LEN>::Ins();
    }

    PointIndex GetListHead(PointIndex node_i) const {
        return nodes_[node_i].list_head_;
    }
    void CreateList(PointIndex node_i, const AirSet &air_set);
//    void AppendToList(PointIndex node_i, const AirSet &air_set,
//                      PointIndex list_i);

    PointIndex MergeLists(PointIndex list_a, PointIndex list_b);
//return the merged list head.

    void RemoveList(PointIndex head);

    AirCount GetAirCountOfAChain(PointIndex list_i) const;
    AirCount GetAirCountOfAChain(const Position &pos) const;
    PntIndxVector GetPiecesOfAChain(PointIndex list_i) const;

    static const PointIndex NONE_LIST = -1;

    Node nodes_[FOO_SQUARE(BOARD_LEN)];
    List lists_[FOO_SQUARE(BOARD_LEN)];
};



#include "chain_set-TLT.h"

#ifdef FOO_TEST
#include "chain_set_TEST.h"
#endif

#endif
