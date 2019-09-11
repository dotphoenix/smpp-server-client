# smpp server & smpp client
A stable and complete smpp server&client in c++ , you can easily extended it to cmpp/sgip/smgp, it can be run on Linux, MacOS and windows, it can also be easily built under Android and iOS.

If you want to build it , you should first install boost after 1.54 on your computer, and set the path in the Make file or the xcode/visual studio project.

Under xcode, you should set:
HEADER_SEARCH_PATHS = $(inherited) /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include /Users/alex/Public/boost/include/
LIBRARY_SEARCH_PATHS = /Users/alex/Public/boost/lib

Under visual studio:

In Make file:


It also depends on a smppparser:http://smppcxx.sourceforge.net, the sourcecode is added  to xcode/visual studio project, but under linux, you need go to public/smppparser, the run make, it will generate a libsmppparser.a file, you should set its path  the client's and the server's Makefile.


Any questions , please contact dotphoenix@qq.com.
