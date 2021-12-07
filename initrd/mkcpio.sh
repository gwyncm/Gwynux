# make a cpio ramdisk file system from the bootfs directory
# remove old tar file if it exists
rm initrd.igz 2>/dev/null
# start in the root of the boot filesystem directory
cd bootfs
# create a cpio
find . | cpio -H newc -o > ../cpio-initrd
# go back to the main directory
cd ..
# make the new gz image
gzip cpio-initrd
# rename the new image
mv cpio-initrd.gz initrd.igz
