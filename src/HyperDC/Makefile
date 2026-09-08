# HDchamber is an interactive environment where geometric figures from
# higher dimensions are brought to life and can be studied.
# Copyright (C) 2024  Davide Garbato (deeglome)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

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