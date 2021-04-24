#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");

#define MMAP_DEV_MAJOR 44

struct cdev cdev;
unsigned long *mem;

static int mmap_dev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int mmap_dev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static int mmap_dev_mmap(struct file *filep, struct vm_area_struct *vma)
{
	unsigned long len = vma->vm_end - vma->vm_start;
	unsigned long pfn;
	int ret ;

	if (len > PAGE_SIZE) {
		pr_err("vm_end[%lu] - vm_start[%lu] [%lu] > 4096\n",
		       vma->vm_end, vma->vm_start, len);
		return -EINVAL;
	}

	pfn = virt_to_phys((void *)mem) >> PAGE_SHIFT;
	ret = remap_pfn_range(vma, vma->vm_start, pfn, len, vma->vm_page_prot);
	if (ret < 0) {
		pr_err("could not map the address area\n");
		return -EIO;
	}

	return 0;
}

const struct file_operations mmap_dev_fops = {
	.owner = THIS_MODULE,
	.open = mmap_dev_open,
	.release = mmap_dev_release,
	.mmap = mmap_dev_mmap,
};

int init_module(void)
{
	int err;

	err = register_chrdev_region(MKDEV(MMAP_DEV_MAJOR, 0), 1,
				     "mmap_device_driver");
	if (err)
		return err;

	mem = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!mem)
		return -ENOMEM;

	SetPageReserved(virt_to_page(mem));

	pr_info(KBUILD_MODNAME ": init value (offset 0x0): 0x%lx.\n",
		*(unsigned long*)mem);

	cdev_init(&cdev, &mmap_dev_fops);
	cdev_add(&cdev, MKDEV(MMAP_DEV_MAJOR, 0), 1);

	pr_info(KBUILD_MODNAME ": loaded.\n");
	return 0;
}

void cleanup_module(void)
{
	pr_info(KBUILD_MODNAME ": exit value (offset 0x0): 0x%lx.\n",
		*(unsigned long*)mem);

	ClearPageReserved(virt_to_page(((unsigned long)mem)));
	kfree(mem);

	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(MMAP_DEV_MAJOR, 0), 1);

	pr_info(KBUILD_MODNAME ": unloaded.\n");
}
