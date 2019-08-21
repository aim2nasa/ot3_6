OPTEE_VERSION=3.6
ifeq ($(OPTEE_VERSION), )
$(error OPTEE_VERSION is not set)
endif

CURDIR = $(shell pwd)
export TEE_TA_LOG_LEVEL ?= 3
export OPTEE_ROOT = $(CURDIR)/optee/$(OPTEE_VERSION)/TALL$(TEE_TA_LOG_LEVEL)
export CROSS_COMPILE =$(OPTEE_ROOT)/../toolchains/aarch64/bin/aarch64-linux-gnu-
export TEEC_EXPORT= $(OPTEE_ROOT)
export TA_DEV_KIT_DIR=$(OPTEE_ROOT)/arm/export-ta_arm64

all:
	@echo CURDIR=$(CURDIR)
	@echo TEE_TA_LOG_LEVEL=$(TEE_TA_LOG_LEVEL)
	@echo OPTEE_VERSION=$(OPTEE_VERSION)
	@echo OPTEE_ROOT=$(OPTEE_ROOT)
	@echo CROSS_COMPLIE=$(CROSS_COMPILE)
	@echo TEEC_EXPORT=$(TEEC_EXPORT)
	@echo TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)

	@cd optee/$(OPTEE_VERSION)/toolchains;make -f toolchain.mk
	@cd googletest;cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm64.cmake && make

	@cd host;make
	@cd ta;make
	@./tnfs3.6.sh $(OPTEE_VERSION) $(TEE_TA_LOG_LEVEL)

clean:
	@cd host;make clean
	@cd ta;make clean
