	.file	"devicetable-offsets.c"
 # GNU C11 (GCC) version 13.0.0 20220906 (experimental) (loongarch64-unknown-linux-gnu)
 #	compiled by GNU C version 11.0.1 20210324 (Red Hat 11.0.1-0), GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version none
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: -G 0 -mexplicit-relocs -mno-check-zero-division -mdirect-extern-access -mabi=lp64s -march=loongarch64 -mfpu=none -mcmodel=normal -mtune=la464 -g -O0 -O2 -Os -std=gnu11 -fno-strict-aliasing -fno-common -fshort-wchar -funsigned-char -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -fno-allow-store-data-races -fstack-protector-strong -ftrivial-auto-var-init=zero -fno-stack-clash-protection -fstack-check=no -fpie -ffreestanding -fno-stack-protector -fomit-frame-pointer -fno-strict-overflow -fconserve-stack -fno-tree-scev-cprop
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/huqingwei/repositories/XOS-loongarch64" "scripts/mod/devicetable-offsets.c"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
.LFB0 = .
	.file 1 "scripts/mod/devicetable-offsets.c"
	.loc 1 9 1 view -0
	.cfi_startproc
	.loc 1 10 2 view .LVU1
#APP
 # 10 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_usb_device_id 32 sizeof(struct usb_device_id)	 #
 # 0 "" 2
	.loc 1 11 2 view .LVU2
 # 11 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_match_flags 0 offsetof(struct usb_device_id, match_flags)	 #
 # 0 "" 2
	.loc 1 12 2 view .LVU3
 # 12 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_idVendor 2 offsetof(struct usb_device_id, idVendor)	 #
 # 0 "" 2
	.loc 1 13 2 view .LVU4
 # 13 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_idProduct 4 offsetof(struct usb_device_id, idProduct)	 #
 # 0 "" 2
	.loc 1 14 2 view .LVU5
 # 14 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bcdDevice_lo 6 offsetof(struct usb_device_id, bcdDevice_lo)	 #
 # 0 "" 2
	.loc 1 15 2 view .LVU6
 # 15 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bcdDevice_hi 8 offsetof(struct usb_device_id, bcdDevice_hi)	 #
 # 0 "" 2
	.loc 1 16 2 view .LVU7
 # 16 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bDeviceClass 10 offsetof(struct usb_device_id, bDeviceClass)	 #
 # 0 "" 2
	.loc 1 17 2 view .LVU8
 # 17 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bDeviceSubClass 11 offsetof(struct usb_device_id, bDeviceSubClass)	 #
 # 0 "" 2
	.loc 1 18 2 view .LVU9
 # 18 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bDeviceProtocol 12 offsetof(struct usb_device_id, bDeviceProtocol)	 #
 # 0 "" 2
	.loc 1 19 2 view .LVU10
 # 19 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bInterfaceClass 13 offsetof(struct usb_device_id, bInterfaceClass)	 #
 # 0 "" 2
	.loc 1 20 2 view .LVU11
 # 20 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bInterfaceSubClass 14 offsetof(struct usb_device_id, bInterfaceSubClass)	 #
 # 0 "" 2
	.loc 1 21 2 view .LVU12
 # 21 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bInterfaceProtocol 15 offsetof(struct usb_device_id, bInterfaceProtocol)	 #
 # 0 "" 2
	.loc 1 22 2 view .LVU13
 # 22 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_usb_device_id_bInterfaceNumber 16 offsetof(struct usb_device_id, bInterfaceNumber)	 #
 # 0 "" 2
	.loc 1 24 2 view .LVU14
 # 24 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_hid_device_id 24 sizeof(struct hid_device_id)	 #
 # 0 "" 2
	.loc 1 25 2 view .LVU15
 # 25 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_hid_device_id_bus 0 offsetof(struct hid_device_id, bus)	 #
 # 0 "" 2
	.loc 1 26 2 view .LVU16
 # 26 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_hid_device_id_group 2 offsetof(struct hid_device_id, group)	 #
 # 0 "" 2
	.loc 1 27 2 view .LVU17
 # 27 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_hid_device_id_vendor 4 offsetof(struct hid_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 28 2 view .LVU18
 # 28 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_hid_device_id_product 8 offsetof(struct hid_device_id, product)	 #
 # 0 "" 2
	.loc 1 30 2 view .LVU19
 # 30 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_ieee1394_device_id 32 sizeof(struct ieee1394_device_id)	 #
 # 0 "" 2
	.loc 1 31 2 view .LVU20
 # 31 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ieee1394_device_id_match_flags 0 offsetof(struct ieee1394_device_id, match_flags)	 #
 # 0 "" 2
	.loc 1 32 2 view .LVU21
 # 32 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ieee1394_device_id_vendor_id 4 offsetof(struct ieee1394_device_id, vendor_id)	 #
 # 0 "" 2
	.loc 1 33 2 view .LVU22
 # 33 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ieee1394_device_id_model_id 8 offsetof(struct ieee1394_device_id, model_id)	 #
 # 0 "" 2
	.loc 1 34 2 view .LVU23
 # 34 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ieee1394_device_id_specifier_id 12 offsetof(struct ieee1394_device_id, specifier_id)	 #
 # 0 "" 2
	.loc 1 35 2 view .LVU24
 # 35 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ieee1394_device_id_version 16 offsetof(struct ieee1394_device_id, version)	 #
 # 0 "" 2
	.loc 1 37 2 view .LVU25
 # 37 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_pci_device_id 32 sizeof(struct pci_device_id)	 #
 # 0 "" 2
	.loc 1 38 2 view .LVU26
 # 38 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_vendor 0 offsetof(struct pci_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 39 2 view .LVU27
 # 39 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_device 4 offsetof(struct pci_device_id, device)	 #
 # 0 "" 2
	.loc 1 40 2 view .LVU28
 # 40 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_subvendor 8 offsetof(struct pci_device_id, subvendor)	 #
 # 0 "" 2
	.loc 1 41 2 view .LVU29
 # 41 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_subdevice 12 offsetof(struct pci_device_id, subdevice)	 #
 # 0 "" 2
	.loc 1 42 2 view .LVU30
 # 42 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_class 16 offsetof(struct pci_device_id, class)	 #
 # 0 "" 2
	.loc 1 43 2 view .LVU31
 # 43 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pci_device_id_class_mask 20 offsetof(struct pci_device_id, class_mask)	 #
 # 0 "" 2
	.loc 1 45 2 view .LVU32
 # 45 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_ccw_device_id 16 sizeof(struct ccw_device_id)	 #
 # 0 "" 2
	.loc 1 46 2 view .LVU33
 # 46 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ccw_device_id_match_flags 0 offsetof(struct ccw_device_id, match_flags)	 #
 # 0 "" 2
	.loc 1 47 2 view .LVU34
 # 47 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ccw_device_id_cu_type 2 offsetof(struct ccw_device_id, cu_type)	 #
 # 0 "" 2
	.loc 1 48 2 view .LVU35
 # 48 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ccw_device_id_cu_model 6 offsetof(struct ccw_device_id, cu_model)	 #
 # 0 "" 2
	.loc 1 49 2 view .LVU36
 # 49 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ccw_device_id_dev_type 4 offsetof(struct ccw_device_id, dev_type)	 #
 # 0 "" 2
	.loc 1 50 2 view .LVU37
 # 50 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ccw_device_id_dev_model 7 offsetof(struct ccw_device_id, dev_model)	 #
 # 0 "" 2
	.loc 1 52 2 view .LVU38
 # 52 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_ap_device_id 16 sizeof(struct ap_device_id)	 #
 # 0 "" 2
	.loc 1 53 2 view .LVU39
 # 53 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ap_device_id_dev_type 2 offsetof(struct ap_device_id, dev_type)	 #
 # 0 "" 2
	.loc 1 55 2 view .LVU40
 # 55 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_css_device_id 16 sizeof(struct css_device_id)	 #
 # 0 "" 2
	.loc 1 56 2 view .LVU41
 # 56 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_css_device_id_type 1 offsetof(struct css_device_id, type)	 #
 # 0 "" 2
	.loc 1 58 2 view .LVU42
 # 58 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_serio_device_id 4 sizeof(struct serio_device_id)	 #
 # 0 "" 2
	.loc 1 59 2 view .LVU43
 # 59 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_serio_device_id_type 0 offsetof(struct serio_device_id, type)	 #
 # 0 "" 2
	.loc 1 60 2 view .LVU44
 # 60 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_serio_device_id_proto 3 offsetof(struct serio_device_id, proto)	 #
 # 0 "" 2
	.loc 1 61 2 view .LVU45
 # 61 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_serio_device_id_id 2 offsetof(struct serio_device_id, id)	 #
 # 0 "" 2
	.loc 1 62 2 view .LVU46
 # 62 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_serio_device_id_extra 1 offsetof(struct serio_device_id, extra)	 #
 # 0 "" 2
	.loc 1 64 2 view .LVU47
 # 64 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_acpi_device_id 24 sizeof(struct acpi_device_id)	 #
 # 0 "" 2
	.loc 1 65 2 view .LVU48
 # 65 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_acpi_device_id_id 0 offsetof(struct acpi_device_id, id)	 #
 # 0 "" 2
	.loc 1 67 2 view .LVU49
 # 67 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_pnp_device_id 16 sizeof(struct pnp_device_id)	 #
 # 0 "" 2
	.loc 1 68 2 view .LVU50
 # 68 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pnp_device_id_id 0 offsetof(struct pnp_device_id, id)	 #
 # 0 "" 2
	.loc 1 70 2 view .LVU51
 # 70 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_pnp_card_device_id 80 sizeof(struct pnp_card_device_id)	 #
 # 0 "" 2
	.loc 1 71 2 view .LVU52
 # 71 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pnp_card_device_id_devs 16 offsetof(struct pnp_card_device_id, devs)	 #
 # 0 "" 2
	.loc 1 73 2 view .LVU53
 # 73 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_pcmcia_device_id 80 sizeof(struct pcmcia_device_id)	 #
 # 0 "" 2
	.loc 1 74 2 view .LVU54
 # 74 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_match_flags 0 offsetof(struct pcmcia_device_id, match_flags)	 #
 # 0 "" 2
	.loc 1 75 2 view .LVU55
 # 75 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_manf_id 2 offsetof(struct pcmcia_device_id, manf_id)	 #
 # 0 "" 2
	.loc 1 76 2 view .LVU56
 # 76 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_card_id 4 offsetof(struct pcmcia_device_id, card_id)	 #
 # 0 "" 2
	.loc 1 77 2 view .LVU57
 # 77 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_func_id 6 offsetof(struct pcmcia_device_id, func_id)	 #
 # 0 "" 2
	.loc 1 78 2 view .LVU58
 # 78 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_function 7 offsetof(struct pcmcia_device_id, function)	 #
 # 0 "" 2
	.loc 1 79 2 view .LVU59
 # 79 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_device_no 8 offsetof(struct pcmcia_device_id, device_no)	 #
 # 0 "" 2
	.loc 1 80 2 view .LVU60
 # 80 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_pcmcia_device_id_prod_id_hash 12 offsetof(struct pcmcia_device_id, prod_id_hash)	 #
 # 0 "" 2
	.loc 1 82 2 view .LVU61
 # 82 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_of_device_id 200 sizeof(struct of_device_id)	 #
 # 0 "" 2
	.loc 1 83 2 view .LVU62
 # 83 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_of_device_id_name 0 offsetof(struct of_device_id, name)	 #
 # 0 "" 2
	.loc 1 84 2 view .LVU63
 # 84 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_of_device_id_type 32 offsetof(struct of_device_id, type)	 #
 # 0 "" 2
	.loc 1 85 2 view .LVU64
 # 85 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_of_device_id_compatible 64 offsetof(struct of_device_id, compatible)	 #
 # 0 "" 2
	.loc 1 87 2 view .LVU65
 # 87 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_vio_device_id 64 sizeof(struct vio_device_id)	 #
 # 0 "" 2
	.loc 1 88 2 view .LVU66
 # 88 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_vio_device_id_type 0 offsetof(struct vio_device_id, type)	 #
 # 0 "" 2
	.loc 1 89 2 view .LVU67
 # 89 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_vio_device_id_compat 32 offsetof(struct vio_device_id, compat)	 #
 # 0 "" 2
	.loc 1 91 2 view .LVU68
 # 91 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_input_device_id 192 sizeof(struct input_device_id)	 #
 # 0 "" 2
	.loc 1 92 2 view .LVU69
 # 92 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_flags 0 offsetof(struct input_device_id, flags)	 #
 # 0 "" 2
	.loc 1 93 2 view .LVU70
 # 93 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_bustype 8 offsetof(struct input_device_id, bustype)	 #
 # 0 "" 2
	.loc 1 94 2 view .LVU71
 # 94 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_vendor 10 offsetof(struct input_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 95 2 view .LVU72
 # 95 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_product 12 offsetof(struct input_device_id, product)	 #
 # 0 "" 2
	.loc 1 96 2 view .LVU73
 # 96 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_version 14 offsetof(struct input_device_id, version)	 #
 # 0 "" 2
	.loc 1 97 2 view .LVU74
 # 97 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_evbit 16 offsetof(struct input_device_id, evbit)	 #
 # 0 "" 2
	.loc 1 98 2 view .LVU75
 # 98 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_keybit 24 offsetof(struct input_device_id, keybit)	 #
 # 0 "" 2
	.loc 1 99 2 view .LVU76
 # 99 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_relbit 120 offsetof(struct input_device_id, relbit)	 #
 # 0 "" 2
	.loc 1 100 2 view .LVU77
 # 100 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_absbit 128 offsetof(struct input_device_id, absbit)	 #
 # 0 "" 2
	.loc 1 101 2 view .LVU78
 # 101 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_mscbit 136 offsetof(struct input_device_id, mscbit)	 #
 # 0 "" 2
	.loc 1 102 2 view .LVU79
 # 102 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_ledbit 144 offsetof(struct input_device_id, ledbit)	 #
 # 0 "" 2
	.loc 1 103 2 view .LVU80
 # 103 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_sndbit 152 offsetof(struct input_device_id, sndbit)	 #
 # 0 "" 2
	.loc 1 104 2 view .LVU81
 # 104 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_ffbit 160 offsetof(struct input_device_id, ffbit)	 #
 # 0 "" 2
	.loc 1 105 2 view .LVU82
 # 105 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_input_device_id_swbit 176 offsetof(struct input_device_id, swbit)	 #
 # 0 "" 2
	.loc 1 107 2 view .LVU83
 # 107 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_eisa_device_id 16 sizeof(struct eisa_device_id)	 #
 # 0 "" 2
	.loc 1 108 2 view .LVU84
 # 108 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_eisa_device_id_sig 0 offsetof(struct eisa_device_id, sig)	 #
 # 0 "" 2
	.loc 1 110 2 view .LVU85
 # 110 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_parisc_device_id 8 sizeof(struct parisc_device_id)	 #
 # 0 "" 2
	.loc 1 111 2 view .LVU86
 # 111 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_parisc_device_id_hw_type 0 offsetof(struct parisc_device_id, hw_type)	 #
 # 0 "" 2
	.loc 1 112 2 view .LVU87
 # 112 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_parisc_device_id_hversion 2 offsetof(struct parisc_device_id, hversion)	 #
 # 0 "" 2
	.loc 1 113 2 view .LVU88
 # 113 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_parisc_device_id_hversion_rev 1 offsetof(struct parisc_device_id, hversion_rev)	 #
 # 0 "" 2
	.loc 1 114 2 view .LVU89
 # 114 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_parisc_device_id_sversion 4 offsetof(struct parisc_device_id, sversion)	 #
 # 0 "" 2
	.loc 1 116 2 view .LVU90
 # 116 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_sdio_device_id 16 sizeof(struct sdio_device_id)	 #
 # 0 "" 2
	.loc 1 117 2 view .LVU91
 # 117 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_sdio_device_id_class 0 offsetof(struct sdio_device_id, class)	 #
 # 0 "" 2
	.loc 1 118 2 view .LVU92
 # 118 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_sdio_device_id_vendor 2 offsetof(struct sdio_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 119 2 view .LVU93
 # 119 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_sdio_device_id_device 4 offsetof(struct sdio_device_id, device)	 #
 # 0 "" 2
	.loc 1 121 2 view .LVU94
 # 121 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_ssb_device_id 6 sizeof(struct ssb_device_id)	 #
 # 0 "" 2
	.loc 1 122 2 view .LVU95
 # 122 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ssb_device_id_vendor 0 offsetof(struct ssb_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 123 2 view .LVU96
 # 123 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ssb_device_id_coreid 2 offsetof(struct ssb_device_id, coreid)	 #
 # 0 "" 2
	.loc 1 124 2 view .LVU97
 # 124 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ssb_device_id_revision 4 offsetof(struct ssb_device_id, revision)	 #
 # 0 "" 2
	.loc 1 126 2 view .LVU98
 # 126 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_bcma_device_id 6 sizeof(struct bcma_device_id)	 #
 # 0 "" 2
	.loc 1 127 2 view .LVU99
 # 127 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_bcma_device_id_manuf 0 offsetof(struct bcma_device_id, manuf)	 #
 # 0 "" 2
	.loc 1 128 2 view .LVU100
 # 128 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_bcma_device_id_id 2 offsetof(struct bcma_device_id, id)	 #
 # 0 "" 2
	.loc 1 129 2 view .LVU101
 # 129 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_bcma_device_id_rev 4 offsetof(struct bcma_device_id, rev)	 #
 # 0 "" 2
	.loc 1 130 2 view .LVU102
 # 130 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_bcma_device_id_class 5 offsetof(struct bcma_device_id, class)	 #
 # 0 "" 2
	.loc 1 132 2 view .LVU103
 # 132 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_virtio_device_id 8 sizeof(struct virtio_device_id)	 #
 # 0 "" 2
	.loc 1 133 2 view .LVU104
 # 133 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_virtio_device_id_device 0 offsetof(struct virtio_device_id, device)	 #
 # 0 "" 2
	.loc 1 134 2 view .LVU105
 # 134 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_virtio_device_id_vendor 4 offsetof(struct virtio_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 136 2 view .LVU106
 # 136 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_hv_vmbus_device_id 24 sizeof(struct hv_vmbus_device_id)	 #
 # 0 "" 2
	.loc 1 137 2 view .LVU107
 # 137 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_hv_vmbus_device_id_guid 0 offsetof(struct hv_vmbus_device_id, guid)	 #
 # 0 "" 2
	.loc 1 139 2 view .LVU108
 # 139 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_i2c_device_id 32 sizeof(struct i2c_device_id)	 #
 # 0 "" 2
	.loc 1 140 2 view .LVU109
 # 140 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_i2c_device_id_name 0 offsetof(struct i2c_device_id, name)	 #
 # 0 "" 2
	.loc 1 142 2 view .LVU110
 # 142 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_spi_device_id 40 sizeof(struct spi_device_id)	 #
 # 0 "" 2
	.loc 1 143 2 view .LVU111
 # 143 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_spi_device_id_name 0 offsetof(struct spi_device_id, name)	 #
 # 0 "" 2
	.loc 1 145 2 view .LVU112
 # 145 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_dmi_system_id 344 sizeof(struct dmi_system_id)	 #
 # 0 "" 2
	.loc 1 146 2 view .LVU113
 # 146 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_dmi_system_id_matches 16 offsetof(struct dmi_system_id, matches)	 #
 # 0 "" 2
	.loc 1 148 2 view .LVU114
 # 148 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_platform_device_id 32 sizeof(struct platform_device_id)	 #
 # 0 "" 2
	.loc 1 149 2 view .LVU115
 # 149 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_platform_device_id_name 0 offsetof(struct platform_device_id, name)	 #
 # 0 "" 2
	.loc 1 151 2 view .LVU116
 # 151 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_mdio_device_id 8 sizeof(struct mdio_device_id)	 #
 # 0 "" 2
	.loc 1 152 2 view .LVU117
 # 152 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_mdio_device_id_phy_id 0 offsetof(struct mdio_device_id, phy_id)	 #
 # 0 "" 2
	.loc 1 153 2 view .LVU118
 # 153 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_mdio_device_id_phy_id_mask 4 offsetof(struct mdio_device_id, phy_id_mask)	 #
 # 0 "" 2
	.loc 1 155 2 view .LVU119
 # 155 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_zorro_device_id 16 sizeof(struct zorro_device_id)	 #
 # 0 "" 2
	.loc 1 156 2 view .LVU120
 # 156 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_zorro_device_id_id 0 offsetof(struct zorro_device_id, id)	 #
 # 0 "" 2
	.loc 1 158 2 view .LVU121
 # 158 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_isapnp_device_id 16 sizeof(struct isapnp_device_id)	 #
 # 0 "" 2
	.loc 1 159 2 view .LVU122
 # 159 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_isapnp_device_id_vendor 4 offsetof(struct isapnp_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 160 2 view .LVU123
 # 160 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_isapnp_device_id_function 6 offsetof(struct isapnp_device_id, function)	 #
 # 0 "" 2
	.loc 1 162 2 view .LVU124
 # 162 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_ipack_device_id 12 sizeof(struct ipack_device_id)	 #
 # 0 "" 2
	.loc 1 163 2 view .LVU125
 # 163 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ipack_device_id_format 0 offsetof(struct ipack_device_id, format)	 #
 # 0 "" 2
	.loc 1 164 2 view .LVU126
 # 164 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ipack_device_id_vendor 4 offsetof(struct ipack_device_id, vendor)	 #
 # 0 "" 2
	.loc 1 165 2 view .LVU127
 # 165 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_ipack_device_id_device 8 offsetof(struct ipack_device_id, device)	 #
 # 0 "" 2
	.loc 1 167 2 view .LVU128
 # 167 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_amba_id 16 sizeof(struct amba_id)	 #
 # 0 "" 2
	.loc 1 168 2 view .LVU129
 # 168 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_amba_id_id 0 offsetof(struct amba_id, id)	 #
 # 0 "" 2
	.loc 1 169 2 view .LVU130
 # 169 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_amba_id_mask 4 offsetof(struct amba_id, mask)	 #
 # 0 "" 2
	.loc 1 171 2 view .LVU131
 # 171 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_mips_cdmm_device_id 1 sizeof(struct mips_cdmm_device_id)	 #
 # 0 "" 2
	.loc 1 172 2 view .LVU132
 # 172 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_mips_cdmm_device_id_type 0 offsetof(struct mips_cdmm_device_id, type)	 #
 # 0 "" 2
	.loc 1 174 2 view .LVU133
 # 174 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_x86_cpu_id 16 sizeof(struct x86_cpu_id)	 #
 # 0 "" 2
	.loc 1 175 2 view .LVU134
 # 175 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_x86_cpu_id_feature 6 offsetof(struct x86_cpu_id, feature)	 #
 # 0 "" 2
	.loc 1 176 2 view .LVU135
 # 176 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_x86_cpu_id_family 2 offsetof(struct x86_cpu_id, family)	 #
 # 0 "" 2
	.loc 1 177 2 view .LVU136
 # 177 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_x86_cpu_id_model 4 offsetof(struct x86_cpu_id, model)	 #
 # 0 "" 2
	.loc 1 178 2 view .LVU137
 # 178 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_x86_cpu_id_vendor 0 offsetof(struct x86_cpu_id, vendor)	 #
 # 0 "" 2
	.loc 1 180 2 view .LVU138
 # 180 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_cpu_feature 2 sizeof(struct cpu_feature)	 #
 # 0 "" 2
	.loc 1 181 2 view .LVU139
 # 181 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_cpu_feature_feature 0 offsetof(struct cpu_feature, feature)	 #
 # 0 "" 2
	.loc 1 183 2 view .LVU140
 # 183 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_mei_cl_device_id 40 sizeof(struct mei_cl_device_id)	 #
 # 0 "" 2
	.loc 1 184 2 view .LVU141
 # 184 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_mei_cl_device_id_name 0 offsetof(struct mei_cl_device_id, name)	 #
 # 0 "" 2
	.loc 1 186 2 view .LVU142
 # 186 "scripts/mod/devicetable-offsets.c" 1
	
