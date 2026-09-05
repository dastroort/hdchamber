GXX := g++
EIGEN := /usr/include/eigen3
EMSDK := ~/emsdk

.PHONY: all clean wasm test

all: clean wasm

SRC_DIR := ./src

HYPERDC_FILES := HyperCam.cpp
HYPERDC_SRCS := $(addprefix $(SRC_DIR)/, $(HYPERDC_FILES))

WASM_DIR := ../wasm
WASM_FILES := geolib.wasm geolib.js
WASM_SRCS := $(addprefix $(WASM_DIR)/, $(WASM_FILES))

wasm: $(SRC_DIR)/wasm.cpp
	bash -c "source $(EMSDK)/emsdk_env.sh && em++ $(SRC_DIR)/wasm.cpp $(HYPERDC_SRCS) \
	-I$(EIGEN) \
	-lembind \
	-s WASM=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s MODULARIZE=1 \
	-s EXPORT_ES6=1 \
	-s EXPORT_NAME='createGeolib' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	-s DISABLE_EXCEPTION_CATCHING=0 \
	-s ASSERTIONS=2 \
	-s SAFE_HEAP=1 \
	-s STACK_OVERFLOW_CHECK=2 \
	-g \
	-o $(WASM_DIR)/geolib.js"

TESTS_DIR := ./tests
TEST_SRC  := $(addprefix $(TESTS_DIR)/, test.cpp)
TEST_OUT  := $(addprefix $(TESTS_DIR)/, test.out)

test: $(TEST_SRC)
	$(GXX) $(TEST_SRC) -o $(TEST_OUT) -I$(EIGEN)
	$(TEST_OUT)

clean:
	rm -fv $(WASM_SRCS)