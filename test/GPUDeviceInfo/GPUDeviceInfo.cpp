// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         GPUDeviceInfo.cpp
* @author       Clement Berthaud
* @brief        GPUDeviceInfo application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
//#include <CL/cl.hpp>

int main( int argc, char *argv[] ) {
    // OpenCL
    //get all platforms (drivers)

    /*
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get( &all_platforms );
    if( all_platforms.size() == 0 ){
        std::cout<< " No platforms found. Check OpenCL installation!" << std::endl;
        return  1;
    }

    for( int i = 0; i < all_platforms.size(); ++i ) {
        cl::Platform platform = all_platforms[i];
        std::cout << "====================================================" << std::endl;
        std::cout << "Platform Profile      : "<< platform.getInfo< CL_PLATFORM_PROFILE     >() << std::endl;
        std::cout << "Platform Version      : "<< platform.getInfo< CL_PLATFORM_VERSION     >() << std::endl;
        std::cout << "Platform Name         : "<< platform.getInfo< CL_PLATFORM_NAME        >() << std::endl;
        std::cout << "Platform Vendor       : "<< platform.getInfo< CL_PLATFORM_VENDOR      >() << std::endl;
        std::cout << "Platform Extensions   : "<< platform.getInfo< CL_PLATFORM_EXTENSIONS  >() << std::endl;

        std::vector<cl::Device> all_devices;
        platform.getDevices( CL_DEVICE_TYPE_ALL, &all_devices );

        for( int j = 0; j < all_devices.size(); ++j ) {
            cl::Device device = all_devices[j];
            std::cout<< "   ----------------------------------------- " << std::endl;
            std::cout<< "   Num device                              : " << j << std::endl;
            std::cout<< "   CL_DEVICE_VENDOR_ID                     : " << device.getInfo< CL_DEVICE_VENDOR_ID                      >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_COMPUTE_UNITS             : " << device.getInfo< CL_DEVICE_MAX_COMPUTE_UNITS              >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      : " << device.getInfo< CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS       >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_WORK_GROUP_SIZE           : " << device.getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE            >() << std::endl;
            auto max_work_item_sizes = device.getInfo< CL_DEVICE_MAX_WORK_ITEM_SIZES >();
            for( int k = 0; k < max_work_item_sizes.size(); ++k ) {
                std::cout<< "       CL_DEVICE_MAX_WORK_ITEM_SIZES: Num: " << k << " " << max_work_item_sizes[0] << std::endl;
            }
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR    >() << std::endl;
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT   >() << std::endl;
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT     >() << std::endl;
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG    >() << std::endl;
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT   >() << std::endl;
            std::cout<< "   CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE : " << device.getInfo< CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE  >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_CLOCK_FREQUENCY           : " << device.getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY            >() << std::endl;
            std::cout<< "   CL_DEVICE_ADDRESS_BITS                  : " << device.getInfo< CL_DEVICE_ADDRESS_BITS                   >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_READ_IMAGE_ARGS           : " << device.getInfo< CL_DEVICE_MAX_READ_IMAGE_ARGS            >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_WRITE_IMAGE_ARGS          : " << device.getInfo< CL_DEVICE_MAX_WRITE_IMAGE_ARGS           >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_MEM_ALLOC_SIZE            : " << device.getInfo< CL_DEVICE_MAX_MEM_ALLOC_SIZE             >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE2D_MAX_WIDTH             : " << device.getInfo< CL_DEVICE_IMAGE2D_MAX_WIDTH              >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE2D_MAX_HEIGHT            : " << device.getInfo< CL_DEVICE_IMAGE2D_MAX_HEIGHT             >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE3D_MAX_WIDTH             : " << device.getInfo< CL_DEVICE_IMAGE3D_MAX_WIDTH              >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE3D_MAX_HEIGHT            : " << device.getInfo< CL_DEVICE_IMAGE3D_MAX_HEIGHT             >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE3D_MAX_DEPTH             : " << device.getInfo< CL_DEVICE_IMAGE3D_MAX_DEPTH              >() << std::endl;
            std::cout<< "   CL_DEVICE_IMAGE_SUPPORT                 : " << device.getInfo< CL_DEVICE_IMAGE_SUPPORT                  >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_PARAMETER_SIZE            : " << device.getInfo< CL_DEVICE_MAX_PARAMETER_SIZE             >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_SAMPLERS                  : " << device.getInfo< CL_DEVICE_MAX_SAMPLERS                   >() << std::endl;
            std::cout<< "   CL_DEVICE_MEM_BASE_ADDR_ALIGN           : " << device.getInfo< CL_DEVICE_MEM_BASE_ADDR_ALIGN            >() << std::endl;
            std::cout<< "   CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      : " << device.getInfo< CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE       >() << std::endl;
            std::cout<< "   CL_DEVICE_SINGLE_FP_CONFIG              : " << device.getInfo< CL_DEVICE_SINGLE_FP_CONFIG               >() << std::endl;
            std::cout<< "   CL_DEVICE_GLOBAL_MEM_CACHE_TYPE         : " << device.getInfo< CL_DEVICE_GLOBAL_MEM_CACHE_TYPE          >() << std::endl;
            std::cout<< "   CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     : " << device.getInfo< CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE      >() << std::endl;
            std::cout<< "   CL_DEVICE_GLOBAL_MEM_CACHE_SIZE         : " << device.getInfo< CL_DEVICE_GLOBAL_MEM_CACHE_SIZE          >() << std::endl;
            std::cout<< "   CL_DEVICE_GLOBAL_MEM_SIZE               : " << device.getInfo< CL_DEVICE_GLOBAL_MEM_SIZE                >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      : " << device.getInfo< CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE       >() << std::endl;
            std::cout<< "   CL_DEVICE_MAX_CONSTANT_ARGS             : " << device.getInfo< CL_DEVICE_MAX_CONSTANT_ARGS              >() << std::endl;
            std::cout<< "   CL_DEVICE_LOCAL_MEM_TYPE                : " << device.getInfo< CL_DEVICE_LOCAL_MEM_TYPE                 >() << std::endl;
            std::cout<< "   CL_DEVICE_LOCAL_MEM_SIZE                : " << device.getInfo< CL_DEVICE_LOCAL_MEM_SIZE                 >() << std::endl;
            std::cout<< "   CL_DEVICE_ERROR_CORRECTION_SUPPORT      : " << device.getInfo< CL_DEVICE_ERROR_CORRECTION_SUPPORT       >() << std::endl;
            std::cout<< "   CL_DEVICE_PROFILING_TIMER_RESOLUTION    : " << device.getInfo< CL_DEVICE_PROFILING_TIMER_RESOLUTION     >() << std::endl;
            std::cout<< "   CL_DEVICE_ENDIAN_LITTLE                 : " << device.getInfo< CL_DEVICE_ENDIAN_LITTLE                  >() << std::endl;
            std::cout<< "   CL_DEVICE_AVAILABLE                     : " << device.getInfo< CL_DEVICE_AVAILABLE                      >() << std::endl;
            std::cout<< "   CL_DEVICE_COMPILER_AVAILABLE            : " << device.getInfo< CL_DEVICE_COMPILER_AVAILABLE             >() << std::endl;
            std::cout<< "   CL_DEVICE_EXECUTION_CAPABILITIES        : " << device.getInfo< CL_DEVICE_EXECUTION_CAPABILITIES         >() << std::endl;
            std::cout<< "   CL_DEVICE_QUEUE_PROPERTIES              : " << device.getInfo< CL_DEVICE_QUEUE_PROPERTIES               >() << std::endl;
            std::cout<< "   CL_DEVICE_QUEUE_ON_HOST_PROPERTIES      : " << device.getInfo< CL_DEVICE_QUEUE_ON_HOST_PROPERTIES       >() << std::endl;
            std::cout<< "   CL_DEVICE_NAME                          : " << device.getInfo< CL_DEVICE_NAME                           >() << std::endl;
            std::cout<< "   CL_DEVICE_VENDOR                        : " << device.getInfo< CL_DEVICE_VENDOR                         >() << std::endl;
            std::cout<< "   CL_DRIVER_VERSION                       : " << device.getInfo< CL_DRIVER_VERSION                        >() << std::endl;
            std::cout<< "   CL_DEVICE_PROFILE                       : " << device.getInfo< CL_DEVICE_PROFILE                        >() << std::endl;
            std::cout<< "   CL_DEVICE_VERSION                       : " << device.getInfo< CL_DEVICE_VERSION                        >() << std::endl;
            std::cout<< "   CL_DEVICE_EXTENSIONS                    : " << device.getInfo< CL_DEVICE_EXTENSIONS                     >() << std::endl;
            std::cout<< "   CL_DEVICE_PLATFORM                      : " << device.getInfo< CL_DEVICE_PLATFORM                       >() << std::endl;
            std::cout<< "   CL_DEVICE_DOUBLE_FP_CONFIG              : " << device.getInfo< CL_DEVICE_DOUBLE_FP_CONFIG               >() << std::endl;
        }
    }
    */
    return  0;
}

