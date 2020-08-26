from django.shortcuts import render
from django.http import  HttpResponse,HttpResponseRedirect
import ocr.imghandler as imghandle


# Create your views here.

runningflag = False
def uploadtem(request):
    if request.method == 'POST':
        try:
            pic = request.POST.get('img')
            idname = request.POST.get('id')
            imghandle.b64temptofile(pic,idname)
        except NameError:
            return HttpResponse('参数传入错误')
        return HttpResponse('suc')
    else:
        return HttpResponse('erro')


def uploadpic(request):
    if request.method == 'POST':
        try:
            pic = request.POST.get('img')
            idname = request.POST.get('id')
        except NameError:
            return HttpResponse('参数传入错误')
        name = imghandle.b64tofile(pic,idname)
        return HttpResponse(name)
    if request.method == 'GET':
        return HttpResponse('helloworld')


def shibie(request):
    global runningflag
    if runningflag == False:
        if request.method == 'GET':
            if request.GET.get('id') is not None:
                idname = request.GET.get('id')
                runningflag = True
                try:
                    imghandle.initdir(idname)
                    with open('static/result/'+'20190322232755'+'.txt','r')as f:
                        res = f.read()
                    return HttpResponse(res.encode())
                except NameError:
                    return HttpResponse('id错误')
                finally:
                    runningflag = False
            else:
                    return HttpResponse('传入id')
        else:
            return HttpResponse('GET')

    else:
        return HttpResponse('项目正在运行，请稍后再试')

def uploadpage(request):
    return render(request,'upload.html')