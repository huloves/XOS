losetup_device=`losetup -f`
sudo losetup $losetup_device ./hd.img
sudo mkfs -q $losetup_device

if [ ! -d "./hdisk" ]; then
	sudo mkdir ./hdisk
fi

sudo mount -o loop ./hd.img ./hdisk

if [ ! -d "./hdisk/boot" ]; then
	sudo mkdir ./hdisk/boot
	sudo grub-install --boot-directory=./hdisk/boot/ --force --allow-floppy $losetup_device
fi

