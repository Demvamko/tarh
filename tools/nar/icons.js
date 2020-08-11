let fs = require('fs');
let canvas = require('canvas');

let processors = {
    voxel: Voxel
}

function Process(cmd){
    console.log(cmd);
    return processors[cmd.process](cmd);
}

async function Voxel(cmd){
    console.log(cmd);
    cmd.size = Number(cmd.size);

    let transforms = [
        [1, -0.5, 1, 0.5, 0, 0.5],
        [1,  0.5, 0,   1, 0, 0.5],
        [1, -0.5, 0,   1, 1,   1]
    ];

    let cvs = canvas.createCanvas(cmd.size, cmd.size);
    let ctx = cvs.getContext('2d');

    let image = await canvas.loadImage(cmd.path);

    let upscale = canvas.createCanvas(cmd.size, cmd.size);
    let upscale_ctx = upscale.getContext('2d');
    upscale_ctx.imageSmoothingEnabled = false;
    upscale_ctx.drawImage(image, 0, 0, cmd.size, cmd.size);

    image = upscale;

    for(let transform of transforms){
        transform[4] *= cmd.size / 2;
        transform[5] *= cmd.size / 2;

        ctx.setTransform(...transform);
        ctx.drawImage(image, 0, 0, cmd.size / 2, cmd.size / 2);
    }

    ctx.setTransform(1, 0, 0, 1, 0, 0);

    fs.writeFileSync('./test.png', cvs.toBuffer('image/png'));
}

module.exports = { Process, Voxel };