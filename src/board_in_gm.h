#ifndef BOARD_IN_GM
#define BOARD_IN_GM

#include "board.h"
#include "chain_set.h"



template <BoardLen BOARD_LEN>
class BoardInGm
{
public:
    explicit BoardInGm();
    ~BoardInGm();

private:
    Board<BOARD_LEN> board_;
};



#endif
