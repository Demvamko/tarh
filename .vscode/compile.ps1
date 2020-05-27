cp "C:\Program Files (x86)\GLFW\bin\glfw3.dll" ./bin/glfw3.dll

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
/Fe: `
bin\\window.exe `
/Fo.\\bin\\ `
/Fdbin\\vc140.pdb `
src\\* `
glfw3dll.lib `
opengl32.lib `
/FI arh.h `
/link `
/LIBPATH:"C:\Program Files (x86)\GLFW\lib" `
/LIBPATH:".\\lib";