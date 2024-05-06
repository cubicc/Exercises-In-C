#include "nuclei.h"
#include <stdio.h>
int main(int argc, char** argv) {
    if (argc > 1) {
        printf("File name: %s\n", argv[1]);
    }else {
        on_error("No file name given");
    }

    FILE *fp = nfopen(argv[1], "r");
    if (fp == NULL) {
        on_error("Could not open file");
    }

    Program *prog = ncalloc(1, sizeof(Program));
    prog->cw = 0;
    char tmp[100];
    while (fgets(tmp, 100, fp) != NULL) {
        if (tmp[0] == '#') continue;
        int len = strlen(tmp) - 1;
        if (tmp[len] == '\n') {
            tmp[len] = '\0';
        }
        strcat(prog->wds, tmp);
        strcat(prog->wds, " ");
    }
    prog->wds[strlen(prog->wds) - 1] = '\0';
    printf("%s\n", prog->wds);
    Prog(prog);
    printf("Parsed OK\n");
}


void Prog(Program* p) {
    SKIP_SPACES
    LOG("PROG");
    // printf("%s\n", p->wds);
    // printf("%d\n", p->cw);
    if (!strsame(p->wds + p->cw, "(", 1)) {
        on_error("PROG:Expected '('");
        return;
    }
    p->cw++;
    Instrcts(p);
}

void Instrcts(Program* p) {
    SKIP_SPACES
    LOG("INSTRCTS");
    if (strsame(p->wds + p->cw, ")", 1)) {
        LOG("INSTRCTS:END");
        p->cw++;
        return;
    }
    Instrct(p);
    SKIP_SPACES
    Instrcts(p);
}

void Instrct(Program* p) {
    SKIP_SPACES
    LOG("INSTRCT");
    if (!strsame(p->wds + p->cw, "(", 1)) {
        on_error("INSTRCT: Expected '('");
    }
    p->cw++;
    Func(p);
    LOG("INSTRCT:FUNCEND");
    SKIP_SPACES
    if (!strsame(p->wds + p->cw , ")", 1)) {
        LOG("INSTRCT:ERROR");
        on_error("Instrct: Expected ')'");
    }
    p->cw++;
}

void Func(Program* p) {
    SKIP_SPACES
    LOG("FUNC");
    int cw = p->cw;
    if(RetFunc(p)) {
        return;
    }
    p->cw = cw;
    if(IOFunc(p)){
        return;
    }
    p->cw = cw;
    if(If(p)){
        return;
    }
    p->cw = cw;
    if(Loop(p)){
        return;
    }
    p->cw = cw;
}

bool RetFunc(Program* p) {
    SKIP_SPACES
    LOG("RETFUNC");
    int cw = p->cw;
    if (ListFunc(p)) {
        return true;
    }
    p->cw = cw;
    if (IntFunc(p)) {
        return true;
    }
    p->cw = cw;
    if (BoolFunc(p)) {
        return true;
    }
    p->cw = cw;
    return false;
}

bool ListFunc(Program* p) {
    SKIP_SPACES
    LOG("LISTFUNC");
    if (strsame(p->wds + p->cw, "CAR", 3)) {
        p->cw += 3;
        return List(p);
    }else if (strsame(p->wds + p->cw, "CDR", 3)) {
        p->cw += 3;
        return List(p);
    }else if (strsame(p->wds + p->cw, "CONS", 4)) {
        p->cw += 4;
        LOG("LISTFUNC:CONS");
        if (List(p)) {
            LOG("LISTFUNC:CONS:SECOND");
            return List(p);
        }
    }
    return false;
}

bool IntFunc(Program* p) {
    SKIP_SPACES
    LOG("INTFUNC");
    if (strsame(p->wds + p->cw, "PLUS", 4)) {
        p->cw += 4;
        if (List(p)) {
            return List(p);
        }
    } else if (strsame(p->wds + p->cw, "LENGTH", 6)) {
        p->cw += 6;
        return List(p);
    }
    return false;
}

bool BoolFunc(Program* p) {
    SKIP_SPACES
    LOG("BOOLFUNC");
    if (strsame(p->wds + p->cw, "EQUAL", 5)) {
        p->cw += 5;
        if (List(p)) {
            return List(p);
        }
    } else if (strsame(p->wds + p->cw, "LESS", 4)) {
        p->cw += 4;
        if (List(p)) {
            return List(p);
        }
    } else if (strsame(p->wds + p->cw, "GREATER", 7)) {
        p->cw += 7;
        if (List(p)) {
            return List(p);
        }
    }
    return false;
}

