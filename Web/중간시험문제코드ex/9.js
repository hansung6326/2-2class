// 웹 페이지로부터 두 수가 오기를 기다린다.
onmessage = function (e) {
	let op1 = parseInt(e.data.op1);
	let op2 = parseInt(e.data.op2);
	let parameters = {
		first : op1,
		second : op2
	};
	postMessage(parameters); 
}
