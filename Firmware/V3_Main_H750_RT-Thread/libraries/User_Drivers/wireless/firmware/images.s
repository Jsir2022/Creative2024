	AREA	ExtFlashSection, DATA, READONLY

	EXPORT	wireless_wifi_image
	EXPORT	wireless_bt_image

wireless_wifi_image
	INCBIN	ap6212-wifi-image-1.0.rbl
		
wireless_bt_image
	INCBIN	ap6212-bt-image-1.0.rbl
		
	END
