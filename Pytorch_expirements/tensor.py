import torch
import numpy as np
# This is to play around with pytorch to attempt to learn how to use it.

# This creates a basic tensor, and moves that tensor data to an accelerator (in my case, cuda)

data = [[1,2],[3,4]]
tensor_data = torch.tensor(data)
print(f"Device tensor is stored on: {tensor_data.device} (should be CPU)")

cuda_available = torch.cuda.is_available()
print(f"Is CUDA available? {cuda_available}")

if torch.accelerator.is_available():
    tensor_data = tensor_data.to(torch.accelerator.current_accelerator())
    
print(f"Device tensor is stored on: {tensor_data.device} (should be Cuda)")
