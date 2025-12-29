//
//  metal_backend.cpp
//  gpu_primitives
//
//  Created by Krrish Kishore Kumar on 12/29/25.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <stdio.h>
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

using namespace MTL;

Device* device;
CommandQueue* commandQueue;

void init_metal() {
    device = CreateSystemDefaultDevice();
    commandQueue = device->newCommandQueue();
}

ComputePipelineState* createFillPipeline(Device* device) {
    NS::Error* err = nullptr;
    
    MTL::Library* library = device->newDefaultLibrary();
    
    MTL::Function* function = library->newFunction(NS::String::string("fill_kernel", NS::UTF8StringEncoding));
    
    MTL::ComputePipelineState* pipeline =
    device->newComputePipelineState(function, &err);
    
    function->release();
    library->release();
    
    return pipeline;
}

ComputePipelineState* createTransformPipeline(Device* device) {
    NS::Error* err = nullptr;
    
    Library* library = device->newDefaultLibrary();
    Function* function = library->newFunction(NS::String::string("transform_kernel", NS::UTF8StringEncoding));
    ComputePipelineState* pipeline = device->newComputePipelineState(function, &err);
    
    function->release();
    library->release();
    
    return pipeline;
}

void gpu_fill(float* cpuData, size_t count, float val) {
    
    Buffer* buffer = device->newBuffer(
//       cpuData,
       count * sizeof(float),
       ResourceStorageModeShared
    );
    
    //to-do list for gpu
    CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    
    //recording gpu compute cmds
    ComputeCommandEncoder* encoder = commandBuffer->computeCommandEncoder();
    
    //load pipeline
    ComputePipelineState* pipeline = createFillPipeline(device);
    
    //telling encoder which kernel to run
    encoder->setComputePipelineState(pipeline);
    
    //binding gpu memory to buffer0, which is what we had in fill.metal
    encoder->setBuffer(buffer, 0, 0);
    
    //set val to buffer(1)
    encoder->setBytes(&val, sizeof(float), 1);
    
    //how many gpu threads to launch
    MTL::Size gridSize = MTL::Size(count, 1, 1);
    
    //threads per group -> cuda block
    MTL::Size threadgroupSize = MTL::Size(256, 1, 1);
    
    encoder->dispatchThreads(gridSize, threadgroupSize);
    
    encoder->endEncoding();
    
    commandBuffer->commit();
    
    commandBuffer->waitUntilCompleted();
    
    memcpy(cpuData, buffer->contents(), count*sizeof(float));
    
    pipeline->release();
    buffer->release();
}

void gpu_transform(float* cpuData, size_t count, int val) {
    Buffer* buffer = device->newBuffer(
        count*sizeof(uint),
        ResourceStorageModeShared
    );
    
    CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    ComputeCommandEncoder* commandEncoder = commandBuffer->computeCommandEncoder();

    ComputePipelineState* pipeline = createTransformPipeline(device);
    
    commandEncoder->setComputePipelineState(pipeline);
    commandEncoder->setBuffer(buffer, 0, 0);
    commandEncoder->setBytes(&val, sizeof(uint), 1);
    MTL::Size gridSize = MTL::Size(count, 1, 1);
    MTL::Size threadgroupSize = MTL::Size(256, 1, 1);
    
    commandEncoder->dispatchThreads(gridSize, threadgroupSize);
    
    commandEncoder->endEncoding();
    
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();
    
    memcpy(cpuData, buffer->contents(), count*sizeof(uint));
    
    pipeline->release();
    buffer->release();
}
