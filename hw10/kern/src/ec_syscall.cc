#include "ec.h"
#include "ptab.h"
#include "bits.h"
#include "string.h"

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yield  = 5,
} Syscall_numbers;




void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);

    switch (number) {
        case sys_print: {
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_sum: {
            // Naprosto nepotřebné systémové volání na sečtení dvou čísel
            int first_number = r->esi;
            int second_number = r->edi;
            r->eax = first_number + second_number;
            break;
        }
        case sys_break: {
        
            mword first_break = break_current;
            mword address = r->esi;
            
            mword pages_address_break = align_up(address, PAGE_SIZE);
            mword normal_break_for_page = align_up(break_current, PAGE_SIZE);
            
            if (address == 0) {
                
                r->eax = break_current;
                break;

            }else if(address < break_min || address > 0xc0000000) {
                
                r->eax = 0x0;
                break;
            }
            
            //void * p = reinterpret_cast<mword>(Kalloc::phys2virt(Ptab::get_mapping(normal_break_for_page - PAGE_SIZE) & ~PAGE_SIZE)) + (break_current % PAGE_SIZE);
            if(break_current % PAGE_SIZE){
                memset (reinterpret_cast<void*>(reinterpret_cast<mword>(Kalloc::phys2virt(Ptab::get_mapping(normal_break_for_page - PAGE_SIZE) & ~PAGE_MASK)) 
                    + (break_current % PAGE_SIZE)), 0 , PAGE_SIZE - (break_current % PAGE_SIZE));
            }
            //alokovat a namapovat stranky
            if(break_current < address){
                
                while(normal_break_for_page != pages_address_break){
                    
                    void*stack = Kalloc::allocator.alloc_page(1,Kalloc::FILL_0); // stack --- virtual address
                    
                    if (!stack){
                        break;
                    }
                
                    if (!Ptab::insert_mapping (normal_break_for_page, Kalloc::virt2phys (stack), Ptab::PRESENT | Ptab::RW | Ptab::USER)){
                        Kalloc::allocator.free_page(stack);
                        break;
                    }
                    normal_break_for_page += PAGE_SIZE;
                }
                //failed malloc
                if(normal_break_for_page != pages_address_break){
                    
                    mword start_break_for_page = align_up(break_current, PAGE_SIZE);

                    while(normal_break_for_page != start_break_for_page){
                        
                        normal_break_for_page -= PAGE_SIZE;
                        Kalloc::allocator.free_page(Kalloc::phys2virt(Ptab::get_mapping(normal_break_for_page) & ~PAGE_MASK));
                        Ptab::insert_mapping (normal_break_for_page, 0,0);
                        
                    }
                    
                    r->eax = 0x0;
                    break;
                }
            }
            
            //free
            if (break_current > address){
                
                while(normal_break_for_page != pages_address_break){
                    
                    normal_break_for_page -= PAGE_SIZE;
                    Kalloc::allocator.free_page(Kalloc::phys2virt(Ptab::get_mapping(normal_break_for_page) & ~PAGE_MASK));
                    Ptab::insert_mapping(normal_break_for_page, 0,0);
                }
                
            }
            
            r->eax = first_break;
            break_current = address;
            
            break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}
