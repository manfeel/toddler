#include "common/include/data.h"
#include "hal/include/print.h"
#include "hal/include/cpu.h"
#include "hal/include/int.h"


dec_per_cpu(int, cur_in_user_mode);
dec_per_cpu(u32, cur_page_dir);


void init_int()
{
    u32 ebase = 0;
    u32 sr = 0;
    
    // Obtain old SR
    __asm__ __volatile__ (
        "mfc0   %0, $12;"
        : "=r" (sr)
        :
    );
    
    // Set BEV to 0 -> use our custom handlers
    sr &= ~0x400000;
    
    // Update SR
    __asm__ __volatile__ (
        "mtc0   %0, $12;"
        "nop;"
        :
        : "r" (sr)
    );
    
    // Obtain the old ebase
    __asm__ __volatile__ (
        "mfc0   %0, $15, 1;"
        : "=r" (ebase)
        :
    );
    
    // Clear and set exception base
    ebase &= ~0x3FFFF000;
    ebase |= ((u32)&int_entry_template_begin) & ~0xFFF;
    
    // Update ebase
    __asm__ __volatile__ (
        "mtc0   %0, $15, 1;"
        "nop;"
        :
        : "r" (ebase)
    );
    
    kprintf("Interrupt base updated, Template starts @ %x, SR: %x, EBase: %x\n", (u32)&int_entry_template_begin, sr, ebase);
    
//     // Test our handler
//     volatile u32 *bad_addr = (u32 *)0x4096;
//     u32 bad_value = *bad_addr;
//     kprintf("Bad value: %x\n", bad_value);
}

void tlb_refill_handler()
{
    kprintf("TLB Refill!\n");
    while (1);
}

void cache_error_handler()
{
    kprintf("Toddler doesn't support cache error handling!!\n");
    while (1);
}

void general_except_handler()
{
    kprintf("General exception!\n");
    while (1);
}