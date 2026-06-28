#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/kernel.h>

#define SECRET_MAX 256

static char secret_buf[SECRET_MAX];
static unsigned long secret_len;
static DEFINE_MUTEX(secret_lock);

SYSCALL_DEFINE2(set_secret, const char __user *, buf, unsigned long, len) {
	if (len > SECRET_MAX)
		return -EINVAL;

	mutex_lock(&secret_lock);
	if (copy_from_user(secret_buf, buf, len)) {
		mutex_unlock(&secret_lock);
		return -EFAULT;
	}
	
	secret_len = len;
	mutex_unlock(&secret_lock);

	pr_info("set_secret: %lu bytes armazenados\n", len);
	return 0;
}

SYSCALL_DEFINE2(get_secret, char __user *, buf, unsigned long, len) {
	unsigned long n;
	int ret = 0;

	mutex_lock(&secret_lock);
	n = min(len, secret_len);
	if (copy_to_user(buf, secret_buf, n))
		ret = -EFAULT;
		
	mutex_unlock(&secret_lock);

	if (ret)
		return ret;

	pr_info("get_secret: %lu bytes retornados\n", n);
	return n;
}
