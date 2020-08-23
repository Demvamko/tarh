const fbx_loader = require('three-fbx-loader');
const fs = require('fs');

let loader = new fbx_loader();

let file = fs.readFileSync('res/obj/ugh3.fbx');

let scene = loader.parse(file.buffer);


for(let object in scene.children){
    if(object.type == "SkinnedMesh"){
        ProcessMesh(object);
    }
}

function ProcessMesh(mesh){
    let vertcount = mesh.geometry.attributes.position.count;

    // VERT { float pos[3]; float uv[2]; char skin; }
    let buffer = new Uint8Array(vertcount * (4 * 5 + 1));
}