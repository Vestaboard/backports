@nd@
identifier dev;
@@
struct net_device *dev;
@@
identifier nd.dev;
expression E;
@@
-dev->tstats = E;
+netdev_assign_tstats(dev, E);
