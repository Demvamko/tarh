const path = require('path');
const fs = require('fs').promises;

async function tree(root, callback){
    const dir = await fs.readdir(root);

    for(let file of dir){
        fpath = path.resolve(root, file);

        let stat = await fs.stat(fpath);

        if(stat.isDirectory())
            await tree(fpath, callback);
        else
            await callback(fpath);        
    }
}

module.exports = {
    tree
}