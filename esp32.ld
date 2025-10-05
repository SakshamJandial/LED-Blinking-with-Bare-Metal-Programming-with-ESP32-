ENTRY(_start)

MEMORY
{
  DRAM (rwx) : ORIGIN = 0x3FFB0000, LENGTH = 128K
  IRAM (rwx) : ORIGIN = 0x40080000, LENGTH = 128K
}

SECTIONS
{
  /* The entry section.We collect the entry code AND its own literal pool together. */
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