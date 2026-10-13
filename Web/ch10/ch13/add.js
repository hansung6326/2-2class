
onmessage = function (e){
    let sum = 0;
    let form = parseInt(e.data.form);
    let to = parseInt(e.data.to);
    for(let i=form; i<=to; i++){
        sum += i;
    }
    this.postMessage(sum);
}