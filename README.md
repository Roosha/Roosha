### GRPC usage

#### Getting grpc
We use [grpc](https://github.com/grpc/grpc) for client-server communication, so first of all you need to install grpc to generate
RPC code. In Java we use Maven plugin to generate code, so you don't need to do anything special.

In C++ you need to build grpc from sources. To do this, follow
[official installation guide](https://github.com/grpc/grpc/blob/master/INSTALL.md).

#### Generating sources

To generate RPC code for C++ and update .proto files in Java modules, you need to run make in Proto/ subdirectory:
```
cd Proto/
make clean && make
```

This will copy .proto files to Java modules to appropriate folder, and generate C++ sources in appropriate folder.

**Note:** you need to `make` generated files whenever you change any .proto file. Also you should not create any other file in `Client/Network/Proto` and `TranslationService/src/main/proto` directories because they will be removed by `make clean`
