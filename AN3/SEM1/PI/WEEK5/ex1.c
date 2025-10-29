#ex1
import hashlib
import os
import requests
import re
import collections
import random
import torch
from torch import nn
from torch.nn import functional as F
import math
import matplotlib.pyplot as plt
import zipfile

torch.manual_seed(42);

X_in = torch.rand((5,1,2))
X_h = torch.rand((1,1,4))

in_size = 2
in_hiden = 4

lay = nn.RNN(in_size, in_hiden)
lay(X_in,X_h)