import os
import codecs
import random
import keras
import numpy as np
import tensorflow as tf
from scipy.misc import imread
from the_model import model_8
from os import listdir
import cv2
random.seed(888)
np.random.seed(888)
tf.set_random_seed(888)
IMG_SIZE = 96

WEIGHTS_PATH = os.path.join("zh_model", "weights08.h5")
LABELS_PATH = os.path.join("zh_model", "labels.txt")

label_file = codecs.open(LABELS_PATH, "r", "UTF-8")
klasses = [a.strip() for a in label_file.readlines()]
label_file.close()

model=model_8(IMG_SIZE,3755)
model.load_weights(WEIGHTS_PATH)

model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])


predFile=listdir("savedir_zh")
predFile.sort()


m=len(predFile)

pred_data=np.ndarray([m,IMG_SIZE,IMG_SIZE],dtype=np.uint)
for i in range(m):
    pred_data[i]=imread("./savedir_zh/"+predFile[i])



pred_data=pred_data.reshape(m,IMG_SIZE,IMG_SIZE,1)
pred_data=pred_data.astype(np.float32)
pred_data/=255.0


preds=model.predict(pred_data)
pred=''
for i in range(m):
    j=np.argmax(preds[i])
    pred+=klasses[j]

#******************************************************************************
list = os.listdir('./savedir_digit/')
list.sort()
image = np.zeros([len(list), 28,28,1])
for i in range(len(list)):
    img = cv2.imread('./savedir_digit/' + list[i], 0)
    img = img.reshape(28, 28, 1) / 255.0
    image[i] = img


model = keras.models.load_model('./digit_model/mnist_model.h5')

pre=model.predict(image,batch_size=1)
pred2=np.argmax(pre,1)
# f=open('row_col.txt')
# row_col=f.read()
# f.close()
# f=open('out.txt')
# nums=f.read()
# f.close()
# row_col=row_col.split(' ')
#
#
import xlwt
# workbook=xlwt.Workbook(encoding='utf-8')
# sheet1=workbook.add_sheet("测试表格")
# for k in range(int(row_col[0])+1):
#     for i in range(int(row_col[1])):
#         a = int(nums[k*int(row_col[1])+i])
#         tmp = pred[0:a]
#         pred = pred[a:]
#         sheet1.write(k, i, tmp)
#
#
#
# workbook.save('./test.xlsx')

workbook=xlwt.Workbook(encoding='utf-8')
sheet1=workbook.add_sheet("测试表格")

f=open("title.txt")
title=f.read()
f.close()
title=title.split(' ')
title=title[0:-1]
for index,value in enumerate(title):
    sheet1.write(0,index,value)

f=open("img_nums.txt")
img_nums=f.read()
f.close()
zh_index = 0
digit_index = 0


start_index=0

for img_num in range(int(img_nums)):

    name="zh_nu_%c.txt"%chr(ord('a')+img_num)

    f = open(name)
    zh_nu = f.read()
    f.close()
    zh_nu = zh_nu.split(' ')

    name="digit_num_%c.txt"%chr(ord('a')+img_num)

    f = open(name)
    digit_num = f.read()
    f.close()
    digit_num = digit_num.split(' ')

    name = "num_real_%c.txt" % chr(ord('a') + img_num)
    f = open(name)
    num_real = f.read()
    f.close()
    num_real = num_real.split(' ')
    num_real = num_real[0:-1]

    type = num_real[1::2]
    num = num_real[::2]
    t=0
    zh_ii = 0
    digit_ii = 0

    for index, n in enumerate(num):
        if type[index] == '0':
            for j in range(int(n)):
                tmp = pred[zh_index:zh_index + int(zh_nu[zh_ii])]
                zh_index = zh_index + int(zh_nu[zh_ii])
                zh_ii += 1
                sheet1.write(start_index+j + 1, index, tmp)

        else:
            for j in range(int(n)):
                tmp = str(pred2[digit_index:digit_index + int(digit_num[digit_ii])])
                digit_index = digit_index + int(digit_num[digit_ii])
                digit_ii += 1
                tmp = tmp.replace(' ', '')
                sheet1.write(start_index+j + 1, index, tmp[1:-1])
        if t < int(n):
            t = int(n)

    start_index+=t


workbook.save('../test.xlsx')



