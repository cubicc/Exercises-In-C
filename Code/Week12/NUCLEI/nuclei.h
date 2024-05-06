#include "general.h"
#define MAXNUMTOKENS 10000
#define strsame(A,B,N) (strncmp(A, B, N) == 0)
#define LOG(X) printf("%s: %s cw:%d\n", X, p->wds + p->cw, p->cw);
#define SKIP_SPACES \
    while(p->wds[p->cw] == ' ') { \
        p->cw++; \
    } 

struct prog {
    char wds[MAXNUMTOKENS];
    int cw; // current word
};

typedef struct prog Program;

void Prog(Program* p);
void Instrcts(Program* p);
void Instrct(Program* p);
void Func(Program* p);
bool RetFunc(Program* p);
bool ListFunc(Program* p);
bool IntFunc(Program* p);
bool BoolFunc(Program* p);
bool IOFunc(Program* p);
bool Set(Program* p);
bool Print(Program* p);
bool If(Program* p);
bool Loop(Program* p);
bool List(Program* p);
bool Var(Program* p);
bool String(Program* p);
bool Literal(Program* p);
