#include <elf.h>
#include <common.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_DEPTH 64 // 栈的最大深度

/*创建字符串记录表*/
typedef struct{
    char s_name[32];
    paddr_t addr;
    unsigned char info;
    Elf32_Xword size;
}Stm_Table;

/*建立数组表单记录*/
Stm_Table *S_table = NULL;
int S_table_size = 0;
int Fun_depth = 0;

typedef struct fun_node{
    paddr_t pc;
    paddr_t depend;
    struct fun_node *next;
}FunNode;
FunNode *fun_node_head = NULL;

typedef struct function{
    char fun_name[32];
    paddr_t fun_addr;
}function;

function fun_list[64];
int fun_index;
char *fun_name_stack[MAX_DEPTH]; // 函数名栈
int fun_name_stack_top = 0; // 栈顶指针

void push_fun_name(char *fun_name) {
    if (fun_name_stack_top < MAX_DEPTH) {
        fun_name_stack[fun_name_stack_top++] = fun_name;
    } else {
        printf("Function call depth exceeds maximum limit!\n");
    }
}

// void push_fun_name(char *fun_name) {
//     if (fun_name_stack_top < MAX_DEPTH) {
//         fun_name_stack[fun_name_stack_top++] = fun_name;
//         printf("Current function call stack:\n");
//         ftrace_write("\n==> stack name :");
//         for(int i = 0; i < fun_name_stack_top; i++) {
//             ftrace_write("%s, ", fun_name_stack[i]);
//         }
//         ftrace_write("\n\n");
//     } else {
//         printf("Function call depth exceeds maximum limit!\n");
//     }
// }


// 从栈中弹出函数名
char* pop_fun_name() {
    if (fun_name_stack_top > 0) {
        return fun_name_stack[--fun_name_stack_top];
    } else {
        printf("Function call stack is empty!\n");
        return NULL;
    }
}

// 读取elf头判断是否为elf文件
static void Read_ELFhead(int fd, Elf32_Ehdr *eh){
    assert(lseek(fd, 0, SEEK_SET) == 0);
    assert(read(fd, (void *)eh, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
    if(memcmp((char*)eh->e_ident, "\177ELF", 4)){
        panic("no a ELF file");
    }
}

// 判断节头表内部信息是否正常
static void Check_Section(int fd, Elf32_Shdr sh_32, void *set){
    assert(set != NULL);
    assert(lseek(fd, (off_t)sh_32.sh_offset, SEEK_SET) == (off_t)sh_32.sh_offset);
    assert(read(fd, set , sh_32.sh_size) == sh_32.sh_size);
}

// 读取节表信息 size, offset
static void read_sheader(int fd, Elf32_Ehdr eh, Elf32_Shdr *sh_table){
    assert(lseek(fd, eh.e_shoff, SEEK_SET) == eh.e_shoff);
    for(int i = 0; i < eh.e_shnum; i ++){
        assert(read(fd, (void *)&sh_table[i], eh.e_shentsize));
    }
}

static void Read_Symble(int file, Elf32_Ehdr eh, Elf32_Shdr sh_table[], int sym_idx){
    Elf32_Sym sym_table[sh_table[sym_idx].sh_size];
    Check_Section(file, sh_table[sym_idx], sym_table);

    int str_idx = sh_table[sym_idx].sh_link;
    char str_table[sh_table[sym_idx].sh_size];
    Check_Section(file, sh_table[str_idx], str_table);
    int sym_count = (sh_table[sym_idx].sh_size / sizeof(Elf32_Sym));
    ftrace_write("=======Functions in the symbol table.======\n\n");
    for(int i = 0; i < sym_count; i++){
        unsigned char type = ELF32_ST_TYPE(sym_table[i].st_info);
        if(type == STT_FUNC) {
            ftrace_write("0x%08x: call [%s@0x%08x]\n", (unsigned int)sym_table[i].st_value, &str_table[sym_table[i].st_name], (unsigned int)sym_table[i].st_value);
            strncpy(fun_list[fun_index].fun_name, &str_table[sym_table[i].st_name], 31); // 使用 strncpy 来复制字符串
            fun_list[fun_index].fun_name[31] = '\0'; // 确保字符串以 null 结尾
            fun_list[fun_index++].fun_addr = (unsigned int)sym_table[i].st_value;
        }
    }

    for(int i = 0; i < fun_index; i++){
        ftrace_write("%s ", fun_list[i].fun_name);
    }

    ftrace_write("\n=======The invocation of the function======\n");
    S_table_size = sym_count;
    S_table = malloc(sizeof(Stm_Table)* sym_count);
    for(int i = 0; i < sym_count; i ++){
        S_table[i].addr = sym_table[i].st_value;
        S_table[i].info = sym_table[i].st_info;
        S_table[i].size = sym_table[i].st_size;
        memset(S_table[i].s_name, 0 , 32);
    }
}

static void read_sbols(int fd, Elf32_Ehdr eh, Elf32_Shdr sh_table[]){
    for(int i = 0 ; i < eh.e_shnum; i ++){
        switch(sh_table[i].sh_type){
            case SHT_SYMTAB: case SHT_DYNSYM:
            Read_Symble(fd, eh, sh_table, i);break;
        }
    }
}

static void init_funtail(){
    fun_node_head = (FunNode *)malloc(sizeof(FunNode));
    fun_node_head->pc = 0;
    fun_node_head->next = NULL;
}

void init_elf(const char *elf_file){
    if(elf_file == NULL){
        return;
    }
    
    int fd = open(elf_file, O_RDONLY|O_SYNC);
    Elf32_Ehdr eh;
    Read_ELFhead(fd, &eh);
    
    Elf32_Shdr sh_table[eh.e_shentsize * eh.e_shnum];
    read_sheader(fd, eh, sh_table);//读取节头

    read_sbols(fd, eh, sh_table);
    init_funtail();//初始化记录函数的空节头

    close(fd);
}

static int find_callfun(paddr_t target){
    int i;
    for(i = 0; i < fun_index; i ++){
        if(fun_list[i].fun_addr == target){
            // ftrace_write("=============dnpc:0x%08x\n", target);
            break;
        }
    }
    return i < S_table_size? i : -1;
}

void f_trace_call(paddr_t pc, paddr_t target){
    if(S_table == NULL) return ;
    ++Fun_depth;

    int i = find_callfun(target);
    char *fun_call_name;
    if (i >= 0 && i < fun_index) {
        fun_call_name = fun_list[i].fun_name; // 从 fun_list 中获取函数名
    } else {
        fun_call_name = "???"; // 如果找不到，就设为 "???"
    }
    ftrace_write("0x%08x: %*scall [%s@"FMT_PADDR "]\n",pc, Fun_depth, " ", fun_call_name, target);
    push_fun_name(fun_call_name); // 将函数名压入栈
    // 每次压入后输出一次全栈内容
    
}

void f_trace_fun_ret(paddr_t pc){
    if(S_table == NULL || fun_name_stack_top == 0) return;

    char *fun_ret_name = pop_fun_name();
    ftrace_write("0x%08x: %*sret  [%s]\n", pc, Fun_depth, " ", fun_ret_name);

    --Fun_depth;
    if(Fun_depth < 0) Fun_depth = 0;
}
