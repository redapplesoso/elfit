#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

#define PAGE_SIZE 4096
#define MAX_FILENAME 100
#define TMP "tmp.bin"

typedef struct {
    char *name;
    unsigned char *mem;
    struct stat *file; 
} Elfit_t;

typedef struct {
    /* name of host file */
    char host[MAX_FILENAME];
    /* name of parasite file */
    char parasite[MAX_FILENAME];
    /* address to patch parasite code with */
    unsigned long patch_addr;
    /* patch position */
    unsigned int patch_pos;
    /* entrypoint redirect */
    unsigned int entrypoint;
    /* got table redirect */
    unsigned int gottable; 
    char pltsymbol[MAX_FILENAME];
    /* ctors redirect */
    unsigned int ctors; 
    /* dtors redirect */
    unsigned int dtors; 
    /* arbitrary function hook */
    unsigned int arbfunc; 
    /* text padding method */
    unsigned int textpadding; 
    /* reverse padding method */
    unsigned int reversepadding; 
    /* shared object method */
    unsigned int soinject; 
    /* et_rel method */
    unsigned int etrelinject; 
} opts_t;


/* Redirectors */
int entry_redirect_32(Elfit_t *, uint32_t);
int entry_redirect_64(Elfit_t *, uint64_t);

Elf32_Addr got_redirect_32(Elfit_t *, char *, uint32_t);

/* Injectors */
uint32_t textpadding_inject_32(Elfit_t *, char *, uint32_t, uint32_t);
uint64_t textpadding_inject_64(Elfit_t *, char *, uint64_t, uint64_t);

/* Utilities */
int load_host(char *, Elfit_t *);
void unload_host(Elfit_t *);

int get_section_by_name_32(char *, Elfit_t *);
int get_section_by_name_64(char *, Elfit_t *);

/* Interface */
opts_t *usage(int, char **);
