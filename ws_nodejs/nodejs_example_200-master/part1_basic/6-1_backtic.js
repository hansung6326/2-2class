const greeting1 = 'hello';
const greeting2 = 'bye';
const name1 = 'kyeongok';
const name2 = 'jihyun';

const statement = `${greeting1}! my name is ${name2}`;
const statement2 = `${greeting2}! my name is ${name2}`;

console.log('%s! my name is %s',greeting1,name1);//홑따움표 사용시
console.log(`${greeting1}! my name is ${name1}`);//백틱 사용시
console.log('');
console.log('%s! my name is %s',greeting2,name1);//홑따움표 사용시
console.log(`${greeting2}! my name is ${name1}`);//백틱 사용시
console.log('');
console.log('statement:', statement);
console.log('statement2:', statement2);
