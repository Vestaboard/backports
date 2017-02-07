@r1@
struct sk_buff *skb;
expression E1;
@@
 if (E1
+#if LINUX_VERSION_IS_GEQ(3,18,0)
 || !skb->xmit_more
+#endif /* if LINUX_VERSION_IS_GEQ(3,18,0) */
 ) {...}
