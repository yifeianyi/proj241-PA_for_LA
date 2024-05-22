#include <proc.h>
// #include <usr>
#include </usr/include/elf.h>

/* new insert: EM_Loogarch32r */
#define EM_Loongarch32r 258

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

/*============== About ISA check ================*/
#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_X86__)
#define EXPECT_TYPE panic("Unknown type")
#elif defined(__ISA_RISCV32__) || defined(__ISA_RISCV64__)
#define EXPECT_TYPE panic("Unknown type")
#elif defined(__ISA_MIPS32__)
# define EXPECT_TYPE panic("Unknown type")
#elif defined(__ISA_LOONGARCH32R__)
# define EXPECT_TYPE EM_Loongarch32r
#else
# error Unsupported ISA
#endif
/*===============================================*/

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  char buf[64];

  /*===== check magic number ====*/
  ramdisk_read(buf, 0, 8);
  uint64_t magic_number = 0;
  for (int i = 0; i < 8; ++i) magic_number = (magic_number << 8) + buf[i];
  assert(magic_number == 0x7f454c4601010100);
  
  /*==== check machine architecture ====*/
  ramdisk_read(buf, 18, 2);
  uint16_t type = 0;
  for (int i = 1; i >= 0; --i) type = (type << 8) + buf[i];
  //printf("%d\n",type);
  assert(type == EXPECT_TYPE);

  /*==== get entry point address ====*/
  uintptr_t entry_point_address = 0;
  ramdisk_read(buf, 24, 4);
  for (int i = 3; i >= 0; --i) entry_point_address = (entry_point_address << 8) + buf[i];

  /*==== get program header basic infos ====*/
  uint32_t pg_header_index  = 0;       
  uint32_t pg_header_len    = 0;
  uint32_t pg_header_number = 0;
  
  ramdisk_read(buf, 28, 4);
  for(int i = 3; i >= 0; --i) pg_header_index = (pg_header_index << 8) + buf[i];
  ramdisk_read(buf, 42, 2);
  for(int i = 1; i >= 0; --i) pg_header_len   = (pg_header_len << 8) + buf[i];
  ramdisk_read(buf, 44, 4);
  for(int i = 1; i >= 0; --i) pg_header_number = (pg_header_number << 8) + buf[i];

  /*==== handle program header=====*/
  for(int i = 0; i < pg_header_number; ++i){
    uint32_t type = 0;
    uint32_t base = pg_header_index + i * pg_header_len;
    ramdisk_read(buf, base, 4);
    for(int i = 3;i >= 0;--i) type = (type << 8) + buf[i];

    // judeg LOAD
    if(type == 1)  
    {
      printf("come in\n");

      uint32_t offset   = 0;
      uint32_t virtAddr = 0;
      uint32_t fileSiz  = 0;
      uint32_t memSiz   = 0;

      ramdisk_read(buf, base + 4, 4);
      for (int i = 3; i >= 0; --i) offset = (offset << 8) + buf[i];
      ramdisk_read(buf, base + 8, 4);
      for (int i = 3; i >= 0; --i) virtAddr = (virtAddr << 8) + buf[i];
      ramdisk_read(buf, base + 16, 4);
      for (int i = 3; i >= 0; --i) fileSiz = (fileSiz << 8) + buf[i];
      ramdisk_read(buf, base + 20, 4);
      for (int i = 3; i >= 0; --i) memSiz = (memSiz << 8) + buf[i];

      printf("tend to handle\n");

      char *buf_malloc = (char*)malloc(fileSiz * sizeof(char) + 1);
      ramdisk_read(buf_malloc, offset, fileSiz);

      printf("read finish\n");

      memcpy((void *)virtAddr, buf_malloc, fileSiz);
      memset((void *)(virtAddr + fileSiz), 0, memSiz - fileSiz);

      printf("finish handle\n");
    }
  }
  
  return entry_point_address;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

