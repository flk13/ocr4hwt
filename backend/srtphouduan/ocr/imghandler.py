import time
import base64
import os
import shutil
import subprocess

def b64tofile(bs64img,id):
    t = time.time()
    imgdata = base64.b64decode(bs64img)
    timenow = str(int(round(t * 1000)))
    filename = timenow+'.jpg'
    try:
        with open('img/'+id+'/'+filename, 'wb+') as destination:
                destination.write(imgdata)
    except IOError:
        newdir(id)
        with open('img/'+id+'/'+filename, 'wb+') as destination:
                destination.write(imgdata)

    return filename

def b64temptofile(bsimg,id):
    filname = 'template.jpg'
    imgdata = base64.b64decode(bsimg)
    with open('img/'+id+'/'+filname,'wb') as f:
        f.write(imgdata)

def newdir(id):
    path = 'img/'+str(id)
    folder = os.path.exists(path)

    if not folder:
        os.makedirs(path)
        return True
    else:
        return False


def initdir(proid):
    #delet = subprocess.Popen('./handwrite_table_recongnition/delete',shell=True)
    #delet.wait()
    shutil.rmtree('handwrite_table_recongnition/original_img')
    os.mkdir('handwrite_table_recongnition/original_img')
    for root,dirs,files in os.walk('img/'+proid):
        break
    for imgs in files:
        shutil.copy('img/'+proid+'/'+imgs,'handwrite_table_recongnition/original_img/')


def runpro():
    runing = subprocess.Popen('sh ./handwrite_table_recongnition/run.sh',shell=True)
    runing.wait()
    time.sleep(1)