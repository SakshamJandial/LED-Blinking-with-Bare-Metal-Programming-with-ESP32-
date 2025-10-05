.section .entry.text, "ax"  # Place this in a special entry section
.global _start              # Make the _start symbol visible to the linker

_start:
    # Set up the stack pointer.
    movi sp, _stack_top

    # Now that the stack is set up, we can call our C function.
    call0 app_main

# This should never be reached, but as a safeguard,
# create an infinite loop here to catch the CPU if app_main ever returns.
_hang:
    j _hang