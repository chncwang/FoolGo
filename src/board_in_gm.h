#ifndef BOARD_IN_GM
#define BOARD_IN_GM

#include <stdint.h>

#include <bitset>
#include <vector>

#include "board.h"
#include "chain_set.h"
#include "eye_set.h"

typedef char PlayerColor;
const PlayerColor BLACK_PLAYER = 0;
const PlayerColor WHITE_PLAYER = 1;

typedef uint32_t HashKey;

PlayerColor OppstColor(PlayerColor color);

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
    virtual HashKey GetHash(const BoardInGm<BOARD_LEN> &b) const = 0;
};



template <BoardLen BOARD_LEN>
class BoardInGm : public Board<BOARD_LEN>
{
public:
    typedef std::bitset<BLSq<BOARD_LEN>()> BitSet;

    static const PointIndex NONE = -1;

    BoardInGm() = default;
    ~BoardInGm() = default;
    DISALLOW_COPY_AND_ASSIGN(BoardInGm);
    void Init();
    void Copy(const BoardInGm &b);

    PlayerColor LastPlayer() const {return last_player_;}
    PointIndex KoIndex() const {return ko_indx_;}
    const BitSet &PlayableIndexes(PlayerColor color) const {
        return playable_indxs_[color];
    }
    PointIndex BlackRegion() const {
        return b_pcs_c_ + eye_sets_[BLACK_PLAYER].RealCount();
    }
    HashKey HashKey() const {return hash_key_;}

    void PlayMove(const Move &move);
    void Pass(PlayerColor color);

//#ifdef DTEST
    void PRINT_EYES() const;
    void PRINT_PLAYABLE() const;
    void PRINT_BOARD() const {Board<BOARD_LEN>::PRINT();}
    static void TEST();
//#endif

private:
    typedef std::vector<PointIndex> PointIndxVector;

    ChainSet<BOARD_LEN> chain_sets_[2];
    BitSet playable_indxs_[2];
    EyeSet<BOARD_LEN> eye_sets_[2];
    PointIndex ko_indx_ = -1;
    PlayerColor last_player_ = WHITE_PLAYER;
    PointIndex b_pcs_c_ = 0;
    BrdInGmDlgt<BOARD_LEN> *delegate_;
    uint32_t hash_key_;

    bool IsSelfPieceOrEye(const Move &move) const;

    bool IsEmptySingly(PointIndex indx) const;
    bool IsSuiside(const Move &move) const;
    bool IsPlayable(const Move &move) const;

    bool PlayBasicMove(const Move &move, PointIndxVector *v = nullptr);
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
bool IsEnd(const BoardInGm<BOARD_LEN> &b)
{
    return b.PlayableIndexes(BLACK_PLAYER).count() == 0 &&
           b.PlayableIndexes(WHITE_PLAYER).count() == 0;
}


template <BoardLen BOARD_LEN>
PlayerColor NextPlayer(const BoardInGm<BOARD_LEN> &b)
{
    return OppstColor(b.LastPlayer());
}


#include "board_in_gm-TLT.h"

//#ifdef DTEST
#include "board_in_gm_TEST.h"
//#endif

#endif
