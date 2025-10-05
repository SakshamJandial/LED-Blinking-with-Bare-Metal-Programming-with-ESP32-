/*
 * Final Bare-Metal Linker Script for ESP32 (v3)
 * Correctly handles all literal placements, including the entry section.
 */
ENTRY(_start)

MEMORY
{
  DRAM (rwx) : ORIGIN = 0x3FFB0000, LENGTH = 128K
  IRAM (rwx) : ORIGIN = 0x40080000, LENGTH = 128K
}

SECTIONS
{
  /* The entry section. The key change is here: we now collect
     the entry code AND its own literal pool together first. */
  .entry.text :
  {
    *(.entry.literal .entry.text)
  } > IRAM

  /* The main code section. Collect all other code and literals. */
  .text :
  {
    *(.literal .text*)
  } > IRAM

  /* Define the top of the stack at the end of Data RAM */
  _stack_top = ORIGIN(DRAM) + LENGTH(DRAM);
}