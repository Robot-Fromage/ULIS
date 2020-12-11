ULIS_KERNEL_BEGIN

void
kernel
Clear( global uint8* iDst )
{
    iDst[ get_global_id( 0 ) ] = 0;
}

ULIS_KERNEL_END

