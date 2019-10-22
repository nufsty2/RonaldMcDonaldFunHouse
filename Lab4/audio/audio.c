#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/device.h>

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
} audio_device; 

static struct class *the_class; // for our class_create function, gets init'd later
static dev_t dev_device; // gets init'd in alloc_chrdev_region
static struct of_device_id audio_of_match[] = 
{
  { .compatible = "xlnx,xps-intc-1.00.a", },
  {}
};

MODULE_DEVICE_TABLE(of, audio_of_match);
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
  pr_info("DEBUG: %s: Initializing Audio Driver!\n", MODULE_NAME);

  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  // 1st param - output - will hold first number of allocated range
  // 2nd param - requested first minor number to use, usually 0
  // 3rd param - count - total number of contagious device numbers requested
  // 4th param - name - name of device associated with number range (appear in /proc/devices and sysfs)
  int major_num = alloc_chrdev_region(&dev_device, 0, 1, MODULE_NAME);
  pr_info("DEBUG: Major Number for Audio: %d\n", dev_device);
 
  // Create a device class. -- class_create() - this will put in in /sys/class
  // 1st param - owner - pointer to the module that is to "own" this struct class
  // 2nd param - name - this is a pointer to a string to the name
  the_class = class_create(THIS_MODULE, MODULE_NAME); // TODO: first paramter might be wrong

 
  // Register the driver as a platform driver -- platform_driver_register
  // Param - platform driver structure
  int reg_platform_driver = platform_driver_register(&pd);
  pr_info("DEBUG: Platform driver return code: %d\n", reg_platform_driver);

 
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
  // // platform_driver_unregister
  // // Param - platform driver structure (platform_driver* drv)
  // platform_driver_unregister(pd);

  // // class_unregister and class_destroy
  // // Param - class struct
  // class_destroy(the_class);

  // // unregister_chrdev_region
  // // 1st param - first number in the range
  // // 2nd param - number of device numbers to unregister
  // unregister_chrdev_region(dev_device, 1);

  return;
}
 
// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev) 
{   
  // // Initialize the character device structure (cdev_init)
  // // 1st Param - the cdev to init - Output
  // // 2nd Param - file operations for this device
  // cdev_init(&audio_device.cdev, struct file_operations *fops);
  // // Register the character device with Linux (cdev_add)  
  // // 1st Param - the cdev structrure
  // // 2nd Param - first device number
  // // 3rd Param - number of consecutive minor numbers corresponding to deviec
  // int cdev_added = cdev_add(&audio_device.cdev, dev_device, 1); // TODO: 3rd param was just a guess
 
  // // Create a device file in /dev so that the character device can be accessed from user space (device_create).
  // // TODO: Check this and get error codes
  // device_create(the_class, NULL, dev_device, NULL, MODULE_NAME);
 
  // // Get the physical device address from the device tree -- platform_get_resource
  // struct resource* the_resource = platform_get_resource(pdev, 0, 0); // TODO: check 2nd and 3rd params

  // // Reserve the memory region -- request_mem_region
  // // 1st param - starting point
  // // 2nd param - length of bytes
  // // 3rd param - name of module
  // struct resource* mem_resource = request_mem_region(0, 0x10000, MODULE_NAME); // TODO: check 1st and 2nd params

  // // Get a (virtual memory) pointer to the device -- ioremap
  // // 1st param - physical address
  // // 2nd param - size in bytes
  // ioremap(audio_device.phys_addr, 0x10000); // TODO: find param 2
 
  // // Get the IRQ number from the device tree -- platform_get_resource
  // // Register your interrupt service routine -- request_irq
  // // TODO: THIS STUFF
 
  // // If any of the above functions fail, return an appropriate linux error code, and make sure
  // // you reverse any function calls that were successful.
  // if (cdev_added < 0)
  //   return AUDIO_PROBE_CDEV_ADD_FAIL;
 
  return AUDIO_PROBE_SUCCESS; //(success)
}
 
static int audio_remove(struct platform_device * pdev) 
{
  // // iounmap
  // // 1st Param - virutal address
  // iounmap(pdev.virt_addr);

  // // release_mem_region
  // // 1st param - start - TODO - find right values for it
  // // 2nd param - length
  // release_mem_region(0, 1);

  // // device_destroy
  // // 1st Param - pointer to struct class that this device is registered with (class)
  // // 2nd Param - dev_t of device regstered
  // device_destroy(the_class, dev_device);

  // // cdev_del
  // // 1st Param - cdev structure
  // cdev_del(pdev.cdev);

  return 0;
}