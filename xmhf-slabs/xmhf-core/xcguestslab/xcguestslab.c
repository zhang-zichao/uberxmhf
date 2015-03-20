/*
 * @XMHF_LICENSE_HEADER_START@
 *
 * eXtensible, Modular Hypervisor Framework (XMHF)
 * Copyright (c) 2009-2012 Carnegie Mellon University
 * Copyright (c) 2010-2012 VDG Inc.
 * All Rights Reserved.
 *
 * Developed by: XMHF Team
 *               Carnegie Mellon University / CyLab
 *               VDG Inc.
 *               http://xmhf.org
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of Carnegie Mellon or VDG Inc, nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @XMHF_LICENSE_HEADER_END@
 */

#include <xmhf.h>
#include <xmhfhicslab.h>
#include <xmhf-debug.h>

#include <xc.h>
#include <xcguestslab.h>

//////
//XMHF_SLAB_GUEST(xcguestslab)



/*
 * slab code
 *
 * author: amit vasudevan (amitvasudevan@acm.org)
 */


static void xcguestslab_dotest_vmcall(void){

    {
        u64 tscbefore, tscafter, tscavg=0;
        u32 iterations=8192;
        u32 i;

        _XDPRINTF_("%s: proceeding with test...\n", __func__);



        for(i=0; i < iterations; i++){
            tscbefore = rdtsc64();

            {

                //asm volatile ("vmcall \r\n");

            }

            tscafter = rdtsc64();
            tscavg += (tscafter - tscbefore);
        }

        tscavg = tscavg / iterations;

        _XDPRINTF_("%s: clock cycles for test = %u\n", __func__, (u32)tscavg);

    }



}


static void xcguestslab_do_xmhfhw_cpu_cpuid(void){
    u32 dummy;
    u32 vendor_dword1, vendor_dword2, vendor_dword3;


    _XDPRINTF_("%s: Preparing to execute CPUID...\n",
                __func__);

    xmhfhw_cpu_cpuid(0, &dummy, &vendor_dword1, &vendor_dword3, &vendor_dword2);

    _XDPRINTF_("%s: CPUID(0) %x, %x, %x\n",
                __func__, vendor_dword1, vendor_dword2, vendor_dword3);
}


static void xcguestslab_do_msrtest(void){
    u64 sysenter_cs_msr;


    wrmsr64(IA32_SYSENTER_CS_MSR, 0xAA);

    _XDPRINTF_("%s: wrote SYSENTER_CS_MSR.\n", __func__);

    sysenter_cs_msr = rdmsr64(IA32_SYSENTER_CS_MSR);

    _XDPRINTF_("%s: read SYSENTER_CS_MSR=%016llx...\n", __func__, sysenter_cs_msr);

}






static void xcguestslab_do_testxhssteptrace(void){
    /*u64 gpa = &_xcguestslab_do_testxhssteptrace_func;

    _XDPRINTF_("%s: Going to register function at %016llx\n", __func__, gpa);

    asm volatile(
        "movl %0, %%eax \r\n"
        "movl %1, %%edx \r\n"
        "movl %2, %%ebx \r\n"
        "vmcall \r\n"
        :
        : "i" (SSTEPTRACE_REGISTER),
          "g" ( (u32) ((u64)(gpa >> 32)) ),
          "g" ((u32)gpa)
        : "eax", "ebx", "edx"
    );

    _XDPRINTF_("%s: Registered function\n", __func__);
*/
    _XDPRINTF_("%s: Proceeding to call function...\n", __func__);

    _xcguestslab_do_testxhssteptrace_func();

    _XDPRINTF_("%s: Came back from calling function.\n", __func__);

}




