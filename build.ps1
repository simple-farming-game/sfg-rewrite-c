echo --------------------
echo build
echo --------------------
cmake -Bbuild . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
cmake --build build --config Release --target SFG
echo --------------------
echo run
echo --------------------
.\build\Release\SFG.exe
