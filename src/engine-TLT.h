#include "engine.h"

#include <cmath>

#include <bitset>
#include <vector>

#include "bitset_util.h"


template <BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::Init()
{
    brdingm_.Init();
}


template <BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::Play(PointIndex nexti)
{
    if (!this->GetPosClcltr().IsInBoard(nexti)) return;
    brdingm_.PlayMove(Move(next_player_, nexti));
    next_player_ = OppstColor(next_player_);
}


template <BoardLen BOARD_LEN>
inline void Engine<BOARD_LEN>::Pass()
{
    brdingm_.Pass(next_player_);
    next_player_ = OppstColor(next_player_);
}


template <BoardLen BOARD_LEN>
PointIndex Engine<BOARD_LEN>::NextMove() const
{
    BoardInGm<BOARD_LEN> root;
    root.Copy(brdingm_);
    int mc_count = 0;

    do {
        BoardInGm<BOARD_LEN> cur_node;
        cur_node.Copy(root);
        std::vector<HashKey> path;
//        FOO_PRINT_LINE("do loop");

        while (true) {
//            FOO_PRINT_LINE("\n\nbegin while loop");
            PointIndex chld_i(0);
            if (cur_node.IsEnd()) {
                return this->BestChild(root);
            } else if (this->HasNewChild(cur_node, &chld_i)) {
//                FOO_PRINT_LINE("child i = %d", chld_i);
                PlayerColor color = OppstColor(cur_node.LastPlayer());
                BoardInGm<BOARD_LEN> child;
                child.Copy(cur_node);
                child.PlayMove(Move(color, chld_i));
                PointIndex black = this->Simulator().Simulate(child);
//                FOO_PRINT_LINE("black = %d", black);
                ++mc_count;
                PointIndex result = child.LastPlayer() == BLACK_PLAYER ?
                                black : BLSq<BOARD_LEN>() - black;
//                FOO_PRINT_LINE("result = %d", result);
                float profit = (float)result / BLSq<BOARD_LEN>();
                Engine<BOARD_LEN>::TableItem item(1, profit);
                table_[child.HashKey()] = item;

                if (path.size() > 0) {
                    for (auto it=path.end()-1; it>=path.begin(); --it) {
//                        FOO_PRINT_LINE("in for loop");
                        profit = 1 - profit;
                        auto pathitem = table_[*it];
                        auto t = pathitem.tried_times_;
                        auto p = pathitem.avg_prft_;
                        ++pathitem.tried_times_;
                        pathitem.avg_prft_ = (p*t + profit) / (t + 1);
                        table_[*it] = pathitem;
                    }
                }

                break;
            } else {
                PointIndex nexti = this->MaxUCBChild(cur_node);
//                FOO_PRINT_LINE("next i = %d", nexti);
//                cur_node.PRINT_BOARD();
                PlayerColor cur_player = OppstColor(cur_node.LastPlayer());
                cur_node.PlayMove(Move(cur_player, nexti));
                path.push_back(cur_node.HashKey());
            }
        }
    } while (mc_count < Engine<BOARD_LEN>::SIMULATE_COUNT);

    return this->BestChild(root);
}


template <BoardLen BOARD_LEN>
HashKey Engine<BOARD_LEN>::ChildKey(const BoardInGm<BOARD_LEN> &parent,
                                    PointIndex indx) const
{
    HashKey prnt_key = parent.HashKey();
    HashKey chldrn_key = table_[prnt_key].children_key_[indx];
    if (chldrn_key == NONE) {
        BoardInGm<BOARD_LEN> b;
        b.Copy(parent);
        PlayerColor color = OppstColor(b.LastPlayer());
        b.PlayMove(Move(color, indx));
        HashKey key = b.HashKey();
        table_[prnt_key].children_key_[indx] = key;
        return key;
    } else {
        return chldrn_key;
    }
}


template <BoardLen BOARD_LEN>
bool Engine<BOARD_LEN>::HasNewChild(const BoardInGm<BOARD_LEN> &node,
                                    PointIndex *p_indx) const
{
    PlayerColor color = OppstColor(node.LastPlayer());
    auto playable = node.NokoPlayableIndexes(color);
    auto children = Get1s<BLSq<BOARD_LEN>()>(playable);

    for (PointIndex nexti : children) {
        HashKey ck = this->ChildKey(node, nexti);
        if (table_.find(ck) == table_.end()) {
            *p_indx = nexti;
            return true;
        }
    }

    return false;
}


template <BoardLen BOARD_LEN>
PointIndex
Engine<BOARD_LEN>::MaxUCBChild(const BoardInGm<BOARD_LEN> &node) const
{
    PlayerColor nextc = OppstColor(node.LastPlayer());
    auto playable = node.NokoPlayableIndexes(nextc);
    auto playable_v = Get1s<BLSq<BOARD_LEN>()>(playable);
    int32_t sum = 0;

    for (PointIndex indx : playable_v) {
        sum += table_[this->ChildKey(node, indx)].tried_times_;
    }

    float max_ucb(0);
    PointIndex best(0);

    for (PointIndex indx : playable_v) {
        auto item = table_[this->ChildKey(node, indx)];

        float ucb = item.avg_prft_ + sqrt(2 * log(sum) / item.tried_times_);
        if (ucb > max_ucb) {
            max_ucb = ucb;
            best = indx;
        }
    }

    return best;
}


template <BoardLen BOARD_LEN>
PointIndex Engine<BOARD_LEN>::BestChild(const BoardInGm<BOARD_LEN> &node) const
{
    PlayerColor nextc = OppstColor(node.LastPlayer());
    auto playable = node.NokoPlayableIndexes(nextc);
    auto playable_v = Get1s<BLSq<BOARD_LEN>()>(playable);
    float max_profit = 0;
    PointIndex result(-1);

    for (PointIndex indx : playable_v) {
        BoardInGm<BOARD_LEN> b;
        b.Copy(node);
        b.PlayMove(Move(nextc, indx));
        auto item = table_[b.HashKey()];
        if (item.avg_prft_ > max_profit) {
            max_profit = item.avg_prft_;
            result = indx;
        }
    }

    return result;
}