/*
//////
// syscalllog test harness


//*
// GDT
__attribute__(( aligned(16) )) u64 _xcguestslab_gdt_start[]  = {
	0x0000000000000000ULL,	//NULL descriptor
	0x00cf9a000000ffffULL,	//CPL-0 32-bit code descriptor (CS32)
	0x00cf92000000ffffULL,	//CPL-0 32-bit data descriptor (DS/SS/ES/FS/GS)
	0x00cffa000000ffffULL,	//TODO: CPL-3 32-bit code descriptor (CS32)
	0x00cff2000000ffffULL,	//TODO: CPL-3 32-bit data descriptor (DS/SS/ES/FS/GS)
	0x00cffa000000ffffULL,	//TODO: CPL-3 32-bit code descriptor (CS32)
	0x00cff2000000ffffULL,	//TODO: CPL-3 32-bit data descriptor (DS/SS/ES/FS/GS)
	0x0000000000000000ULL,  //TSS descriptors
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,
	0x0000000000000000ULL,

	0x0000000000000000ULL,
};

// GDT descriptor
__attribute__(( aligned(16) )) arch_x86_gdtdesc_t _xcguestslab_gdt  = {
	.size=sizeof(_xcguestslab_gdt_start)-1,
	.base=(u32)&_xcguestslab_gdt_start,
};


/////////
#define SYSCALLLOG_REGISTER     			0xF0

static u8 _xcguestslab_do_testxhsyscalllog_sysenterhandler_stack[PAGE_SIZE_4K];




void xcguestslab_do_testxhsyscalllog(void){
    u64 gpa = &_xcguestslab_do_testxhsyscalllog_sysenterhandler;

    _XDPRINTF_("%s: proceeding to load GDT\n", __FUNCTION__);

    //load GDTR
	asm volatile(
		"lgdt  %0 \r\n"
		"pushl	%1 \r\n"
		"pushl	$reloadsegs \r\n"
		"lret \r\n"
		"reloadsegs: \r\n"
		"movw	%2, %%ax \r\n"
		"movw	%%ax, %%ds \r\n"
		"movw	%%ax, %%es \r\n"
		"movw	%%ax, %%fs \r\n"
		"movw	%%ax, %%gs \r\n"
		"movw   %%ax, %%ss \r\n"
		: //no outputs
		: "m" (_xcguestslab_gdt), "i" (__CS_CPL0), "i" (__DS_CPL0)
		: "rax"
	);

    _XDPRINTF_("%s: GDT loaded\n", __FUNCTION__);


    //set IOPL to CPL-3
	asm volatile(
        "pushfl \r\n"
        "popl %%eax \r\n"
		"orl $0x3000, %%eax \r\n"					// clear flags, but set IOPL=3 (CPL-3)
		"pushl %%eax \r\n"
		"popfl \r\n"
		: //no outputs
		: //no inputs
		: "eax", "cc"
	);


    //register syscall handler
    asm volatile(
        "movl %0, %%eax \r\n"
        "movl %1, %%edx \r\n"
        "movl %2, %%ebx \r\n"
        "vmcall \r\n"
        :
        : "i" (SYSCALLLOG_REGISTER),
          "g" ( (u32) ((u64)(gpa >> 32)) ),
          "g" ((u32)gpa)
        : "eax", "ebx", "edx"
    );


    _XDPRINTF_("%s: registered syscall handler on page %x\n", __FUNCTION__, gpa);



    //setup SYSENTER/SYSEXIT mechanism
    {
        wrmsr64(IA32_SYSENTER_CS_MSR, __CS_CPL0);
        wrmsr64(IA32_SYSENTER_EIP_MSR, &_xcguestslab_do_testxhsyscalllog_sysenterhandler);
        wrmsr64(IA32_SYSENTER_ESP_MSR, ((u32)&_xcguestslab_do_testxhsyscalllog_sysenterhandler_stack + (u32)PAGE_SIZE_4K));
    }
    _XDPRINTF_("%s: setup SYSENTER/SYSEXIT mechanism\n", __FUNCTION__);
    _XDPRINTF_("%s: SYSENTER CS=%016llx\n", __FUNCTION__, rdmsr64(IA32_SYSENTER_CS_MSR));
    _XDPRINTF_("%s: SYSENTER RIP=%016llx\n", __FUNCTION__, rdmsr64(IA32_SYSENTER_EIP_MSR));
    _XDPRINTF_("%s: SYSENTER RSP=%016llx\n", __FUNCTION__, rdmsr64(IA32_SYSENTER_ESP_MSR));


    //switch to ring-3
    asm volatile(
         "movl $1f, %%edx \r\n"
         "movl %%esp, %%ecx \r\n"
         "sysexit \r\n"
         "1: \r\n"
        :
        :
        : "edx", "ecx"
    );

    _XDPRINTF_("%s: Guest Slab at Ring-3. Proceeding to execute sysenter...Halting!\n", __FUNCTION__);

    //invoke sysenter
    asm volatile(
         "movl $1f, %%edx \r\n"
         "movl %%esp, %%ecx \r\n"
         "sysenter \r\n"
         "1: \r\n"
        :
        :
        : "edx", "ecx"
    );

    _XDPRINTF_("%s: Came back from SYSENTER\n", __FUNCTION__);


    _XDPRINTF_("%s: Guest Slab Halting\n", __FUNCTION__);
    HALT();
}

*/





void slab_main(slab_params_t *sp){
    _XDPRINTF_("%s: Hello world from Guest slab!\n", __func__);

    //xcguestslab_do_vmcall();

    //xcguestslab_do_xmhfhw_cpu_cpuid();

    //xcguestslab_do_msrtest();

    //xcguestslab_do_testxhhyperdep();

    //xcguestslab_do_testxhapprovexec();

    //xcguestslab_do_testxhssteptrace();

    xcguestslab_do_testxhsyscalllog();

    _XDPRINTF_("%s: Guest Slab Halting\n", __func__);
    HALT();
}
