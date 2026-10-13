
onmessage = function (e){
    if(100 >= e.data && e.data >= 90)
        this.postMessage("A");
    else if(90 > e.data && e.data >= 80)
        this.postMessage("B");
    else if(80 > e.data && e.data >= 70)
        this.postMessage("C");
    else if(70 > e.data && e.data >= 60)
        this.postMessage("D");
    else if(60 > e.data && e.data >= 0)
        this.postMessage("F");
    else
        this.postMessage("잘못된 점수입니다.");
}