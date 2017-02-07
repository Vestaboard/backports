@r1@
expression E1;
struct ethtool_cmd *ecmd;
@@
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
 ecmd->eth_tp_mdix_ctrl = E1;
+#endif /* if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0) */

@r2@
struct ethtool_cmd *ecmd;
@@
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
 if (ecmd->eth_tp_mdix_ctrl) {...}
+#endif /* if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0) */
