rmdir /Q /S dbg
rmdir /Q /S rel
rmdir /Q /S spy

make
make CONF=rel
make CONF=spy

