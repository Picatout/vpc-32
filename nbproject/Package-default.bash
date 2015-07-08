#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_CONF=default
CND_DISTDIR=dist
TMPDIR=build/${CND_CONF}/${IMAGE_TYPE}/tmp-packaging
TMPDIRNAME=tmp-packaging
<<<<<<< HEAD
OUTPUT_PATH=dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
OUTPUT_BASENAME=VPC-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
=======
OUTPUT_PATH=dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
OUTPUT_BASENAME=vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
>>>>>>> 1abd89f007cb013a29040570370025f6a1d9c7ce
PACKAGE_TOP_DIR=vpc-32/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/package
rm -rf ${TMPDIR}
mkdir -p ${TMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory ${TMPDIR}/vpc-32/bin
copyFileToTmpDir "${OUTPUT_PATH}" "${TMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755


# Generate tar file
cd "${TOP}"
rm -f ${CND_DISTDIR}/${CND_CONF}/package/vpc-32.tar
cd ${TMPDIR}
tar -vcf ../../../../${CND_DISTDIR}/${CND_CONF}/package/vpc-32.tar *
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${TMPDIR}
