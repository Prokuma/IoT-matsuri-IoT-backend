# IoT-matsuri-IoT-backend
組込みデバイス用のバックエンド

## 開発
ライブラリの導入
```
mkdir build
cd build

conan install ..
```
ビルド
```
//buildディレクトリ内で
//Windows
cmake .. -G "Visual Studio 16"
cmake --build . --config Release

//Linux, Unix
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```