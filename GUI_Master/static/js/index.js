connected_devices = []


function sleep(ms){
    return new Promise(resolve => setTimeout(resolve, ms));
}

function refresh_devices(){
    console.log("refresh_device");
    let host_url = window.location.host
    showLoading();
    fetch('/scan_devices')
    .then(response => response.json())
    .then(data =>{
        console.log(data.devices)
        devices = data['devices']   
        let device_list = document.getElementById("device_list");
        // remove all list elements
        device_list.innerHTML = "" 
        let device_element_template = document.getElementById("device_element_template");
        for(let i = 0; i < devices.length; i++){
            let new_device = device_element_template.content.cloneNode(true);
            let device_name = new_device.querySelector('h5');
            device_name.textContent = devices[i];
            device_list.appendChild(new_device);
        }
        setTimeout(removeLoading, 300);
    })

}