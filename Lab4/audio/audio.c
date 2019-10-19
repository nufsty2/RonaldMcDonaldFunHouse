#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ronald McDonald");
MODULE_DESCRIPTION("ECEn 427 Audio Driver");
 
#define MODULE_NAME "audio" 

static int audio_init(void);

static void audio_exit(void);

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
 
// This is called when Linux loads your driver
static int audio_init(void) 
{
  pr_info("%s: Initializing Audio Driver!\n", MODULE_NAME);


  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  dev_t* first_num;

  // DON'T MAKE YET!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  // 1st param - output - will hold first number of allocated range
  // 2nd param - requested first minor number to use, usually 0
  // 3rd param - count - total number of contagious device numbers requested
  // 4th param - name - name of device associated with number range (appear in /proc/devices and sysfs)
  int major_num = alloc_chrdev_region(first_num, 0, 1, "/proc/devices");
  printk("Major Number: %d\n\r", major_num);
 
  // Create a device class. -- class_create()
 
  // Register the driver as a platform driver -- platform_driver_register
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
 
  return 0; // (Success)
}
 
// This is called when Linux unloads your driver
static void audio_exit(void) 
{
  // platform_driver_unregister
  // class_unregister and class_destroy
  // unregister_chrdev_region
  return;
}
 
// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev) 
{   
  // Initialize the character device structure (cdev_init)
  // Register the character device with Linux (cdev_add)  
 
  // Create a device file in /dev so that the character device can be accessed from user space (device_create).
 
  // Get the physical device address from the device tree -- platform_get_resource
  // Reserve the memory region -- request_mem_region
  // Get a (virtual memory) pointer to the device -- ioremap
 
  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq
 
  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
 
  return 0; //(success)
}
 
static int audio_remove(struct platform_device * pdev) 
{
  // iounmap
  // release_mem_region
  // device_destroy
  // cdev_del
  return 0;
}