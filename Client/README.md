## Roosha Client

### How to build client

#### Getting grpc
We use [grpc](https://github.com/grpc/grpc) for client-server communication, so first of all you need to install grpc to generate
RPC code. In Java we use Maven plugin to generate code, so you don't need to do anything special.

In C++ you need to build grpc from sources. To do this, follow
[official installation guide](https://github.com/grpc/grpc/blob/master/INSTALL.md).


**Note:** you need to `make` generated files whenever you change any .proto file. Also you should not create any other file in `Client/Network/Proto` and `TranslationService/src/main/proto` directories because they will be removed by `make clean`

**Note:** if you have some problems with protobuf after you've installed grpc(e.g. protobuf headers not found or wrong version, protobuf compiler
not found etc.), this is probably because you already had some parts of protobuf. To fix this, go to grpc sources directory(after you have
built it) and do the following:
```
cd third_party/protobuf
sudo make uninstall && sudo make install
```

#### Getting Qt 5
Roosha client is based on [Qt 5](https://www.qt.io/), so you need it to [install](https://www.qt.io/download/) 
this framework.

In order to simplify build process you can set environment variable to specify path to your Qt installation for CMake.
```
export CMAKE_PREFIX_PATH=/path/to/Qt/version/gcc_64
```
#### Building project

To build project, create build directory and copy all project files there.
```
mkdir build
cp * build -r
```

Then you can build project.
```
cd build
cmake . -DCMAKE_PREFIX_PATH=/path/to/Qt/version/gcc_64
cmake --build . --target roosha.client
```

And than you can launch the app
```
./roosha.client
```