var Roles = {}
var Roles_State = "Show"
var Device_State = "Show"
var connected_devices = []
var focus_state = {
    "device": null,
    "role": null
}



function assign_role(device_name, role_name){
    //start loading animation
    showLoading();
    // fetch devices information from server
    fetch('/assign_role', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            "device_name": device_name,
            "role_name": role_name
        })
    })
    .then(response => response.json())
    .then(data =>{
        console.log(data)
        //stop loading animation
        if(data.status == "success"){
            Roles[role_name].current_device = device_name
            connected_devices[device_name]["role"] = role_name
        }
        else{
            alert("Assign role failed")
        }
        setTimeout(removeLoading, 300);
    }
    )
}

function remove_role(device_name, role_name){
    //start loading animation
    showLoading();
    // fetch devices information from server
    fetch('/remove_role', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            "device_name": device_name,
            "role_name": role_name
        })
    })
    .then(response => response.json())
    .then(data =>{
        console.log(data)
        //stop loading animation
        if(data.status == "success"){
            Roles[role_name].current_device = null
            connected_devices[device_name]["role"] = null
        }
        else{
            alert("Remove role failed")
        }
        setTimeout(removeLoading, 300);
    }
    )
}



function update_status(connected_devices, focus){
    // update device list in upper right panel
    console.log(focus)
    role_node = document.querySelector(".role-id")
    device_node = document.querySelector(".device-id")
    connet_time_node = document.querySelector(".connect-time")
    control_value_node = document.querySelector(".control-value")
    if(focus.device == null && focus.role == null){
        [role_node, device_node, connet_time_node, control_value_node].forEach(function(node){
            node.querySelector('div').classList.add("disabled")
            if(node.querySelector('button'))
                node.querySelector('button').classList.add("disabled")
            else
                node.querySelector('input').classList.add("disabled")
            node.querySelector('p').textContent = "--"
        })
    }
    else if(focus.device == null){
        role_node.querySelector('p').textContent = focus.role
        role_node.querySelector('div').classList.remove("disabled")
        role_node.querySelector('button').classList.remove("disabled")


        device_node.querySelector('p').textContent = "Not Assigned"
        device_node.querySelector('div').classList.add("disabled")
        device_node.querySelector('button').classList.add("disabled")

        [connet_time_node, control_value_node].forEach(function(node){
            node.querySelector('div').classList.add("disabled")
            if(node.querySelector('button'))
                node.querySelector('button').classList.add("disabled")
            else
                node.querySelector('input').classList.add("disabled")
            node.querySelector('p').textContent = "--"
        })
    }
    else{
        let current_role = connected_devices[focus.device]["role"]
        role_node.querySelector('p').textContent = current_role? current_role : "Not Assigned"
        role_node.querySelector('div').classList.remove("disabled")
        role_node.querySelector('button').classList.remove("disabled")

        device_node.querySelector('p').textContent = focus.device
        device_node.querySelector('div').classList.remove("disabled")
        device_node.querySelector('button').classList.remove("disabled")

        connet_time_node.querySelector('p').textContent = connected_devices[focus.device]["Connect Time"]
        connet_time_node.querySelector('div').classList.remove("disabled")
        connet_time_node.querySelector('button').classList.remove("disabled")

        control_value_node.querySelector('p').textContent = connected_devices[focus.device]["Control Value"]
        control_value_node.querySelector('input').value = parseInt(connected_devices[focus.device]["Control Value"])
        control_value_node.querySelector('div').classList.remove("disabled")
        control_value_node.querySelector('input').classList.remove("disabled")
    }
}



function update_devices(connected_devices){
    // update device list in lower right panel
    let device_list = document.getElementById("device_list");
    // remove all list elements
    device_list.innerHTML = "" 
    let device_element_template = document.getElementById("device_element_template");
    for(let name_text of Object.keys(connected_devices)){
        let new_device = device_element_template.content.cloneNode(true);
        let device_name = new_device.querySelector('h5');
        device_name.textContent = name_text;
        
        
       
    
        device_list.appendChild(new_device);

        let new_node = device_list.children[device_list.children.length - 1]

         //set callback function
        new_node.onmouseover = () => {
            let current_focus =  {device: name_text, role: connected_devices[name_text]["role"]}
            update_status(connected_devices, current_focus)
        }
        new_node.onclick = () => {
            let current_focus =  {device: name_text, role: connected_devices[name_text]["role"]}
            update_status(connected_devices, current_focus)
            focus_state = current_focus
            Array.from((device_list.children)).forEach((node)=>{node.classList.remove("active")})
            new_node.classList.add("active")
        }

        new_node.onmouseleave = () => {
            let current_focus =  {device: name_text, role: connected_devices[name_text]["role"]}
            update_status(connected_devices, focus_state)
        }
    }                
        
    
}

function update_roles(Roles, focus_state){
    
}

function refresh_devices(){
    //start loading animation
    showLoading();
    // fetch devices information from server
    fetch('/scan_devices')
    .then(response => response.json())
    .then(data =>{
        console.log(data.devices)
        connected_devices = data.devices
        for(let key of Object.keys(connected_devices)){
            device = connected_devices[key]
            console.log(key)
            if(device["role"] != null){
                Roles[device["role"]].current_device = key
            }
        }
        update_devices(connected_devices)
        update_roles(Roles, focus_state)

        //stop loading animation
        setTimeout(removeLoading, 300);
    })

}