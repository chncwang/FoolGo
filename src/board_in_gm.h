#ifndef BOARD_IN_GM
#define BOARD_IN_GM

#include <bitset>
#include <vector>
#include <ostream>

#include "types_and_constants.h"
#include "board.h"
#include "chain_set.h"
#include "eye_set.h"
#include "brd_change.h"


struct Move {
    PlayerColor color_;
    PointIndex indx_;

    Move() : color_(0), indx_(0) {}
    Move(PlayerColor color, PointIndex indx) : color_(color), indx_(indx) {}
};


template <BoardLen BOARD_LEN> class BoardInGm;

template <BoardLen BOARD_LEN>
class BrdInGmDlgt {
public:
    virtual HashKeyType GetHash(const BoardInGm<BOARD_LEN> &b) const = 0;
    virtual HashKeyType
    GetHash(HashKeyType hash, const BrdChange &change) const = 0;
};



template <BoardLen BOARD_LEN>
class BoardInGm : private Board<BOARD_LEN>
{
public:
    typedef std::bitset<BoardLenSquare<BOARD_LEN>()> BitSet;

    static const PointIndex NONE = -1;

    BoardInGm() : ko_indx_(-1), last_player_(WHITE_PLAYER), b_pcs_c_(0),
                  delegate_(nullptr),
                  hash_key_(0) {}
    ~BoardInGm() = default;
    DISALLOW_COPY_AND_ASSIGN(BoardInGm);
    void Init();
    void Copy(const BoardInGm &b);

    Point GetPoint(PointIndex indx) const {
        return Board<BOARD_LEN>::GetPoint(indx);
    }
    Point GetPoint(const Position &pos) const {
        return Board<BOARD_LEN>::GetPoint(pos);
    }
    PlayerColor LastPlayer() const {return last_player_;}
    PointIndex KoIndex() const {return ko_indx_;}
    const BitSet &PlayableIndexes(PlayerColor color) const {
        return playable_indxs_[color];
    }
    PointIndex BlackRegion() const {
        return b_pcs_c_ + eye_sets_[BLACK_PLAYER].RealCount();
    }
    HashKeyType HashKey() const {return hash_key_;}

    void PlayMove(const Move &move);
    void Pass(PlayerColor color);

    // For test.
    friend std::ostream &operator <<(std::ostream &os,
                                     const BoardInGm<BOARD_LEN> &board) {
        return os << static_cast<const Board<BOARD_LEN> &>(board);
    }

private:
    typedef std::vector<PointIndex> PointIndxVector;

    ChainSet<BOARD_LEN> chain_sets_[2];
    BitSet playable_indxs_[2];
    EyeSet<BOARD_LEN> eye_sets_[2];
    PointIndex ko_indx_;
    PlayerColor last_player_;
    PointIndex b_pcs_c_;
    BrdInGmDlgt<BOARD_LEN> *delegate_;
    HashKeyType hash_key_;

    void LetAdjChnsSetAir(PointIndex indx, bool v);

    bool IsSelfPieceOrEye(const Move &move) const;

    bool IsEmptySingly(PointIndex indx) const;
    bool IsSuiside(const Move &move) const;
    bool IsPlayable(const Move &move) const;

    void PlayBasicMove(const Move &move, PointIndxVector *v,
                       PointIndxVector *sssdv);
    PointIndxVector RemoveChain(const Move &move);

    void UpdateEye(const Move &move);
    void UpdateRealEye(const Move &move);

    void UpdtAdjPlblIndxsOfChn(PointIndex indx);
    void UpdtPlblIndxsArnd(const Move &move);
    void UpdtAtePcsAdjChns(const PointIndxVector &v, PlayerColor ate_color);
};



template<BoardLen BOARD_LEN>
typename BoardInGm<BOARD_LEN>::BitSet
NokoPlayableIndexes(const BoardInGm<BOARD_LEN> &b, PlayerColor color)
{
    if (b.KoIndex() == BoardInGm<BOARD_LEN>::NONE) {
        return b.PlayableIndexes(color);
    } else {
        typename BoardInGm<BOARD_LEN>::BitSet kobits;
        kobits.set();
        kobits.reset(b.KoIndex());
        return kobits & b.PlayableIndexes(color);
    }
}


template <BoardLen BOARD_LEN>
inline bool IsEnd(const BoardInGm<BOARD_LEN> &b)
{
    return b.PlayableIndexes(BLACK_PLAYER).count() == 0 &&
           b.PlayableIndexes(WHITE_PLAYER).count() == 0;
}


template <BoardLen BOARD_LEN>
inline PlayerColor NextPlayer(const BoardInGm<BOARD_LEN> &b)
{
    return OppstColor(b.LastPlayer());
}


#include "board_in_gm-TLT.h"
#include "brd_change-TLT.h"

#endif
