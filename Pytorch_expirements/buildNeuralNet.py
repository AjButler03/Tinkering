import os
import torch
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets, transforms

# Grab accelerator, if we have one
device = torch.accelerator.current_accelerator().type if torch.accelerator.is_available() else "cpu"
print(f"Using {device} device")

class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 10),
        )

    def forward(self, x):
        x = self.flatten(x)
        logits = self.linear_relu_stack(x)
        return logits

# Create instance of NeuralNetwork
model = NeuralNetwork().to(device)
print(model)

# passing data to the network
X = torch.rand(1, 28, 28, device=device)
logits = model(X)
pred_probab = nn.Softmax(dim=1)(logits)
y_pred = pred_probab.argmax(1)
print(f"Predicted class: {y_pred}")

# Taking 3 random 28x28 images
input_image = torch.rand(3,28,28)
print(input_image.size())
# Flatten images, creating single size 784 array
flatten = nn.Flatten()
flat_image = flatten(input_image)
print(flat_image.size())

# I think this compresses the 28*28 pieces of information into just 20 pieces of identifiable information
# I *THINK*. How it determines those 20 pieces I haven't a clue.
# Simple linear transformation? So this is pretty basic as far as linear algebra goes
layer1 = nn.Linear(in_features=28*28, out_features=20)
hidden1 = layer1(flat_image)
print(hidden1.size())

# This is non-linear transformation; in theory, this is what enables the model to pick up on super nuanced stuff
print(f"Before ReLU: {hidden1}\n\n")
hidden1 = nn.ReLU()(hidden1)
print(f"After ReLU: {hidden1}")

# scales values between 0 and 1 for probability
# all options must sum to 1 (no other possibilities, i.e., 0% chance of any non-possible option)
softmax = nn.Softmax(dim=1)
pred_probab = softmax(logits)

# Iterate over parameters
print(f"Model structure: {model}\n\n")
for name, param in model.named_parameters():
    print(f"Layer: {name} | Size: {param.size()} | Values : {param[:2]} \n")