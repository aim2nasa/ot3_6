NFS=/srv/nfs/linaro-stretch-developer-20180416-89
ROOT=$NFS/root
TA_DIR=$NFS/lib/optee_armtz

echo "NFS="$NFS
echo "ROOT="$ROOT
echo "TA_DIR="$TA_DIR

mkdir -p $TA_DIR

cp hello/host/hello $ROOT
cp hello/ta/973c225c-4f59-465c-9aa5-9b6b4d40b910.ta $TA_DIR