->SIZE_rio_device_id 8 sizeof(struct rio_device_id)	 #
 # 0 "" 2
	.loc 1 187 2 view .LVU143
 # 187 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_rio_device_id_did 0 offsetof(struct rio_device_id, did)	 #
 # 0 "" 2
	.loc 1 188 2 view .LVU144
 # 188 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_rio_device_id_vid 2 offsetof(struct rio_device_id, vid)	 #
 # 0 "" 2
	.loc 1 189 2 view .LVU145
 # 189 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_rio_device_id_asm_did 4 offsetof(struct rio_device_id, asm_did)	 #
 # 0 "" 2
	.loc 1 190 2 view .LVU146
 # 190 "scripts/mod/devicetable-offsets.c" 1
	
->OFF_rio_device_id_asm_vid 6 offsetof(struct rio_device_id, asm_vid)	 #
 # 0 "" 2
	.loc 1 192 2 view .LVU147
 # scripts/mod/devicetable-offsets.c:193: }
	.loc 1 193 1 is_stmt 0 view .LVU148
#NO_APP
	or	$r4,$r0,$r0	 #,
	jr	$r1		 #
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.text
.Letext0:
	.file 2 "include/linux/types.h"
	.file 3 "include/linux/mod_devicetable.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0xb2e
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0x10
	.4byte	.LASF135
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.4byte	.LLRL0
	.8byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x7
	.byte	0x4
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0xc
	.ascii	"u8\000"
	.byte	0x19
	.byte	0x18
	.4byte	0x3b
	.uleb128 0x7
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0xa
	.4byte	.LASF4
	.byte	0x2
	.byte	0x1a
	.byte	0x16
	.4byte	0x31
	.uleb128 0xc
	.ascii	"u16\000"
	.byte	0x1b
	.byte	0x19
	.4byte	0x59
	.uleb128 0x7
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0xa
	.4byte	.LASF6
	.byte	0x2
	.byte	0x1c
	.byte	0x1d
	.4byte	0x4e
	.uleb128 0xc
	.ascii	"u32\000"
	.byte	0x1d
	.byte	0x17
	.4byte	0x2a
	.uleb128 0xa
	.4byte	.LASF7
	.byte	0x2
	.byte	0x1e
	.byte	0x1d
	.4byte	0x6c
	.uleb128 0x7
	.byte	0x8
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x7
	.byte	0x2
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x7
	.byte	0x8
	.byte	0x5
	.4byte	.LASF10
	.uleb128 0xa
	.4byte	.LASF11
	.byte	0x2
	.byte	0x2a
	.byte	0x18
	.4byte	0xa4
	.uleb128 0x7
	.byte	0x8
	.byte	0x7
	.4byte	.LASF12
	.uleb128 0x4
	.4byte	0x42
	.4byte	0xbb
	.uleb128 0x5
	.4byte	0xa4
	.byte	0xf
	.byte	0
	.uleb128 0x11
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0xa
	.4byte	.LASF13
	.byte	0x3
	.byte	0xd
	.byte	0x17
	.4byte	0xa4
	.uleb128 0x6
	.4byte	.LASF21
	.byte	0x20
	.byte	0x12
	.4byte	0x12e
	.uleb128 0x2
	.4byte	.LASF14
	.byte	0x13
	.byte	0x8
	.4byte	0x77
	.byte	0
	.uleb128 0x2
	.4byte	.LASF15
	.byte	0x13
	.byte	0x10
	.4byte	0x77
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF16
	.byte	0x14
	.byte	0x8
	.4byte	0x77
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF17
	.byte	0x14
	.byte	0x13
	.4byte	0x77
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF18
	.byte	0x15
	.byte	0x8
	.4byte	0x77
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF19
	.byte	0x15
	.byte	0xf
	.4byte	0x77
	.byte	0x14
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0x16
	.byte	0x11
	.4byte	0xc2
	.byte	0x18
	.byte	0
	.uleb128 0x6
	.4byte	.LASF22
	.byte	0x20
	.byte	0x1f
	.4byte	0x182
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0x20
	.byte	0x8
	.4byte	0x77
	.byte	0
	.uleb128 0x2
	.4byte	.LASF24
	.byte	0x21
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF25
	.byte	0x22
	.byte	0x8
	.4byte	0x77
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF26
	.byte	0x23
	.byte	0x8
	.4byte	0x77
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF27
	.byte	0x24
	.byte	0x8
	.4byte	0x77
	.byte	0x10
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0x25
	.byte	0x11
	.4byte	0xc2
	.byte	0x18
	.byte	0
	.uleb128 0x12
	.4byte	.LASF93
	.byte	0x20
	.byte	0x8
	.byte	0x3
	.byte	0x65
	.byte	0x8
	.4byte	0x22f
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0x67
	.byte	0x9
	.4byte	0x60
	.byte	0
	.uleb128 0x2
	.4byte	.LASF28
	.byte	0x6a
	.byte	0x9
	.4byte	0x60
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF29
	.byte	0x6b
	.byte	0x9
	.4byte	0x60
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF30
	.byte	0x6c
	.byte	0x9
	.4byte	0x60
	.byte	0x6
	.uleb128 0x2
	.4byte	.LASF31
	.byte	0x6d
	.byte	0x9
	.4byte	0x60
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF32
	.byte	0x70
	.byte	0x8
	.4byte	0x42
	.byte	0xa
	.uleb128 0x2
	.4byte	.LASF33
	.byte	0x71
	.byte	0x8
	.4byte	0x42
	.byte	0xb
	.uleb128 0x2
	.4byte	.LASF34
	.byte	0x72
	.byte	0x8
	.4byte	0x42
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF35
	.byte	0x75
	.byte	0x8
	.4byte	0x42
	.byte	0xd
	.uleb128 0x2
	.4byte	.LASF36
	.byte	0x76
	.byte	0x8
	.4byte	0x42
	.byte	0xe
	.uleb128 0x2
	.4byte	.LASF37
	.byte	0x77
	.byte	0x8
	.4byte	0x42
	.byte	0xf
	.uleb128 0x2
	.4byte	.LASF38
	.byte	0x7a
	.byte	0x8
	.4byte	0x42
	.byte	0x10
	.uleb128 0x13
	.4byte	.LASF47
	.byte	0x3
	.byte	0x7d
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0x18
	.byte	0
	.uleb128 0x6
	.4byte	.LASF39
	.byte	0x18
	.byte	0x92
	.4byte	0x277
	.uleb128 0x9
	.ascii	"bus\000"
	.byte	0x93
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x2
	.4byte	.LASF40
	.byte	0x94
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF14
	.byte	0x95
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF41
	.byte	0x96
	.byte	0x8
	.4byte	0x77
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0x97
	.byte	0x11
	.4byte	0xc2
	.byte	0x10
	.byte	0
	.uleb128 0x6
	.4byte	.LASF42
	.byte	0x10
	.byte	0x9b
	.4byte	0x2cb
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0x9c
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x2
	.4byte	.LASF43
	.byte	0x9e
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF44
	.byte	0x9f
	.byte	0x8
	.4byte	0x60
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF45
	.byte	0xa0
	.byte	0x7
	.4byte	0x42
	.byte	0x6
	.uleb128 0x2
	.4byte	.LASF46
	.byte	0xa1
	.byte	0x7
	.4byte	0x42
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF47
	.byte	0xa3
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.4byte	.LASF48
	.byte	0x10
	.byte	0xac
	.4byte	0x2fb
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0xad
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x2
	.4byte	.LASF44
	.byte	0xae
	.byte	0x7
	.4byte	0x42
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF47
	.byte	0xaf
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.4byte	.LASF49
	.byte	0x10
	.byte	0xb5
	.4byte	0x32b
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0xb6
	.byte	0x7
	.4byte	0x42
	.byte	0
	.uleb128 0x2
	.4byte	.LASF50
	.byte	0xb7
	.byte	0x7
	.4byte	0x42
	.byte	0x1
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0xb8
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.4byte	.LASF51
	.byte	0x18
	.byte	0xbd
	.4byte	0x34e
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0xbe
	.byte	0x7
	.4byte	0x34e
	.byte	0
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0xbf
	.byte	0x11
	.4byte	0xc2
	.byte	0x10
	.byte	0
	.uleb128 0x4
	.4byte	0x42
	.4byte	0x35e
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.4byte	.LASF52
	.byte	0x10
	.byte	0xc5
	.4byte	0x381
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0xc6
	.byte	0x7
	.4byte	0x381
	.byte	0
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0xc7
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.4byte	0x42
	.4byte	0x391
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x7
	.byte	0
	.uleb128 0x14
	.byte	0x8
	.byte	0x3
	.byte	0xcd
	.byte	0x2
	.4byte	0x3a6
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0xce
	.byte	0x8
	.4byte	0x381
	.byte	0
	.byte	0
	.uleb128 0x6
	.4byte	.LASF53
	.byte	0x50
	.byte	0xca
	.4byte	0x3d5
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0xcb
	.byte	0x7
	.4byte	0x381
	.byte	0
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0xcc
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF54
	.byte	0xcf
	.byte	0x4
	.4byte	0x3d5
	.byte	0x10
	.byte	0
	.uleb128 0x4
	.4byte	0x391
	.4byte	0x3e5
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x7
	.byte	0
	.uleb128 0x6
	.4byte	.LASF55
	.byte	0x4
	.byte	0xd5
	.4byte	0x420
	.uleb128 0x2
	.4byte	.LASF50
	.byte	0xd6
	.byte	0x7
	.4byte	0x42
	.byte	0
	.uleb128 0x2
	.4byte	.LASF56
	.byte	0xd7
	.byte	0x7
	.4byte	0x42
	.byte	0x1
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0xd8
	.byte	0x7
	.4byte	0x42
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF57
	.byte	0xd9
	.byte	0x7
	.4byte	0x42
	.byte	0x3
	.byte	0
	.uleb128 0x6
	.4byte	.LASF58
	.byte	0xc8
	.byte	0xdf
	.4byte	0x45c
	.uleb128 0x2
	.4byte	.LASF59
	.byte	0xe0
	.byte	0x7
	.4byte	0x45c
	.byte	0
	.uleb128 0x2
	.4byte	.LASF50
	.byte	0xe1
	.byte	0x7
	.4byte	0x45c
	.byte	0x20
	.uleb128 0x2
	.4byte	.LASF60
	.byte	0xe2
	.byte	0x7
	.4byte	0x478
	.byte	0x40
	.uleb128 0x2
	.4byte	.LASF61
	.byte	0xe3
	.byte	0xe
	.4byte	0x488
	.byte	0xc0
	.byte	0
	.uleb128 0x4
	.4byte	0x46c
	.4byte	0x46c
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x1f
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.byte	0x8
	.4byte	.LASF62
	.uleb128 0xd
	.4byte	0x46c
	.uleb128 0x4
	.4byte	0x46c
	.4byte	0x488
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x7f
	.byte	0
	.uleb128 0xb
	.4byte	0x48d
	.uleb128 0x15
	.uleb128 0x6
	.4byte	.LASF63
	.byte	0x40
	.byte	0xe7
	.4byte	0x4b2
	.uleb128 0x2
	.4byte	.LASF50
	.byte	0xe8
	.byte	0x7
	.4byte	0x45c
	.byte	0
	.uleb128 0x2
	.4byte	.LASF64
	.byte	0xe9
	.byte	0x7
	.4byte	0x45c
	.byte	0x20
	.byte	0
	.uleb128 0x6
	.4byte	.LASF65
	.byte	0x50
	.byte	0xee
	.4byte	0x538
	.uleb128 0x2
	.4byte	.LASF23
	.byte	0xef
	.byte	0x9
	.4byte	0x60
	.byte	0
	.uleb128 0x2
	.4byte	.LASF66
	.byte	0xf1
	.byte	0x9
	.4byte	0x60
	.byte	0x2
	.uleb128 0x2
	.4byte	.LASF67
	.byte	0xf2
	.byte	0xa
	.4byte	0x60
	.byte	0x4
	.uleb128 0x2
	.4byte	.LASF68
	.byte	0xf4
	.byte	0xa
	.4byte	0x42
	.byte	0x6
	.uleb128 0x2
	.4byte	.LASF69
	.byte	0xf7
	.byte	0xa
	.4byte	0x42
	.byte	0x7
	.uleb128 0x2
	.4byte	.LASF70
	.byte	0xfa
	.byte	0xa
	.4byte	0x42
	.byte	0x8
	.uleb128 0x2
	.4byte	.LASF71
	.byte	0xfc
	.byte	0xa
	.4byte	0x538
	.byte	0xc
	.uleb128 0x2
	.4byte	.LASF72
	.byte	0xff
	.byte	0xf
	.4byte	0x548
	.byte	0x20
	.uleb128 0x1
	.4byte	.LASF47
	.2byte	0x102
	.byte	0x11
	.4byte	0xc2
	.byte	0x40
	.uleb128 0x1
	.4byte	.LASF73
	.2byte	0x103
	.byte	0xa
	.4byte	0x55d
	.byte	0x48
	.byte	0
	.uleb128 0x4
	.4byte	0x77
	.4byte	0x548
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x3
	.byte	0
	.uleb128 0x4
	.4byte	0x558
	.4byte	0x558
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x3
	.byte	0
	.uleb128 0xb
	.4byte	0x473
	.uleb128 0xb
	.4byte	0x46c
	.uleb128 0x3
	.4byte	.LASF74
	.byte	0xc0
	.2byte	0x12d
	.4byte	0x632
	.uleb128 0x1
	.4byte	.LASF75
	.2byte	0x12f
	.byte	0x11
	.4byte	0xc2
	.byte	0
	.uleb128 0x1
	.4byte	.LASF76
	.2byte	0x131
	.byte	0x8
	.4byte	0x60
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x132
	.byte	0x8
	.4byte	0x60
	.byte	0xa
	.uleb128 0x1
	.4byte	.LASF41
	.2byte	0x133
	.byte	0x8
	.4byte	0x60
	.byte	0xc
	.uleb128 0x1
	.4byte	.LASF27
	.2byte	0x134
	.byte	0x8
	.4byte	0x60
	.byte	0xe
	.uleb128 0x1
	.4byte	.LASF77
	.2byte	0x136
	.byte	0x11
	.4byte	0x632
	.byte	0x10
	.uleb128 0x1
	.4byte	.LASF78
	.2byte	0x137
	.byte	0x11
	.4byte	0x642
	.byte	0x18
	.uleb128 0x1
	.4byte	.LASF79
	.2byte	0x138
	.byte	0x11
	.4byte	0x632
	.byte	0x78
	.uleb128 0x1
	.4byte	.LASF80
	.2byte	0x139
	.byte	0x11
	.4byte	0x632
	.byte	0x80
	.uleb128 0x1
	.4byte	.LASF81
	.2byte	0x13a
	.byte	0x11
	.4byte	0x632
	.byte	0x88
	.uleb128 0x1
	.4byte	.LASF82
	.2byte	0x13b
	.byte	0x11
	.4byte	0x632
	.byte	0x90
	.uleb128 0x1
	.4byte	.LASF83
	.2byte	0x13c
	.byte	0x11
	.4byte	0x632
	.byte	0x98
	.uleb128 0x1
	.4byte	.LASF84
	.2byte	0x13d
	.byte	0x11
	.4byte	0x652
	.byte	0xa0
	.uleb128 0x1
	.4byte	.LASF85
	.2byte	0x13e
	.byte	0x11
	.4byte	0x632
	.byte	0xb0
	.uleb128 0x1
	.4byte	.LASF47
	.2byte	0x140
	.byte	0x11
	.4byte	0xc2
	.byte	0xb8
	.byte	0
	.uleb128 0x4
	.4byte	0xc2
	.4byte	0x642
	.uleb128 0x5
	.4byte	0xa4
	.byte	0
	.byte	0
	.uleb128 0x4
	.4byte	0xc2
	.4byte	0x652
	.uleb128 0x5
	.4byte	0xa4
	.byte	0xb
	.byte	0
	.uleb128 0x4
	.4byte	0xc2
	.4byte	0x662
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x1
	.byte	0
	.uleb128 0x3
	.4byte	.LASF86
	.byte	0x10
	.2byte	0x148
	.4byte	0x689
	.uleb128 0x8
	.ascii	"sig\000"
	.2byte	0x149
	.byte	0x10
	.4byte	0x689
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x14a
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.4byte	0x46c
	.4byte	0x699
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x7
	.byte	0
	.uleb128 0x3
	.4byte	.LASF87
	.byte	0x8
	.2byte	0x14f
	.4byte	0x6da
	.uleb128 0x1
	.4byte	.LASF88
	.2byte	0x150
	.byte	0x7
	.4byte	0x42
	.byte	0
	.uleb128 0x1
	.4byte	.LASF89
	.2byte	0x151
	.byte	0x7
	.4byte	0x42
	.byte	0x1
	.uleb128 0x1
	.4byte	.LASF90
	.2byte	0x152
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF91
	.2byte	0x153
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF92
	.byte	0x10
	.2byte	0x15f
	.4byte	0x71b
	.uleb128 0x1
	.4byte	.LASF18
	.2byte	0x160
	.byte	0x7
	.4byte	0x42
	.byte	0
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x161
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF15
	.2byte	0x162
	.byte	0x8
	.4byte	0x60
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x163
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0xe
	.4byte	.LASF94
	.2byte	0x167
	.4byte	0x75b
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x168
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x1
	.4byte	.LASF95
	.2byte	0x169
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF96
	.2byte	0x16a
	.byte	0x7
	.4byte	0x42
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF97
	.2byte	0x16b
	.byte	0x7
	.4byte	0x42
	.byte	0x5
	.byte	0
	.uleb128 0xe
	.4byte	.LASF98
	.2byte	0x175
	.4byte	0x79a
	.uleb128 0x1
	.4byte	.LASF99
	.2byte	0x176
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x8
	.ascii	"id\000"
	.2byte	0x177
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x8
	.ascii	"rev\000"
	.2byte	0x178
	.byte	0x7
	.4byte	0x42
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF18
	.2byte	0x179
	.byte	0x7
	.4byte	0x42
	.byte	0x5
	.byte	0
	.uleb128 0x3
	.4byte	.LASF100
	.byte	0x8
	.2byte	0x183
	.4byte	0x7c1
	.uleb128 0x1
	.4byte	.LASF15
	.2byte	0x184
	.byte	0x8
	.4byte	0x77
	.byte	0
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x185
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF101
	.byte	0x18
	.2byte	0x18c
	.4byte	0x7e8
	.uleb128 0x1
	.4byte	.LASF102
	.2byte	0x18d
	.byte	0x7
	.4byte	0xab
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x18e
	.byte	0x11
	.4byte	0xc2
	.byte	0x10
	.byte	0
	.uleb128 0x3
	.4byte	.LASF103
	.byte	0x20
	.2byte	0x19f
	.4byte	0x80f
	.uleb128 0x1
	.4byte	.LASF59
	.2byte	0x1a0
	.byte	0x7
	.4byte	0x80f
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x1a1
	.byte	0x11
	.4byte	0xc2
	.byte	0x18
	.byte	0
	.uleb128 0x4
	.4byte	0x46c
	.4byte	0x81f
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x13
	.byte	0
	.uleb128 0x3
	.4byte	.LASF104
	.byte	0x28
	.2byte	0x1a9
	.4byte	0x846
	.uleb128 0x1
	.4byte	.LASF59
	.2byte	0x1aa
	.byte	0x7
	.4byte	0x45c
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x1ab
	.byte	0x11
	.4byte	0xc2
	.byte	0x20
	.byte	0
	.uleb128 0x3
	.4byte	.LASF105
	.byte	0x50
	.2byte	0x1ce
	.4byte	0x87a
	.uleb128 0xf
	.4byte	.LASF106
	.2byte	0x1cf
	.4byte	0x3b
	.byte	0x7
	.byte	0
	.uleb128 0xf
	.4byte	.LASF107
	.2byte	0x1d0
	.4byte	0x3b
	.byte	0x1
	.byte	0x7
	.uleb128 0x1
	.4byte	.LASF108
	.2byte	0x1d1
	.byte	0x7
	.4byte	0x87a
	.byte	0x1
	.byte	0
	.uleb128 0x4
	.4byte	0x46c
	.4byte	0x88a
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x4e
	.byte	0
	.uleb128 0x16
	.4byte	.LASF109
	.2byte	0x158
	.byte	0x3
	.2byte	0x1d4
	.byte	0x8
	.4byte	0x8d0
	.uleb128 0x1
	.4byte	.LASF110
	.2byte	0x1d5
	.byte	0x8
	.4byte	0x8e9
	.byte	0
	.uleb128 0x1
	.4byte	.LASF111
	.2byte	0x1d6
	.byte	0xe
	.4byte	0x558
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF112
	.2byte	0x1d7
	.byte	0x16
	.4byte	0x8ee
	.byte	0x10
	.uleb128 0x17
	.4byte	.LASF20
	.byte	0x3
	.2byte	0x1d8
	.byte	0x8
	.4byte	0x8fe
	.2byte	0x150
	.byte	0
	.uleb128 0xd
	.4byte	0x88a
	.uleb128 0x18
	.4byte	0xbb
	.4byte	0x8e4
	.uleb128 0x19
	.4byte	0x8e4
	.byte	0
	.uleb128 0xb
	.4byte	0x8d0
	.uleb128 0xb
	.4byte	0x8d5
	.uleb128 0x4
	.4byte	0x846
	.4byte	0x8fe
	.uleb128 0x5
	.4byte	0xa4
	.byte	0x3
	.byte	0
	.uleb128 0x1a
	.byte	0x8
	.uleb128 0x3
	.4byte	.LASF113
	.byte	0x20
	.2byte	0x1e8
	.4byte	0x927
	.uleb128 0x1
	.4byte	.LASF59
	.2byte	0x1e9
	.byte	0x7
	.4byte	0x80f
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x1ea
	.byte	0x11
	.4byte	0xc2
	.byte	0x18
	.byte	0
	.uleb128 0x3
	.4byte	.LASF114
	.byte	0x8
	.2byte	0x202
	.4byte	0x94e
	.uleb128 0x1
	.4byte	.LASF115
	.2byte	0x203
	.byte	0x8
	.4byte	0x77
	.byte	0
	.uleb128 0x1
	.4byte	.LASF116
	.2byte	0x204
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF117
	.byte	0x10
	.2byte	0x207
	.4byte	0x974
	.uleb128 0x8
	.ascii	"id\000"
	.2byte	0x208
	.byte	0x8
	.4byte	0x77
	.byte	0
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x209
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF118
	.byte	0x10
	.2byte	0x211
	.4byte	0x9c2
	.uleb128 0x1
	.4byte	.LASF119
	.2byte	0x212
	.byte	0x11
	.4byte	0x59
	.byte	0
	.uleb128 0x1
	.4byte	.LASF120
	.2byte	0x212
	.byte	0x1e
	.4byte	0x59
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x213
	.byte	0x11
	.4byte	0x59
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF69
	.2byte	0x213
	.byte	0x19
	.4byte	0x59
	.byte	0x6
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x214
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF121
	.byte	0x10
	.2byte	0x21f
	.4byte	0x9f5
	.uleb128 0x8
	.ascii	"id\000"
	.2byte	0x220
	.byte	0x10
	.4byte	0x2a
	.byte	0
	.uleb128 0x1
	.4byte	.LASF122
	.2byte	0x221
	.byte	0x10
	.4byte	0x2a
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF61
	.2byte	0x222
	.byte	0xa
	.4byte	0x8fe
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF123
	.byte	0x1
	.2byte	0x229
	.4byte	0xa0f
	.uleb128 0x1
	.4byte	.LASF50
	.2byte	0x22a
	.byte	0x7
	.4byte	0x42
	.byte	0
	.byte	0
	.uleb128 0x3
	.4byte	.LASF124
	.byte	0x10
	.2byte	0x237
	.4byte	0xa5d
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x238
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x1
	.4byte	.LASF125
	.2byte	0x239
	.byte	0x8
	.4byte	0x60
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF126
	.2byte	0x23a
	.byte	0x8
	.4byte	0x60
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF127
	.2byte	0x23b
	.byte	0x8
	.4byte	0x60
	.byte	0x6
	.uleb128 0x1
	.4byte	.LASF20
	.2byte	0x23c
	.byte	0x11
	.4byte	0xc2
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF128
	.byte	0x2
	.2byte	0x24c
	.4byte	0xa77
	.uleb128 0x1
	.4byte	.LASF127
	.2byte	0x24d
	.byte	0x8
	.4byte	0x60
	.byte	0
	.byte	0
	.uleb128 0x3
	.4byte	.LASF129
	.byte	0xc
	.2byte	0x252
	.4byte	0xaab
	.uleb128 0x1
	.4byte	.LASF130
	.2byte	0x253
	.byte	0x8
	.4byte	0x42
	.byte	0
	.uleb128 0x1
	.4byte	.LASF14
	.2byte	0x254
	.byte	0x8
	.4byte	0x77
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF15
	.2byte	0x255
	.byte	0x8
	.4byte	0x77
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF131
	.byte	0x28
	.2byte	0x25b
	.4byte	0xad2
	.uleb128 0x1
	.4byte	.LASF59
	.2byte	0x25c
	.byte	0x7
	.4byte	0x45c
	.byte	0
	.uleb128 0x1
	.4byte	.LASF47
	.2byte	0x25d
	.byte	0x11
	.4byte	0xc2
	.byte	0x20
	.byte	0
	.uleb128 0x3
	.4byte	.LASF132
	.byte	0x8
	.2byte	0x26e
	.4byte	0xb13
	.uleb128 0x8
	.ascii	"did\000"
	.2byte	0x26f
	.byte	0x8
	.4byte	0x60
	.byte	0
	.uleb128 0x8
	.ascii	"vid\000"
	.2byte	0x26f
	.byte	0xd
	.4byte	0x60
	.byte	0x2
	.uleb128 0x1
	.4byte	.LASF133
	.2byte	0x270
	.byte	0x8
	.4byte	0x60
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF134
	.2byte	0x270
	.byte	0x11
	.4byte	0x60
	.byte	0x6
	.byte	0
	.uleb128 0x1b
	.4byte	.LASF136
	.byte	0x1
	.byte	0x8
	.byte	0x5
	.4byte	0xbb
	.8byte	.LFB0
	.8byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 2
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x88
	.uleb128 0x21
	.sleb128 2
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 16
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0x6b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x88
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x88
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.LFB0
	.8byte	.LFE0-.LFB0
	.8byte	0
	.8byte	0
	.section	.debug_rnglists,"",@progbits
