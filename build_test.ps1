# 기존 Debug 디렉토리 삭제
Remove-Item -Path .\build\Debug -Recurse -Force

# 구분선 출력
echo --------------------
echo build
echo --------------------

# CMake로 Debug 빌드 구성 생성
cmake -Bbuild . -DCMAKE_BUILD_TYPE=Debug

# Debug 빌드 실행
cmake --build build --config Debug --target SFG

# 구분선 출력
echo --------------------
echo run
echo --------------------

# 디렉토리 이동 및 실행
.\build\Debug\SFG.exe
