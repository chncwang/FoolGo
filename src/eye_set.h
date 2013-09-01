#ifndef EYE_H
#define EYE_H

#include <bitset>

#include "def.h"
#include "position.h"



template <BoardLen BOARD_LEN>
class EyeSet
{
public:
    typedef std::bitset<BoardLenSquare<BOARD_LEN>()> BitSet;

    EyeSet() = default;
    ~EyeSet() = default;
    DISALLOW_COPY_AND_ASSIGN(EyeSet);
    void Copy(const EyeSet &es);

    void SetEye(PointIndex indx, bool v) {eyes_[indx] = v;}
    void SetRealEye(PointIndex indx, bool v) {real_eyes_[indx] = v;}
    bool IsEye(PointIndex indx) const {return eyes_[indx];}
    bool IsRealEye(PointIndex indx) const {return real_eyes_[indx];}
    PointIndex RealCount() const {return real_eyes_.count();}

    const BitSet &GetRealEyes() const {return real_eyes_;}

private:
    BitSet eyes_;
    BitSet real_eyes_;
};



template <BoardLen BOARD_LEN>
bool IsFakeEye(const EyeSet<BOARD_LEN> &eyeset, PointIndex indx)
{
    return eyeset.IsEye(indx) && !eyeset.IsRealEye(indx);
}


#include "eye_set-TLT.h"

#endif
