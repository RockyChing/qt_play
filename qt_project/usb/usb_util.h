/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/12
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef _USB_UTIL_H
#define _USB_UTIL_H
#include "libusb.h"

class UsbUtil {
public:
    UsbUtil(uint16_t vid, uint16_t pid);
	~UsbUtil();

public:
    usb_devices_list();

private:
	void usb_devices_print(libusb_device **devs);

private:
	bool mLibusbInit;
    int mDdevice;
};

#endif /* _USB_UTIL_H */

