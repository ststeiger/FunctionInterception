
#ifndef OPCODE_H
#define OPCODE_H





#ifndef OPCODE_NOT_DEFINED
#define OPCODE_NOT_DEFINED 1
#else
// Defined IA32-80x86 by default, but abort program at runtime because OP-Code is not defined
#undef OPCODE_NOT_DEFINED
#define OPCODE_NOT_DEFINED 1
#endif


// ========================

#ifndef JMP_OPCODE
#define JMP_OPCODE 0xE9
#endif

#ifndef OPCODE_LENGTH
#define OPCODE_LENGTH 1
#endif

#ifndef DATATYPE_ADDRESS
#define DATATYPE_ADDRESS long
#endif

#ifndef ADDRESS_LENGTH
#define ADDRESS_LENGTH (sizeof(DATATYPE_ADDRESS))
#endif

#ifndef MIN_REQUIRED_FOR_DETOUR
#define MIN_REQUIRED_FOR_DETOUR (OPCODE_LENGTH + ADDRESS_LENGTH)
#endif

#ifndef INT_DETOUR_FACTOR
#define INT_DETOUR_FACTOR 1
#endif

#ifdef OPCODE_NOT_DEFINED
#undef OPCODE_NOT_DEFINED
#define OPCODE_NOT_DEFINED 0
#endif




// Select Processor
#if ( defined (__x86__) || defined (__x86) || defined (_x86) || defined (x86) || \
	defined (__i386__) || defined (__i386) || defined (_i386) || defined (i386) || \
	defined (__m_ix86__) || defined (__m_ix86) || defined (_m_ix86) || defined (m_ix86))
// little endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Defines for IA-32 80x86 Architecture
#define JMP_OPCODE 0xE9
#define OPCODE_LENGTH 1
#define DATATYPE_ADDRESS long
#define ADDRESS_LENGTH (sizeof(DATATYPE_ADDRESS))
#define MIN_REQUIRED_FOR_DETOUR (OPCODE_LENGTH + ADDRESS_LENGTH)
#define INT_DETOUR_FACTOR 1
#define OPCODE_NOT_DEFINED 0

// For Mac only
#define MAC_DETLEN_RE_RenderScene 7
#define MAC_DETLEN_RE_Com_MD5File 7
#define MAC_DETLEN_RE_VM_Create 7
#define MAC_DETLEN_RE_VM_CallCompiled 7
#define MAC_DETLEN_RE_VM_Free 7
#define MAC_DETLEN_RE_Sys_ConsoleInputShutdown 7
// end of Mac only
#endif


#if ( defined (__x86_64__) || defined (__x86_64) || defined (_x86_64)|| defined (x86_64) || \
	defined (__ia64__) || defined (__ia64) || defined (_ia64) || defined (ia64) || \
	defined (__m_ia64__) || defined (__m_ia64) || defined (_m_ia64) || defined (m_ia64) || \
	defined (__amd64__) || defined (__amd64) || defined (_amd64) || defined (amd64))
// switchable endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Defines for IA-64 80x86 architecture 
// To be done: don't know if they work if it's truly 64 bit, but assume full 32-bit compatibility
#define JMP_OPCODE 0xE9
#define OPCODE_LENGTH 1
#define DATATYPE_ADDRESS long
#define ADDRESS_LENGTH (sizeof(DATATYPE_ADDRESS))
#define MIN_REQUIRED_FOR_DETOUR (OPCODE_LENGTH + ADDRESS_LENGTH)
#define INT_DETOUR_FACTOR 1
#define OPCODE_NOT_DEFINED 0

// For Mac only
#define MAC_DETLEN_RE_RenderScene 7
#define MAC_DETLEN_RE_Com_MD5File 7
#define MAC_DETLEN_RE_VM_Create 7
#define MAC_DETLEN_RE_VM_CallCompiled 7
#define MAC_DETLEN_RE_VM_Free 7
#define MAC_DETLEN_RE_Sys_ConsoleInputShutdown 7
// end of Mac only
#endif


#if ( defined(__mips__) || defined(__mips) || defined(_mips) || defined(mips) )
// switchable endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Godson/Loongson processor Architecture, Lemote Box Debian GNU Linux
// To be done
#endif


#if ( defined (__alpha__) || defined (__alpha) || defined (_alpha) || defined (alpha) || \
	defined (__m_alpha__) || defined (__m_alpha) || defined (_m_alpha) || defined (m_alpha))
// little endian
// DEC alpha: switchable endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Defines for Alpha Architecture
// To be done
#endif


#if ( defined(__sparc__) || defined(__sparc) || defined(_sparc) || defined(sparc) )
// big endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Defines for 32-bit SPARC (R) Architecture
// To be done
#endif


#if (defined(__sparcv9__) || defined(__sparcv9) || defined(_sparcv9) || defined(sparcv9) )
// switchable endian
#undef JMP_OPCODE
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

// Defines for 64-bit SPARC (R) Architecture
// To be done
#endif


#if ( defined(__powerpc__) || defined(__powerpc)  || defined(_powerpc) || defined(powerpc) || \
	defined(__ppc__) || defined(__ppc) || defined(_ppc) || defined(ppc) || \
	defined(__power__) || defined(__power) || defined(_power) || defined(power))
// big endian
#undef JMP_OPCODE 
#undef OPCODE_LENGTH
#undef DATATYPE_ADDRESS
#undef ADDRESS_LENGTH
#undef MIN_REQUIRED_FOR_DETOUR
#undef INT_DETOUR_FACTOR
#undef OPCODE_NOT_DEFINED

#define BRANCH_OPCODE 0x12
#define NO_THX 0
#define YES_PLEASE 1



// Defines for PowerPC Architecture
// Is very different!!! Does not have JMP Rel32!!!
// Must change the Interception routine
// Who cares, it's anyway Intel, now

// For Mac only
#define MAC_DETLEN_RE_RenderScene 4
#define MAC_DETLEN_RE_Com_MD5File 4
#define MAC_DETLEN_RE_VM_Create 4
#define MAC_DETLEN_RE_VM_CallCompiled 4
#define MAC_DETLEN_RE_VM_Free 4
#define MAC_DETLEN_RE_Sys_ConsoleInputShutdown 4
// end of Mac only
#endif


#endif // OPCODE_H 
