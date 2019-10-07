from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import os

def showMask(mask, w, h):
    data = np.array([mask])
    data = data.reshape((h, w))
    plt.imshow(data, 'gray')
    plt.show()


def screen_0(img_path, save_path):
    time = 12
    K = 12
    L = 12
    N = 144   #总数
    im = Image.open(img_path).convert('L')
    im2 = Image.new("1",(im.size[0]*time,im.size[1]*time))

    Mask=[144,140,132,122,107,63,54,93,106,123,133,142,
        143,137,128,104,94,41,31,65,98,116,120,139,
        135,131,114,97,61,35,24,55,80,103,113,125,
        126,117,88,83,56,29,15,51,68,90,99,111,
        109,100,81,77,48,22,8,28,47,76,85,96,
        91,44,16,12,9,3,5,21,25,33,37,73,
        59,58,30,18,10,1,2,4,11,19,34,42,
        92,64,57,52,26,6,7,14,32,46,53,74,
        101,95,70,67,38,13,20,36,50,75,82,108,
        121,110,86,78,45,17,27,39,69,79,102,119,
        134,129,112,89,49,23,43,60,71,87,115,127,
        141,138,124,118,66,40,62,72,84,105,130,136]

    showMask(Mask, 12, 12)

    for m in range(im2.size[1]):
        k = m % K
        for n in range(im2.size[0]):
            l = n % L
            pix = int(im.getpixel((m/time,n/time))/255.0*N + 0.5)  # 取上整
            if pix > Mask0[k*L+l]: 
                im2.putpixel((m,n),1)
            else:
                im2.putpixel((m,n),0)
                
    im2.save(os.path.join(save_path, 'screen_result.bmp'))


def screen_15(img_path, save_path):
    time = 12
    K = 3
    L = 51
    N = 153
    q = 4
    im = Image.open(img_path).convert('L')
    im2 = Image.new("1",(im.size[0]*time,im.size[1]*time))

    Mask=[  153,148,120,77,53,28,26,60,87,122,131,135,132,124,116,104,73,47,23,6,56,66,85,57,51,39, 
    19,8,15,2,7,17,55,79,83,99,102,109,112,117,105,74,54,14,24,64,84,121,137,142,150,
    145,139,101,69,48,11,34,68,100,128,138,143,147,141,125,97,71,43,13,30,62,90,107,110,96,91,
    76,52,27,20,5,4,21,25,37,45,82,92,94,95,98,63,41,1,38,67,89,127,134,140,149,
    136,126,88,59,31,12,46,75,114,130,146,151,152,144,136,86,61,40,18,49,70,103,119,123,115,111,
    108,93,80,65,36,3,22,50,35,9,16,32,44,81,78,58,29,10,42,72,106,113,118,129,133]

    showMask(Mask, 51, 3)

    for m in range(im2.size[1]):
        k = m % K
        t = int(L-(q*K*(m/K))%L)
        for n in range(im2.size[0]):
            l = int((n%L+t)%L)
            pix = int(im.getpixel((m/time,n/time))/255.0*N+0.5)
            if pix > Mask[k*L+l]: 
                im2.putpixel((m,n),1)
            else:
                im2.putpixel((m,n),0)
    im2.save(os.path.join(save_path, 'screen_result.bmp'))


def screen_45(img_path, save_path):
    time = 12
    K = 8
    L = 16
    N = 128
    im = Image.open(img_path).convert('L')
    im2 = Image.new("1",(im.size[0]*time,im.size[1]*time))

    Mask=[128,120,109,92,74,66,46,8,15,10,64,79,97,111,122,127,
            123,116,87,69,62,38,6,39,42,3,19,55,86,105,115,119,
            107,96,71,59,24,12,28,52,63,47,20,1,58,95,108,112,
            84,73,56,2,18,23,48,78,82,67,35,5,31,61,91,101,
            77,53,32,4,25,43,75,85,100,89,60,30,9,34,68,80,
            51,41,21,27,40,70,94,102,110,103,93,57,26,11,37,65,
            44,29,33,45,72,90,104,121,117,114,106,88,54,17,13,16,
            14,36,49,76,83,98,118,126,125,124,113,99,81,50,22,7]

    showMask(Mask, 16, 8)

    for m in range(im2.size[1]):
        k = m % K
        if m/K%2 == 0:
            t = 0
        else:
            t = K
        for n in range(im2.size[0]):
            l = (n%L+t)%L
            pix = int(im.getpixel((m/time,n/time))/255.0*N+0.5)
            if pix > Mask[k*L+l]: 
                im2.putpixel((m,n),1)
            else:
                im2.putpixel((m,n),0)
    im2.save(os.path.join(save_path, 'screen_result.bmp'))


def screen_75(img_path, save_path):
    time = 12
    K = 3
    L = 51
    N = 153
    q = 4
    im = Image.open(img_path).convert('L')
    im2 = Image.new("1",(im.size[0]*time,im.size[1]*time))

    Mask=[153,145,136,117,95,81,8,52,93,104,97,86,77,69,59,54,41,29,7,5,36,23,13,18,26,34,
            46,64,67,72,79,25,50,66,90,103,122,128,130,137,134,118,102,82,16,51,96,115,132,147,152,
            148,139,126,105,98,78,15,27,80,73,71,61,53,48,31,14,1,10,17,4,3,6,30,49,60,68,
            75,84,89,106,83,37,35,85,107,119,131,138,146,142,140,129,109,92,32,39,91,111,124,141,144,
            120,101,88,74,63,58,2,20,65,47,43,40,28,11,12,24,38,42,55,21,22,56,62,70,87,100,
            114,121,127,113,99,45,9,57,110,123,135,143,151,150,149,133,112,94,44,19,76,108,116,125,136]

    showMask(Mask, 51, 3)

    for m in range(im2.size[1]):
        k = m % K
        t = q*K*(m/K)%L
        for n in range(im2.size[0]):
            l = (n%L+t)%L
            pix = int(im.getpixel((m/time,n/time))/255.0*N+0.5)
            if pix > Mask[int(k*L+l)]: 
                im2.putpixel((m,n),1)
            else:
                im2.putpixel((m,n),0)
    im2.save(os.path.join(save_path, 'screen_result.bmp'))


screen_45('F:\\lena.bmp', 'F:\\')

print('finish')
