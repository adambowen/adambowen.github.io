---
layout: post
title: "The Case for Exceptions Part 2: Putting the Performance Argument to Bed"
tags:
  - exceptions
---



One criticism levelled against exceptions is that they negatively
impact on performance. In this article I will attempt to address this
misconception.

The first thing we need to address is what exceptions are for, exceptions are
for _exceptional circumstances_. If exceptions are being used as part of the
normal flow of control, or they are being raised frequently by performance
critical code then, yes, you probably will see a performance hit. In practice
though, exceptions should be raised rarely so the only performance impact we
need to consider is _the cost of the ability to throw and catch exceptions_.

![Exception Thrower 9000]({{ site.baseurl }}/images/ball-thrower.png "All good programmers practice their return using an automatic exception launcher.")

In fact, modern C++ compilers pretty much all implement exceptions using what
are called exception handling tables, these "zero cost exceptions" come from the
Itanium ABI and are intended to have zero performance impact if no exception is
thrown [^mortoray].

[^mortoray]: [The True Cost of Zero Cost Exceptions](https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/)

![An Exception Handler Table]({{ site.baseurl }}/images/exception-tables.png "Not that kind of table!")

The oversimplified description of how EH tables work is this: every scope
(including implicit scopes) has an entry in the EH table that describes the
_instructions_ (whose addresses are known at link time) covered by the scope,
the _personality routine_ which essentially maps to some language specific code
on how exceptions are handled and a _language specific data area_, which will be
the (again language specific) information on the exception being propagated.

The personality routine then attempts to handle the exception, as part of the
handling it may execute a catch block and stop the exception from propagating
further, or it might just execute a _cleanup_ routine that forms part of the
unwind - for example executing object destructors.

If the routine fails to handle the exception, then the runtime unwinds the stack
(most platform ABIs specify how this can be done reliably, at least from a
certain set of well defined code locations), looks up the address of the new
program counter in the EH table and tries again to handle the exception. This
process continues until the exception is handled.

The key point here is that, until an exception is handled, no new instructions
are introduced into the normal flow of control to support exceptions. Once an
exception is thrown there is a lot of overhead (because we have to do all these

If we want, we can actually see what this looks like. Take this simple code
snippet:

{% highlight cpp %}
void perf_function(int i)
{
	perf_object o;
	o.method(i);
}
{% endhighlight %}

First, let's look at the assembly g++ generates without exceptions:

	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rbx, -16(%rbp)
	movq	%r12, -8(%rbp)
	subq	$32, %rsp
	movl	%edi, %r12d
	leaq	-32(%rbp), %rbx
	movq	%rbx, %rdi
	call	__ZN11perf_objectC1Ev
	movl	%r12d, %esi
	movq	%rbx, %rdi
	call	__ZN11perf_object6methodEi
	movq	%rbx, %rdi
	call	__ZN11perf_objectD1Ev
	movq	-16(%rbp), %rbx
	movq	-8(%rbp), %r12
	leave
	ret

and now with:

	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rbx, -16(%rbp)
	movq	%r12, -8(%rbp)
	subq	$32, %rsp
	movl	%edi, %ebx
	leaq	-32(%rbp), %r12
	movq	%r12, %rdi
	call	__ZN11perf_objectC1Ev
	movl	%ebx, %esi
	movq	%r12, %rdi
	call	__ZN11perf_object6methodEi
	movq	%r12, %rdi
	call	__ZN11perf_objectD1Ev
	movq	-16(%rbp), %rbx
	movq	-8(%rbp), %r12
	leave
	ret
	movq	%rax, %rbx
	movq	%r12, %rdi
	call	__ZN11perf_objectD1Ev
	movq	%rbx, %rdi
	call	__Unwind_Resume

Apart from one instance where a different register was selected, the code leading
up to the `ret` (which represents the normal flow of control) is identical. The
only difference is the addition of a few instructions that will be invoked if
(and only if) an exception is raised. In particular we can see that the new
fragment is responsible for invoking the destructor of `o` before resuming the
unwind.

One area where exceptions will introduce a penalty is in code size, aside from
storing the exception tables we must also generate the cleanup routines for
any region of code that might see an exception. The additional data and code
can have impacts on instruction and data locality, and potentially trigger
more cache misses. Except in extreme cases where space is at a significant
premium (such as embedded systems), the additional space required for exception
handling is unlikely to introduce a significant performance overhead - it will
almost certainly be the case that there are many other inefficiencies to tackle
first.

Ultimately it seems that outlawing exceptions purely on performance grounds is
an extreme form of premature optimisation. If you believe that
exceptions are a useful tool, then you should always favour making them
available over their exclusion until you have profile driven evidence that they
are causing a performance bottleneck.
