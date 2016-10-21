PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

REGISTER_METATYPES=./register_metatypes.py
PROTOS_DIR=.
CPP_GENERATED_DIR=../Client/Network/Proto
TRANSLATION_SERVICE_PROTO_DIR=../TranslationService/src/main/proto

all: cxx_generated_sources java_directory_files

java_directory_files: java_proto_directory
	cp $(PROTOS_DIR)/*.proto $(TRANSLATION_SERVICE_PROTO_DIR)

java_proto_directory:
	mkdir $(TRANSLATION_SERVICE_PROTO_DIR)

# optional target for registering generated classes in QMetaType
register_metatypes: cxx_generated_sources make_register_metatypes_script_executable
	$(REGISTER_METATYPES) -i $(PROTOS_DIR) -o $(CPP_GENERATED_DIR)

cxx_generated_sources: translation_service.pb.cc translation_service.grpc.pb.cc commons.pb.cc

%.grpc.pb.cc: %.proto
	$(PROTOC) -I=$(PROTOS_DIR) --grpc_out=$(CPP_GENERATED_DIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto cxx_generated_code_dir
	$(PROTOC) -I=$(PROTOS_DIR) --cpp_out=$(CPP_GENERATED_DIR) $<

cxx_generated_code_dir:
	mkdir $(CPP_GENERATED_DIR)

make_register_metatypes_script_executable:
	echo 'making script executable'
	chmod +x $(REGISTER_METATYPES)

clean:
	rm -rf $(CPP_GENERATED_DIR) $(TRANSLATION_SERVICE_PROTO_DIR)
