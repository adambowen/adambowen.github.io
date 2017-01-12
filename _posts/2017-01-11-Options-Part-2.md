---
layout: post
title: "Writing a Better Options Library Part 2"
tags:
  - options
---

In the last post on this topic we talked about some general characteristics we
would like to see in an options library, and what some of the existing libraries
do.

In this post we'll do some high level feature design.

### Features

We wanted the library to be _minimal_ and _complete_ - so it should provide
everything one would expect from an option parsing library and no more.
For inspiration, let's look at what options some existing tools accept - if we
can't cope with these options then we would obviously not be providing a
complete interface.

    $ cp --help
    Usage: cp [OPTION]... [-T] SOURCE DEST
      or:  cp [OPTION]... SOURCE... DIRECTORY
      or:  cp [OPTION]... -t DIRECTORY SOURCE...
    Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.

    Mandatory arguments to long options are mandatory for short options too.
      -a, --archive                same as -dR --preserve=all
          --attributes-only        don't copy the file data, just the attributes
          --backup[=CONTROL]       make a backup of each existing destination file
      -b                           like --backup but does not accept an argument
          --copy-contents          copy contents of special files when recursive
      -d                           same as --no-dereference --preserve=links
      -f, --force                  if an existing destination file cannot be
                                     opened, remove it and try again (redundant if
                                     the -n option is used)
      -i, --interactive            prompt before overwrite (overrides a previous -n
                                      option)
      -H                           follow command-line symbolic links in SOURCE
      -l, --link                   hard link files instead of copying
      -L, --dereference            always follow symbolic links in SOURCE
      -n, --no-clobber             do not overwrite an existing file (overrides
                                     a previous -i option)
      -P, --no-dereference         never follow symbolic links in SOURCE
      -p                           same as --preserve=mode,ownership,timestamps
          --preserve[=ATTR_LIST]   preserve the specified attributes (default:
                                     mode,ownership,timestamps), if possible
                                     additional attributes: context, links, xattr,
                                     all
          --no-preserve=ATTR_LIST  don't preserve the specified attributes
          --parents                use full source file name under DIRECTORY
      -R, -r, --recursive          copy directories recursively
          --reflink[=WHEN]         control clone/CoW copies. See below
          --remove-destination     remove each existing destination file before
                                     attempting to open it (contrast with --force)
          --sparse=WHEN            control creation of sparse files. See below
          --strip-trailing-slashes  remove any trailing slashes from each SOURCE
                                     argument
      -s, --symbolic-link          make symbolic links instead of copying
      -S, --suffix=SUFFIX          override the usual backup suffix
      -t, --target-directory=DIRECTORY  copy all SOURCE arguments into DIRECTORY
      -T, --no-target-directory    treat DEST as a normal file
      -u, --update                 copy only when the SOURCE file is newer
                                     than the destination file or when the
                                     destination file is missing
      -v, --verbose                explain what is being done
      -x, --one-file-system        stay on this file system
          --help     display this help and exit
          --version  output version information and exit

We can see several types of command line option here:

* "Arguments" like `SOURCE` and `DEST`, which can occur in various combinations
    (for example, one `SOURCE` and one `DEST`, or multiple `SOURCE...` with one
    target `DIRECTORY`).
* "Flags" like `--archive` or `--verbose` which can turn specific behaviour on
    or off.
* "Parameters" like `--target-directory=DIRECTORY` which can set some
    configuration setting.

We can also see a couple of interesting features:

* Flags and parameters can have a short form (`-v`) and a long form (`--verbose`).
* Short form flags can be specified all together (`-dR`)
* Some parameters or flags could affect the interpretation of later command line
    options.
* Some parameters (`--reflink[=WHEN]`) have an optional argument.
* Parameters can be specified in several ways (e.g. `--suffix=.orig`,
    `--suffix .orig` `-S.orig` and `-S .orig`).

