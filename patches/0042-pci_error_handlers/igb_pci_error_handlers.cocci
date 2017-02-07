@@
identifier s;
@@

static
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
const
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0) */
struct pci_error_handlers s = { ... };
