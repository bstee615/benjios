#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "stdint.h"
#include "regs.h"

// GDT
struct gdt_segment_descriptor_t
{
	uint16_t limit_low_16;
	uint16_t base_low_16;
	uint8_t base_mid_8;
	/**
	 * ----------------------
	 * | 7 | 6 5 | 4 | 3210 |
	 * | P | DPL | S | TYPE |
	 * ----------------------
	 *  P — Segment present
	 *  DPL — Descriptor privilege level
	 *  S — Descriptor type (0 = system; 1 = code or data)
	 *  TYPE — Segment type
	 */
	uint8_t access;
	/**
	 * -----------------------------------
	 * | 7 |  6  | 5 |  4  | 3  2  1  0  |
	 * | G | D/B | L | AVL | Limit_upp_4 |
	 * -----------------------------------
	 *  G — Granularity
	 *  D/B — Default operation size (0 = 16-bit segment; 1 = 32-bit segment)
	 * 	L — 64-bit code segment (IA-32e mode only)
	 *  AVL    — Available for use by system software
	 */

	uint8_t granularity;
	uint8_t base_upp_8;
} __attribute__((packed));
typedef struct gdt_segment_descriptor_t gdt_segment_descriptor_t;

// Special pointer to the GDT
struct gdt_ptr_t
{
	uint16_t limit; // The size of the GDT in bytes
	uint32_t base;  // The address of the first GDT entry
} __attribute__((packed));
typedef struct gdt_ptr_t gdt_ptr_t;

// Actual array of segment descriptors
#define GDT_SIZE 5
gdt_ptr_t gdt_ptr;
gdt_segment_descriptor_t gdt[GDT_SIZE];

// Defined in ASM
extern void load_gdt(unsigned int gdt_ptr_address);
extern void flush_gdt();
extern void store_gdt_ptr(unsigned int gdt_ptr_address);

void gdt_init();

// IDT
struct idt_descriptor_t
{
	uint16_t offset_low_16;
	uint16_t segment_selector;
	uint8_t unused; // Always set to 0
	/**
	 * ---------------------------
	 * | 7 | 6 5 | 4 |  3 2 1 0  |
	 * | P | DPL | S | Gate Type |
	 * ---------------------------
	 */
	uint8_t type_and_attr;
	uint16_t offset_upp_16;
} __attribute__((packed));
typedef struct idt_descriptor_t idt_descriptor_t;

struct idt_ptr_t
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_t idt_ptr_t;

extern void load_idt(uint32_t addr);

#define NUM_IRQS 256
idt_descriptor_t idt[NUM_IRQS];
idt_ptr_t idt_ptr;

void set_irq_handler(uint8_t index, uint8_t flags, uint16_t seg, uint32_t offset);
void idt_init();

void fault_handler(struct regs *r);

#endif