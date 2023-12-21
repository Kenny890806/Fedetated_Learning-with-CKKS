
import os
import copy
import time
import pickle
import numpy as np

from tqdm import tqdm
import torch
from tensorboardX import SummaryWriter
from collections import OrderedDict



            #conv1.weight-250 torch.Size([10, 1, 5, 5])
            #、conv1.bias-10、torch.Size([10])
            #conv2.weight-5000、torch.Size([20, 10, 5, 5])
            #conv2.bias-20、torch.Size([20])
            #fc1.weight=16000、torch.Size([50, 320])
            #fc1.bias-50、torch.Size([50])
            #fc2.weight-500、torch.Size([10, 50])
            #fc2.bias-10、torch.Size([10])

oriarr = []
for i in range(0, 21840):
    oriarr.append(i)
#print(len(oriarr))
#print(oriarr[0])

#print(oriarr[0:3])
temparr1 = oriarr[0:250].copy() #250
#print(temparr1)
temparr2 = oriarr[250:260].copy() #+10
#print(temparr2)
temparr3 = oriarr[260:5260].copy() #+5000
#print(temparr3)
temparr4 = oriarr[5260:5280].copy() #+20
#print(temparr4)
temparr5 = oriarr[5280:21280].copy() #+16000
#print(temparr5)
temparr6 = oriarr[21280:21330].copy() #+50
#print(temparr6)
temparr7 = oriarr[21330:21830].copy() #+500
#print(temparr7)
temparr8 = oriarr[21830:21840].copy() #+10
#print(temparr8)

tempnparr1 = np.array(temparr1)
#print(tempnparr1)
tensorarr1 = torch.tensor(tempnparr1)
tensorarr1 = tensorarr1.reshape([10, 1, 5, 5])
#print(tensorarr1)

tempnparr2 = np.array(temparr2)
tensorarr2 = torch.tensor(tempnparr2)

tempnparr3 = np.array(temparr3)
tensorarr3 = torch.tensor(tempnparr3)
tensorarr3 = tensorarr3.reshape([20, 10, 5, 5])

tempnparr4 = np.array(temparr4)
tensorarr4 = torch.tensor(tempnparr4)

tempnparr5 = np.array(temparr5)
tensorarr5 = torch.tensor(tempnparr5)
tensorarr5 = tensorarr5.reshape([50, 320])

tempnparr6 = np.array(temparr6)
tensorarr6 = torch.tensor(tempnparr6)

tempnparr7 = np.array(temparr7)
tensorarr7 = torch.tensor(tempnparr7)
tensorarr7 = tensorarr7.reshape([10, 50])

tempnparr8 = np.array(temparr8)
tensorarr8 = torch.tensor(tempnparr8)

example_ordered_dict_1 = OrderedDict([
    ('conv1.weight', tensorarr1),
    ('conv1.bias', tensorarr2),
    ('conv2.weight', tensorarr3),
    ('conv2.bias', tensorarr4),
    ('fc1.weight', tensorarr5),
    ('fc1.bias', tensorarr6),
    ('fc2.weight', tensorarr7),
    ('fc2.bias', tensorarr8),
])

print(example_ordered_dict_1)

