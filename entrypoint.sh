#!/bin/bash



echo "************** caprice32 **************"
cd /
git clone https://github.com/ColinPitrat/caprice32.git
cd /caprice32
export VERSION=`git tag | grep v | sort -r | head -n 1| sed -e 's/v//i'`

BASE_PATH="amstrad-cpc-${VERSION}"

mkdir -p /${BASE_PATH}/DEBIAN
mkdir -p /${BASE_PATH}/usr/local/bin
mkdir -p /${BASE_PATH}/usr/local/share
cat /caprice32/debian/control \
    | sed -e 's/^Depends.*$/Depends: libsdl2-2.0-0, libfreetype6, zlib1g, libpng16-16, libncurses5/' \
    | sed -e 's/^Architecture.*$/Architecture: amd64/' \
    | sed -e 's/Package.*/Package: amstrad-cpc/' \
    | sed -r '/^\s*$/d'>/${BASE_PATH}/DEBIAN/control
cd /caprice32
export VERSION=`git tag | grep v | sort -r | head -n 1| sed -e 's/v//i'`
echo "Version: ${VERSION}" >>/${BASE_PATH}/DEBIAN/control

cd caprice32
make install
cp -r /usr/local/share/caprice32 /${BASE_PATH}/usr/local/share/caprice32
cp -r /usr/local/bin/cap32 /${BASE_PATH}/usr/local/bin/.
mkdir -p /${BASE_PATH}/etc
cat /etc/cap32.cfg |sed -e 's#/cpc/#/#g' > /${BASE_PATH}/etc/cap32.cfg
mkdir -p /${BASE_PATH}/usr/local/share/caprice32/cart
chmod ugoa+rw /${BASE_PATH}/usr/local/share/caprice32/cart
mkdir -p /${BASE_PATH}/usr/local/share/caprice32/disk
chmod ugoa+rw /${BASE_PATH}/usr/local/share/caprice32/disk
mkdir -p /${BASE_PATH}/usr/local/share/caprice32/tape
chmod ugoa+rw /${BASE_PATH}/usr/local/share/caprice32/tape
mkdir -p /${BASE_PATH}/usr/local/share/caprice32/snap
chmod ugoa+rw /${BASE_PATH}/usr/local/share/caprice32/snap
mkdir -p /${BASE_PATH}/usr/local/share/caprice32/screenshots
chmod ugoa+rw /${BASE_PATH}/usr/local/share/caprice32/screenshots

echo "************** Roudoudou assembler **************"
cd /
git clone https://github.com/EdouardBERGE/rasm.git
cd rasm
mkdir -p /${BASE_PATH}/usr/local/share/rasm
cp -r resources /${BASE_PATH}/usr/local/share/rasm/.
cp -r decrunch /${BASE_PATH}/usr/local/share/rasm/.
make
mv /rasm/rasm.exe /${BASE_PATH}/usr/local/bin/rasm

echo "************** dsk **************"
cd /opt
curl -L https://go.dev/dl/go1.17.6.linux-amd64.tar.gz  | tar -xz
export PATH="${PATH}:/opt/go/bin"

cd /
git clone https://github.com/jeromelesaux/dsk.git
cd dsk
go build -o /${BASE_PATH}/usr/local/bin/dsk ./cli/main.go

echo "************** playtzx **************"
cd /
git clone https://github.com/tcat-qldom/playtzx.git
cd playtzx
make install
cp /usr/local/bin/playtzx /${BASE_PATH}/usr/local/bin/.

echo "************** idsk **************"
cd /
git clone https://github.com/jeromelesaux/idsk.git
cd idsk
cmake .
make
cp iDSK /${BASE_PATH}/usr/local/bin/.

echo "************** png2cpcsprite **************"
cd /
gcc -o /${BASE_PATH}/usr/local/bin/png2cpcsprite png2cpcsprite.c


echo "************** gfx2crtc **************"
cd /
git clone https://github.com/cpcsdk/gfx2crtc.git
cd gfx2crtc
make
cp raw2crtc /${BASE_PATH}/usr/local/bin/.
cp png2crtc /${BASE_PATH}/usr/local/bin/.

echo "************** 2cdt **************"
cd /2cdt
make
cp 2cdt /${BASE_PATH}/usr/local/bin/.

echo "************** cpcxfs **************"
cd /cpcxfs/src
make -f makefile.lnx
cp cpcxfs /${BASE_PATH}/usr/local/bin/.

echo "************** diskinfo **************"
cd /dskinfo
make
cp dskinfo /${BASE_PATH}/usr/local/bin/.


echo "************** creating DEB file **************"
cd /
dpkg-deb --build ${BASE_PATH}
mv /${BASE_PATH}.deb /out/.