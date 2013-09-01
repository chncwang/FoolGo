#include "engine.h"

#include <cmath>

#include <bitset>
#include <vector>

#include "bitset_util.h"
#include "vector_util.h"


template <BoardLen BOARD_LEN>
inline void Engine<BOARD_LEN>::Init()
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
void Engine<BOARD_LEN>::Pass()
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
        std::vector<HashKeyType> path;
//        PRINT_LINE("do loop");

        while (true) {
//            PRINT_LINE("\n\nbegin while loop");
            PointIndex chld_i(0);
            if (IsEnd(cur_node)) {
                return this->BestChild(root);
            } else if (this->HasNewChild(cur_node, &chld_i)) {
//                PRINT_LINE("child i = %d", chld_i);
                PlayerColor color = NextPlayer(cur_node);
                BoardInGm<BOARD_LEN> child;
                child.Copy(cur_node);
                child.PlayMove(Move(color, chld_i));
                PointIndex black = this->Simulator().Simulate(child);
//                PRINT_LINE("black = %d", black);
                ++mc_count;
                PointIndex result = child.LastPlayer() == BLACK_PLAYER ?
                                black : BoardLenSquare<BOARD_LEN>() - black;
//                PRINT_LINE("result = %d", result);
                float profit = result > (BoardLenSquare<BOARD_LEN>() / 2.0) ? 1 : 0;
                Engine<BOARD_LEN>::TableItem item(1, profit);
                table_[child.HashKey()] = item;

                if (path.size() > 0) {
                    for (auto it=path.end()-1; it>=path.begin(); --it) {
//                        PRINT_LINE("in for loop");
                        profit = 1 - profit;
                        auto pathitem = table_[*it];
                        auto t = pathitem.visited_times_;
                        auto p = pathitem.avg_prft_;
                        ++pathitem.visited_times_;
                        pathitem.avg_prft_ = (p*t + profit) / (t + 1);
                        table_[*it] = pathitem;
                    }
                }
                break;
            } else {
                PointIndex nexti = this->MaxUCBChild(cur_node);
                if (nexti == -1) return -1;
                PlayerColor cur_player = NextPlayer(cur_node);
                cur_node.PlayMove(Move(cur_player, nexti));
                path.push_back(cur_node.HashKey());
            }
        }
    } while (mc_count < Engine<BOARD_LEN>::SIMULATE_COUNT);

    return this->BestChild(root);
}


template <BoardLen BOARD_LEN>
HashKeyType Engine<BOARD_LEN>::ChildKey(const BoardInGm<BOARD_LEN> &parent,
                                    PointIndex indx) const
{
    HashKeyType prnt_key = parent.HashKey();
    HashKeyType chldrn_key = table_[prnt_key].children_key_[indx];
    if (chldrn_key == Engine<BOARD_LEN>::NONE) {
        BoardInGm<BOARD_LEN> b;
        b.Copy(parent);
        PlayerColor color = NextPlayer(b);
        b.PlayMove(Move(color, indx));
        HashKeyType key = b.HashKey();
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
    PlayerColor color = NextPlayer(node);
    auto playable = NokoPlayableIndexes(node, color);
    auto children = Get1s<BoardLenSquare<BOARD_LEN>()>(playable);
    RandomizeVector(&children);

    for (PointIndex nexti : children) {
        HashKeyType ck = this->ChildKey(node, nexti);
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
    PlayerColor nextc = NextPlayer(node);
    auto playable = NokoPlayableIndexes(node, nextc);
    auto playable_v = Get1s<BoardLenSquare<BOARD_LEN>()>(playable);
    int32_t sum = 0;

    for (PointIndex indx : playable_v) {
        sum += table_[this->ChildKey(node, indx)].visited_times_;
    }

    static const float INITIAL = -10000;
    float max_ucb(INITIAL);
    PointIndex best(0);

    for (PointIndex indx : playable_v) {
        auto item = table_[this->ChildKey(node, indx)];

        float ucb = item.avg_prft_ + sqrt(2 * log(sum) / item.visited_times_);
        if (ucb > max_ucb) {
            max_ucb = ucb;
            best = indx;
        }
    }

    return max_ucb > INITIAL ? best : -1;
}


template <BoardLen BOARD_LEN>
PointIndex Engine<BOARD_LEN>::BestChild(const BoardInGm<BOARD_LEN> &node) const
{
    PlayerColor nextc = NextPlayer(node);
    auto playable = NokoPlayableIndexes(node, nextc);
    auto playable_v = Get1s<BoardLenSquare<BOARD_LEN>()>(playable);
    int most_visited = 0;
    PointIndex result(-1);

    for (PointIndex indx : playable_v) {
        auto item = table_[this->ChildKey(node, indx)];
        if (item.visited_times_ > most_visited) {
            most_visited = item.visited_times_;
            result = indx;
        }
    }

    return result;
}
