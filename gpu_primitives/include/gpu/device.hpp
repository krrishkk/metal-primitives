//
//  device.hpp
//  gpu_primitives
//
//  Created by Krrish Kishore Kumar on 12/29/25.
//

#ifndef device_h
#define device_h
#pragma once

void init_metal();
void gpu_fill(float* cpuData, size_t count, float val);
void gpu_transform(float* cpuData, size_t count, int val);
#endif /* device_h */
