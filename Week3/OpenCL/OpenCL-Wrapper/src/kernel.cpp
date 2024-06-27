#include "kernel.hpp" // note: unbalanced round brackets () are not allowed and string literals can't be arbitrarily long, so periodically interrupt with )+R(
string opencl_c_container() { return R( // ########################## begin of OpenCL C code ####################################################################



kernel void add_kernel(global float* A, global float* B, global float* C) { // equivalent to "for(uint n=0u; n<N; n++) {", but executed in parallel
	const uint n = get_global_id(0);
	C[n] = A[n]+B[n];
}

kernel void mul_kernel(global float* A, global float* B, global float* C) {
	// TASK 1 CODE BEGINS HERE
	const uint n = get_global_id(0);
	C[n] = A[n]*B[n];
	// TASK 1 CODE ENDS HERE
}

kernel void matMul (__global float* A, __global float *B, __global float *C, int aCol, int cRow, int cCol) {
	// TASK 2 CODE BEGINS HERE
	// HINT : IMPLEMENT DOT PRODUCT HERE
    const uint row = get_global_id(0); // Row index
    const uint col = get_global_id(1); // Column index

    float sum = 0.0f;
    for (int k = 0; k < aCol; ++k) {
        sum += A[row * aCol + k] * B[k * cCol + col];
    }

    C[row * cCol + col] = sum;
	// TASK 2 CODE ENDS HERE
}


);} // ############################################################### end of OpenCL C code #####################################################################
