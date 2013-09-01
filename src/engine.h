#ifndef ENGINE
#define ENGINE

#include <stdint.h>

#include <unordered_map>
#include <algorithm>

#include "def.h"
#include "board_in_gm.h"
#include "mc_simulator.h"



template <BoardLen BOARD_LEN>
class Engine
{
public:
    Engine() : next_player_(BLACK_PLAYER) {}
    ~Engine() = default;
    DISALLOW_COPY_AND_ASSIGN(Engine);
    void Init();

    void Play(PointIndex nexti);
    void Pass();
    PointIndex NextMove() const;
//#ifdef DTEST
    static void TEST();
    void PRINT_PROFITS(const BoardInGm<BOARD_LEN> &b) const;
//#endif

private:
    struct TableItem {
        int32_t visited_times_;
        float avg_prft_;
        HashKeyType children_key_[BoardLenSquare<BOARD_LEN>()];

        TableItem(int32_t t, float a) : visited_times_(t), avg_prft_(a) {Init();}
        TableItem() {Init();}

        void Init() {
            std::fill(children_key_, children_key_ + BoardLenSquare<BOARD_LEN>(), NONE);
        }
    };
    typedef std::unordered_map<HashKeyType, TableItem> TrnspsitionTbl;

    static const int32_t SIMULATE_COUNT = 30000;
    static const uint32_t NONE = 0;

    BoardInGm<BOARD_LEN> brdingm_;
    PlayerColor next_player_;
    mutable TrnspsitionTbl table_;

    HashKeyType ChildKey(const BoardInGm<BOARD_LEN> &parent, PointIndex indx) const;
    bool HasNewChild(const BoardInGm<BOARD_LEN> &node, PointIndex *nexti) const;
    PointIndex MaxUCBChild(const BoardInGm<BOARD_LEN> &node) const;
    PointIndex BestChild(const BoardInGm<BOARD_LEN> &node) const;

    inline MCSimulator<BOARD_LEN> &Simulator() const {
        return MCSimulator<BOARD_LEN>::Ins();
    }
    inline PosCalculator<BOARD_LEN> &GetPosClcltr() const {
        return PosCalculator<BOARD_LEN>::Ins();
    }
};



#include "engine-TLT.h"

//#ifndef ENGINE_TEST_H
#include "engine_TEST.h"
//#endif

#endif
