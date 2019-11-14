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
#define SPACE_AVAIL ((1024 * 3) / 4)

#define WORD_SIZE 4
#define DEV_NUM 1

/* Volume stuff */
#define INC_VOLUME 1
#define DEC_VOLUME 0
#define INC_FLAG 1
#define DEC_FLAG -1
#define VOLUME_MAX 5
#define VOLUME_MIN -4
#define NORMAL_VOL 1

/* Audio struct attributes */
#define AUDIO_PHYS_ADDR 0x43C20000
#define AUDIO_MEM_SIZE 0x10000
#define RIGHT_CH_OFFSET 0x08
#define LEFT_CH_OFFSET 0x0C

/* IOCTL STUFF */
// magic number = unique number - use major number
// command number = number tah tis assigned to the ioctl
// Last line is type of data (int32_t, etc.)
#define WR_VALUE _IOW(242, 242, s32*) // 242 = major number
#define RD_VALUE _IOR(242, 243, s32*) // 243 = different number

/* Bitmasks */
#define RIGHT_FIFO_BITMASK 0x000003FE
#define LEFT_FIFO_BITMASK 0x001FF800
#define RIGHT_FIFO_SHIFT 1
#define LEFT_FIFO_SHIFT 11

/* Globals for this file */
static s32* kern_buf;
static u32 buf_index = 0;
static u32 size_buf = 0;
static s8 volume_level = NORMAL_VOL; // volume level 

// Function declarations for the kernal
static int audio_init(void);
static void audio_exit(void);
static int audio_probe(struct platform_device *pdev);
static int audio_remove(struct platform_device * pdev); 
static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off);
static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off);
static irqreturn_t audio_irq(int i, void *v);
static long audio_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

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
}; 

static struct resource* phys_address;
static struct resource* irq;
struct audio_device adev;
static struct file_operations fops = 
{
  .owner = THIS_MODULE,
  .read = audio_read,
  .write = audio_write,
  .unlocked_ioctl = audio_ioctl
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
// @return either a success code (1) or fail (neg)
static int audio_init(void) 
{
  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  // 1st param - output - will hold first number of allocated range
  // 2nd param - requested first minor number to use, usually 0
  // 3rd param - count - total number of contiguous device numbers requested
  // 4th param - name - name of device associated with number range (appear in /proc/devices and sysfs)
  int major_num = alloc_chrdev_region(&dev_device, 0, DEV_NUM, MODULE_NAME);
  pr_info("DEBUG: Major Number for Audio: %d\n", MAJOR(dev_device));
 
  // Create a device class. -- class_create() - this will put in in /sys/class
  // 1st param - owner - pointer to the module that is to "own" this struct class
  // 2nd param - name - this is a pointer to a string to the name
  the_class = class_create(THIS_MODULE, MODULE_NAME);

  adev.minor_num = MINOR(dev_device);
  adev.phys_addr = AUDIO_PHYS_ADDR;
  adev.mem_size = AUDIO_MEM_SIZE;

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
  unregister_chrdev_region(dev_device, DEV_NUM);

  return;
}
 
// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev) 
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
  pr_info("DEBUG: Cdev Added: %d\n", cdev_added);
  pr_info("DEBUG: adev.cdev major number on init: %d\n", MAJOR(adev.cdev.dev));
 
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

  return AUDIO_PROBE_SUCCESS; //(success)
}
 
