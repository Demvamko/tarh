param (
    [switch]$clean,
    [switch]$run
)

if($clean){
    rm ./build/*
}

$SOURCES = ls src -Recurse -Filter *.c | % { $_.FullName}

node ./tools/nar/main.js

cl /Zi /EHsc `
/I ./inc `
/I C:\Users\Demi\Desktop\Files\Installs\vcpkg\installed\x64-windows\include `
/I C:\Users\Demi\Desktop\Files\Installs\cglm\include\cglm `
/I C:\Users\Demi\Desktop\Files\Installs\libs `
/Fe: bin\\window.exe `
/Fo: bin\\ `
/Fd: bin\\vc140.pdb `
/FI arh\\arh.h `
@SOURCES `
glfw3dll.lib `
opengl32.lib `
glew32.lib `
/link `
/LIBPATH:"C:\Users\Demi\Desktop\Files\Installs\vcpkg\installed\x64-windows\lib";

cp C:\Users\Demi\Desktop\Files\Installs\vcpkg\installed\x64-windows\bin\glfw3.dll ./bin/glfw3.dll
cp C:\Users\Demi\Desktop\Files\Installs\vcpkg\installed\x64-windows\bin\glew32.dll ./bin/glew32.dll