.Ldebug_ranges0:
	.4byte	.Ldebug_ranges3-.Ldebug_ranges2
.Ldebug_ranges2:
	.2byte	0x5
	.byte	0x8
	.byte	0
	.4byte	0
.LLRL0:
	.byte	0x7
	.8byte	.LFB0
	.uleb128 .LFE0-.LFB0
	.byte	0
.Ldebug_ranges3:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF53:
	.ascii	"pnp_card_device_id\000"
.LASF36:
	.ascii	"bInterfaceSubClass\000"
.LASF11:
	.ascii	"size_t\000"
.LASF43:
	.ascii	"cu_type\000"
.LASF66:
	.ascii	"manf_id\000"
.LASF132:
	.ascii	"rio_device_id\000"
.LASF64:
	.ascii	"compat\000"
.LASF21:
	.ascii	"pci_device_id\000"
.LASF96:
	.ascii	"revision\000"
.LASF107:
	.ascii	"exact_match\000"
.LASF31:
	.ascii	"bcdDevice_hi\000"
.LASF38:
	.ascii	"bInterfaceNumber\000"
.LASF124:
	.ascii	"x86_cpu_id\000"
.LASF8:
	.ascii	"long long unsigned int\000"
.LASF76:
	.ascii	"bustype\000"
.LASF6:
	.ascii	"__u16\000"
