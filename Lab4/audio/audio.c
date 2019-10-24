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

// Function declarations for the kernal
static int audio_init(void);
static void audio_exit(void);
static int audio_probe(struct platform_device *pdev);
static int audio_remove(struct platform_device * pdev); 
static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off);
static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off);
static irqreturn_t audio_irq(int i, void *v);
void audio_interrupts_write(uint32_t offset, uint32_t value);
uint32_t audio_interrupts_read(uint32_t offset);

// Init and exit declarations 
module_init(audio_init);
module_exit(audio_exit);
 
 
struct audio_device
{
  int minor_num;                      // Device minor number
  struct cdev cdev;                   // Character device structure
  struct platform_device * pdev;      // Platform device pointer
  struct device* dev;                 // device (/dev)
 
  unsigned long phys_addr;              // Physical address
  unsigned long mem_size;                       // Allocated mem space size 
  unsigned long* virt_addr;                     // Virtual address
 
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

// TEMP
static struct cdev temp_cdev;
 
// This is called when Linux loads your driver
static int audio_init(void) 
{
  pr_info("DEBUG: %s: Initializing Audio Driver!\n", MODULE_NAME);

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

 
  // Register the driver as a platform driver -- platform_driver_register
  // Param - platform driver structure
  int reg_platform_driver = platform_driver_register(&pd);
  pr_info("DEBUG: Platform driver return code: %d\n", reg_platform_driver);

  adev.minor_num = MINOR(dev_device);
  adev.phys_addr = 0x43C20000;
  adev.mem_size = 0x10000;

 
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
  pr_info("DEBUG: Entering audio_exit!\n");
  // platform_driver_unregister
  // Param - platform driver structure (platform_driver* drv)
  platform_driver_unregister(&pd);
  pr_info("DEBUG: Got past driver_unregister!\n");

  // class_unregister and class_destroy
  // Param - class struct
  class_unregister(the_class);
  pr_info("DEBUG: Got past class_unregister\n");
  class_destroy(the_class);
  pr_info("DEBUG: Got past class_destroy\n");

  // unregister_chrdev_region
  // 1st param - first number in the range
  // 2nd param - number of device numbers to unregister
  unregister_chrdev_region(dev_device, 1);
  pr_info("DEBUG: Got past unregister_chrdev_region\n");

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
  int cdev_added = cdev_add(&adev.cdev, dev_device, 0); // TODO: 3rd param was just a guess
  pr_info("DEBUG: Cdev Added: %d\n", cdev_added);
  pr_info("DEBUG: adev.cdev major number on init: %d\n", MAJOR(adev.cdev.dev));
 
  // Create a device file in /dev so that the character device can be accessed from user space (device_create).
  adev.dev = device_create(the_class, NULL, dev_device, NULL, MODULE_NAME);
 
  // Get the physical device address from the device tree -- platform_get_resource
  phys_address = platform_get_resource(pdev, IORESOURCE_MEM, 0); // TODO: check 2nd and 3rd params

  // Reserve the memory region -- request_mem_region
  // 1st param - starting point
  // 2nd param - length of bytes
  // 3rd param - name of module
  pr_info("DEBUG: phys_address = 0x%X - 0x%X\n", phys_address->start, phys_address->end);
  pr_info("DEBUG: mem_size = %x\n", adev.mem_size);
  struct resource *mem_resource = request_mem_region(phys_address->start, 0x10000, MODULE_NAME); // TODO: check 1st and 2nd params
  if (mem_resource == NULL) 
    pr_info("DEBUG: mem_resource is NULL!\n");
  else
    pr_info("DEBUG: mem_resource = 0x%X - 0x%X, adev.phys_addr = 0x%X\n", mem_resource->start, mem_resource->end, adev.phys_addr);

  // Get a (virtual memory) pointer to the device -- ioremap
  // 1st param - physical address
  // 2nd param - size in bytes
  adev.virt_addr = ioremap(phys_address->start, 0x10000);
  pr_info("DEBUG: adev.virt_addr in the probe function = 0x%X", adev.virt_addr);

  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq
  irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
  int ret = request_irq(irq->start, audio_irq, irq->flags, MODULE_NAME, NULL);
  pr_info("DEBUG: The request_irq returned: %d\n", ret);
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  if (cdev_added < 0)
    return AUDIO_PROBE_CDEV_ADD_FAIL;
 
  u32 status = ioread32((void*)(adev.virt_addr) + 0x10);
  status |= 0x1;
  iowrite32(status, (void*)(adev.virt_addr) + 0x10);

  return AUDIO_PROBE_SUCCESS; //(success)
}
 
static int audio_remove(struct platform_device * pdev) 
{
  // iounmap
  // 1st Param - virutal address
  pr_info("DEBUG: adev.virt_addr in the remove function = 0x%X", adev.virt_addr);
  iounmap(adev.virt_addr);

  // release_mem_region
  // 1st param - start - TODO - find right values for it
  // 2nd param - length
  pr_info("DEBUG: adev.phys_addr amd adev.mem_size in the remove function = 0x%X and 0x%X", adev.phys_addr, adev.mem_size);
  release_mem_region(adev.phys_addr, adev.mem_size);

  // device_destroy
  // 1st Param - pointer to struct class that this device is registered with (class)
  // 2nd Param - dev_t of device regstered
  pr_info("DEBUG: Entering device destroy!!!\n");
  device_destroy(the_class, dev_device);

  // cdev_del
  // 1st Param - cdev structure
  pr_info("DEBUG: Entering cdev del!!!\n");
  pr_info("DEBUG: adev.cdev major number: %d\n", MAJOR(adev.cdev.dev));
  cdev_del(&adev.cdev);

  pr_info("DEBUG: Returning from audio remove!!\n");
  return 0;
}

static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off)
{
  pr_info("AUDIO_READ() ran!\n");
  return 0;
}

static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off)
{
  pr_info("AUDIO_WRITE() ran!\n");
  return 0;
}

static irqreturn_t audio_irq(int i, void *v) {
  return IRQ_NONE;
}