--- a/net/mac80211/iface.c
+++ b/net/mac80211/iface.c
@@ -1643,6 +1643,7 @@
  * Remove all interfaces, may only be called at hardware unregistration
  * time because it doesn't do RCU-safe list removals.
  */
+#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33))
 void ieee80211_remove_interfaces(struct ieee80211_local *local)
 {
 	struct ieee80211_sub_if_data *sdata, *tmp;
@@ -1670,6 +1671,22 @@
 		kfree(sdata);
 	}
 }
+#else
+void ieee80211_remove_interfaces(struct ieee80211_local *local)
+{
+	struct ieee80211_sub_if_data *sdata, *tmp;
+
+	ASSERT_RTNL();
+
+	list_for_each_entry_safe(sdata, tmp, &local->interfaces, list) {
+		mutex_lock(&local->iflist_mtx);
+		list_del(&sdata->list);
+		mutex_unlock(&local->iflist_mtx);
+
+		unregister_netdevice(sdata->dev);
+	}
+}
+#endif
 
 static int netdev_notify(struct notifier_block *nb,
 			 unsigned long state,