.LASF109:
	.ascii	"dmi_system_id\000"
.LASF42:
	.ascii	"ccw_device_id\000"
.LASF15:
	.ascii	"device\000"
.LASF10:
	.ascii	"long long int\000"
.LASF45:
	.ascii	"cu_model\000"
.LASF16:
	.ascii	"subvendor\000"
.LASF69:
	.ascii	"function\000"
.LASF126:
	.ascii	"model\000"
.LASF81:
	.ascii	"mscbit\000"
.LASF40:
	.ascii	"group\000"
.LASF118:
	.ascii	"isapnp_device_id\000"
.LASF52:
	.ascii	"pnp_device_id\000"
.LASF41:
	.ascii	"product\000"
.LASF26:
	.ascii	"specifier_id\000"
.LASF24:
	.ascii	"vendor_id\000"
.LASF104:
	.ascii	"spi_device_id\000"
.LASF80:
	.ascii	"absbit\000"
.LASF85:
	.ascii	"swbit\000"
.LASF67:
	.ascii	"card_id\000"
.LASF25:
	.ascii	"model_id\000"
.LASF22:
	.ascii	"ieee1394_device_id\000"
.LASF20:
	.ascii	"driver_data\000"
.LASF19:
	.ascii	"class_mask\000"
.LASF72:
	.ascii	"prod_id\000"
