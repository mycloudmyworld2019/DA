#! /bin/bash

NAME=$1
DA_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
#mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${DA_PREFIX}/bin
mkdir -p ${DA_PREFIX}/licenses/DAio
#mkdir -p ${DA_PREFIX}/include
#mkdir -p ${DA_PREFIX}/lib/cmake/${PROJECT}
#mkdir -p ${DA_PREFIX}/cmake
#mkdir -p ${DA_PREFIX}/scripts

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${DA_PREFIX}/bin  || exit 1

# install licenses
cp -R ${BUILD_DIR}/licenses/DAio/* ${DA_PREFIX}/licenses || exit 1

# install libraries
#cp -R ${BUILD_DIR}/lib/* ${DA_PREFIX}/lib

# install cmake modules
#sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/DAioTesterPackage.cmake &> ${DA_PREFIX}/lib/cmake/${PROJECT}/DAioTester.cmake
#sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${DA_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install includes
#cp -R ${BUILD_DIR}/include/* ${DA_PREFIX}/include

# make symlinks
#pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
#ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/DAioTester.cmake DAioTester.cmake
#popd &> /dev/null

for f in $(ls "${BUILD_DIR}/bin/"); do
   bn=$(basename $f)
   ln -sf ../${SUBPREFIX}/bin/$bn ${PREFIX}/bin/$bn || exit 1
done
echo "Generating Tarball $NAME.tar.gz..."
tar -cvzf $NAME.tar.gz ./${PREFIX}/* || exit 1
rm -r ${PREFIX} || exit 1
