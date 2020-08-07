const glob = require('glob');
const fs = require('fs').promises;

async function Process(cmd){
    if(!cmd.path.includes('*'))
        return { [cmd.name]: await fs.readFile(path) }

    let loaded = [];
    let pack_size = 0;

    for(let path of glob.sync(cmd.path)){
        let data = await fs.readFile(path);
        loaded.push(data);
        pack_size += data.length + 4;
    }

    let packed = new Uint8Array(pack_size + 4);
    let len = 4;

    let count = Buffer.from(new Uint32Array([ loaded.length ]).buffer);
    packed.set(count, 0);

    for(let data of loaded){
        let offset = Buffer.from(new Uint32Array([ data.length ]).buffer);
        packed.set(offset, len);
        len += 4;
        packed.set(data, len);
        len += data.length;
    }

    return { [cmd.name]: Buffer.from(packed.buffer) };
}

module.exports = { Process };