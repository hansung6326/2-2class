const http = require('http');
const fs = require('fs');
const url = require('url');

http.createServer((request, response) => {
  // get /14/ex14-02.html http/1.1 (request line)
  let pathname = url.parse(request.url);
  pathname = url.parse(request.url).pathname;
  let filename = pathname.substring(1, pathname.length);
  console.log('pathname ', pathname);
  if (pathname == '/') {
    fs.readFile('index.html', (error, data) => {
      response.writeHead(200, { 'Content-Type': 'text/html' });
      response.end(data);
    });
  } else {
    fs.readFile(filename, (error, data) => {
      let extname = filename.split('.')[1];
      if (error) {
        response.writeHead(404, { 'Content-Type': 'text/html' });
        response.end('<head><meta charset="UTF-8"></head>'
          + '<h1>요청한 파일이 없습니다.</h1>'
          + '<br> <a href="/"> 홈으로');
      } else if (extname == 'html') {
        console.log('extname ', extname);
        response.writeHead(200, { 'Content-Type': 'text/html' });
        //Content-Type 지정하지 않으면
        //Browser화면에 tag실행이 되지 않고 tag가 글자로 표시
        response.end(data);
      } else {
        // ex14-02.css, ex14-02.js, media/img1.gif, media/clock.mp3등
        // content-type 지정하지 않아도 정상 동작함
        response.writeHead(200);
        response.end(data);
      }
    });
  }
}).listen(80, () => {
  console.log('Http Server is running port 80');
});
