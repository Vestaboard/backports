--- a/drivers/net/wireless/ti/wlcore/main.c
+++ b/drivers/net/wireless/ti/wlcore/main.c
@@ -6054,13 +6054,24 @@
 	wl->platform_quirks = pdata->platform_quirks;
 	wl->if_ops = pdev_data->if_ops;
 
+#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
+	irqflags = IRQF_TRIGGER_RISING;
+#else
 	if (wl->platform_quirks & WL12XX_PLATFORM_QUIRK_EDGE_IRQ)
 		irqflags = IRQF_TRIGGER_RISING;
 	else
 		irqflags = IRQF_TRIGGER_HIGH | IRQF_ONESHOT;
+#endif
 
+#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
+	ret = compat_request_threaded_irq(&wl->irq_compat, wl->irq,
+					  NULL, wlcore_irq,
+					  irqflags,
+					  pdev->name, wl);
+#else
 	ret = request_threaded_irq(wl->irq, NULL, wlcore_irq,
 				   irqflags, pdev->name, wl);
+#endif
 	if (ret < 0) {
 		wl1271_error("request_irq() failed: %d", ret);
 		goto out_free_nvs;
@@ -6135,7 +6146,11 @@
 	wl1271_unregister_hw(wl);
 
 out_irq:
+#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
+	compat_free_threaded_irq(&wl->irq_compat);
+#else
 	free_irq(wl->irq, wl);
+#endif
 
 out_free_nvs:
 	kfree(wl->nvs);
@@ -6181,7 +6196,12 @@
 		disable_irq_wake(wl->irq);
 	}
 	wl1271_unregister_hw(wl);
+#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
+	compat_free_threaded_irq(&wl->irq_compat);
+	compat_destroy_threaded_irq(&wl->irq_compat);
+#else
 	free_irq(wl->irq, wl);
+#endif
 	wlcore_free_hw(wl);
 
 	return 0;