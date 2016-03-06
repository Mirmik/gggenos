#ifndef __KERN_LEVELS_H__

extern int printk_level;

  #define KERN_ALERT      1,    /* action must be taken immediately */
  #define KERN_CRIT       2,    /* critical conditions */
  #define KERN_ERR        3,    /* error conditions */
  #define KERN_WARNING    4,    /* warning conditions */
  #define KERN_NOTICE     5,    /* normal but significant condition */
  #define KERN_INFO       6,    /* informational */
  #define KERN_DEBUG      7,    /* debug-level messages */
  
 #endif