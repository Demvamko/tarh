const fs = require('fs');
const path = require('path');
const readline = require('readline');
const process = require('process');

const util = require('./util.js');
const atlas = require('./atlas.js');
const font = {};
const shader = require('./shader.js');

process.on('unhandledRejection', up => { throw up });

commands = [];

fs.WriteStream.prototype.async_write = function (data) {
    return new Promise((res, rej) => {
        this.write(data, (err) => {
            err ? rej(err) : res();
        })
    });
}

const processors = {
    'atlas': atlas,
    'image': atlas,
    'font': font,
    'shader': shader
};

function Command(line){
    let self = this;
    let regex = /--\S+=\S+/g;

    self.line = line;

    let args = line.match(regex)
    for(let arg of args){
        let split = arg.split('=');
        self[split[0].slice(2)] = split[1];
    }
}

(async () => {
    await util.tree('./inc', async (file) => {
        reader = readline.createInterface({ input: fs.createReadStream(file) });
        for await (const line of reader)
            if(line.includes('//pack'))
                commands.push(new Command(line))
    });

    let outputs = [];

    for(let cmd of commands){
        let out = await processors[cmd.type].Process(cmd);
        outputs.push(out);
    }

    console.log(outputs);

    let bin = fs.createWriteStream('./res/bin/pack_nar.bin');
    let head = fs.createWriteStream('./inc/ext/pack_nar.h');
    let code = fs.createWriteStream('./inc/ext/init_nar.h');

    head.write(`extern char* resources;\n\n`);
    
    Serialize(outputs, bin, head, code);
})();

async function Serialize(input, bin, head, code){
    for(let key in input){
        if(!Buffer.isBuffer(input[key])){
            await Serialize(input[key], bin, head, code);
            continue;
        }
        
        head.write(`#define NAR_${key} (resources + ${bin.bytesWritten + 4})\n`);
        await bin.async_write(Buffer.from(new Uint32Array([input[key].length]).buffer));
        await bin.async_write(input[key]);
    }
}