.LASF122:
	.ascii	"mask\000"
.LASF117:
	.ascii	"zorro_device_id\000"
.LASF129:
	.ascii	"ipack_device_id\000"
.LASF94:
	.ascii	"ssb_device_id\000"
.LASF127:
	.ascii	"feature\000"
.LASF128:
	.ascii	"cpu_feature\000"
.LASF2:
	.ascii	"unsigned int\000"
.LASF74:
	.ascii	"input_device_id\000"
.LASF63:
	.ascii	"vio_device_id\000"
.LASF33:
	.ascii	"bDeviceSubClass\000"
.LASF82:
	.ascii	"ledbit\000"
.LASF89:
	.ascii	"hversion_rev\000"
.LASF112:
	.ascii	"matches\000"
.LASF4:
	.ascii	"__u8\000"
.LASF135:
	.ascii	"GNU C11 13.0.0 20220906 (experimental) -G 0 -mexplicit-r"
	.ascii	"elocs -mno-check-zero-division -mdirect-extern-access -m"
	.ascii	"abi=lp64s -march=loongarch64 -mfpu=none -mcmodel=normal "
	.ascii	"-mtune=la464 -g -O0 -O2 -Os -std=gnu11 -fno-strict-alias"
	.ascii	"ing -fno-common -fshort-wchar -funsigned-char -fno-async"
	.ascii	"hronous-unwind-tables -fno-delete-null-pointer-checks -f"
	.ascii	"no-allow-store-data-races -fstack-protector-strong -ftri"
	.ascii	"vial-auto-var-init=zero -fno-stack-clash-protection -fst"
	.ascii	"ack-check=no -fpie -ffreestanding -fno-stack-protector -"
	.ascii	"fomit-frame-pointer -fno-strict-overflow -fconserve-stac"
	.ascii	"k -fno-tree-scev-cprop\000"
