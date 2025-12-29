//
//  main.cpp
//  gpu_primitives
//
//  Created by Krrish Kishore Kumar on 12/29/25.
//

#include <iostream>
#include "device.hpp"

int main(int argc, const char * argv[]) {
    
    init_metal();
    const size_t N = 2048;
    float data[N];
    
    gpu_fill(data, N, 2);
    gpu_transform(data, N, 30);
    
    for (int i = 0; i < 10; i++) {
        printf("%f\n", data[i]);
    }
    return 0;
}
