// University of Illinois/NCSA
// Open Source License
// 
// Copyright (c) 2013, Advanced Micro Devices, Inc.
// All rights reserved.
// 
// Developed by:
// 
//     Runtimes Team
// 
//     Advanced Micro Devices, Inc
// 
//     www.amd.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimers.
// 
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimers in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the names of the LLVM Team, University of Illinois at
//       Urbana-Champaign, nor the names of its contributors may be used to
//       endorse or promote products derived from this Software without specific
//       prior written permission.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
// SOFTWARE.
//===----------------------------------------------------------------------===//

#include "okraContext.h"
#include <stdio.h>

// These functions are here to expose an "extern C" interaface into Okra
// that can be dynamically loaded and looked up using dlopen/dlsym etc.
// These functions do not use JNI.
// The workflow is create context, create kernel, clear args (if not first use).
// push args, execute
extern "C" DLLExport  void* okra_create_context() {
    return OkraContext::Create();
}

extern "C" DLLExport  void* okra_create_kernel(void* context, const char *source, const char *entryName) {
    return (void*) ((OkraContext*)context)->createKernel(source, entryName);
}

extern "C" DLLExport  bool okra_push_object(void* kernel, void* object) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushPointerArg(object);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_boolean(void* kernel, jboolean val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushBooleanArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_byte(void* kernel, jbyte val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushByteArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_double(void* kernel, jdouble val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushDoubleArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_float(void* kernel, jfloat val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushFloatArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_int(void* kernel, jint val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushIntArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  bool okra_push_long(void* kernel, jlong val) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->pushLongArg(val);
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

// At this time we only support 1-d kernel ranges
extern "C" DLLExport  bool okra_execute_with_range(void* kernel, jint numWorkItems) {
    OkraContext::OkraStatus okraStatus;    
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    size_t globalDims[] = {numWorkItems}; 
    size_t localDims[] = {0};
    okraStatus = realKernel->setLaunchAttributes(1, globalDims, localDims);
    if (okraStatus == OkraContext::OKRA_OK) {
        okraStatus = realKernel->dispatchKernelWaitComplete();
    } else {
        cerr << "setLaunchAttributes error: " << okraStatus << endl;        
    }
    return (okraStatus == OkraContext::OKRA_OK);
}

// Call clearargs between executions of a kernel before setting the new args
extern "C" DLLExport  bool okra_clearargs(void* kernel) {
    // kernel* is a Kernel
    OkraContext::Kernel* realKernel = (OkraContext::Kernel*) kernel;
    OkraContext::OkraStatus okraStatus = realKernel->clearArgs();
    return okraStatus == OkraContext::OKRA_OK ? true : false;
}

extern "C" DLLExport  void okra_register_heap(void* base, size_t size) {
    void* endAddr = (void*)((jlong)base + size);
    commitAndRegisterWholeHeap(base, endAddr);
}
