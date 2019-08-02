@@
struct genl_info *info;
@@
-info->extack
+genl_info_extack(info)

@@
struct genl_info *info;
@@
-info->userhdr
+genl_info_userhdr(info)

@@
struct netlink_callback *cb;
@@
-cb->extack
+genl_callback_extack(cb)
