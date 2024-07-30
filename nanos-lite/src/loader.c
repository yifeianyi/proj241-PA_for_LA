#include <proc.h>
// #include <usr>
#include </usr/include/elf.h>
#include <fs.h>
#include "ramdisk.h"

/* new insert: EM_Loogarch32r */
#define EM_Loongarch32r 258
#define PT_LOAD          1

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

/*
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr eh;
  ramdisk_read(&eh, 0, sizeof(Elf_Ehdr));
  // check elf
  assert((*(uint32_t *)eh.e_ident == 0x464c457f));
  // check ISA
  assert(eh.e_machine == EM_Loongarch32r);
  
  Elf_Phdr ph[eh.e_phnum];
  ramdisk_read(ph, eh.e_phoff, sizeof(Elf_Phdr)*eh.e_phnum);
  for (int i = 0; i < eh.e_phnum; i++) {
    if (ph[i].p_type == PT_LOAD) {
      
      ramdisk_read((void*)ph[i].p_vaddr, ph[i].p_offset, ph[i].p_memsz);
      
      memset((void*)(ph[i].p_vaddr+ph[i].p_filesz), 0, ph[i].p_memsz - ph[i].p_filesz);
    }
  }
  
  return eh.e_entry;
}
*/

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename);
  if (fd < 0) {
    panic("should not reach here");
  }
  Elf_Ehdr elf;
 
  assert(fs_read(fd, &elf, sizeof(elf)) == sizeof(elf));
  // check elf
  assert(*(uint32_t *)elf.e_ident == 0x464c457f);
  // check ISA
  assert(elf.e_machine == EM_Loongarch32r);

  Elf_Phdr phdr;
  for (int i = 0; i < elf.e_phnum; i++) {
    uint32_t base = elf.e_phoff + i * elf.e_phentsize;
 
    fs_lseek(fd, base, 0);
    assert(fs_read(fd, &phdr, elf.e_phentsize) == elf.e_phentsize);
    
    // need to Load
    if (phdr.p_type == PT_LOAD) {
 
      char * buf_malloc = (char *)malloc(phdr.p_filesz);
 
      fs_lseek(fd, phdr.p_offset, 0);
      assert(fs_read(fd, buf_malloc, phdr.p_filesz) == phdr.p_filesz);
      
      memcpy((void*)phdr.p_vaddr, buf_malloc, phdr.p_filesz);
      memset((void*)phdr.p_vaddr + phdr.p_filesz, 0, phdr.p_memsz - phdr.p_filesz);
      
      free(buf_malloc);
    }
  }
 
  assert(fs_close(fd) == 0);

  return elf.e_entry;
}



void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

uintptr_t uloader(PCB *pcb, const char *filename){
  return loader(pcb, filename);
} 