.LASF7:
	.ascii	"__u32\000"
.LASF120:
	.ascii	"card_device\000"
.LASF59:
	.ascii	"name\000"
.LASF61:
	.ascii	"data\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF98:
	.ascii	"bcma_device_id\000"
.LASF50:
	.ascii	"type\000"
.LASF97:
	.ascii	"__pad\000"
.LASF46:
	.ascii	"dev_model\000"
.LASF110:
	.ascii	"callback\000"
.LASF106:
	.ascii	"slot\000"
.LASF58:
	.ascii	"of_device_id\000"
.LASF133:
	.ascii	"asm_did\000"
.LASF108:
	.ascii	"substr\000"
.LASF92:
	.ascii	"sdio_device_id\000"
.LASF70:
	.ascii	"device_no\000"
.LASF119:
	.ascii	"card_vendor\000"
.LASF100:
	.ascii	"virtio_device_id\000"
.LASF13:
	.ascii	"kernel_ulong_t\000"
.LASF27:
	.ascii	"version\000"
.LASF113:
	.ascii	"platform_device_id\000"
.LASF95:
	.ascii	"coreid\000"
.LASF39:
	.ascii	"hid_device_id\000"
.LASF93:
	.ascii	"usb_device_id\000"
.LASF101:
	.ascii	"hv_vmbus_device_id\000"
