#ifndef MC_SIMULATOR_H
#define MC_SIMULATOR_H

#include "def.h"
#include "position.h"



template <BoardLen BOARD_LEN>
class MCSimulator
{
public:
    MCSimulator &Ins();

private:
    explicit MCSimulator();
    DISALLOW_COPY_AND_ASSIGN(MCSimulator);
};



#endif
