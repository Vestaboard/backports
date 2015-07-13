/* c5384048 */

@ vb2_mem_ops @
identifier s, dma_fn;
identifier dma_op =~ "(get_dma|map_dma|unmap_dma|attach_dma|detach_dma)";
@@

 struct vb2_mem_ops s = {
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
 	.dma_op = dma_fn,
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) */
};

@ mod_dma_fn depends on vb2_mem_ops @
identifier vb2_mem_ops.dma_fn;
@@

+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
 dma_fn(...)
 {
 	...
 }
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) */

@ v4l_ioctl_ops @
identifier s, ioctl_fn;
identifier ioctl_op =~ "(vidioc_expbuf)";
@@
struct v4l2_ioctl_ops s = {
+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
	.ioctl_op = ioctl_fn,
+#endif
};

@ mod_ioctl_fn depends on v4l_ioctl_ops @
identifier v4l_ioctl_ops.ioctl_fn;
@@

+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
 ioctl_fn(...)
 {
 	...
 }
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) */

@ dma_buf_op_add_ifdef @
identifier s, dma_fn;
@@

+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
 struct dma_buf_ops s = {
 ...
};
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) */

@ dma_buf_op @
identifier s, dma_fn;
identifier dma_op;
@@

 struct dma_buf_ops s = {
	.dma_op = dma_fn,
};

@ modify_dma_fn depends on dma_buf_op @
identifier dma_buf_op.dma_fn;
@@

+#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
 dma_fn(...)
 {
 	...
 }
+#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) */

