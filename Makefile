OPTEE_VERSION=3.6
ifeq ($(OPTEE_VERSION), )
$(error OPTEE_VERSION is not set)
endif

CURDIR = $(shell pwd)
export CROSS_COMPILE=$(CURDIR)/optee/$(OPTEE_VERSION)/toolchains/aarch64/bin/aarch64-linux-gnu-
export TEEC_EXPORT=$(CURDIR)/optee/$(OPTEE_VERSION)
export TA_DEV_KIT_DIR=$(CURDIR)/optee/$(OPTEE_VERSION)/arm/export-ta_arm64
export OKEY_ROOT=$(CURDIR)/keygen/host/libokey

all:
	@echo CURDIR=$(CURDIR)
	@echo OPTEE_VERSION=$(OPTEE_VERSION)
	@echo CROSS_COMPLIE=$(CROSS_COMPILE)
	@echo TEEC_EXPORT=$(TEEC_EXPORT)
	@echo TA_DEV_KIT_DIR=$(TA_DEV_KIT_DIR)
	@echo OKEY_ROOT=$(OKEY_ROOT)

	@cd optee/$(OPTEE_VERSION)/toolchains;make -f toolchain.mk

	@cd host;make
	@cd ta;make

clean:
	@cd host;make clean
	@cd ta;make clean
