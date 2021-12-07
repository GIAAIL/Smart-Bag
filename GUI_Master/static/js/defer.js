
// collect svg tags inside each img_container
let F_img = document.querySelectorAll('.front_board .img_container')
let B_img = document.querySelectorAll('.back_board .img_container')
F_img.forEach(function(e,i){
      if (e.children.length>0){
        //console.log(e.id)
        e.name = e.id
        // store element in Role dict
        Roles[e.name]= e
      } 
    })
B_img.forEach(function(e,i){
      if (e.children.length>0){
        //console.log(e.id)
        e.name = e.id
        // store element in Role dict
        Roles[e.name] = e 
      }
    })

console.log(Roles)
console.log(Object.keys(Roles))
// attach callbacks to track mouseover and mouseout events
for(role_name of Object.keys(Roles)){
  console.log(role_name)
  let e = Roles[role_name]
  // current devices assigned to this role
  e.current_device = null
  //mouse events handlers
  e.mouseover = false
  e.onmouseover = function(){
    if (e.mouseover == false){
      e.mouseover = true
      // available selection option 
      if (e.current_device == null || Roles_State == "show"){
        let currnet_focus = {device:e.current_device, role:e.name}
        update_roles(currnet_focus)
        update_status(currnet_focus)
        update_devices(currnet_focus)
        e.style.cursor = 'pointer'
      }
      else{
        //device is not available in selection
        e.style.cursor = 'not-allowed'
      }
    }
  }
  e.onmouseout = function(){
    if (e.mouseover == true){
      e.mouseover = false
      update_roles(focus_state)
      update_status(focus_state)
      update_devices(focus_state)
      e.style.cursor = 'default'
    }
  }
  e.onclick = function(){
    if (Roles_State == "show"){
      // update focus state
      let current_focus = {device:e.current_device, role:e.name}
      focus_state = current_focus
      update_roles(current_focus)
      update_status(current_focus)
      update_devices(current_focus)
    
    }
    else{
      if(e.current_device == null){
        // assign role to device
        showLoading()
        assign_role(focus_state.device, e.name)
        update_roles(current_focus)
        update_status(current_focus)
        update_devices(current_focus)
        setTimeout(removeLoading, 300)
      }
    }
  }
  // adjust div svg css
  e.children[0].setAttribute("object-fit", "contain")
  e.children[0].setAttribute("width", "95%")
  e.children[0].setAttribute("height", "95%")
}


update_status(focus_state)
update_devices(focus_state)
setTimeout(()=>update_roles(focus_state), 200)