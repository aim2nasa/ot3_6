# common environment
export TEE_TA_LOG_LEVEL=3
export OPTEE_ROOT=~/ot3.6/optee/3.6/TALL$TEE_TA_LOG_LEVEL
export CROSS_COMPILE=$OPTEE_ROOT/../toolchains/aarch64/bin/aarch64-linux-gnu-
export TEEC_EXPORT=$OPTEE_ROOT
export TA_DEV_KIT_DIR=$OPTEE_ROOT/arm/export-ta_arm64