bool IOFunc(Program* p) {
    SKIP_SPACES

    LOG("IOFUNC");
    int cw = p->cw;
    if (Set(p)) {
        return true;
    }
    p->cw = cw;
    if (Print(p)){
        return true;
    }
    p->cw = cw;
    return false;
}

bool Set(Program* p) {
    SKIP_SPACES

    LOG("SET");
    if (strsame(p->wds + p->cw, "SET", 3)) {
        p->cw += 3;
        if (Var(p)) {
            p->cw++;
            return List(p);
        }
    }
    return false;
}

bool Print(Program* p) {
    SKIP_SPACES

    LOG("PRINT");
    if (strsame(p->wds + p->cw, "PRINT", 5)) {
        p->cw += 5;
        if (List(p)){
            return true;
        }
        if (String(p)){
            return true;
        }
    }
    return false;
}

bool If(Program* p) {
    SKIP_SPACES
    LOG("IF");
    if (!strsame(p->wds + p->cw, "IF", 2)) {
        LOG("IF:NOTIF");
        return false;
    }
    p->cw += 2;
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, "(", 1)) {
        on_error("IF1:Expected '('");
        return false;
    }
    p->cw++;
    if (!BoolFunc(p)) {
        return false;
    }
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, ")", 1)) {
        on_error("Expected ')'");
        return false;
    }
    p->cw++;
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, "(", 1)){
        on_error("IF2:Expected '('");
        return false;
    }
    p->cw++;
    LOG( "IF:BEFORE");
    Instrcts(p);
    SKIP_SPACES
    LOG("IF:AFTER");
    if (!strsame(p->wds + p->cw, "(", 1)){
        on_error("IF3:Expected '('");
        return false;
    }
    p->cw++;
    Instrcts(p);
    return true;
}

bool Loop(Program* p) {
    SKIP_SPACES
    LOG("LOOP");
    if (!strsame(p->wds + p->cw, "WHILE", 5)) {
        LOG("LOOP:NOTWHILE");
        return false;
    }
    p->cw += 5;
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, "(", 1)){
        on_error("LOOP:Expected '('");
        return false;
    }
    p->cw++;
    SKIP_SPACES
    if (!BoolFunc(p)){
        return false;
    }
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, ")", 1)){
        on_error("Expected ')'");
        return false;
    }
    p->cw++;
    SKIP_SPACES
    if (!strsame(p->wds + p->cw, "(", 1)){
        on_error("LOOP:Expected '('");
        return false;
    }
    p->cw++;
    LOG("LOOP:END");
    Instrcts(p);
    return true;
}

bool List(Program* p) {
    SKIP_SPACES

    LOG("LIST");
    if (strsame(p->wds + p->cw, "(", 1)) {
        p->cw++;
        if (RetFunc(p)){
            SKIP_SPACES
            if (strsame(p->wds + p->cw, ")", 1)) {
                p->cw++;
                return true;
            }
        }

    } else if (strsame(p->wds + p->cw, "NIL", 3)) {
        p->cw += 3;
        return true;
    }
     else if (Var(p)){
        return true;
    } else if (Literal(p)) {
        return true;
    }
    LOG("LIST:NOTLIST");
    return false;
}

bool Var(Program* p) {
    SKIP_SPACES

    LOG("VAR");
    if (p->wds[p->cw] >= 'A' && p->wds[p->cw] <= 'Z') {
        p->cw++;
        return true;
    }
    //printf("%s\n", p->wds+p->cw);
    //on_error("Expected variable");
    return false;
}

bool String(Program* p) {
    SKIP_SPACES

    LOG("STRING");
    if (p->wds[p->cw] != '"') {
        on_error("\": Expected String");
    }
    int i = p->cw + 1;
    while(p->wds[i] != '"' && p->wds[i] != '\0') {
        i++;
    }
    if (p->wds[i] == '\0') {
        on_error("'\0': Expected String");
    }
    p->cw = i + 1;
    return true;
}

bool Literal(Program* p) {
    SKIP_SPACES

    LOG("LITERAL");
    if (p->wds[p->cw] != '\'') {
        LOG("LITERAL:NOTLITERAL");
        return false;
    }
    int i = p->cw + 1;
    while(p->wds[i] != '\'' && p->wds[i] != '\0') {
        i++;
    }
    if (p->wds[i] == '\0') {
        LOG("LITERAL:NOTLITERAL");
        return false;
    }
    p->cw = i + 1;
    return true;
}

