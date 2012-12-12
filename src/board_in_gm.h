#ifndef BOARD_IN_GM
#define BOARD_IN_GM

#include <bitset>
#include <vector>

#include "board.h"
#include "chain_set.h"

typedef char PlayerColor;
const PlayerColor BLACK_PLAYER = 0;
const PlayerColor WHITE_PLAYER = 1;


inline PlayerColor OppstColor(PlayerColor color)
{
    static const PlayerColor T[2] = {1, 0};
    return T[color];
}


struct Move {
    PlayerColor color_;
    PointIndex indx_;

    Move() : color_(0), indx_(0) {}
    Move(PlayerColor color, PointIndex indx) : color_(color), indx_(indx) {}
};



template <BoardLen BOARD_LEN>
class BoardInGm
{
public:
    typedef std::bitset<BLSq<BOARD_LEN>()> Bitset;

    static const PointIndex NONE = -1;

    BoardInGm() = default;
    ~BoardInGm() = default;
    DISALLOW_COPY_AND_ASSIGN(BoardInGm);
    void Init();
    void Copy(const BoardInGm &b);

    PlayerColor LastPlayer() const {return last_player_;}
    PointIndex KoIndex() const {return ko_indx_;}
    inline const Bitset &PlayableIndexes(PlayerColor color) const {
        return playable_indxs_[color];
    }

    void PlayMove(const Move &move);
    void Pass(PlayerColor color);
    inline PointIndex BlackRegion() const {
        return b_pcs_c_ + real_eyes_[BLACK_PLAYER].count();
    }

#ifdef FOO_TEST
    void PRINT_EYES() const;
    void PRINT_PLAYABLE() const;
    void PRINT_BOARD() const {board_.PRINT();}
    static void TEST();
#endif

private:
    typedef std::vector<PointIndex> PointIndxVector;

    Board<BOARD_LEN> board_;
    ChainSet<BOARD_LEN> chain_sets_[2];
    Bitset playable_indxs_[2];
    Bitset eyes_[2];
    Bitset real_eyes_[2];
    PointIndex ko_indx_ = -1;
    PlayerColor last_player_ = WHITE_PLAYER;
    PointIndex b_pcs_c_ = 0;

    void BasicCopy(const BoardInGm &b);

    PosCalculator<BOARD_LEN> &GetPosClcltr() const;

    bool IsEye(const Move &move) const;
    bool IsEyeByCal(const Move &move) const;
    bool IsRealEye(const Move &move) const;
    bool IsFakeEye(const Move &move) const;
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



#include "board_in_gm-TLT.h"

#ifdef FOO_TEST
#include "board_in_gm_TEST.h"
#endif

#endif
