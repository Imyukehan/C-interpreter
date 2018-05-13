#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h> // open()函数原型所在头文件
#include <unistd.h> // read()函数原型所在头文件

int token;            // 当前字
char *src, *old_src;  // 指向源代码
int poolsize;         // 内存池大小
int line;             // 行号

int *text,            // 字段
    *old_text,        // 垃圾字段
    *stack;           // 栈指针
char *data;           // 数据段
/*
 * PC 程序计数器。它存放的是一个内存地址，该地址中存放着 下一条 要执行的计算机指令。
 * SP 指针寄存器。永远指向当前的栈顶。注意的是由于栈是位于高地址并向低地址增长的，所以入栈时 SP 的值减小。
 * BP 基址指针。也是用于指向栈的某些位置，在调用函数时会使用到它。
 * AX 通用寄存器。我们的虚拟机中，它用于存放一条指令执行后的结果。
 */
int *pc, *bp, *sp, ax, cycle; // XC虚拟机寄存器

enum instructions {
    LEA, IMM, JMP, CALL, JZ, JNZ, ENT,
    ADJ, LEV, LI, LC, SI, SC, PUSH,OR, 
    XOR, AND, EQ, NE, LT, GT, LE, GE, 
    SHL, SHR, ADD, SUB, MUL, DIV, MOD, 
    OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT
};

enum tokens {
    Num = 128, Fun, Sys, Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, 
    Sizeof, While, Assign, Cond, Lor, 
    Or, Xor, And, Eq, Ne, Lt, Gt, Le, 
    Ge, Shl, Shr, Add, Sub, Mul, Div, 
    Mod, Inc, Dec, Brak
};

/*
 * next函数用于词法分析，返回下一个字符的模式
 */
void next() {
    char *last_pos;
    int hash;

    while(token == *src) {
        ++src;
        
    }
    return;
}

/*
 * expression函数用于解析一句表达式
 */
void expression(int level) {
    // 暂时什么也不做
}

/*
 * program函数用于对整个源程序的分析
 */
void program() {
    next();
    
    while (token > 0) {
        printf("当前字符为: %c\n", token);
        next();
    }
}

/*
 * eval函数用于执行XC虚拟机
 */
int eval() {
    int op, *tmp;
    while(1) {
        op = *pc++;
        switch(op) {
            case IMM: ax = *pc++; break;
            case LC: ax = *(char *)ax; break;
            case LI: ax = *(int *)ax; break;
            case SC: ax = *(char *)*sp++ = ax; break;
            case SI: *(int *)*sp++ = ax;

            case PUSH: *--sp = ax; break;
            case JMP: pc = (int *)*pc; break;
            // 运算指令
            case OR: ax = *sp++ | ax; break;
            case XOR: ax = *sp++ ^ ax; break;
            case AND: ax = *sp++ & ax; break;
            case EQ: ax = *sp++ == ax; break;

            case ADD: ax = *sp++ + ax; break;

            case EXIT: printf("exit(%d)\n", *sp); return *sp; break;

            default: printf("未知指令: %d\n", op); return -1; break;

        }

    }
    return 0;
}

int main(int argc, const char *argv[]) {
    int i, fd;

    argc--;
    argv++;

    poolsize = 256 * 1024;
    line = 1;
/*
    if ((fd = open(*argv, O_RDONLY)) < 0) {
        printf("无法打开文件（%s）\n", *argv);
        return -1;
    }

    
    if (!(src = old_src = malloc(poolsize))) {
        printf("无法为源程序分配 %d 字节内存\n", poolsize);
        return -1;
    }

    // 读取源程序文件
    if ((i = read(fd, src, poolsize-1)) < 0) {
        printf("read() 返回 %d\n", i);
    }
    src[i] = 0;
    close(fd);*/

    // 分配内存池
    if (!(text = old_text = malloc(poolsize))) {
        printf("无法为字段分配 %d 字节内存\n", poolsize);
        return -1;
    }
    if (!(data = malloc(poolsize))) {
        printf("无法为数据段分配 %d 字节内存\n", poolsize);
        return -1;
    }
    if (!(stack = malloc(poolsize))) {
        printf("无法堆栈分配 %d 字节内存\n", poolsize);
        return -1;
    }
    memset(text, 0 , poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);

    bp = sp = (int *)((int)stack + poolsize);
    ax = 0;

    i = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc = text;
    
    //program();
    
    return eval();
}