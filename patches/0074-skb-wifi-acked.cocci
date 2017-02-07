@@
expression SKB;
expression A, V;
@@
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,3,0)
SKB->wifi_acked_valid = V;
SKB->wifi_acked = A;
+#endif