.LASF131:
	.ascii	"mei_cl_device_id\000"
.LASF103:
	.ascii	"i2c_device_id\000"
.LASF115:
	.ascii	"phy_id\000"
.LASF88:
	.ascii	"hw_type\000"
.LASF47:
	.ascii	"driver_info\000"
.LASF71:
	.ascii	"prod_id_hash\000"
.LASF18:
	.ascii	"class\000"
.LASF30:
	.ascii	"bcdDevice_lo\000"
.LASF48:
	.ascii	"ap_device_id\000"
.LASF83:
	.ascii	"sndbit\000"
.LASF125:
	.ascii	"family\000"
.LASF134:
	.ascii	"asm_vid\000"
.LASF3:
	.ascii	"unsigned char\000"
.LASF35:
	.ascii	"bInterfaceClass\000"
.LASF68:
	.ascii	"func_id\000"
.LASF84:
	.ascii	"ffbit\000"
.LASF9:
	.ascii	"short int\000"
.LASF87:
	.ascii	"parisc_device_id\000"
.LASF60:
	.ascii	"compatible\000"
.LASF105:
	.ascii	"dmi_strmatch\000"
.LASF34:
	.ascii	"bDeviceProtocol\000"
.LASF65:
	.ascii	"pcmcia_device_id\000"
