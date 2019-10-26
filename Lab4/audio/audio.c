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

/* MODULES */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronald McDonald");
MODULE_DESCRIPTION("ECEn 427 Audio Driver");
#define MODULE_NAME "audio" 

/* Error and success code defines */
#define AUDIO_INIT_SUCCESS 0
#define AUDIO_INIT_ALLOC_CHRDEV_REGION_FAIL -1
#define AUDIO_INIT_ALLOC_PLATFORM_DRIVER_REG_FAIL -2

#define AUDIO_PROBE_SUCCESS 0
#define AUDIO_PROBE_CDEV_ADD_FAIL -3

#define IRQ_OFFSET 0x10
#define DISABLE_IRQ 0xFFFFFFFE
#define ENABLE_IRQ 0x1

#define WORD_SIZE 4

// Function declarations for the kernal
static int audio_init(void);
static void audio_exit(void);
static int audio_probe(struct platform_device *pdev);
static int audio_remove(struct platform_device * pdev); 
static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off);
static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off);
static irqreturn_t audio_irq(int i, void *v);

// Init and exit declarations 
module_init(audio_init);
module_exit(audio_exit);
 
 
struct audio_device
{
  int minor_num;                      // Device minor number
  struct cdev cdev;                   // Character device structure
  struct platform_device * pdev;      // Platform device pointer
  struct device* dev;                 // device (/dev)
 
  phys_addr_t phys_addr;              // Physical address
  u32 mem_size;                       // Allocated mem space size 
  u32* virt_addr;                     // Virtual address
 
    // Add any items to this that you need
}; 

static struct resource* phys_address;
static struct resource* irq;
struct audio_device adev;
static struct file_operations fops = 
{
  .owner = THIS_MODULE,
  .read = audio_read,
  .write = audio_write
};
static struct class *the_class; // for our class_create function, gets init'd later
static dev_t dev_device; // gets init'd in alloc_chrdev_region
static struct of_device_id audio_of_match[] = 
{
  { .compatible = "byu,ecen427-audio_codec", },
  {}
};

static struct platform_driver pd =
{
   .probe = audio_probe,
   .remove = audio_remove, 
   .driver =
   {
     .name = MODULE_NAME,
     .owner = THIS_MODULE,
     .of_match_table = audio_of_match,
   },
};
 
// This is called when Linux loads your driver
static int audio_init(void) 
{
  pr_info("DEBUG: Initializing Audio Driver!\n");

  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  // 1st param - output - will hold first number of allocated range
  // 2nd param - requested first minor number to use, usually 0
  // 3rd param - count - total number of contiguous device numbers requested
  // 4th param - name - name of device associated with number range (appear in /proc/devices and sysfs)
  int major_num = alloc_chrdev_region(&dev_device, 0, 1, MODULE_NAME);
  pr_info("DEBUG: Major Number for Audio: %d\n", MAJOR(dev_device));
 
  // Create a device class. -- class_create() - this will put in in /sys/class
  // 1st param - owner - pointer to the module that is to "own" this struct class
  // 2nd param - name - this is a pointer to a string to the name
  the_class = class_create(THIS_MODULE, MODULE_NAME); // TODO: first paramter might be wrong

  adev.minor_num = MINOR(dev_device);
  adev.phys_addr = 0x43C20000;
  adev.mem_size = 0x10000;

  // Register the driver as a platform driver -- platform_driver_register
  // Param - platform driver structure
  int reg_platform_driver = platform_driver_register(&pd);
  pr_info("DEBUG: Reg platform driver return value = %d\n", reg_platform_driver);
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  if (major_num < 0) // this means error for alloc_chrdev_region
    return AUDIO_INIT_ALLOC_CHRDEV_REGION_FAIL;
  else if (reg_platform_driver < 0)
    return AUDIO_INIT_ALLOC_PLATFORM_DRIVER_REG_FAIL;
 
  return AUDIO_INIT_SUCCESS; // Come here when all good
}
 
// This is called when Linux unloads your driver
static void audio_exit(void) 
{
  pr_info("DEBUG: Exiting audio module...\n");
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
  unregister_chrdev_region(dev_device, 1);

  pr_info("DEBUG: Audio module exited!\n");
  return;
}
 
// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev) 
{   
  pr_info("DEBUG: Calling audio_probe!\n");
  // Initialize the character device structure (cdev_init)
  // 1st Param - the cdev to init - Output
  // 2nd Param - file operations for this device
  cdev_init(&adev.cdev, &fops);
  // Register the character device with Linux (cdev_add)  
  // 1st Param - the cdev structrure
  // 2nd Param - first device number
  // 3rd Param - number of consecutive minor numbers corresponding to deviec
  int cdev_added = cdev_add(&adev.cdev, dev_device, 1); // TODO: 3rd param was just a guess
  pr_info("DEBUG: Cdev Added: %d\n", cdev_added);
  pr_info("DEBUG: adev.cdev major number on init: %d\n", MAJOR(adev.cdev.dev));
 
  // Create a device file in /dev so that the character device can be accessed from user space (device_create).
  adev.dev = device_create(the_class, NULL, dev_device, NULL, MODULE_NAME);
 
  // Get the physical device address from the device tree -- platform_get_resource
  phys_address = platform_get_resource(pdev, IORESOURCE_MEM, adev.minor_num); // TODO: check 2nd and 3rd params

  // Reserve the memory region -- request_mem_region
  // 1st param - starting point
  // 2nd param - length of bytes
  // 3rd param - name of module
  struct resource *mem_resource = request_mem_region(phys_address->start, adev.mem_size, MODULE_NAME); // TODO: check 1st and 2nd params
  if (mem_resource == NULL)
    pr_info("MEM is null in probe!\n");

  // Get a (virtual memory) pointer to the device -- ioremap
  // 1st param - physical address
  // 2nd param - size in bytes
  adev.virt_addr = ioremap(phys_address->start, adev.mem_size);

  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq
  irq = platform_get_resource(pdev, IORESOURCE_IRQ, adev.minor_num);
  int ret = request_irq(irq->start, audio_irq, irq->flags, MODULE_NAME, NULL);
  pr_info("DEBUG: The request_irq returned: %d\n", ret);
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  if (cdev_added < 0)
    return AUDIO_PROBE_CDEV_ADD_FAIL;
 
  // Enabling interrupts
  u32 status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  pr_info("DEBUG: Before oring status %x\n", status);
  status |= ENABLE_IRQ;
  pr_info("DEBUG: After oring status %x\n", status);
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));
  pr_info("DEBUG: After iowrite32!!\n");

  return AUDIO_PROBE_SUCCESS; //(success)
}
 
static int audio_remove(struct platform_device * pdev) 
{
  pr_info("DEBUG: Removing audio module...\n");
  free_irq(irq->start, NULL);

  // iounmap
  // 1st Param - virutal address
  iounmap(adev.virt_addr);

  // release_mem_region
  // 1st param - start - TODO - find right values for it
  // 2nd param - length
  release_mem_region(adev.phys_addr, adev.mem_size);

  // device_destroy
  // 1st Param - pointer to struct class that this device is registered with (class)
  // 2nd Param - dev_t of device regstered
  device_destroy(the_class, dev_device);

  // cdev_del
  // 1st Param - cdev structure
  cdev_del(&adev.cdev);

  pr_info("DEBUG: Audio module successfully removed!\n");
  return 0;
}

static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off)
{
  pr_info("DEBUG: Called audio_read()!\n"); // make sure we enter

  // Return 1 byte of data with 0 or 1, indicated whether an audio sample is currently being played
  // 1st param = destination address in user space
  // 2nd param = source address in kernal space
  // 3rd param = number of bytes to copy
  // Return 0 if everything went well
  //long no_bytes_not_copied = copy_to_user(something, something, size);

  return 0;
}

static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off)
{
  pr_info("DEBUG: Called audio_write()!\n"); // make sure we enter

  // Immediatley Disable the interrupts
  u32 status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  status &= DISABLE_IRQ;
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));

  // Free the buffer used to store old sound sample
  // kfree(something);

  // Allocate new buffer
  // kmalloc(something);

  // Copy the userspace to newly allocated buffer (LDD3 pg 64)
  // 1st param = destination address in kernal space
  // 2nd param = source address in user space
  // 3rd param = number of bytes to copy
  // Return 0 if everything went well
  //long no_bytes_not_copied = copy_from_user();

  // Make sure interrupts are enabled
  status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  status |= ENABLE_IRQ;
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));

  return 0;
}

static irqreturn_t audio_irq(int i, void *v) 
{
  pr_info("DEBUG: Called audio_irq()!\n");
  u32 status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  status &= DISABLE_IRQ;
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));
  return IRQ_HANDLED;
}
