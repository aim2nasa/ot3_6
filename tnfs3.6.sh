NFS=/srv/nfs/linaro-stretch-developer-20180416-89
ROOT=$NFS/root
TA_DIR=$NFS/lib/optee_armtz

echo "NFS="$NFS
echo "ROOT="$ROOT
echo "TA_DIR="$TA_DIR

mkdir -p $TA_DIR

cp host/hello/hello $ROOT
cp ta/hello/973c225c-4f59-465c-9aa5-9b6b4d40b910.ta $TA_DIR

cp host/storageTest/enumFiles/enumFiles $ROOT
cp host/storageTest/perObj/perObj $ROOT

cp -a host/libfs/libfs.so* $NFS/lib
cp host/libfs/test/create/create $ROOT

cp ./ta/concurrent_large/5ce0c432-0ab0-40e5-a056-782ca0e6aba2.ta $TA_DIR
cp ./ta/rpc_test/d17f73a0-36ef-11e1-984a-0002a5d5c51b.ta $TA_DIR
cp ./ta/concurrent/e13010e0-2ae1-11e5-896a-0002a5d5c51b.ta $TA_DIR
cp ./ta/storage/b689f2a7-8adf-477a-9f99-32e90c0ad0a2.ta $TA_DIR
cp ./ta/sims/e6a33ed4-562b-463a-bb7e-ff5e15a493c8.ta $TA_DIR
cp ./ta/sdp_basic/12345678-5b69-11e4-9dbb-101f74f00099.ta $TA_DIR
cp ./ta/create_fail_test/c3f6e2c0-3548-11e1-b86c-0800200c9a66.ta $TA_DIR
cp ./ta/sims_keepalive/a4c04d50-f180-11e8-8eb2-f2801f1b9fd1.ta $TA_DIR
cp ./ta/sha_perf/614789f2-39c0-4ebf-b235-92b32ac107ed.ta $TA_DIR
cp ./ta/hello/973c225c-4f59-465c-9aa5-9b6b4d40b910.ta $TA_DIR
cp ./ta/socket/873bcd08-c2c3-11e6-a937-d0bf9c45c61c.ta $TA_DIR
cp ./ta/storage2/731e279e-aafb-4575-a771-38caa6f0cca6.ta $TA_DIR
cp ./ta/storage_benchmark/f157cda0-550c-11e5-a6fa-0002a5d5c51b.ta $TA_DIR
cp ./ta/os_test_lib/ffd2bded-ab7d-4988-95ee-e4962fff7154.ta $TA_DIR
cp ./ta/crypt/cb3e5ba0-adf1-11e0-998b-0002a5d5c51b.ta $TA_DIR
cp ./ta/aes_perf/e626662e-c0e2-485c-b8c8-09fbce6edf3d.ta $TA_DIR
cp ./ta/os_test/5b9e0e40-2636-11e1-ad9e-0002a5d5c51b.ta $TA_DIR
cp ./ta/miss/528938ce-fc59-11e8-8eb2-f2801f1b9fd1.ta $TA_DIR
