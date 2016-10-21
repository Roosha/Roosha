### Network management

#### Prerequisits

Client-server communication is provided by [grpc](https://github.com/grpc/grpc), so you need to follow 
[official installation guide](https://github.com/grpc/grpc/blob/master/INSTALL.md)
in order to build project.

#### Generated sources

Grpc uses(by default) [protobuf](https://github.com/google/protobuf) for serialization. 
Protobuf files are stored in Network/Proto directory. 
Generated .h and .cc files are stored
in the same directory through command ```make clean && make -f ProtoMakefile```.
*Note:* whenever you change any .proto file, you have to manually regenerate corresponding C++ sources.
