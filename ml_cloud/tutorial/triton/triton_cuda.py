import sys
sys.path.append("/home/lingkun/Develop/courage/ml_cloud/triton/python")

import triton
import triton.language as tl
import torch


@triton.jit
def _add(z_ptr, x_ptr, y_ptr, N):
    # same as torch.arange
    offsets = tl.arrange(0, 1024)

    # create 1024 pointers to X, Y, Z
    x_ptrs = x_ptr + offsets
    y_ptrs = y_ptr + offsets
    z_ptrs = z_ptr + offsets

    # load 1024 elements of X, Y, Z
    x = tl.load(x_ptrs)
    y = tl.load(y_ptrs)

    # do computations
    z = x + y

    # write-back 1024 elements of X, Y, Z
    tl.store(z_ptrs, z)

N = 1024
x = torch.randn(N, device='cpu')
y = torch.randn(N, device='cpu')
z = torch.randn(N, device='cpu')
grid=(1,)
_add[grid](z, x, y, N)