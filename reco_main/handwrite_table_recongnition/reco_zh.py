import os
import codecs
import random
import copy
import argparse
import numpy as np
import tensorflow as tf
from scipy.misc import imread
from keras.utils.np_utils import to_categorical
from keras import backend
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


predFile=listdir("savedir_zh0")
predFile.sort()


m=len(predFile)
pred_data=np.ndarray([m,IMG_SIZE,IMG_SIZE],dtype=np.uint)
for i in range(m):
    pred_data[i]=imread("./savedir_zh0/"+predFile[i])



pred_data=pred_data.reshape(m,IMG_SIZE,IMG_SIZE,1)
pred_data=pred_data.astype(np.float32)
pred_data/=255.0


preds=model.predict(pred_data)
pred=''
for i in range(m):
    j=np.argmax(preds[i])
    pred+=klasses[j]


f=open("out2.txt")
out=f.read()

f.close()
out=out.split(' ')
out=out[:-1]
index=0
f=open("title.txt",'w')
import xlwt
# workbook=xlwt.Workbook(encoding='utf-8')
# sheet1=workbook.add_sheet("测试表格")
for i in range(len(out)):
    tmp=pred[index:index+int(out[int(i)])]
    index+=int(out[int(i)])
    f.write(tmp+' ')
f.close()

# workbook.save('./test.xlsx')