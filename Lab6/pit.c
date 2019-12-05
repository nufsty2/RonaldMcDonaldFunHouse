#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/sysfs.h>
#include <linux/kobject.h> 
#include <linux/string.h>

/* MODULES */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronald McDonald");
MODULE_DESCRIPTION("ECEn 427 PIT Driver");
#define MODULE_NAME "pit" 

/* Error and success code defines */
#define PIT_INIT_SUCCESS 0
#define PIT_INIT_ALLOC_CHRDEV_REGION_FAIL -1
#define PIT_INIT_ALLOC_PLATFORM_DRIVER_REG_FAIL -2
#define PIT_PROBE_SUCCESS 0
#define PIT_PROBE_CDEV_ADD_FAIL -3

#define DEV_NUM 1

/* PIT struct attributes */
#define PIT_PHYS_ADDR 0x43C30000
#define PIT_MEM_SIZE 0x10000

#define SLV_REG0 0
#define SLV_REG1 1
#define WORD_SIZE 4

// Function declarations for the kernal
static int pit_init(void);
static void pit_exit(void);
static int pit_probe(struct platform_device *pdev);
static int pit_remove(struct platform_device * pdev); 
static long pit_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

// Init and exit declarations 
module_init(pit_init);
module_exit(pit_exit);

// SYSFS stuff
volatile int period_val = 0;
volatile int start_val = 0;
volatile int int_val = 0;
static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count);

struct kobject *pit_obj;
static struct kobj_attribute pit_period = __ATTR(period_val, 0660, sysfs_show, sysfs_store);
static struct kobj_attribute pit_start = __ATTR(start_val, 0660, sysfs_show, sysfs_store);
static struct kobj_attribute pit_int = __ATTR(int_val, 0660, sysfs_show, sysfs_store);
static struct attribute *attrs[] = 
{
  &pit_period.attr,
  &pit_start.attr,
  &pit_int.attr,
  NULL,
};
static struct attribute_group attr_group = 
{
  .attrs = attrs,
};

struct pit_device
{
  int minor_num;                      // Device minor number
  struct cdev cdev;                   // Character device structure
  struct platform_device * pdev;      // Platform device pointer
  struct device* dev;                 // device (/dev)
 
  phys_addr_t phys_addr;              // Physical address
  u32 mem_size;                       // Allocated mem space size 
  u32* virt_addr;                     // Virtual address
}; 


static struct resource* phys_address;
struct pit_device adev;
static struct file_operations fops = 
{
  .owner = THIS_MODULE,
  .unlocked_ioctl = pit_ioctl
};
static struct class *the_class; // for our class_create function, gets init'd later
static dev_t dev_device; // gets init'd in alloc_chrdev_region
static struct of_device_id pit_of_match[] = 
{
  { .compatible = "xlnx,xps-intc-1.00.b", },
  {}
};

static struct platform_driver pd =
{
   .probe = pit_probe,
   .remove = pit_remove, 
   .driver =
   {
     .name = MODULE_NAME,
     .owner = THIS_MODULE,
     .of_match_table = pit_of_match,
   },
};
 
