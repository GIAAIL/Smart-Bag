let F = [[],[],[],[],[]]
let B = [[],[],[],[],[]]
let F_img = document.querySelectorAll('.front_board .img_container')
let B_img = document.querySelectorAll('.back_board .img_container')
F_img.forEach(function(e,i){
      if (e.children.length>0){
        e.current_device = undefined
        console.log(e.id)
        e.name = e.id
        F[e.name[1]][e.name[2]] = e
        e.onmouseover = function(){
          //this.children[0].src = this.hover
          e.querySelectorAll('path, polyline').forEach(function(e,i){
            e.style.stroke = '#ff0000'
            e.style.strokeWidth = '2px'
          })
        }
        e.onmouseout = function(){
          e.querySelectorAll('path, polyline').forEach(function(e,i){
            if(e.current_device){
              e.style.stroke = '#00ff00'
              e.style.strokeWidth = '2px'
            }
            else{
              e.style.stroke = '#000000'
              e.style.strokeWidth = '1px'
            }
          })
        }
        e.children[0].setAttribute("object-fit", "contrain")
        e.children[0].setAttribute("width", "100%")
        e.children[0].setAttribute("height", "80%")
      } 
    })
B_img.forEach(function(e,i){
      if (e.children.length>0){
        e.current_device = undefined
        console.log(e.id)
        e.name = e.id
        B[e.name[1]][e.name[2]] = e
        e.onmouseover = function(){
          //this.children[0].src = this.hover
          e.querySelectorAll('path, polyline').forEach(function(e,i){
            e.style.stroke = '#ff0000'
            e.style.strokeWidth = '2px'
          })
        }
        e.onmouseout = function(){
          e.querySelectorAll('path, polyline').forEach(function(e,i){
            if(e.current_device){
              e.style.stroke = '#00ff00'
              e.style.strokeWidth = '2px'
            }
            else{
              e.style.stroke = '#000000'
              e.style.strokeWidth = '1px'
            }
          })
        }
        e.children[0].setAttribute("object-fit", "fill")
        e.children[0].setAttribute("width", "100%")
        e.children[0].setAttribute("height", "90%")

      }
    })