.LASF130:
	.ascii	"format\000"
.LASF57:
	.ascii	"proto\000"
.LASF116:
	.ascii	"phy_id_mask\000"
.LASF37:
	.ascii	"bInterfaceProtocol\000"
.LASF12:
	.ascii	"long unsigned int\000"
.LASF17:
	.ascii	"subdevice\000"
.LASF62:
	.ascii	"char\000"
.LASF32:
	.ascii	"bDeviceClass\000"
.LASF55:
	.ascii	"serio_device_id\000"
.LASF14:
	.ascii	"vendor\000"
.LASF77:
	.ascii	"evbit\000"
.LASF79:
	.ascii	"relbit\000"
.LASF123:
	.ascii	"mips_cdmm_device_id\000"
.LASF23:
	.ascii	"match_flags\000"
.LASF44:
	.ascii	"dev_type\000"
.LASF51:
	.ascii	"acpi_device_id\000"
.LASF73:
	.ascii	"cisfile\000"
.LASF121:
	.ascii	"amba_id\000"
.LASF111:
	.ascii	"ident\000"
.LASF91:
	.ascii	"sversion\000"
.LASF29:
	.ascii	"idProduct\000"
.LASF78:
	.ascii	"keybit\000"
.LASF90:
	.ascii	"hversion\000"
.LASF75:
	.ascii	"flags\000"
.LASF86:
	.ascii	"eisa_device_id\000"
.LASF56:
	.ascii	"extra\000"
.LASF49:
	.ascii	"css_device_id\000"
.LASF54:
	.ascii	"devs\000"
.LASF136:
	.ascii	"main\000"
.LASF114:
	.ascii	"mdio_device_id\000"
.LASF28:
	.ascii	"idVendor\000"
.LASF99:
	.ascii	"manuf\000"
.LASF102:
	.ascii	"guid\000"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.ascii	"scripts/mod/devicetable-offsets.c\000"
.LASF1:
	.ascii	"/home/huqingwei/repositories/XOS-loongarch64\000"
	.ident	"GCC: (GNU) 13.0.0 20220906 (experimental)"
	.section	.note.GNU-stack,"",@progbits
