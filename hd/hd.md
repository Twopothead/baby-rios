curie@girl:~/gitcode/baby-rios/hd$ bximage
========================================================================
                                bximage
                  Disk Image Creation Tool for Bochs
          $Id: bximage.c 11315 2012-08-05 18:13:38Z vruppert $
========================================================================

Do you want to create a floppy disk image or a hard disk image?
Please type hd or fd. [hd] hd

What kind of image should I create?
Please type flat, sparse or growing. [flat] flat

Enter the hard disk size in megabytes, between 1 and 8257535
[10] 10

I will create a 'flat' hard disk image with
  cyl=20
  heads=16
  sectors per track=63
  total sectors=20160
  total size=9.84 megabytes

What should I name the image?
[c.img] rios_hd.img

Writing: [] Done.

I wrote 10321920 bytes to rios_hd.img.

The following line should appear in your bochsrc:
  ata0-master: type=disk, path="rios_hd.img", mode=flat, cylinders=20, heads=16, spt=63

