#!/bin/bash

# Copyright (C) 2021 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

usage() {
    echo
    echo "USAGE"
    echo "       ./build.sh [suite=BUILD_TARGET] [target_os=TARGET_OS] [target_arch=TARGET_ARCH] [variant=BUILD_VARIANT] [target_subsystem=TARGET_SUBSYSTEM]"
    echo "                  suite            : BUILD_TARGET       cts/hit/vts and so on, default value is hit"
    echo "                  target_arch      : TARGET_ARCH      arm64 or arm32, default value is arm64"
    echo "                  variant          : BUILD_VARIANT    release or debug, default value is debug"
    echo "                  target_subsystem : TARGET_SUBSYSTEM the target subsystem to build"
    echo "                  system_size      : SYSTEM_SIZE      standard, large and son on, large is for L3-L5, standard is for L2 default value is large"
    echo "                  product_name     : PRODUCT_NAME     the name of product. such as hikey960, Hi3516DV300, and so on."
    echo
    exit 1
}

parse_cmdline() {
    BASE_HOME=$(dirname $(cd $(dirname $0); pwd))
    BASE_HOME=${BASE_HOME}/../..
    # build all parts for all products by default
    BUILD_TARGET=""
    TARGET_PLATFORM=all
    TARGET_ARCH=arm
    BUILD_VARIANT=release
    UPLOAD_API_INFO=False
    SYSTEM_SIZE=standard
    PRODUCT_NAME=""
    USE_MUSL=false
    export PATH=${BASE_HOME}/prebuilts/python/linux-x86/3.8.3/bin:$PATH

    while [ -n "$1" ]; do
        var="$1"
        OPTIONS=${var%%=*}
        PARAM=${var#*=}
        echo "OPTIONS=$OPTIONS, PARAM=$PARAM"
        case "$OPTIONS" in
        suite)            BUILD_TARGET="$PARAM"
                          ;;
        target_arch)      TARGET_ARCH="$PARAM"
                          ;;
        variant)          BUILD_VARIANT="$PARAM"
                          ;; 
        use_musl)         USE_MUSL="$PARAM"
                          ;;
	    target_platform)  TARGET_PLATFORM="$PARAM"
                          ;;
        target_subsystem) export target_subsystem=${PARAM}
                          ;;
        system_size)      SYSTEM_SIZE="$PARAM"
                          ;;
        product_name)     PRODUCT_NAME="$PARAM"
                          ;;
        upload_api_info)  UPLOAD_API_INFO=$(echo $PARAM | tr [a-z] [A-Z])
                          ;;
        cache_type)       CACHE_TYPE="$PARAM"
                          ;;
        make_osp)         MAKE_OSP="$PARAM"
                          ;;
        *)
            usage
            break ;;
        esac
        shift
    done
    if [ "$SYSTEM_SIZE" = "standard" ]; then
        BUILD_TARGET=${BUILD_TARGET:-"test/xts/hats:xts_hats"}
        PRODUCT_NAME=${PRODUCT_NAME:-"Hi3516DV300"}
    else
        BUILD_TARGET=${BUILD_TARGET:-"hats hats_ivi hats_intellitv hats_wearable"}
        PRODUCT_NAME=${PRODUCT_NAME:-"arm64"}
    fi
}

do_make() {
    cd $BASE_HOME
    HATS_ROOT="$BASE_HOME/test/xts/hats"
    OSP_ARG=""
    if [ $MAKE_OSP == true ]; then
        OSP_ARG="--gn-args make_osp=true"
    fi

    ${BASE_HOME}/prebuilts/python/linux-x86/current/bin/python3 -B ${HATS_ROOT}/check_hvigor.py
    if [ "$?" != 0 ]; then
        exit 1
    fi

    rm -rf "$BASE_HOME/test/xts/autogen_apiobjs"
    export XTS_SUITENAME=hats
    if [ "$SYSTEM_SIZE" = "standard" ]; then
        MUSL_ARGS=""
        if [ "$PRODUCT_NAME" = "m40" ]; then
            if [ "$USE_MUSL" = "false" ]; then
                MUSL_ARGS="--gn-args use_musl=false --gn-args use_custom_libcxx=true --gn-args use_custom_clang=true"
            fi
        fi
        CACHE_ARG=""
        if [ "$CACHE_TYPE" == "xcache" ]; then
            CACHE_ARG="--ccache false --xcache true"
        fi
        ./build.sh --product-name $PRODUCT_NAME --gn-args build_xts=true --build-target $BUILD_TARGET --build-target "deploy_testtools" --gn-args is_standard_system=true $MUSL_ARGS --target-cpu $TARGET_ARCH --get-warning-list=false --stat-ccache=true --compute-overlap-rate=false --deps-guard=false --generate-ninja-trace=false $CACHE_ARG $OSP_ARG --gn-args skip_generate_module_list_file=true
    else
        if [ "$BUILD_TARGET" = "hats hats_ivi hats_intellitv hats_wearable" ]; then
            ./build.sh --product-name $PRODUCT_NAME --gn-args build_xts=true --build-target "hats" --build-target "hats_ivi" --build-target "hats_intellitv" --build-target "hats_wearable" --build-target "deploy_testtools"
        else
            ./build.sh --product-name $PRODUCT_NAME --gn-args build_xts=true --build-target $BUILD_TARGET --build-target "deploy_testtools"
        fi
    fi
    ret=$?

    rm -rf "$BASE_HOME/test/xts/autogen_apiobjs"
    if [ "$ret" != 0 ]; then
        echo "build error"
        exit 1
    fi
}
parse_cmdline $@
do_make
exit 0