// This is called when Linux loads your driver
// @return either a success code (1) or fail (neg)
static int pit_init(void) 
{
  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  // 1st param - output - will hold first number of allocated range
  // 2nd param - requested first minor number to use, usually 0
  // 3rd param - count - total number of contiguous device numbers requested
  // 4th param - name - name of device associated with number range (appear in /proc/devices and sysfs)
  int major_num = alloc_chrdev_region(&dev_device, 0, DEV_NUM, MODULE_NAME);
  printk("DEBUG: Major Number for PIT: %d\n", MAJOR(dev_device));
 
  // Create a device class. -- class_create() - this will put in in /sys/class
  // 1st param - owner - pointer to the module that is to "own" this struct class
  // 2nd param - name - this is a pointer to a string to the name
  the_class = class_create(THIS_MODULE, MODULE_NAME);

  adev.minor_num = MINOR(dev_device);
  adev.phys_addr = PIT_PHYS_ADDR;
  adev.mem_size = PIT_MEM_SIZE;

  // Register the driver as a platform driver -- platform_driver_register
  // Param - platform driver structure
  printk("DEBUG: Set adev.minor_num\n");
  int reg_platform_driver = platform_driver_register(&pd);
  printk("DEBUG: Reg platform driver return value = %d\n", reg_platform_driver);

  // SYSFS STUFF
  pit_obj = kobject_create_and_add("pit_sysfs", kernel_kobj);
  // if (sysfs_create_file(pit_obj, &attr.attr))
  // {
  //   printk("Cannot create sys file\n");
  //   kobject_put(pit_obj);
  //   sysfs_remove_file(kernel_kobj, &attr.attr);
  // }

  int retval;
  retval = sysfs_create_group(pit_obj, &attr_group);
  if (retval) {
    kobject_put(pit_obj);
    sysfs_remove_group(pit_obj, &attr_group);
  }

 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  if (major_num < 0) // this means error for alloc_chrdev_region
    return PIT_INIT_ALLOC_CHRDEV_REGION_FAIL;
  else if (reg_platform_driver < 0)
    return PIT_INIT_ALLOC_PLATFORM_DRIVER_REG_FAIL;
 
  return PIT_INIT_SUCCESS; // Come here when all good
}
 
// This is called when Linux unloads your driver
static void pit_exit(void) 
{
  printk("DEBUG: Exiting PIT module...\n");
  // platform_driver_unregister
  // Param - platform driver structure (platform_driver* drv)
  platform_driver_unregister(&pd);

  // class_unregister and class_destroy
  // Param - class struct
  class_unregister(the_class);
  class_destroy(the_class);

  // unregister_chrdev_region
  // 1st param - first number in the range
  // 2nd param - number of device numbers to unregister
  unregister_chrdev_region(dev_device, DEV_NUM);

  // SYSFS Stuff
  kobject_put(pit_obj);
  sysfs_remove_group(pit_obj, &attr_group);

  return;
}
 
// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int pit_probe(struct platform_device *pdev) 
{   
  // Initialize the character device structure (cdev_init)
  // 1st Param - the cdev to init - Output
  // 2nd Param - file operations for this device
  cdev_init(&adev.cdev, &fops);

  // Register the character device with Linux (cdev_add)  
  // 1st Param - the cdev structrure
  // 2nd Param - first device number
  // 3rd Param - number of consecutive minor numbers corresponding to deviec
  int cdev_added = cdev_add(&adev.cdev, dev_device, DEV_NUM);
  printk("DEBUG: Cdev Added: %d\n", cdev_added);
  printk("DEBUG: adev.cdev major number on init: %d\n", MAJOR(adev.cdev.dev));
 
  // Create a device file in /dev so that the character device can be accessed from user space (device_create).
  adev.dev = device_create(the_class, NULL, dev_device, NULL, MODULE_NAME);
 
  // Get the physical device address from the device tree -- platform_get_resource
  phys_address = platform_get_resource(pdev, IORESOURCE_MEM, adev.minor_num);

  // Reserve the memory region -- request_mem_region
  // 1st param - starting point
  // 2nd param - length of bytes
  // 3rd param - name of module
  struct resource *mem_resource = request_mem_region(phys_address->start, adev.mem_size, MODULE_NAME);
  if (mem_resource == NULL)
    printk("MEM is null in probe!\n");

  // Get a (virtual memory) pointer to the device -- ioremap
  // 1st param - physical address
  // 2nd param - size in bytes
  adev.virt_addr = ioremap(phys_address->start, adev.mem_size);
  if (adev.virt_addr == NULL)
  {
    printk("ERROR: VIRT ADDRESS IZ NULL\n");
  }

  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq
  // irq = platform_get_resource(pdev, IORESOURCE_IRQ, adev.minor_num);
  // if (irq == NULL) {
  //   printk("ERROR: irq is NULL!");
  // }
  // printk("244\n");
  // int ret = request_irq(irq->start, pit_irq, irq->flags, MODULE_NAME, NULL);
  // printk("DEBUG: The request_irq returned: %d\n", ret);
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  if (cdev_added < 0)
    return PIT_PROBE_CDEV_ADD_FAIL;

  return PIT_PROBE_SUCCESS; //(success)
}
 