`cp` is one of the simpler options, when it comes to command line options
[gcc](https://linux.die.net/man/1/gcc) has more than most.
If we look through the GCC man page we can see a few variations on the types of
option accepted by `cp`:

* GCC does _not_ accept combined single letter flags.
* Some options (e.g. `-Idir`) can be specified multiple times with different
   parameters.
* Some options (`-f` and `-W`) could be considered an option with a parameter
  (`-Wall` could be thought of as `-W` with parameter `all`), or as a self
  contained option (just `-Wall`). Some usages suggest the latter as there is an
  additional parameter (`-fabi-version=n`).
* Some options (`-DMACRO=VALUE`) use the equals sign as part of their parameter.

We also said we wanted our library to be cross platform, so we should look at
some Windows programs too.

    > cl /help

    Microsoft (R) C/C++ Optimizing Compiler Version 19.00.24213.1 for x86
    Copyright (C) Microsoft Corporation.  All rights reserved.

                             C/C++ COMPILER OPTIONS


                                  -OPTIMIZATION-

    /O1 minimize space                      /O2 maximize speed
    /Ob<n> inline expansion (default n=0)   /Od disable optimizations (default)
    /Og enable global optimization          /Oi[-] enable intrinsic functions
    /Os favor code space                    /Ot favor code speed
    /Ox maximum optimizations               /Oy[-] enable frame pointer omission
    /favor:<blend|ATOM> select processor to optimize for, one of:
        blend - a combination of optimizations for several different x86 processors
        ATOM - Intel(R) Atom(TM) processors

                                 -CODE GENERATION-

    /Gw[-] separate global variables for linker
    /GF enable read-only string pooling     /Gm[-] enable minimal rebuild
    /Gy[-] separate functions for linker    /GS[-] enable security checks
    /GR[-] enable C++ RTTI                  /GX[-] enable C++ EH (same as /EHsc)
    /guard:cf[-] enable CFG (control flow guard)
    /EHs enable C++ EH (no SEH exceptions)  /EHa enable C++ EH (w/ SEH exceptions)
    /EHc extern "C" defaults to nothrow
    /EHr always generate noexcept runtime termination checks
    /fp:<except[-]|fast|precise|strict> choose floating-point model:
        except[-] - consider floating-point exceptions when generating code
        fast - "fast" floating-point model; results are less predictable
        precise - "precise" floating-point model; results are predictable
        strict - "strict" floating-point model (implies /fp:except)
    /Qfast_transcendentals generate inline FP intrinsics even with /fp:except
    /Qpar[-] enable parallel code generation
    /Qpar-report:1 auto-parallelizer diagnostic; indicate parallelized loops
    /Qpar-report:2 auto-parallelizer diagnostic; indicate loops not parallelized
    /Qvec-report:1 auto-vectorizer diagnostic; indicate vectorized loops
    /Qvec-report:2 auto-vectorizer diagnostic; indicate loops not vectorized
    /GL[-] enable link-time code generation
    /volatile:<iso|ms> choose volatile model:
        iso - Acquire/release semantics not guaranteed on volatile accesses
        ms  - Acquire/release semantics guaranteed on volatile accesses
    /GA optimize for Windows Application    /Ge force stack checking for all funcs
    /Gs[num] control stack checking calls   /Gh enable _penter function call
    /GH enable _pexit function call         /GT generate fiber-safe TLS accesses
    /RTC1 Enable fast checks (/RTCsu)       /RTCc Convert to smaller type checks
    /RTCs Stack Frame runtime checking      /RTCu Uninitialized local usage checks
    /clr[:option] compile for common language runtime, where option is:
        pure - produce IL-only output file (no native executable code)
        safe - produce IL-only verifiable output file
        initialAppDomain - enable initial AppDomain behavior of Visual C++ 2002
        noAssembly - do not produce an assembly
        nostdlib - ignore the default \clr directory
    /Gd __cdecl calling convention          /Gr __fastcall calling convention
    /Gz __stdcall calling convention        /GZ Enable stack checks (/RTCs)
    /Gv __vectorcall calling convention     /QIfist[-] use FIST instead of ftol()
    /hotpatch ensure function padding for hotpatchable images
    /arch:<IA32|SSE|SSE2|AVX|AVX2> minimum CPU architecture requirements, one of:
       IA32 - use no enhanced instructions and use x87 for floating point
       SSE - enable use of instructions available with SSE-enabled CPUs
       SSE2 - (default) enable use of instructions available with SSE2-enabled CPUs
       AVX - enable use of instructions available with AVX-enabled CPUs
    /Fm[file] name map file                 /Fo<file> name object file
    /Fp<file> name precompiled header file  /Fr[file] name source browser file
    /FR[file] name extended .SBR file       /Fi[file] name preprocessed file
    /Fd: <file> name .PDB file              /Fe: <file> name executable file
    /Fm: <file> name map file               /Fo: <file> name object file
    /Fp: <file> name .PCH file              /FR: <file> name extended .SBR file
    /Fi: <file> name preprocessed file
    /doc[file] process XML documentation comments and optionally name the .xdc file

                                  -PREPROCESSOR-

    /AI<dir> add to assembly search path    /FU<file> forced using assembly/module
    /C don't strip comments                 /D<name>{=|#}<text> define macro
    /E preprocess to stdout                 /EP preprocess to stdout, no #line
    /P preprocess to file                   /Fx merge injected code to file
    /FI<file> name forced include file      /U<name> remove predefined macro
    /u remove all predefined macros         /I<dir> add to include search path
    /X ignore "standard places"

                                    -LANGUAGE-

    /Zi enable debugging information        /Z7 enable old-style debug info
    /Zp[n] pack structs on n-byte boundary  /Za disable extensions
    /Ze enable extensions (default)         /Zl omit default library name in .OBJ
    /Zs syntax check only                   /vd{0|1|2} disable/enable vtordisp
    /vm<x> type of pointers to members
    /Zc:arg1[,arg2] C++ language conformance, where arguments can be:
      forScope[-]           enforce Standard C++ for scoping rules
      wchar_t[-]            wchar_t is the native type, not a typedef
      auto[-]               enforce the new Standard C++ meaning for auto
      trigraphs[-]          enable trigraphs (off by default)
      rvalueCast[-]         enforce Standard C++ explicit type conversion rules
      strictStrings[-]      disable string-literal to [char|wchar_t]*
                            conversion (off by default)
      implicitNoexcept[-]   enable implicit noexcept on required functions
      threadSafeInit[-]     enable thread-safe local static initialization
      inline[-]             remove unreferenced function or data if it is
                            COMDAT or has internal linkage only (off by default)
      sizedDealloc[-]       enable C++14 global sized deallocation
                            functions (on by default)
      throwingNew[-]        assume operator new throws on failure (off by default)
      referenceBinding[-]   a temporary will not bind to an non-const
                            lvalue reference (off by default)
    /ZH:SHA_256             use SHA256 for file checksum in debug info (experimental)

    /Zo[-] generate richer debugging information for optimized code (on by default)
    /ZW enable WinRT language extensions
    /constexpr:depth<N>     use <N> as the recursion depth limit
                            for constexpr (default: 512)
    /constexpr:backtrace<N> show <N> constexpr evaluations
                            in diagnostics (default: 10)
    /constexpr:steps<N>     terminate constexpr evaluation after
                            <N> steps (default: 100000)
    /ZI enable Edit and Continue debug info
    /openmp enable OpenMP 2.0 language extensions

                                  -MISCELLANEOUS-

    @<file> options response file           /?, /help print this help message
    /bigobj generate extended object format /c compile only, no link
    /errorReport:option Report internal compiler errors to Microsoft
        none - do not send report
        prompt - prompt to immediately send report
        queue - at next admin logon, prompt to send report (default)
        send - send report automatically
    /FC use full pathnames in diagnostics   /H<num> max external name length
    /J default char type is unsigned
    /MP[n] use up to 'n' processes for compilation
    /nologo suppress copyright message
    /sdl enable additional security features and warnings
    /showIncludes show include file names   /Tc<source file> compile file as .c
    /Tp<source file> compile file as .cpp   /TC compile all files as .c
    /TP compile all files as .cpp           /V<string> set version string
    /w disable all warnings                 /wd<n> disable warning n
    /we<n> treat warning n as an error      /wo<n> issue warning n once
    /w<l><n> set warning level 1-4 for n    /W<n> set warning level (default n=1)
    /Wall enable all warnings               /WL enable one line diagnostics
    /WX treat warnings as errors            /Yc[file] create .PCH file
    /Yd put debug info in every .OBJ        /Yl[sym] inject .PCH ref for debug lib
    /Yu[file] use .PCH file                 /Y- disable all PCH options
    /Zm<n> max memory alloc (% of default)  /FS force to use MSPDBSRV.EXE
    /await enable resumable functions extension
    /Wv:xx[.yy[.zzzzz]] disable warnings introduced after version xx.yy.zzzzz
    /source-charset:<iana-name>|.nnnn set source character set
    /execution-charset:<iana-name>|.nnnn set execution character set
    /utf-8 set source and execution character set to UTF-8
    /validate-charset[-] validate UTF-8 files for only legal characters

                                    -LINKING-

    /LD Create .DLL                         /LDd Create .DLL debug library
    /LN Create a .netmodule                 /F<num> set stack size
    /link [linker options and libraries]    /MD link with MSVCRT.LIB
    /MT link with LIBCMT.LIB                /MDd link with MSVCRTD.LIB debug lib
    /MTd link with LIBCMTD.LIB debug lib

                                  -CODE ANALYSIS-

    /analyze[-] Enable native analysis      /analyze:quiet[-] No warning to console
    /analyze:log<name> Warnings to file     /analyze:autolog Log to *.pftlog
    /analyze:autolog:ext<ext> Log to *.<ext>/analyze:autolog- No log file
    /analyze:WX- Warnings not fatal         /analyze:stacksize<num> Max stack frame
    /analyze:max_paths<num> Max paths       /analyze:only Analyze, no code gen

There are a couple of new points here:

* The character used to start a flag or parameter is `/`, but this would obviously
  be inappropriate on Unix filesystems.
* A colon (`:`) is frequently used to separate an option from some kind of
  parameter.
* An `@` can be used to read options from a file.
* A `-` can be used to turn options off.
* Some options take a variable number of parameters (`/Zc:arg1[,arg2]`).

As a final use case we need to consider programs like GDB and Valgrind which
_wrap_ another program and forward arguments.
For example, we can start GDB with `gdb a.out --args arg1 arg2 arg3` to pass
`arg1 arg2 arg3` through to `a.out`, similarly with Valgrind we would have
`valgrind a.out arg1 arg2 arg3`.
With CMake we can use `cmake --build <dir> -- <native build tool options>` to
forward options to the build tool.

If we want to support programs like this we will need to make sure our argument
parser is able to recognise constructs like this, and treat them appropriately.

## Yet another options library

![YAOL]({{ site.baseurl }}/images/yaol.png "Yet another yet another meme")

So, parameter wise we need to support

* Positional arguments, that may be optional or occur multiple times.
* Options, which are named switches to the program and must be specified separately.
* Flags, which are named switches that may be collapsed into a single argument.
* Parameters to options and flags.
* Options and flags can be specified any number of times.
* Configurable "switch" prefix (`-` or `/`).
* Configurable parameter separation.
* Arguments, options, and flags need to be able to modify the interpretation of later arguments, options, and flags.

In the next post of this series we'll start to flesh out an interface design.
