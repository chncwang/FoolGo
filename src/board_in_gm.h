#ifndef BOARD_IN_GM
#define BOARD_IN_GM

#include <vector>
#include <bitset>

#include "board.h"
#include "chain_set.h"

typedef char PlayerColor;
const PlayerColor BLACK_PLAYER = 0;
const PlayerColor WHITE_PLAYER = 1;

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
    typedef std::bitset<BoardLenSquare<BOARD_LEN>()> Bitset;

    BoardInGm();
    void Init();
    void Copy(const BoardInGm &b);

    bool PlayMove(const Move &move);

#ifdef FOO_TEST
    void PRINT_EYES() const;
    static void TEST();
#endif

private:
    Board<BOARD_LEN> board_;
    ChainSet<BOARD_LEN> chain_sets_[2];
    Bitset playable_indxs_[2];
    Bitset eyes_[2];
    Bitset real_eyes_[2];

    DISALLOW_COPY_AND_ASSIGN(BoardInGm);

    PosCalculator<BOARD_LEN> &GetPosClcltr() const;
    bool IsEye(const Move &move) const;
    bool IsRealEye(const Move &move) const;
    bool IsFakeEye(const Move &move) const;
    bool IsEmptySingly(PointIndex indx) const;
    bool IsMoveSuiside(const Move &move) const;

    void UpdateEyes(const Move &move);
    void UpdateRealEyes(const Move &move);
    void RemoveChain(const Move &move);
    void UpdtAdjPlblIndxsOfChn(PointIndex indx);
    void UpdtPlblIndxsArnd(PointIndex indx);
};



#include "board_in_gm-TLT.h"

#ifdef FOO_TEST
#include "board_in_gm_TEST.h"
#endif

#endif
