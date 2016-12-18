（1）群聊主界面，有工具栏，工具栏功能分别是发送文件、打开音乐播放器、保存聊天记录、清空聊天记录、打开记事本、退出登录、打开系统默认浏览器、屏幕截屏。发送框上面是发送框字体的编辑和打开涂鸦板功能，可以让用户自定义字体，比如字体加粗、字体倾斜、加下划线、修改字体颜色、打开涂鸦板、发送表情(还没实现)，发送图片。
登录成后还有显示登录者的名字和在线人数，左边的TableWidget有显示登录者的名字、主机名、ip
![这里写图片描述](http://img.blog.csdn.net/20161218223247316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDQyNzM5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 
可以实现群聊，互发信息
 
可以根据用户需要设置字体的颜色，加粗，倾斜，加下划线
 
发送图片
 
向好友发送图片

 
打开标准文件对话框
 ![这里写图片描述](http://img.blog.csdn.net/20161218223322073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDQyNzM5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对方接受成功，提示登录名
 
为了让用户可以边聊天边听音乐，加了一个音乐播放器，带有音乐播放列表，带有音量调节、静音、上一首、下一首、添加音乐等功能 
这是在播放音乐
 ![这里写图片描述](http://img.blog.csdn.net/20161218223340901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDQyNzM5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
单击列表的音乐就播放那一首
 
这是一个记事本，其作用是方便用户记下重要的事情，它有很多功能，如打开文件、保存文件、退出、打印、复制、黏贴、剪切、加粗、倾斜、下划线、左对齐、右对齐、设置字体风格、字体、字体字号、插入图片、插入表格、搜索等
 ![这里写图片描述](http://img.blog.csdn.net/20161218223355489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDQyNzM5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这是涂鸦板界面，有设置画板风格、画笔线宽、画笔颜色、清空聊天记录、保存图片、加载图片、退出
 
这是设置画笔样式后画的
 
保存图片成功

 
这是单击打开默认浏览器按钮后打开的系统默认浏览器、我的默认浏览器是最新版的QQ浏览器，打开很快
 
这是打开的屏幕截图工具
 ![这里写图片描述](http://img.blog.csdn.net/20161218223414396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDQyNzM5MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
正在截图
 
截图成功后，鼠标右键单击，有一个保存框(其实是QAction加在QLabel里)显示出来，单击保存，弹出标准文件对话框

 
这是用系统工具打开的保存文件，就是刚才的截图
