# make a floppy file system from the bootfs directory
# resulting ramdisk is bootfs.new.gz
# remove old tar file if it exists
rm bootfs.tar 2>/dev/null
mkdir /fd 2>/dev/null
# start in the root of the boot filesystem directory
cd bootfs
# create a tarball
tar cf ../bootfs.tar .
# go back to the main directory
cd ..
# zero it out for the compressed file system
# extra space is zero and will compress
dd if=/dev/zero of=/dev/ram0 bs=1k count=4096
# make an ext2 file system
mke2fs -N 1024 /dev/ram0  
# mount it
mount /dev/ram0 /fd
# go to the root of the ramdisk
cd /fd
# untar the file system files
tar xf ~-/bootfs.tar
# remove the file system files
rm ~-/bootfs.tar
# go back to the working directory
cd ~-
# unmount the ramdisk
umount /fd
# dd the image to a file
dd if=/dev/ram0 of=initrd bs=1k count=4096
# remove the old gz if it exists
rm initrd.gz 2>/dev/null
# make the new gz image
gzip initrd
