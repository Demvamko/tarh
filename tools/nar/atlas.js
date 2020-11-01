const glob = require('glob');
const path = require('path');
const canvas = require('canvas');
const fs = require('fs');

function Rect(x, y, w, h){
    this.x = x || 0;
    this.y = y || 0;
    this.w = w || 0;
    this.h = h || 0;
}

Rect.prototype.bin = function(div) {
    let arr = new Uint16Array(4);

    let border = this.border || 1;

    arr[0] = (this.x + border) / div * 0xFFFF;
    arr[1] = (this.y + border) / div * 0xFFFF;
    arr[2] = (this.x + this.w - border) / div * 0xFFFF;
    arr[3] = (this.y + this.h - border) / div * 0xFFFF;
    
    return Buffer.from(arr.buffer);
}

function Pack(rects){
    let area = 0;
    let maxw = 0;

    for(let rect of rects){
        area += rect.w * rect.h;

        if(maxw < rect.w)
            maxw = rect.w;
    }

    rects.sort((a, b) => { a.h - b.h });

    let start = Math.max(Math.ceil(Math.sqrt(area / 0.95)), maxw);
    start = 1 << 32 - Math.clz32(start);

    let spaces = [ new Rect(0, 0, start, start) ];

    for(let rect of rects){
        for(let space of spaces){
            if(rect.w > space.w || rect.h > space.h)
                continue;

            rect.x = space.x;
            rect.y = space.y;

            if(rect.w == space.w && rect.h == space.h)
                spaces.splice(spaces.indexOf(space), 1);

            else if(rect.h == space.h){
                space.x += rect.w;
                space.w -= rect.w;
            }

            else if(rect.w == space.w){
                space.y += rect.h;
                space.h -= rect.h;
            }

            else{
                let nspace = new Rect(space.x + rect.w, space.y, space.w - rect.w, rect.h);
                spaces.splice(spaces.indexOf(space), 0, nspace);
                space.y += rect.h;
                space.h -= rect.h;
            }

            break;
        }
    }

    return new Rect(0, 0, start, start);
}

async function Process(cmd){
    console.log("Processing: " + cmd.name);

    if(!cmd.path)
        return console.log(`ERROR: No path given in command ${cmd}`);

    let border = Number(cmd.border) || 1;
    let rects = [];

    for(let path of glob.sync(cmd.path)){
        let image = await canvas.loadImage(path);
    
        let rect = new Rect(0, 0, image.width + border * 2, image.height + border * 2);
        rect.image = image;
        rect.path = path;
        rect.border = border;

        rects.push(rect);
    }

    let atlas = Pack(rects);

    let cvs = canvas.createCanvas(atlas.w, atlas.h);
    let ctx = cvs.getContext('2d');
    ctx.imageSmoothingEnabled = false;

    for(let rect of rects){
        ctx.drawImage(rect.image, rect.x, rect.y, rect.w, rect.h);
        ctx.drawImage(rect.image, rect.x + border, rect.y + border, rect.w - border * 2, rect.h - border * 2);
    }

    let buffer = cvs.toBuffer('image/png');
    let uvs = Buffer.concat(rects.map((rect) => rect.bin(atlas.w)));

    fs.writeFileSync(`./res/img/${cmd.name}_ATLAS.png`, buffer);

    return {
        [`${cmd.name}_ATLAS`]: buffer,
        [`${cmd.name}_ATLAS_UVS`]: uvs
    };
}

module.exports = {
    Rect, Pack, Process
}