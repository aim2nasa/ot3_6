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
cp ta/storage/d18e20ef-01fe-4e9d-a469-69e3227ee6e9.ta $TA_DIR
