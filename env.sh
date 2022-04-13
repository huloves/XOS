losetup_device=`losetup -f`
sudo losetup $losetup_device ./hd.img
sudo mkfs -q $losetup_device

if [ ! -d "./hdisk" ]; then
	mkdir ./hdisk
fi

sudo mount -o loop ./hd.img ./hdisk

if [ ! -d "./hdisk/boot" ]; then
	mkdir ./hdisk/boot
	sudo grub-install --boot-directory=./hdisk/boot/ --force --allow-floppy /dev/loop0
fi

