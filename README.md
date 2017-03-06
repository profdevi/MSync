# MSync.exe
Windows Apps to synchronize files in the current directory with same files in
another directory hierarchy.

The intent of this tool is to permit modular development of various systems
such as web page development, and software package management.

#Example Usage for a website

Assume that you have a web site development that has a hierarchical structure
of dependencies.  Assume that a portion of the website consists of a single
webpage Alpha.html, associated CSS file Alpha.css, a Javascript file Alpha.js,
and a single image  Alpha.png.  Define the module Alpha as comprised of the
four files Alpha.html, Alpha.css, Alpha.js, and Alpha.png.  In modular 
development, we would like to reuse this module Alpha across various web sites.

Ideally, we would like to group this module Alpha and its associated file 
together, but in a normal structured website, the files will be physically located
in various directories.  For instance assume, the file Alpha.html is found in 
the root folder, the Alpha.css is found in the css subfolder, the Alpha.js is 
found the subfolder js, and the Alpha.png is found in the images folder.

In our work folder, we place our files together and create an Alpha.msync file.
The Alpha.msync file contains the information to synchronize the files to a 
target directory.  The Alpha.msync file consists of the filename=target directory
values.  Comments are started with a pound(#).

```
# This is the contents of #Alpha.msync
#  Blank lines are skipped
Alpha.html = /           # this will place Alpha.html in the root folder
Alpha.png  = images      # this will place Alpha.png to images folder
Alpha.js   = js          # this will place Alpha.js to js folder
Alpha.css  = css         # this will place Alpha.css to css folder
```

In the folder where you have the four files and the .msync file, you can 
run the next command.  

'''
C:\work> MSync -w SomeDir 
'''

This will create the folder SomeDir and place the files in the correct 
subfolders of images, js, css.

If you should modify the files in the SomeDir folder, you can use the next 
command to read the files back to your work folder.

```
C:\work> MSync -r SomeDir
```

Running the next command lists out all the .msync files in the current folder.

```
C:/work> MSync -l
```

The next command shows if any of the files of hierarchy have changed.

```
C:\work> MSync SomeDir
```


## Download Links 

The following links is for the direct download of the 32 bit executable with
an MD5 digest of the executable.

Windows 32-bit Binary : [MSync.exe](bin/MSync.exe)

MD5 Digest for binary : [MSync.exe.md5](bin/MSync.exe.md5)


