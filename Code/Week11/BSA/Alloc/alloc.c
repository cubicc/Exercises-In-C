#include "specific.h"

int get_row(int indx, int* offset) {
    if (indx < 0) return -1;
    if (indx == 0) {
        *offset = 0;
        return 0;
    }
    int minn = 1;
    for (int i = 1; i < BSA_ROWS; i++) {
        int maxx = (minn << 1) | 1;
        if (indx >= minn && indx < maxx) {
            *offset = indx - minn;
            return i;
        }
        minn = maxx;
    }
    return -1;
}


bsa* bsa_init(void) {
    bsa *new_bsa = (bsa*)malloc(sizeof(bsa));
    if (new_bsa == NULL) {
        printf("alloc bsa failed");
        return NULL;
    }
    memset(new_bsa->row, 0, sizeof(new_bsa->row));
    memset(new_bsa->count, 0, sizeof(new_bsa->count));
    new_bsa->is_alloc = false;
    return new_bsa;
}


bool bsa_set(bsa* b, int indx, int d) {
    //printf("indx: %d\n", indx);
    int offset = -1;
    int row_idx = get_row(indx, &offset);
    //printf("%d %d\n", row_idx, offset);
    if (row_idx == -1 || offset == -1) return false;
    
    
    if (b->row[row_idx] == NULL) {
        //alloc
        int *p = malloc((1 << row_idx) * sizeof(int));
       // printf("alloc mem %ld\n", (1 << row_idx) * sizeof(int));
        memset(p, 0xFF, (1 << row_idx) * sizeof(int));
        b->row[row_idx] = p;
    }
    if (b->row[row_idx][offset] == INF) {
        b->count[row_idx]++;
    }
    b->row[row_idx][offset] = d;
    b->is_alloc = true;
    return true;
}

int* bsa_get(bsa* b, int indx) {
    int offset = -1;
    int row_idx = get_row(indx, &offset);
    if (row_idx == -1 || offset == -1) return NULL;
    if (b->row[row_idx] == NULL || b->row[row_idx][offset] == INF) {
        return NULL;
    }
    return &b->row[row_idx][offset];
}

bool bsa_delete(bsa* b, int indx) {
    int offset = -1;
    int row_idx = get_row(indx, &offset);
    if (row_idx == -1 || offset == -1) return false;
    if (b->row[row_idx] == NULL || b->row[row_idx][offset] == INF) {
        return false;
    }
    b->row[row_idx][offset] = INF;
    if (--b->count[row_idx] <= 0) {
        free(b->row[row_idx]);
        b->row[row_idx] = NULL;
    }
    return true;
}


int bsa_maxindex(bsa* b) {
    if (b == NULL) return -1;
    // for (int i = 0; i < BSA_ROWS; i++) {
    //     printf("%d ", b->count[i]);
    // }
    // printf("\n");

    for (int i = BSA_ROWS - 1; i >= 0; i--) {
        if (b->count[i] == 0) continue;
        for (int j = (1 << i) - 1; j >= 0; j--) {
            if (b->row[i][j] != INF){
                //printf("max index: %d\n", (1 << i) + j - 1);
                return (1 << i) + j - 1;
            }
        }
    }
    //printf("max index:-1\n");
    return -1;
}

bool bsa_tostring(bsa* b, char* str) {
    if (b == NULL) return false;
    str[0] = '\0';
    int max_idx = bsa_maxindex(b);
    if (max_idx == -1) {
        if (b->is_alloc == true) {
            return true;
        }
        return false;
    }

    int offset = -1;
    int row_idx = get_row(max_idx, &offset);
   // printf("row_idx: %d\n", row_idx);
    for (int i = 0; i <= row_idx; i++) {
       // printf("str: %s\n", str);
        char tmp[100] = "{";
        if (b->count[i] != 0) {
            for (int j = 0; j < (1 << i); j++) {
                if (b->row[i][j] != INF) {
                    char s[50] = "";
                    sprintf(s, "[%d]=%d ", (1 << i) + j - 1, b->row[i][j]);
                    strcat(tmp, s);
                }
            }
        }
        int len = strlen(tmp);
        if (tmp[len - 1] == ' ') {
            len--;
        }
        tmp[len] = '}';
        tmp[len + 1] = '\0';
       // printf("tmp:%s\n", tmp);
        strcat(str, tmp);
    }
   // printf("str: %s\n", str);
    return true;
}

bool bsa_free(bsa* b) {
    if (b == NULL) {
        return false;
    }
    for (int i = 0; i < BSA_ROWS; i++) {
        if (b->row[i] != NULL) {
            free(b->row[i]);
            b->row[i] = NULL;
        }
    }
    free(b);
    return true;
}

void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc) {
    for (int i = 0; i < BSA_ROWS; i++) {
        if (b->count[i] == 0) continue;
        for (int j = 0; j < (1 << i); j++) {
            if (b->row[i][j] != INF) {
                func(&b->row[i][j], acc);
            }
        }
    }
}

void test(){

}
