var express = require('express');
var app = express();
var fs = require('fs');
var path = require('path');
var http = require('http');
var pug = require('pug');
var morgan = require('morgan');
const multer = require('multer');


app.use(multer({dest: '/tmp/dist'}).array('file'));
app.use(morgan('short'));
app.use("/uploads",express.static(path.join(__dirname, '/uploads')))
app.use("/template",express.static(path.join(__dirname, '/template')))


app.get('/', function(req, res) {
    var action = req.query.action?req.query.action:"index";
    if( action.includes("/") || action.includes("\\") ){
        res.send("Errrrr, You have been Blocked");
    }
    file = path.join(__dirname + '/template/'+ action +'.pug');
    var html = pug.renderFile(file);
    res.send(html);
});

app.post('/file_upload', function(req, res){
    var ip = req.connection.remoteAddress;
    var obj = {
        msg: '',
    }
    if (!ip.includes('127.0.0.1')) {
        obj.msg="only admin's ip can use it"
        res.send(JSON.stringify(obj));
        return 
    }
    fs.readFile(req.files[0].path, function(err, data){
        if(err){
            obj.msg = 'upload failed';
            res.send(JSON.stringify(obj));
        }else{
            var file_path = '/uploads/' + req.files[0].mimetype +"/";
            var file_name = req.files[0].originalname
            var dir_file = __dirname + file_path + file_name
            if(!fs.existsSync(__dirname + file_path)){
                try {
                    fs.mkdirSync(__dirname + file_path)
                } catch (error) {
                    obj.msg = "file type error";
                    res.send(JSON.stringify(obj));
                    return
                }
            }
            try {
                fs.writeFileSync(dir_file,data)
                obj = {
                    msg: 'upload success',
                    filename: file_path + file_name
                } 
            } catch (error) {
                obj.msg = 'upload failed';
            }
            res.send(JSON.stringify(obj));    
        }
    })
})

app.get('/source', function(req, res) {
    res.sendFile(path.join(__dirname + '/template/source.txt'));
});


app.get('/core', function(req, res) {
    var q = req.query.q;
    var resp = "";
    if (q) {
        var url = 'http://localhost:8081/source?q=' + q
        console.log(url)
        var trigger = blacklist(url);
        if (trigger === true) {
            res.send("<p>error occurs!</p>");
        } else {
            try {
                http.get(url, function(resp) {
                    resp.setEncoding('utf8');
                    resp.on('error', function(err) {
                    if (err.code === "ECONNRESET") {
                     console.log("Timeout occurs");
                     return;
                    }
                   });

                    resp.on('data', function(chunk) {
                        try {
                         resps = chunk.toString();
                         res.send(resps);
                        }catch (e) {
                           res.send(e.message);
                        }
 
                    }).on('error', (e) => {
                         res.send(e.message);});
                });
            } catch (error) {
                console.log(error);
            }
        }
    } else {
        res.send("search param 'q' missing!");
    }
})

function blacklist(url) {
    var evilwords = ["global", "process","mainModule","require","root","child_process","exec","\"","'","!"];
    var arrayLen = evilwords.length;
    for (var i = 0; i < arrayLen; i++) {
        const trigger = url.includes(evilwords[i]);
        if (trigger === true) {
            return true
        }
    }
}

var server = app.listen(8081, function() {
    var host = server.address().address
    var port = server.address().port
    console.log("Example app listening at http://%s:%s", host, port)
})
