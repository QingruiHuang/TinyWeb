
## CentOS -> yum 
## Debian -> apt-get

# 【linux 安装 工具】
```
sudo apt-get install git
sudo apt-get install automake
sudo apt-get install libtool
# gcc  g++ gdb ...
```

# 【linux 安装 libuv】
```
git clone https://github.com/libuv/libuv.git
cd libuv
sh autogen.sh
./configure
make
make check
sudo make install
```
