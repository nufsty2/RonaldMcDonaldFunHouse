# See README file for instructions
WORKDIR = build
SRCDIR = dts
BINDIR = dtb
BOOTDIR = boot

RELEASE ?= Pynq-Z2-Xenial
SD_BUILD = sdbuild

include ${SD_BUILD}/releases/${RELEASE}.config

# BOOT_CONFIG_DIR := $(realpath ${SD_BUILD}/boot_configs/${BOOT_CONFIG})
BOOT_CONFIG_DIR := $(realpath ${SD_BUILD}/boot_configs/Pynq-Z2-defconfig)
BINDIR_ABS := $(realpath ${BINDIR})


BOARD_PART = xc7z020clg400-1
PS_CONFIG_TCL ?= ${BOOT_CONFIG_DIR}/ps7_config.tcl
BOARD_CONSTRAINTS ?= ${BOOT_CONFIG_DIR}/constraints.xdc
BOARD_DTSI ?= ${BOOT_CONFIG_DIR}/pynq.dtsi
DTC_DIR = dtc-1.4.4
DTC = ${DTC_DIR}/dtc

# Relative to this directory
SCRIPTDIR = $(realpath ${SD_BUILD}/boot_configs/common)

export BOARD_PART
export PS_CONFIG_TCL
export BOARD_CONSTRAINTS

GIT_CLONE_CHECKOUT = ${SD_BUILD}/scripts/git_clone_checkout
DTC_REPO := https://github.com/Xilinx/device-tree-xlnx.git
DTC_COMMIT := 11f81055d1afad67398fa5ef443b32be8bc74433

UBOOT_REPO ?= https://github.com/Digilent/u-boot-digilent.git
UBOOT_COMMIT ?= e2382ceaffc0c36dc21c01576c1e454af0e01ca1
UBOOT_CONFIG ?= ${BOOT_CONFIG_DIR}/u-boot.config
UBOOT_MAKE_ARGS ?= CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm -j4

BOOT_BITSTREAM ?= ../vivado/ecen427.bit

DTO_DTS = $(wildcard dtso/*.dts)
DTO_DTB = $(patsubst %.dts,%.dtbo,$(patsubst dtso/%,dtbo/%,${DTO_DTS}))

source: ${WORKDIR}/pynq_dts/system.dts
binary: ${BINDIR}/devicetree.dtb
decompile: ${BINDIR}/decompiled.dts
bootimg: ${BOOTDIR}/BOOT.bin
overlays: ${DTC_DIR}/dtc ${DTO_DTB}

${DTC}:
	cd ${DTC_DIR} && make

dtbo/%.dtbo: dtso/%.dts
	${DTC} -@ -I dts -O dtb -o $@ $<

${BINDIR}/decompiled.dts: ${BINDIR}/devicetree.dtb | ${DTC}
	${DTC} -I dtb -O dts -o $@ $<


${BINDIR}/devicetree.dtb: SHELL := /bin/bash
${BINDIR}/devicetree.dtb: $(shell find ${SRCDIR})
	cd ${SRCDIR} && cat system.dts <(echo '/include/ "board.dtsi"') | ../${DTC} -@ -I dts -O dtb -o ../dtb/devicetree.dtb
	
# ${WORKDIR}/pynq_dts/devicetree.dtb: ${WORKDIR}/pynq_dts/system.dts ${WORKDIR}/pynq_dts/board.dtsi | ${DTC}
# 	#cd ${WORKDIR}/pynq_dts && bash ${SCRIPTDIR}/compile_dtc.sh > devicetree.dtb
# 	cd ${WORKDIR}/pynq_dts && cat system.dts <(echo '/include/ "board.dtsi"') | ${DTC} -I dts -O dtb -o devicetree.dtb

${WORKDIR}:
	mkdir ${WORKDIR}

${WORKDIR}/pynq.hdf: | ${WORKDIR}
	cd ${WORKDIR} && vivado -mode batch -source ${SCRIPTDIR}/create_zynq_hdf.tcl

${WORKDIR}/device-tree-xlnx: | ${WORKDIR}
	${GIT_CLONE_CHECKOUT} ${DTC_REPO} ${DTC_COMMIT} ${WORKDIR}/device-tree-xlnx

${WORKDIR}/pynq_dts/board.dtsi: ${BOARD_DTSI}
	cp $< $@

${WORKDIR}/pynq_dts/system.dts: ${WORKDIR}/pynq.hdf | ${WORKDIR}/device-tree-xlnx
	cd ${WORKDIR} && hsi -mode batch -source ${SCRIPTDIR}/create_zynq_devicetree.tcl


# Boot image
${BOOTDIR}/BOOT.bin: ${SCRIPTDIR}/zynq.bif ${WORKDIR}/fsbl.elf ${WORKDIR}/u-boot.elf ${WORKDIR}/bitstream.bit | ${BOOTDIR}
	cd ${WORKDIR} && bootgen -image ${SCRIPTDIR}/zynq.bif -o $(realpath ${BOOTDIR})/BOOT.bin -w

${WORKDIR}/fsbl.elf: ${WORKDIR}/fsbl/executable.elf
	cp $< $@

${WORKDIR}/fsbl/executable.elf: ${WORKDIR}/pynq.hdf
	cd ${WORKDIR} && hsi -mode batch -source ${SCRIPTDIR}/create_zynq_fsbl.tcl

${BOOTDIR}:
	mkdir ${BOOTDIR}

${WORKDIR}/u-boot.elf: ${WORKDIR}/u-boot/u-boot
	cp $< $@

${WORKDIR}/u-boot:
	${GIT_CLONE_CHECKOUT} ${UBOOT_REPO} ${UBOOT_COMMIT} ${WORKDIR}/u-boot

${WORKDIR}/u-boot/u-boot: ${WORKDIR}/u-boot/.config
	cd ${WORKDIR}/u-boot && make ${UBOOT_MAKE_ARGS} u-boot

${WORKDIR}/u-boot/.config: ${UBOOT_CONFIG} | ${WORKDIR}/u-boot
	cp $< $@

${WORKDIR}/bitstream.bit: ${BOOT_BITSTREAM} 
	cp $< $@

clean:
	rm -rf ${WORKDIR}
