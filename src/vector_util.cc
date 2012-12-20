#include "vector_util.h"

#include "rand.h"


void RandomizeVector(std::vector<PointIndex> *vctr)
{
    int len = vctr->size();
    ASSERT(len > 0);

    for (int i=0; i<len; ++i) {
        int max = len - i - 1;
        int rand = Rand(max) + i;
        std::swap(vctr->at(i), vctr->at(rand));
    }
}


#ifdef DTEST
void VectorUtil_TEST()
{
    std::vector<PointIndex> v;
    for (int i=0; i<20; ++i) v.push_back(i);
    printf("here.\n");
    RandomizeVector(&v);
    printf("b\n");
    for (int i=0; i<20; ++i) printf("%d, ", v[i]);
    printf("\n");
}
#endif
