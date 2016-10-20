##修改说明
 
SumatraPDF 是非常优秀的epub/mobi/pdf阅读器,启动速度快,轻量小巧。但有些美中不足的是对中文书籍的支持不太好，经常出现标点符号导致不正确的换行，看起来很扎眼。



所幸这是个开源软件，我就把下面的问题改了一下：

- 每行固定字数，遇到标点不会导致换行，排版很工整
- 把行间的行距加大了一些，看书时更舒服



修改版下载地址

https://github.com/jijinggang/sumatrapdf/raw/master/download/SumatraPDF.zip

## SumatraPDF Reader

SumatraPDF is a multi-format (PDF, EPUB, MOBI, FB2, CHM, XPS, DjVu) reader
for Windows under (A)GPLv3 license, with some code under BSD license (see
AUTHORS).

More information:
* [main website](http://www.sumatrapdfreader.org) with downloads and documentation
* [wiki with more docs](https://github.com/sumatrapdfreader/sumatrapdf/wiki)

To compile you need Visual Studio 2015. [Free Community edition](https://www.visualstudio.com/products/visual-studio-community-vs)
works.

Open `vs2015/SumatraPDF.sln`.

[![Build status](https://ci.appveyor.com/api/projects/status/tesjtgmpy26uf8p7?svg=true)](https://ci.appveyor.com/project/kjk/sumatrapdf)
