@r@
identifier OPS;
identifier stats64_fn;
fresh identifier stats64_fn_wrap = "bp_" ## stats64_fn;
position p;
@@
struct net_device_ops OPS@p = {
+#if LINUX_VERSION_IS_GEQ(4,11,0)
	.ndo_get_stats64 = stats64_fn,
+#else
+	.ndo_get_stats64 = stats64_fn_wrap,
+#endif
};

@@
identifier r.stats64_fn_wrap;
identifier r.stats64_fn;
@@
void stats64_fn(...) {...}
+#if LINUX_VERSION_IS_LESS(4,11,0)
+static struct rtnl_link_stats64 *
+stats64_fn_wrap(struct net_device *dev,
+		 struct rtnl_link_stats64 *stats)
+{
+	stats64_fn(dev, stats);
+	return stats;
+}
+#endif
