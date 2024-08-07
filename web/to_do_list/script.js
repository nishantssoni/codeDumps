var mText=document.getElementById('txt');

var mBtn=document.getElementById('btn');

var mTemp=document.getElementById('temp');

var mDone=document.getElementById('done');

function did(str,onCheck,onDelete){
  if(str!=""||str.length>0){
    var el=document.createElement('li');
    var mark=document.createElement('input');
    var finish=document.createElement('button');
    var lable=document.createElement('span');
    mark.type="checkbox";
    finish.innerHTML="X"
    mark.addEventListener('click',onCheck);
    finish.addEventListener('click',onDelete);
    lable.innerHTML=str;
    el.appendChild(mark);
    el.appendChild(finish);
    el.appendChild(lable);
    mTemp.appendChild(el);
    mText.value=""
    mText.focus()
    }else{
        alert('write somthing and then click add')
    }

}

function onCheck(event){
var task = event.target.parentElement;
if(task.parentElement.id==="temp"){
    mDone.appendChild(task)
}
else{
    mTemp.appendChild(task)
}
mText.focus()
}

function onDelete(event){
var task = event.target.parentElement;
task.parentNode.removeChild(task);
}


function make(a){
did(a,onCheck,onDelete)
}
mText.addEventListener('keyup',(event)=>{
    var code=event.keyCode
    if(code==13){
        make(mText.value.trim())
    }
});