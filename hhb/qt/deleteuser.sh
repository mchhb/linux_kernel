	#!/bin/sh
#用于删除qt项目的用户配置文件*.user*
for file in `find -name "*.user*"`
do
 rm $file 
done
