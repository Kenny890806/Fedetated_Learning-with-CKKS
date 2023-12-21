#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Python version: 3.6


#from importlib.resources import contents
import os
import copy
import time
import pickle
import numpy as np
from tqdm import tqdm

#串接測試import
from ctypes import cdll, c_double, c_uint, POINTER
import ctypes
from numpy.ctypeslib import ndpointer
from collections import OrderedDict
import torch
from tensorboardX import SummaryWriter


from options import args_parser
from update import LocalUpdate, test_inference
from models import MLP, CNNMnist, CNNFashion_Mnist, CNNCifar
from utils import get_dataset, average_weights, exp_details


if __name__ == '__main__':
    start_time = time.time()

    # define paths
    path_project = os.path.abspath('..')
    logger = SummaryWriter('../logs')

    args = args_parser()
    exp_details(args)

    if args.gpu:
        torch.cuda.set_device(args.gpu)
    device = 'cuda' if args.gpu else 'cpu'

    # load dataset and user groups
    train_dataset, test_dataset, user_groups = get_dataset(args)
    print(train_dataset[0][0].shape)
    #input()
    # BUILD MODEL
    if args.model == 'cnn':
        # Convolutional neural netork
        if args.dataset == 'mnist':
            global_model = CNNMnist(args=args)
        elif args.dataset == 'fmnist':
            global_model = CNNFashion_Mnist(args=args)
        elif args.dataset == 'cifar':
            global_model = CNNCifar(args=args)

    elif args.model == 'mlp':
        # Multi-layer preceptron
        img_size = train_dataset[0][0].shape
        len_in = 1
        for x in img_size:
            len_in *= x
            global_model = MLP(dim_in=len_in, dim_hidden=64,
                               dim_out=args.num_classes)
    else:
        exit('Error: unrecognized model')

    # Set the model to train and send it to device.
    global_model.to(device)
    global_model.train()
    print(global_model)

    # copy weights
    global_weights = global_model.state_dict()
    #print(global_weights)

    # Training
    train_loss, train_accuracy = [], []
    val_acc_list, net_list = [], []
    cv_loss, cv_acc = [], []
    print_every = 2
    val_loss_pre, counter = 0, 0


    
    #開始串接
    mydll = cdll.LoadLibrary('./libtemp_shared.so')
    #mydll.encrypt()
    mydll.keygenSer()

    for epoch in tqdm(range(args.epochs)):
        local_weights, local_losses = [], []
        print(f'\n | Global Training Round : {epoch+1} |\n')

        global_model.train()
        m = max(int(args.frac * args.num_users), 1)
        idxs_users = np.random.choice(range(args.num_users), m, replace=False)
        client = 0
        for idx in idxs_users: #10
            local_model = LocalUpdate(args=args, dataset=train_dataset, #local train
                                      idxs=user_groups[idx], logger=logger)
            w, loss = local_model.update_weights(
                model=copy.deepcopy(global_model), global_round=epoch)
            #-------------------------------------------------------------------------------------------#
            #conv1.weight-250 torch.Size([10, 1, 5, 5])
            #、conv1.bias-10、torch.Size([10])
            #conv2.weight-5000、torch.Size([20, 10, 5, 5])
            #conv2.bias-20、torch.Size([20])
            #fc1.weight=16000、torch.Size([50, 320])
            #fc1.bias-50、torch.Size([50])
            #fc2.weight-500、torch.Size([10, 50])
            #fc2.bias-10、torch.Size([10])

            temparr1 = w.setdefault('conv1.weight')
            temparr1 = temparr1.view(-1)
            temparr1 = temparr1.numpy()
            temparr1 = np.array(temparr1)
            #print(len(temparr1))

            temparr2 = w.setdefault('conv1.bias')
            temparr2 = temparr2.view(-1)
            temparr2 = temparr2.numpy()
            temparr2 = np.array(temparr2)
            #print(len(temparr2))
            totalarr = np.append(temparr1,temparr2)

            temparr3 = w.setdefault('conv2.weight')
            temparr3 = temparr3.view(-1)
            temparr3 = temparr3.numpy()
            temparr3 = np.array(temparr3)
            totalarr = np.append(totalarr,temparr3)

            temparr4 = w.setdefault('conv2.bias')
            temparr4 = temparr4.view(-1)
            temparr4 = temparr4.numpy()
            temparr4 = np.array(temparr4)
            totalarr = np.append(totalarr,temparr4)

            temparr5 = w.setdefault('fc1.weight')
            temparr5 = temparr5.view(-1)
            temparr5 = temparr5.numpy()
            temparr5 = np.array(temparr5)
            totalarr = np.append(totalarr,temparr5)

            temparr6 = w.setdefault('fc1.bias')
            temparr6 = temparr6.view(-1)
            temparr6 = temparr6.numpy()
            temparr6 = np.array(temparr6)
            totalarr = np.append(totalarr,temparr6)

            temparr7 = w.setdefault('fc2.weight')
            temparr7 = temparr7.view(-1)
            temparr7 = temparr7.numpy()
            temparr7 = np.array(temparr7)
            totalarr = np.append(totalarr,temparr7)
            
            temparr8 = w.setdefault('fc2.bias')
            temparr8 = temparr8.view(-1)
            temparr8 = temparr8.numpy()
            temparr8 = np.array(temparr8)
            totalarr = np.append(totalarr,temparr8)
            
            
            print(len(totalarr))
            print(client)
            #input()
            #totalarr = temparr1
            #Encrypt
            mydll.uniqueEnc.argtypes = [POINTER(c_double), c_uint, c_uint]
            #mydll.encrypt.argtypes = [c_uint, c_uint]
            mydll.uniqueEnc.restype = None
            #print(inputdata[0])
            totalarr=totalarr.astype(np.double)
            #print(type(inputdata[0]))
            #print(inputdata[0])
            #mydll.encrypt(inputdata.size,inputdata.size)
            dataPtr = totalarr.ctypes.data_as(POINTER(c_double))
            #print(dataPtr)
            #print(dataPtr[0])
            #temp_encrypt_val = mydll.encrypt(dataPtr,inputdata.size)
            mydll.uniqueEnc(dataPtr,totalarr.size,client)
            #-------------------------------------------------------------------------------------------#
            #print(w)
            #print(w[0])
            #加密w
            local_weights.append(copy.deepcopy(w)) #local_weight[0]~[10]
            local_losses.append(copy.deepcopy(loss))
            #input()
            client = client+1
            #print(local_weights.shape)
            #print(local_weights)
            #print(w[0])
        #同態加法
        mydll.ciphertextAdd.restype = None
        mydll.ciphertextAdd()
        #轉加密
        mydll.evalKeygen.restype = None
        mydll.evalKeygen()

        
        totalclient = 10
        #mydll.redeEnc.restype = ctypes.POINTER(c_double * 21840)
        mydll.redeEnc.restype = ndpointer(dtype=c_double, shape=(21840,))
        totalarray = mydll.redeEnc()

        for i in range(0,len(totalarray)):
            totalarray[i] = totalarray[i]/totalclient

        oriarr = totalarray.copy()

        #---------------------------------------------------------------------------------------------------------------------------#
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

        global_weights = OrderedDict([
            ('conv1.weight', tensorarr1),
            ('conv1.bias', tensorarr2),
            ('conv2.weight', tensorarr3),
            ('conv2.bias', tensorarr4),
            ('fc1.weight', tensorarr5),
            ('fc1.bias', tensorarr6),
            ('fc2.weight', tensorarr7),
            ('fc2.bias', tensorarr8),
        ])
        #print(example_ordered_dict_1)
        #---------------------------------------------------------------------------------------------------------------------------#
        #input()
        #------------------------test------------------------#
       
        #total_HE_weights = local_weight[0]+...+[10] #同態加法
        #total_HE_weights轉加密
        #total_HE_weights -> client
        #each get total_HE_weights
        #total_HE_weights decrypt
        #total_HE_weight/len(idxs_users) #len(idxs_users)=10
        #client weight update
        #print(local_weights)

        #print(np.shape(local_weights)) -> (10,)
        #print(local_weights[0]) ->第一個client端訓練完的所有資料(cov1.weights、conv1.bias、....、fc2.bias) OrderDict型態
        #print(local_weights[1]) ->第二個client端訓練完的所有資料
        


        # update global weights

        #global_weights = average_weights(local_weights)


        #for key, value in global_weights.items():
               #print(key(0),value)
        #val = global_weights.setdefault('conv1.weight')
        

        #input()
        #print(val)
        

        
        
        #arr1 = torch.Tensor([v.numpy() for k, v in global_weights.items() if k == 'conv1.weight']).squeeze().numpy()
        #arr2 = torch.Tensor([v.numpy() for k, v in global_weights.items() if k == 'conv1.bias']).squeeze().numpy()
        #arr3 = torch.Tensor([v.numpy() for k, v in global_weights.items() if k == 'conv2.weight']).squeeze().numpy()
        #arr4 = torch.Tensor([v.numpy() for k, v in global_weights.items() if k == 'conv2.bias']).squeeze().numpy()
        #cpp_arr1 = arr1.flatten().tolist()

        #cpp_arr2 = arr2.flatten().tolist()

        #print("weight1:", cpp_arr1)
        #print("bias1:", cpp_arr2)
        #print(cpp_arr[0])
        #print(len(cpp_arr))
        # update global weights
        global_model.load_state_dict(global_weights)

        loss_avg = sum(local_losses) / len(local_losses)
        train_loss.append(loss_avg)

        # Calculate avg training accuracy over all users at every epoch
        list_acc, list_loss = [], []
        global_model.eval()
        for c in range(args.num_users):
            local_model = LocalUpdate(args=args, dataset=train_dataset,
                                      idxs=user_groups[idx], logger=logger)
            acc, loss = local_model.inference(model=global_model)
            list_acc.append(acc)
            list_loss.append(loss)
        train_accuracy.append(sum(list_acc)/len(list_acc))

        # print global training loss after every 'i' rounds
        if (epoch+1) % print_every == 0:
            print(f' \nAvg Training Stats after {epoch+1} global rounds:')
            print(f'Training Loss : {np.mean(np.array(train_loss))}')
            print('Train Accuracy: {:.2f}% \n'.format(100*train_accuracy[-1]))

    # Test inference after completion of training
    test_acc, test_loss = test_inference(args, global_model, test_dataset)

    print(f' \n Results after {args.epochs} global rounds of training:')
    print("|---- Avg Train Accuracy: {:.2f}%".format(100*train_accuracy[-1]))
    print("|---- Test Accuracy: {:.2f}%".format(100*test_acc))

    # Saving the objects train_loss and train_accuracy:
    file_name = '/home/z31034602/BigP/TEST/Federated-Learning-PyTorch-master/save/objects/{}_{}_{}_C[{}]_iid[{}]_E[{}]_B[{}].pkl'.\
        format(args.dataset, args.model, args.epochs, args.frac, args.iid,
               args.local_ep, args.local_bs)

    with open(file_name, 'wb') as f:
        pickle.dump([train_loss, train_accuracy], f)

    print('\n Total Run Time: {0:0.4f}'.format(time.time()-start_time))

    # PLOTTING (optional)
    # import matplotlib
    # import matplotlib.pyplot as plt
    # matplotlib.use('Agg')

    # Plot Loss curve
    # plt.figure()
    # plt.title('Training Loss vs Communication rounds')
    # plt.plot(range(len(train_loss)), train_loss, color='r')
    # plt.ylabel('Training loss')
    # plt.xlabel('Communication Rounds')
    # plt.savefig('../save/fed_{}_{}_{}_C[{}]_iid[{}]_E[{}]_B[{}]_loss.png'.
    #             format(args.dataset, args.model, args.epochs, args.frac,
    #                    args.iid, args.local_ep, args.local_bs))
    #
    # # Plot Average Accuracy vs Communication rounds
    # plt.figure()
    # plt.title('Average Accuracy vs Communication rounds')
    # plt.plot(range(len(train_accuracy)), train_accuracy, color='k')
    # plt.ylabel('Average Accuracy')
    # plt.xlabel('Communication Rounds')
    # plt.savefig('../save/fed_{}_{}_{}_C[{}]_iid[{}]_E[{}]_B[{}]_acc.png'.
    #             format(args.dataset, args.model, args.epochs, args.frac,
    #                    args.iid, args.local_ep, args.local_bs))

