============
GAS Assembly
============


----------------------
Addressing `ref link`_
----------------------

An Intel syntax indirect memory reference of the form
::

    section:[base + index*scale + disp]

is translated into the AT&T syntax

::

    section:disp(base, index, scale)

where base and index are the optional 32-bit base and index registers, disp is the optional displacement, and scale, taking the values 1, 2, 4, and 8, multiplies index to calculate the address of the operand. If no scale is specified, scale is taken to be 1. section specifies the optional section register for the memory operand, and may override the default section register (see a 80386 manual for section register defaults). Note that section overrides in AT&T syntax must have be preceded by a `%'. If you specify a section override which coincides with the default section register, as does not output any section register override prefixes to assemble the given instruction. Thus, section overrides can be specified to emphasize which section register is used for a given memory operand.

Here are some examples of Intel and AT&T style memory references:

::

    AT&T: `-4(%ebp)', Intel: `[ebp - 4]'
    base is `%ebp'; disp is `-4'. section is missing, and the default section is used (`%ss' for addressing with `%ebp' as the base register). index, scale are both missing.
    AT&T: `foo(,%eax,4)', Intel: `[foo + eax*4]'
    index is `%eax' (scaled by a scale 4); disp is `foo'. All other fields are missing. The section register here defaults to `%ds'.
    AT&T: `foo(,1)'; Intel `[foo]'
    This uses the value pointed to by `foo' as a memory operand. Note that base and index are both missing, but there is only one `,'. This is a syntactic exception.
    AT&T: `%gs:foo'; Intel `gs:foo'
    This selects the contents of the variable `foo' with section register section being `%gs'.
    Absolute (as opposed to PC relative) call and jump operands must be prefixed with `*'. If no `*' is specified, as always chooses PC relative addressing for jump/call labels.

Any instruction that has a memory operand must specify its size (byte, word, or long) with an opcode suffix (`b', `w', or `l', respectively).


.. _ref link: https://ftp.gnu.org/old-gnu/Manuals/gas-2.9.1/html_node/as_200.html>