static int pit_remove(struct platform_device * pdev) 
{
  // iounmap
  // 1st Param - virutal address
  iounmap(adev.virt_addr);

  // release_mem_region
  // 1st param - start
  // 2nd param - length
  release_mem_region(adev.phys_addr, adev.mem_size);

  // device_destroy
  // 1st Param - pointer to struct class that this device is registered with (class)
  // 2nd Param - dev_t of device regstered
  device_destroy(the_class, dev_device);

  // cdev_del
  // 1st Param - cdev structure
  cdev_del(&adev.cdev);

  return 0;
}

// This function interacts with userspace and passes in values from the arg
// 1st Param - file - the file pointer passed from the application
// 2nd Param - cmd - is the ioctl command that was called from user space
// 3rd Param - arg - the arguements passed from the user space
static long pit_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  return 0; // just return zero, we really don't need to do anything
}

/*********************************** SYSFS STUFF **********************************************/
// Notes
// Sysfs function - this allows us to cat and echo through the terminal
// This function should get/set timer period
// Start/stop timer
// Turn timer interrupts on/off

// https://embetronicx.com/tutorials/linux/device-drivers/sysfs-in-linux-kernel/#Complete_Driver_Code

static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  printk("LAB 6: Sysfs - Read!!\n");

  int var;
  if (strcmp(attr->attr.name, "period_val") == 0) 
  {
    var = period_val;
  }
  else if (strcmp(attr->attr.name, "start_val") == 0) 
  {
    var = start_val;
  }
  else if (strcmp(attr->attr.name, "int_val") == 0)
  {
    var = int_val;
  }
  else {
    printk("ERROR: The attribute requested does not exist!\n");
    var = 0;
  }

  return sprintf(buf, "%d\n", var);
}

static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
  printk("LAB 6: Sysfs - Write!!!\n");

  int var;
  sscanf(buf, "%d", &var);

  if (strcmp(attr->attr.name, "period_val") == 0) 
  {
    period_val = var;
    // THIS PART SETS THE TIMER PERIOD
    iowrite32(period_val, (adev.virt_addr + SLV_REG0 / WORD_SIZE));
  }
  else if (strcmp(attr->attr.name, "start_val") == 0) 
  {
    start_val = var;
    // THIS PART SETS THE START VALUE
    int slv_reg1 = ioread32(adev.virt_addr + SLV_REG1 / WORD_SIZE);
    iowrite32(slv_reg1 | start_val, (adev.virt_addr + SLV_REG1 / WORD_SIZE));
  }
  else if (strcmp(attr->attr.name, "int_val") == 0)
  {
    int_val = var;
        // THIS PART SETS THE INTERRUPT VALUE
    int slv_reg1 = ioread32(adev.virt_addr + SLV_REG1 / WORD_SIZE);
    iowrite32(slv_reg1 | int_val, (adev.virt_addr + SLV_REG1 / WORD_SIZE));
  }
  else {
    printk("ERROR: The attribute requested does not exist!\n");
    var = 0;
  }

  return count;
}

/*************************************** OLD STUFF ******************************************/

// struct attribute
// {
//   char* name;
//   struct module* owner;
//   umode_t mode; // octal permissions
// };

// struct kobject* ko;// = adev.dev->kobj; // used for adding files

// struct device_attribute
// {
//   struct attribute attr;
//   ssize_t (*show) // called when sysfs is read
//     (
//       struct device* dev, 
//       struct device_attribute *attr,
//       char* buf // PAGE_SIZE bytes
//     );
//   ssize_t (*store) // called when sysfs is write, returns number of bytes consumed, normally count
//   (
//     struct device* dev,
//     struct device_attribute* attr,
//     const char* buf, // data
//     size_t count // size of data
//   );
// };

// ssize_t pit_do_read(struct device* dev, struct device_attribute* attr, char* buf);
// ssize_t pit_do_write(struct device* dev, struct device_attributes* attr, const char* buf, size_t count);

// // Global declaration of device_attribute
// static struct device_attribute my_dev_attr;