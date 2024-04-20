#include "../bsa.h"
#define INF -1

struct bsa {
    int* row[BSA_ROWS];
    int count[BSA_ROWS];
    bool is_alloc;
};

