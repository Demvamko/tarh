const path = require('path');
const fs = require('fs').promises;

async function Process(cmd){
    console.log("Processing: " + cmd.name);

    let repr = await Parse(cmd.path);
    repr.name = cmd.name;
    let sources = Generate(repr);

    return sources;
}

function Var(match){
    let self = {};

    self.vtype = match[1];
    self.bind = match[2];
    self.type = match[3];
    self.name = match[4];
    self.body = match[5] || '';

    if(self.vtype == 'inp')
        self.def_str = `layout (location = ${self.bind}) in ${self.type} ${self.name};\n`;
    else if(self.vtype == 'fun')
        self.def_str = `${self.type} ${self.name} ${self.body};\n`;
    else if(self.vtype == 'ubo')
        self.def_str = `layout (binding = ${self.bind}, std140) uniform struct_${self.name} ${self.type} ${self.name};\n`;
    else if(self.vtype == 'uni')
        self.def_str = `layout (binding = ${self.bind}) uniform ${self.type} ${self.name};\n`;
    else if(self.vtype == 'var')
        self.def_str = ``;

    return self;
}

async function Parse(fpath, repr){
    let source = await fs.readFile(fpath);
    source = source.toString();

    repr = repr || {
        name: '',
        vars: [],
        defs: {},
        seen: [],
        vert: '',
        frag: '',
        ver: 440
    };

    let inc_regex = /#include <(\S+)>/gm;
    let ver_regex = /#version ([0-9]+)/gm;
    let src_regex = /(frag|vert)\s?\{([\S\s]+?)\}/gm;
    let def_regex = /#define\s+(\w+)\s+([\S\s]+?)\s*\n/gm;
    let var_regex = /(inp|ubo|var|uni|fun)\s+([0-9])?\s?(\w+|\{[\S\s]+?\})\s+(\w+)\s?(\([\S\s]+?\)\{[\S\s]+?\})?;/gm;

    while(match = inc_regex.exec(source))
        await Parse(path.resolve(path.dirname(fpath), match[1]), repr);

    while(match = src_regex.exec(source))
        repr[match[1]] = match[2];

    while(match = var_regex.exec(source))
        repr.vars.push(new Var(match));

    while(match = def_regex.exec(source))
        repr.defs[match[1]] = match[2];

    ver_match = ver_regex.exec(source);
    if(ver_match)
        repr.ver = ver_match[1];

    repr.vert = repr.vert.replace(/\/\/[.\r]*\n/g, '\n');
    repr.frag = repr.frag.replace(/\/\/.*\r\n/g, '\n');

    return repr;
}

function Generate(repr){
    let vert = `#version ${repr.ver} \n`;
    let frag = `#version ${repr.ver} \nout vec4 _nar_color; \n`;

    let inputs = [];
    let others = [];

    for(let i of repr.vars){
        if(i.vtype == 'inp' || i.vtype == 'var')
            inputs.push(i);
        else
            others.push(i);
    }

    for(let i of inputs){
        vert += i.def_str;
        
        let regex = new RegExp(`(?<!\\.)\\b${i.name}\\b`, 'g');
        let rvert = repr.vert.match(regex);
        let rfrag = repr.frag.match(regex);

        repr.vert = repr.vert.replace(regex, `r_${i.name}`);
        repr.frag = repr.frag.replace(regex, `r_${i.name}`);

        if((rvert || rfrag) && i.vtype != 'var')
            repr.vert = `    ${rfrag ? '' : i.type + ' '}r_${i.name} = ${i.name};\n` + repr.vert;

        if(rfrag){
            frag += `in ${i.type} r_${i.name};\n`;
            vert += `out ${i.type} r_${i.name};\n`;
        }
    }

    for(let i of others){
        let regex = new RegExp(`\\b(?<!\\.)${i.name}\\b`, 'g');

        if(regex.test(repr.vert))
            vert += i.def_str;
        
        if(regex.test(repr.frag))
            frag += i.def_str;
    }

    repr.vert = repr.vert.replace(/return /, 'gl_Position = ');
    repr.frag = repr.frag.replace(/return /, '_nar_color = ');

    vert += `void main(){ \n${repr.vert} } \n`;
    frag += `void main(){ \n${repr.frag} } \n`;

    for(let def in repr.defs){
        let regex = new RegExp(`\\b${def}\\b`, 'g');
        vert = vert.replace(regex, repr.defs[def]);
        frag = frag.replace(regex, repr.defs[def]);
    }

    vert = vert.replace(/\r/g, '');
    frag = frag.replace(/\r/g, '');


    console.log(vert);
    console.log(frag);

    return { 
        [`${repr.name}_VERT`]: Buffer.from(vert), 
        [`${repr.name}_FRAG`]: Buffer.from(frag) 
    };
}

module.exports = { Process, Parse, Var, Generate };