static int audio_remove(struct platform_device * pdev) 
{
  free_irq(irq->start, NULL);

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

static ssize_t audio_read(struct file *f, char __user *u, size_t size, loff_t *off)
{
  // Return 1 byte of data with 0 or 1, indicated whether an audio sample is currently being played
  // 1st param = destination address in user space
  // 2nd param = source address in kernal space
  // 3rd param = number of bytes to copy
  // Return 0 if everything went well
  long no_bytes_not_copied = copy_to_user(u, kern_buf, size);
  if (no_bytes_not_copied != 0)
    pr_info("DEBUG M2: BAD - read not reading bytes = %ld\n", no_bytes_not_copied);


  return (buf_index >= size_buf); // 0 = audio playing, anything else = audio not playing
}

static ssize_t audio_write(struct file *f, const char __user *u, size_t size, loff_t *off)
{
  // Immediatley Disable the interrupts
  u32 status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  status &= DISABLE_IRQ;
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));

  // Free the buffer used to store old sound sample
  if (kern_buf != NULL) {
    kfree(kern_buf);
  }

  // Allocate new buffer
  kern_buf = kmalloc(size, GFP_KERNEL); // GFP_KERNAL = allocate memory based on kernal
  buf_index = 0; // reset the buffer
  size_buf = (size) / WORD_SIZE; // get the size

  if (!kern_buf)
  {
    pr_info("DEBUG M2: BAD - kmalloc for write not working\n");
    return -ENOMEM;
  }

  // Copy the userspace to newly allocated buffer (LDD3 pg 64)
  // 1st param = destination address in kernal space
  // 2nd param = source address in user space
  // 3rd param = number of bytes to copy
  // Return 0 if everything went well
  long no_bytes_not_copied = copy_from_user(kern_buf, u, size);
  if (no_bytes_not_copied != 0)
  {
    pr_info("DEBUG M2: BAD - Bytes not copied = %ld\n", no_bytes_not_copied);
    kfree(kern_buf);
    return -EFAULT;
  }

  // Make sure interrupts are enabled
  status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
  status |= ENABLE_IRQ;
  iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));

  return 0;
}

// This function interacts with userspace and passes in values from the arg
// This is mainly used for our volume control
// 1st Param - file - the file pointer passed from the application
// 2nd Param - cmd - is the ioctl command that was called from user space
// 3rd Param - arg - the arguements passed from the user space
static long audio_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  s8 val = (arg == INC_VOLUME) ? INC_FLAG : DEC_FLAG;

  switch (cmd)
  {
    case WR_VALUE: // when we write
      if ((val+volume_level) > VOLUME_MAX) // if we go above max volume, don't go higher
      { 
         volume_level = VOLUME_MAX; // keep it set
      } 
      else if ((val+volume_level) < VOLUME_MIN) // if we go under
      {
         volume_level = VOLUME_MIN; // keep it min and not any lower
      }
      else if ((val+volume_level) == 0) // if it's zero, it's mute
      {
         if (val > 0) // just go normal vlaue
         {
           volume_level = NORMAL_VOL;
         }
         else
         {
           volume_level = -NORMAL_VOL;
         }
      }
      else // if all else good, just inc the volume by the value passed in
      {
        volume_level += val;
      }
      break;
     case RD_VALUE: // when we read
      break;
  }

  return 0; // just return zero, we really don't need to do anything
}

// This is our ISR for the audio, it gets called after every interrupt
// @return returns a flag, either ISR_HANDLED meaning that we take responsability for the interrupt or
//         it returns IRQ_NONE meaning we are not responsible for the interrupt
static irqreturn_t audio_irq(int i, void *v) 
{
  // Getting the data count in the FIFOs - we do this to check we are actually getting stuff
  u32 fifo = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);

  printk("FIFO VALUE: %x\n", fifo);

  for (u32 j = 0; j < SPACE_AVAIL; j++)
  {
    if (buf_index < (size_buf))
    {
      if (volume_level > 0) // if volume level is high
      { 
        iowrite32(kern_buf[buf_index]*volume_level, (adev.virt_addr + RIGHT_CH_OFFSET / WORD_SIZE)); // RIGHT
        iowrite32(kern_buf[buf_index]*volume_level, (adev.virt_addr + LEFT_CH_OFFSET / WORD_SIZE)); // left
      }
      else // if volume is not high
      {
        iowrite32(kern_buf[buf_index]/-(volume_level), (adev.virt_addr + RIGHT_CH_OFFSET / WORD_SIZE)); // RIGHT
        iowrite32(kern_buf[buf_index]/-(volume_level), (adev.virt_addr + LEFT_CH_OFFSET / WORD_SIZE)); // left
      }
    }
    buf_index++;
  }

  if (buf_index >= size_buf) // If the clip is done
  {
    // DISABLE INTERRUPTS SO NO INFINITE LOOP
    u32 status = ioread32(adev.virt_addr + IRQ_OFFSET / WORD_SIZE);
    status &= DISABLE_IRQ;
    iowrite32(status, (adev.virt_addr + IRQ_OFFSET / WORD_SIZE));
  }

  return IRQ_HANDLED;
}
