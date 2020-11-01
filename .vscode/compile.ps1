cp "C:\Users\Demi\Desktop\glfw-3.3.2.bin.WIN64\lib-vc2019\glfw3.dll" ./bin/glfw3.dll

node ./tools/nar/main.js

cl `
/W3 `
/wd4312 `
/wd4244 `
/Zi `
/EHsc `
/D GLEW_STATIC `
/I ./inc `
/I ./dep `
/I ./dep/cglm/include/cglm `
/I ./dep/glfw/include/GLFW `
/I ./dep/freetype/include `
/Fe: `
bin\\window.exe `
/Fo.\\bin\\ `
/Fdbin\\vc140.pdb `
src\\*.c `
src\\arh\\*.c `
src\\game\\*.c `
src\\lib\\*.c `
src\\voxel\\*.c `
src\\gl\\*.c `
glfw3dll.lib `
opengl32.lib `
/FI arh\\arh.h `
/link `
/LIBPATH:"C:\Program Files (x86)\GLFW\lib" `
/LIBPATH:"C:\Users\Demi\Desktop\glfw-3.3.2.bin.WIN64\lib-vc2019\" `
/LIBPATH:".\\lib";