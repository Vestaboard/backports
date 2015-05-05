@nd@
identifier dev;
@@
struct net_device *dev;
@@
identifier nd.dev;
@@
-dev->tstats
+netdev_tstats(dev)
