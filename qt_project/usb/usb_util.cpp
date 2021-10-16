/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/12
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#define LOG_TAG "usb"
#include "log/Log.h"

#include "usb_util.h"



UsbUtil::UsbUtil(uint16_t vid, uint16_t pid): mLibusbInit(false)
{
	int r;
	r = libusb_init(NULL);
	if (r < 0) {
        log_debug("libusb_init failed!");
		return;
	}

	mLibusbInit = true;
	if (0 == vid && 0 == pid)
    	usb_devices_list();
}

UsbUtil::~UsbUtil()
{
	if (mLibusbInit)
		libusb_exit(NULL);
}

void UsbUtil::usb_devices_print(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8];

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
            log_error("failed to get device descriptor");
			return;
		}

        log_info("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
            log_info(" path: %d", path[0]);
			for (j = 1; j < r; j++)
                log_info(".%d", path[j]);
		}
        log_info("\n");
	}
}

int UsbUtil::usb_devices_list()
{
	libusb_device **devs;
	ssize_t cnt;

	if (!mLibusbInit)
		return -1;
    cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0) {
		return (int) cnt;
	}

	usb_devices_print(devs);
	libusb_free_device_list(devs, 1);
    return 0;
}
