#ifndef __BP_USB_CDC_H
#define __BP_USB_CDC_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(4,8,0)
#include_next <linux/usb/cdc.h>
#else
#include <uapi/linux/usb/cdc.h>

/*
 * inofficial magic numbers
 */

#define CDC_PHONET_MAGIC_NUMBER		0xAB

/*
 * parsing CDC headers
 */

struct usb_cdc_parsed_header {
	struct usb_cdc_union_desc *usb_cdc_union_desc;
	struct usb_cdc_header_desc *usb_cdc_header_desc;

	struct usb_cdc_call_mgmt_descriptor *usb_cdc_call_mgmt_descriptor;
	struct usb_cdc_acm_descriptor *usb_cdc_acm_descriptor;
	struct usb_cdc_country_functional_desc *usb_cdc_country_functional_desc;
	struct usb_cdc_network_terminal_desc *usb_cdc_network_terminal_desc;
	struct usb_cdc_ether_desc *usb_cdc_ether_desc;
	struct usb_cdc_dmm_desc *usb_cdc_dmm_desc;
	struct usb_cdc_mdlm_desc *usb_cdc_mdlm_desc;
	struct usb_cdc_mdlm_detail_desc *usb_cdc_mdlm_detail_desc;
	struct usb_cdc_obex_desc *usb_cdc_obex_desc;
	struct usb_cdc_ncm_desc *usb_cdc_ncm_desc;
	struct usb_cdc_mbim_desc *usb_cdc_mbim_desc;
	struct usb_cdc_mbim_extended_desc *usb_cdc_mbim_extended_desc;

	bool phonet_magic_present;
};

#define cdc_parse_cdc_header LINUX_BACKPORT(cdc_parse_cdc_header)
int cdc_parse_cdc_header(struct usb_cdc_parsed_header *hdr,
			 struct usb_interface *intf,
			 u8 *buffer, int buflen);
#endif

#endif /* __BP_USB_CDC_H */
