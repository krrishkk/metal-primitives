//
//  fill.metal
//  gpu_primitives
//
//  Created by Krrish Kishore Kumar on 12/29/25.
//

#include <metal_stdlib>
using namespace metal;

kernel void fill_kernel(
    device float* data [[buffer(0)]],
    constant float& value [[buffer(1)]],
    uint id [[thread_position_in_grid]]
) {
    data[id] = value;
}
